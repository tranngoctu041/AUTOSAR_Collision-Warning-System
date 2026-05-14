#ifndef MCU_H
#define MCU_H

#include "Std_Types.h"

typedef uint8 Mcu_PllStatusType;

#define MCU_PLL_LOCKED           ((Mcu_PllStatusType)0x00u)
#define MCU_PLL_UNLOCKED         ((Mcu_PllStatusType)0x01u)
#define MCU_PLL_STATUS_UNDEFINED ((Mcu_PllStatusType)0x02u)

typedef struct {
    uint32 Reserved;
} Mcu_ConfigType;

/* khởi tạo cấu hình cơ bản của MCU */
void Mcu_Init(const Mcu_ConfigType* ConfigPtr);

/* khởi tạo các nguồn clock cần dùng */
void Mcu_InitClock(void);

/* trả về trạng thái clock hệ thống */
Mcu_PllStatusType Mcu_GetPllStatus(void);

/* chọn nguồn clock chính cho hệ thống */
void Mcu_DistributePllClock(void);

#endif /* MCU_H */