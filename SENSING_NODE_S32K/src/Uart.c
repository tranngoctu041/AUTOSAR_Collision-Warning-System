#include "Uart.h"
#include "S32K144.h"

#define PCC ((PCC_Type *)IP_PCC_BASE)
#define PCC_LPUART1_INDEX  107
#define LPUART1            ((LPUART_Type *)IP_LPUART1_BASE)

#define PCC_LPUART0_INDEX  106
#define LPUART0            ((LPUART_Type *)IP_LPUART0_BASE)

void Uart_Init(void) {
    /* --- 1. LPUART1 (Dùng để in lên PuTTY qua chân PTC7) --- */
    PCC->PCCn[PCC_LPUART1_INDEX] &= ~(1u << 30); 
    PCC->PCCn[PCC_LPUART1_INDEX] &= ~(7u << 24); 
    PCC->PCCn[PCC_LPUART1_INDEX] |= (3u << 24);  /* Chọn FIRC 48MHz */
    PCC->PCCn[PCC_LPUART1_INDEX] |= (1u << 30);  

    LPUART1->BAUD &= ~(0x1FFFu); 
    LPUART1->BAUD |= 26;         /* Baudrate 115200 */
    LPUART1->CTRL |= (1u << 19); /* Bật Transmitter (TE) */

    /* --- 2. LPUART0 (Dùng để nhận Cảm biến qua chân PTC2) --- */
    PCC->PCCn[PCC_LPUART0_INDEX] &= ~(1u << 30); 
    PCC->PCCn[PCC_LPUART0_INDEX] &= ~(7u << 24); 
    PCC->PCCn[PCC_LPUART0_INDEX] |= (3u << 24);  
    PCC->PCCn[PCC_LPUART0_INDEX] |= (1u << 30);  

    LPUART0->BAUD &= ~(0x1FFFu); 
    LPUART0->BAUD |= 26;         /* Baudrate 115200 */
    LPUART0->CTRL |= (1u << 18); /* Bật Receiver (RE) */
}

void Uart_SendChar(char c) {
    while (!(LPUART1->STAT & (1u << 23))) { }
    LPUART1->DATA = c;
}

void Uart_SendString(const char* str) {
    while (*str != '\0') { 
        Uart_SendChar(*str++); 
    }
}

void Uart_SendNumber(uint16 num) {
    char buffer[10];
    int i = 0;
    if (num == 0) {
        Uart_SendChar('0');
    } else {
        while (num > 0) {
            buffer[i++] = (num % 10) + '0';
            num /= 10;
        }
        for (int j = i - 1; j >= 0; j--) {
            Uart_SendChar(buffer[j]);
        }
    }
}

void Uart_SendFloat(float num) {
    if (num < 0) {
        Uart_SendChar('-');
        num = -num;
    }
    uint16 integer_part = (uint16)num;
    uint16 fractional_part = (uint16)((num - integer_part) * 100);
    
    Uart_SendNumber(integer_part);
    Uart_SendChar('.');
    if (fractional_part < 10) {
        Uart_SendChar('0');
    }
    Uart_SendNumber(fractional_part);
}

void Uart_SendHex(uint8 byte) {
    char hex[] = "0123456789ABCDEF";
    Uart_SendChar(hex[byte >> 4]);   
    Uart_SendChar(hex[byte & 0x0F]); 
    Uart_SendChar(' ');              
}

void Uart_ClearOverrun_LPUART0(void) {
    if (LPUART0->STAT & (1u << 19)) {
        LPUART0->STAT |= (1u << 19); 
    }
}

uint8 Uart_ReadChar_LPUART0(uint8 *data) {
    /* Cờ RDRF (Bit 21) lên 1 báo hiệu có dữ liệu trong thanh ghi DATA */
    if (LPUART0->STAT & (1u << 21)) {
        *data = (uint8)LPUART0->DATA;
        return 1;
    }
    return 0;
}