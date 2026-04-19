#ifndef PORT_H
#define PORT_H

#include "Std_Types.h"

#define PORT_A ((uint8)0u)
#define PORT_B ((uint8)1u)
#define PORT_C ((uint8)2u)
#define PORT_D ((uint8)3u)

typedef struct {
    uint8 PortIndex;
    uint8 PinNumber;
    uint8 ConfigValue;
} Port_PinConfigType;

typedef struct {
    const Port_PinConfigType* PinList;
    uint8 NumOfPins;
} Port_ConfigType;

extern const Port_ConfigType Port_Config;

/* khởi tạo cấu hình các chân đã khai báo */
void Port_Init(const Port_ConfigType* ConfigPtr);

#endif /* PORT_H */