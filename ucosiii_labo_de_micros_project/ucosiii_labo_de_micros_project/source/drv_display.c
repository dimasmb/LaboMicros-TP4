/***************************************************************************//**
  @file     drv_display
  @brief    Drivers del display de 7 segmentos x4
  @author   Dimas Bosch
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "drv_display.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gpio.h"
#include "timer.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
enum{
	PIN_SEL1 = PORTNUM2PIN(PC, 17),
	PIN_SEL0 = PORTNUM2PIN(PC, 16)
};

enum{
	PIN_SEGDP = PORTNUM2PIN(PC, 5),
	PIN_SEGA = PORTNUM2PIN(PB, 18),
	PIN_SEGB = PORTNUM2PIN(PB, 19),
	PIN_SEGC = PORTNUM2PIN(PC, 1),
	PIN_SEGD = PORTNUM2PIN(PC, 8),
	PIN_SEGE = PORTNUM2PIN(PC, 9),
	PIN_SEGF = PORTNUM2PIN(PC, 0),
	PIN_SEGG = PORTNUM2PIN(PC, 7)
};//Defines de los pines del display

enum{
	DP = 0x80,
	num_0 = 0x7E,
	num_1 = 0x30,
	num_2 = 0x6D,
	num_3 = 0x79,
	num_4 = 0x33,
	num_5 = 0x5B,
	num_6 = 0x5F,
	num_7 = 0x70,
	num_8 = 0x7F,
	num_9 = 0x7B,
	A = 0x77,
	B = 0x1F,
	C = 0x4E,
	D = 0x3D,
	E = 0x4F,
	F = 0x47,
	G = 0x7B,
	H = 0x37,
	I = 0x06,
	J = 0x3C,
	//K,
	L = 0x0E,
	M,
	N = 0x15,
	//Ñ,
	O = 0x7E,
	P = 0x67,
	Q = 0x73,
	R = 0x05,
	S = 0x5B,
	//T,
	U = 0x3E,
	//V,
	//W,
	//X,
	Y = 0x3B,
	//Z
	ESPACIO = 0X00,
	ERROR = 0x4F,
	GUION = 0x01
};//7 segmentos en hexa

enum{
	DIGIT_0,
	DIGIT_1,
	DIGIT_2,
	DIGIT_3
}; //Para elegir el dígito



/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void set7Seg(int segs, bool dp);
static void setDigit(int digit);
static void sendChar(int digit, char caract);
static void refreshDisplay(void);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static int pines_select[2]={
					PIN_SEL0,
					PIN_SEL1
};
static int pines_seg[8]={
		PIN_SEGDP,
		PIN_SEGA,
		PIN_SEGB,
		PIN_SEGC,
		PIN_SEGD,
		PIN_SEGE,
		PIN_SEGF,
		PIN_SEGG
};

static int num_7seg[10]={
		num_0,
		num_1,
		num_2,
		num_3,
		num_4,
		num_5,
		num_6,
		num_7,
		num_8,
		num_9
};

static tim_id_t timer_id;
static char word[4];
static float brillo;
static int pointer_digit = -1;
static float opciones_brillo[4] = {DISPLAY_REFRESH_RATE_MS, 3, 4, 6};

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void DRV_Init_Display(void)
{
	gpioMode(pines_select[0], OUTPUT);
	gpioMode(pines_select[1], OUTPUT);
	for (int i=0;i<(sizeof(pines_seg)/sizeof(pines_seg[0]));i++)
		gpioMode(pines_seg[i], OUTPUT);
	timer_id = timerGetId();
	timerStart(timer_id, TIMER_MS_2_TICKS(DISPLAY_REFRESH_RATE_MS), TIM_MODE_PERIODIC, refreshDisplay);
}

void print2display(char str[4])
{
	for(int i=0; i<MAX_DIGIT;i++)
	{
		word[i]=str[i];
	}
}

void setPointer(int digit)
{
	pointer_digit=digit;
}

void changeBright(void)
{
	static int ind = 0;
	ind++;
	if(ind>=4)
	{
		ind=0;
	}
	brillo = opciones_brillo[ind];
	timerStop(timer_id);
	timerStart(timer_id, TIMER_MS_2_TICKS(brillo), TIM_MODE_PERIODIC, refreshDisplay);
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void refreshDisplay(void)
{
	for (int i=0;i<4;i++)
	{
		if (i==pointer_digit)
		{
			sendChar(i, (word[i] | DP));
		}
		else
		{
			sendChar(i, word[i]);
		}
	}
}

void sendChar(int digit, char caract)
{
	set7Seg(ESPACIO, false);
	setDigit(digit);
	bool dp = caract & DP;
	caract &= ~DP;
	if ((caract>=48) && (caract<=57))
	{
		set7Seg(num_7seg[caract-48], dp);
	}
	else if(caract == 'E')
	{
		set7Seg(E, dp);
	}
	else if(caract == 'R')
	{
		set7Seg(R, dp);
	}
	else if(caract==' ')
	{
		set7Seg(ESPACIO, dp);
	}
	else if(caract == '-')
	{
		set7Seg(GUION, dp);
	}
	else
	{
		set7Seg(ERROR, dp);
	}
	set7Seg(ESPACIO, false);
}


void set7Seg(int segs, bool dp)
{
	for (int i=0;i<(sizeof(pines_seg)/sizeof(pines_seg[0]));i++)
	{
		gpioWrite(pines_seg[(sizeof(pines_seg)/sizeof(pines_seg[0]))-1-i], segs & (1<<i));
	}

	gpioWrite(pines_seg[0], dp);
}

void setDigit(int digit)
{
	gpioWrite(pines_select[0], (digit & (1)));
	gpioWrite(pines_select[1], (digit & (1<<1)));
}
 
