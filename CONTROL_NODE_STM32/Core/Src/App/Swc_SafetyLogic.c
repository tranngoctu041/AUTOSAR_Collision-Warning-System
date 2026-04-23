#include "Rte_Control_Node.h"

extern Std_ReturnType Swc_SensorFusion_GetResults(uint16 *DfinalPtr, float32 *VrelPtr, float32 *ArelPtr);

#define FCW_DISTANCE_HIGH_CM (35u)
#define FCW_DISTANCE_MEDIUM_CM (100u)

#define FCW_ETTC_HIGH_S (1.5f)
#define FCW_ETTC_MEDIUM_S (3.0f)

#define FCW_MIN_CLOSING_SPEED_MPS (0.05f)
#define FCW_MIN_ACCEL_ABS (0.01f)
#define FCW_ETTC_INVALID (999.0f)

#define FCW_SIM_SPEED_DEADBAND_MPS (0.15f)
#define FCW_SIM_SPEED_LIMIT_MPS (2.8f)
#define FCW_BRAKE_DECEL_MPS2 (2.0f)
#define FCW_BRAKE_RESPONSE_TIME_S (0.20f)
#define FCW_BRAKE_SAFETY_MARGIN_M (0.20f)
#define FCW_BRAKE_WARNING_MARGIN_M (0.60f)

#define BSD_DISTANCE_THRESHOLD_CM (60u)
#define BSD_STABLE_DELTA_CM (5u)

#define BSD_STATIONARY_TIMEOUT_TICKS (500u)

static uint8 Swc_Buffer_FcwState = 0u;
static uint8 Swc_Buffer_BsdLeftState = 0u;
static uint8 Swc_Buffer_BsdRightState = 0u;

static uint16 Swc_BsdLeftLastDist = 0xFFFFu;
static uint16 Swc_BsdRightLastDist = 0xFFFFu;

static uint16 Swc_BsdLeftStableTicks = 0u;
static uint16 Swc_BsdRightStableTicks = 0u;

static uint8 Swc_IsValidDistance(uint16 dist_cm)
{
    if ((dist_cm == 0xFFFFu) || (dist_cm == 0u))
    {
        return FALSE;
    }

    return TRUE;
}

static uint16 Swc_AbsDiff_U16(uint16 a, uint16 b)
{
    if (a >= b)
    {
        return (uint16)(a - b);
    }

    return (uint16)(b - a);
}

static float32 Swc_LimitSimSpeed(float32 Sim_speed)
{
    if (Sim_speed < FCW_SIM_SPEED_DEADBAND_MPS)
    {
        return 0.0f;
    }

    if (Sim_speed > FCW_SIM_SPEED_LIMIT_MPS)
    {
        return FCW_SIM_SPEED_LIMIT_MPS;
    }

    return Sim_speed;
}

static float32 Swc_CalcBrakeDistance(float32 Sim_speed)
{
    float32 brake_dist;

    Sim_speed = Swc_LimitSimSpeed(Sim_speed);

    brake_dist = (Sim_speed * FCW_BRAKE_RESPONSE_TIME_S) +
                 ((Sim_speed * Sim_speed) / (2.0f * FCW_BRAKE_DECEL_MPS2)) +
                 FCW_BRAKE_SAFETY_MARGIN_M;

    return brake_dist;
}

static float32 Swc_SqrtApprox(float32 x)
{
    float32 guess;
    uint8 i;

    if (x <= 0.0f)
    {
        return 0.0f;
    }

    if (x >= 1.0f)
    {
        guess = x;
    }
    else
    {
        guess = 1.0f;
    }

    for (i = 0u; i < 10u; i++)
    {
        guess = 0.5f * (guess + (x / guess));
    }

    return guess;
}

/*
 * tính ETTC với:
 * distance = khoảng cách hiện tại (m)
 * v_closing = vận tốc đóng tương đối, >0 là đang tiến lại gần (m/s)
 * a_closing = gia tốc đóng tương đối, >0 là đóng nhanh hơn (m/s2)
 *
 * phương trình:
 * distance = v*t + 0.5*a*t^2
 */
static float32 Swc_CalcETTC(float32 distance_m, float32 v_closing, float32 a_closing)
{
    float32 discriminant;
    float32 sqrt_discriminant;
    float32 ettc;

    if ((distance_m <= 0.0f) || (v_closing <= FCW_MIN_CLOSING_SPEED_MPS))
    {
        return FCW_ETTC_INVALID;
    }

    if ((a_closing > -FCW_MIN_ACCEL_ABS) && (a_closing < FCW_MIN_ACCEL_ABS))
    {
        return distance_m / v_closing;
    }

    discriminant = (v_closing * v_closing) + (2.0f * a_closing * distance_m);

    /*
     * nếu a_closing âm quá mạnh, vật có thể giảm tốc tương đối
     * và không còn va chạm theo mô hình ETTC
     */
    if (discriminant <= 0.0f)
    {
        return FCW_ETTC_INVALID;
    }

    sqrt_discriminant = Swc_SqrtApprox(discriminant);

    ettc = (-v_closing + sqrt_discriminant) / a_closing;

    if (ettc <= 0.0f)
    {
        return FCW_ETTC_INVALID;
    }

    return ettc;
}

/*
 * BSD:
 * - nếu không có vật hoặc vật ngoài ngưỡng: tắt cảnh báo và reset bộ đếm
 * - nếu có vật trong ngưỡng và khoảng cách còn thay đổi: bật cảnh báo
 * - nếu có vật trong ngưỡng nhưng đứng yên quá lâu: tắt cảnh báo
 */
static uint8 Swc_UpdateBsdSide(uint16 dist_cm, uint16 *LastDistPtr, uint16 *StableTicksPtr)
{
    uint16 diff;

    if ((LastDistPtr == NULL_PTR) || (StableTicksPtr == NULL_PTR))
    {
        return FALSE;
    }

    if ((Swc_IsValidDistance(dist_cm) == FALSE) || (dist_cm > BSD_DISTANCE_THRESHOLD_CM))
    {
        *LastDistPtr = 0xFFFFu;
        *StableTicksPtr = 0u;
        return FALSE;
    }

    if (*LastDistPtr == 0xFFFFu)
    {
        *LastDistPtr = dist_cm;
        *StableTicksPtr = 0u;
        return TRUE;
    }

    diff = Swc_AbsDiff_U16(dist_cm, *LastDistPtr);

    if (diff > BSD_STABLE_DELTA_CM)
    {
        *LastDistPtr = dist_cm;
        *StableTicksPtr = 0u;
        return TRUE;
    }

    if (*StableTicksPtr < BSD_STATIONARY_TIMEOUT_TICKS)
    {
        (*StableTicksPtr)++;
        return TRUE;
    }

    return FALSE;
}

void Swc_SafetyLogic_Task_10ms(void)
{
    uint16 dfinal = 0xFFFFu;
    float32 vrel = 0.0f;
    float32 arel = 0.0f;

    uint16 left_ultra = 0xFFFFu;
    uint16 right_ultra = 0xFFFFu;

    float32 alpha = 0.0f;
    float32 Sim_speed = 0.0f;
    uint8 status = 0u;

    float32 distance_m = 0.0f;
    float32 ettc = FCW_ETTC_INVALID;
    float32 brake_dist = 0.0f;

    uint8 bsd_left_warning;
    uint8 bsd_right_warning;

    (void)Swc_SensorFusion_GetResults(&dfinal, &vrel, &arel);
    (void)Rte_Read_RpMsg101_BlindSpot(&left_ultra, &right_ultra);
    (void)Rte_Read_RpMsg102_SysStat(&alpha, &Sim_speed, &status);

    (void)alpha;
    (void)status;

    /*
     * FCW:
     * 0 = bình thường
     * 1 = cảnh báo mức vừa
     * 2 = cảnh báo mức cao
     *
     * quy ước:
     * vrel > 0: vật đang tiến lại gần
     * arel > 0: tốc độ tiến lại gần đang tăng
     */
    Swc_Buffer_FcwState = 0u;

    if (Swc_IsValidDistance(dfinal) == TRUE)
    {
        distance_m = (float32)dfinal / 100.0f;
        brake_dist = Swc_CalcBrakeDistance(Sim_speed);

        if (vrel > FCW_MIN_CLOSING_SPEED_MPS)
        {
            ettc = Swc_CalcETTC(distance_m, vrel, arel);
        }
        else
        {
            ettc = FCW_ETTC_INVALID;
        }

        if ((dfinal <= FCW_DISTANCE_HIGH_CM) ||
            (ettc <= FCW_ETTC_HIGH_S) ||
            (distance_m <= brake_dist))
        {
            Swc_Buffer_FcwState = 2u;
        }
        else if ((dfinal <= FCW_DISTANCE_MEDIUM_CM) ||
                 (ettc <= FCW_ETTC_MEDIUM_S) ||
                 (distance_m <= (brake_dist + FCW_BRAKE_WARNING_MARGIN_M)))
        {
            Swc_Buffer_FcwState = 1u;
        }
        else
        {
            Swc_Buffer_FcwState = 0u;
        }
    }

    /*
     * BSD:
     * 0 = không cảnh báo
     * 1 = có vật trong vùng cảnh báo
     */
    Swc_Buffer_BsdLeftState = 0u;
    Swc_Buffer_BsdRightState = 0u;

    bsd_left_warning = Swc_UpdateBsdSide(
        left_ultra,
        &Swc_BsdLeftLastDist,
        &Swc_BsdLeftStableTicks);

    bsd_right_warning = Swc_UpdateBsdSide(
        right_ultra,
        &Swc_BsdRightLastDist,
        &Swc_BsdRightStableTicks);

    if (bsd_left_warning == TRUE)
    {
        Swc_Buffer_BsdLeftState = 1u;
    }

    if (bsd_right_warning == TRUE)
    {
        Swc_Buffer_BsdRightState = 1u;
    }
}

Std_ReturnType Swc_SafetyLogic_GetResults(uint8 *FcwStatePtr,
                                          uint8 *BsdLeftStatePtr,
                                          uint8 *BsdRightStatePtr)
{
    if ((FcwStatePtr == NULL_PTR) ||
        (BsdLeftStatePtr == NULL_PTR) ||
        (BsdRightStatePtr == NULL_PTR))
    {
        return E_NOT_OK;
    }

    *FcwStatePtr = Swc_Buffer_FcwState;
    *BsdLeftStatePtr = Swc_Buffer_BsdLeftState;
    *BsdRightStatePtr = Swc_Buffer_BsdRightState;

    return E_OK;
}