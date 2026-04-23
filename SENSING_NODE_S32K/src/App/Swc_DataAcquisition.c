#include "Rte_Sensing_Node.h"
#include "IoHwAb_Sensors.h"

void Swc_DataAcquisition_Task_30ms(void) {
    Rte_Msg100_FrontObstacle msg100;
    float32 raw_radar_fd = 0.0f;
    uint16 raw_lidar_dist = 0u;
    uint16 raw_front_ultra = 0u;

    Rte_Read_RpLidar_Distance(&raw_lidar_dist);
    Rte_Read_RpRadar_Fd(&raw_radar_fd);
    Rte_Read_RpFrontUltra_Distance(&raw_front_ultra);

    msg100.FrontUltra_Distance_cm = raw_front_ultra;
    msg100.Lidar_Distance_cm = raw_lidar_dist;
    /* Fd Hz -> scale x100 để gửi CAN */
    msg100.Cdm324_Fd_scaled = (uint16)(raw_radar_fd * 100.0f);

    Rte_Write_PpMsg100_FrontObstacle(&msg100);
}

void Swc_DataAcquisition_Task_50ms(void) {
    Rte_Msg101_BlindSpot msg101;
    uint16 raw_left_ultra = 0u;
    uint16 raw_right_ultra = 0u;

    Rte_Read_RpSideUltras_Distance(&raw_left_ultra, &raw_right_ultra);

    msg101.LeftUltra_Distance_cm = raw_left_ultra;
    msg101.RightUltra_Distance_cm = raw_right_ultra;

    Rte_Write_PpMsg101_BlindSpot(&msg101);
}

void Swc_DataAcquisition_Task_100ms(void) {
    Rte_Msg102_SysStat msg102;
    uint16 raw_rain_adc = 0u;
    uint16 raw_sim_speed_adc = 0u;
    float32 current_alpha = 0.0f;
    float32 sim_speed_mps = 0.0f;

    Rte_Read_RpRainSensor_AdcRaw(&raw_rain_adc);
    Rte_Read_RpSimSpeedAdcRaw(&raw_sim_speed_adc);

    /* tính alpha từ rain raw */
    current_alpha = ((float32)raw_rain_adc / 4095.0f) * 0.8f + 0.1f;

    /* map adc biến trở sang tốc độ mô phỏng */
    sim_speed_mps = 0.15f + ((float32)raw_sim_speed_adc / 2780.0f) * 2.6f;

    msg102.Alpha_Scaled = (uint8)(current_alpha * 100.0f);
    msg102.SimSpeed_Scaled = (uint16)(sim_speed_mps * 100.0f);
    msg102.Node_Status = 0x01u;

    Rte_Write_PpMsg102_SysStat(&msg102);
}
