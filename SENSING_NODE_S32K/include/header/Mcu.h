#ifndef MCU_H
#define MCU_H

#include "Std_Types.h" 

/* Định nghĩa trạng thái của hệ thống Clock (PLL) */
typedef uint8 Mcu_PllStatusType;

#define MCU_PLL_LOCKED           ((Mcu_PllStatusType)0x00U)
#define MCU_PLL_UNLOCKED         ((Mcu_PllStatusType)0x01U)
#define MCU_PLL_STATUS_UNDEFINED ((Mcu_PllStatusType)0x02U)

/* Cấu trúc cấu hình MCU */
typedef struct {
    uint32 DummyClockSetting;
} Mcu_ConfigType;

/* Khởi tạo cấu hình cơ bản của MCU */
void Mcu_Init(const Mcu_ConfigType* ConfigPtr);

/* Khởi tạo nguồn xung nhịp (Clock) từ thạch anh ngoài */
void Mcu_InitClock(void);

/* Kiểm tra xem xung nhịp đã ổn định chưa */
Mcu_PllStatusType Mcu_GetPllStatus(void);

/* Cấp phát xung nhịp đã ổn định cho toàn bộ hệ thống (System Clock) */
void Mcu_DistributePllClock(void);

#endif /* MCU_H */
