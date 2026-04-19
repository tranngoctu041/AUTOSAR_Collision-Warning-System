#include "Mcu.h"
#include "stm32f1xx.h"

void Mcu_Init(const Mcu_ConfigType* ConfigPtr)
{
    if (ConfigPtr == NULL_PTR) {
        return;
    }
}

void Mcu_InitClock(void)
{
    /* bật hse 8 mhz */
    RCC->CR |= RCC_CR_HSEON;
    while ((RCC->CR & RCC_CR_HSERDY) == 0u) {}

    /* cấu hình flash cho 72 mhz */
    FLASH->ACR |= FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_2;

    /* ahb = 72 mhz, apb1 = 36 mhz, apb2 = 72 mhz */
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

    /* pll = hse x9 = 72 mhz */
    RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL);
    RCC->CFGR |= RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL9;

    RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) == 0u) {}
}

Mcu_PllStatusType Mcu_GetPllStatus(void)
{
    if ((RCC->CR & RCC_CR_PLLRDY) != 0u) {
        return MCU_PLL_LOCKED;
    }

    return MCU_PLL_UNLOCKED;
}

void Mcu_DistributePllClock(void)
{
    /* chọn pll làm clock chính */
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {}

    SystemCoreClockUpdate();
}