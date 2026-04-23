#include "IoHwAb_Sensors.h"
#include "Adc.h"
#include "Dio.h"
#include "Icu.h"

#define TRIG_FRONT 112u /* PTD16 */
#define TRIG_LEFT  109u /* PTD13 */
#define TRIG_RIGHT 110u /* PTD14 */

/* led trên kit, thường active-low */
#define LED_RED_PTD15       111u
#define LED_GREEN_PTD16     112u
#define LED_YELLOW_PTD14    110u

#define ULTRA_INVALID_CM        0xFFFFu
#define ULTRA_MIN_VALID_CM      2u
#define ULTRA_MAX_VALID_CM      350u

/*
 * Front chạy task nhanh hơn, nên lọc chặt hơn.
 * Task 30ms nhưng chỉ trigger thật mỗi 60ms.
 */
#define FRONT_READ_DIVIDER_MAX      2u
#define FRONT_JUMP_REJECT_CM        50u
#define FRONT_SPIKE_CONFIRM_COUNT   2u
#define FRONT_MAX_MISS_COUNT        3u

/*
 * Side đọc luân phiên trái/phải nên mỗi con chậm hơn,
 * vẫn lọc nhẹ để tránh timeout/spike.
 */
#define SIDE_JUMP_REJECT_CM         60u
#define SIDE_SPIKE_CONFIRM_COUNT    2u
#define SIDE_MAX_MISS_COUNT         3u

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
 * Lọc siêu âm:
 * - giá trị hợp lệ và gần last_valid: nhận luôn
 * - timeout/miss vài lần: giữ last_valid
 * - nhảy quá xa: coi là spike, chỉ nhận nếu lặp lại đủ số lần
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

    /*
     * Nhảy xa bất thường.
     * Ví dụ đang 183cm mà nhảy 70cm đúng 1-2 frame thì giữ 183cm.
     */
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

    /*
     * TRIG idle low.
     * Nếu chân này trùng LED active-low trên kit thì LED có thể sáng,
     * đây là xung đột phần cứng chứ không phải lỗi code.
     */
    Dio_WriteChannel(trig_channel, STD_LOW);
    delay_3us();

    Dio_WriteChannel(trig_channel, STD_HIGH);
    delay_10us();
    Dio_WriteChannel(trig_channel, STD_LOW);

    echo_us = Icu_MeasurePulseWidth_us(icu_channel);

    if (echo_us == 0xFFFFu) {
        return ULTRA_INVALID_CM;
    }

    dist_cm = (uint16)(echo_us / 58u);

    if (IoHwAb_Ultra_IsValid(dist_cm) == FALSE) {
        return ULTRA_INVALID_CM;
    }

    return dist_cm;
}

static void IoHwAb_TurnOffUnusedBoardLed(void)
{
    /*
     * Chỉ tắt chắc được LED không dùng làm TRIG.
     * PTD14/PTD16 đang làm TRIG_RIGHT/TRIG_FRONT nên không thể vừa idle LOW
     * cho HC-SR04 vừa tắt LED active-low.
     */
    Dio_WriteChannel(LED_RED_PTD15, STD_HIGH);
}

void IoHwAb_Sensors_Init(void)
{
    /* tắt hết chân kích xung siêu âm lúc khởi động */
    Dio_WriteChannel(TRIG_FRONT, STD_LOW);
    Dio_WriteChannel(TRIG_LEFT, STD_LOW);
    Dio_WriteChannel(TRIG_RIGHT, STD_LOW);

    IoHwAb_TurnOffUnusedBoardLed();

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

    /*
     * Task 30ms vẫn gọi đều, nhưng HC-SR04 front chỉ đo thật mỗi 60ms.
     * Các frame còn lại trả last_valid đã lọc.
     */
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

    /*
     * Đọc luân phiên trái/phải để tránh nhiễu sóng.
     * Task 50ms => mỗi con được đo thật khoảng 100ms/lần.
     */
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
    while (Adc_GetGroupStatus(ADC_GROUP_POT) == ADC_BUSY) {
    }
    Adc_ReadGroup(ADC_GROUP_POT, adc_raw);
}