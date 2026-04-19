#include "Rte_Control_Node.h"
#include "Uart.h"

extern void Swc_SafetyLogic_Task_10ms(void);

#define SWC_FD_VALID_THRESHOLD_SCALED   300u
#define SWC_DFINAL_SIGN_DEADBAND_CM     2.0f
#define SWC_DFINAL_FILTER_ALPHA         0.7f
#define SWC_AREL_FILTER_ALPHA           0.7f
#define SWC_DOPPLER_TO_VREL_GAIN        0.0062f
#define SWC_TASK_PERIOD_SEC             0.01f

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

    static float32 prev_vrel = 0.0f;
    static float32 prev_arel = 0.0f;

    float32 alpha_used;
    float32 dfinal_now;
    float32 delta_d;
    float32 fd_hz;
    float32 vrel_mag;
    float32 vrel_now;
    float32 arel_raw;

    uint8 lidar_valid;
    uint8 ultra_valid;

    (void)sim_speed;
    (void)status;

    (void)Rte_Read_RpMsg100_FrontObstacle(&front_ultra, &fd_scaled, &lidar);
    (void)Rte_Read_RpMsg102_SysStat(&alpha, &sim_speed, &status);

    lidar_valid = Swc_IsValidDistance(lidar);
    ultra_valid = Swc_IsValidDistance(front_ultra);

    alpha_used = alpha;
    if (alpha_used < 0.0f) {
        alpha_used = 0.0f;
    } else if (alpha_used > 1.0f) {
        alpha_used = 1.0f;
    }

    /* hợp nhất khoảng cách trước xe */
    if ((lidar_valid != 0u) && (ultra_valid != 0u)) {
        dfinal_now = (alpha_used * (float32)lidar) +
                     ((1.0f - alpha_used) * (float32)front_ultra);
        Swc_Buffer_Dfinal = (uint16)dfinal_now;
    }
    else if (lidar_valid != 0u) {
        dfinal_now = (float32)lidar;
        Swc_Buffer_Dfinal = lidar;
    }
    else if (ultra_valid != 0u) {
        dfinal_now = (float32)front_ultra;
        Swc_Buffer_Dfinal = front_ultra;
    }
    else {
        dfinal_now = 0.0f;
        Swc_Buffer_Dfinal = 0xFFFFu;
    }

    /* xác định dấu vận tốc tương đối từ xu hướng D_final */
    if (Swc_Buffer_Dfinal != 0xFFFFu) {
        if (first_valid_dfinal != 0u) {
            dfinal_filt = dfinal_now;
            prev_dfinal_filt = dfinal_now;
            first_valid_dfinal = FALSE;
            vrel_sign = 0;
        } else {
            dfinal_filt = (SWC_DFINAL_FILTER_ALPHA * dfinal_filt) +
                          ((1.0f - SWC_DFINAL_FILTER_ALPHA) * dfinal_now);

            delta_d = dfinal_filt - prev_dfinal_filt;

            if (delta_d <= -SWC_DFINAL_SIGN_DEADBAND_CM) {
                vrel_sign = 1;   /* vật lại gần */
            }
            else if (delta_d >= SWC_DFINAL_SIGN_DEADBAND_CM) {
                vrel_sign = -1;  /* vật ra xa */
            }
            else {
                /* nằm trong vùng chết thì giữ dấu cũ */
            }

            prev_dfinal_filt = dfinal_filt;
        }
    } else {
        first_valid_dfinal = TRUE;
        dfinal_filt = 0.0f;
        prev_dfinal_filt = 0.0f;
        vrel_sign = 0;
    }

    /* tính Vrel từ Fd */
    if ((fd_scaled >= SWC_FD_VALID_THRESHOLD_SCALED) &&
        (Swc_Buffer_Dfinal != 0xFFFFu) &&
        (vrel_sign != 0)) {

        fd_hz = (float32)fd_scaled / 100.0f;
        vrel_mag = fd_hz * SWC_DOPPLER_TO_VREL_GAIN;
        vrel_now = (float32)vrel_sign * vrel_mag;

        arel_raw = (vrel_now - prev_vrel) / SWC_TASK_PERIOD_SEC;
        Swc_Buffer_Arel = (SWC_AREL_FILTER_ALPHA * prev_arel) +
                          ((1.0f - SWC_AREL_FILTER_ALPHA) * arel_raw);
        Swc_Buffer_Vrel = vrel_now;

        prev_vrel = Swc_Buffer_Vrel;
        prev_arel = Swc_Buffer_Arel;
    } else {
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
