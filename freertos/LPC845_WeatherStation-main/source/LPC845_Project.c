/*******************************************************************************
* Estación Meteorológica
******************************************************************************/

#include <stdio.h>
#include "board.h"
#include "LPC845.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "fsl_Systick_Delay.h"
#include "fsl_i2c.h"
#include "fsl_usart.h"
#include "fsl_swm.h"
#include "fsl_power.h"

#include "fsl_SSD1306_I2C.h"
#include "dht22.h"
#include "eeprom.h"
#include "rtc_ds3231.h"
#include "display.h"
#include "usart.h"

/*******************************************************************************
* DEFINES
******************************************************************************/

#define DELAY_ANTIREBOTE	10
#define TIMEOUT_USART_MS 	2200
#define UPDATE_INTERVAL_MS 	2000
#define BOTON				GPIO, 0, 8
#define LED_ON				GPIO, 0, 16
#define MAX_DISPLAY_COUNT 	10		//Cantidad de refrescos de pantalla


/*******************************************************************************
* VARIABLES
******************************************************************************/
uint16_t temp = 0;
uint16_t hum = 0;
rtc_time_t time;
pantalla_t pantalla_actual = PANTALLA_SENSORES;
_Bool estado_anterior_boton = 0;
uint32_t tiempo_anterior = 0;
typedef void (*pantalla_func_t)(void);
uint8_t display_Powerdown_count = 0;

/*******************************************************************************
* PROTOTIPOS
******************************************************************************/
void i2c_config();
void ctimer_config();
_Bool antiRebote(GPIO_Type *base, uint32_t port, uint32_t pin, _Bool *estado_anterior);
void pantalla_hora(void);
void pantalla_usart(void);
void pantalla_sensores(void);

/*******************************************************************************
* MAIN
******************************************************************************/

int main(void) {

    Systick_Init(SystemCoreClock / 1000);
    i2c_config();
    RTC_intFlag = rtc_alarm_flag();

    if (RTC_intFlag && POWER_GetDeepPowerDownModeFlag()) {
            // El sistema se despertó desde Deep Power-Down usando el pin de Wake-up.
            POWER_ClrDeepPowerDownModeFlag(); // Limpiar el flag después de la detección.

            pantalla_actual = NUM_PANTALLAS;
            display_Powerdown_count = MAX_DISPLAY_COUNT; // no hay cuenta hasta Powerdown (solo guarda valor y se apaga)

    }
    else{
        /* Inicializar display */
        OLED_Init();
        OLED_Refresh();
        OLED_Clear();

        GPIO_PinInit(BOTON, kGPIO_DigitalInput);

        ConfigurarInterrupcionRTC(RTC_PINT_PIN_INT1_SRC, kPINT_PinInt0, rtc_intr_callback);
        rtc_Read_time(&time);


        POWER_EnableWakeupPinForDeepPowerDown(true, true);
        POWER_EnableResetPinForDeepPowerDown(true,true);


        if (rtc_ReadAndClear_OscillatorStopFlag()){
            OLED_Clear();
            OLED_Set_Text(0, 0, 1, "RTC ERROR", 2);
            OLED_Set_Text(0, 20, 1, "Reinicio no esperado", 1);
            OLED_Set_Text(0, 40, 1, "Iniciar RTC", 1);
            OLED_Set_Text(0, 55, 1, "Configurar Fecha/Hora", 1);
            OLED_Refresh();
            Ticks_Delay(5000);
        }

    }

    ctimer_config();	//necesario para el DHT22

    /*****************************************************************************************************/



    /*****************************************************************************************************
    * Provisorio. Se pone direcciones de memoria para probar
    *****************************************************************************************************/

    /*
    datalogger_info data = {
        .startAddress = 0x7FFC,
        .endAddress = 0x7FFC,
        .hours = 0,
        .minutes = 1
    };

    guardar_en_flash(&data);

	*/

    /*****************************************************************************************************/



    /*****************************************************************************************************/

    pantalla_func_t pantallas[NUM_PANTALLAS] = {pantalla_sensores, pantalla_hora, pantalla_usart};

    while (1) {

        if (pantalla_actual < NUM_PANTALLAS) {

        	if (antiRebote(BOTON, &estado_anterior_boton)){
        		pantalla_actual = (pantalla_actual + 1) % NUM_PANTALLAS;
        		display_Powerdown_count = 0;
        	}

            pantallas[pantalla_actual]();
        }

    	if (((uint32_t)(Return_Tick_Value() - tiempo_anterior) >= UPDATE_INTERVAL_MS) && RTC_intFlag && pantalla_actual!=PANTALLA_USART) {
    	    tiempo_anterior = Return_Tick_Value();
    	    rtc_Read_time(&time);
    	    DHT22_Read(&temp, &hum);

    	    uint8_t write_buffer[10];

    	    // Copiar los datos de temperatura y humedad al principio del buffer
    	    memcpy(&write_buffer[0], &temp, 2);
    	    memcpy(&write_buffer[2], &hum, 2);

    	    // Copiar los campos de time que se quieren guardar (excluyendo segundos)
    	    write_buffer[4] = time.minutos;
    	    write_buffer[5] = time.horas;
    	    write_buffer[6] = time.diaSemana;
    	    write_buffer[7] = time.diaMes;
    	    write_buffer[8] = time.mes;
    	    write_buffer[9] = time.año;

    	    // Escribir el buffer completo en la EEPROM
    	    eeprom_write(flash_data->endAddress, write_buffer, sizeof(write_buffer));


    	    actualizar_direcciones(&ram_data, flash_data, sizeof(write_buffer), EEPROM_SIZE);

    	    guardar_en_flash(&ram_data);

    	    rtc_clear_alarm_flags(); // Resetear los flags de las alarmas
    	    RTC_intFlag = false; // Resetear el flag de interrupción del RTC

    	}


        if (!RTC_intFlag && display_Powerdown_count >= MAX_DISPLAY_COUNT) {
        	OLED_Clear();
        	OLED_Refresh();
            POWER_EnterDeepPowerDownMode();
        }



    }
    return 0;
}

/*******************************************************************************
* FUNCIONES
******************************************************************************/
void i2c_config() {
    uint32_t baudRate = 400000; // Tasa de baudios deseada en bits por segundo
    uint32_t frecuency = 12000000; // Frecuencia en Hertz del reloj funcional i2c
    i2c_master_config_t i2config; // Estructura con configuraciones para inicializar el módulo maestro I2C

    CLOCK_Select(kI2C1_Clk_From_MainClk); // Fuente del reloj principal para el periférico i2c

    CLOCK_EnableClock(kCLOCK_Swm); // Habilitar reloj de la matriz de conmutación
    SWM_SetMovablePinSelect(SWM0, kSWM_I2C1_SDA, kSWM_PortPin_P0_0); // Configuración del pin SDA
    SWM_SetMovablePinSelect(SWM0, kSWM_I2C1_SCL, kSWM_PortPin_P0_1); // Configuración del pin SCL
    CLOCK_DisableClock(kCLOCK_Swm); // Deshabilitar reloj de la matriz de conmutación

    I2C_MasterGetDefaultConfig(&i2config); // Obtener la configuración predeterminada de i2c
    i2config.baudRate_Bps = baudRate; // Ajustar la tasa de baudios al oscilador del OLED
    I2C_MasterInit(I2C1, &i2config, frecuency); // Inicialización

}

void ctimer_config() {
    ctimer_config_t config;

    CTIMER_GetDefaultConfig(&config);
    CTIMER_Init(CTIMER0, &config);
    CTIMER0->CTCR = CTIMER_CTCR_CTMODE(0) | CTIMER_CTCR_SELCC(0) | CTIMER_CTCR_ENCC(1);
    CTIMER_RegisterCallBack(CTIMER0, &ctimer_callback_table[0], kCTIMER_SingleCallback);
}



_Bool antiRebote(GPIO_Type *base, uint32_t port, uint32_t pin, _Bool *estado_anterior){
    if (GPIO_PinRead(base, port, pin) == 0 && *estado_anterior == 1) {
    	Ticks_Delay(DELAY_ANTIREBOTE);
    	*estado_anterior = GPIO_PinRead(base, port, pin);
    }
    else if (GPIO_PinRead(base, port, pin) == 1 && *estado_anterior == 0) {
    	Ticks_Delay(DELAY_ANTIREBOTE);
    	if (GPIO_PinRead(base, port, pin) == 1)
    	{
    		*estado_anterior = 1;
    		return true;
    	}
    }
    return false;
}

void pantalla_sensores(void){
    if (((uint32_t)(Return_Tick_Value() - tiempo_anterior) >= UPDATE_INTERVAL_MS) && !RTC_intFlag) {
        tiempo_anterior = Return_Tick_Value();
        rtc_Read_time(&time);
        DHT22_Read(&temp, &hum);
        display_Powerdown_count++;
    }
    display_Sensors();
}

void pantalla_hora(void) {
    if (((uint32_t)(Return_Tick_Value() - tiempo_anterior) >= UPDATE_INTERVAL_MS) && !RTC_intFlag) {
        tiempo_anterior = Return_Tick_Value();
        rtc_Read_time(&time);
        display_Powerdown_count++;
    }
    display_Time();
}

void pantalla_usart(void) {
    OLED_Clear();
    OLED_Set_Text(30, 0, 1, "USART", 3);
    OLED_Set_Text(20, 30, 1, "Esperando", 2);
    OLED_Set_Text(22, 45, 1, "conexion", 2);
    OLED_Refresh();
    Iniciar_USART();

    uint8_t write_buffer;
    uint32_t tiempoUltimaComunicacion;
    _Bool primeraComunicacion = false;	// Flag para indicar si hubo comunicación

    while (pantalla_actual == PANTALLA_USART){

    	if (antiRebote(BOTON, &estado_anterior_boton)){
    		pantalla_actual = (pantalla_actual + 1) % NUM_PANTALLAS;
    	}

    	// Verificar si han pasado mucho tiempo sin recibir datos USART
    	if (primeraComunicacion && ((uint32_t)(Return_Tick_Value() - tiempoUltimaComunicacion) > TIMEOUT_USART_MS)) {
    	    break;
    	}


    	if(usart_data_received){
    		usart_data_received = false;

            // Activar flag e inicializar el tiempo de la primera comunicación
            if (!primeraComunicacion) {
                primeraComunicacion = true;
            }

    		if (dataUsart == PEDIDO_CONFIRMACION){
    			write_buffer = CONFIRMAR;
    			usart1_write(&write_buffer, 1);
    			dataUsart = 0;

    		    OLED_Clear();
    		    OLED_Set_Text(30, 0, 1, "USART", 3);
    		    OLED_Set_Text(5, 30, 1, "Comunicacion", 2);
    		    OLED_Set_Text(22, 45, 1, "iniciada", 2);
    		    OLED_Refresh();
    		}
    		if (dataUsart == DESCONEXION){
    			dataUsart = 0;
    			break;
    		}
    		if (dataUsart == PEDIDO_DATOS_ACTUALES){
    			rtc_Read_time(&time);
    			DHT22_Read(&temp, &hum);
    			enviar_datos_actuales();
    			dataUsart = 0;
    		}
    		if (dataUsart == PEDIDO_DATOS_EEPROM) {
    		    uint8_t datos[10];
    		    uint32_t SubAdress = flash_data->startAddress;

    		    while (SubAdress != flash_data->endAddress) {
    		    	eeprom_Sequential_Random_Read(SubAdress, datos, 10);
    		        usart1_write(datos, 10);

    		        SubAdress += 10;

    		        if (SubAdress >= EEPROM_SIZE) {
    		                SubAdress -= EEPROM_SIZE;
    		        }
    		    }

		        for (int i = 0; i < 10; i++) {
		            datos[i] = 0xFF;
		        }
		        usart1_write(datos, 10);  // Enviar los 10 bytes de 0xFF para indicar el final

    		    dataUsart = 0;
    		}
    		else if(dataUsart == BORRAR_DATOS_EEPROM){

    			memcpy(&ram_data, flash_data, sizeof(datalogger_info));
    			ram_data.startAddress = flash_data->endAddress;

    			guardar_en_flash(&ram_data);

    			write_buffer = CONFIRMAR;
    			usart1_write(&write_buffer, 1);

                dataUsart = 0;
    		}
    		else if(dataUsart == CONFIG_TIME) {
    		    write_buffer = CONFIRMAR;
    		    usart1_write(&write_buffer, 1);  // Enviar la confirmación inicial


    		    NVIC_DisableIRQ(USART1_IRQn);	// Deshabilitar temporalmente la interrupción

    		    // Buffer para almacenar los 7 bytes recibidos (segundos, minutos, horas, día de la semana, día del mes, mes, año)
    		    uint8_t timeData[7];

    		    // Recibir los 7 bytes de tiempo
    		    if (USART_ReadBlocking(USART1, timeData, 7) == kStatus_Success) {

    		        // Configurar el RTC con los datos recibidos
    		        time.segundos = timeData[0];
    		        time.minutos = timeData[1];
    		        time.horas = timeData[2];
    		        time.diaSemana = timeData[3];
    		        time.diaMes = timeData[4];
    		        time.mes = timeData[5];
    		        time.año = timeData[6];

    		        rtc_Set_Time(&time);

        		    write_buffer = CONFIRMAR;
        		    usart1_write(&write_buffer, 1);  // Enviar la confirmación

    		        NVIC_EnableIRQ(USART1_IRQn);	// Habilitar nuevamente la interrupción

    		        dataUsart = 0;  // Resetear la bandera de recepción
    		    }
    		    else{
    		    	write_buffer = ERROR;
    		    	usart1_write(&write_buffer, 1);  // Enviar que hubo error

    		        NVIC_EnableIRQ(USART1_IRQn);	// Habilitar nuevamente la interrupción
    		        dataUsart = 0;  // Resetear la bandera de recepción
    		    }


    		}
    		else if(dataUsart == INICIAR_RTC){

    	        //Inicializar RTC
    	        rtc_init();

    	        //Configuro la alarma para cada minuto
    	        rtc_Set_Alarm2_once_per_minute();
    	        rtc_Enable_Alarm2(true);

                dataUsart = 0;
    		}

    		tiempoUltimaComunicacion = Return_Tick_Value();  // Actualizar tiempo luego de terminar con el pedido

    	}
    }
    USART_Deinit(USART1);
}
