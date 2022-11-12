/***************************************************************************/ /**
   @file     +fsm.h (ej: template.h)+
   @brief    +macros y funciones relacionadas al funcionamiento de la fsm+
   @author   +Nombre del autor (ej: Salvador Allende)+
  ******************************************************************************/

#ifndef _FSM_H_
#define _FSM_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "drv_display.h"
#include "drv_leds.h"
#include "drv_switch.h"
#include "drv_FRDM.h"
#include "lectora.h"
#include "timer.h"

#include <string.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define MAX_NUM_ID 8 //numero m�ximo de numeros de un id
#define MAX_NUM_PSW 5 //numero m�ximo de numeros de una contrae�a
#define NAN 10

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

// Different type events
typedef enum
{
	Card_Read,
	Card_Error,
	Right_Turn,
	Left_Turn,
	press,
	double_press,
	giant_press,
	last_Event
} eSystemEvent;

// Different state of fsm
typedef enum
{
	wait_State,
	pswrd_Input_State,
	block_State, //FALTA IMPLEMENTAR (USAR CLOCK)
	last_State
} eSystemState;

// typedef of function pointer
typedef eSystemState (*pfEventHandler)(eSystemEvent);
// structure of state and event with event handler

typedef struct
{
	eSystemState eStateMachine;
	pfEventHandler pfStateMachineEvnentHandler;
} sStateMachine;

typedef struct
{
	int position;
	int selected;
} screenPointer;

typedef struct
{
	char screen[8];
	int screenNum;
} screen;

typedef struct
{
	int id[8];
	char card[40];
	int password[5];
} user;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: extern unsigned int anio_actual;+

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void loadUsers(user userList[], int quantity);
void printDisplay(void);

void FSM_Run(eSystemState *nextState);
#include "fsm.h"

//handler de eventos





#endif // _FSM_H_
