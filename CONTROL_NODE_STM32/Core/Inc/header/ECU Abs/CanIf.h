#ifndef CANIF_H
#define CANIF_H

#include "Std_Types.h"
#include "Can.h"

/* nhận 1 PDU từ CAN driver */
Std_ReturnType CanIf_RxIndication(Can_PduType* CanPduPtr);

#endif /* CANIF_H */