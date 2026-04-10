#include "Can.h"
#include "S32K144.h"

#define PCC  ((PCC_Type *)IP_PCC_BASE)
#define SCG  ((SCG_Type *)IP_SCG_BASE)
#define CAN0 (IP_FLEXCAN0)

#define PCC_FlexCAN0_INDEX 36

void Can_Init(void) {
    /* cấp clock cho FlexCAN0 */
    PCC->PCCn[PCC_FlexCAN0_INDEX] |= (1u << 30);

    CAN0->MCR &= ~(1u << 31);

    while ((CAN0->MCR & (1u << 20)) != 0u) {}

    /* vào freeze mode để cấu hình */
    CAN0->MCR |= (1u << 30) | (1u << 28);
    while ((CAN0->MCR & (1u << 24)) == 0u) {}

    CAN0->CTRL1 &= ~(1u << 13);

    /* reset mềm module */
    CAN0->MCR |= (1u << 25);
    while ((CAN0->MCR & (1u << 25)) != 0u) {}

    CAN0->CTRL1 &= ~(1u << 13);

    /* vào lại freeze mode */
    CAN0->MCR |= (1u << 30) | (1u << 28);
    while ((CAN0->MCR & (1u << 24)) == 0u) {}

    /* baudrate 500 kbps */
    CAN0->CTRL1 = 0x005B0006u;

    for (uint32 i = 0u; i < 128u; i++) {
        CAN0->RAMn[i] = 0u;
    }

    /* dùng tối đa 16 message buffer */
    CAN0->MCR &= ~(0x3Fu);
    CAN0->MCR |= 16u;

    /* thoát freeze mode */
    CAN0->MCR &= ~(1u << 30);
    CAN0->MCR &= ~(1u << 28);
    while ((CAN0->MCR & (1u << 24)) != 0u) {}
}

Std_ReturnType Can_Write(uint8 Controller, uint8 Hth, uint32 CanId, const uint8* PduInfoPtr, uint8 Length)
{
    uint32 mb_index;
    uint32 word0 = 0u;
    uint32 word1 = 0u;

    (void)Controller;

    if ((PduInfoPtr == NULL_PTR) || (Length > 8u)) {
        return E_NOT_OK;
    }

    if ((Hth == 0u) || (Hth > 15u)) {
        return E_NOT_OK;
    }

    mb_index = (uint32)Hth * 4u;

    /* gán standard ID */
    CAN0->RAMn[mb_index + 1u] = (CanId << 18);

    /* đóng gói 8 byte dữ liệu vào 2 word */
    if (Length > 0u) word0 |= ((uint32)PduInfoPtr[0] << 24);
    if (Length > 1u) word0 |= ((uint32)PduInfoPtr[1] << 16);
    if (Length > 2u) word0 |= ((uint32)PduInfoPtr[2] << 8);
    if (Length > 3u) word0 |= ((uint32)PduInfoPtr[3]);

    if (Length > 4u) word1 |= ((uint32)PduInfoPtr[4] << 24);
    if (Length > 5u) word1 |= ((uint32)PduInfoPtr[5] << 16);
    if (Length > 6u) word1 |= ((uint32)PduInfoPtr[6] << 8);
    if (Length > 7u) word1 |= ((uint32)PduInfoPtr[7]);

    CAN0->RAMn[mb_index + 2u] = word0;
    CAN0->RAMn[mb_index + 3u] = word1;

    /* ghi control word để bắt đầu truyền */
    CAN0->RAMn[mb_index] = 0x0C000000u | ((uint32)Length << 16);

    return E_OK;
}
