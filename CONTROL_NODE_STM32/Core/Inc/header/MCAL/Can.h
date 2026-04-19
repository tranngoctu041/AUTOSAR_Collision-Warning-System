#ifndef CAN_H
#define CAN_H

#include "Std_Types.h"

typedef struct {
    uint32 CanId;
    uint8 CanDlc;
    uint8 CanData[8];
} Can_PduType;

/* khởi tạo can1 */
void Can_Init(void);

/* đọc 1 frame can từ fifo0 */
Std_ReturnType Can_Read(Can_PduType* CanPduPtr);

#endif /* CAN_H */