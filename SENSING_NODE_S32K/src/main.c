#include "S32K144.h"

/* Các module MCAL */
#include "Port.h"
#include "Adc.h"
#include "Uart.h"
#include "Can.h"
#include "Gpt.h"
#include "Mcu.h"
#include "Icu.h"

/* Các module ECU Abstraction & CDD */
#include "IoHwAb_Sensors.h"
#include "CDD_LiDAR_Radar.h"

#include "Os.h"

int main(void) {
    /* =========================================================
     * BƯỚC 1: KHỞI TẠO TẦNG MCAL (Phần cứng lõi)
     * ========================================================= */
    /* Cấu hình các chân MUX (Siêu âm, Mưa, UART, CAN) */
    Mcu_Init(NULL_PTR); 
    Mcu_InitClock();
    Mcu_DistributePllClock();

    Port_Init(&Port_Config); 
    Adc_Init(NULL_PTR);      /* Cấu hình ADC cho Mưa & Radar */
    Uart_Init();             /* Cấu hình UART đọc LiDAR */
    Can_Init();              /* Kích hoạt FlexCAN0 500kbps */
    Icu_Init();

    /* =========================================================
     * BƯỚC 2: KHỞI TẠO TẦNG ECUAL & CDD (Giao tiếp Cảm biến)
     * ========================================================= */
    IoHwAb_Sensors_Init();
    CDD_LiDAR_Radar_Init();

    /* =========================================================
     * BƯỚC 3: KÍCH HOẠT HỆ ĐIỀU HÀNH VÀ VÒNG LẶP CHÍNH
     * ========================================================= */
    /* Hàm Gpt_Init() sẽ bật ngắt LPIT0_Ch0 mỗi 1ms.
       Từ lúc này, biến đếm thời gian của OS bắt đầu nhảy! */
    Gpt_Init(); 
    Os_Start(); 

    while (1)
    {
    	CDD_LiDAR_Radar_Update();
    	Os_Scheduler();
    }

    return 0;
}
