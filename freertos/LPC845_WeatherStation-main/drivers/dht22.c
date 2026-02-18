#include "dht22.h"
#include "board.h"
#include "fsl_Systick_Delay.h"

/********************************************************************************/

volatile uint32_t pulse_capture[40]; 		//Captura el valor de cada pulso alto
volatile int8_t pulse; 						//Contador de pulsos
uint32_t timeout_ms = 6; 				// Configurar temporizador para controlar el tiempo máximo de espera

ctimer_callback_t ctimer_callback_table[] = {ctimer_cap0_callback, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

/********************************************************************************/

const char *DHT22_GetReturnCodeString(DHT22_ErrorCode code) {
  switch(code) {
    case DHT22_OK: return "OK";
    case DHT22_NO_PULLUP: return "NO_PULLUP";
    case DHT22_NO_DATA: return "NO_DATA";
    case DHT22_BAD_CRC: return "BAD_CRC";
    default: return "UNKNOWN?";
  }
}


DHT22_ErrorCode DHT22_Read(uint16_t *temperatureCentigrade, uint16_t *humidityCentipercent) {

 gpio_pin_config_t in_config = {kGPIO_DigitalInput};
 gpio_pin_config_t out_config= {kGPIO_DigitalOutput, 0};
 uint8_t buffer[5];
 pulse = -1;

    /* inicializar buffer */
 memset(buffer, 0, sizeof(buffer));
 memset((void*)pulse_capture, 0, sizeof(pulse_capture));

 // configurar como entrada y verificar si la señal se eleva
 GPIO_PinInit(GPIO, DHT22_Port, DHT22_Pin, &in_config);

 if(GPIO_PinRead(GPIO, DHT22_Port, DHT22_Pin)==0) {
  return DHT22_NO_PULLUP;
 }

 // enviar señal de inicio
 GPIO_PinInit(GPIO, DHT22_Port, DHT22_Pin, &out_config);
 Ticks_Delay(20); // mantener la señal baja durante 20 ms

 // inicializar CTIMER

    CLOCK_EnableClock(kCLOCK_Swm); //Habilitar reloj para la matriz de conmutación: habilitar
    SWM_SetMovablePinSelect(SWM0, kSWM_T0_CAP_CHN0, SWM_PORT_PIN(DHT22_Port, DHT22_Pin)); //Configurar CAP0 conectado al DHT22
    CLOCK_DisableClock(kCLOCK_Swm); //Deshabilitar reloj para la matriz de conmutación.


    CTIMER_SetupCapture(CTIMER0, kCTIMER_Capture_0, kCTIMER_Capture_FallEdge, true);

    CTIMER_StartTimer(CTIMER0);

    uint32_t start_ticks = Return_Tick_Value();

 while (pulse < 40){ //Esperar los 40 pulsos de datos
     // Verificar si ha pasado el tiempo máximo de espera
     if ((Return_Tick_Value() - start_ticks) >= timeout_ms) {
    	 return DHT22_NO_DATA;
     }

 }

    CLOCK_EnableClock(kCLOCK_Swm);
    SWM0->PINASSIGN_DATA[14] |= 0x0000ff00;
    CLOCK_DisableClock(kCLOCK_Swm);
    GPIO_PinWrite(GPIO, DHT22_Port, DHT22_Pin, 1);

    CTIMER_StopTimer(CTIMER0);
    CTIMER_SetupCapture(CTIMER0, kCTIMER_Capture_0, kCTIMER_Capture_FallEdge, false);

    // Inspeccionar pulsos y determinar cuáles son 0 o 1
    for (int8_t i = 0; i < 40; ++i) {
     pulse_capture[i] = pulse_capture[i] / (CLOCK_GetFreq(kCLOCK_CoreSysClk)/1000000);

     buffer[i / 8] <<= 1; // siguiente bit de datos

     if (pulse_capture[i] > 50){
      buffer[i / 8] |= 0x01;
     }

    }

    /* ahora tenemos los 40 bits (5 bytes) de datos:
    * byte 1: datos enteros de humedad
    * byte 2: datos decimales de humedad (no utilizados para DTH11, siempre cero)
    * byte 3: datos enteros de temperatura
    * byte 4: datos fraccionarios de temperatura (no utilizados para DTH11, siempre cero)
    * byte 5: checksum, la suma de byte 1 + 2 + 3 + 4
    */

    /* verificar CRC */
    if (buffer[4] != ((buffer[0]+buffer[1]+buffer[2]+buffer[3]) & 0xFF)){
     return DHT22_BAD_CRC;
      }

    *humidityCentipercent = (((uint16_t)buffer[0]<<8)+buffer[1])*10;
    *temperatureCentigrade = (((uint16_t)buffer[2]<<8)+buffer[3])*10;

    return DHT22_OK;

}

//---------------------------------------------------------------//
// Interrupciones
//---------------------------------------------------------------//

void ctimer_cap0_callback(uint32_t flags)
{

 if(pulse >= 0){
  pulse_capture[pulse] = CTIMER_GetCaptureValue(CTIMER0, kCTIMER_Capture_0);
 }
 pulse ++;

}
