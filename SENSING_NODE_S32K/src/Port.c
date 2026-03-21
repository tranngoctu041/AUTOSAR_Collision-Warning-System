#include "Port.h"
#include "S32K144.h"

const Port_PinConfigType Port_PinConfigList[] = {
    {PORT_D, 15, 1, PORT_PIN_OUT},
    {PORT_D, 16, 1, PORT_PIN_OUT},
    {PORT_D, 0,  1, PORT_PIN_OUT}
};

/* Đóng gói mảng trên vào biến cấu hình tổng để giao tiếp với hàm Init */
const Port_ConfigType Port_Config = {
    Port_PinConfigList,
    3 /* Có 3 chân cần khởi tạo */
};

static PORT_Type * const portBases[] = {
    (PORT_Type *)0x40049000u, (PORT_Type *)0x4004A000u,
    (PORT_Type *)0x4004B000u, (PORT_Type *)0x4004C000u, (PORT_Type *)0x4004D000u
};

static GPIO_Type * const gpioBases[] = {
    (GPIO_Type *)0x400FF000u, (GPIO_Type *)0x400FF040u,
    (GPIO_Type *)0x400FF080u, (GPIO_Type *)0x400FF0C0u, (GPIO_Type *)0x400FF100u
};

void Port_Init(const Port_ConfigType* ConfigPtr) {
    if (ConfigPtr == NULL_PTR) return;

    for (uint8 i = 0; i < ConfigPtr->NumOfPins; i++) {
        uint8 port = ConfigPtr->PinList[i].PortIndex;
        uint8 pin  = ConfigPtr->PinList[i].PinNumber;
        uint8 mux  = ConfigPtr->PinList[i].Mux;
        uint8 dir  = ConfigPtr->PinList[i].Direction;

        /* 1. Cấp Clock bằng địa chỉ vật lý (Base của PCC_PORTA là 0x40065124, mỗi port cách nhau 4 byte) */
        *((volatile uint32 *)(0x40065124u + (port * 4))) |= (1 << 30);

        /* 2. Cấu hình MUX (Dùng struct chuẩn) */
        portBases[port]->PCR[pin] &= ~(0x700u); /* Xóa bit 8-10 */
        portBases[port]->PCR[pin] |= (mux << 8);

        /* 3. Cấu hình Direction */
        if (dir == PORT_PIN_OUT) {
            gpioBases[port]->PDDR |= (1 << pin);
        } else {
            gpioBases[port]->PDDR &= ~(1 << pin);
        }
    }
}
