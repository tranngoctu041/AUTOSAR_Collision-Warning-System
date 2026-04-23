#include "Port.h"
#include "stm32f1xx.h"

const Port_PinConfigType Port_PinConfigList[] = {
    /* uart1 tx, rx */
    {PORT_A, 9u,  0x0Bu},
    {PORT_A, 10u, 0x04u},

    /* can1 rx, tx */
    {PORT_A, 11u, 0x08u},
    {PORT_A, 12u, 0x0Bu}
};

const Port_ConfigType Port_Config = {
    Port_PinConfigList,
    4u
};

static GPIO_TypeDef * const Port_GpioBase[] = {
    GPIOA,
    GPIOB,
    GPIOC,
    GPIOD
};

void Port_Init(const Port_ConfigType* ConfigPtr)
{
    uint8 i;

    if (ConfigPtr == NULL_PTR) {
        return;
    }

    /* bật clock gpioa và afio */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN |
                    RCC_APB2ENR_AFIOEN;

    for (i = 0u; i < ConfigPtr->NumOfPins; i++) {
        uint8 port = ConfigPtr->PinList[i].PortIndex;
        uint8 pin = ConfigPtr->PinList[i].PinNumber;
        uint8 cfg = ConfigPtr->PinList[i].ConfigValue;

        if (pin < 8u) {
            Port_GpioBase[port]->CRL &= ~(0x0Fu << (pin * 4u));
            Port_GpioBase[port]->CRL |= ((uint32)cfg << (pin * 4u));
        } else {
            uint8 shift = (uint8)((pin - 8u) * 4u);
            Port_GpioBase[port]->CRH &= ~(0x0Fu << shift);
            Port_GpioBase[port]->CRH |= ((uint32)cfg << shift);
        }
    }

    /* kéo lên cho can rx */
    GPIOA->ODR |= (1u << 11);
}
