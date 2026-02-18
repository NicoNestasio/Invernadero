#ifndef USART_H
#define USART_H

#include "fsl_usart.h"
#include "rtc_ds3231.h"
#include "fsl_swm.h"
#include "fsl_clock.h"

extern _Bool usart_data_received;
extern uint8_t dataUsart;
extern uint16_t temp;
extern uint16_t hum;
extern rtc_time_t time;

#define DESCONEXION 			0XF7
#define INICIAR_RTC 			0XF8
#define CONFIG_TIME			 	0XF9
#define PEDIDO_CONFIRMACION 	0xFA
#define PEDIDO_DATOS_ACTUALES 	0xFC
#define PEDIDO_DATOS_EEPROM 	0xFD
#define BORRAR_DATOS_EEPROM		0XFE

#define CONFIRMAR 				0xFB
#define ERROR					0xFF

#define USART_BUFFER_LENGTH 7  // Definir la longitud del buffer de datos usart


/*!
 * @brief Inicializa USART1.
 *
 * Esta función configura e inicia USART1 para la comunicación serial.
 * Configura los pines de transmisión (TX) y recepción (RX) asociados con USART1,
 * selecciona la fuente de reloj, establece los parámetros de configuración predeterminados,
 * y habilita las interrupciones para la recepción de datos.
 */
void Iniciar_USART(void);

/*!
 * @brief Envía una secuencia de bytes a través de USART1.
 *
 * Esta función transmite una secuencia de bytes utilizando USART1.
 * Envía cada byte de la secuencia hasta completar el número especificado
 * de bytes (`len`). La función espera a que el registro de transmisión
 * esté listo antes de enviar cada byte.
 *
 * @param p Puntero al buffer de bytes que se enviarán.
 * @param len Número de bytes a enviar desde el buffer.
 */
void usart1_write(uint8_t *p, uint8_t len);

/*!
 * @brief Envía los datos actuales de temperatura, humedad y tiempo a través de USART1.
 *
 * Esta función recopila los datos de temperatura y humedad, junto con
 * la información de tiempo (minutos, horas, día de la semana, día del mes, mes y año),
 * y los empaqueta en un buffer. Luego, se envía el buffer completo a través
 * de USART1. La función asegura que los datos se envíen en el formato adecuado
 * para su procesamiento en el receptor.
 *
 * @note La función utiliza el tamaño fijo del buffer (10 bytes) y
 *       se espera que las variables `temp`, `hum` y `time` estén definidas y
 *       contengan los valores adecuados antes de la llamada.
 */
void enviar_datos_actuales(void);

/*!
 * @brief Manejador de interrupción para USART1.
 *
 * Esta función se llama automáticamente cuando ocurre una interrupción en USART1.
 * Lee el byte recibido a través de USART1 y lo almacena en la variable global `dataUsart`.
 * También establece la bandera `usart_data_received` como `true` para indicar que hay datos disponibles.
 */
void USART1_IRQHandler(void);

#endif // USART_H
