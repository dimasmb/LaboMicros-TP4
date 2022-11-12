/***************************************************************************/ /**
   @file     fsm.c
   @brief    Application functions
   @author   Grupo 3
  ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/


#include "fsm.h"
#include <stdio.h>
#include <stdbool.h>


/*******************************************************************************
 * STATIC VARIABLES
 ******************************************************************************/

static screen myScreen = { .screen = {0, 0, 0, 0, 0, 0, 0, 0}, .screenNum = 0 };
static screenPointer currPointer = {.position = 0, .selected = 0};
static user currUser = { .id = {NAN,NAN,NAN,NAN,NAN,NAN,NAN,NAN}, .password = {NAN,NAN,NAN,NAN,NAN}};

static char blockCounter = 0; //CONTADOR DE INTENTOS FALLIDOS

static user registeredUsers[25];
static int usernum = 0; //NUMERO DE USUARIOS REGISTRADOS EN LA DATABASE

static int adminAddMode = 0;
static int adminDelMode = 0;
static int currUserIndex=-1;


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

eSystemEvent read_event(void);

//derivador de handlers
eSystemState wait_derivate(eSystemEvent);
eSystemState password_derivate(eSystemEvent);
eSystemState block_derivate(eSystemEvent);

//wait state
eSystemState wait_turn_handler(eSystemEvent);
eSystemState wait_press_handler(void);
eSystemState wait_doublepress_handler(void);
eSystemState wait_pass_handler(void);
eSystemState wait_carderror_handler(void);
//password input state
eSystemState password_turn_handler(eSystemEvent);
eSystemState password_press_handler(void);
eSystemState password_doublepress_handler(void);


void clearScreenId();
void clearScreenPass();
void clearUser(void);

void printDisplayERR(void);
void printDisplayPassword(void);
void printDisplayId(void);

char compareArray(int a[], int b[], int size);
char compareCharArray(char a[], char b[], int size);

void clearOldUser(void);
int saveNewUserId(void);
void saveNewUserPass(int currUserIndex);


/*******************************************************************************
 *******************************************************************************
						GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void clearScreenId() {
	int newscr[8] = {0,0,0,0,0,0,0,0 };
	for (int i = 0; i < 8; i++) {
		myScreen.screen[i] = newscr[0];
	}
	currPointer.position = 0;
	currPointer.selected = 0;
	printDisplay();
}

void clearScreenPass() {
	int newscr[8] = {0,0,0,0,NAN,NAN,NAN,NAN};
	for (int i = 0; i < 8; i++) {
		myScreen.screen[i] = newscr[i];
	}
	currPointer.position = 0;
	currPointer.selected = 0;
	printDisplayPassword();
}

// Initialize array of structure with states and event with proper handler
static const sStateMachine asStateMachine[] =
{
	{wait_State, wait_derivate},
	{pswrd_Input_State, password_derivate},
	{block_State, block_derivate}
};

static eSystemEvent local_nextEvent = last_Event;
static bool admin_id = false;

/*Función llenar usuarios permitidos*/
void loadUsers(user userList[], int userQuantity) {
	for (int i = 0; i < userQuantity; i++) {
		registeredUsers[i] = userList[i];
	}
	usernum = userQuantity;
}

/* Función que se llama constantemente en un ciclo infinito */
void FSM_Run(eSystemState *nextState)
{
	// Api read the event


	eSystemEvent eNewEvent = read_event();
	local_nextEvent = eNewEvent;

	if ((*nextState < last_State) && (eNewEvent < last_Event) && (asStateMachine[*nextState].pfStateMachineEvnentHandler != NULL))
	{
		// function call as per the state and event and return the next state of the finite state machine

		*nextState = (*asStateMachine[*nextState].pfStateMachineEvnentHandler)(eNewEvent);
	}
	else
	{
		// Invalid
	}
}

/*******************************************************************************
 *******************************************************************************
						LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

eSystemEvent read_event()
{

	static eSystemEvent evStack[100];
	static int pushPointer = -1;
	static int pullPointer = -1;

	int p = readLectoraStatus(); //eventos lectora de tarjeta
	if (p==VALUEOK_) {
		if (pushPointer++ > 99) { pushPointer = 0; }
		evStack[pushPointer] = Card_Read;
		readLectoraCardInfo(currUser.card);
	}
	else if (p==ERRORi_){
		if (pushPointer++ > 99) { pushPointer = 0; }
		evStack[pushPointer] = Card_Error;
	}

	int i = getEvent(); //eventos encoder
	if (i) {

		if (pushPointer++ > 99) { pushPointer = 0; }

		switch (i) {
		case ENC_TURN_CLOCK:
			evStack[pushPointer] = Right_Turn;
			break;
		case ENC_TURN_ANTI_CLOCK:
			evStack[pushPointer] = Left_Turn;
			break;
		case ENC_SMALL_PRESS:
			evStack[pushPointer] = press;
			break;
		case ENC_LONG_PRESS:
			evStack[pushPointer] = double_press;
			break;
		case ENC_GIANT_PRESS:
			evStack[pushPointer] = giant_press;
		}
	}
	if(pullPointer!=pushPointer){
		if (pullPointer++ > 99) { pullPointer = 0; }
		return evStack[pullPointer];
	}
	else{
		return last_Event;
	}



}

eSystemState wait_derivate(eSystemEvent newEvent) {
	//printf("wait state\n");

	eSystemState newState = wait_State;
	switch (newEvent) {
		case Right_Turn:
			newState = wait_turn_handler(newEvent);
			break;
		case Left_Turn:
			newState = wait_turn_handler(newEvent);
			break;
		case press:
			newState = wait_press_handler();
			break;
		case double_press:
			newState = wait_doublepress_handler();
			break;
		case Card_Read:
			newState = wait_pass_handler();
			break;
		case giant_press:
			changeBright();
			break;
		case Card_Error:
			newState = wait_carderror_handler();
		default:
			break;

	}
	return newState;
}

eSystemState password_derivate(eSystemEvent newEvent) {
	//printf("password state\n");

	eSystemState newState = pswrd_Input_State;

	switch (newEvent) {
			case Right_Turn:
				newState = password_turn_handler(newEvent);
				break;
			case Left_Turn:
				newState = password_turn_handler(newEvent);
				break;
			case press:
				newState = password_press_handler();
				break;
			case double_press:
				newState = password_doublepress_handler();
				break;
			case giant_press:
				changeBright();
				break;
			default:
				break;

	}
	return newState;
}

eSystemState block_derivate(eSystemEvent newEvent) {

	eSystemState newState = wait_State;

	TurnOn_LED_FRDM_RED();
	printDisplayERR();
	for (int i=10000000; i > 0; i--){
		int j=0;
		j++;
	}

	TurnOff_LED_FRDM_RED();
	clearScreenId();
	return newState;
}




// function call to dispatch the amount and return the ideal state
eSystemState wait_carderror_handler(){
	eSystemState nextState = wait_State;

	blockCounter++;
	turnOn_Secs_RED(1);
	if (blockCounter==3) {
		blockCounter = 0;
		nextState = block_State;
	}
	return nextState;

}

eSystemState wait_turn_handler(eSystemEvent newEvent)
{
	if (currPointer.selected == 1)
	{
		if (newEvent == Right_Turn)
		{
			if (myScreen.screen[currPointer.position] == 9)
			{
				myScreen.screen[currPointer.position] = 0;
			}
			else {
				myScreen.screen[currPointer.position]++;
			}
		}
		else if (newEvent == Left_Turn)
		{
			if (myScreen.screen[currPointer.position] == 0)
			{
				myScreen.screen[currPointer.position] = 9;
			}
			else {
				myScreen.screen[currPointer.position]--;
			}
		}
	}
	else
	{
		if (newEvent == Right_Turn)
		{
			if (currPointer.position == MAX_NUM_ID - 1)
			{
				currPointer.position = 0;
				myScreen.screenNum = 0;
				changeState(1);
			}
			else if (currPointer.position == 3) {
				currPointer.position++;
				myScreen.screenNum = 1;
				changeState(3);
			}
			else {
				currPointer.position++;
			}
		}
		else if (newEvent == Left_Turn)
		{
			if (currPointer.position == 0)
			{
				clearUser();
				clearScreenId();
			}
			else if (currPointer.position == 4) {
				currPointer.position--;
				myScreen.screenNum = 0;
				changeState(1);
			}
			else {
				currPointer.position--;
			}
		}


	}

	printDisplay();

	return wait_State;
}

eSystemState wait_press_handler(void)
{

	if (currPointer.selected == 1)
	{
		currPointer.selected = 0;
	}
	else
	{
		currPointer.selected = 1;
	}
	return wait_State;
}

eSystemState wait_doublepress_handler() {		

	int c;
	for (c = 0; c < 8; c++) {
		currUser.id[c] = myScreen.screen[c];
	}

	eSystemState nextState = wait_State;

	char allGood = 0;

	for (int i= 0; i < 25 ; i++) {
		if (compareArray(currUser.id, registeredUsers[i].id, (sizeof(currUser.id) / sizeof(int)))) {
			allGood++;
			currUserIndex=i;
			if (i == 0) { admin_id = true; }

			for (int j = 0; j < MAX_NUM_PSW; j++) {
				currUser.password[j] = registeredUsers[i].password[j];
			}
		}
	}
		

	if(!adminAddMode && !adminDelMode){

		if (allGood) {
			nextState = pswrd_Input_State;
			clearScreenPass();
			
		}
		else {
			blockCounter++;
			turnOn_Secs_RED(1);
			if (blockCounter == 3) {
				blockCounter = 0;
				nextState = block_State;
			}
			clearScreenId();

		}
	}
	if(adminAddMode && !adminDelMode ){

		if(currUserIndex != 0){

			currUserIndex=saveNewUserId();//agregar funcion de guardar id en nuevo usuario y pasar a password con el currUserIndex que va a ser el nuevo id
			if (allGood) {			
				nextState = pswrd_Input_State;
				clearScreenPass();
			}
			

		}
		else{

			if (allGood) {			
				nextState = pswrd_Input_State;			//pasar a esperar contraseña con el id de currState
				clearScreenPass();
			}
		}

	}
	else if (!adminAddMode && adminDelMode ){

			adminDelMode=0;
			clearScreenId();
			clearOldUser();//agregar funcion de  elimina el id de ese usuario si no es el del admin y luego se va al inicio



	}

	return nextState;


}
eSystemState wait_pass_handler() {

	eSystemState nextState = wait_State;

		char allGood = 0;
			for (int i = 0; i < usernum; i++) {
				if (!allGood && compareCharArray(currUser.card, registeredUsers[i].card, (sizeof(currUser.card) / sizeof(char)))) {
					allGood++;

					for (int j = 0; j < MAX_NUM_PSW; j++) {
						currUser.password[j] = registeredUsers[i].password[j];
					}
				}

			}

			if (allGood) {

				clearScreenPass();
				return pswrd_Input_State;
			}
			else{
				blockCounter++;
				turnOn_Secs_RED(1);
				if (blockCounter == 3) {
					blockCounter = 0;
					nextState = block_State;
				}
			}



	return nextState;
}

eSystemState password_turn_handler(eSystemEvent newEvent) {
	if (currPointer.selected == 1)
	{
		if (newEvent == Right_Turn)
		{
			if(currPointer.position==4){
				if (myScreen.screen[currPointer.position] == NAN)
					{
						myScreen.screen[currPointer.position] = 0;
					}
					else {
						myScreen.screen[currPointer.position]++;
					}
			}
			else{
				if (myScreen.screen[currPointer.position] == 9)
				{
					myScreen.screen[currPointer.position] = 0;
				}
				else {
					myScreen.screen[currPointer.position]++;
				}

			}

		}
		else if (newEvent == Left_Turn)
		{
			if (myScreen.screen[currPointer.position] == 0)
			{

				if(currPointer.position==4){
					myScreen.screen[currPointer.position] = NAN;
				}
				else{
					myScreen.screen[currPointer.position] = 9;
				}

			}
			else {
				myScreen.screen[currPointer.position]--;
			}
		}
	}
	else
	{
		if (newEvent == Right_Turn)
		{
			if (currPointer.position == MAX_NUM_PSW - 1)
			{
				currPointer.position = 0;
				myScreen.screenNum = 0;
				changeState(6);

			}
			else if (currPointer.position == 3) {
				currPointer.position++;
				myScreen.screenNum = 1;
				changeState(7);

			}
			else {
				currPointer.position++;
			}
		}
		else if (newEvent == Left_Turn)
		{
			if (currPointer.position == 0)
			{
				clearUser();
				clearScreenId();
				printDisplay();
				return wait_State;
			}
			else if (currPointer.position == 4) {
				currPointer.position--;
				myScreen.screenNum = 0;
				changeState(6);
			}
			else {
				currPointer.position--;
			}
		}


	}

	printDisplayPassword();


	return pswrd_Input_State;
}
eSystemState password_press_handler(void) {

	if (currPointer.selected == 1)
	{
		currPointer.selected = 0;
	}
	else
	{
		currPointer.selected = 1;
	}


	return pswrd_Input_State;

}

eSystemState password_doublepress_handler(void) {

	int newPass[MAX_NUM_PSW];
	int i = 0;
	char allGood = 0;

	if((!adminAddMode  && !adminDelMode) || (adminAddMode && currUserIndex==0) ){

		for (int i = 0; i < MAX_NUM_PSW; i++) {
			newPass[i] = myScreen.screen[i];
		}

		if (compareArray(currUser.password, newPass, (sizeof(currUser.password) / sizeof(int)))) {
			allGood++;
			for (i = 0; i < MAX_NUM_PSW; i++) {
				currUser.password[i] = newPass[i];
			}

		}
	}
	else {
		allGood=1;
	}

	eSystemState nextState = pswrd_Input_State;

	if (allGood ) {
		
		if(currUserIndex==0 && !adminAddMode && !adminDelMode){
			adminAddMode=1;
			TurnOn_LED_FRDM_BLUE();
			TurnOn_LED_FRDM_GREEN();
			TurnOn_LED_FRDM_RED();
		}
		else if(currUserIndex==0 && adminAddMode && !adminDelMode){
			adminAddMode=0;
			adminDelMode=1;
			TurnOff_LED_FRDM_BLUE();
			TurnOff_LED_FRDM_GREEN();
			TurnOff_LED_FRDM_RED();
			TurnOn_LED_FRDM_RED();
			TurnOn_LED_FRDM_GREEN();
			TurnOn_LED_FRDM_BLUE();
		} 
		else if (currUserIndex!=0 && adminAddMode && !adminDelMode){

			adminAddMode=0;
			for (int i = 0; i < MAX_NUM_PSW; i++) {
				registeredUsers[currUserIndex].password[i] = myScreen.screen[i];
			}
			TurnOff_LED_FRDM_BLUE();
			TurnOff_LED_FRDM_GREEN();
			TurnOff_LED_FRDM_RED();

		}
		else
		{
			turnOn_Secs_GREEN(10);
		}
		nextState = wait_State;
	}
	else {
		blockCounter++;
		turnOn_Secs_RED(1);
		clearScreenPass();
		if (blockCounter == 3) {
			blockCounter = 0;
			nextState = block_State;
		}

	}
	clearUser();
	clearScreenId();
	currUserIndex=-1;
	return nextState;

}



/*******************************************************************************
 ******************************************************************************/

 //function to compare array elements
char compareArray(int a[], int b[], int size) {
	int i;
	for (i = 0; i < size; i++) {
		if (a[i] != b[i])
			return 0;
	}
	return 1;
}

char compareCharArray(char a[], char b[], int size) {
	int i;
	for (i = 0; i < size; i++) {
		if (a[i] != b[i])
			return 0;
	}
	return 1;
}


void copyPsw(char a[], char b[], int size) {
	int i;
	for (i = 0; i < size; i++) {
		b[i] = a[i];
	}
	return;
}

void clearUser() {
	for (int i = 0; i < MAX_NUM_ID; i++) {
		currUser.id[i] = NAN;
	}
	for (int i = 0; i < MAX_NUM_PSW; i++) {
		currUser.password[i] = NAN;
	}

	return;

}

void printDisplay() {
	int base = myScreen.screenNum*4;
	char newDisp[4];
	for (int i = 0; i < 4; i++) {
		if (myScreen.screen[base + i] == 10) { newDisp[i] = ' '; }
		else {
			newDisp[i] = myScreen.screen[myScreen.screenNum*4 + i] + '0';
		}
	}
	if (currPointer.position > 3) {
		print2display(newDisp);
		setPointer(currPointer.position - 4);
	}
	else {
		print2display(newDisp);
		setPointer(currPointer.position);
	}
}

void printDisplayPassword() {
	int base = myScreen.screenNum*4;
	char newDisp[4];
	for (int i = 0; i < 4; i++) {
		if (base+i == currPointer.position){
			if (myScreen.screen[base + i] == 10) { newDisp[i] = ' '; }
			else {
				newDisp[i] = myScreen.screen[myScreen.screenNum*4 + i] + '0';
			}
		}
		else if (base+i>4){
			newDisp[i]=' ';
		}
		else{
			newDisp[i] = '-';
		}

	}
	if (currPointer.position > 3) {
		print2display(newDisp);
		setPointer(currPointer.position - 4);
	}
	else {
		print2display(newDisp);
		setPointer(currPointer.position);
	}
}

void printDisplayERR(){
	char newDisp[4];
	newDisp[0] = 'E';
	newDisp[1] = 'R';
	newDisp[2] = 'R';
	newDisp[3] = ' ';
	print2display(newDisp);
	setPointer(0);
}

void clearOldUser(void){

									//aca la idea es borrar el usuario y hay q decrementar usernum y tirar todo el resto un lugar para atras.
	// int i ;
	// char allGood=0;
	// char flag=0;
	// for (i= 0; i < usernum; i++) {
	// 	if (compareArray(currUser.id, registeredUsers[i].id, (sizeof(currUser.id) / sizeof(int)))) {
	// 		allGood++;
	// 		flag=i;
	// 		if (i == 0) { admin_id = true; }

	// 		for (int i = 0; i < MAX_NUM_PSW; i++) {
	// 			currUser.password[i] = registeredUsers[i].password[i];
	// 		}
	// 	}
	// }

	
}
int saveNewUserId(void){

	for(int i=0;i<8;i++){
		registeredUsers[usernum].id[i]=currUser.id[i];
	}
	usernum++;

	return usernum-1;

}

void saveNewUserPass(int currUserIndex){

	for(int i=0;i<5;i++){

		registeredUsers[currUserIndex].password[i]=currUser.password[i];
	}
}
