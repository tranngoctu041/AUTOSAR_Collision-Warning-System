#include "Icu.h"
#include "S32K144.h"

#define PCC  ((PCC_Type *)IP_PCC_BASE)
#define FTM0 ((FTM_Type *)IP_FTM0_BASE)

#define ICU_INVALID_PULSE_US    0xFFFFu
#define ICU_TIMEOUT_COUNT       600000u

#define FTM_CHF_MASK            (1u << 7)
#define FTM_ELSA_MASK           (1u << 2)
#define FTM_ELSB_MASK           (1u << 3)
#define FTM_CLKS_MASK           (3u << 3)
#define FTM_CLKS_SYSTEM         (1u << 3)

/*
 * Hệ số quy đổi tick FTM -> us.
 * Trước để 2.666f nên HC-SR04 đo thấp hơn LiDAR khá nhiều.
 * Giá trị 3.58f tương ứng gần với clock FTM thực tế khoảng 36MHz / prescaler 128.
 */
#define FTM_TICK_TO_US          3.58f

static void Icu_StopTimer(void)
{
    FTM0->SC &= ~FTM_CLKS_MASK;
}

static void Icu_ClearChannelFlag(uint8 channel)
{
    FTM0->CONTROLS[channel].CnSC &= ~FTM_CHF_MASK;
}

void Icu_Init(void)
{
    PCC->PCCn[PCC_FTM0_INDEX] &= ~(0xD3000000u);
    PCC->PCCn[PCC_FTM0_INDEX] |= (3u << 24) | (1u << 30);

    /* bỏ bảo vệ ghi thanh ghi ftm */
    FTM0->MODE |= (1u << 2);

    Icu_StopTimer();

    FTM0->CNTIN = 0u;
    FTM0->MOD = 0xFFFFu;

    /* prescaler = 128, chưa chạy timer */
    FTM0->SC = 0x00000007u;

    FTM0->CONTROLS[0].CnSC = 0x00u;
    FTM0->CONTROLS[2].CnSC = 0x00u;
    FTM0->CONTROLS[3].CnSC = 0x00u;
}

uint16 Icu_MeasurePulseWidth_us(uint8 channel)
{
    uint16 start_time = 0u;
    uint16 end_time = 0u;
    uint32 pulse_ticks;
    float32 pulse_us;
    uint32 timeout;

    if (channel >= 8u) {
        return ICU_INVALID_PULSE_US;
    }

    Icu_StopTimer();

    FTM0->CNT = 0u;
    FTM0->CONTROLS[channel].CnSC = 0x00u;
    Icu_ClearChannelFlag(channel);

    /* chạy timer */
    FTM0->SC &= ~FTM_CLKS_MASK;
    FTM0->SC |= FTM_CLKS_SYSTEM;

    /* chờ cạnh lên */
    FTM0->CONTROLS[channel].CnSC = FTM_ELSA_MASK;
    Icu_ClearChannelFlag(channel);

    timeout = ICU_TIMEOUT_COUNT;
    while ((FTM0->CONTROLS[channel].CnSC & FTM_CHF_MASK) == 0u) {
        if (timeout == 0u) {
            Icu_StopTimer();
            FTM0->CONTROLS[channel].CnSC = 0x00u;
            return ICU_INVALID_PULSE_US;
        }
        timeout--;
    }

    start_time = FTM0->CONTROLS[channel].CnV;
    Icu_ClearChannelFlag(channel);

    /* chờ cạnh xuống */
    FTM0->CONTROLS[channel].CnSC = FTM_ELSB_MASK;
    Icu_ClearChannelFlag(channel);

    timeout = ICU_TIMEOUT_COUNT;
    while ((FTM0->CONTROLS[channel].CnSC & FTM_CHF_MASK) == 0u) {
        if (timeout == 0u) {
            Icu_StopTimer();
            FTM0->CONTROLS[channel].CnSC = 0x00u;
            return ICU_INVALID_PULSE_US;
        }
        timeout--;
    }

    end_time = FTM0->CONTROLS[channel].CnV;
    Icu_ClearChannelFlag(channel);

    Icu_StopTimer();
    FTM0->CONTROLS[channel].CnSC = 0x00u;

    if (end_time >= start_time) {
        pulse_ticks = (uint32)(end_time - start_time);
    }
    else {
        pulse_ticks = (uint32)((0xFFFFu - start_time) + end_time + 1u);
    }

    pulse_us = (float32)pulse_ticks * FTM_TICK_TO_US;

    if (pulse_us >= 65535.0f) {
        return ICU_INVALID_PULSE_US;
    }

    return (uint16)(pulse_us + 0.5f);
}