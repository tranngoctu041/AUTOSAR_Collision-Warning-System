#include "Mcu.h"
#include "Port.h"
#include "Dio.h"

/* * Hàm delay "chay" (Vì chúng ta chưa cấu hình module Gpt hay Os)
 * Số vòng lặp có thể cần điều chỉnh tùy thuộc vào tốc độ 48MHz
 */
void delay_dummy(volatile uint32 cycles) {
    while(cycles--) {
        /* Vòng lặp rỗng */
    }
}

int main(void) {
    /* =========================================================
     * BƯỚC 1: KHỞI TẠO TRÁI TIM (XUNG NHỊP)
     * ========================================================= */

    /* Khởi tạo Driver MCU. Vì dùng VariantPreCompile, ta có thể truyền NULL_PTR
       (hoặc truyền &Mcu_Config tùy thuộc vào phiên bản RTD sinh ra) */
    Mcu_Init(NULL_PTR);

    /* Khởi tạo cấu hình Clock số 0 (FIRC 48MHz mà bạn đã setup) */
    Mcu_InitClock(0);

    /* Chờ Clock ổn định. Theo chuẩn AUTOSAR, kể cả không dùng PLL,
       bạn vẫn phải gọi hàm Distribute này để hệ thống chính thức dùng Clock mới */
    /* =========================================================
     * BƯỚC 2: KHỞI TẠO TAY CHÂN (CHÂN CẮM PTD15)
     * ========================================================= */

    /* Hàm này sẽ đọc cấu hình EB Tresos và gán chân PTD15 thành GPIO Output */
    Port_Init(NULL_PTR);

    /* =========================================================
     * BƯỚC 3: VÒNG LẶP VÔ HẠN (NHÁY LED)
     * ========================================================= */
    while (1) {
        /* * Bật LED
         * Macro 'DioConf_DioChannel_RED_LED' được EB Tresos tự động sinh ra
         * từ cái tên 'RED_LED' mà bạn đặt bên module Dio!
         */
        Dio_WriteChannel(DioConf_DioChannel_RED_LED, STD_HIGH);
        delay_dummy(5000000); /* Delay một khoảng thời gian */

        /* Tắt LED */
        Dio_WriteChannel(DioConf_DioChannel_RED_LED, STD_LOW);
        delay_dummy(5000000);
    }

    return 0;
}
