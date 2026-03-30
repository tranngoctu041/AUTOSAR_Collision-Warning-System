#include "Icu.h"
#include "S32K144.h"

#define PCC ((PCC_Type *)IP_PCC_BASE)
#define FTM0           ((FTM_Type *)IP_FTM0_BASE)

void Icu_Init(void) {
    /* Cấp xung nhịp FIRC (48MHz) cho FTM0 */
    PCC->PCCn[PCC_FTM0_INDEX] &= ~(1u << 30);
    PCC->PCCn[PCC_FTM0_INDEX] &= ~(7u << 24);
    PCC->PCCn[PCC_FTM0_INDEX] |= (3u << 24);
    PCC->PCCn[PCC_FTM0_INDEX] |= (1u << 30);

    FTM0->MODE |= (1u << 2); /* Xóa Write Protection */

    /* Timer Prescaler: PS = 111 (Chia 128) -> 1 tick = ~2.66us */
    FTM0->SC = 0x00000000u;
    FTM0->SC |= 0x07u;

    FTM0->CONTROLS[0].CnSC = 0x00u;
}

uint16 Icu_MeasurePulseWidth_us(void) {
    uint16 start_time = 0;
    uint16 end_time = 0;
    uint16 pulse_ticks = 0;
    uint32 timeout;

    FTM0->CNT = 0;
    FTM0->SC |= (1u << 3); /* Bật Timer chạy với System Clock */
    FTM0->CONTROLS[0].CnSC &= ~(1u << 7); /* Clear CHF */

    /* BƯỚC 1: BẮT CẠNH LÊN (Rising Edge) */
    FTM0->CONTROLS[0].CnSC = (1u << 2); /* ELSB=0, ELSA=1 */

    timeout = 0x4FFFFF; /* Timeout dài chống treo chip */
    while (!(FTM0->CONTROLS[0].CnSC & (1u << 7))) {
        __asm volatile("nop"); /* Nhường Bus cho S32DS Debugger */
        if (--timeout == 0) return 0xFFFF;
    }

    start_time = FTM0->CONTROLS[0].CnV;
    FTM0->CONTROLS[0].CnSC &= ~(1u << 7);

    /* BƯỚC 2: BẮT CẠNH XUỐNG (Falling Edge) */
    FTM0->CONTROLS[0].CnSC = (1u << 3); /* ELSB=1, ELSA=0 */

    timeout = 0x4FFFFF;
    while (!(FTM0->CONTROLS[0].CnSC & (1u << 7))) {
        __asm volatile("nop");
        if (--timeout == 0) return 0xFFFF;
    }

    end_time = FTM0->CONTROLS[0].CnV;
    FTM0->CONTROLS[0].CnSC &= ~(1u << 7);

    FTM0->SC &= ~(3u << 3); /* Tắt Timer chờ chu kỳ đo kế tiếp */

    /* BƯỚC 3: TÍNH TOÁN ĐỘ RỘNG XUNG */
    if (end_time >= start_time) {
        pulse_ticks = end_time - start_time;
    } else {
        pulse_ticks = (0xFFFF - start_time) + end_time + 1;
    }

    /* Quy đổi Tick ra Micro-giây */
    return (uint16)((float)pulse_ticks * 2.666f);
}
