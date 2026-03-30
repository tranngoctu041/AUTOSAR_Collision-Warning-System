#include "Adc.h"
#include "S32K144.h"

#define PCC  ((PCC_Type *)IP_PCC_BASE)
#define ADC0 ((ADC_Type *)IP_ADC0_BASE)

/* Chỉ số Index của ADC0 trong thanh ghi PCC */
#define PCC_ADC0_INDEX 59

static const uint8 Adc_ChannelMap[] = {
    0u, /* ADC_GROUP_RADAR -> Channel 0 (PTA0 / ADC0_SE0) */
    1u  /* ADC_GROUP_RAIN  -> Channel 1 (PTA1 / ADC0_SE1) */
};

void Adc_Init(const Adc_ConfigType* ConfigPtr)
{
    (void)ConfigPtr;

    /* 1. Cấp clock cho ADC0 từ FIRC 48MHz */
    PCC->PCCn[PCC_ADC0_INDEX] &= ~(1u << 30);           /* Tắt CGC trước */

    /* ---> THÊM DÒNG NÀY: Xóa sạch 3 bit PCS cũ (Mặt nạ số 7) <--- */
    PCC->PCCn[PCC_ADC0_INDEX] &= ~(7u << 24);

    PCC->PCCn[PCC_ADC0_INDEX] |= (3u << 24);            /* PCS = 011 = FIRC */
    PCC->PCCn[PCC_ADC0_INDEX] |= (1u << 30);            /* Bật CGC */

    /* 2. Cấu hình ADC (Giữ nguyên phần dưới của bạn vì nó rất chuẩn rồi) */
    ADC0->CFG1 = (1u << 2) | (1u << 5);
    ADC0->CFG2 = (12u << 0);
    ADC0->SC2  = 0x00u;
    ADC0->SC3  = 0x00u;
    ADC0->SC1[0] = 0x1Fu;
}

void Adc_StartGroupConversion(Adc_GroupType Group) {
    uint8 channel = Adc_ChannelMap[Group];
    
    /* Ghi ID của channel vật lý vào 5 bit đầu (0-4) của thanh ghi SC1[0]
    -> TỰ ĐỘNG kích hoạt quá trình chuyển đổi ADC*/
    ADC0->SC1[0] = channel; 
}

Adc_StatusType Adc_GetGroupStatus(Adc_GroupType Group) {
    /* Kiểm tra cờ COCO ở Bit 7 của thanh ghi SC1[0] */
	(void)Group;
    if ((ADC0->SC1[0] & (1u << 7)) != 0u) {
        return ADC_COMPLETED;
    }
    return ADC_BUSY;
}

void Adc_ReadGroup(Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr) {
	(void)Group;
    if (DataBufferPtr != NULL_PTR) {
        /* Đọc giá trị từ thanh ghi Result R[0].
        & 0xFFF đảm bảo chỉ lấy đúng 12 bit dữ liệu (0 - 4095). */
        *DataBufferPtr = (Adc_ValueGroupType)(ADC0->R[0] & 0xFFFu); 
        /* đọc R[0] sẽ tự động xóa cờ COCO trên phần cứng */
    }
}
