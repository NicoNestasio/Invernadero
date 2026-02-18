#include "fsl_common.h"
#include "fsl_pint.h"
#include "fsl_syscon.h"
#include "fsl_i2c.h"
#include "rtc_ds3231.h"

_Bool RTC_intFlag = false;


uint8_t decimalToBCD(uint8_t valor) {
    return ((valor / 10) << 4) | (valor % 10);
}

uint8_t BCDToDecimal(uint8_t valorBCD) {
    return ((valorBCD >> 4) * 10) + (valorBCD & 0x0F);
}


void rtc_intr_callback(pint_pin_int_t pintr, uint32_t pmatch_status)
{
    RTC_intFlag = true;
}

void ConfigurarInterrupcionRTC(syscon_connection_t pinSrc, pint_pin_int_t pintInt, pint_cb_t callback)
{
    /* Conectar la fuente del pin RTC a la entrada de interrupción del PINT */
    SYSCON_AttachSignal(SYSCON, pintInt, pinSrc);

    /* Inicializar PINT */
    PINT_Init(PINT);

    /* Configurar la interrupción del pin para flanco de bajada */
    PINT_PinInterruptConfig(PINT, pintInt, kPINT_PinIntEnableFallEdge, callback);

    /* Habilitar el callback para la interrupción PINT seleccionada */
    PINT_EnableCallbackByIndex(PINT, pintInt);
}

void rtc_Write(uint16_t sub_addres, uint8_t *data, size_t size){
	i2c_master_transfer_t xfer = {0};

	xfer.data = data;
	xfer.dataSize = size;
	xfer.flags = kI2C_TransferDefaultFlag;
	xfer.slaveAddress = RTC_ADDRESS;
	xfer.direction = kI2C_Write;
	xfer.subaddress = sub_addres;
	xfer.subaddressSize = 1;

	I2C_MasterTransferBlocking(I2C1, &xfer);
}


void rtc_Read(uint16_t sub_addres, uint8_t *data, size_t size){
	i2c_master_transfer_t xfer = {0};

	xfer.data = data;
	xfer.dataSize = size;
	xfer.flags = kI2C_TransferRepeatedStartFlag;
	xfer.slaveAddress = RTC_ADDRESS;
	xfer.direction = kI2C_Read;
	xfer.subaddress = sub_addres;
	xfer.subaddressSize = 1;

	I2C_MasterTransferBlocking(I2C1, &xfer);
}

void rtc_init(){
	uint8_t control [2] = {
			0b00000100,		// | EOSC | BBSQW | CONV | RS2 | RS1 | INTCN | A2IE | A1IE |
			0x00,			// Registro de estado en cero
		};

	rtc_Write(DS3231_CONTROL, control, 2);
}

_Bool rtc_alarm_flag(){
	uint8_t buffer;
	rtc_Read(DS3231_STATUS, &buffer, 1);

	if ((buffer & 0x03) != 0) {  // Verifica si el bit 0 (A1F) o el bit 1 (A2F) están en 1
		return true; // Indicar que hubo una interrupción
	}

	return false;
}

void rtc_Set_Time(rtc_time_t *time) {
    uint8_t buffer[sizeof(rtc_time_t)];

    // Configurar el buffer con los valores de la estructura rtc_time_t
    buffer[0] = time->segundos;
    buffer[1] = time->minutos;
    buffer[2] = time->horas;			//formato 24hs
    buffer[3] = time->diaSemana;
    buffer[4] = time->diaMes;
    buffer[5] = time->mes;				//bit 7 es el siglo
    buffer[6] = time->año;

    // Escribir los datos de tiempo en el RTC DS3231
    rtc_Write(DS3231_TIME, buffer, sizeof(buffer));
}

void rtc_Read_time(rtc_time_t *time) {
    uint8_t buffer[7];

    // Leer los registros del DS3231
    rtc_Read(DS3231_TIME, buffer, sizeof(buffer));

    // Llenar la estructura rtc_tiempo_t con los valores leídos
    time->segundos = buffer[0];
    time->minutos = buffer[1];
    time->horas = buffer[2];
    time->diaSemana = buffer[3];
    time->diaMes = buffer[4];
    time->mes = buffer[5];
    time->año = buffer[6];
}

void rtc_Enable_Alarm1(bool enable){
	uint8_t buffer;
	rtc_Read(DS3231_CONTROL, &buffer, 1);

	if (enable) {
	    buffer |= 0x01;		// A1IE = 1
	} else {
	    buffer &= ~0x01;	// A1IE = 0
	}

	rtc_Write(DS3231_CONTROL, &buffer, 1);
}

void rtc_Enable_Alarm2(bool enable){
	uint8_t buffer;
	rtc_Read(DS3231_CONTROL, &buffer, 1);

	if (enable) {
	    buffer |= 0x02;		// A2IE = 1
	} else {
	    buffer &= ~0x02;	// A2IE = 0
	}

	rtc_Write(DS3231_CONTROL, &buffer, 1);
}

void rtc_Set_Alarm1(rtc_time_t *time){
    uint8_t buffer[sizeof(rtc_time_t)];

    // Configurar el buffer con los valores de la estructura rtc_time_t
    buffer[0] = time->segundos;
    buffer[1] = time->minutos;
    buffer[2] = time->horas;			//formato 24hs
    buffer[3] = time->diaSemana | 0x40;

    // Escribir los datos de tiempo en el RTC DS3231
    rtc_Write(DS3231_ALARM1, buffer, sizeof(buffer));

}

void rtc_Set_Alarm2_once_per_minute(void){
    uint8_t buffer[3];

    buffer[0] = 0x80;
    buffer[1] = 0x80;
    buffer[2] = 0x80;

    rtc_Write(DS3231_ALARM2, buffer, sizeof(buffer));
}


void rtc_clear_alarm_flags(void){
	uint8_t buffer;
	rtc_Read(DS3231_STATUS, &buffer, 1);

	// Limpiar los bits A1F (bit 0) y A2F (bit 1)
	buffer &= 0xFC;
	rtc_Write(DS3231_STATUS, &buffer, 1);
}

_Bool rtc_ReadAndClear_OscillatorStopFlag() {
    uint8_t buffer;
    rtc_Read(DS3231_STATUS, &buffer, 1);

    // Verificar si el bit 7 (Oscillator Stop Flag - OSF) está en 1
    if (buffer & (1 << 7)) {
        buffer &= ~(1 << 7);  // Pone el bit 7 a 0
        rtc_Write(DS3231_STATUS, &buffer, 1);
        return true;
    }

    return false;
}
