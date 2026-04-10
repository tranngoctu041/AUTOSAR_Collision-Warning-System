#ifndef GPT_H_
#define GPT_H_

#include "Std_Types.h"

/* khởi tạo LPIT0 cho timer hệ thống và delay polling */
void Gpt_Init(void);

/* tạo trễ theo micro giây bằng Timer 1 */
void Gpt_Delay_us(uint32 us);

/* tạo trễ theo mili giây */
void Gpt_Delay_ms(uint32 ms);

#endif /* GPT_H_ */