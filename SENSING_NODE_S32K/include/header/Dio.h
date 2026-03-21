#ifndef DIO_H
#define DIO_H

#include "Std_Types.h"

/* Kiểu dữ liệu chuẩn AUTOSAR cho DIO */
typedef uint16 Dio_ChannelType; /* Định danh ID của chân */
typedef uint8  Dio_LevelType;   /* Mức logic: STD_HIGH hoặc STD_LOW */

/* Hàm ghi mức logic ra một chân */
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level);

/* Hàm đọc mức logic từ một chân */
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId);

/* Hàm đảo trạng thái logic của một chân (Toggle) */
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId);

#endif /* DIO_H */