#include "teclado.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#include <stdbool.h>

static const char mapa_teclado[FILAS][COLUMNAS] = {
    {'D','C','B','A'},
    {'#','9','6','3'},
    {'0','8','5','2'},
    {'*','7','4','1'}
};

void Teclado_Init(void)
{
    gpio_pin_config_t pinOut = {kGPIO_DigitalOutput, 1};
    gpio_pin_config_t pinIn  = {kGPIO_DigitalInput, 0};

    //GPIO_PortInit(GPIO, 0);

    for (int i = 0; i < FILAS; i++) {
        GPIO_PinInit(GPIO, 0, filas[i], &pinOut);
        GPIO_PinWrite(GPIO, 0, filas[i], 1); // default en alto
    }

    for (int j = 0; j < COLUMNAS; j++) {
        GPIO_PinInit(GPIO, 0, columnas[j], &pinIn);
    }
}

char Teclado_Scan(void)
{
    for (int i = 0; i < FILAS; i++) {
        // Poner en bajo la fila activa
        GPIO_PinWrite(GPIO, 0, filas[i], 0);

        for (int j = 0; j < COLUMNAS; j++) {
            if (GPIO_PinRead(GPIO, 0, columnas[j]) == 0) {
                // Esperar a que se suelte
                while (GPIO_PinRead(GPIO, 0, columnas[j]) == 0);
                GPIO_PinWrite(GPIO, 0, filas[i], 1); // restaurar
                return mapa_teclado[i][j];
            }
        }

        GPIO_PinWrite(GPIO, 0, filas[i], 1); // restaurar
    }
    return 0;
}
