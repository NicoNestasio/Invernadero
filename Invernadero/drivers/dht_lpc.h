/*
 * dht_lpc.h
 * Adaptación de la librería DHT para LPC
 *
 * Esta es la versión correcta que coincide con
 * dht_lpc.c (versión optimizada bare-metal).
 */

#ifndef DHT_LPC_H_
#define DHT_LPC_H_

#include <stdint.h>
#include <stdbool.h>

// --- Tipos de Sensor ---
#define DHT11 11
#define DHT22 22

// --- Códigos de Error ---
#define DHT_OK              0
#define DHT_ERR_TIMEOUT     -1 // Error de Timeout (el sensor no respondió)
#define DHT_ERR_CHECKSUM    -2 // Error de Checksum (datos corruptos)

// --- Funciones Públicas ---

/**
 * @brief Inicializa el timer MRT (requerido para los delays)
 */
void dht_timer_init(void);

/**
 * @brief Inicializa el sensor DHT
 * @param port  Puerto GPIO (ej. 0 para PIO0)
 * @param pin   Pin GPIO (ej. 10 para PIO0_10)
 * @param type  Tipo de sensor (DHT11 o DHT22)
 */
void dht_init(uint8_t port, uint8_t pin, uint8_t type);

/**
 * @brief Lee el sensor. Los datos se guardan internamente.
 * @return DHT_OK (0) si la lectura fue exitosa, o un código de error.
 */
int dht_read(void); // <--- ESTA ES LA DECLARACIÓN CORRECTA (coincide con el .c)

/**
 * @brief Obtiene la humedad (solo llamar después de un dht_read() exitoso)
 * @return Humedad en %RH
 */
float dht_get_humidity(void);

/**
 * @brief Obtiene la temperatura (solo llamar después de un dht_read() exitoso)
 * @return Temperatura en *C
 */
float dht_get_temperature(void);

#endif /* DHT_LPC_H_ */
