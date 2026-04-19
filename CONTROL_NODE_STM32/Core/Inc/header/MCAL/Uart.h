#ifndef UART_H
#define UART_H

#include "Std_Types.h"

/* khởi tạo uart1 */
void Uart_Init(void);

/* gửi 1 ký tự */
void Uart_SendChar(uint8 data);

/* gửi 1 chuỗi */
void Uart_SendString(const char* str);

/* gửi 1 số nguyên */
void Uart_SendNumber(sint32 num);

/* gửi 1 số thực để debug */
void Uart_SendFloat(float32 num, uint8 decimal_places);

/* gửi 1 buffer */
void Uart_SendBuffer(const uint8* data, uint16 length);

#endif /* UART_H */