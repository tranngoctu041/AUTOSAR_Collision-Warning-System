#ifndef UART_H_
#define UART_H_

#include "Std_Types.h"

/* Khởi tạo toàn bộ LPUART (LPUART1 cho PuTTY, LPUART0 cho Cảm biến) */
void Uart_Init(void);

/* =======================================================
 * CÁC HÀM TRUYỀN DỮ LIỆU (Mặc định dùng LPUART1 - PuTTY)
 * ======================================================= */
void Uart_SendChar(char c);
void Uart_SendString(const char* str);
void Uart_SendNumber(uint16 num);
void Uart_SendFloat(float num);
void Uart_SendHex(uint8 byte);

/* =======================================================
 * CÁC HÀM NHẬN DỮ LIỆU (Mặc định dùng LPUART0 - Cảm biến)
 * ======================================================= */

/* Xóa cờ lỗi Overrun (Tràn bộ đệm) cho LPUART0 */
void Uart_ClearOverrun_LPUART0(void);

/* Hàm đọc 1 byte từ LPUART0 (Non-blocking)
 * - Trả về 1: Nếu có dữ liệu bay vào (data được cập nhật)
 * - Trả về 0: Nếu không có tín hiệu 
 */
uint8 Uart_ReadChar_LPUART0(uint8 *data);

#endif /* UART_H_ */