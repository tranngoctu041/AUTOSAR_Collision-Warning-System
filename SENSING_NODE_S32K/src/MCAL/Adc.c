#include "Adc.h"
#include "S32K144.h"

#define PCC  ((PCC_Type *)IP_PCC_BASE)
#define ADC0 ((ADC_Type *)IP_ADC0_BASE)
#define ADC1 ((ADC_Type *)IP_ADC1_BASE)

static const uint8 Adc_ChannelMap[ADC_GROUP_COUNT] = {
    7u, /* ADC_GROUP_RADAR -> PTE2 / ADC1_SE7 */
    1u, /* ADC_GROUP_RAIN  -> PTA1 / ADC0_SE1 */
    2u  /* ADC_GROUP_POT   -> PTA6 / ADC0_SE2 */
};

void Adc_Init(const Adc_ConfigType* ConfigPtr)
{
    /* ép kiểu để compiler không báo lỗi unused parameter */
    (void)ConfigPtr;

    /* cấp clock cho ADC0 và ADC1 từ FIRC 48 MHz */
    PCC->PCCn[PCC_ADC0_INDEX] &= ~((1u << 30) | (7u << 24));
    PCC->PCCn[PCC_ADC0_INDEX] |= (3u << 24) | (1u << 30);
    PCC->PCCn[PCC_ADC1_INDEX] &= ~((1u << 30) | (7u << 24));
    PCC->PCCn[PCC_ADC1_INDEX] |= (3u << 24) | (1u << 30);

    /* cấu hình ADC0 cho mưa và biến trở */
    ADC0->CFG1 = (1u << 2) | (1u << 5);
    ADC0->CFG2 = (12u << 0);
    ADC0->SC2  = 0x00u;
    ADC0->SC3  = 0x00u;

    /* cấu hình ADC1 cho radar */
    ADC1->CFG1 = (1u << 2) | (1u << 5);
    ADC1->CFG2 = (12u << 0);
    ADC1->SC2  = 0x00u;
    ADC1->SC3  = 0x00u;

    /* disable channel trước khi bắt đầu */
    ADC0->SC1[0] = 0x1Fu;
    ADC1->SC1[0] = 0x1Fu;
}

void Adc_StartGroupConversion(Adc_GroupType Group)
{
    if (Group == ADC_GROUP_RADAR) {
        /* bắt đầu chuyển đổi trên ADC1 */
        ADC1->SC1[0] = Adc_ChannelMap[Group];
    } else {
        /* bắt đầu chuyển đổi trên ADC0 */
        ADC0->SC1[0] = Adc_ChannelMap[Group];
    }
}

Adc_StatusType Adc_GetGroupStatus(Adc_GroupType Group)
{
    /* kiểm tra cờ COCO: 1 = chuyển đổi xong */
    if (Group == ADC_GROUP_RADAR) {
        if ((ADC1->SC1[0] & (1u << 7)) != 0u) {
            return ADC_COMPLETED;
        }
    } else {
        if ((ADC0->SC1[0] & (1u << 7)) != 0u) {
            return ADC_COMPLETED;
        }
    }
    return ADC_BUSY;
}

void Adc_ReadGroup(Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr)
{
    if (DataBufferPtr == NULL_PTR) {
        return;
    }

    /* đọc kết quả 12-bit từ thanh ghi R[0] tương ứng */
    if (Group == ADC_GROUP_RADAR) {
        *DataBufferPtr = (Adc_ValueGroupType)(ADC1->R[0] & 0x0FFFu);
    } else {
        *DataBufferPtr = (Adc_ValueGroupType)(ADC0->R[0] & 0x0FFFu);
    }
}
