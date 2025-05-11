//---------------------------------------------------------------//
// includes
//---------------------------------------------------------------//
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC845.h"
#include "fsl_debug_console.h"
#include "LSE_kit_v1.h"
#include "lcd_char.h"

//---------------------------------------------------------------//
// defines
//---------------------------------------------------------------//
#define TIME_XMSEG		20
//---------------------------------------------------------------//
// Variables
//---------------------------------------------------------------//
long i;
uint8_t dig1=0, dig2=0, flag=0, count_xmseg = TIME_XMSEG, count = 0;
uint32_t count_mseg;
uint16_t data;
uint8_t line[20];

//---------------------------------------------------------------//
// Prototypes
//---------------------------------------------------------------//
void Delay_mseg(uint32_t ms);

//---------------------------------------------------------------//
// main
//---------------------------------------------------------------//
int main(void) {

    BOARD_InitDebugConsole();
    (void) SysTick_Config(SystemCoreClock / 1000);

    gpio_init_lse_kit();
	lcd_char_init4();
	line_lcd4("  Hola mundo1   ",1);
    line_lcd4("  Hola mundo2   ",2);

    while(1) {
    	PRINTF("Ingrese un numero\n");
    	SCANF("%d",&data);
    	PRINTF("Ingreso: %d\n", data);
    	sprintf(line,"Ingreso: %d    ", data);
        line_lcd4(line,2);

    }
}
//---------------------------------------------------------------//
// interrupciones
//---------------------------------------------------------------//
void SysTick_Handler(void)
{
    if (count_mseg != 0) count_mseg--;
}

//---------------------------------------------------------------//
// funciones
//---------------------------------------------------------------//
void Delay_mseg(uint32_t ms)
{
	count_mseg = ms;
    while(count_mseg != 0);
}
//---------------------------------------------------------------//
