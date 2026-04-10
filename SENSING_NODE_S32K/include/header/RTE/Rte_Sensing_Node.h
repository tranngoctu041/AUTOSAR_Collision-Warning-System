#ifndef RTE_SENSING_NODE_H
#define RTE_SENSING_NODE_H

#include "Std_Types.h"

typedef struct {
    uint16 FrontUltra_Distance_cm;
    sint16 Cdm324_Speed_scaled;
    uint16 Lidar_Distance_cm;
} Rte_Msg100_FrontObstacle;

typedef struct {
    uint16 LeftUltra_Distance_cm;
    uint16 RightUltra_Distance_cm;
} Rte_Msg101_BlindSpot;

typedef struct {
    uint8  Alpha_Scaled;
    uint16 SimSpeed_Scaled;
    uint8  Node_Status;
} Rte_Msg102_SysStat;

/* cập nhật dữ liệu đầu vào cho RTE theo chu kỳ */
void Rte_Update_Inputs_Task30ms(void);
void Rte_Update_Inputs_Task50ms(void);
void Rte_Update_Inputs_Task100ms(void);

/* API để SWC đọc dữ liệu từ RTE */
Std_ReturnType Rte_Read_RpLidar_Distance(uint16* data);
Std_ReturnType Rte_Read_RpRadar_Speed(float32* data);
Std_ReturnType Rte_Read_RpFrontUltra_Distance(uint16* data);
Std_ReturnType Rte_Read_RpSideUltras_Distance(uint16* left_data, uint16* right_data);
Std_ReturnType Rte_Read_RpRainSensor_AdcRaw(uint16* data);
Std_ReturnType Rte_Read_RpSimSpeedAdcRaw(uint16* data);

/* API để ghi message xuống lớp truyền thông */
Std_ReturnType Rte_Write_PpMsg100_FrontObstacle(const Rte_Msg100_FrontObstacle* data);
Std_ReturnType Rte_Write_PpMsg101_BlindSpot(const Rte_Msg101_BlindSpot* data);
Std_ReturnType Rte_Write_PpMsg102_SysStat(const Rte_Msg102_SysStat* data);

#endif /* RTE_SENSING_NODE_H */