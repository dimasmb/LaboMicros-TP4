/***************************************************************************//**
  @file     drv_switch.h
  @brief    Drivers del switch
  @author   Dimas Bosch
 ******************************************************************************/

#ifndef _DRV_SWITCH_H_
#define _DRV_SWITCH_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdio.h>
#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef enum{
	ENC_NO_EVENT,
	ENC_TURN_CLOCK,
	ENC_TURN_ANTI_CLOCK,
	ENC_SMALL_PRESS,
	ENC_LONG_PRESS,
	ENC_GIANT_PRESS
}encoder_event_t;


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief TODO: Inicializa el encoder.
 * @return Nada
*/
void DRV_Init_Switch(void);

/**
 * @brief TODO: Devuelve el último evento del encoder.
 * @return Ultimo evento del encoder según encoder_event_t
*/
encoder_event_t getEvent(void);

/*******************************************************************************
 ******************************************************************************/

#endif // _TEMPLATE_H_
