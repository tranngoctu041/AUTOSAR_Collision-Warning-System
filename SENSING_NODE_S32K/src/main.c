#include "S32K144.h"
#include "Mcu.h"
#include "Port.h"
#include "Uart.h"
#include "Gpt.h"
#include "Icu.h"

#define PTD  ((GPIO_Type *)IP_PTD_BASE)
#define PCC  ((PCC_Type *)IP_PCC_BASE)

int main(void) {
    /* 1. KHỞI TẠO XUNG NHỊP CƠ BẢN */
    Mcu_ConfigType mcuCfg = {0};
    Mcu_Init(&mcuCfg);
    Mcu_InitClock();
    Mcu_DistributePllClock();

    /* Bật Clock PORTC, PORTD trước khi gọi Port_Init */
    PCC->PCCn[75] |= (1u << 30);
    PCC->PCCn[76] |= (1u << 30);

    /* 2. KHỞI TẠO TẤT CẢ MCAL DRIVERS */
    Port_Init(&Port_Config);
    Uart_Init();
    Gpt_Init();
    Icu_Init();

    Uart_SendString("\r\n=================================\r\n");
    Uart_SendString("   ADAS SENSING NODE S32K144     \r\n");
    Uart_SendString("=================================\r\n");

    /* Cấu hình chiều Output cho chân Trigger (PTD16) */
    PTD->PDDR |= (1u << 16);
    PTD->PCOR = (1u << 16);  /* Kéo xuống mức THẤP để đảm bảo an toàn */

    while (1) {
        /* BƯỚC 1: Bắn xung 10us siêu mượt nhờ phần cứng GPT */
        PTD->PSOR = (1u << 16);
        Gpt_Delay_us(10);
        PTD->PCOR = (1u << 16);

        /* BƯỚC 2: Đo xung Echo dội về bằng phần cứng ICU */
        uint16 echo_time_us = Icu_MeasurePulseWidth_us();

        /* BƯỚC 3: Quy đổi và In ra màn hình */
        if (echo_time_us != 0xFFFF) {
            float distance_cm = ((float)echo_time_us * 0.0343f) / 2.0f;
            Uart_SendString("Khoang cach: ");
            Uart_SendFloat(distance_cm);
            Uart_SendString(" cm\r\n");
        } else {
            Uart_SendString("CBAO: Khong co vat can / Vuot tam do!\r\n");
        }

        /* BƯỚC 4: Nhịp thở cảm biến 100ms */
        Gpt_Delay_ms(100);
    }

    return 0;
}
