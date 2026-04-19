#include "Os.h"
#include "S32K144.h"

#define LPIT0 ((LPIT_Type *)IP_LPIT0_BASE)

static volatile uint32 Os_SystemTick = 0u;

extern void Rte_Update_Inputs_Task30ms(void);
extern void Rte_Update_Inputs_Task50ms(void);
extern void Rte_Update_Inputs_Task100ms(void);
extern void Swc_DataAcquisition_Task_30ms(void);
extern void Swc_DataAcquisition_Task_50ms(void);
extern void Swc_DataAcquisition_Task_100ms(void);

void LPIT0_Ch0_IRQHandler(void) {
    Os_SystemTick++;
    LPIT0->MSR |= (1u << 0);
}

void Os_Start(void) {
    Os_SystemTick = 0u;
    LPIT0->TMR[0].TCTRL |= (1u << 0);
}

void Os_Scheduler(void) {
    /* lưu mốc tick gần nhất của từng task */
    static uint32 last_run_30ms = 0u;
    static uint32 last_run_50ms = 10u;
    static uint32 last_run_100ms = 20u;

    uint32 current_tick = Os_SystemTick;

    if ((current_tick - last_run_30ms) >= 30u) {
        last_run_30ms = current_tick;
        Rte_Update_Inputs_Task30ms();
        Swc_DataAcquisition_Task_30ms();
    }

    if ((current_tick - last_run_50ms) >= 50u) {
        last_run_50ms = current_tick;
        Rte_Update_Inputs_Task50ms();
        Swc_DataAcquisition_Task_50ms();
    }

    if ((current_tick - last_run_100ms) >= 100u) {
        last_run_100ms = current_tick;
        Rte_Update_Inputs_Task100ms();
        Swc_DataAcquisition_Task_100ms();
    }
}
