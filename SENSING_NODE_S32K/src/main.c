#include "S32K144.h"
#include "Mcu.h"
#include "Port.h"
#include "Adc.h"
#include "Uart.h" /* <--- Chỉ cần gọi Include này */

int main(void) {
    /* 1. Khởi tạo hệ thống cứng */
    Mcu_ConfigType mcuCfg = {0};
    Mcu_Init(&mcuCfg);
    Mcu_InitClock();
    Mcu_DistributePllClock();
    Port_Init(&Port_Config);
    
    /* 2. Khởi tạo Ngoại vi bằng Driver đã đóng gói */
    Uart_Init(); 

    Uart_SendString("\r\n=================================\r\n");
    Uart_SendString("    MODULE UART DA DUOC DONG GOI \r\n");
    Uart_SendString("=================================\r\n");

    uint8 received_byte;

    while (1) {
        Uart_ClearOverrun_LPUART0();

        /* Sử dụng hàm đọc Non-blocking siêu gọn */
        if (Uart_ReadChar_LPUART0(&received_byte) == 1) {
            Uart_SendHex(received_byte);
        }
    }
    return 0;
}