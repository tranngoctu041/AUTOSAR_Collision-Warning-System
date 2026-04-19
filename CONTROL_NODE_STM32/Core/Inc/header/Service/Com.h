#ifndef COM_H
#define COM_H

#include "Std_Types.h"
#include "Can.h"

/* nhận pdu từ pdur rồi lưu vào queue com */
Std_ReturnType Com_RxIndication(const Can_PduType* CanPduPtr);

/* đọc 1 pdu từ queue com */
Std_ReturnType Com_Receive(Can_PduType* CanPduPtr);

#endif /* COM_H */