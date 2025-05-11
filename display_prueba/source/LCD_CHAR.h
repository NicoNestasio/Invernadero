#define LCD_BUSY	1
#define LCD_NBUSY	0

/*-------------------------------------------------------------------*/
// mode 4bit 
void nibble_lcd4(unsigned char nb);
unsigned char check_busy4(void);
void mode4bit(void);		// mode 4bit
void write_com_lcd4(unsigned char w4b);
void ClearDisplay4(void);
void ReturnHome4(void);
void EntryModeSet4(unsigned char ems);
void DisplayOnOff4(unsigned char dof);
void CurDisShift4(unsigned char cdf);
void FuntionSet4(unsigned char fs);
void SetCGRAMAddress4(unsigned char sca);
void SetDDRAMAddress4(unsigned char sda);
void write_data_lcd4(unsigned char wd);
unsigned char read_data_lcd4(void);
void lcd_char_init4(void);
/*-------------------------------------------------------------------*/

void lcd_init4(void);
void line_lcd4(unsigned char *p,unsigned char r);

