//---------------------------------------------------------------//
// includes
//---------------------------------------------------------------//
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
#include "fsl_adc.h"
#include "fsl_power.h"
#include "fsl_swm.h"
#include "fsl_iocon.h"
#include "LPC845.h"
#include "fsl_debug_console.h"
#include "fsl_usart.h"


//---------------------------------------------------------------//
// defines
//---------------------------------------------------------------//
#define FALSE        0
#define TRUE         1
#define ADC0_CH2     2
#define SYS_TICK_FREQ 1000

//---------------------------------------------------------------//
// Variables
//---------------------------------------------------------------//
long i;
uint32_t count_mseg, d0;
adc_result_info_t adcResultInfoStruct;
uint32_t frequency, count = 0, report = 5000;
uint8_t adc_conv_complete,ch, flagReceived = 0;
uint8_t txdBuffer[64],key = 1;
volatile uint16_t flag_tick = 0, time = 0, interval = 0, nreport=0; ;
float peso = 0;
//---------------------------------------------------------------//
// Prototypes
//---------------------------------------------------------------//
void ADC_Configuration(uint32_t chs);
void delay_mseg(int mseg);
void SysTick_Handler(void);
void USART1_IRQHandler(void);
void Unidad_gramo(void);
void usart1_write(uint8_t *p);
//---------------------------------------------------------------//
// main
//---------------------------------------------------------------//
int main(void) {


	BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_Swm);

    SWM_SetFixedPinSelect(SWM0, kSWM_ADC_CHN2, true);
    SWM_SetMovablePinSelect(SWM0, kSWM_USART1_TXD, kSWM_PortPin_P0_25);
    SWM_SetMovablePinSelect(SWM0, kSWM_USART1_RXD, kSWM_PortPin_P0_24);

    CLOCK_DisableClock(kCLOCK_Swm);

    CLOCK_Select(kADC_Clk_From_Fro);
    CLOCK_SetClkDivider(kCLOCK_DivAdcClk, 1U);
    CLOCK_Select(kUART1_Clk_From_MainClk);

    POWER_DisablePD(kPDRUNCFG_PD_ADC0);
    frequency = CLOCK_GetFreq(kCLOCK_Fro) / CLOCK_GetClkDivider(kCLOCK_DivAdcClk);
    (void) ADC_DoSelfCalibration(ADC0, frequency);
    ADC_Configuration((1 << ADC0_CH2));  // Configura el canal 2

    ADC_EnableInterrupts(ADC0, kADC_ConvSeqAInterruptEnable);
    NVIC_EnableIRQ(ADC0_SEQA_IRQn);
    adc_conv_complete = FALSE;
    ADC_DoSoftwareTriggerConvSeqA(ADC0);

    uint8_t txdBuffer[64];

    usart_config_t user_config;
    USART_GetDefaultConfig(&user_config);
    user_config.baudRate_Bps = 9600;
    user_config.enableTx = true;
    user_config.enableRx = true;
    USART_Init(USART1, &user_config, CLOCK_GetFreq(kCLOCK_MainClk));
    USART_EnableInterrupts(USART1, kUSART_RxReadyInterruptEnable);
    NVIC_EnableIRQ(USART1_IRQn);


    gpio_pin_config_t out_config = {kGPIO_DigitalOutput, 1};
    GPIO_PortInit(GPIO, 1);
    GPIO_PinInit(GPIO, 1, 2, &out_config); // LED Rojo
    GPIO_PinInit(GPIO, 1, 0, &out_config); // LED Verde
    GPIO_PinInit(GPIO, 1, 1, &out_config); // LED Azul

    //Teclado
    gpio_pin_config_t in_config = {kGPIO_DigitalInput, 0};
    GPIO_PortInit(GPIO, 0);
    GPIO_PinInit(GPIO, 0, 17, &out_config); // FILA
    GPIO_PinInit(GPIO, 0, 18, &in_config); // 1
    GPIO_PinInit(GPIO, 0, 19, &in_config); // 2
    GPIO_PinInit(GPIO, 0, 20, &in_config); // 3


    SysTick_Config(SystemCoreClock / SYS_TICK_FREQ);

    usart1_write("Balanza \r \nPresione (2) para sensar cada 2 segundos o (4) para 4. \r \n(1) kilogramos (2) gramos (3) Toneladas. \r \n ");


    while (1) {
		Unidad_gramo();
    	if (adc_conv_complete == TRUE) {
    		if (flagReceived != 0 && interval >= report)
    	    {
    			nreport++;
    			peso = ((200.0 - 50.0) / 4096.0) * (float)d0 + 50.0;
    			switch(key) {
    			    case 1:
    			    	//kg
    	        	    sprintf(txdBuffer, "%d - %0.2f kg %d seg \r \n",nreport, peso, time);

    			        break;
    			    case 2:
    			    	//gramos

    	        	    sprintf(txdBuffer, "%d - %0.2f g %d seg \r \n",nreport, peso*1000, time);
    			        break;
    			    case 3:
    			    	//toneladas

    	        	    sprintf(txdBuffer, "%d - %0.3f Tg %d seg \r \n",nreport, peso/1000, time);
    			        break;
    			}
    			USART_WriteBlocking(USART1, txdBuffer, strlen(txdBuffer));
    			interval = 0;

    	    }


    	    if ((d0 >= 409) && (d0 < 1228)) {
    	        GPIO_PinWrite(GPIO, 1, 1, 0); // LED Azul On
    	        delay_mseg(1000);
    	        GPIO_PinWrite(GPIO, 1, 1, 1); // LED Azul Off

    	    } else if ((d0 >= 1433) && (d0 < 2262)) {
    	        GPIO_PinWrite(GPIO, 1, 0, 0); // LED Verde On
    	        delay_mseg(1000);
    	        GPIO_PinWrite(GPIO, 1, 0, 1); // LED Verde Off
    	    } else if (d0 >= 2867) {
    	        GPIO_PinWrite(GPIO, 1, 2, 0); // LED Rojo On
    	        delay_mseg(1000);
    	        GPIO_PinWrite(GPIO, 1, 2, 1); // LED Rojo Off
    	    }

    	    adc_conv_complete = FALSE;
    	    ADC_DoSoftwareTriggerConvSeqA(ADC0);
    	}

    }
}

//---------------------------------------------------------------//
// interrupciones
//---------------------------------------------------------------//
void SysTick_Handler(void) {
    flag_tick++;
	if (flagReceived == 1){
        if (flag_tick % 1000 == 0) {
            time++;
    		interval += 1000;
        }

	}
}

void ADC0_SEQA_IRQHandler(void) {
    if (kADC_ConvSeqAInterruptFlag == (kADC_ConvSeqAInterruptFlag & ADC_GetStatusFlags(ADC0))) {
        ADC_GetChannelConversionResult(ADC0, ADC0_CH2, &adcResultInfoStruct);
        d0 = adcResultInfoStruct.result;
        ADC_ClearStatusFlags(ADC0, kADC_ConvSeqAInterruptFlag);
        adc_conv_complete = TRUE;
    }
}

//---------------------------------------------------------------//
// funciones
//---------------------------------------------------------------//
void Unidad_gramo(void){
	GPIO_PinWrite(GPIO, 0, 17, 0);
	if(GPIO_PinRead(GPIO, 0, 18) == 0){
		delay_mseg(50);
		key = 1;

	} else if (GPIO_PinRead(GPIO, 0, 19) == 0) {
		delay_mseg(50);
		key = 2;

	} else if (GPIO_PinRead(GPIO, 0, 20) == 0) {
		delay_mseg(50);
		key = 3;

	}

};
void ADC_Configuration(uint32_t chs) {
    adc_config_t adcConfigStruct;
    adc_conv_seq_config_t adcConvSeqConfigStruct;
    adcConfigStruct.clockMode = kADC_ClockSynchronousMode;
    adcConfigStruct.clockDividerNumber = 1;
    adcConfigStruct.enableLowPowerMode = false;
    adcConfigStruct.voltageRange = kADC_HighVoltageRange;
    ADC_Init(ADC0, &adcConfigStruct);

    adcConvSeqConfigStruct.channelMask = chs;
    adcConvSeqConfigStruct.triggerMask = 0;
    adcConvSeqConfigStruct.triggerPolarity = kADC_TriggerPolarityPositiveEdge;
    adcConvSeqConfigStruct.enableSingleStep = false;
    adcConvSeqConfigStruct.enableSyncBypass = false;
    adcConvSeqConfigStruct.interruptMode = kADC_InterruptForEachSequence;
    ADC_SetConvSeqAConfig(ADC0, &adcConvSeqConfigStruct);
    ADC_EnableConvSeqA(ADC0, true);
}

void USART1_IRQHandler(void)
{
	ch = USART_ReadByte(USART1);
	if (ch == '2') {
	    report = 2000;
	} else if (ch == '4') {
	    report = 4000;
	}

	flagReceived = 1;
}

void delay_mseg(int mseg) {
    flag_tick = 0;
    while (flag_tick < mseg);
}

void usart1_write(uint8_t *p){
	while(*p != 0){
		USART_WriteByte(USART1, *(p++));
		while (!(USART1->STAT & USART_STAT_TXIDLE_MASK));
	}
}
