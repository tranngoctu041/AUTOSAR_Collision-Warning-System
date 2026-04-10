#include "IoHwAb_Sensors.h"
#include "Adc.h"
#include "Dio.h"
#include "Icu.h"

#define TRIG_FRONT 112u /* PTD16 */
#define TRIG_LEFT  109u /* PTD13 */
#define TRIG_RIGHT 110u /* PTD14 */

static void delay_10us(void) {
    for (volatile uint32 i = 0u; i < 120u; i++);
}

void IoHwAb_Sensors_Init(void) {
    Dio_WriteChannel(TRIG_FRONT, STD_LOW);
    Dio_WriteChannel(TRIG_LEFT, STD_LOW);
    Dio_WriteChannel(TRIG_RIGHT, STD_LOW);
}

void IoHwAb_Read_FrontUltra(uint16* dist_cm) {
    uint16 echo;

    Dio_WriteChannel(TRIG_FRONT, STD_HIGH);
    delay_10us();
    Dio_WriteChannel(TRIG_FRONT, STD_LOW);

    echo = Icu_MeasurePulseWidth_us(0u); /* FTM0_CH0 */
    *dist_cm = (echo == 0xFFFFu) ? 0xFFFFu : (echo / 58u);
}

void IoHwAb_Read_SideUltras(uint16* left_cm, uint16* right_cm) {
    static uint8 toggle = 0u;
    static uint16 last_left = 0xFFFFu;
    static uint16 last_right = 0xFFFFu;
    uint16 echo;

    if (toggle == 0u) {
        /* đọc cảm biến bên trái */
        Dio_WriteChannel(TRIG_LEFT, STD_HIGH);
        delay_10us();
        Dio_WriteChannel(TRIG_LEFT, STD_LOW);

        echo = Icu_MeasurePulseWidth_us(3u); /* FTM0_CH3 */
        last_left = (echo == 0xFFFFu) ? 0xFFFFu : (echo / 58u);
        toggle = 1u;
    } else {
        /* đọc cảm biến bên phải */
        Dio_WriteChannel(TRIG_RIGHT, STD_HIGH);
        delay_10us();
        Dio_WriteChannel(TRIG_RIGHT, STD_LOW);

        echo = Icu_MeasurePulseWidth_us(2u); /* FTM0_CH2 */
        last_right = (echo == 0xFFFFu) ? 0xFFFFu : (echo / 58u);
        toggle = 0u;
    }

    *left_cm = last_left;
    *right_cm = last_right;
}

void IoHwAb_Read_Rain(uint16* adc_raw) {
    Adc_StartGroupConversion(ADC_GROUP_RAIN);
    while (Adc_GetGroupStatus(ADC_GROUP_RAIN) == ADC_BUSY);
    Adc_ReadGroup(ADC_GROUP_RAIN, adc_raw);
}

void IoHwAb_Read_SimSpeedAdc(uint16* adc_raw) {
    Adc_StartGroupConversion(ADC_GROUP_POT);
    while (Adc_GetGroupStatus(ADC_GROUP_POT) == ADC_BUSY);
    Adc_ReadGroup(ADC_GROUP_POT, adc_raw);
}