#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC845.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "fsl_Systick_Delay.h"

/*
 * PROGRAMA DE PRUEBA DE MOTOR / BOMBA DE AGUA
 * * Hardware:
 * - Pin: PIO0_16 (High-Drive para máxima corriente al transistor)
 * - Transistor: 2N2222 con resistencia de 200-330 ohms en la base.
 * - Diodo: 1N4007 en paralelo con el motor.
 */

#define MOTOR_PORT 0
#define MOTOR_PIN  16

int main(void) {
    // 1. Inicialización básica del sistema y consola de depuración
    BOARD_InitDebugConsole();

    // Inicializar el System Tick para retardos (asumiendo 1ms por tick)
    Systick_Init(SystemCoreClock / 1000);

    // 2. Configuración del Reloj y GPIO
    // Habilitar el reloj para el puerto 0
    GPIO_PortInit(GPIO, 0);

    // Definir configuración del pin: Salida digital, iniciando en 0 (apagado)
    gpio_pin_config_t motor_config = {
        kGPIO_DigitalOutput,
        0
    };

    // Inicializar el pin físico PIO0_16
    GPIO_PinInit(GPIO, MOTOR_PORT, MOTOR_PIN, &motor_config);

    PRINTF("--- INICIANDO TEST DE MOTOR (PIO0_16) ---\r\n");
    PRINTF("Configuracion: 3 segundos ENCENDIDO / 3 segundos APAGADO\r\n");

    while (1) {
        // --- ENCENDER ---
        PRINTF("Motor: [ ON ]\r\n");
        GPIO_PinWrite(GPIO, MOTOR_PORT, MOTOR_PIN, 1);
        Ticks_Delay(3000); // Espera 3000 milisegundos

        // --- APAGAR ---
        PRINTF("Motor: [ OFF ]\r\n");
        GPIO_PinWrite(GPIO, MOTOR_PORT, MOTOR_PIN, 0);
        Ticks_Delay(3000);
    }

    return 0;
}
