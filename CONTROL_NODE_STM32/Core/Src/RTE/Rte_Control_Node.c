#include "Rte_Control_Node.h"
#include "PduR.h"
#include "Com.h"

#define RTE_MAX_RX_PULL_PER_TASK     8u
#define RTE_MAX_COM_READ_PER_TASK    8u

static uint16 Rte_Buffer_FrontUltra = 0xFFFFu;
static uint16 Rte_Buffer_Fd = 0u;
static uint16 Rte_Buffer_Lidar = 0xFFFFu;
static uint16 Rte_Buffer_LeftUltra = 0xFFFFu;
static uint16 Rte_Buffer_RightUltra = 0xFFFFu;
static float32 Rte_Buffer_Alpha = 0.9f;
static float32 Rte_Buffer_SimSpeed = 0.0f;
static uint8 Rte_Buffer_Status = 0u;

static void Rte_DecodeRxPdu(const Can_PduType* RxPdu)
{
    if (RxPdu == NULL_PTR) {
        return;
    }

    if ((RxPdu->CanId == 0x100u) && (RxPdu->CanDlc >= 6u)) {
        Rte_Buffer_FrontUltra = (uint16)(((uint16)RxPdu->CanData[1] << 8) | RxPdu->CanData[0]);
        Rte_Buffer_Fd = (uint16)(((uint16)RxPdu->CanData[3] << 8) | RxPdu->CanData[2]);
        Rte_Buffer_Lidar = (uint16)(((uint16)RxPdu->CanData[5] << 8) | RxPdu->CanData[4]);
    }
    else if ((RxPdu->CanId == 0x101u) && (RxPdu->CanDlc >= 4u)) {
        Rte_Buffer_LeftUltra = (uint16)(((uint16)RxPdu->CanData[1] << 8) | RxPdu->CanData[0]);
        Rte_Buffer_RightUltra = (uint16)(((uint16)RxPdu->CanData[3] << 8) | RxPdu->CanData[2]);
    }
    else if ((RxPdu->CanId == 0x102u) && (RxPdu->CanDlc >= 4u)) {
        uint8 alpha_scaled;
        uint16 sim_speed_scaled;

        alpha_scaled = RxPdu->CanData[0];
        sim_speed_scaled = (uint16)(((uint16)RxPdu->CanData[2] << 8) | RxPdu->CanData[1]);

        Rte_Buffer_Alpha = (float32)alpha_scaled / 100.0f;
        Rte_Buffer_SimSpeed = (float32)sim_speed_scaled / 100.0f;
        Rte_Buffer_Status = RxPdu->CanData[3];
    }
    else {}
}

void Rte_Update_Inputs_Task10ms(void)
{
    Can_PduType RxPdu;
    uint8 count;

    count = 0u;
    while ((PduR_CanIfRxIndication() == E_OK) && (count < RTE_MAX_RX_PULL_PER_TASK)) {
        count++;
    }

    count = 0u;
    while ((Com_Receive(&RxPdu) == E_OK) && (count < RTE_MAX_COM_READ_PER_TASK)) {
        Rte_DecodeRxPdu(&RxPdu);
        count++;
    }
}

Std_ReturnType Rte_Read_RpMsg100_FrontObstacle(uint16* FrontUltraPtr, uint16* FdPtr, uint16* LidarPtr)
{
    if ((FrontUltraPtr == NULL_PTR) || (FdPtr == NULL_PTR) || (LidarPtr == NULL_PTR)) {
        return E_NOT_OK;
    }

    *FrontUltraPtr = Rte_Buffer_FrontUltra;
    *FdPtr = Rte_Buffer_Fd;
    *LidarPtr = Rte_Buffer_Lidar;

    return E_OK;
}

Std_ReturnType Rte_Read_RpMsg101_BlindSpot(uint16* LeftUltraPtr, uint16* RightUltraPtr)
{
    if ((LeftUltraPtr == NULL_PTR) || (RightUltraPtr == NULL_PTR)) {
        return E_NOT_OK;
    }

    *LeftUltraPtr = Rte_Buffer_LeftUltra;
    *RightUltraPtr = Rte_Buffer_RightUltra;

    return E_OK;
}

Std_ReturnType Rte_Read_RpMsg102_SysStat(float32* AlphaPtr, float32* SimSpeedPtr, uint8* StatusPtr)
{
    if ((AlphaPtr == NULL_PTR) || (SimSpeedPtr == NULL_PTR) || (StatusPtr == NULL_PTR)) {
        return E_NOT_OK;
    }

    *AlphaPtr = Rte_Buffer_Alpha;
    *SimSpeedPtr = Rte_Buffer_SimSpeed;
    *StatusPtr = Rte_Buffer_Status;

    return E_OK;
}
