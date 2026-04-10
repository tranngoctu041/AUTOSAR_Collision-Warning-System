#include "Adc.h"
#include "S32K144.h"

#define PCC  ((PCC_Type *)IP_PCC_BASE)
#define ADC0 ((ADC_Type *)IP_ADC0_BASE)

#define PCC_ADC0_INDEX 59u

static const uint8 Adc_ChannelMap[ADC_GROUP_COUNT] = {
    0u, /* ADC_GROUP_RADAR -> PTA0 / ADC0_SE0 */
    1u, /* ADC_GROUP_RAIN  -> PTA1 / ADC0_SE1 */
    2u  /* ADC_GROUP_POT   -> PTA6 / ADC0_SE2 */
};

void Adc_Init(const Adc_ConfigType* ConfigPtr)
{
    (void)ConfigPtr;

    /* cấp clock cho ADC0 từ FIRC 48 MHz */
    PCC->PCCn[PCC_ADC0_INDEX] &= ~(1u << 30);
    PCC->PCCn[PCC_ADC0_INDEX] &= ~(7u << 24);
    PCC->PCCn[PCC_ADC0_INDEX] |= (3u << 24);
    PCC->PCCn[PCC_ADC0_INDEX] |= (1u << 30);

    /* cấu hình ADC0 */
    ADC0->CFG1 = (1u << 2) | (1u << 5);
    ADC0->CFG2 = (12u << 0);
    ADC0->SC2  = 0x00u;
    ADC0->SC3  = 0x00u;

    /* disable channel trước khi bắt đầu */
    ADC0->SC1[0] = 0x1Fu;
}

void Adc_StartGroupConversion(Adc_GroupType Group)
{
    if (Group >= ADC_GROUP_COUNT) {
        return;
    }

    /* ghi channel vào SC1[0] để bắt đầu chuyển đổi */
    ADC0->SC1[0] = Adc_ChannelMap[Group];
}

Adc_StatusType Adc_GetGroupStatus(Adc_GroupType Group)
{
    (void)Group;

    /* kiểm tra cờ COCO: 1 = chuyển đổi xong */
    if ((ADC0->SC1[0] & (1u << 7)) != 0u) {
        return ADC_COMPLETED;
    }

    return ADC_BUSY;
}

void Adc_ReadGroup(Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr)
{
    (void)Group;

    if (DataBufferPtr == NULL_PTR) {
        return;
    }

    /* đọc kết quả 12-bit từ thanh ghi R[0] */
    *DataBufferPtr = (Adc_ValueGroupType)(ADC0->R[0] & 0x0FFFu);
}