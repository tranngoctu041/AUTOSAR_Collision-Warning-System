#include "Dio.h"
#include "S32K144.h"

static GPIO_Type * const Dio_GpioBasePtrs[] = {
    (GPIO_Type *)IP_PTA_BASE, /* Port A */
    (GPIO_Type *)IP_PTB_BASE, /* Port B */
    (GPIO_Type *)IP_PTC_BASE, /* Port C */
    (GPIO_Type *)IP_PTD_BASE, /* Port D */
    (GPIO_Type *)IP_PTE_BASE  /* Port E */
};

void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level)
{
    uint8 port = (uint8)(ChannelId / 32u);
    uint8 pin  = (uint8)(ChannelId % 32u);

    if (Level == STD_HIGH) {
        /* ghi 1 vào PSOR để set mức cao */
        Dio_GpioBasePtrs[port]->PSOR = (1u << pin);
    } else {
        /* ghi 1 vào PCOR để kéo chân xuống mức thấp */
        Dio_GpioBasePtrs[port]->PCOR = (1u << pin);
    }
}

Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId)
{
    uint8 port = (uint8)(ChannelId / 32u);
    uint8 pin  = (uint8)(ChannelId % 32u);
    Dio_LevelType level;

    /* đọc trạng thái chân từ thanh ghi PDIR */
    if ((Dio_GpioBasePtrs[port]->PDIR & (1u << pin)) != 0u) {
        level = STD_HIGH;
    } else {
        level = STD_LOW;
    }

    return level;
}

Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId)
{
    uint8 port = (uint8)(ChannelId / 32u);
    uint8 pin  = (uint8)(ChannelId % 32u);

    /* ghi 1 vào PTOR để đảo trạng thái chân */
    Dio_GpioBasePtrs[port]->PTOR = (1u << pin);

    /* đọc lại để trả về trạng thái mới */
    return Dio_ReadChannel(ChannelId);
}