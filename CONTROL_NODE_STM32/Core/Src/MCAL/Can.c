#include "Can.h"
#include "stm32f1xx.h"

void Can_Init(void)
{
    /* bật clock cho CAN1 */
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;

    CAN1->MCR |= CAN_MCR_INRQ;
    while ((CAN1->MSR & CAN_MSR_INAK) == 0u) {}

    CAN1->MCR &= ~CAN_MCR_SLEEP;

    /* baudrate 500 kbps với APB1 = 36 MHz */
    CAN1->BTR = (0u << 24) | ((3u - 1u) << 20) | ((14u - 1u) << 16) | (4u - 1u);

    /* filter 0 nhận tất cả ID vào FIFO0 */
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

    /* kiểm tra xem có frame nào trong FIFO0 không */
    if ((CAN1->RF0R & 0x03u) == 0u) {
        return E_NOT_OK;
    }

    rir = CAN1->sFIFOMailBox[0].RIR; /* thanh ghi chứa ID và loại frame */ 
    rdtr = CAN1->sFIFOMailBox[0].RDTR; /* thanh ghi chứa DLC */
    data_low = CAN1->sFIFOMailBox[0].RDLR; /* thanh ghi chứa 4 byte dữ liệu đầu */
    data_high = CAN1->sFIFOMailBox[0].RDHR; /* thanh ghi chứa 4 byte dữ liệu sau */

    /* chỉ nhận standard data frame */
    if (((rir & CAN_RI0R_IDE) != 0u) || ((rir & CAN_RI0R_RTR) != 0u)) {
        CAN1->RF0R |= CAN_RF0R_RFOM0;
        return E_NOT_OK;
    }

    /* ID tiêu chuẩn 11-bit nằm ở nửa trên của thanh ghi RIR => dịch phải 21 bit */
    CanPduPtr->CanId = (uint32)(rir >> 21);
    CanPduPtr->CanDlc = (uint8)(rdtr & 0x0Fu);

    /* chuyển đổi dữ liệu từ các thanh ghi vào mảng CanData */
    CanPduPtr->CanData[0] = (uint8)((data_low >> 0) & 0xFFu);
    CanPduPtr->CanData[1] = (uint8)((data_low >> 8) & 0xFFu);
    CanPduPtr->CanData[2] = (uint8)((data_low >> 16) & 0xFFu);
    CanPduPtr->CanData[3] = (uint8)((data_low >> 24) & 0xFFu);
    CanPduPtr->CanData[4] = (uint8)((data_high >> 0) & 0xFFu);
    CanPduPtr->CanData[5] = (uint8)((data_high >> 8) & 0xFFu);
    CanPduPtr->CanData[6] = (uint8)((data_high >> 16) & 0xFFu);
    CanPduPtr->CanData[7] = (uint8)((data_high >> 24) & 0xFFu);

    /* xoá các byte ko dùng */
    for (i = CanPduPtr->CanDlc; i < 8u; i++) {
        CanPduPtr->CanData[i] = 0u;
    }

    /* frame trong FIFO0 đã được đọc xong */
    CAN1->RF0R |= CAN_RF0R_RFOM0;

    return E_OK;
}
