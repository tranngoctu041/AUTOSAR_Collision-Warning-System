#ifndef DIO_H
#define DIO_H

#include "Std_Types.h"

/* kiểu dữ liệu cho ID chân */
typedef uint16 Dio_ChannelType;

/* mức logic của chân */
typedef uint8 Dio_LevelType;

/* ghi mức logic ra chân */
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level);

/* đọc mức logic của chân */
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId);

/* đảo trạng thái chân và trả về mức mới */
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId);

#endif /* DIO_H */