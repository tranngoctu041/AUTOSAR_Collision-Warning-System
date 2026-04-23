#ifndef CAN_H
#define CAN_H

#include "Std_Types.h"

/* khởi tạo FlexCAN0 */
void Can_Init(void);

/* ghi một frame CAN ra mailbox truyền */
Std_ReturnType Can_Write(uint8 Hth,
                         uint32 CanId,
                         const uint8* PduInfoPtr,
                         uint8 Length);

#endif /* CAN_H */