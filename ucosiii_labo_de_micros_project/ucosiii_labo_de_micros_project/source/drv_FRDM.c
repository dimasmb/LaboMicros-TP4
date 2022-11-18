/*
 * drv_FRDM.c
 *
 *  Created on: 2 sep. 2022
 *      Author: Salta
 */
#include "drv_FRDM.h"
#include "gpio.h"
#include "board.h"
#include "timer.h"

static tim_id_t id_timer;

void DRV_FRDM_Init_LEDs(void){
	gpioMode(PIN_LED_BLUE, OUTPUT);
	gpioWrite(PIN_LED_BLUE, true);
	gpioMode(PIN_LED_GREEN, OUTPUT);
	gpioWrite(PIN_LED_GREEN, true);
	gpioMode(PIN_LED_RED, OUTPUT);
	gpioWrite(PIN_LED_RED, true);

	id_timer=timerGetId();
	return;
}
// PIN_LED_RED    PTB 22
//PIN_LED_GREEN PTE 26
 //PIN_LED_BLUE     PTB21
void TurnOn_LED_FRDM_GREEN(void){
	gpioWrite(PIN_LED_GREEN, false);

}
void TurnOn_LED_FRDM_BLUE(void){
	gpioWrite(PIN_LED_BLUE, false);

}
void TurnOn_LED_FRDM_RED(void){
	gpioWrite (PIN_LED_RED, false);

}
void TurnOff_LED_FRDM_GREEN(void){
	gpioWrite (PIN_LED_GREEN, true);

}
void TurnOff_LED_FRDM_BLUE(void){
	gpioWrite (PIN_LED_BLUE, true);

}
void TurnOff_LED_FRDM_RED(void){
	gpioWrite (PIN_LED_RED, true);

}
void Toggle_LED_FRDM_RED(void){
	gpioToggle(PIN_LED_RED);

}
void Toggle_LED_FRDM_GREEN(void){
	gpioToggle(PIN_LED_GREEN);

}
void Toggle_LED_FRDM_BLUE(void){
	gpioToggle(PIN_LED_BLUE);

}

void turnOn_Secs_RED(int secs)
{
	TurnOn_LED_FRDM_RED();
	timerStart(id_timer, TIMER_MS_2_TICKS(500*secs), TIM_MODE_SINGLESHOT, TurnOff_LED_FRDM_RED);
}

void turnOn_Secs_GREEN(int secs)
{
	TurnOn_LED_FRDM_GREEN();
	timerStart(id_timer, TIMER_MS_2_TICKS(1000*secs), TIM_MODE_SINGLESHOT, TurnOff_LED_FRDM_GREEN);
}

void turnOn_Secs_BLUE(int secs)
{
	TurnOn_LED_FRDM_BLUE();
	timerStart(id_timer, TIMER_MS_2_TICKS(1000*secs), TIM_MODE_SINGLESHOT, TurnOff_LED_FRDM_BLUE);
}



