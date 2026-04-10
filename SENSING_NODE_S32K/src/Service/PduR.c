#include "PduR.h"
#include "CanIf.h"

Std_ReturnType PduR_ComTransmit(uint16 TxPduId, const uint8* PduInfoPtr, uint8 Length) {
    /* hệ thống này chỉ dùng CAN nên chuyển xuống CanIf */
    return CanIf_Transmit(TxPduId, PduInfoPtr, Length);
}