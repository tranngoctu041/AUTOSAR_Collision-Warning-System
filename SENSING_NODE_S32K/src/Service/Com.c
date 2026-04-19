#include "Com.h"
#include "PduR.h"

Std_ReturnType Com_Transmit(uint16 TxPduId, const uint8* PduInfoPtr, uint8 Length) {
    if (PduInfoPtr == NULL_PTR) return E_NOT_OK;

    /* truyền thẳng dữ liệu xuống PduR */
    return PduR_ComTransmit(TxPduId, PduInfoPtr, Length);
}