#include "CDD_LiDAR_Radar.h"
#include "Uart.h"
#include "S32K144.h"
#include "arm_math.h"
#include <math.h>
#include <stdlib.h>

/* cấu hình LiDAR */
#define LIDAR_HEADER     0x59u
#define LIDAR_FRAME_SIZE 9u

/* cấu hình Radar FFT */
#define N_SAMPLES              1024u
#define FFT_STEP               204u
#define THRESHOLD_P2P          3.5f
#define DETECT_MIN_MAG         0.2f
#define DETECT_MIN_SNR         2.0f
#define DETECT_HIT_FRAMES      1u
#define RELEASE_DROP_FRAMES    1u
#define NUM_FRAME_SMOOTHING    2u
#define BG_ALPHA               0.05f

#ifndef PCC_LPIT_INDEX
#define PCC_LPIT_INDEX 84u
#endif

#define LPIT0 ((LPIT_Type *)IP_LPIT0_BASE)
#define ADC1  ((ADC_Type *)IP_ADC1_BASE)
#define NVIC_ISER ((volatile uint32 *)0xE000E100u)

static CDD_HighSpeedData CurrentSensorData;

/* buffer cho LiDAR */
static uint8 lidar_buffer[LIDAR_FRAME_SIZE];
static uint8 lidar_index = 0u;

/* buffer và biến cho Radar */
static volatile uint16 adc_ring[N_SAMPLES];
static volatile uint32 adc_write_idx = 0u;
static volatile uint32 step_counter = 0u;
static volatile uint8  data_ready = 0u;

static float32_t bg_magnitude[N_SAMPLES / 2u] = {0.0f};
static float32_t float_adc[N_SAMPLES];
static float32_t fft_input[N_SAMPLES];
static float32_t fft_output[N_SAMPLES];
static float32_t magnitude[N_SAMPLES / 2u];
static arm_rfft_fast_instance_f32 fft_inst;

static uint8 detect_count = 0u;
static uint8 drop_count = 0u;
static uint8 is_target_valid = 0u;
static float32_t fd_history[NUM_FRAME_SMOOTHING] = {0.0f};
static uint8 fd_history_idx = 0u;

/* hàm tính cửa sổ Hanning */
static float32_t Compute_Hanning(uint32_t i, uint32_t n)
{
    return 0.5f * (1.0f - arm_cos_f32(2.0f * PI * (float32_t)i / (float32_t)(n - 1u)));
}

/* hàm đặt lại trạng thái detect */
static void Reset_Detect_State(void)
{
    uint32 i;

    detect_count = 0u;
    drop_count = 0u;
    is_target_valid = 0u;

    for (i = 0u; i < NUM_FRAME_SMOOTHING; i++) {
        fd_history[i] = 0.0f;
    }

    fd_history_idx = 0u;
}

/* cấu hình LPIT channel 2 để lấy mẫu radar */
static void CDD_RadarTimer_Init(void)
{
    /* Gpt_Init đã bật LPIT và đang dùng channel 0,1
       ở đây chỉ cấu hình thêm channel 2 */

    LPIT0->TMR[2].TCTRL = 0x00000000u;
    LPIT0->MSR |= (1u << 2);

    /* LPIT đang chạy FIRC 48MHz từ Gpt_Init nên để Fs ~ 2048Hz
       thì phải tăng TVAL lên khoảng 48M / 2048 = 23438 */
    LPIT0->TMR[2].TVAL = 23438u;
    LPIT0->MIER |= (1u << 2);

    /* IRQ LPIT0 channel 2 */
    NVIC_ISER[1] |= (1u << 18);

    LPIT0->TMR[2].TCTRL |= (1u << 0);
}

/* ngắt lấy mẫu radar từ ADC1_SE7 = PTE2 */
void LPIT0_Ch2_IRQHandler(void)
{
    uint32 timeout = 1000u;
    LPIT0->MSR = (1u << 2);

    /* đọc trực tiếp ADC1 channel 7 để không tranh với ADC0 của cảm biến mưa */
    ADC1->SC1[0] = 10u;
    while ((ADC1->SC1[0] & (1u << 7)) == 0u) {
        if (--timeout == 0u) {
            return;
        }
    }
    
    adc_ring[adc_write_idx] = (uint16)(ADC1->R[0] & 0x0FFFu);

    adc_write_idx = (adc_write_idx + 1u) % N_SAMPLES;
    step_counter++;

    if (step_counter >= FFT_STEP) {
        step_counter = 0u;
        data_ready = 1u;
    }
}

void CDD_LiDAR_Radar_Init(void)
{
    uint32 i;

    CurrentSensorData.Lidar_Distance_cm = 0u;
    CurrentSensorData.Radar_Fd_Hz = 0.0f;
    CurrentSensorData.Lidar_IsValid = FALSE;
    CurrentSensorData.Radar_IsValid = FALSE;

    lidar_index = 0u;
    adc_write_idx = 0u;
    step_counter = 0u;
    data_ready = 0u;

    detect_count = 0u;
    drop_count = 0u;
    is_target_valid = 0u;
    fd_history_idx = 0u;

    for (i = 0u; i < N_SAMPLES; i++) {
        adc_ring[i] = 0u;
        float_adc[i] = 0.0f;
        fft_input[i] = 0.0f;
        fft_output[i] = 0.0f;
    }

    for (i = 0u; i < (N_SAMPLES / 2u); i++) {
        magnitude[i] = 0.0f;
        bg_magnitude[i] = 0.0f;
    }

    for (i = 0u; i < NUM_FRAME_SMOOTHING; i++) {
        fd_history[i] = 0.0f;
    }

    arm_rfft_fast_init_f32(&fft_inst, N_SAMPLES);
    CDD_RadarTimer_Init();
}

void CDD_LiDAR_Radar_Update(void)
{
    uint8 rx_byte;

    /* =========================================================
     * 1. parse LiDAR
     * ========================================================= */
    Uart_ClearOverrun_LPUART0();

    while (Uart_ReadChar_LPUART0(&rx_byte) == 1u) {
        if (lidar_index == 0u) {
            if (rx_byte == LIDAR_HEADER) {
                lidar_buffer[lidar_index++] = rx_byte;
            }
        }
        else if (lidar_index == 1u) {
            if (rx_byte == LIDAR_HEADER) {
                lidar_buffer[lidar_index++] = rx_byte;
            } else {
                lidar_index = 0u;
                if (rx_byte == LIDAR_HEADER) {
                    lidar_buffer[lidar_index++] = rx_byte;
                }
            }
        }
        else {
            lidar_buffer[lidar_index++] = rx_byte;

            if (lidar_index == LIDAR_FRAME_SIZE) {
                uint16 checksum = 0u;
                uint32 i;

                for (i = 0u; i < 8u; i++) {
                    checksum += lidar_buffer[i];
                }

                if ((checksum & 0xFFu) == lidar_buffer[8]) {
                    CurrentSensorData.Lidar_Distance_cm =
                        (uint16)(lidar_buffer[2] | (lidar_buffer[3] << 8));
                    CurrentSensorData.Lidar_IsValid = TRUE;
                }

                lidar_index = 0u;
            }
        }
    }

    /* =========================================================
     * 2. xử lý radar khi đủ block mới
     * ========================================================= */
    if (data_ready != 0u) {
        uint32 i;
        uint32 snapshot_idx;
        float32_t mean_val = 0.0f;
        float32_t min_adc = 4096.0f;
        float32_t max_adc = 0.0f;
        float32_t p2p = 0.0f;
        float32_t max_mag = 0.0f;
        float32_t snr = 0.0f;
        float32_t noise_sum = 0.0f;
        float32_t noise_floor = 0.0f;
        float32_t fd_smoothed = 0.0f;

        uint32_t search_start = 8u;
        uint32_t search_end   = 60u;

        uint32_t local_max_idx = 0u;
        uint32_t peak_idx = 0u;
        uint32_t noise_count = 0u;
        uint8 is_hit = 0u;

        data_ready = 0u;
        snapshot_idx = adc_write_idx;

        /* copy mẫu từ ring buffer */
        for (i = 0u; i < N_SAMPLES; i++) {
            uint32_t read_idx = (snapshot_idx + i) % N_SAMPLES;
            float32_t val = (float32_t)adc_ring[read_idx];

            float_adc[i] = val;

            if (val < min_adc) {
                min_adc = val;
            }
            if (val > max_adc) {
                max_adc = val;
            }
        }

        p2p = max_adc - min_adc;

        /* bỏ DC + Hanning */
        arm_mean_f32(float_adc, N_SAMPLES, &mean_val);

        for (i = 0u; i < N_SAMPLES; i++) {
            fft_input[i] = (float_adc[i] - mean_val) * Compute_Hanning(i, N_SAMPLES);
        }

        /* FFT */
        arm_rfft_fast_f32(&fft_inst, fft_input, fft_output, 0);

        magnitude[0] = fabsf(fft_output[0]) / 512.0f;
        for (i = 1u; i < (N_SAMPLES / 2u); i++) {
            float32_t real = fft_output[2u * i];
            float32_t imag = fft_output[2u * i + 1u];
            float32_t mag_raw;

            arm_sqrt_f32((real * real) + (imag * imag), &mag_raw);
            magnitude[i] = mag_raw / 512.0f;
        }

        /* lọc nhiễu điện đứng lâu: học nền rồi trừ khỏi phổ hiện tại */
        for (i = search_start; i <= search_end; i++) {
            bg_magnitude[i] =
                (BG_ALPHA * magnitude[i]) +
                ((1.0f - BG_ALPHA) * bg_magnitude[i]);

            magnitude[i] = magnitude[i] - bg_magnitude[i];

            if (magnitude[i] < 0.0f) {
                magnitude[i] = 0.0f;
            }
        }

        /* tìm peak Doppler */
        arm_max_f32(&magnitude[search_start],
                    (search_end - search_start + 1u),
                    &max_mag,
                    &local_max_idx);

        peak_idx = search_start + local_max_idx;

        /* tính SNR */
        for (i = search_start; i <= search_end; i++) {
            if (abs((int)i - (int)peak_idx) > 3) {
                noise_sum += magnitude[i];
                noise_count++;
            }
        }

        noise_floor = (noise_count > 0u) ? (noise_sum / (float32_t)noise_count) : 0.1f;
        if (noise_floor < 0.1f) {
            noise_floor = 0.1f;
        }

        snr = max_mag / noise_floor;

        /* state machine detect */
        if ((p2p >= THRESHOLD_P2P) &&
            (max_mag > DETECT_MIN_MAG) &&
            (snr > DETECT_MIN_SNR)) {
            is_hit = 1u;
        } else {
            is_hit = 0u;
        }

        if (is_hit != 0u) {
            drop_count = 0u;

            if (detect_count < DETECT_HIT_FRAMES) {
                detect_count++;
            }

            if (detect_count >= DETECT_HIT_FRAMES) {
                is_target_valid = 1u;
            }
        } else {
            detect_count = 0u;

            if (drop_count < RELEASE_DROP_FRAMES) {
                drop_count++;
            }

            if (drop_count >= RELEASE_DROP_FRAMES) {
                Reset_Detect_State();
            }
        }

        /* xuất kết quả radar chỉ khi detect pass mới cho Fd khác 0 đi xuống pipeline */
        if (is_target_valid != 0u) {
            float32_t fd_raw = (float32_t)peak_idx * 2.0f;
            float32_t sum_fd = 0.0f;

            fd_history[fd_history_idx] = fd_raw;
            fd_history_idx = (uint8)((fd_history_idx + 1u) % NUM_FRAME_SMOOTHING);

            for (i = 0u; i < NUM_FRAME_SMOOTHING; i++) {
                sum_fd += fd_history[i];
            }

            fd_smoothed = sum_fd / (float32_t)NUM_FRAME_SMOOTHING;

            /* đang gửi Fd (Hz) */
            CurrentSensorData.Radar_Fd_Hz = fd_smoothed;
        } else {
            /* không có mục tiêu hợp lệ thì ép Fd = 0 */
            CurrentSensorData.Radar_Fd_Hz = 0.0f;
        }
        /* có block radar mới được xử lý xong */
        CurrentSensorData.Radar_IsValid = TRUE;
    }
}

void CDD_LiDAR_Radar_GetData(CDD_HighSpeedData* DataPtr)
{
    if (DataPtr != NULL_PTR) {
        *DataPtr = CurrentSensorData;

        CurrentSensorData.Lidar_IsValid = FALSE;
        CurrentSensorData.Radar_IsValid = FALSE;
    }
}
