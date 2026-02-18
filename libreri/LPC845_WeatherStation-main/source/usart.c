#include "usart.h"

_Bool usart_data_received = false;
uint8_t dataUsart = 0;
uint8_t UsartBufferData[USART_BUFFER_LENGTH]; // Buffer para almacenar datos usart


void Iniciar_USART(void) {
	// Configurar los pines para USART1
    CLOCK_EnableClock(kCLOCK_Swm);
    SWM_SetMovablePinSelect(SWM0, kSWM_USART1_TXD, kSWM_PortPin_P0_25); // USART1_TXD conectado a P0_25
    SWM_SetMovablePinSelect(SWM0, kSWM_USART1_RXD, kSWM_PortPin_P0_24); // USART1_RXD conectado a P0_24
    CLOCK_DisableClock(kCLOCK_Swm);

    // Seleccionar la fuente de reloj para USART1
    CLOCK_Select(kUART1_Clk_From_MainClk);

	// Crear y configurar la estructura para USART
    usart_config_t config;
    USART_GetDefaultConfig(&config);
    config.enableRx = true;
    config.enableTx = true;

    // Inicializar el USART con la configuración especificada
    USART_Init(USART1, &config, CLOCK_GetFreq(kCLOCK_MainClk));

    // Habilitar interrupciones para USART1 cuando hay datos listos para ser recibidos
    USART_EnableInterrupts(USART1, kUSART_RxReadyInterruptEnable);

    // Habilitar el manejador de interrupción para USART1
    NVIC_EnableIRQ(USART1_IRQn);
}

void usart1_write(uint8_t *p, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        USART_WriteByte(USART1, *(p++));
        while (!(USART1->STAT & USART_STAT_TXIDLE_MASK));
    }
}

void enviar_datos_actuales(void){
	uint8_t write_buffer[10];
	uint8_t index = 0;


	// Copiar los datos de temperatura y humedad al principio del buffer
    memcpy(&write_buffer[index], &temp, 2);
    index += 2;
    memcpy(&write_buffer[index], &hum, 2);
    index += 2;

	// Copiar los campos de time que se quieren guardar (excluyendo segundos)
    write_buffer[index++] = time.minutos;
    write_buffer[index++] = time.horas;
    write_buffer[index++] = time.diaSemana;
    write_buffer[index++] = time.diaMes;
    write_buffer[index++] = time.mes;
    write_buffer[index++] = time.año;


	usart1_write(write_buffer, index);
}

void USART1_IRQHandler(void)
{
    dataUsart = USART_ReadByte(USART1);
    usart_data_received = true;
}
