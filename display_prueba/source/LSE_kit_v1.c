#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC845.h"
#include "LSE_kit_v1.h"

//---------------------------------------------------------------//
void gpio_init_lse_kit(void){
    gpio_pin_config_t out_config = {kGPIO_DigitalOutput, 1};
    gpio_pin_config_t in_config = {kGPIO_DigitalInput};

    GPIO_PortInit(GPIO, 0);
    GPIO_PortInit(GPIO, 1);
    GPIO_PinInit(GPIO, 1, 0, &out_config);
    GPIO_PinInit(GPIO, 1, 1, &out_config);
    GPIO_PinInit(GPIO, 1, 2, &out_config);
    GPIO_PinInit(GPIO, 0, 28, &out_config);

    GPIO_PinInit(GPIO, 0, 11, &out_config);
    GPIO_PinInit(GPIO, 0, 10, &out_config);
    GPIO_PinInit(GPIO, 0, 6, &out_config);
    GPIO_PinInit(GPIO, 0, 0, &out_config);
    GPIO_PinInit(GPIO, 0, 14, &out_config);
    GPIO_PinInit(GPIO, 0, 13, &out_config);
    GPIO_PinInit(GPIO, 0, 15, &out_config);
    GPIO_PinInit(GPIO, 0, 1, &out_config);
    GPIO_PinInit(GPIO, 0, 8, &out_config);
    GPIO_PinInit(GPIO, 0, 9, &out_config);

    GPIO_PinInit(GPIO, 0, 4, &in_config);
    GPIO_PinInit(GPIO, 0, 12, &in_config);
    GPIO_PinInit(GPIO, 0, 16, &in_config);
    GPIO_PinInit(GPIO, 0, 25, &in_config);

    GREEN_LED_OFF;
    BLUE_LED_OFF;
    RED_LED_OFF;
    BUZZER_OFF;
    GPIO_PinWrite(GPIO, 0, 8, 1);
    GPIO_PinWrite(GPIO, 0, 9, 1);
    LCD_DB7(0);
    LCD_DB6(0);
    LCD_DB5(0);
    LCD_DB4(0);
    LCD_E(0);
    LCD_RS(0);
    LCD_RW(0);
    LCD_BL(1);
}
//---------------------------------------------------------------//
void Write_7segment(uint8_t num, uint8_t dp, uint8_t digit){
	union {
		uint8_t segments;
		struct{
			uint8_t a:1;
			uint8_t b:1;
			uint8_t c:1;
			uint8_t d:1;
			uint8_t e:1;
			uint8_t f:1;
			uint8_t g:1;
			uint8_t dp:1;
		} segment;
	} lcd;
							// 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   .
	uint8_t tabla_7seg[12]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x10,0x7F};
	lcd.segments = tabla_7seg[num];
	lcd.segment.dp = dp;
	GPIO_PinWrite(GPIO, 0, 10, lcd.segment.a);
	GPIO_PinWrite(GPIO, 0, 11, lcd.segment.b);
	GPIO_PinWrite(GPIO, 0, 6, lcd.segment.c);
	GPIO_PinWrite(GPIO, 0, 14, lcd.segment.d);
	GPIO_PinWrite(GPIO, 0, 0, lcd.segment.e);
	GPIO_PinWrite(GPIO, 0, 13, lcd.segment.f);
	GPIO_PinWrite(GPIO, 0, 15, lcd.segment.g);
	GPIO_PinWrite(GPIO, 0, 1, lcd.segment.dp);
	if(digit == 1){
		GPIO_PinWrite(GPIO, 0, 8, 1);
		GPIO_PinWrite(GPIO, 0, 9, 0);
	}
	if(digit == 2){
		GPIO_PinWrite(GPIO, 0, 8, 0);
		GPIO_PinWrite(GPIO, 0, 9, 1);
	}

}
//---------------------------------------------------------------//
