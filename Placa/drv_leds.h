/***************************************************************************//**
  @file     drv_leds.h
  @brief    Drivers de los leds
  @author   Dimas Bosch
 ******************************************************************************/

#ifndef _DRV_LEDS_H_
#define _DRV_LEDS_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/



/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define LEDS_REFRESH_RATE_MS	1
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
enum{
	STATUS_0,
	STATUS_1,
	STATUS_3,
	STATUS_4,	//NO FUNCIONA
	STATUS_5,	//NO FUNCIONA
	STATUS_6,
	STATUS_7
};


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief TODO: Inicializa los LEDs. Imprime lo que indica changeState cada DISPLAY_REFRESH_RATE ms
 * @return Nada
*/
void DRV_Init_LEDs(void);

/**
 * @brief TODO: Cambia los LEDs que se prenden
 * @param state numero entre 0 y 7 para setear los LEDs. LED1 es el primer bit, LED3 es el tercer bit
 * @return Nada
*/
void changeState(int state);

/*******************************************************************************
 ******************************************************************************/

#endif // _TEMPLATE_H_
