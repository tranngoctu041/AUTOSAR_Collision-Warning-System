#include "CanIf.h"
#include "Can.h"

Std_ReturnType CanIf_Transmit(uint16 TxPduId, const uint8* PduInfoPtr, uint8 Length) {
    uint8 mailbox = 0u;

    /* map các PDU ID sang các mailbox tương ứng của phần cứng CAN */
    switch (TxPduId) {
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
            /* nếu ID không hợp lệ để tránh ghi rác vào thanh ghi */
            return E_NOT_OK; 
    }

    return Can_Write(mailbox, TxPduId, PduInfoPtr, Length);
}