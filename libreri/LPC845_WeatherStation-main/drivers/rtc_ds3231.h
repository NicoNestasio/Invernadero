#ifndef RTC_DS3231_H_
#define RTC_DS3231_H_

#include "fsl_pint.h"
#include "fsl_syscon.h"

#define RTC_ADDRESS 	0x68
#define DS3231_TIME		0x00		// Registro hora
#define DS3231_ALARM1	0x07		// Registro alarma 1
#define DS3231_ALARM2	0x0B		// Registro alarma 2
#define DS3231_CONTROL	0x0E		// Registro control
#define DS3231_STATUS	0x0F		// Registro estado
#define RTC_PINT_PIN_INT1_SRC kSYSCON_GpioPort0Pin4ToPintsel	//Pin interrupcion del RTC

// Estructura para el tiempo en formato BCD
typedef struct {
    uint8_t segundos;  // Segundos en BCD (00-59)
    uint8_t minutos;   // Minutos en BCD (00-59)
    uint8_t horas;     // Horas en BCD (00-23)
    uint8_t diaSemana; // Día de la semana en BCD (01-07)
    uint8_t diaMes;    // Día del mes en BCD (01-31)
    uint8_t mes;       // Mes en BCD (01-12)
    uint8_t año;       // Año en BCD (00-99)
} rtc_time_t;

extern _Bool RTC_intFlag;

/*!
 * @brief Convierte un valor decimal a BCD (Binary-Coded Decimal).
 *
 * Esta función toma un valor decimal (de 0 a 99) y lo convierte a su
 * representación en BCD.
 *
 * @param val Valor decimal a convertir.
 * @return Valor convertido en BCD.
 */
uint8_t decimalToBCD(uint8_t valor);

/*!
 * @brief Convierte un valor en BCD (Binary-Coded Decimal) a decimal.
 *
 * Esta función toma un valor en formato BCD (de 0x00 a 0x99) y lo convierte a su
 * representación decimal correspondiente.
 *
 * @param valorBCD Valor en BCD a convertir.
 * @return Valor convertido en decimal.
 */
uint8_t BCDToDecimal(uint8_t valorBCD);




void rtc_intr_callback(pint_pin_int_t pintr, uint32_t pmatch_status);

/*!
 * @brief Configura una interrupción para el pin asociado al RTC.
 *
 * Esta función conecta el pin del RTC a la entrada de interrupción del PINT,
 * inicializa el módulo PINT, configura la interrupción para detectar un flanco
 * de bajada y habilita el callback correspondiente.
 *
 * @param pinSrc Fuente de la señal de pin a conectar (por ejemplo, `kSYSCON_GpioPort0Pin12ToPintsel`).
 * @param pintInt Número de interrupción PINT a usar (por ejemplo, `kPINT_PinInt0`).
 * @param callback Función de callback que se llamará cuando ocurra la interrupción.
 */
void ConfigurarInterrupcionRTC(syscon_connection_t pinSrc, pint_pin_int_t pintInt, pint_cb_t callback);

/*!
 * @brief Escribe los registros del RTC.
 *
 * Esta función escribe a partir de la subdirección especificada.
 *
 * @param sub_address La direccion donde se escribirán los datos.
 * @param data Puntero a los datos que se van a escribir.
 * @param size El número de bytes a escribir.
 */
void rtc_Write(uint16_t sub_addres, uint8_t *data, size_t size);


/*!
 * @brief Lee los registros del RTC.
 *
 * Esta función lee los registros a partir de la subdirección especificada.
 *
 * @param sub_address La direccion desde donde comenzará la operación de lectura.
 * @param data Puntero al buffer donde se almacenarán los datos leídos.
 * @param size El número de bytes a leer.
 */
void rtc_Read(uint16_t sub_addres, uint8_t *data, size_t size);


/*!
 * @brief Inicializa los registros de control y estado del RTC DS3231.
 *
 * - Oscillator Stop Flag (OSF) se pone en cero.
 * - 32kHz Output deshabilitado.
 * - Esta función configura los registros de control y estado del RTC, pero no habilita las alarmas.
 *
 */
void rtc_init(void);

/*!
 * @brief Lee los flags de alarma del RTC.
 *
 * - Verifica si hubo una interrupción (alarma) mientras el microcontrolador estuvo apagado,
 *   comprobando los bits de las alarmas (A1F y A2F) en el registro de estado del RTC DS3231.
 *
 * @return true si hubo una interrupción (alarma), false en caso contrario.
 */
_Bool rtc_alarm_flag(void);


/*!
 * @brief Configura la hora en el RTC DS3231.
 *
 * Esta función toma un puntero a una estructura rtc_time_t que contiene
 * los valores de tiempo que se desean establecer en el RTC y los escribe
 * en los registros correspondientes del RTC DS3231.
 *
 * @param time Puntero a la estructura rtc_time_t que contiene la hora actual.
 */
void rtc_Set_Time(rtc_time_t *time);

/*!
 * @brief Lee la hora actual del RTC DS3231.
 *
 * Esta función lee los registros de tiempo del RTC DS3231 y llena una
 * estructura rtc_time_t con los valores leídos. Los valores leídos incluyen
 * segundos, minutos, horas, día de la semana, día del mes, mes y año.
 *
 * @param time Puntero a la estructura rtc_time_t donde se almacenarán los valores leídos.
 */
void rtc_Read_time(rtc_time_t *time);

/**
 * @brief Habilita la alarma 1 en el RTC DS3231.
 *
 * Esta función habilita la alarma 1 en el módulo RTC DS3231 al establecer el bit A1IE (Alarm 1 Interrupt Enable)
 * del registro de control a 1. El bit A1IE se encuentra en el bit menos significativo del registro de control.
 *
 * * @param enable Un valor booleano que determina si la alarma 1 se habilita (true) o se deshabilita (false).
 *
 */
void rtc_Enable_Alarm1(bool enable);

/**
 * @brief Habilita la alarma 2 en el RTC DS3231.
 *
 * Esta función habilita la alarma 2 en el módulo RTC DS3231 al establecer el bit A2IE (Alarm 2 Interrupt Enable)
 * del registro de control a 1.
 *
 * * @param enable Un valor booleano que determina si la alarma 2 se habilita (true) o se deshabilita (false).
 *
 */
void rtc_Enable_Alarm2(bool enable);

/*!
 * @brief Configura la Alarma 1 en el RTC DS3231.
 *
 * Esta función configura la Alarma 1 del RTC DS3231. La alarma se activa
 * cuando los segundos, minutos, horas y día de la semana coinciden con los
 * valores especificados en la estructura rtc_time_t. El día de la semana
 * se establece con el prefijo 0b0100XXXX para indicar que debe coincidir el
 * día de la semana y no la fecha.
 *
 * @param time Puntero a la estructura rtc_time_t que contiene los valores de tiempo para la alarma.
 */
void rtc_Set_Alarm1(rtc_time_t *time);

/*!
 * @brief Configura la Alarma 2 para activarse una vez por minuto en el RTC DS3231.
 *
 * Esta función configura la Alarma 2 del RTC DS3231 para que se active
 * una vez por minuto. Para ello, se establece el valor 0x80 en los registros
 * de minutos, horas y día de la semana.
 */
void rtc_Set_Alarm2_once_per_minute(void);


/*!
 * @brief Restablece los flags de alarma del RTC DS3231.
 *
 * Esta función limpia los flags de alarma en el registro de estado del RTC DS3231.
 * El registro de estado contiene dos flags de alarma: A1F (Alarm 1 Flag) y A2F (Alarm 2 Flag).
 *
 * Esta función lee el registro de estado del RTC, borra los flags de alarma (estableciendo
 * los bits correspondientes a 0) y luego escribe el valor modificado de vuelta al registro.
 *
 */
void rtc_clear_alarm_flags(void);

/*!
 * @brief Lee y limpia el Oscillator Stop Flag (OSF) del RTC DS3231.
 *
 * Esta función verifica el estado del Oscillator Stop Flag (OSF) en el registro de estado del RTC DS3231,
 * el cual indica si el oscilador dejó de funcionar en algún momento (por ejemplo si la batería se agotó).
 *
 * Si el OSF está activado (bit 7 del registro de estado), la función lo limpia estableciendo ese bit en 0
 * y luego escribe el valor modificado de vuelta al registro.
 *
 * @return true si el OSF estaba activado (bit 7 en 1), false si no estaba activado (bit 7 en 0).
 */
_Bool rtc_ReadAndClear_OscillatorStopFlag(void);

#endif /* RTC_DS3231_H_ */
