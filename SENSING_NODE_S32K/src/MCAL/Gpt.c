#include "Gpt.h"
#include "S32K144.h"

#define PCC       ((PCC_Type *)IP_PCC_BASE)
#define LPIT0     ((LPIT_Type *)IP_LPIT0_BASE)
#define NVIC_ISER ((volatile uint32 *)0xE000E100u)
#define LPIT_TICKS_1MS 48000u /* 48 MHz * 1 ms = 48000 ticks */

void Gpt_Init(void) {
    PCC->PCCn[PCC_LPIT_INDEX] &= ~(1u << 30);
    PCC->PCCn[PCC_LPIT_INDEX] &= ~(7u << 24);
    PCC->PCCn[PCC_LPIT_INDEX] |= (3u << 24);
    PCC->PCCn[PCC_LPIT_INDEX] |= (1u << 30);

    /* bật module LPIT */
    LPIT0->MCR = 0x00000001u;

    /* Timer 0 dùng cho tick 1 ms */
    LPIT0->TMR[0].TVAL = LPIT_TICKS_1MS - 1u;
    LPIT0->TMR[0].TCTRL = 0x00000000u;

    /* Timer 1 dùng cho delay polling */
    LPIT0->TMR[1].TCTRL = 0x00000000u;
    LPIT0->MSR |= (1u << 1);

    /* bật ngắt cho Timer 0 */
    LPIT0->MIER |= (1u << 0);
    NVIC_ISER[1] |= (1u << 16);
}

void Gpt_Delay_us(uint32 us) {
    uint32 ticks;

    if (us == 0u) {
        return;
    }

    ticks = us * 48u;

    LPIT0->TMR[1].TCTRL &= ~(1u << 0);
    LPIT0->MSR |= (1u << 1);
    LPIT0->TMR[1].TVAL = ticks;
    LPIT0->TMR[1].TCTRL |= (1u << 0);

    while ((LPIT0->MSR & (1u << 1)) == 0u) {
        __asm volatile("nop");
    }

    LPIT0->MSR |= (1u << 1);
    LPIT0->TMR[1].TCTRL &= ~(1u << 0);
}

void Gpt_Delay_ms(uint32 ms) {
    while (ms--) {
        Gpt_Delay_us(1000u);
    }
}