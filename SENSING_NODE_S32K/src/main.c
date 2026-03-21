#include "Port.h"
#include "Dio.h"

/* Bổ sung đủ 3 Channel ID */
#define DIO_CHANNEL_LED_BLUE  96  /* PTD0:  (3 * 32) + 0  */
#define DIO_CHANNEL_LED_RED   111 /* PTD15: (3 * 32) + 15 */
#define DIO_CHANNEL_LED_GREEN 112 /* PTD16: (3 * 32) + 16 */

void delay(volatile uint32 count) {
    while(count--) {}
}

int main(void) {
    /* Khởi tạo chức năng chân và chiều Output */
    Port_Init(&Port_Config);

    /* TẮT GỌN GÀNG CẢ 3 LED BAN ĐẦU (Kéo lên mức HIGH) */
    Dio_WriteChannel(DIO_CHANNEL_LED_RED, STD_HIGH);
    Dio_WriteChannel(DIO_CHANNEL_LED_GREEN, STD_HIGH);
    Dio_WriteChannel(DIO_CHANNEL_LED_BLUE, STD_HIGH);

    while(1) {
        /* Nháy LED Đỏ */
        Dio_WriteChannel(DIO_CHANNEL_LED_RED, STD_LOW);   /* Bật */
        delay(1000000);
        Dio_WriteChannel(DIO_CHANNEL_LED_RED, STD_HIGH);  /* Tắt */
        delay(1000000);
        /* Nháy LED Xanh lá */
        //Dio_WriteChannel(DIO_CHANNEL_LED_GREEN, STD_LOW); /* Bật */
        //delay(500000);
        //Dio_WriteChannel(DIO_CHANNEL_LED_GREEN, STD_HIGH);/* Tắt */

        /* Nháy LED Xanh dương */
        //Dio_WriteChannel(DIO_CHANNEL_LED_BLUE, STD_LOW);  /* Bật */
        //delay(500000);
        //Dio_WriteChannel(DIO_CHANNEL_LED_BLUE, STD_HIGH); /* Tắt */
    }
    return 0;
}
