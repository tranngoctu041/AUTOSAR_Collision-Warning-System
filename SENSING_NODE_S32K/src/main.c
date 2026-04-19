#include "S32K144.h"
#include "Port.h"
#include "Adc.h"
#include "Uart.h"
#include "Can.h"
#include "Gpt.h"
#include "Mcu.h"
#include "Icu.h"
#include "IoHwAb_Sensors.h"
#include "CDD_LiDAR_Radar.h"
#include "Os.h"

int main(void) {
    /* khởi tạo các module MCAL */
    Mcu_Init(NULL_PTR); 
    Mcu_InitClock();
    Mcu_DistributePllClock();

    Port_Init(&Port_Config); 
    Adc_Init(NULL_PTR);      
    Uart_Init();            
    Can_Init();           
    Icu_Init();

    /* khởi tạo các module ECU Abstraction và CDD */
    Gpt_Init(); 
    IoHwAb_Sensors_Init();
    CDD_LiDAR_Radar_Init();

    /* khởi tạo hệ điều hành */
    Os_Start(); 

    while (1)
    {
    	CDD_LiDAR_Radar_Update();
    	Os_Scheduler();
    }

    return 0;
}