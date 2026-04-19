#include "Port.h"
#include "S32K144.h"

#define PCC ((PCC_Type *)IP_PCC_BASE)

const Port_PinConfigType Port_PinConfigList[] = {
    /* radar CDM324 */
    {PORT_E, 2u, 0u, PORT_PIN_IN},

    /* cảm biến mưa */
    {PORT_A, 1u, 0u, PORT_PIN_IN},

    /* biến trở mô phỏng tốc độ */
    {PORT_A, 6u, 0u, PORT_PIN_IN},

    /* LiDAR RX */
    {PORT_C, 2u, 4u, PORT_PIN_IN},

    /* siêu âm trước */
    {PORT_D, 16u, 1u, PORT_PIN_OUT}, /* TRIG_FRONT */
    {PORT_D, 15u, 2u, PORT_PIN_IN},  /* ECHO_FRONT */

    /* siêu âm trái */
    {PORT_D, 13u, 1u, PORT_PIN_OUT}, /* TRIG_LEFT */
    {PORT_D, 1u, 2u, PORT_PIN_IN},   /* ECHO_LEFT */

    /* siêu âm phải */
    {PORT_D, 14u, 1u, PORT_PIN_OUT}, /* TRIG_RIGHT */
    {PORT_D, 0u, 2u, PORT_PIN_IN},   /* ECHO_RIGHT */

    /* CAN0 RX/TX */
    {PORT_E, 4u, 5u, PORT_PIN_IN},
    {PORT_E, 5u, 5u, PORT_PIN_OUT}
};

const Port_ConfigType Port_Config = {
    Port_PinConfigList,
    12u
};

static PORT_Type * const portBases[] = {
    (PORT_Type *)IP_PORTA_BASE,
    (PORT_Type *)IP_PORTB_BASE,
    (PORT_Type *)IP_PORTC_BASE,
    (PORT_Type *)IP_PORTD_BASE,
    (PORT_Type *)IP_PORTE_BASE
};

static GPIO_Type * const gpioBases[] = {
    (GPIO_Type *)IP_PTA_BASE,
    (GPIO_Type *)IP_PTB_BASE,
    (GPIO_Type *)IP_PTC_BASE,
    (GPIO_Type *)IP_PTD_BASE,
    (GPIO_Type *)IP_PTE_BASE
};

void Port_Init(const Port_ConfigType* ConfigPtr)
{
    uint8 i;

    if (ConfigPtr == NULL_PTR) {
        return;
    }

    for (i = 0u; i < ConfigPtr->NumOfPins; i++) {
        uint8 port = ConfigPtr->PinList[i].PortIndex;
        uint8 pin  = ConfigPtr->PinList[i].PinNumber;
        uint8 mux  = ConfigPtr->PinList[i].Mux;
        uint8 dir  = ConfigPtr->PinList[i].Direction;

        /* cấp clock cho PORT tương ứng */
        PCC->PCCn[PCC_PORTA_INDEX + port] |= (1u << 30);

        /* cấu hình mux cho chân */
        portBases[port]->PCR[pin] &= ~(0x700u);
        portBases[port]->PCR[pin] |= ((uint32)mux << 8);

        /* cấu hình hướng dữ liệu */
        if (dir == PORT_PIN_OUT) {
            gpioBases[port]->PDDR |= (1u << pin);
        } else {
            gpioBases[port]->PDDR &= ~(1u << pin);
        }
    }
}
