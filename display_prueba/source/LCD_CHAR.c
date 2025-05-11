#include "LCD_CHAR.h"
#include "LSE_kit_v1.h"

#define delay_short	2500
#define delay_long	25000
union {
	uint8_t data;
	struct{
		uint8_t b0:1;
		uint8_t b1:1;
		uint8_t b2:1;
		uint8_t b3:1;
		uint8_t b4:1;
		uint8_t b5:1;
		uint8_t b6:1;
		uint8_t b7:1;
	} d;
} lcd;
/*-------------------------------------------------------------------*/
void delay(uint32_t dl){
	uint32_t d;
	for(d=0;d<dl;d++);
}
/*-------------------------------------------------------------------*/
void write_com_lcd4(unsigned char wb){
	LCD_RS(0);
	LCD_RW(0);
	lcd.data = wb;
	LCD_DB7(lcd.d.b7);
	LCD_DB6(lcd.d.b6);
	LCD_DB5(lcd.d.b5);
	LCD_DB4(lcd.d.b4);
	LCD_E(1);
	delay(delay_short);
	LCD_E(0);
	LCD_DB7(lcd.d.b3);
	LCD_DB6(lcd.d.b2);
	LCD_DB5(lcd.d.b1);
	LCD_DB4(lcd.d.b0);
	LCD_E(1);
	delay(delay_short);
	LCD_E(0);
	delay(delay_short);
}
/*-------------------------------------------------------------------*/
void write_data_lcd4(unsigned char wd){
	LCD_RS(1);
	LCD_RW(0);
	lcd.data = wd;
	LCD_DB7(lcd.d.b7);
	LCD_DB6(lcd.d.b6);
	LCD_DB5(lcd.d.b5);
	LCD_DB4(lcd.d.b4);
	LCD_E(1);
	delay(delay_short);
	LCD_E(0);
	LCD_DB7(lcd.d.b3);
	LCD_DB6(lcd.d.b2);
	LCD_DB5(lcd.d.b1);
	LCD_DB4(lcd.d.b0);
	LCD_E(1);
	delay(delay_short);
	LCD_E(0);
	delay(delay_short);
}
/*-------------------------------------------------------------------*/
void mode4bit(void){
	delay(delay_long);			
	delay(delay_long);
	delay(delay_long);
	delay(delay_long);
	delay(delay_long);
	delay(delay_long);
	LCD_RS(0);
	// primer 3
	lcd.data = 3;
	LCD_DB7(lcd.d.b3);
	LCD_DB6(lcd.d.b2);
	LCD_DB5(lcd.d.b1);
	LCD_DB4(lcd.d.b0);
	LCD_E(1);
	delay(delay_short);
	LCD_E(0);
	delay(delay_short);
	LCD_RS(0);
	// segundo 3
	lcd.data = 3;
	LCD_DB7(lcd.d.b3);
	LCD_DB6(lcd.d.b2);
	LCD_DB5(lcd.d.b1);
	LCD_DB4(lcd.d.b0);
	LCD_E(1);
	delay(delay_short);
	LCD_E(0);
	delay(delay_short);
	LCD_RS(0);
	// tercer 3
	lcd.data = 3;
	LCD_DB7(lcd.d.b3);
	LCD_DB6(lcd.d.b2);
	LCD_DB5(lcd.d.b1);
	LCD_DB4(lcd.d.b0);
	LCD_E(1);
	delay(delay_short);
	LCD_E(0);
	delay(delay_short);
	LCD_RS(0);
	// Modo 4 bit
	lcd.data = 2;
	LCD_DB7(lcd.d.b3);
	LCD_DB6(lcd.d.b2);
	LCD_DB5(lcd.d.b1);
	LCD_DB4(lcd.d.b0);
	LCD_E(1);
	delay(delay_short);
	LCD_E(0);
	delay(delay_short);				
}
/*-------------------------------------------------------------------*/
void ClearDisplay4(void){
	write_com_lcd4(0x01);		// Display clear
	delay(delay_long);			// delay 1.53mseg
}
/*-------------------------------------------------------------------*/
void ReturnHome4(void){
	write_com_lcd4(0x02);		// Display clear
	delay(delay_long);			// delay 1.53mseg
}
/*-------------------------------------------------------------------*/
void EntryModeSet4(unsigned char ems){
	ems &= 0x03; ems |= 0x04;
	write_com_lcd4(ems);		// Entry mode set: I/D=1-increment mode,SH=0-shift off
	delay(delay_short);			// delay 39useg
}
/*-------------------------------------------------------------------*/
void DisplayOnOff4(unsigned char dof){
	dof &= 0x07; dof |= 0x08;
	write_com_lcd4(dof);		// Display ON/OFF:D=1 display on,C=0 cursor off,B=0,blink off
	delay(delay_short);			// delay 39useg
}
/*-------------------------------------------------------------------*/
void CurDisShift4(unsigned char cdf){
	cdf &= 0x0C; cdf |= 0x10;
	write_com_lcd4(cdf);		// cursor display shift
	delay(delay_short);			// delay 39useg
}
/*-------------------------------------------------------------------*/
void FuntionSet4(unsigned char fs){
	fs &= 0x1C; fs |= 0x20;
	write_com_lcd4(fs);		// Funtion set: 8bit bus,N=1-1line,F=0-5x8dots
	delay(delay_short);			// delay 39useg
}
/*-------------------------------------------------------------------*/
void SetCGRAMAddress4(unsigned char sca){
	sca &= 0x3F; sca |= 0x40;
	write_com_lcd4(sca);		// Funtion set: 8bit bus,N=1-1line,F=0-5x8dots
	delay(delay_short);			// delay 39useg
}
/*-------------------------------------------------------------------*/
void SetDDRAMAddress4(unsigned char sda){
	sda &= 0x7F; sda |= 0x80;
	write_com_lcd4(sda);		// Funtion set: 8bit bus,N=1-1line,F=0-5x8dots
	delay(delay_short);			// delay 39useg
}
/*-------------------------------------------------------------------*/
void lcd_char_init4(void){
	LCD_RW(0);LCD_RS(1);LCD_E(0);
	mode4bit();
	FuntionSet4(0x28);			
	EntryModeSet4(0x06);
	ClearDisplay4();
	ReturnHome4();
}
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/
void line_lcd4(unsigned char *p,unsigned char r){
	if(r==1) r = 0;
	if(r==2) r = 0x40;
	if(r==3) r = 0x14;
	if(r==4) r = 0x54;

	SetDDRAMAddress4(r);
	r = 0;
	while(r<20){
		write_data_lcd4(*p);
		p++;
		r++;
		if(*p == 0) r = 20;
	}
}
/*-------------------------------------------------------------------*/
unsigned char char_read_lcd4(void){
	unsigned char l;
	l = read_data_lcd4();
	return l;
}
/*-------------------------------------------------------------------*/

