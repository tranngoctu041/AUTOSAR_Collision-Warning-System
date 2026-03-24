#include "Adc.h"
#include "S32K144.h"

#define PCC  ((PCC_Type *)IP_PCC_BASE)
#define ADC0 ((ADC_Type *)IP_ADC0_BASE)

/* Chỉ số Index của ADC0 trong thanh ghi PCC */
#define PCC_ADC0_INDEX 39

static const uint8 Adc_ChannelMap[] = {
    0u, /* ADC_GROUP_RADAR -> Channel 0 (PTA0 / ADC0_SE0) */
    1u  /* ADC_GROUP_RAIN  -> Channel 1 (PTA1 / ADC0_SE1) */
};

void Adc_Init(const Adc_ConfigType* ConfigPtr) {
    if (ConfigPtr == NULL_PTR) return;

    /* 1. CẤP XUNG NHỊP CHO ADC0 QUA PCC */
    /* Bit 30: CGC xóa về 0 trước khi đổi cấu hình */
    PCC->PCCn[PCC_ADC0_INDEX] &= ~(1u << 30); 
    
    /* Bit 24-26: PCS (Peripheral Clock Source). Chọn FIRC 48MHz -> giá trị 3 (011) */
    PCC->PCCn[PCC_ADC0_INDEX] |= (3u << 24);  
    
    /* Bật lại CGC (Bit 30 = 1) để cấp Clock */
    PCC->PCCn[PCC_ADC0_INDEX] |= (1u << 30);  

    /* 2. CẤU HÌNH ĐỘ PHÂN GIẢI (12-BIT) */
    /* Thanh ghi CFG1: Bit 2-3 (MODE) = 01 -> 12-bit resolution */
    ADC0->CFG1 = (1u << 2); 

    /* 3. CẤU HÌNH CHẾ ĐỘ HOẠT ĐỘNG */
    /* Thanh ghi SC2: Bit 6 (ADTRG) = 0 -> Software Trigger */
    ADC0->SC2 = 0x00u; 
    
    /* Thanh ghi SC3: Bit 3 (ADCO) = 0 -> Single Conversion */
    ADC0->SC3 = 0x00u; 
}

void Adc_StartGroupConversion(Adc_GroupType Group) {
    uint8 channel = Adc_ChannelMap[Group];
    
    /* Ghi ID của channel vật lý vào 5 bit đầu (0-4) của thanh ghi SC1[0]
    -> TỰ ĐỘNG kích hoạt quá trình chuyển đổi ADC*/
    ADC0->SC1[0] = channel; 
}

Adc_StatusType Adc_GetGroupStatus(Adc_GroupType Group) {
    /* Kiểm tra cờ COCO ở Bit 7 của thanh ghi SC1[0] */
    if ((ADC0->SC1[0] & (1u << 7)) != 0u) {
        return ADC_COMPLETED;
    }
    return ADC_BUSY;
}

void Adc_ReadGroup(Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr) {
    if (DataBufferPtr != NULL_PTR) {
        /* Đọc giá trị từ thanh ghi Result R[0].
        & 0xFFF đảm bảo chỉ lấy đúng 12 bit dữ liệu (0 - 4095). */
        *DataBufferPtr = (Adc_ValueGroupType)(ADC0->R[0] & 0xFFFu); 
        /* đọc R[0] sẽ tự động xóa cờ COCO trên phần cứng */
    }
}
