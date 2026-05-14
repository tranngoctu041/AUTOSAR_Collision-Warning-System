#include "Uart.h"
#include "stm32f1xx.h"

void Uart_Init(void)
{
    /* bật clock cho USART1 */
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    /* baudrate 115200 với PCLK2 = 72 MHz => BRR = 72000000 / 115200 = 625 => 0x271 */
    USART1->BRR = 0x271u;

    /* bật UART, TX, RX */
    USART1->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

void Uart_SendChar(uint8 data)
{
    /* chờ cờ TXE */
    while ((USART1->SR & USART_SR_TXE) == 0u) {}

    /* gửi dữ liệu */
    USART1->DR = (uint16)data;
}

void Uart_SendString(const char* str)
{
    if (str == NULL_PTR) {
        return;
    }

    while (*str != '\0') {
        Uart_SendChar((uint8)*str);
        str++;
    }
}

void Uart_SendNumber(sint32 num)
{
    char buffer[11];
    uint32 value;
    sint32 i = 0;

    if (num < 0) {
        Uart_SendChar((uint8)'-');
        value = (uint32)(-(num + 1)) + 1u;
    } else {
        value = (uint32)num;
    }

    if (value == 0u) {
        Uart_SendChar((uint8)'0');
        return;
    }

    /* chuyển đổi số nguyên thành chuỗi */
    while (value > 0u) {
        buffer[i] = (char)('0' + (value % 10u));
        value /= 10u;
        i++;
    }

    /* in ngược mảng lại để ra đúng thứ tự số */
    while (i > 0) {
        i--;
        Uart_SendChar((uint8)buffer[i]);
    }
}

void Uart_SendFloat(float32 num, uint8 decimal_places)
{
    uint8 i;
    sint32 int_part;
    float32 frac_part;

    if (num < 0.0f) {
        Uart_SendChar((uint8)'-');
        num = -num;
    }

    /* tách phần nguyên và phần thập phân */
    int_part = (sint32)num;
    frac_part = num - (float32)int_part;

    Uart_SendNumber(int_part);
    Uart_SendChar((uint8)'.');

    /* dịch phần thập phân */
    for (i = 0u; i < decimal_places; i++) {
        frac_part *= 10.0f;
        Uart_SendChar((uint8)('0' + (uint8)frac_part));
        frac_part -= (float32)((uint8)frac_part);
    }
}

void Uart_SendBuffer(const uint8* data, uint16 length)
{
    uint16 i;

    if (data == NULL_PTR) {
        return;
    }

    for (i = 0u; i < length; i++) {
        Uart_SendChar(data[i]);
    }
}