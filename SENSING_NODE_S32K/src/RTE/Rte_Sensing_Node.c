#include "Rte_Sensing_Node.h"
#include "IoHwAb_Sensors.h"
#include "CDD_LiDAR_Radar.h"
#include "Com.h"

/* buffer nội bộ của RTE */
static uint16  Rte_Buffer_FrontUltra  = 0xFFFFu;
static uint16  Rte_Buffer_LeftUltra   = 0xFFFFu;
static uint16  Rte_Buffer_RightUltra  = 0xFFFFu;
static uint16  Rte_Buffer_LidarCm     = 0xFFFFu;
static float32 Rte_Buffer_RadarFd  = 0.0f;
static uint16  Rte_Buffer_RainAdc     = 0u;
static uint16  Rte_Buffer_SimSpeedAdc = 0u;

void Rte_Update_Inputs_Task30ms(void) {
    CDD_HighSpeedData cddData;

    CDD_LiDAR_Radar_GetData(&cddData);

    if (cddData.Lidar_IsValid) {
        Rte_Buffer_LidarCm = cddData.Lidar_Distance_cm;
    }

    if (cddData.Radar_IsValid) {
        Rte_Buffer_RadarFd = cddData.Radar_Fd_Hz;
    }

    IoHwAb_Read_FrontUltra(&Rte_Buffer_FrontUltra);
}

void Rte_Update_Inputs_Task50ms(void) {
    IoHwAb_Read_SideUltras(&Rte_Buffer_LeftUltra, &Rte_Buffer_RightUltra);
}

void Rte_Update_Inputs_Task100ms(void) {
    IoHwAb_Read_Rain(&Rte_Buffer_RainAdc);
    IoHwAb_Read_SimSpeedAdc(&Rte_Buffer_SimSpeedAdc);
}

Std_ReturnType Rte_Read_RpLidar_Distance(uint16* data) {
    *data = Rte_Buffer_LidarCm;
    return E_OK;
}

Std_ReturnType Rte_Read_RpRadar_Fd(float32* data) {
    *data = Rte_Buffer_RadarFd;
    return E_OK;
}

Std_ReturnType Rte_Read_RpFrontUltra_Distance(uint16* data) {
    *data = Rte_Buffer_FrontUltra;
    return E_OK;
}

Std_ReturnType Rte_Read_RpSideUltras_Distance(uint16* left_data, uint16* right_data) {
    *left_data = Rte_Buffer_LeftUltra;
    *right_data = Rte_Buffer_RightUltra;
    return E_OK;
}

Std_ReturnType Rte_Read_RpRainSensor_AdcRaw(uint16* data) {
    *data = Rte_Buffer_RainAdc;
    return E_OK;
}

Std_ReturnType Rte_Read_RpSimSpeedAdcRaw(uint16* data) {
    *data = Rte_Buffer_SimSpeedAdc;
    return E_OK;
}

Std_ReturnType Rte_Write_PpMsg100_FrontObstacle(const Rte_Msg100_FrontObstacle* data) {
    uint8 can_payload[8] = {0u};

    can_payload[0] = (uint8)(data->FrontUltra_Distance_cm & 0xFFu);
    can_payload[1] = (uint8)((data->FrontUltra_Distance_cm >> 8) & 0xFFu);
    can_payload[2] = (uint8)((uint16)data->Cdm324_Fd_scaled & 0xFFu);
    can_payload[3] = (uint8)(((uint16)data->Cdm324_Fd_scaled >> 8) & 0xFFu);
    can_payload[4] = (uint8)(data->Lidar_Distance_cm & 0xFFu);
    can_payload[5] = (uint8)((data->Lidar_Distance_cm >> 8) & 0xFFu);

    return Com_Transmit(0x100u, can_payload, 8u);
}

Std_ReturnType Rte_Write_PpMsg101_BlindSpot(const Rte_Msg101_BlindSpot* data) {
    uint8 can_payload[4] = {0u};

    can_payload[0] = (uint8)(data->LeftUltra_Distance_cm & 0xFFu);
    can_payload[1] = (uint8)((data->LeftUltra_Distance_cm >> 8) & 0xFFu);
    can_payload[2] = (uint8)(data->RightUltra_Distance_cm & 0xFFu);
    can_payload[3] = (uint8)((data->RightUltra_Distance_cm >> 8) & 0xFFu);

    return Com_Transmit(0x101u, can_payload, 4u);
}

Std_ReturnType Rte_Write_PpMsg102_SysStat(const Rte_Msg102_SysStat* data) {
    uint8 can_payload[4] = {0u};

    can_payload[0] = data->Alpha_Scaled;
    can_payload[1] = (uint8)(data->SimSpeed_Scaled & 0xFFu);
    can_payload[2] = (uint8)((data->SimSpeed_Scaled >> 8) & 0xFFu);
    can_payload[3] = data->Node_Status;

    return Com_Transmit(0x102u, can_payload, 4u);
}