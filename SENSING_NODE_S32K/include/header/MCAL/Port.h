#ifndef PORT_H
#define PORT_H

#include "Std_Types.h"

#define PORT_PIN_IN  0x00u
#define PORT_PIN_OUT 0x01u

#define PORT_A 0u
#define PORT_B 1u
#define PORT_C 2u
#define PORT_D 3u
#define PORT_E 4u

typedef struct {
    uint8 PortIndex;
    uint8 PinNumber;
    uint8 Mux;
    uint8 Direction;
} Port_PinConfigType;

typedef struct {
    const Port_PinConfigType *PinList;
    uint8 NumOfPins;
} Port_ConfigType;

extern const Port_ConfigType Port_Config;

/* khởi tạo toàn bộ chân theo cấu hình truyền vào */
void Port_Init(const Port_ConfigType* ConfigPtr);

#endif /* PORT_H */