#include "Can.h"
#include "stm32f1xx.h"

void Can_Init(void)
{
    /* bật clock cho can1 */
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;

    /* vào init mode */
    CAN1->MCR |= CAN_MCR_INRQ;
    while ((CAN1->MSR & CAN_MSR_INAK) == 0u) {}

    CAN1->MCR &= ~CAN_MCR_SLEEP;

    /* baudrate 500 kbps với apb1 = 36 mhz */
    CAN1->BTR = (0u << 24) | ((3u - 1u) << 20) | ((14u - 1u) << 16) | (4u - 1u);

    /* filter 0 nhận tất cả id vào fifo0 */
    CAN1->FMR |= CAN_FMR_FINIT;
    CAN1->FA1R &= ~1u;
    CAN1->FS1R |= 1u;
    CAN1->FM1R &= ~1u;
    CAN1->sFilterRegister[0].FR1 = 0u;
    CAN1->sFilterRegister[0].FR2 = 0u;
    CAN1->FFA1R &= ~1u;
    CAN1->FA1R |= 1u;
    CAN1->FMR &= ~CAN_FMR_FINIT;

    /* thoát init mode */
    CAN1->MCR &= ~CAN_MCR_INRQ;
    while ((CAN1->MSR & CAN_MSR_INAK) != 0u) {}
}

Std_ReturnType Can_Read(Can_PduType* CanPduPtr)
{
    uint32 rir;
    uint32 rdtr;
    uint32 data_low;
    uint32 data_high;
    uint8 i;

    if (CanPduPtr == NULL_PTR) {
        return E_NOT_OK;
    }

    if ((CAN1->RF0R & 0x03u) == 0u) {
        return E_NOT_OK;
    }

    rir = CAN1->sFIFOMailBox[0].RIR;
    rdtr = CAN1->sFIFOMailBox[0].RDTR;
    data_low = CAN1->sFIFOMailBox[0].RDLR;
    data_high = CAN1->sFIFOMailBox[0].RDHR;

    /* chỉ nhận standard data frame */
    if (((rir & CAN_RI0R_IDE) != 0u) || ((rir & CAN_RI0R_RTR) != 0u)) {
        CAN1->RF0R |= CAN_RF0R_RFOM0;
        return E_NOT_OK;
    }

    CanPduPtr->CanId = (uint32)(rir >> 21);
    CanPduPtr->CanDlc = (uint8)(rdtr & 0x0Fu);

    CanPduPtr->CanData[0] = (uint8)((data_low >> 0) & 0xFFu);
    CanPduPtr->CanData[1] = (uint8)((data_low >> 8) & 0xFFu);
    CanPduPtr->CanData[2] = (uint8)((data_low >> 16) & 0xFFu);
    CanPduPtr->CanData[3] = (uint8)((data_low >> 24) & 0xFFu);
    CanPduPtr->CanData[4] = (uint8)((data_high >> 0) & 0xFFu);
    CanPduPtr->CanData[5] = (uint8)((data_high >> 8) & 0xFFu);
    CanPduPtr->CanData[6] = (uint8)((data_high >> 16) & 0xFFu);
    CanPduPtr->CanData[7] = (uint8)((data_high >> 24) & 0xFFu);

    for (i = CanPduPtr->CanDlc; i < 8u; i++) {
        CanPduPtr->CanData[i] = 0u;
    }

    CAN1->RF0R |= CAN_RF0R_RFOM0;

    return E_OK;
}