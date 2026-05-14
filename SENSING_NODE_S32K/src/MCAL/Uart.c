#include "Uart.h"
#include "S32K144.h"

#define PCC ((PCC_Type *)IP_PCC_BASE)
#define LPUART0 ((LPUART_Type *)IP_LPUART0_BASE)

void Uart_Init(void)
{
    /* cấu hình clock cho LPUART0 */
    PCC->PCCn[PCC_LPUART0_INDEX] &= ~(1u << 30);
    PCC->PCCn[PCC_LPUART0_INDEX] &= ~(7u << 24);
    PCC->PCCn[PCC_LPUART0_INDEX] |= (3u << 24);
    PCC->PCCn[PCC_LPUART0_INDEX] |= (1u << 30);

    /*
     * cấu hình baudrate 115200
     * SBR = 48,000,000 / (115200 * 16) = 26.04 ~ 26
     */
    LPUART0->BAUD &= ~(0x1FFFu);
    LPUART0->BAUD |= 26u;
    LPUART0->CTRL |= (1u << 18); /* bật receiver */
}

void Uart_ClearOverrun_LPUART0(void)
{
    /* xóa các cờ lỗi để tránh kẹt bộ nhận */
    LPUART0->STAT |= (0xFu << 16);
}

uint8 Uart_ReadChar_LPUART0(uint8 *data)
{
    /* kiểm tra cờ RDRF (bit 21), lên 1 có data đến */
    if (LPUART0->STAT & (1u << 21)) {
        *data = (uint8)LPUART0->DATA;
        return 1u;
    }

    return 0u;
}
