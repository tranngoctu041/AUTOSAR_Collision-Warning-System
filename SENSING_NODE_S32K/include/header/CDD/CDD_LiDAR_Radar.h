#ifndef CDD_LIDAR_RADAR_H
#define CDD_LIDAR_RADAR_H

#include "Std_Types.h"

// struct chứa dữ liệu cảm biến
typedef struct {
    uint16 Lidar_Distance_cm;
    float32 Radar_Fd_Hz;
    boolean Lidar_IsValid;  // cờ báo có data lidar mới
    boolean Radar_IsValid;  // cờ báo có data radar mới
} CDD_HighSpeedData;

// khởi tạo các biến và buffer
void CDD_LiDAR_Radar_Init(void);

// parse uart lidar và chạy fft radar, gọi liên tục trong task
void CDD_LiDAR_Radar_Update(void);

// tầng trên gọi hàm này để lấy data ra và reset lại cờ
void CDD_LiDAR_Radar_GetData(CDD_HighSpeedData* DataPtr);

#endif /* CDD_LIDAR_RADAR_H */