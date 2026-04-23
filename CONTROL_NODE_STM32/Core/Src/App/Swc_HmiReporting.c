#include "Rte_Control_Node.h"
#include "IoHwAb_PcCom.h"

extern Std_ReturnType Swc_SensorFusion_GetResults(uint16 *DfinalPtr, float32 *VrelPtr, float32 *ArelPtr);
extern Std_ReturnType Swc_SafetyLogic_GetResults(uint8 *FcwStatePtr,
                                                 uint8 *BsdLeftStatePtr,
                                                 uint8 *BsdRightStatePtr);
static sint16 Swc_Hmi_ScaleFloatToS16(float32 value)
{
    float32 scaled;

    scaled = value * 100.0f;

    if (scaled > 32767.0f)
    {
        scaled = 32767.0f;
    }
    else if (scaled < -32768.0f)
    {
        scaled = -32768.0f;
    }
    else
    {
    }

    if (scaled >= 0.0f)
    {
        return (sint16)(scaled + 0.5f);
    }

    return (sint16)(scaled - 0.5f);
}

void Swc_Hmi_Task_50ms(void)
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

    sint16 v_rel_scaled = 0;
    sint16 a_rel_scaled = 0;

    uint8 fcw_state = 0u;
    uint8 bsd_left_state = 0u;
    uint8 bsd_right_state = 0u;

    (void)Rte_Read_RpMsg100_FrontObstacle(&front_ultra, &fd_scaled, &lidar);
    (void)Rte_Read_RpMsg101_BlindSpot(&left_ultra, &right_ultra);
    (void)Rte_Read_RpMsg102_SysStat(&alpha, &sim_speed, &status);

    (void)Swc_SensorFusion_GetResults(&d_final, &v_rel, &a_rel);
    (void)Swc_SafetyLogic_GetResults(&fcw_state,
                                     &bsd_left_state,
                                     &bsd_right_state);

    v_rel_scaled = Swc_Hmi_ScaleFloatToS16(v_rel);
    a_rel_scaled = Swc_Hmi_ScaleFloatToS16(a_rel);

    (void)IoHwAb_PcCom_SendHmiFrame(front_ultra,
                                    fd_scaled,
                                    lidar,
                                    left_ultra,
                                    right_ultra,
                                    alpha,
                                    sim_speed,
                                    d_final,
                                    v_rel_scaled,
                                    a_rel_scaled,
                                    fcw_state,
                                    bsd_left_state,
                                    bsd_right_state,
                                    status);
}