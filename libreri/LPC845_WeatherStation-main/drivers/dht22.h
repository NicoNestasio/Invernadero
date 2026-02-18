#ifndef DHT22_H_
#define DHT22_H_

#include <stdint.h>
#include "fsl_ctimer.h"
#include "fsl_swm.h"


/*******************************************************************************
 * Definiciones
 ******************************************************************************/
#define DHT_PERIOD_MS 2000 /*Solo se pueden leer los valores del sensor con 0.5 Hz*/
#define DHT22_Port 0 /*Puerto de datos conectado al DHT22*/
#define DHT22_Pin 6 /*Pin de datos en el GPIO_Port conectado al DHT22*/
#define SWM_PORT_PIN(port, pin) ((swm_port_pin_type_t)((port) * 32U + (pin))) //Macro para combinar el número de puerto y el número de pin

/*******************************************************************************
 * Códigos de error
 ******************************************************************************/
typedef enum {
  DHT22_OK, /*!< OK */
  DHT22_NO_PULLUP, /*!< no hay pull-up presente */
  DHT22_NO_DATA, /*!< no se recibe ningun dato */
  DHT22_BAD_CRC, /*!< CRC incorrecto */
} DHT22_ErrorCode;


/*!
 * \brief Devuelve la cadena de descripción para un código de error dado.
 * \param code Código de error
 * \return Cadena de descripción del código de error
 */
const char *DHT22_GetReturnCodeString(DHT22_ErrorCode code);

/*!
 * \brief Lee los valores de datos del sensor
 * \param temperatureCentigrade Valor de la temperatura, en unidades de 1/100. Ej. 1517 es 15.17°C
 * \param humidityCentipercent Valor de la humedad, en unidades de 1/100. Ej. 3756 es 37.56%
 * \return Código de error
 */
DHT22_ErrorCode DHT22_Read(uint16_t *temperatureCentigrade, uint16_t *humidityCentipercent);

void ctimer_cap0_callback(uint32_t flags);



extern ctimer_callback_t ctimer_callback_table[8]; //Array de punteros a funciones para la devolución de llamada para cada canal
extern uint32_t freq_us;


#endif /* DHT22_H_ */
