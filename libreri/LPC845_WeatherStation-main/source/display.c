#include "display.h"
#include "fsl_SSD1306_I2C.h"
#include "stdio.h"
#include "rtc_ds3231.h"
#include "fsl_Font5x7.h"


uint8_t calcularPosX(char* String, uint8_t Scale) {
	uint8_t ancho_texto = strlen(String) * 5 * Scale; // 5 píxeles por letra
    return (OLED_WIDTH - ancho_texto) / 2;
}


void display_Sensors(){
	char temp_str[5];
	char hum_str[5];
    char time_str[6];  // Buffer para la hora
    char date_str[9];  // Buffer para la fecha


    // Verificar si la temperatura es negativa
    if (temp & 0x8000) {
        // Ignorar el bit más significativo para obtener el valor absoluto
        temp = temp & 0x7FFF; // Limpiar el bit más significativo
        sprintf(temp_str, "-%d.%d", temp / 100, (temp % 100) / 10);
    } else {
        // Temperatura positiva
        sprintf(temp_str, "%02d.%02d", temp / 100, temp % 100);
    }

    sprintf(hum_str, "%02d.%02d", hum / 100, hum % 100);
    sprintf(time_str, "%02x:%02x", time.horas, time.minutos);
    sprintf(date_str, "%02x/%02x/%02x", time.diaMes, time.mes, time.año);


    OLED_Clear();
    OLED_Copy_Image(&display_temp[0], sizeof(display_temp));
    OLED_Set_Text(5, 20, 1, temp_str, 2);
    OLED_Set_Text(5, 50, 1, hum_str, 2);
    OLED_Set_Text(1, 0, 1, time_str, 1);
    OLED_Set_Text(85, 0, 1, date_str, 1);
    OLED_Refresh();
}

void display_Time(){
	// Definiciones de días de la semana y meses
	const char *diasSemana[] = {
	    "lunes", "martes", "miercoles", "jueves", "viernes", "sabado", "domingo"
	};

	const char *meses[] = {
	    "enero", "febrero", "marzo", "abril", "mayo", "junio", "julio", "agosto",
	    "septiembre", "octubre", "noviembre", "diciembre"
	};

    char time_str[5];   // Buffer para la hora
    char dia_str[10];  // Buffer para el dia de la semana
    char date_str[20];  // Buffer para la fecha completa
    char año_str[4];	// Buffer para el año


    OLED_Clear();

    sprintf(time_str, "%02x:%02x", time.horas, time.minutos);
    OLED_Set_Text(calcularPosX(time_str, 3), 0, 1, time_str, 3);

    sprintf(dia_str, "%s", diasSemana[(time.diaSemana)-1]);
    OLED_Set_Text(calcularPosX(dia_str, 2), 20, 1, dia_str, 2);

    sprintf(date_str,"%x de %s", time.diaMes, meses[BCDToDecimal(time.mes)-1]);
    OLED_Set_Text(calcularPosX(date_str, 1), 38, 1, date_str, 1);

    sprintf(año_str, "20%02x", time.año);
    OLED_Set_Text(calcularPosX(año_str, 2), 50, 1, año_str, 2);

    OLED_Refresh();

}
