#ifndef CDD_LIDAR_RADAR_H
#define CDD_LIDAR_RADAR_H

#include "Std_Types.h"

typedef struct {
    uint16 Lidar_Distance_cm;
    float32 Radar_Fd_Hz;
    boolean Lidar_IsValid;
    boolean Radar_IsValid;
} CDD_HighSpeedData;

/* khởi tạo trạng thái ban đầu của CDD */
void CDD_LiDAR_Radar_Init(void);

/* cập nhật dữ liệu LiDAR và radar */
void CDD_LiDAR_Radar_Update(void);

/* trả dữ liệu hiện tại cho tầng trên */
void CDD_LiDAR_Radar_GetData(CDD_HighSpeedData* DataPtr);

#endif /* CDD_LIDAR_RADAR_H */