#ifndef UART_H_
#define UART_H_

#include "Std_Types.h"

/* khởi tạo LPUART0 ở chế độ nhận dữ liệu */
void Uart_Init(void);

/* xóa cờ lỗi overrun và các cờ lỗi liên quan của LPUART0 */
void Uart_ClearOverrun_LPUART0(void);

/* đọc 1 byte từ LPUART0, trả về 1 nếu có dữ liệu */
uint8 Uart_ReadChar_LPUART0(uint8 *data);

#endif /* UART_H_ */