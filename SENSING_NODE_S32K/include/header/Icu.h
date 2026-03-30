#ifndef ICU_H_
#define ICU_H_

#include "Std_Types.h"

/* Khởi tạo FTM0 Kênh 0 (PTD15) ở chế độ Input Capture */
void Icu_Init(void);

/* Bắt đầu quá trình đo độ rộng xung ở chân Echo (Đơn vị: micro-giây) 
 * Trả về: Thời gian của xung High. Nếu lỗi/quá giờ trả về 0xFFFF.
 */
uint16 Icu_MeasurePulseWidth_us(void);

#endif /* ICU_H_ */