#ifndef COM_H
#define COM_H

#include "Std_Types.h"

/* truyền PDU xuống lớp định tuyến */
Std_ReturnType Com_Transmit(uint16 TxPduId, const uint8* PduInfoPtr, uint8 Length);

#endif /* COM_H */