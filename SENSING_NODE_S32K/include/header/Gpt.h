#ifndef GPT_H_
#define GPT_H_

#include "Std_Types.h"

/* Khởi tạo bộ định thời LPIT0 */
void Gpt_Init(void);

/* Hàm tạo trễ chính xác theo micro-giây (Dùng Timer 0) */
void Gpt_Delay_us(uint32 us);

/* Hàm tạo trễ theo mili-giây (Dùng Timer 0) */
void Gpt_Delay_ms(uint32 ms);

#endif /* GPT_H_ */