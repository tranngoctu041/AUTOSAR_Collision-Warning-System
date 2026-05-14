#include "IoHwAb_PcCom.h"
#include "Uart.h"

#define IOHWAB_PCCOM_HEADER_1 0xAAu
#define IOHWAB_PCCOM_HEADER_2 0x55u

/* tổng frame 26 bytes bao gồm cả checksum */
#define IOHWAB_PCCOM_FRAME_SIZE 26u

/* tính checksum cho frame bằng toán tử XOR*/
static uint8 IoHwAb_PcCom_CalcChecksum(const uint8 *data, uint8 length)
{
    uint8 i;
    uint8 checksum = 0u;

    if (data == NULL_PTR)
    {
        return 0u;
    }

    for (i = 0u; i < length; i++)
    {
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
                                         sint16 a_rel_scaled,
                                         uint8 fcw_state,
                                         uint8 bsd_left_state,
                                         uint8 bsd_right_state,
                                         uint8 status)
{
    uint8 frame[IOHWAB_PCCOM_FRAME_SIZE];
    uint8 alpha_scaled;
    uint16 sim_speed_scaled;
    uint16 v_rel_u16;
    uint16 a_rel_u16;

    /* kiểm tra và giới hạn giá trị alpha để tránh tràn số*/
    if (alpha < 0.0f)
    {
        alpha = 0.0f;
    }
    else if (alpha > 2.55f)
    {
        alpha = 2.55f;
    }

    if (sim_speed < 0.0f)
    {
        sim_speed = 0.0f;
    }
    else if (sim_speed > 655.35f)
    {
        sim_speed = 655.35f;
    }

    /* scale dữ liệu thực sang số nguyên để truyền UART */
    alpha_scaled = (uint8)(alpha * 100.0f);
    sim_speed_scaled = (uint16)(sim_speed * 100.0f);
    v_rel_u16 = (uint16)v_rel_scaled;
    a_rel_u16 = (uint16)a_rel_scaled;

    frame[0] = IOHWAB_PCCOM_HEADER_1;
    frame[1] = IOHWAB_PCCOM_HEADER_2;

    frame[2] = (uint8)(front_ultra & 0xFFu);
    frame[3] = (uint8)((front_ultra >> 8) & 0xFFu);

    frame[4] = (uint8)(fd_scaled & 0xFFu);
    frame[5] = (uint8)((fd_scaled >> 8) & 0xFFu);

    frame[6] = (uint8)(lidar & 0xFFu);
    frame[7] = (uint8)((lidar >> 8) & 0xFFu);

    frame[8] = (uint8)(left_ultra & 0xFFu);
    frame[9] = (uint8)((left_ultra >> 8) & 0xFFu);

    frame[10] = (uint8)(right_ultra & 0xFFu);
    frame[11] = (uint8)((right_ultra >> 8) & 0xFFu);

    frame[12] = alpha_scaled;

    frame[13] = (uint8)(sim_speed_scaled & 0xFFu);
    frame[14] = (uint8)((sim_speed_scaled >> 8) & 0xFFu);

    frame[15] = (uint8)(d_final & 0xFFu);
    frame[16] = (uint8)((d_final >> 8) & 0xFFu);

    frame[17] = (uint8)(v_rel_u16 & 0xFFu);
    frame[18] = (uint8)((v_rel_u16 >> 8) & 0xFFu);

    frame[19] = (uint8)(a_rel_u16 & 0xFFu);
    frame[20] = (uint8)((a_rel_u16 >> 8) & 0xFFu);

    frame[21] = fcw_state;
    frame[22] = bsd_left_state;
    frame[23] = bsd_right_state;
    frame[24] = status;

    /* tính checksum cho frame */
    frame[25] = IoHwAb_PcCom_CalcChecksum(frame, (uint8)(IOHWAB_PCCOM_FRAME_SIZE - 1u));

    /* gửi frame qua UART */
    Uart_SendBuffer(frame, IOHWAB_PCCOM_FRAME_SIZE);

    return E_OK;
}