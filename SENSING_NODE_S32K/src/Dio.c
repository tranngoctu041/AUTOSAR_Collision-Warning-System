#include "Dio.h"
#include "S32K144.h"

static GPIO_Type * const Dio_GpioBasePtrs[] = {
    (GPIO_Type *)0x400FF000u, /* Port A */
    (GPIO_Type *)0x400FF040u, /* Port B */
    (GPIO_Type *)0x400FF080u, /* Port C */
    (GPIO_Type *)0x400FF0C0u, /* Port D */
    (GPIO_Type *)0x400FF100u  /* Port E */
};

/* Hàm ghi write */
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level) {
    uint8 port = (uint8)(ChannelId / 32);
    uint8 pin  = (uint8)(ChannelId % 32);

    if (Level == STD_HIGH) {
        /* PSOR: Port Set Output Register - Ghi 1 để kéo chân lên mức cao */
        Dio_GpioBasePtrs[port]->PSOR = (1u << pin);
    } else {
        /* PCOR: Port Clear Output Register - Ghi 1 để kéo chân xuống mức thấp */
        Dio_GpioBasePtrs[port]->PCOR = (1u << pin);
    }
}

/* Hàm đọc read */
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId) {
    uint8 port = (uint8)(ChannelId / 32);
    uint8 pin  = (uint8)(ChannelId % 32);
    Dio_LevelType level;

    /* PDIR: Port Data Input Register - Đọc trạng thái vật lý của chân */
    if ((Dio_GpioBasePtrs[port]->PDIR & (1u << pin)) != 0u) {
        level = STD_HIGH;
    } else {
        level = STD_LOW;
    }
    
    return level;
}

/* Hàm đảo trạng thái flip */
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId) {
    uint8 port = (uint8)(ChannelId / 32);
    uint8 pin  = (uint8)(ChannelId % 32);

    /* PTOR: Port Toggle Output Register - Đảo trạng thái hiện tại */
    Dio_GpioBasePtrs[port]->PTOR = (1u << pin);

    /* Sau khi đảo, đọc lại và trả về trạng thái mới để đúng đặc tả AUTOSAR */
    return Dio_ReadChannel(ChannelId);
}