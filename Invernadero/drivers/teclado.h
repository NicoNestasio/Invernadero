#ifndef TECLADO_H_
#define TECLADO_H_

#include <stdint.h>
#include "fsl_gpio.h"

//---------------------------------------------------------------//
// Definiciones
//---------------------------------------------------------------//
#define FILAS 4
#define COLUMNAS 4


// Pines conectados a filas y columnas (puerto 0 en LPC845)
static const uint8_t filas[FILAS] = {8, 9, 11, 12};
static const uint8_t columnas[COLUMNAS] = {29, 28, 27, 26};

//---------------------------------------------------------------//
// Prototipos
//---------------------------------------------------------------//
void Teclado_Init(void);
char Teclado_Scan(void);

#endif /* TECLADO_H_ */
