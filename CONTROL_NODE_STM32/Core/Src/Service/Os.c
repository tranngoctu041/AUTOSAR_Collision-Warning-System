#include "Os.h"
#include "stm32f1xx.h"

static volatile uint32 Os_SystemTick = 0u;

extern void Rte_Update_Inputs_Task10ms(void);
extern void Swc_ControlNode_Task_10ms(void);
extern void Swc_Hmi_Task_50ms(void);

void Os_Tick(void)
{
    Os_SystemTick++;
}

void Os_Start(void)
{
    Os_SystemTick = 0u;

    /* dùng systick tạo tick 1 ms */
    SysTick_Config(SystemCoreClock / 1000u);
}

void Os_Scheduler(void)
{
    /* lưu mốc tick gần nhất của từng task */
    static uint32 last_run_10ms = 0u;
    static uint32 last_run_50ms = 0u;

    uint32 current_tick = Os_SystemTick;

    if ((current_tick - last_run_10ms) >= 10u) {
        last_run_10ms = current_tick;
        Rte_Update_Inputs_Task10ms();
        Swc_ControlNode_Task_10ms();
    }

    if ((current_tick - last_run_50ms) >= 50u) {
        last_run_50ms = current_tick;
        Swc_Hmi_Task_50ms();
    }
}