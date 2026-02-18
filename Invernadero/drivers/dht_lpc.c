/*
 * dht_lpc.c
 * Adaptación de la librería DHT para LPC
 *
 * VERSIÓN OPTIMIZADA (BARE-METAL)
 * Esta versión usa acceso directo a registros GPIO (GPIO->PIN)
 * en lugar de GPIO_PinRead() para ser lo suficientemente rápida
 * para el "bit-banging".
 */

#include "dht_lpc.h"
#include "fsl_gpio.h"
#include "fsl_mrt.h"
#include "fsl_Systick_Delay.h" // Para Ticks_Delay (ms)
#include "fsl_common.h"        // Para __disable_irq
#include <stdbool.h>
#include <math.h> // <--- ¡¡AÑADIDO PARA CORREGIR EL ERROR DE 'NAN'!!

// --- Variables Globales Privadas ---
static uint8_t g_dht_port;
static uint8_t g_dht_pin;
static uint8_t g_dht_type;
static uint8_t g_dht_data[5]; // Buffer para los 40 bits
static uint32_t g_dht_maxcycles; // Máximo de ciclos para timeout

// --- Funciones de Timer (MRT para µs) ---

void dht_timer_init(void) {
    CLOCK_EnableClock(kCLOCK_Mrt);
    mrt_config_t mrtConfig;
    MRT_GetDefaultConfig(&mrtConfig);
    MRT_Init(MRT0, &mrtConfig);
    // No habilitamos interrupciones, usaremos polling
}

// Delay en milisegundos (usa Systick)
static void dht_delay_ms(uint32_t ms) {
    Ticks_Delay(ms);
}

// Delay en microsegundos (usa MRT por Polling)
static void dht_delay_us(uint32_t us) {
    if (us == 0) return;

    // Calcular ticks basados en el reloj (30MHz)
    // (us * 30) - 1
    uint32_t ticks = (us * (SystemCoreClock / 1000000U)) - 1;

    MRT_SetupChannelMode(MRT0, kMRT_Channel_0, kMRT_OneShotMode);
    MRT_StartTimer(MRT0, kMRT_Channel_0, ticks);

    // Esperar por Polling (sin interrupción)
    while (!(MRT_GetStatusFlags(MRT0, kMRT_Channel_0) & kMRT_TimerInterruptFlag)) {
    }
    MRT_ClearStatusFlags(MRT0, kMRT_Channel_0, kMRT_TimerInterruptFlag);
}

// --- Funciones de Pin (Optimizadas) ---

// Configura la dirección del pin (Input/Output)
static void _dht_set_pin_dir(bool output) {
    if (output) {
        gpio_pin_config_t out_config = {kGPIO_DigitalOutput, 1};
        GPIO_PinInit(GPIO, g_dht_port, g_dht_pin, &out_config);
    } else {
        gpio_pin_config_t in_config = {kGPIO_DigitalInput, 0};
        GPIO_PinInit(GPIO, g_dht_port, g_dht_pin, &in_config);
    }
}

// Escribe en el pin (solo cuando es salida)
static void _dht_write_pin(bool level) {
    GPIO_PinWrite(GPIO, g_dht_port, g_dht_pin, level);
}

// *** LA CLAVE ESTÁ AQUÍ ***
// Lee el pin RÁPIDAMENTE usando acceso directo al registro
static inline bool _dht_read_pin_fast(void) {
    // GPIO->PIN[port] lee el estado de todos los pines del puerto
    // Hacemos un AND con la máscara de nuestro pin
    return (GPIO->PIN[g_dht_port] & (1 << g_dht_pin)) != 0;
}


// --- Lógica del Protocolo ---

// Mide la duración de un pulso (ALTO o BAJO)
// Devuelve un contador de bucles, o 0 si hay timeout
static uint32_t _dht_expect_pulse(bool level) {
    uint32_t counter = 0;

    // g_dht_maxcycles se calcula en dht_init()
    // Espera a que el pin esté en el 'level' esperado
    // Usamos la lectura RÁPIDA del pin
    while (_dht_read_pin_fast() == level) {
        if (counter++ >= g_dht_maxcycles) {
            return 0; // Error de Timeout
        }
    }
    return counter; // Devuelve el número de bucles que duró el pulso
}

// --- Funciones Públicas ---

void dht_init(uint8_t port, uint8_t pin, uint8_t type) {
    g_dht_port = port;
    g_dht_pin = pin;
    g_dht_type = type;

    // Calcular maxcycles para 1ms (nuestro timeout de pulso)
    // 1ms a 30MHz (SystemCoreClock)
    // El número de ciclos de CPU para nuestro bucle while
    // (Asumimos ~6 ciclos por bucle: leer, comparar, incrementar, saltar)
    g_dht_maxcycles = (SystemCoreClock / 1000U) / 6U;

    // Poner el pin en ALTO (estado de reposo)
    _dht_set_pin_dir(true);
    _dht_write_pin(true);
}

int dht_read(void) {
    // Limpiar el buffer de datos
    g_dht_data[0] = g_dht_data[1] = g_dht_data[2] = g_dht_data[3] = g_dht_data[4] = 0;

    // --- 1. Pulso de START ---
    _dht_set_pin_dir(true); // Modo Salida
    _dht_write_pin(false);   // Pin BAJO

    // Delay de 18ms (como el foro) o 1.1ms (Adafruit)
    // El DHT22 (AM2302) usa 1.1ms (1100us)
    // El DHT11 usa 18ms
    // Vamos a usar el del foro, 18ms
    dht_delay_ms(18);

    // --- 2. Preparar para escuchar ---

    // Deshabilitar interrupciones (¡CRÍTICO!)
    uint32_t primask = __get_PRIMASK();
    __disable_irq();

    _dht_set_pin_dir(false); // Modo Entrada
    dht_delay_us(30);        // Delay de 30us para que el sensor responda

    // --- 3. Leer Respuesta (ACK) del Sensor ---
    // Esperar pulso BAJO de ~80us
    if (_dht_expect_pulse(false) == 0) {
        __set_PRIMASK(primask); // Reactivar interrupciones
        return DHT_ERR_TIMEOUT; // Falló (Timeout)
    }
    // Esperar pulso ALTO de ~80us
    if (_dht_expect_pulse(true) == 0) {
        __set_PRIMASK(primask); // Reactivar interrupciones
        return DHT_ERR_TIMEOUT; // Falló (Timeout)
    }

    // --- 4. Leer los 40 bits de datos ---
    uint32_t cycles[80];
    for (int i = 0; i < 80; i += 2) {
        cycles[i]   = _dht_expect_pulse(false); // Pulso BAJO (50us)
        cycles[i+1] = _dht_expect_pulse(true);  // Pulso ALTO (28us o 70us)

        if (cycles[i] == 0 || cycles[i+1] == 0) {
            __set_PRIMASK(primask); // Reactivar interrupciones
            return DHT_ERR_TIMEOUT; // Falló (Timeout)
        }
    }

    // Reactivar interrupciones (ya terminamos la parte crítica)
    __set_PRIMASK(primask);

    // --- 5. Procesar los bits ---
    for (int i = 0; i < 40; ++i) {
        uint32_t lowCycles = cycles[2 * i];
        uint32_t highCycles = cycles[2 * i + 1];

        // Shift a la izquierda para hacer espacio al nuevo bit
        g_dht_data[i / 8] <<= 1;

        // Lógica de Adafruit: si el pulso ALTO es más largo
        // que el pulso BAJO, es un '1'.
        if (highCycles > lowCycles) {
            g_dht_data[i / 8] |= 1;
        }
    }

    // --- 6. Verificar Checksum ---
    uint8_t checksum = (g_dht_data[0] + g_dht_data[1] + g_dht_data[2] + g_dht_data[3]) & 0xFF;
    if (checksum == g_dht_data[4]) {
        return DHT_OK;
    } else {
        return DHT_ERR_CHECKSUM;
    }
}

float dht_get_humidity(void) {
    float f = NAN;
    if (g_dht_type == DHT22) {
        f = ((uint16_t)g_dht_data[0] << 8) | g_dht_data[1];
        f *= 0.1;
    } else { // DHT11
        f = (float)g_dht_data[0];
    }
    return f;
}

float dht_get_temperature(void) {
    float f = NAN;
    if (g_dht_type == DHT22) {
        f = ((uint16_t)(g_dht_data[2] & 0x7F) << 8) | g_dht_data[3];
        f *= 0.1;
        if (g_dht_data[2] & 0x80) { // Signo negativo
            f *= -1.0;
        }
    } else { // DHT11
        f = (float)g_dht_data[2];
    }
    return f;
}
