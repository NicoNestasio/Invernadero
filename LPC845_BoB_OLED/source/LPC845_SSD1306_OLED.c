#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_gpio.h"

#define PIN_30 30U
#define PIN_31 31U
#define GPIO_PORT GPIO
#define GPIO_PIN_MASK_30 (1U << PIN_30)
#define GPIO_PIN_MASK_31 (1U << PIN_31)

// Pequeño delay
void delay(void) {
    for (volatile int i = 0; i < 500000; i++);
}

int main(void) {
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    // Configurar ambos pines como GPIO salida
    gpio_pin_config_t pinConfig = {
        kGPIO_DigitalOutput,
        0,
    };

    GPIO_PortInit(GPIO_PORT, 0); // Asegura la inicialización del puerto

    GPIO_PinInit(GPIO_PORT, 0, PIN_30, &pinConfig);
    GPIO_PinInit(GPIO_PORT, 0, PIN_31, &pinConfig);

    while (1) {
        // Enciende ambos (HIGH)
        GPIO_PortSet(GPIO_PORT, 0, GPIO_PIN_MASK_30 | GPIO_PIN_MASK_31);
        delay();

        // Apaga ambos (LOW)
        GPIO_PortClear(GPIO_PORT, 0, GPIO_PIN_MASK_30 | GPIO_PIN_MASK_31);
        delay();
    }
}

