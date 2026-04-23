#include "CanIf.h"
#include "Can.h"

Std_ReturnType CanIf_Transmit(uint16 TxPduId, const uint8* PduInfoPtr, uint8 Length)
{
    uint8 mailbox;

    switch (TxPduId)
    {
        case 0x100:
            mailbox = 1u;
            break;

        case 0x101:
            mailbox = 2u;
            break;

        case 0x102:
            mailbox = 3u;
            break;

        default:
            return E_NOT_OK;
    }

    return Can_Write(mailbox, TxPduId, PduInfoPtr, Length);
}
