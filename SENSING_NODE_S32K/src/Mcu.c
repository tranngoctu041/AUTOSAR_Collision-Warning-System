#include "Mcu.h"
#include "S32K144.h"

#define SCG  ((SCG_Type *)IP_SCG_BASE)
#define WDOG ((WDOG_Type *)IP_WDOG_BASE)

void Mcu_Init(const Mcu_ConfigType* ConfigPtr) {
    if (ConfigPtr == NULL_PTR) return; // Bảo vệ con trỏ NULL

    /* Tắt Watchdog */
    WDOG->CNT   = 0xD928C520u; 
    WDOG->TOVAL = 0x0000FFFFu; 
    WDOG->CS    = 0x00002100u; 
}

void Mcu_InitClock(void) {
    /* 1. Bật dao động nội FIRC (Fast Internal RC - 48MHz)
     * Set bit 0 (FIRCEN) = 1
     */
    SCG->FIRCCSR |= (1 << 0);

    SCG->FIRCDIV |= (1 << 8);

    /* 2. Chờ cho đến khi cờ FIRCVLD (Bit 24) lên 1 (FIRC đã ổn định) */
    while (!(SCG->FIRCCSR & (1 << 24))) {
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
    SCG->RCCR &= ~(0x0F000000u);
    
    /* Ghi giá trị 3 (0011 nhị phân) vào Bit 24-27 để chọn FIRC */
    SCG->RCCR |= (3 << 24);
}
