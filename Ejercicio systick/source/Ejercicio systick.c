/*
 * Copyright 2016-2024 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    Ejercicio systick.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC845.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
volatile uint16_t flag_tick=0;

void delay_mseg(int mseg);

int main(void) {
	BOARD_BootClockFRO24M();
    (void) SysTick_Config(SystemCoreClock/1000);
    gpio_pin_config_t out_config = {kGPIO_DigitalOutput, 1};
    gpio_pin_config_t in_config = {kGPIO_DigitalInput};
	gpio_pin_config_t out_config = {kGPIO_DigitalOutput, 0};
	GPIO_PortInit(GPIO, 0);
	GPIO_PinInit(GPIO, 0, 4, &out_config);
	GPIO_PortInit(GPIO, 1);
	GPIO_PinInit(GPIO, 1, 2, &out_config);

    while(1) {
    	if(GPIO_PinRead(GPIO, 0, 4)==0){

        	GPIO_PinWrite(GPIO, 1, 2, 0); // Led Rojo On
        	delay_mseg(1000);
        	GPIO_PinWrite(GPIO, 1, 2, 1); // Led Rojo Off
    	};

    }
    return 0 ;
}
void SysTick_Handler(void)
{
	flag_tick++;
}

void delay_mseg(int mseg){
	flag_tick = 0;
	while(flag_tick <= mseg);
}
