#include "CanIf.h"
#include "Can.h"

Std_ReturnType CanIf_RxIndication(Can_PduType* CanPduPtr)
{
    return Can_Read(CanPduPtr);
}