#include "CDD_LiDAR_Radar.h"
#include "Uart.h"
#include "Adc.h"

#define LIDAR_HEADER     0x59u
#define LIDAR_FRAME_SIZE 9u

#define RADAR_FFT_SIZE   1024u

static CDD_HighSpeedData CurrentSensorData;

static uint8 lidar_buffer[LIDAR_FRAME_SIZE];
static uint8 lidar_index = 0u;
static uint16 radar_raw_buffer[RADAR_FFT_SIZE];
static uint16 radar_sample_count = 0u;

void CDD_LiDAR_Radar_Init(void) {
    CurrentSensorData.Lidar_Distance_cm = 0u;
    CurrentSensorData.Radar_Speed_mps = 0.0f;
    CurrentSensorData.Lidar_IsValid = FALSE;
    CurrentSensorData.Radar_IsValid = FALSE;

    lidar_index = 0u;
    radar_sample_count = 0u;
}

void CDD_LiDAR_Radar_Update(void) {
    uint8 rx_byte;

    /* đọc và kiểm tra frame LiDAR */
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
                int i;

                for (i = 0; i < 8; i++) {
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

    /* lấy mẫu radar từ ADC */
    Adc_StartGroupConversion(ADC_GROUP_RADAR);
    while (Adc_GetGroupStatus(ADC_GROUP_RADAR) == ADC_BUSY) {}
    Adc_ReadGroup(ADC_GROUP_RADAR, &radar_raw_buffer[radar_sample_count]);
    radar_sample_count++;

    if (radar_sample_count >= RADAR_FFT_SIZE) {
        /* chỗ này dành cho xử lý FFT để tính vận tốc Doppler */
        /* CurrentSensorData.Radar_Speed_mps = Calculate_FFT_Doppler(radar_raw_buffer); */

        CurrentSensorData.Radar_IsValid = TRUE;
        radar_sample_count = 0u;
    }
}

void CDD_LiDAR_Radar_GetData(CDD_HighSpeedData* DataPtr) {
    if (DataPtr != NULL_PTR) {
        *DataPtr = CurrentSensorData;

        CurrentSensorData.Lidar_IsValid = FALSE;
        CurrentSensorData.Radar_IsValid = FALSE;
    }
}