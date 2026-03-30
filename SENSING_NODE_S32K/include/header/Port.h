#ifndef PORT_H
#define PORT_H

#include "Std_Types.h"

#define PORT_PIN_IN  0x00u
#define PORT_PIN_OUT 0x01u

#define PORT_A 0
#define PORT_B 1
#define PORT_C 2
#define PORT_D 3
#define PORT_E 4

typedef struct {
    uint8 PortIndex;
    uint8 PinNumber;
    uint8 Mux;
    uint8 Direction; /* PORT_PIN_OUT hoặc PORT_PIN_IN */
} Port_PinConfigType;

/* Cấu trúc mô tả toàn bộ hệ thống Port (truyền vào hàm Init) */
typedef struct {
    const Port_PinConfigType *PinList; /* Con trỏ trỏ đến mảng các chân */
    uint8 NumOfPins;                   /* Tổng số chân trong mảng */
} Port_ConfigType;

extern const Port_ConfigType Port_Config;

/* Hàm khởi tạo toàn bộ chân dựa trên cấu hình truyền vào */
void Port_Init(const Port_ConfigType* ConfigPtr);

#endif /* PORT_H */
