#include "Gpt.h"
#include "S32K144.h"

#ifndef PCC_LPIT_INDEX
#define PCC_LPIT_INDEX 84u
#endif

#define PCC       ((PCC_Type *)IP_PCC_BASE)
#define LPIT0     ((LPIT_Type *)IP_LPIT0_BASE)
#define NVIC_ISER ((volatile uint32 *)0xE000E100u)

/* các hằng số cấu hình timer */
#define LPIT_TICKS_1MS     48000u /* 48 MHz * 1 ms = 48000 ticks */
#define LPIT_RADAR_TICKS   23438u /* 48 MHz / 2048 Hz ~ 23438 ticks */

void Gpt_Init(void) {
    /* cấp xung clock cho module LPIT0 */
    PCC->PCCn[PCC_LPIT_INDEX] &= ~(1u << 30);
    PCC->PCCn[PCC_LPIT_INDEX] &= ~(7u << 24);
    PCC->PCCn[PCC_LPIT_INDEX] |= (3u << 24);
    PCC->PCCn[PCC_LPIT_INDEX] |= (1u << 30);

    /* bật module LPIT */
    LPIT0->MCR = 0x00000001u;

    /* CH0 dùng cho system tick (1 ms) của OS scheduler */
    LPIT0->TMR[0].TCTRL = 0x00000000u;
    LPIT0->TMR[0].TVAL = LPIT_TICKS_1MS - 1u;

    /*CH1 dùng cho hàm delay (polling) */
    LPIT0->TMR[1].TCTRL = 0x00000000u;
    LPIT0->MSR |= (1u << 1); /* xóa cờ ngắt cũ trong MSR (nếu có) */

    /* CH2 dùng để trigger ngắt lấy mẫu ADC cho radar (~2048 Hz) */
    LPIT0->TMR[2].TCTRL = 0x00000000u;
    LPIT0->MSR |= (1u << 2); /* xóa cờ ngắt cũ trong MSR (nếu có) */
    LPIT0->TMR[2].TVAL = LPIT_RADAR_TICKS - 1u;

    /* cấu hình ngắt và bắt đầu chạy timer */
    
    LPIT0->MIER |= (1u << 0); /* bật ngắt cho CH0 */
    LPIT0->MIER |= (1u << 2); /* bật ngắt cho CH2 */

    /* bật ngắt trên bộ điều khiển ngắt NVIC */
    NVIC_ISER[1] |= (1u << 16);
    NVIC_ISER[1] |= (1u << 18);

    /* bật bit T_EN trong TCTRL để CH0 và CH2 chạy liên tục */
    LPIT0->TMR[0].TCTRL |= (1u << 0);
    LPIT0->TMR[2].TCTRL |= (1u << 0);
}

void Gpt_Delay_us(uint32 us) {
    uint32 ticks;

    if (us == 0u) {
        return;
    }

    /* 1 us tương ứng 48 ticks với clock FIRC 48MHz */
    ticks = us * 48u;

    /* dừng CH1, nạp giá trị TVAL mới và xóa cờ MSR */
    LPIT0->TMR[1].TCTRL &= ~(1u << 0);
    LPIT0->MSR |= (1u << 1);
    LPIT0->TMR[1].TVAL = ticks;
    
    /* chạy CH1 */
    LPIT0->TMR[1].TCTRL |= (1u << 0);

    /* chờ cờ timeout của CH1 bật lên (polling) */
    while ((LPIT0->MSR & (1u << 1)) == 0u) {
        __asm volatile("nop");
    }

    /* xóa cờ timeout và dừng timer */
    LPIT0->MSR |= (1u << 1);
    LPIT0->TMR[1].TCTRL &= ~(1u << 0);
}

void Gpt_Delay_ms(uint32 ms) {
    while (ms--) {
        Gpt_Delay_us(1000u);
    }
}