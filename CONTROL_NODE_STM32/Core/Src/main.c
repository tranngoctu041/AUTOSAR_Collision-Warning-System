#include "Mcu.h"
#include "Port.h"
#include "Can.h"
#include "Os.h"
#include "IoHwAb_PcCom.h"

extern const Port_ConfigType Port_Config;

int main(void)
{
    /*
     * 1. Khởi tạo MCU và clock hệ thống
     */
    Mcu_Init(NULL_PTR);
    Mcu_InitClock();
    Mcu_DistributePllClock();

    /*
     * 2. Cấu hình chân GPIO/AF
     * UART1 TX/RX, CAN1 RX/TX
     */
    Port_Init(&Port_Config);

    /*
     * 3. Khởi tạo truyền thông
     * CAN: nhận dữ liệu từ Sensing Node
     * IoHwAb_PcCom: giao tiếp UART với PC/HMI
     */
    Can_Init();
    IoHwAb_PcCom_Init();

    /*
     * 4. Khởi động OS Scheduler
     * 10 ms: cập nhật RTE + xử lý Control Node
     * 50 ms: gửi dữ liệu lên HMI
     */
    Os_Start();

    /*
     * 5. Vòng lặp chính
     */
    while (1) {
        Os_Scheduler();
    }
}