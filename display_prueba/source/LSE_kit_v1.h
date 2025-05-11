#include "board.h"

//---------------------------------------------------------------//
#define LED_ON 				0
#define LED_OFF 			1

#define RED_LED_ON 			GPIO_PinWrite(GPIO, 1, 2, 0)
#define RED_LED_OFF 		GPIO_PinWrite(GPIO, 1, 2, 1)
#define GREEN_LED_ON 		GPIO_PinWrite(GPIO, 1, 0, 0)
#define GREEN_LED_OFF 		GPIO_PinWrite(GPIO, 1, 0, 1)
#define BLUE_LED_ON 		GPIO_PinWrite(GPIO, 1, 1, 0)
#define BLUE_LED_OFF 		GPIO_PinWrite(GPIO, 1, 1, 1)
#define BUZZER_ON 			GPIO_PinWrite(GPIO, 0, 28, 0)
#define BUZZER_OFF 			GPIO_PinWrite(GPIO, 0, 28, 1)

#define RED_LED_STATUS		GPIO_PinRead(GPIO, 1, 2)
#define GREEN_LED_STATUS	GPIO_PinRead(GPIO, 1, 0)
#define BLUE_LED_STATUS		GPIO_PinRead(GPIO, 1, 1)

#define SW_USER	 			GPIO_PinRead(GPIO,0,4)
#define SW_ISP	 			GPIO_PinRead(GPIO,0,12)
#define SW_S1	 			GPIO_PinRead(GPIO,0,16)
#define SW_S2	 			GPIO_PinRead(GPIO,0,25)

#define LCD_RS(x)			GPIO_PinWrite(GPIO, 0, 18, x)
#define LCD_RW(x)			GPIO_PinWrite(GPIO, 0, 19, x)
#define LCD_E(x)			GPIO_PinWrite(GPIO, 0, 20, x)
#define LCD_DB4(x)			GPIO_PinWrite(GPIO, 0, 25, x)
#define LCD_DB5(x)			GPIO_PinWrite(GPIO, 0, 26, x)
#define LCD_DB6(x)			GPIO_PinWrite(GPIO, 0, 27, x)
#define LCD_DB7(x)			GPIO_PinWrite(GPIO, 0, 28, x)
#define LCD_BL(x)			GPIO_PinWrite(GPIO, 0, 0, x)

//---------------------------------------------------------------//

void Write_7segment(uint8_t num, uint8_t dp, uint8_t digit);
void gpio_init_lse_kit(void);
//---------------------------------------------------------------//
