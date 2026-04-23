#include "Mcu.h"
#include "Port.h"
#include "Can.h"
#include "Uart.h"
#include "Os.h"
#include "Rte_Control_Node.h"
#include "IoHwAb_PcCom.h"

/*
 * 1 = test bằng PuTTY, in text dễ đọc
 * 0 = chạy thật cho Python HMI, gửi frame nhị phân 25 byte
 */
#define MAIN_PUTTY_DEBUG_MODE   0u

extern const Port_ConfigType Port_Config;

extern void Rte_Update_Inputs_Task10ms(void);
extern void Swc_ControlNode_Task_10ms(void);
extern void Swc_Hmi_Task_50ms(void);

extern Std_ReturnType Swc_SensorFusion_GetResults(uint16* DfinalPtr,
                                                  float32* VrelPtr,
                                                  float32* ArelPtr);

extern Std_ReturnType Swc_SafetyLogic_GetResults(uint8* FcwStatePtr,
                                                 uint8* BsdStatePtr);

static void Main_DelayMs(uint32 ms)
{
    uint32 i;

    while (ms > 0u) {
        /*
         * delay thô cho STM32F103 @72MHz
         * chỉ dùng cho mode debug PuTTY, không dùng làm timing chuẩn
         */
        for (i = 0u; i < 7200u; i++) {
            __asm volatile ("nop");
        }

        ms--;
    }
}

static void Main_SendNewLine(void)
{
    Uart_SendString("\r\n");
}

static void Main_SendDistanceValue(uint16 value)
{
    if (value == 0xFFFFu) {
        Uart_SendString("NO");
    }
    else {
        Uart_SendNumber((sint32)value);
    }
}

static void Main_DebugPrintValues(void)
{
    uint16 front_ultra = 0xFFFFu;
    uint16 fd_scaled = 0u;
    uint16 lidar = 0xFFFFu;

    uint16 left_ultra = 0xFFFFu;
    uint16 right_ultra = 0xFFFFu;

    float32 alpha = 0.0f;
    float32 sim_speed = 0.0f;
    uint8 status = 0u;

    uint16 d_final = 0xFFFFu;
    float32 v_rel = 0.0f;
    float32 a_rel = 0.0f;

    uint8 fcw_state = 0u;
    uint8 bsd_state = 0u;

    float32 fd_hz = 0.0f;

    (void)Rte_Read_RpMsg100_FrontObstacle(&front_ultra, &fd_scaled, &lidar);
    (void)Rte_Read_RpMsg101_BlindSpot(&left_ultra, &right_ultra);
    (void)Rte_Read_RpMsg102_SysStat(&alpha, &sim_speed, &status);

    (void)Swc_SensorFusion_GetResults(&d_final, &v_rel, &a_rel);
    (void)Swc_SafetyLogic_GetResults(&fcw_state, &bsd_state);

    fd_hz = (float32)fd_scaled / 100.0f;

    Uart_SendString("========================================");
    Main_SendNewLine();

    Uart_SendString("[RAW] FrontUltra = ");
    Main_SendDistanceValue(front_ultra);
    Uart_SendString(" cm | Fd = ");
    Uart_SendFloat(fd_hz, 2u);
    Uart_SendString(" Hz | LiDAR = ");
    Main_SendDistanceValue(lidar);
    Uart_SendString(" cm");
    Main_SendNewLine();

    Uart_SendString("[RAW] LeftUltra  = ");
    Main_SendDistanceValue(left_ultra);
    Uart_SendString(" cm | RightUltra = ");
    Main_SendDistanceValue(right_ultra);
    Uart_SendString(" cm");
    Main_SendNewLine();

    Uart_SendString("[SYS] Alpha = ");
    Uart_SendFloat(alpha, 2u);
    Uart_SendString(" | SimSpeed = ");
    Uart_SendFloat(sim_speed, 2u);
    Uart_SendString(" km/h | Status = ");
    Uart_SendNumber((sint32)status);
    Main_SendNewLine();

    Uart_SendString("[FUSION] D_final = ");
    Main_SendDistanceValue(d_final);
    Uart_SendString(" cm | V_rel = ");
    Uart_SendFloat(v_rel, 2u);
    Uart_SendString(" m/s | A_rel = ");
    Uart_SendFloat(a_rel, 2u);
    Uart_SendString(" m/s2");
    Main_SendNewLine();

    Uart_SendString("[SAFETY] FCW = ");
    Uart_SendNumber((sint32)fcw_state);
    Uart_SendString(" | BSD = ");
    Uart_SendNumber((sint32)bsd_state);

    if ((bsd_state & 0x01u) != 0u) {
        Uart_SendString(" | BSD-L");
    }

    if ((bsd_state & 0x02u) != 0u) {
        Uart_SendString(" | BSD-R");
    }

    Main_SendNewLine();
}

int main(void)
{
    uint16 debug_counter_10ms = 0u;

    /*
     * 1. Init MCU clock
     */
    Mcu_Init(NULL_PTR);
    Mcu_InitClock();
    Mcu_DistributePllClock();

    /*
     * 2. Init pins
     * UART1 TX/RX, CAN RX/TX, LED nếu có
     */
    Port_Init(&Port_Config);

    /*
     * 3. Init communication
     * IoHwAb_PcCom_Init() gọi Uart_Init()
     */
    IoHwAb_PcCom_Init();

    /*
     * 4. Init CAN receive
     */
    Can_Init();

    Uart_SendString("\r\n");
    Uart_SendString("CONTROL NODE BOOT OK");
    Main_SendNewLine();

#if (MAIN_PUTTY_DEBUG_MODE == 1u)

    Uart_SendString("MODE: PUTTY ASCII DEBUG");
    Main_SendNewLine();
    Uart_SendString("Testing: CAN -> RTE -> SensorFusion -> SafetyLogic");
    Main_SendNewLine();

    /*
     * Mode debug:
     * Không gọi Swc_Hmi_Task_50ms vì hàm đó gửi frame nhị phân cho Python.
     * Ở đây ta chạy task 10ms bằng delay thô và in text mỗi 200ms.
     */
    while (1) {
        Rte_Update_Inputs_Task10ms();
        Swc_ControlNode_Task_10ms();

        debug_counter_10ms++;
        if (debug_counter_10ms >= 20u) {
            debug_counter_10ms = 0u;
            Main_DebugPrintValues();
        }

        Main_DelayMs(10u);
    }

#else

    Uart_SendString("MODE: PYTHON HMI BINARY FRAME");
    Main_SendNewLine();

    /*
     * Mode thật:
     * OS chạy:
     * 10ms: RTE + Control SWC
     * 50ms: Swc_Hmi_Task_50ms -> IoHwAb_PcCom_SendHmiFrame()
     */
    Os_Start();

    while (1) {
        Os_Scheduler();
    }

#endif
}
