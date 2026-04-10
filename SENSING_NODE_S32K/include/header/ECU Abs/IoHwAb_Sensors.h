#ifndef IOHWAB_SENSORS_H
#define IOHWAB_SENSORS_H

#include "Std_Types.h"

/* khởi tạo trạng thái ban đầu cho các cảm biến */
void IoHwAb_Sensors_Init(void);

/* đọc khoảng cách siêu âm phía trước */
void IoHwAb_Read_FrontUltra(uint16* dist_cm);

/* đọc khoảng cách hai cảm biến siêu âm bên hông */
void IoHwAb_Read_SideUltras(uint16* left_cm, uint16* right_cm);

/* đọc giá trị ADC raw của cảm biến mưa */
void IoHwAb_Read_Rain(uint16* adc_raw);

/* đọc giá trị ADC raw của biến trở mô phỏng tốc độ */
void IoHwAb_Read_SimSpeedAdc(uint16* adc_raw);

#endif /* IOHWAB_SENSORS_H */