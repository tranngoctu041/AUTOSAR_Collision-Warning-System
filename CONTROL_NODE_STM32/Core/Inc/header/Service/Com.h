#ifndef COM_H
#define COM_H

#include "Std_Types.h"
#include "Can.h"

/* nhận PDU từ PduR rồi lưu vào queue Com */
Std_ReturnType Com_RxIndication(const Can_PduType* CanPduPtr);

/* đọc 1 PDU từ queue Com */
Std_ReturnType Com_Receive(Can_PduType* CanPduPtr);

#endif /* COM_H */