#include "Port.h"
#include "S32K144.h"

#define PCC ((PCC_Type *)IP_PCC_BASE)

/* Chỉ số Index bắt đầu của PORTA trong mảng cấu hình xung nhịp PCC */
#define PCC_PORTA_INDEX 73

const Port_PinConfigType Port_PinConfigList[] = {
    /* 1. Chân Cảm biến mưa MH-RD (PTA1) -> MUX = 0 (Analog) */
    {PORT_A, 1, 0, PORT_PIN_IN},
    
    /* 2. Chân truyền dữ liệu lên máy tính PuTTY (LPUART1_TX) -> PTC7, MUX = 2 */
    {PORT_C, 7, 2, PORT_PIN_OUT},
    
    /* 3. Chân đọc LiDAR TF-Luna (LPUART0_RX) -> PTC2, MUX = 4 */
    {PORT_C, 2, 4, PORT_PIN_IN},
	{PORT_D, 16, 1, PORT_PIN_OUT},
	{PORT_D, 15, 2, PORT_PIN_IN}
};

/* Đóng gói mảng trên vào biến cấu hình tổng để giao tiếp với hàm Init */
const Port_ConfigType Port_Config = {
    Port_PinConfigList,
    5 /* Có 3 chân cần khởi tạo */
};

static PORT_Type * const portBases[] = {
    (PORT_Type *)IP_PORTA_BASE, (PORT_Type *)IP_PORTB_BASE,
    (PORT_Type *)IP_PORTC_BASE, (PORT_Type *)IP_PORTD_BASE, (PORT_Type *)IP_PORTE_BASE
};

static GPIO_Type * const gpioBases[] = {
    (GPIO_Type *)IP_PTA_BASE, (GPIO_Type *)IP_PTB_BASE,
    (GPIO_Type *)IP_PTC_BASE, (GPIO_Type *)IP_PTD_BASE, (GPIO_Type *)IP_PTE_BASE
};

void Port_Init(const Port_ConfigType* ConfigPtr) {
    if (ConfigPtr == NULL_PTR) return;

    for (uint8 i = 0; i < ConfigPtr->NumOfPins; i++) {
        uint8 port = ConfigPtr->PinList[i].PortIndex;
        uint8 pin  = ConfigPtr->PinList[i].PinNumber;
        uint8 mux  = ConfigPtr->PinList[i].Mux;
        uint8 dir  = ConfigPtr->PinList[i].Direction;

        /* 1. Cấp Clock thông qua thanh ghi cấu trúc của NXP (Bit 30 là CGC) */
        /* port đi từ 0 đến 4 nên cộng thẳng vào Index 73 sẽ trúng đích các Port A -> E */
        PCC->PCCn[PCC_PORTA_INDEX + port] |= (1 << 30);

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
