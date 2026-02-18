/*******************************************************************************
* Test DHT22 - LPC845
*******************************************************************************/

#include <stdio.h>
#include "board.h"
#include "LPC845.h"
#include "clock_config.h"
#include "pin_mux.h"

#include "fsl_debug_console.h"
#include "fsl_Systick_Delay.h"
#include "fsl_ctimer.h"

#include "dht22.h"

/*******************************************************************************
* DEFINES
*******************************************************************************/
#define UPDATE_INTERVAL_MS 2000

/*******************************************************************************
* VARIABLES
*******************************************************************************/
uint16_t temp = 0;
uint16_t hum  = 0;
uint32_t tiempo_anterior = 0;

/*******************************************************************************
* PROTOTIPOS
*******************************************************************************/
void ctimer_config(void);

/*******************************************************************************
* MAIN
*******************************************************************************/
int main(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* SysTick a 1 ms */
    Systick_Init(SystemCoreClock / 1000);

    /* CTIMER necesario para el DHT22 */
    ctimer_config();

    PRINTF("\r\n*** Test DHT22 iniciado ***\r\n");

    while (1)
    {
        if ((uint32_t)(Return_Tick_Value() - tiempo_anterior) >= UPDATE_INTERVAL_MS)
        {
            tiempo_anterior = Return_Tick_Value();

            if (DHT22_Read(&temp, &hum))
            {
                PRINTF("Temperatura: %d.%d C | Humedad: %d.%d %%\r\n",
                       temp / 10, temp % 10,
                       hum  / 10, hum  % 10);
            }
            else
            {
                PRINTF("Error leyendo DHT22\r\n");
            }
        }
    }
}

/*******************************************************************************
* FUNCIONES
*******************************************************************************/
void ctimer_config(void)
{
    ctimer_config_t config;

    CTIMER_GetDefaultConfig(&config);
    CTIMER_Init(CTIMER0, &config);

    /* Modo timer */
    CTIMER0->CTCR =
        CTIMER_CTCR_CTMODE(0) |
        CTIMER_CTCR_SELCC(0)  |
        CTIMER_CTCR_ENCC(1);

    CTIMER_RegisterCallBack(CTIMER0,
                            &ctimer_callback_table[0],
                            kCTIMER_SingleCallback);
}
