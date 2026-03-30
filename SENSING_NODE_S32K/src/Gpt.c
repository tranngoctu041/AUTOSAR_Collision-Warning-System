#include "Gpt.h"
#include "S32K144.h"

#define PCC   ((PCC_Type *)IP_PCC_BASE)
#define LPIT0 ((LPIT_Type *)IP_LPIT0_BASE)

void Gpt_Init(void) {
    /* 1. Cấp xung nhịp FIRC (48MHz) cho LPIT */
    PCC->PCCn[PCC_LPIT_INDEX] &= ~(1u << 30); /* Tắt CGC */
    PCC->PCCn[PCC_LPIT_INDEX] &= ~(7u << 24); /* Xóa PCS cũ */
    PCC->PCCn[PCC_LPIT_INDEX] |= (3u << 24);  /* Chọn PCS = 3 (FIRC 48MHz) */
    PCC->PCCn[PCC_LPIT_INDEX] |= (1u << 30);  /* Bật CGC */

    /* 2. Cấu hình module LPIT */
    LPIT0->MCR = 0x00000001u; /* M_CEN = 1: Bật Module Clock cho LPIT */
    
    /* Cấu hình Timer 0: 32-bit Periodic Counter (Mặc định) */
    LPIT0->TMR[0].TCTRL = 0x00000000u; /* Tạm thời tắt Timer 0 để cài đặt sau */
}

void Gpt_Delay_us(uint32 us) {
    /* Với Clock 48MHz, 1 micro-giây tốn 48 chu kỳ xung nhịp */
    uint32 ticks = us * 48;

    /* Nạp giá trị đếm ngược */
    LPIT0->TMR[0].TVAL = ticks;

    /* Bật Timer 0 (T_EN = 1) */
    LPIT0->TMR[0].TCTRL |= (1u << 0);

    /* Chờ cờ TIF (Timer Interrupt Flag) ở bit 0 của thanh ghi MSR bật lên 1 báo hiệu đếm xong */
    while ((LPIT0->MSR & (1u << 0)) == 0) {
        __asm volatile("nop"); /* <--- Thêm dòng này để giải phóng Bus cho Debugger */
    }

    /* Xóa cờ TIF bằng cách ghi 1 vào chính nó */
    LPIT0->MSR |= (1u << 0);

    /* Tắt Timer 0 */
    LPIT0->TMR[0].TCTRL &= ~(1u << 0);
}

void Gpt_Delay_ms(uint32 ms) {
    while (ms--) {
        Gpt_Delay_us(1000);
    }
}
