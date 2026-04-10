#include "Icu.h"
#include "S32K144.h"

#define PCC  ((PCC_Type *)IP_PCC_BASE)
#define FTM0 ((FTM_Type *)IP_FTM0_BASE)

void Icu_Init(void) {
    PCC->PCCn[PCC_FTM0_INDEX] &= ~(0xD3000000u);
    PCC->PCCn[PCC_FTM0_INDEX] |= (3u << 24) | (1u << 30);
    FTM0->MODE |= (1u << 2);
    FTM0->SC = 0x00000007u; /* prescaler = 128 */

    /* các channel dùng để đo echo */
    FTM0->CONTROLS[0].CnSC = 0x00u;
    FTM0->CONTROLS[2].CnSC = 0x00u;
    FTM0->CONTROLS[3].CnSC = 0x00u;
}

uint16 Icu_MeasurePulseWidth_us(uint8 channel) {
    uint16 start_time = 0u, end_time = 0u;
    uint32 timeout;

    FTM0->CNT = 0u;
    FTM0->SC |= (1u << 3);
    FTM0->CONTROLS[channel].CnSC &= ~(1u << 7);

    /* chờ cạnh lên */
    FTM0->CONTROLS[channel].CnSC = (1u << 2);
    timeout = 0xFFFFu;
    while ((FTM0->CONTROLS[channel].CnSC & (1u << 7)) == 0u) {
        if (--timeout == 0u) return 0xFFFFu;
    }
    start_time = FTM0->CONTROLS[channel].CnV;
    FTM0->CONTROLS[channel].CnSC &= ~(1u << 7);

    /* chờ cạnh xuống */
    FTM0->CONTROLS[channel].CnSC = (1u << 3);
    timeout = 0xFFFFu;
    while ((FTM0->CONTROLS[channel].CnSC & (1u << 7)) == 0u) {
        if (--timeout == 0u) return 0xFFFFu;
    }
    end_time = FTM0->CONTROLS[channel].CnV;
    FTM0->CONTROLS[channel].CnSC &= ~(1u << 7);
    FTM0->SC &= ~(3u << 3);

    /* tính độ rộng xung, có xử lý trường hợp timer tràn */
    return (uint16)((float)((end_time >= start_time) ? 
           (end_time - start_time) : ((0xFFFFu - start_time) + end_time + 1u)) * 2.666f);
}