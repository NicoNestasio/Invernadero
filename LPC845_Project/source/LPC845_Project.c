#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC845.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
int main(void) {

	gpio_pin_config_t out_config = {kGPIO_DigitalOutput, 1};
	GPIO_PortInit(GPIO, 1);
	GPIO_PinInit(GPIO, 1, 2, &out_config);
	GPIO_PinWrite(GPIO, 1, 2, 0); // Led Rojo On
	GPIO_PinWrite(GPIO, 1, 2, 1); // Led Rojo Off
	GPIO_PinInit(GPIO, 1, 0, &out_config);
	GPIO_PinWrite(GPIO, 1, 0, 0); // Led Verde On
	GPIO_PinWrite(GPIO, 1, 0, 1); // Led Verde Off
	GPIO_PinInit(GPIO, 1, 1, &out_config);
	GPIO_PinWrite(GPIO, 1, 1, 0); // Led Azul On
	GPIO_PinWrite(GPIO, 1, 1, 1); // Led Azul Off


    while(1) {

    }
}
