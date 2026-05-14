#ifndef ADC_H
#define ADC_H

#include "Std_Types.h"

typedef uint8  Adc_GroupType;        /* ID của nhóm ADC */
typedef uint16 Adc_ValueGroupType;   /* giá trị ADC đọc được */
typedef uint8  Adc_StatusType;       /* trạng thái chuyển đổi */

#define ADC_IDLE       ((Adc_StatusType)0x00u)
#define ADC_BUSY       ((Adc_StatusType)0x01u)
#define ADC_COMPLETED  ((Adc_StatusType)0x02u)

#define ADC_GROUP_RADAR 0u  /* radar CDM324 - ADC1 */
#define ADC_GROUP_RAIN  1u  /* cảm biến mưa - ADC0 */
#define ADC_GROUP_POT   2u  /* biến trở vận tốc - ADC0 */

#define ADC_GROUP_COUNT 3u

typedef struct {
    uint8 Reserved;
} Adc_ConfigType;

/* khởi tạo cấu hình chung cho ADC0 và ADC1 */
void Adc_Init(const Adc_ConfigType* ConfigPtr);

/* kích hoạt chuyển đổi cho một nhóm ADC cụ thể */
void Adc_StartGroupConversion(Adc_GroupType Group);

/* kiểm tra xem ADC đã chuyển đổi xong chưa */
Adc_StatusType Adc_GetGroupStatus(Adc_GroupType Group);

/* đọc giá trị kết quả từ thanh ghi data của ADC */
void Adc_ReadGroup(Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr);

#endif /* ADC_H */