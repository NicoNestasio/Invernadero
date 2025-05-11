#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
#include "fsl_power.h"
#include "fsl_swm.h"
#include "fsl_iocon.h"
#include "LPC845.h"
#include "fsl_debug_console.h"
#include "fsl_usart.h"


// Definición de pines para filas y columnas
#define ROW_PIN_1   8   // Fila 1
#define COL_PIN_1   9   // Columna 1
#define COL_PIN_2   29  // Columna 2
#define COL_PIN_3   28  // Columna 3
#define SYS_TICK_FREQ 1000 // Frecuencia para 1 ms de intervalo

volatile uint16_t flag_tick = 0;
uint32_t frequency;

void delay_mseg(int mseg);
void SysTick_Handler(void);

void check_buttons(void) {
    GPIO_PinWrite(GPIO, 0, ROW_PIN_1, 0); // Activa la fila (baja)

    // Lee el estado de las columnas
    if (GPIO_PinRead(GPIO, 0, COL_PIN_1) == 0) {
    	GPIO_PinWrite(GPIO, 1, 2, 1); // LED Rojo Off
    	GPIO_PinWrite(GPIO, 1, 0, 0); // LED Verde On
    	delay_mseg(2000);
    	GPIO_PinWrite(GPIO, 1, 0, 1); // LED Verde On
    	GPIO_PinWrite(GPIO, 0, COL_PIN_1,1);


    }
    if (GPIO_PinRead(GPIO, 0, COL_PIN_2) == 0) {
    	GPIO_PinWrite(GPIO, 1, 2, 1); // LED Rojo Off
    	GPIO_PinWrite(GPIO, 1, 0, 0); // LED Verde On
    	delay_mseg(2000);
    	GPIO_PinWrite(GPIO, 1, 0, 1); // LED Verde On
    	GPIO_PinWrite(GPIO, 0, COL_PIN_2,1);

    }
    if (GPIO_PinRead(GPIO, 0, COL_PIN_3) == 0) {
    	GPIO_PinWrite(GPIO, 1, 2, 1); // LED Rojo Off
    	GPIO_PinWrite(GPIO, 1, 1, 0); // LED Verde On
    	delay_mseg(2000);
    	GPIO_PinWrite(GPIO, 1, 1, 1); // LED Verde On
    	GPIO_PinWrite(GPIO, 0, COL_PIN_3,1);
    }
}

int main(void) {
    BOARD_InitDebugConsole();
    CLOCK_EnableClock(kCLOCK_Swm);

    CLOCK_DisableClock(kCLOCK_Swm);

    CLOCK_Select(kADC_Clk_From_Fro);
    CLOCK_SetClkDivider(kCLOCK_DivAdcClk, 1U);
    CLOCK_Select(kUART1_Clk_From_MainClk);

    frequency = CLOCK_GetFreq(kCLOCK_Fro) / CLOCK_GetClkDivider(kCLOCK_DivAdcClk);

    gpio_pin_config_t out_config = {kGPIO_DigitalOutput, 1};
    GPIO_PortInit(GPIO, 1);
    GPIO_PinInit(GPIO, 1, 2, &out_config); // LED Rojo
    GPIO_PinInit(GPIO, 1, 0, &out_config); // LED Verde
    GPIO_PinInit(GPIO, 1, 1, &out_config); // LED Azul

    gpio_pin_config_t row_config = {kGPIO_DigitalOutput, 1}; // Fila como salida
    gpio_pin_config_t col_config = {kGPIO_DigitalInput, 0};  // Columna como entrada

    GPIO_PortInit(GPIO, 0); // Inicializa el puerto 0
    GPIO_PinInit(GPIO, 0, ROW_PIN_1, &row_config); // Inicializa la fila
    GPIO_PinInit(GPIO, 0, COL_PIN_1, &col_config);  // Inicializa columna 1
    GPIO_PinInit(GPIO, 0, COL_PIN_2, &col_config);  // Inicializa columna 2
    GPIO_PinInit(GPIO, 0, COL_PIN_3, &col_config);  // Inicializa columna 3

    SysTick_Config(SystemCoreClock / SYS_TICK_FREQ);


    while (1) {
    	GPIO_PinWrite(GPIO, 1, 2, 0); // LED Rojo On
        // Activa la fila
        check_buttons();
    }
}
void SysTick_Handler(void) {
    flag_tick++;
}

void delay_mseg(int mseg) {
    flag_tick = 0;
    while (flag_tick < mseg); // Espera hasta que `flag_tick` alcance el valor deseado
}
