#include "Rte_Control_Node.h"

extern Std_ReturnType Swc_SensorFusion_GetResults(uint16* DfinalPtr, float32* VrelPtr, float32* ArelPtr);

static uint8 Swc_Buffer_FcwState = 0u;
static uint8 Swc_Buffer_BsdState = 0u;

static uint8 Swc_IsValidDistance(uint16 dist_cm)
{
    if ((dist_cm == 0xFFFFu) || (dist_cm == 0u)) {
        return FALSE;
    }

    return TRUE;
}

void Swc_SafetyLogic_Task_10ms(void)
{
    uint16 dfinal = 0xFFFFu;
    float32 vrel = 0.0f;
    float32 arel = 0.0f;

    uint16 left_ultra = 0xFFFFu;
    uint16 right_ultra = 0xFFFFu;

    float32 distance_m;
    float32 ettc;

    (void)arel;

    (void)Swc_SensorFusion_GetResults(&dfinal, &vrel, &arel);
    (void)Rte_Read_RpMsg101_BlindSpot(&left_ultra, &right_ultra);

    /* FCW:
       0 = bình thường
       1 = cảnh báo mức vừa
       2 = cảnh báo mức cao
    */
    Swc_Buffer_FcwState = 0u;

    if ((Swc_IsValidDistance(dfinal) != 0u) && (vrel > 0.05f)) {
        distance_m = (float32)dfinal / 100.0f;
        ettc = distance_m / vrel;

        if ((dfinal <= 30u) || (ettc <= 1.5f)) {
            Swc_Buffer_FcwState = 2u;
        }
        else if ((dfinal <= 80u) || (ettc <= 3.0f)) {
            Swc_Buffer_FcwState = 1u;
        }
        else {
            Swc_Buffer_FcwState = 0u;
        }
    }

    /* BSD:
       bit0 = bên trái
       bit1 = bên phải
       0 = không có vật
       1 = có vật bên trái
       2 = có vật bên phải
       3 = có vật cả hai bên
    */
    Swc_Buffer_BsdState = 0u;

    if ((Swc_IsValidDistance(left_ultra) != 0u) && (left_ultra <= 40u)) {
        Swc_Buffer_BsdState |= 1u;
    }

    if ((Swc_IsValidDistance(right_ultra) != 0u) && (right_ultra <= 40u)) {
        Swc_Buffer_BsdState |= 2u;
    }
}

Std_ReturnType Swc_SafetyLogic_GetResults(uint8* FcwStatePtr, uint8* BsdStatePtr)
{
    if ((FcwStatePtr == NULL_PTR) || (BsdStatePtr == NULL_PTR)) {
        return E_NOT_OK;
    }

    *FcwStatePtr = Swc_Buffer_FcwState;
    *BsdStatePtr = Swc_Buffer_BsdState;

    return E_OK;
}