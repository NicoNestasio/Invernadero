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

//---------------------------------------------------------------//
// defines
//---------------------------------------------------------------//
#define ADC0_CH0		0
//---------------------------------------------------------------//
// Variables
//---------------------------------------------------------------//
long i;
uint32_t count_mseg;
adc_result_info_t adcResultInfoStruct;
uint32_t frequency;
//---------------------------------------------------------------//
// Prototypes
//---------------------------------------------------------------//
void ADC_Configuration(void);
//---------------------------------------------------------------//
// main
//---------------------------------------------------------------//
int main(void) {

    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_Swm);

    SWM_SetFixedPinSelect(SWM0, kSWM_ADC_CHN0, true);

    CLOCK_DisableClock(kCLOCK_Swm);

    CLOCK_Select(kADC_Clk_From_Fro);
    CLOCK_SetClkDivider(kCLOCK_DivAdcClk, 1U);

    POWER_DisablePD(kPDRUNCFG_PD_ADC0);
    frequency = CLOCK_GetFreq(kCLOCK_Fro) / CLOCK_GetClkDivider(kCLOCK_DivAdcClk);
    (void) ADC_DoSelfCalibration(ADC0, frequency);

	adc_config_t adcConfigStruct;
    adc_conv_seq_config_t adcConvSeqConfigStruct;
    adcConfigStruct.clockMode = kADC_ClockSynchronousMode;
    adcConfigStruct.clockDividerNumber = 1;
    adcConfigStruct.enableLowPowerMode = false;
    adcConfigStruct.voltageRange = kADC_HighVoltageRange;
    ADC_Init(ADC0, &adcConfigStruct);

    adcConvSeqConfigStruct.channelMask = 1 << ADC0_CH0;
    adcConvSeqConfigStruct.triggerMask = 0;
    adcConvSeqConfigStruct.triggerPolarity = kADC_TriggerPolarityPositiveEdge;
    adcConvSeqConfigStruct.enableSingleStep = false;
    adcConvSeqConfigStruct.enableSyncBypass = false;
    adcConvSeqConfigStruct.interruptMode = kADC_InterruptForEachSequence;
    ADC_SetConvSeqAConfig(ADC0, &adcConvSeqConfigStruct);
    ADC_EnableConvSeqA(ADC0, true);
	ADC_DoSoftwareTriggerConvSeqA(ADC0);

	//LED
	gpio_pin_config_t out_config = {kGPIO_DigitalOutput, 0};
	GPIO_PortInit(GPIO, 0);
	GPIO_PinInit(GPIO, 0, 0, &out_config);

    while(1) {
        if(ADC_GetChannelConversionResult(ADC0, ADC0_CH0, &adcResultInfoStruct));
        {
			PRINTF("Canal %d en PIO0_7 tiene un valor = %d\r\n", adcResultInfoStruct.channelNumber, adcResultInfoStruct.result);
			ADC_DoSoftwareTriggerConvSeqA(ADC0);
			if(adcResultInfoStruct.result>=3000){
				GPIO_PinWrite(GPIO, 0, 0, 1); // Led Rojo On
			}else{
				GPIO_PinWrite(GPIO, 0, 0, 0); // Led Rojo Off
			}
        }
    }
}
