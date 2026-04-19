#ifndef RTE_CONTROL_NODE_H
#define RTE_CONTROL_NODE_H

#include "Std_Types.h"

/* cập nhật dữ liệu nhận từ com vào buffer rte */
void Rte_Update_Inputs_Task10ms(void);

/* đọc dữ liệu gói 0x100 từ rte */
Std_ReturnType Rte_Read_RpMsg100_FrontObstacle(uint16* FrontUltraPtr, uint16* FdPtr, uint16* LidarPtr);

/* đọc dữ liệu gói 0x101 từ rte */
Std_ReturnType Rte_Read_RpMsg101_BlindSpot(uint16* LeftUltraPtr, uint16* RightUltraPtr);

/* đọc dữ liệu gói 0x102 từ rte */
Std_ReturnType Rte_Read_RpMsg102_SysStat(float32* AlphaPtr, float32* SimSpeedPtr, uint8* StatusPtr);

#endif /* RTE_CONTROL_NODE_H */