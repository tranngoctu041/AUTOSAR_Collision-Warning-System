#ifndef PDUR_H
#define PDUR_H

#include "Std_Types.h"

/* định tuyến PDU từ COM xuống lớp giao tiếp CAN */
Std_ReturnType PduR_ComTransmit(uint16 TxPduId, const uint8* PduInfoPtr, uint8 Length);

#endif /* PDUR_H */