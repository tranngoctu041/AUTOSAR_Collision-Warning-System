#include "Rte_Control_Node.h"

extern void Swc_SafetyLogic_Task_10ms(void);

#define SWC_DFINAL_SIGN_DEADBAND_CM      2.0f
#define SWC_DFINAL_FILTER_ALPHA          0.7f
#define SWC_AREL_FILTER_ALPHA            0.7f
#define SWC_DOPPLER_TO_VREL_GAIN         0.0062f
#define SWC_TASK_PERIOD_SEC              0.01f

/* giữ dấu vận tốc tối đa 50 tick = 500ms nếu D_final không đổi rõ */
#define SWC_VREL_SIGN_HOLD_MAX_TICKS     50u

#define SWC_AREL_LIMIT_MPS2              5.0f

static uint16  Swc_Buffer_Dfinal = 0xFFFFu;
static float32 Swc_Buffer_Vrel = 0.0f;
static float32 Swc_Buffer_Arel = 0.0f;

static uint8 Swc_IsValidDistance(uint16 dist_cm)
{
    if ((dist_cm == 0xFFFFu) || (dist_cm == 0u)) {
        return FALSE;
    }

    return TRUE;
}

static float32 Swc_ClampFloat(float32 value, float32 min_value, float32 max_value)
{
    if (value < min_value) {
        return min_value;
    }

    if (value > max_value) {
        return max_value;
    }

    return value;
}

void Swc_SensorFusion_Task_10ms(void)
{
    uint16 front_ultra = 0xFFFFu;
    uint16 fd_scaled = 0u;
    uint16 lidar = 0xFFFFu;

    float32 alpha = 0.0f;
    float32 sim_speed = 0.0f;
    uint8 status = 0u;

    static uint8 first_valid_dfinal = TRUE;
    static float32 dfinal_filt = 0.0f;
    static float32 prev_dfinal_filt = 0.0f;

    static sint8 vrel_sign = 0;
    static uint16 sign_hold_ticks = 0u;

    static float32 prev_vrel = 0.0f;
    static float32 prev_arel = 0.0f;

    float32 alpha_used;
    float32 dfinal_now = 0.0f;
    float32 delta_d = 0.0f;

    float32 fd_hz;
    float32 vrel_mag;
    float32 vrel_now;
    float32 arel_raw;

    uint8 lidar_valid;
    uint8 ultra_valid;
    uint8 dfinal_valid = FALSE;

    (void)sim_speed;
    (void)status;

    (void)Rte_Read_RpMsg100_FrontObstacle(&front_ultra, &fd_scaled, &lidar);
    (void)Rte_Read_RpMsg102_SysStat(&alpha, &sim_speed, &status);

    lidar_valid = Swc_IsValidDistance(lidar);
    ultra_valid = Swc_IsValidDistance(front_ultra);

    alpha_used = Swc_ClampFloat(alpha, 0.0f, 1.0f);

    /*
     * hợp nhất khoảng cách trước xe
     * alpha càng lớn thì tin LiDAR nhiều hơn
     */
    if ((lidar_valid == TRUE) && (ultra_valid == TRUE)) {
        dfinal_now = (alpha_used * (float32)lidar) +
                     ((1.0f - alpha_used) * (float32)front_ultra);
        dfinal_valid = TRUE;
    }
    else if (lidar_valid == TRUE) {
        dfinal_now = (float32)lidar;
        dfinal_valid = TRUE;
    }
    else if (ultra_valid == TRUE) {
        dfinal_now = (float32)front_ultra;
        dfinal_valid = TRUE;
    }
    else {
        dfinal_now = 0.0f;
        dfinal_valid = FALSE;
    }

    /*
     * lọc D_final và xác định dấu Vrel từ xu hướng D_final
     *
     * quy ước:
     * D_final giảm  -> vật lại gần -> vrel_sign = +1
     * D_final tăng  -> vật ra xa   -> vrel_sign = -1
     */
    if (dfinal_valid == TRUE) {
        if (first_valid_dfinal == TRUE) {
            dfinal_filt = dfinal_now;
            prev_dfinal_filt = dfinal_now;
            first_valid_dfinal = FALSE;
            vrel_sign = 0;
            sign_hold_ticks = 0u;
        }
        else {
            dfinal_filt = (SWC_DFINAL_FILTER_ALPHA * dfinal_filt) +
                          ((1.0f - SWC_DFINAL_FILTER_ALPHA) * dfinal_now);

            delta_d = dfinal_filt - prev_dfinal_filt;

            if (delta_d <= -SWC_DFINAL_SIGN_DEADBAND_CM) {
                vrel_sign = 1;
                sign_hold_ticks = 0u;
            }
            else if (delta_d >= SWC_DFINAL_SIGN_DEADBAND_CM) {
                vrel_sign = -1;
                sign_hold_ticks = 0u;
            }
            else {
                if (sign_hold_ticks < SWC_VREL_SIGN_HOLD_MAX_TICKS) {
                    sign_hold_ticks++;
                }
                else {
                    vrel_sign = 0;
                }
            }

            prev_dfinal_filt = dfinal_filt;
        }

        Swc_Buffer_Dfinal = (uint16)(dfinal_filt + 0.5f);
    }
    else {
        first_valid_dfinal = TRUE;
        dfinal_filt = 0.0f;
        prev_dfinal_filt = 0.0f;
        vrel_sign = 0;
        sign_hold_ticks = 0u;

        Swc_Buffer_Dfinal = 0xFFFFu;
    }

    /*
     * tính Vrel từ Fd Doppler
     */
    if ((Swc_Buffer_Dfinal != 0xFFFFu) &&
        (vrel_sign != 0)) {

        fd_hz = (float32)fd_scaled / 100.0f;
        vrel_mag = fd_hz * SWC_DOPPLER_TO_VREL_GAIN;
        vrel_now = (float32)vrel_sign * vrel_mag;

        arel_raw = (vrel_now - prev_vrel) / SWC_TASK_PERIOD_SEC;
        arel_raw = Swc_ClampFloat(arel_raw, -SWC_AREL_LIMIT_MPS2, SWC_AREL_LIMIT_MPS2);

        Swc_Buffer_Arel = (SWC_AREL_FILTER_ALPHA * prev_arel) +
                          ((1.0f - SWC_AREL_FILTER_ALPHA) * arel_raw);

        Swc_Buffer_Vrel = vrel_now;

        prev_vrel = Swc_Buffer_Vrel;
        prev_arel = Swc_Buffer_Arel;
    }
    else {
        Swc_Buffer_Vrel = 0.0f;
        Swc_Buffer_Arel = 0.0f;

        prev_vrel = 0.0f;
        prev_arel = 0.0f;
    }
}

Std_ReturnType Swc_SensorFusion_GetResults(uint16* DfinalPtr, float32* VrelPtr, float32* ArelPtr)
{
    if ((DfinalPtr == NULL_PTR) || (VrelPtr == NULL_PTR) || (ArelPtr == NULL_PTR)) {
        return E_NOT_OK;
    }

    *DfinalPtr = Swc_Buffer_Dfinal;
    *VrelPtr = Swc_Buffer_Vrel;
    *ArelPtr = Swc_Buffer_Arel;

    return E_OK;
}

void Swc_ControlNode_Task_10ms(void)
{
    Swc_SensorFusion_Task_10ms();
    Swc_SafetyLogic_Task_10ms();
}
