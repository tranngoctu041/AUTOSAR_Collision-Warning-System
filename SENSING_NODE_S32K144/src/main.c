#include "sdk_project_config.h"
#include <stdint.h>
#include <stdbool.h>

/* Khai báo biến lưu trạng thái hoạt động của FlexCAN */
extern flexcan_state_t flexcanState0;

/* Cấu hình định dạng bản tin CAN (Transmission Data Info) */
flexcan_data_info_t tx_info = {
    .data_length = 8,                   // Gửi 8 bytes
    .msg_id_type = FLEXCAN_MSG_ID_STD,  // Sử dụng Standard ID (11-bit)
    .enable_brs  = false,               // Không dùng Bit Rate Switch (chỉ dành cho CAN FD)
    .fd_enable   = false,               // Chế độ CAN Classic 2.0 (Không dùng CAN FD)
    .fd_padding  = 0U
};

/* Mảng chứa 8 byte dữ liệu muốn gửi sang STM32 */
uint8_t tx_data[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x00};

/* Hàm tạo trễ (Delay) đơn giản */
void delay(volatile uint32_t cycles) {
    while(cycles--) {
        __asm("nop");
    }
}

int main(void)
{
    /* 1. Khởi tạo hệ thống Clock */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                   g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* 2. Khởi tạo cấu hình Chân (Pin Muxing - bao gồm PTB0 và PTB1 cho CAN0) */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);

    /* 3. Khởi tạo ngoại vi FlexCAN0
     * INST_FLEXCAN_CONFIG_1 và flexcanInitConfig0 là các biến/macro tự động sinh ra từ file .mex
     */
    FLEXCAN_DRV_Init(INST_FLEXCAN_CONFIG_1, &flexcanState0, &flexcanInitConfig0);

    while(1)
    {
        /* 4. Cấu hình Mailbox số 0 làm hộp thư gửi (Tx Mailbox) với ID là 0x123 */
        FLEXCAN_DRV_ConfigTxMb(INST_FLEXCAN_CONFIG_1, 0, &tx_info, 0x123);

        /* 5. Ra lệnh gửi bản tin CAN */
        FLEXCAN_DRV_Send(INST_FLEXCAN_CONFIG_1, 0, &tx_info, 0x123, tx_data);

        /* 6. Chờ cho đến khi phần cứng báo gửi thành công */
        while(FLEXCAN_DRV_GetTransferStatus(INST_FLEXCAN_CONFIG_1, 0) == STATUS_BUSY) {
            /* * Mẹo thực tế: Nếu S32K144 bị kẹt ở vòng lặp này, 99% nguyên nhân là
             * thiếu tín hiệu ACK từ mạch STM32 (do sai Baudrate, chưa nối CAN Transceiver,
             * thiếu trở thanh ghi 120 Ohm, hoặc khác GND).
             */
        }

        /* Tăng giá trị byte cuối lên 1 đơn vị mỗi chu kỳ để bạn dễ quan sát sự thay đổi trên Putty */
        tx_data[7]++;

        /* Chờ khoảng 0.5 - 1s trước khi gửi bản tin tiếp theo */
        delay(5000000);
    }

    return 0;
}
