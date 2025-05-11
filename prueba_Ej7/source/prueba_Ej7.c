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
 * @file    prueba_Ej7.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC845.h"
#include "fsl_debug_console.h"
/* void numero(int i, int A,int B,int C,int D,int E,int F){
	int j=1;
	if(A<16){
		j=0;
		GPIO_PinInit(GPIO, 0,A, &out_config);
		GPIO_PinWrite(GPIO, 0,A, j); // Segmentos Arriba
	};
	if(B<16){
		j=0;
		GPIO_PinInit(GPIO, 0,B, &out_config);
		GPIO_PinWrite(GPIO, 0,B, j); // Segmentos Arriba
	};
	if(C<16){
		j=0;
		GPIO_PinInit(GPIO, 0,C, &out_config);
		GPIO_PinWrite(GPIO, 0,C, j); // Segmentos Arriba
	};
	if(C<16){
		j=0;
		GPIO_PinInit(GPIO, 0,D, &out_config);
		GPIO_PinWrite(GPIO, 0,D, j); // Segmentos Arriba
	};
	if(A<16){
		j=0;
		GPIO_PinInit(GPIO, 0,E, &out_config);
		GPIO_PinWrite(GPIO, 0,E, j); // Segmentos Arriba
	};
	if(A<16){
		j=0;
		GPIO_PinInit(GPIO, 0,F, &out_config);
		GPIO_PinWrite(GPIO, 0,F, j); // Segmentos Arriba
	}; */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
void cero (int d) {
	gpio_pin_config_t out_config = {kGPIO_DigitalOutput, 0};
	GPIO_PortInit(GPIO, 0);
	GPIO_PinInit(GPIO, 0,d, &out_config);
	GPIO_PinWrite(GPIO, 0,d, 0); // Segmentos Arriba
	GPIO_PinInit(GPIO, 0, 10, &out_config);
	GPIO_PinWrite(GPIO, 0, 10, 0); // Led Verde On
	GPIO_PinInit(GPIO, 0, 11, &out_config);
	GPIO_PinWrite(GPIO, 0, 11, 0); // Led Verde On
	GPIO_PinInit(GPIO, 0, 6, &out_config);
	GPIO_PinWrite(GPIO, 0, 6, 0); // Led Verde On
	GPIO_PinInit(GPIO, 0, 0, &out_config);
	GPIO_PinWrite(GPIO, 0, 0, 0); // Led Verde On*/
	GPIO_PinInit(GPIO, 0, 13, &out_config);
	GPIO_PinWrite(GPIO, 0, 13, 0); // Led Verde On*/
	GPIO_PinInit(GPIO, 0, 14, &out_config);
	GPIO_PinWrite(GPIO, 0, 14, 0); // Led Verde On*/
};
void delay(int d){
	int i;
	for(i=0;i<d;i++){};
};
void uno (int i){
	gpio_pin_config_t out_config = {kGPIO_DigitalOutput, 0};
	GPIO_PortInit(GPIO, 0);
	GPIO_PinInit(GPIO, 0,i, &out_config);
	GPIO_PinWrite(GPIO, 0,i, 0); // Segmentos Arriba
	GPIO_PinInit(GPIO, 0,11, &out_config);
	GPIO_PinWrite(GPIO, 0,11, 0); // Segmentos Arriba
	GPIO_PinInit(GPIO, 0,6, &out_config);
	GPIO_PinWrite(GPIO, 0,6, 0); // Segmentos Arriba
};
void dos (int i){
	gpio_pin_config_t out_config = {kGPIO_DigitalOutput, 0};
	GPIO_PortInit(GPIO, 0);
	GPIO_PinInit(GPIO, 0,i, &out_config);
	GPIO_PinWrite(GPIO, 0,i, 0); // Segmentos Arriba
	GPIO_PinInit(GPIO, 0,10, &out_config);
	GPIO_PinWrite(GPIO, 0,10, 0); // Segmentos Arriba
	GPIO_PinInit(GPIO, 0,11, &out_config);
	GPIO_PinWrite(GPIO, 0,11, 0); // Segmentos Arriba
	GPIO_PinInit(GPIO, 0,15, &out_config);
	GPIO_PinWrite(GPIO, 0,15, 0); // Segmentos Arriba
	GPIO_PinInit(GPIO, 0,0, &out_config);
	GPIO_PinWrite(GPIO, 0,0, 0); // Segmentos Arriba
	GPIO_PinInit(GPIO, 0,14, &out_config);
	GPIO_PinWrite(GPIO, 0,14, 0); // Segmentos Arriba

};

void decenas (int i, int j){
	void unidades(int d){
		switch (j){
					case 1:
						uno(9);
						break;
					case 2:
						dos(9);
						break;
				};
	};
	int k;
		switch (i){
			case 1:
				uno(8);
				unidades(j);
				break;
			case 2:
				dos(8);
				unidades(j);
				break;
		};

};


int main(void) {
    while(1) {
    	uno(9);
    	delay(500000);
    	dos(9);
    	delay(500000);
    	decenas(1,2);
    	decenas(1,1);

    };
    return 0 ;
}
