#ifndef CANIF_H
#define CANIF_H

#include "Std_Types.h"

/* truyền PDU xuống CAN driver */
Std_ReturnType CanIf_Transmit(uint16 TxPduId, const uint8* PduInfoPtr, uint8 Length);

#endif /* CANIF_H */