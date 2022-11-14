/***************************************************************************//**
  @file     drv_display.h
  @brief    Drivers del display de 7 segmentos x4
  @author   Dimas Bosch
 ******************************************************************************/

#ifndef _DRV_DISPLAY_H_
#define _DRV_DISPLAY_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <string.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define MAX_DIGIT 4
#define DISPLAY_REFRESH_RATE_MS	1

enum{
	DONT_POINT = -1,
	POINT_DIGIT_0,
	POINT_DIGIT_1,
	POINT_DIGIT_2,
	POINT_DIGIT_3
};

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief TODO: Inicializa el display. Imprime la palabra que indica print2display cada DISPLAY_REFRESH_RATE ms
 * @return Nada
*/
void DRV_Init_Display(void);

/**
 * @brief TODO: cambia la palabra que se muestra
 * @param str: arreglo de 4 caracteres a imprimir en el display
 * @return Nada
*/
void print2display(char str[4]);

/**
 * @brief TODO: cambia el dígito donde se prende el punto.
 * @param digit: digito donde prender el LED de punto. -1 para no imprimir nada.
 * @return Nada
*/
void setPointer(int digit);

/**
 * @brief TODO: cambia el brillo del display
 * @return Nada
*/
void changeBright(void);

/*******************************************************************************
 ******************************************************************************/

#endif // _DRV_DISPLAY_H_
