#include "Com.h"

#define COM_RX_QUEUE_SIZE 8u

static Can_PduType Com_RxQueue[COM_RX_QUEUE_SIZE]; /* hàng đợi nhận PDU từ PduR */
static uint8 Com_RxHead = 0u; /* vị trí để ghi dữ liệu mới */
static uint8 Com_RxTail = 0u; /* vị trí để đọc dữ liệu cũ */
static uint8 Com_RxCount = 0u; /* số lượng PDU trong hàng đợi */

Std_ReturnType Com_RxIndication(const Can_PduType* CanPduPtr)
{
    if (CanPduPtr == NULL_PTR) {
        return E_NOT_OK;
    }

    /* kiểm tra nếu hàng đợi đầy */
    if (Com_RxCount >= COM_RX_QUEUE_SIZE) {
        Com_RxTail = (uint8)((Com_RxTail + 1u) % COM_RX_QUEUE_SIZE);
        Com_RxCount--;
    }

    /* ghi bản tin mới vào vị trí Head */
    Com_RxQueue[Com_RxHead] = *CanPduPtr;
    Com_RxHead = (uint8)((Com_RxHead + 1u) % COM_RX_QUEUE_SIZE);
    Com_RxCount++;

    return E_OK;
}

Std_ReturnType Com_Receive(Can_PduType* CanPduPtr)
{
    if (CanPduPtr == NULL_PTR) {
        return E_NOT_OK;
    }

    /* kiểm tra nếu hàng đợi rỗng */
    if (Com_RxCount == 0u) {
        return E_NOT_OK;
    }

    /* lấy bản tin cũ từ vị trí Tail */
    *CanPduPtr = Com_RxQueue[Com_RxTail];
    Com_RxTail = (uint8)((Com_RxTail + 1u) % COM_RX_QUEUE_SIZE);
    Com_RxCount--;

    return E_OK;
}