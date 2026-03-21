#include "Mcu.h"

/* =========================================================================
   ĐỊNH NGHĨA ĐỊA CHỈ THANH GHI BẰNG CON TRỎ (RAW ADDRESSES)
   ========================================================================= */
/* Thanh ghi Watchdog Timer */
#define WDOG_CS      (*((volatile uint32*)0x40052000u))
#define WDOG_CNT     (*((volatile uint32*)0x40052004u))
#define WDOG_TOVAL   (*((volatile uint32*)0x40052008u))

/* Các thanh ghi điều khiển Dao động nội (FIRC) */
#define SCG_FIRCCSR  (*((volatile uint32*)0x40064300u))
#define SCG_RCCR     (*((volatile uint32*)0x40064014u))

/* =========================================================================
   THỰC THI CÁC HÀM
   ========================================================================= */

void Mcu_Init(void) {
    /* Tắt Watchdog */
    WDOG_CNT   = 0xD928C520u; 
    WDOG_TOVAL = 0x0000FFFFu; 
    WDOG_CS    = 0x00002100u; 
}

void Mcu_InitClock(void) {
    /* 1. Bật dao động nội FIRC (Fast Internal RC - 48MHz)
     * Set bit 0 (FIRCEN) = 1
     */
    SCG_FIRCCSR |= (1 << 0);

    /* 2. Chờ cho đến khi cờ FIRCVLD (Bit 24) lên 1 (FIRC đã ổn định) */
    while (!(SCG_FIRCCSR & (1 << 24))) {
        /* Chờ trong nội bộ chip, chắc chắn 100% sẽ qua được vòng lặp này */
    }
}

Mcu_PllStatusType Mcu_GetPllStatus(void) {
    return MCU_PLL_LOCKED;
}

void Mcu_DistributePllClock(void) {
    /* 3. Phân phối FIRC làm nguồn xung nhịp chính cho toàn chip
     * Xóa nguồn cũ (Bit 24-27)
     */
    SCG_RCCR &= ~(0x0F000000u);
    
    /* Ghi giá trị 3 (0011 nhị phân) vào Bit 24-27 để chọn FIRC */
    SCG_RCCR |= (3 << 24);
}