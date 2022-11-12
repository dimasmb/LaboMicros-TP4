/***************************************************************************//**
  @file     Lectora.c
  @brief    Codigo para utilizar lectora de track 2 de tarjetas.
  @author   Enrique Irigoyen
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <lectora.h>
#include "gpio.h"
#include <stdio.h>
#include "timer.h"
#include "drv_FRDM.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define ARRAY_WD_LENGTH 8
#define ARRAY_LENGTH 40

#define DATA_HIGH 0
#define DATA_LOW 1

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static uint32_t checkParity(char cardValue);
static void callback_changeStatus(void);
static void callback_saveCardValue(void);
static void callback_lectoraTimeOut(void);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

// +ej: static const int temperaturas_medias[4] = {23, 26, 24, 29};+
/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

char cardInformation[40]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static uint32_t statusFlag=WAITING_;
static uint32_t absIndex=0;
static char lect_TimeOut=0;

static tim_id_t timer_id_lect;


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void lectora_init(){
	// aca configuro los tres puertos a usar y las interrupciones.
	gpioMode(PIN_DATA_LECT,INPUT);
	gpioMode(PIN_ENABLE_LECT,INPUT);
	gpioMode(PIN_CLK_LECT,INPUT);

	gpioIRQ(PIN_ENABLE_LECT,GPIO_IRQ_MODE_BOTH_EDGES,callback_changeStatus);
	gpioIRQ(PIN_CLK_LECT,GPIO_IRQ_MODE_FALLING_EDGE,callback_saveCardValue);

	timer_id_lect = timerGetId();


}

uint32_t readLectoraStatus(){

	uint32_t lastStatus=statusFlag;
	if(statusFlag == FINISHED_){

		int LRCPos = 40;
		statusFlag = ERRORi_;
		int parity=0;
		int i=0;
		int LRCb0=0;
		int LRCb1=0;
		int LRCb2=0;
		int LRCb3=0;

		for(i=0;i<41;i++){
			if(i<=LRCPos){
				parity+=checkParity(cardInformation[i]);
				LRCb0+=(cardInformation[i] & 1 );
				LRCb1+=((cardInformation[i] & 2 )>>1);
				LRCb2+=((cardInformation[i] & 4 )>>2);
				LRCb3+=((cardInformation[i] & 8 )>>2);
				if(cardInformation[i]==31){
					LRCPos=i+1;
				}
			}
		}

		if(!parity){  //si todos los chequeos dieron cero, entonces la paridad por caracter esta bien
			if(!(LRCb0 % 2)){
				if(!(LRCb1 % 2)){
					if(!(LRCb2 % 2)){
						if(!(LRCb3 % 2)){
							statusFlag=VALUEOK_;
						}
					}
				}
			}
		}


	}

	if(statusFlag == ERRORi_ || statusFlag == VALUEOK_ ){
		lastStatus = statusFlag;
		statusFlag=WAITING_;
	}

	if(lect_TimeOut){
		statusFlag=WAITING_;
		lect_TimeOut=0;
		printf("salio por timeout");
	}

	return lastStatus;
}

void  readLectoraCardInfo(char AppCardInfo[]){		//Esta funcion devuelve la tarjeta leida y la borra del registro.

	int i=0;
	for(i=0;i<40;i++){
		if((cardInformation[i] & 0b00001111) >9 ){			//si esto sucede es porque no es un caracter numerico
			AppCardInfo[i]=(cardInformation[i] & 0b00000111) + 56;		//quito los dos MSB y convierto a ASCII
		}
		else{
			AppCardInfo[i]=(cardInformation[i] & 0b00001111) + 48;		// convierto a ASCII los caracteres numericos
		}
		cardInformation[i]=0;
	}
	absIndex=0;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static uint32_t checkParity(char cardValue){
	int parity=0;
	parity+=(cardValue & 1 );
	parity+=((cardValue & 2 )>>1);
	parity+=((cardValue & 4 )>>2);
	parity+=((cardValue & 8 )>>3);
	parity+=((cardValue & 16 )>>4);

	return 1-(parity%2);		//devuelvo cero si fue impar porque estaria bien


}

static void callback_saveCardValue(void){  // este es con el interrupt de clock en flanco neg .  En realidad el bitValue tengo q leer un GPIO
	TurnOn_LED_FRDM_BLUE();
	uint32_t realPos=absIndex % ARRAY_WD_LENGTH ;
	uint32_t realIndex= (int)absIndex/ARRAY_WD_LENGTH ;

	if(realPos==5){
		absIndex+=3;
		realIndex+=1;
		realPos=0;
	}
	int lect=gpioRead(PIN_DATA_LECT);
	if(lect==DATA_HIGH && realIndex<40){
		cardInformation[realIndex]|=1<<(realPos);
	}
	absIndex++;

	if(absIndex == 5){
		if(cardInformation[0]!=11){
			absIndex-=1;
			cardInformation[0]=cardInformation[0]>>1;
		}
	}
	TurnOff_LED_FRDM_BLUE();

}

static void callback_changeStatus(void){   // este es la interrup del flanco neg y pos del ENABLE.  En el bitEnable es un puerto en realidad
	TurnOn_LED_FRDM_BLUE();
	int i=0;
	int bitEnable = gpioRead(PIN_ENABLE_LECT);

	if(bitEnable == 0 && (statusFlag == WAITING_ || statusFlag == FINISHED_) ){
		timerStart(timer_id_lect,TIMER_MS_2_TICKS(1000),TIM_MODE_SINGLESHOT,callback_lectoraTimeOut);
		statusFlag=READING_;
		for(i=0;i<40;i++){
			cardInformation[i]=0;
		}
		absIndex=0;
	}else if(bitEnable==1 && statusFlag == READING_){
		timerStop(timer_id_lect);
		statusFlag=FINISHED_;
	}
	TurnOff_LED_FRDM_BLUE();
}

static void callback_lectoraTimeOut(void){
	lect_TimeOut=1;

}


