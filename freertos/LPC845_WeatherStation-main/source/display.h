#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "fsl_common.h"
#include "rtc_ds3231.h"

typedef enum {
	PANTALLA_SENSORES,
    PANTALLA_HORA,
	PANTALLA_USART,
    NUM_PANTALLAS // Esto da el número total de pantallas
} pantalla_t;

extern uint16_t temp;
extern uint16_t hum;
extern rtc_time_t time;

uint8_t calcularPosX(char* String, uint8_t Scale);
void display_Sensors(void);
void display_Time(void);

#endif /* DISPLAY_H_ */
