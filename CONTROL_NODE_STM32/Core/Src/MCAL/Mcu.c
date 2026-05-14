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
    /* bật HSE bằng thanh ghi CR */
    RCC->CR |= RCC_CR_HSEON;
    while ((RCC->CR & RCC_CR_HSERDY) == 0u) {}

    /* cấu hình FLASH */
    FLASH->ACR |= FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_2;

    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

    /* PLL = HSE x 9 = 72 MHz */
    RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL);
    RCC->CFGR |= RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL9;

    /* bật PLL */
    RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) == 0u) {}
}

Mcu_PllStatusType Mcu_GetPllStatus(void)
{
    /* kiểm tra trạng thái của PLL */
    if ((RCC->CR & RCC_CR_PLLRDY) != 0u) {
        return MCU_PLL_LOCKED;
    }

    return MCU_PLL_UNLOCKED;
}

void Mcu_DistributePllClock(void)
{
    /* chọn PLL làm clock chính */
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {}

    /* cập nhật tốc độ clock của hệ thống */
    SystemCoreClockUpdate();
}