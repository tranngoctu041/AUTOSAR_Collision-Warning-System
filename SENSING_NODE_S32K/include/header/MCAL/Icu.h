#ifndef ICU_H
#define ICU_H

#include "Std_Types.h"

/* khởi tạo FTM0 để đo độ rộng xung input capture */
void Icu_Init(void);

/* đo độ rộng xung echo theo đơn vị micro giây */
uint16 Icu_MeasurePulseWidth_us(uint8 channel);

#endif /* ICU_H */