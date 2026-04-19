#include "PduR.h"
#include "CanIf.h"
#include "Com.h"

Std_ReturnType PduR_CanIfRxIndication(void)
{
    Can_PduType RxPdu;

    if (CanIf_RxIndication(&RxPdu) != E_OK) {
        return E_NOT_OK;
    }

    return Com_RxIndication(&RxPdu);
}