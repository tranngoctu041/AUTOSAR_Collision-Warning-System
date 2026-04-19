#include "IoHwAb_PcCom.h"
#include "Uart.h"

#define IOHWAB_PCCOM_HEADER_1   0xAAu
#define IOHWAB_PCCOM_HEADER_2   0x55u
#define IOHWAB_PCCOM_FRAME_SIZE 17u

static uint8 IoHwAb_PcCom_CalcChecksum(const uint8* data, uint8 length)
{
    uint8 i;
    uint8 checksum = 0u;

    if (data == NULL_PTR) {
        return 0u;
    }

    for (i = 0u; i < length; i++) {
        checksum ^= data[i];
    }

    return checksum;
}

void IoHwAb_PcCom_Init(void)
{
    Uart_Init();
}

Std_ReturnType IoHwAb_PcCom_SendHmiFrame(uint16 front_ultra,
                                         uint16 fd_scaled,
                                         uint16 lidar,
                                         uint16 left_ultra,
                                         uint16 right_ultra,
                                         float32 alpha,
                                         float32 sim_speed,
                                         uint16 d_final,
                                         sint16 v_rel_scaled,
                                         uint8 fcw_state,
                                         uint8 bsd_state,
                                         uint8 status)
{
    uint8 frame[IOHWAB_PCCOM_FRAME_SIZE];
    uint8 alpha_scaled;
    uint16 sim_speed_scaled;

    if (alpha < 0.0f) {
        alpha = 0.0f;
    } else if (alpha > 2.55f) {
        alpha = 2.55f;
    }

    if (sim_speed < 0.0f) {
        sim_speed = 0.0f;
    } else if (sim_speed > 655.35f) {
        sim_speed = 655.35f;
    }

    alpha_scaled = (uint8)(alpha * 100.0f);
    sim_speed_scaled = (uint16)(sim_speed * 100.0f);

    frame[0]  = IOHWAB_PCCOM_HEADER_1;
    frame[1]  = IOHWAB_PCCOM_HEADER_2;

    frame[2]  = (uint8)(front_ultra & 0xFFu);
    frame[3]  = (uint8)((front_ultra >> 8) & 0xFFu);

    frame[4]  = (uint8)(fd_scaled & 0xFFu);
    frame[5]  = (uint8)((fd_scaled >> 8) & 0xFFu);

    frame[6]  = (uint8)(lidar & 0xFFu);
    frame[7]  = (uint8)((lidar >> 8) & 0xFFu);

    frame[8]  = (uint8)(left_ultra & 0xFFu);
    frame[9]  = (uint8)((left_ultra >> 8) & 0xFFu);

    frame[10] = (uint8)(right_ultra & 0xFFu);
    frame[11] = (uint8)((right_ultra >> 8) & 0xFFu);

    frame[12] = alpha_scaled;

    frame[13] = (uint8)(sim_speed_scaled & 0xFFu);
    frame[14] = (uint8)((sim_speed_scaled >> 8) & 0xFFu);

    frame[15] = status;

    frame[16] = IoHwAb_PcCom_CalcChecksum(frame, (uint8)(IOHWAB_PCCOM_FRAME_SIZE - 1u));

    Uart_SendBuffer(frame, IOHWAB_PCCOM_FRAME_SIZE);

    return E_OK;
}