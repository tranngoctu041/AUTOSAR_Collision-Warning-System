#include "Mcu.h"
#include "S32K144.h"

#define SCG  ((SCG_Type *)IP_SCG_BASE)
#define WDOG ((WDOG_Type *)IP_WDOG_BASE)

void Mcu_Init(const Mcu_ConfigType* ConfigPtr)
{
    /* tắt watchdog để tránh reset khi khởi động */
    WDOG->CNT   = 0xD928C520u;
    WDOG->TOVAL = 0x0000FFFFu;
    WDOG->CS    = 0x00002100u;

    if (ConfigPtr == NULL_PTR) {
        return;
    }
}

void Mcu_InitClock(void)
{
    /* bật SOSC 8 MHz */
    SCG->SOSCCFG = 0x00000024u;
    SCG->SOSCCSR = 0x00000001u;
    while ((SCG->SOSCCSR & (1u << 24)) == 0u) {}
    SCG->SOSCDIV = 0x00000101u;

    /* bật FIRC 48 MHz */
    SCG->FIRCCSR |= (1u << 0);
    SCG->FIRCDIV |= (1u << 8);
    while ((SCG->FIRCCSR & (1u << 24)) == 0u) {}
}

Mcu_PllStatusType Mcu_GetPllStatus(void)
{
    return MCU_PLL_LOCKED;
}

void Mcu_DistributePllClock(void)
{
    /* chọn FIRC làm clock chính */
    SCG->RCCR &= ~(0x0F000000u);
    SCG->RCCR |= (3u << 24);
}