#include "IoHwAb_Sensors.h"
#include "Adc.h"
#include "Dio.h"
#include "Icu.h"

#define TRIG_FRONT 112u
#define TRIG_LEFT  109u
#define TRIG_RIGHT 110u

#define ULTRA_INVALID_CM        0xFFFFu
#define ULTRA_MIN_VALID_CM      2u
#define ULTRA_MAX_VALID_CM      350u

/* cảm biến trước chạy task nhanh hơn nên cần chia tần số đọc (chia 2) */
#define FRONT_READ_DIVIDER_MAX      2u
#define FRONT_JUMP_REJECT_CM        50u
#define FRONT_SPIKE_CONFIRM_COUNT   2u
#define FRONT_MAX_MISS_COUNT        3u

/* cảm biến bên đọc luân phiên trái/phải lọc nhẹ để tránh timeout */
#define SIDE_JUMP_REJECT_CM         60u
#define SIDE_SPIKE_CONFIRM_COUNT    2u
#define SIDE_MAX_MISS_COUNT         3u

/* struct chứa các trạng thái để chạy bộ lọc nhiễu */
typedef struct {
    uint16 last_valid;
    uint16 spike_candidate;
    uint8  spike_count;
    uint8  miss_count;
} IoHwAb_UltraFilterType;

static IoHwAb_UltraFilterType IoHwAb_FrontFilter = {
    ULTRA_INVALID_CM,
    ULTRA_INVALID_CM,
    0u,
    0u
};

static IoHwAb_UltraFilterType IoHwAb_LeftFilter = {
    ULTRA_INVALID_CM,
    ULTRA_INVALID_CM,
    0u,
    0u
};

static IoHwAb_UltraFilterType IoHwAb_RightFilter = {
    ULTRA_INVALID_CM,
    ULTRA_INVALID_CM,
    0u,
    0u
};

/* hàm tạo trễ khoảng 10us để kích xung siêu âm */
static void delay_10us(void)
{
    volatile uint32 i;

    for (i = 0u; i < 120u; i++) {
        __asm volatile ("nop");
    }
}

/* tạo trễ 3us để trigger xuống mức thấp hoàn toàn */
static void delay_3us(void)
{
    volatile uint32 i;

    for (i = 0u; i < 36u; i++) {
        __asm volatile ("nop");
    }
}

static uint8 IoHwAb_Ultra_IsValid(uint16 dist_cm)
{
    if ((dist_cm >= ULTRA_MIN_VALID_CM) && (dist_cm <= ULTRA_MAX_VALID_CM)) {
        return TRUE;
    }

    return FALSE;
}

static uint16 IoHwAb_AbsDiff_U16(uint16 a, uint16 b)
{
    if (a >= b) {
        return (uint16)(a - b);
    }

    return (uint16)(b - a);
}

static void IoHwAb_Ultra_ResetFilter(IoHwAb_UltraFilterType* FilterPtr)
{
    if (FilterPtr == NULL_PTR) {
        return;
    }

    FilterPtr->last_valid = ULTRA_INVALID_CM;
    FilterPtr->spike_candidate = ULTRA_INVALID_CM;
    FilterPtr->spike_count = 0u;
    FilterPtr->miss_count = 0u;
}

/*
 * logic lọc nhiễu:
 * - nếu mất tín hiệu trả về giá trị cũ vài lần trước khi báo lỗi
 * - nếu giá trị đo được nhảy vọt => nhiễu
 * - chờ nếu giá trị nhảy vọt lặp lại đủ số lần thì mới cập nhật
 */
static uint16 IoHwAb_Ultra_ApplyFilter(uint16 measured_cm,
                                       IoHwAb_UltraFilterType* FilterPtr,
                                       uint16 jump_reject_cm,
                                       uint8 spike_confirm_count,
                                       uint8 max_miss_count)
{
    uint16 diff;

    if (FilterPtr == NULL_PTR) {
        return ULTRA_INVALID_CM;
    }

    if (IoHwAb_Ultra_IsValid(measured_cm) == FALSE) {
        if ((FilterPtr->last_valid != ULTRA_INVALID_CM) &&
            (FilterPtr->miss_count < max_miss_count)) {

            FilterPtr->miss_count++;
            return FilterPtr->last_valid;
        }

        FilterPtr->last_valid = ULTRA_INVALID_CM;
        FilterPtr->spike_candidate = ULTRA_INVALID_CM;
        FilterPtr->spike_count = 0u;
        return ULTRA_INVALID_CM;
    }

    FilterPtr->miss_count = 0u;

    if (FilterPtr->last_valid == ULTRA_INVALID_CM) {
        FilterPtr->last_valid = measured_cm;
        FilterPtr->spike_candidate = ULTRA_INVALID_CM;
        FilterPtr->spike_count = 0u;
        return FilterPtr->last_valid;
    }

    diff = IoHwAb_AbsDiff_U16(measured_cm, FilterPtr->last_valid);

    if (diff <= jump_reject_cm) {
        FilterPtr->last_valid = measured_cm;
        FilterPtr->spike_candidate = ULTRA_INVALID_CM;
        FilterPtr->spike_count = 0u;
        return FilterPtr->last_valid;
    }

    /* phát hiện số đo bị nhảy quá ngưỡng */
    if (FilterPtr->spike_candidate == ULTRA_INVALID_CM) {
        FilterPtr->spike_candidate = measured_cm;
        FilterPtr->spike_count = 1u;
        return FilterPtr->last_valid;
    }

    if (IoHwAb_AbsDiff_U16(measured_cm, FilterPtr->spike_candidate) <= 10u) {
        if (FilterPtr->spike_count < spike_confirm_count) {
            FilterPtr->spike_count++;
        }
    }
    else {
        FilterPtr->spike_candidate = measured_cm;
        FilterPtr->spike_count = 1u;
    }

    /* xác nhận đổi giá trị nếu duy trì đủ lâu */
    if (FilterPtr->spike_count >= spike_confirm_count) {
        FilterPtr->last_valid = measured_cm;
        FilterPtr->spike_candidate = ULTRA_INVALID_CM;
        FilterPtr->spike_count = 0u;
    }

    return FilterPtr->last_valid;
}

static uint16 IoHwAb_Ultra_TriggerAndRead(uint16 trig_channel, uint8 icu_channel)
{
    uint16 echo_us;
    uint16 dist_cm;

    /* kéo TRIG xuống thấp rồi đẩy lên cao 10us để kích hoạt siêu âm */
    Dio_WriteChannel(trig_channel, STD_LOW);
    delay_3us();

    Dio_WriteChannel(trig_channel, STD_HIGH);
    delay_10us();
    Dio_WriteChannel(trig_channel, STD_LOW);

    echo_us = Icu_MeasurePulseWidth_us(icu_channel);

    if (echo_us == 0xFFFFu) {
        return ULTRA_INVALID_CM;
    }

    /* công thức tính khoảng cách siêu âm */
    dist_cm = (uint16)(echo_us / 58u);

    if (IoHwAb_Ultra_IsValid(dist_cm) == FALSE) {
        return ULTRA_INVALID_CM;
    }

    return dist_cm;
}

void IoHwAb_Sensors_Init(void)
{
    /* tắt hết chân kích xung siêu âm lúc khởi động */
    Dio_WriteChannel(TRIG_FRONT, STD_LOW);
    Dio_WriteChannel(TRIG_LEFT, STD_LOW);
    Dio_WriteChannel(TRIG_RIGHT, STD_LOW);

    IoHwAb_Ultra_ResetFilter(&IoHwAb_FrontFilter);
    IoHwAb_Ultra_ResetFilter(&IoHwAb_LeftFilter);
    IoHwAb_Ultra_ResetFilter(&IoHwAb_RightFilter);
}

void IoHwAb_Read_FrontUltra(uint16* dist_cm)
{
    static uint8 read_divider = 0u;
    uint16 measured_cm;

    if (dist_cm == NULL_PTR) {
        return;
    }

    /* chia tần số thực thi để tránh xung TRIG quá nhanh gây nhiễu */
    if (read_divider < (FRONT_READ_DIVIDER_MAX - 1u)) {
        read_divider++;

        if (IoHwAb_FrontFilter.last_valid != ULTRA_INVALID_CM) {
            *dist_cm = IoHwAb_FrontFilter.last_valid;
        }
        else {
            *dist_cm = ULTRA_INVALID_CM;
        }

        return;
    }

    read_divider = 0u;

    measured_cm = IoHwAb_Ultra_TriggerAndRead(TRIG_FRONT, 0u);

    *dist_cm = IoHwAb_Ultra_ApplyFilter(measured_cm,
                                        &IoHwAb_FrontFilter,
                                        FRONT_JUMP_REJECT_CM,
                                        FRONT_SPIKE_CONFIRM_COUNT,
                                        FRONT_MAX_MISS_COUNT);
}

void IoHwAb_Read_SideUltras(uint16* left_cm, uint16* right_cm)
{
    static uint8 toggle = 0u;
    uint16 measured_cm;

    if ((left_cm == NULL_PTR) || (right_cm == NULL_PTR)) {
        return;
    }

    /* đọc luân phiên trái/phải để tránh nhiễu sóng siêu âm chéo nhau */
    if (toggle == 0u) {
        measured_cm = IoHwAb_Ultra_TriggerAndRead(TRIG_LEFT, 3u);

        (void)IoHwAb_Ultra_ApplyFilter(measured_cm,
                                       &IoHwAb_LeftFilter,
                                       SIDE_JUMP_REJECT_CM,
                                       SIDE_SPIKE_CONFIRM_COUNT,
                                       SIDE_MAX_MISS_COUNT);

        toggle = 1u;
    }
    else {
        measured_cm = IoHwAb_Ultra_TriggerAndRead(TRIG_RIGHT, 2u);

        (void)IoHwAb_Ultra_ApplyFilter(measured_cm,
                                       &IoHwAb_RightFilter,
                                       SIDE_JUMP_REJECT_CM,
                                       SIDE_SPIKE_CONFIRM_COUNT,
                                       SIDE_MAX_MISS_COUNT);

        toggle = 0u;
    }

    *left_cm = IoHwAb_LeftFilter.last_valid;
    *right_cm = IoHwAb_RightFilter.last_valid;
}

void IoHwAb_Read_Rain(uint16* adc_raw)
{
    if (adc_raw == NULL_PTR) {
        return;
    }

    Adc_StartGroupConversion(ADC_GROUP_RAIN);

    /* chờ cờ COCO đến khi phần cứng ADC0 xử lý xong */
    while (Adc_GetGroupStatus(ADC_GROUP_RAIN) == ADC_BUSY) {
    }
    Adc_ReadGroup(ADC_GROUP_RAIN, adc_raw);
}

void IoHwAb_Read_SimSpeedAdc(uint16* adc_raw)
{
    if (adc_raw == NULL_PTR) {
        return;
    }

    Adc_StartGroupConversion(ADC_GROUP_POT);

    /* chờ cờ COCO đến khi phần cứng ADC0 xử lý xong */
    while (Adc_GetGroupStatus(ADC_GROUP_POT) == ADC_BUSY) {
    }
    Adc_ReadGroup(ADC_GROUP_POT, adc_raw);
}