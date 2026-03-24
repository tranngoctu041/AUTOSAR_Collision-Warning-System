#ifndef ADC_H
#define ADC_H

#include "Std_Types.h"

typedef uint8  Adc_GroupType;        /* Kiểu dữ liệu chứa ID của Group ảo */
typedef uint16 Adc_ValueGroupType;   /* Kiểu dữ liệu chứa kết quả (12-bit -> uint16) */
typedef uint8  Adc_StatusType;       /* Trạng thái của khối ADC */

#define ADC_IDLE       ((Adc_StatusType)0x00u)
#define ADC_BUSY       ((Adc_StatusType)0x01u)
#define ADC_COMPLETED  ((Adc_StatusType)0x02u)

/* --- CÁC GROUP ẢO CHO CẢM BIẾN --- */
#define ADC_GROUP_RADAR 0u  /* ID 0 dành cho Radar CDM324 (PTA0) */
#define ADC_GROUP_RAIN  1u  /* ID 1 dành cho Cảm biến mưa (PTA1) */

typedef struct {
    uint8 DummyConfig;
} Adc_ConfigType;

void Adc_Init(const Adc_ConfigType* ConfigPtr);
void Adc_StartGroupConversion(Adc_GroupType Group);
Adc_StatusType Adc_GetGroupStatus(Adc_GroupType Group);
void Adc_ReadGroup(Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr);

#endif /* ADC_H */
