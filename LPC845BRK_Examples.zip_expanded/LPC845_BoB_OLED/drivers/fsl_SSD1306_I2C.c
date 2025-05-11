/*
 * Copyright 2019 NXP
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
 * o Neither the name of copyright holder nor the names of its
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

/*----esta modificada para usar el I2C1, en caso de utilizar otro cambiar todo lo que diga I2C1 por la base del I2C usado--- */
/*---- BASADO EN fsl_SSD1306.h para SPI de NXP ------*/

#include <fsl_SSD1306_I2C.h>
#include "fsl_gpio.h"
#include "fsl_i2c.h"
#include "fsl_Systick_Delay.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief OLED buffer */
static uint8_t OLED_Buffer[(OLED_WIDTH * OLED_HEIGHT) / 8];

/*******************************************************************************
 * Code
 ******************************************************************************/

static void OLED_Command (uint8_t Cmd)
{
	i2c_master_transfer_t xfer = {0};

	xfer.data = (uint8_t *)&Cmd;
	xfer.dataSize = sizeof(Cmd);
	xfer.flags = kI2C_TransferDefaultFlag;
	xfer.slaveAddress = SSD1306_ADDRESS_1;
	xfer.direction = kI2C_Write;
	xfer.subaddress = 0x0;
	xfer.subaddressSize = 1;

	I2C_MasterTransferBlocking(I2C1, &xfer);

}

static void OLED_Data (uint8_t *Data)
{
	i2c_master_transfer_t xfer = {0};

	/*Start Transfer*/
	xfer.data = Data;
	xfer.dataSize = sizeof(OLED_Buffer);
	xfer.flags = kI2C_TransferDefaultFlag;
	xfer.slaveAddress = SSD1306_ADDRESS_1;
	xfer.direction = kI2C_Write;
	xfer.subaddress = 0x40;
	xfer.subaddressSize = 1;

	I2C_MasterTransferBlocking(I2C1, &xfer);

}

static void OLED_Reset (void)
{
	OLED_Command(OLED_DISPLAYON);
//	for(int i=0; i<1000; i++);					//delay 1ms
	OLED_Command(OLED_DISPLAYOFF);
//	for(int i=0; i<1000; i++);					//delay 1ms
	OLED_Command(OLED_DISPLAYON);
//	for(int i=0; i<10000; i++);					//delay 10ms

}

static void OLED_Config_Display(void)
{
    OLED_Command(0xAE);            // Display OFF
    OLED_Command(0xD5);            // Set display clock divide ratio/oscillator frequency
    OLED_Command(0x80);
    OLED_Command(0xA8);            // Set multiplex ratio
    OLED_Command(0x3F);
    OLED_Command(0xD3);            // Set display offset
    OLED_Command(0x00);
    OLED_Command(0x40);            // Set display start line
    OLED_Command(0xAD);            // DC-DC control mode set
    OLED_Command(0x8B);            // DC-DC ON
    OLED_Command(0xA1);            // Segment remap
    OLED_Command(0xC8);            // COM output scan direction
    OLED_Command(0xDA);            // COM pins hardware configuration
    OLED_Command(0x12);
    OLED_Command(0x81);            // Contrast control
    OLED_Command(0x7F);
    OLED_Command(0xD9);            // Pre-charge period
    OLED_Command(0x22);
    OLED_Command(0xDB);            // VCOMH deselect level
    OLED_Command(0x20);
    OLED_Command(0xA4);            // Entire display ON from RAM
    OLED_Command(0xA6);            // Normal display
    OLED_Command(0xAF);            // Display ON
}



static int OLED_Render_Char (uint8_t X_axis, uint8_t Y_axis, uint8_t SC, int8_t String, uint8_t Scale)
{

	uint8_t px, py;
	uint16_t start_pos;

	if ((X_axis >= OLED_WIDTH) || (Y_axis >= OLED_HEIGHT)) {
		return 1;
	}
	if (String > 127) {
		return 2;
	}
	if (Scale > 3) {
		return 3;
	}

	start_pos = ((uint8_t)String) * 7;			// Characters have a 7 row offset
	for (px=0; px<5; px++) {
		for (py=0; py<7; py++) {
			if ((font5x7[start_pos+py] >> (7-px)) & 1) {
				switch (Scale) {
				case 3:
					OLED_Set_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale),  SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale),  SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+2, Y_axis+(py*Scale),  SC);
					OLED_Set_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale)+1, SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale)+1, SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+2, Y_axis+(py*Scale)+1, SC);
					OLED_Set_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale)+2, SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale)+2, SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+2, Y_axis+(py*Scale)+2, SC);
					break;
				case 2:
					OLED_Set_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale),  SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale),  SC);
					OLED_Set_Pixel(X_axis+(px*Scale),   Y_axis+(py*Scale)+1, SC);
					OLED_Set_Pixel(X_axis+(px*Scale)+1, Y_axis+(py*Scale)+1, SC);
					break;
				case 1:
				default:
					OLED_Set_Pixel(X_axis+px, Y_axis+py, SC);
					break;
				}
			}
		}
	}
	return 0;
}

void OLED_Init(void)
{

	/*Give the display a reset*/
	OLED_Reset();

	/* Clear the framebuffer*/
	OLED_Clear();

	/*Configure the OLED display controller*/
	OLED_Config_Display();

}

void OLED_Refresh(void)
{
    for (uint8_t page = 0; page < (OLED_HEIGHT / 8); page++)
    {
        OLED_Command(0xB0 + page);                    // Set page address
        OLED_Command(0x02);                            // Set lower column address (offset 2)
        OLED_Command(0x10);                            // Set higher column address

        // Ahora mandamos los datos del buffer
        i2c_master_transfer_t xfer = {0};

        xfer.data = &OLED_Buffer[page * OLED_WIDTH];
        xfer.dataSize = OLED_WIDTH;
        xfer.flags = kI2C_TransferDefaultFlag;
        xfer.slaveAddress = SSD1306_ADDRESS_1;
        xfer.direction = kI2C_Write;
        xfer.subaddress = 0x40;
        xfer.subaddressSize = 1;

        I2C_MasterTransferBlocking(I2C1, &xfer);
    }
}

void OLED_Clear(void)
{

	memset(OLED_Buffer, 0, sizeof(OLED_Buffer));

}


void OLED_Fill(uint8_t Pattern)
{

	memset(OLED_Buffer, Pattern, sizeof(OLED_Buffer));

}

void OLED_Display_Mode (uint8_t Mode)
{

	if (Mode) {
		OLED_Command(OLED_INVERTDISPLAY);
	}
	else {
		OLED_Command(OLED_NORMALDISPLAY);
	}

}

void OLED_Set_Pixel (uint8_t X_axis, uint8_t Y_axis, uint8_t SC)
{

	if((X_axis >= OLED_WIDTH) || (Y_axis >= OLED_HEIGHT))
	{
		//Do nothing
	}
	else
	{
		switch(SC)
		{
			case kOLED_Pixel_Clear:
				OLED_Buffer [X_axis + (Y_axis / 8) * OLED_WIDTH] &= ~(1 << (Y_axis & 7));
				break;
			case kOLED_Pixel_Set:
				OLED_Buffer [X_axis + (Y_axis / 8) * OLED_WIDTH] |= (1 << (Y_axis & 7));
				break;
		}
	}
}

void OLED_Set_Text (uint8_t X_axis, uint8_t Y_axis, uint8_t SC, char* String, uint8_t Scale)
{
	uint16_t Cont;
	uint16_t xscaled;

	if ((X_axis >= OLED_WIDTH) || (Y_axis >= OLED_HEIGHT)) {
		//Do nothing
	}
	else
	{
		if (Scale > 3) {
			//Do nothing
		}
		else
		{

			for (Cont = 0; String[Cont] != '\0'; Cont++) {
				// Catch overflow when scaling!
				xscaled = X_axis+(Cont*5*Scale);
				if (xscaled > OLED_WIDTH)
				{
					//Do nothing
				}

				else
				{
					OLED_Render_Char(xscaled, Y_axis, SC, String[Cont], Scale);

				}
			}
		}
	}

}

void OLED_Copy_Image(const uint8_t *Img, uint16_t size){

	uint16_t CpyBuffer;

	OLED_Clear();

	for(CpyBuffer = 0; CpyBuffer < size - 1; CpyBuffer++)
	{
		OLED_Buffer[CpyBuffer] = *(Img + CpyBuffer);
	}
}
