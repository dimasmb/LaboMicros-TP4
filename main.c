#include "hardware.h"
#include "fsm.h"
#include  <os.h>

/* LEDs */
#define LED_R_PORT            PORTB
#define LED_R_GPIO            GPIOB
#define LED_G_PORT            PORTE
#define LED_G_GPIO            GPIOE
#define LED_B_PORT            PORTB
#define LED_B_GPIO            GPIOB
#define LED_R_PIN             22
#define LED_G_PIN             26
#define LED_B_PIN             21
#define LED_B_ON()           (LED_B_GPIO->PCOR |= (1 << LED_B_PIN))
#define LED_B_OFF()          (LED_B_GPIO->PSOR |= (1 << LED_B_PIN))
#define LED_B_TOGGLE()       (LED_B_GPIO->PTOR |= (1 << LED_B_PIN))
#define LED_G_ON()           (LED_G_GPIO->PCOR |= (1 << LED_G_PIN))
#define LED_G_OFF()          (LED_G_GPIO->PSOR |= (1 << LED_G_PIN))
#define LED_G_TOGGLE()       (LED_G_GPIO->PTOR |= (1 << LED_G_PIN))
#define LED_R_ON()           (LED_R_GPIO->PCOR |= (1 << LED_R_PIN))
#define LED_R_OFF()          (LED_R_GPIO->PSOR |= (1 << LED_R_PIN))
#define LED_R_TOGGLE()       (LED_R_GPIO->PTOR |= (1 << LED_R_PIN))

/* Task Start */
#define TASKSTART_STK_SIZE 		512u
#define TASKSTART_PRIO 			2u
static OS_TCB TaskStartTCB;
static CPU_STK TaskStartStk[TASKSTART_STK_SIZE];

/* Task Lectora*/
#define TASK_lectora_STK_SIZE			256u
#define TASK_lectora_STK_SIZE_LIMIT	(TASK_lectora_STK_SIZE / 10u)
#define TASK_lectora_PRIO              2u
static OS_TCB Task_lectora_TCB;
static CPU_STK TaskLectoraStk[TASK_lectora_STK_SIZE];
static OS_SEM semTest_lectora;


/* Task 3 */
#define TASK3_STK_SIZE			256u
#define TASK3_STK_SIZE_LIMIT	(TASK3_STK_SIZE / 10u)
#define TASK3_PRIO              2u

static OS_TCB TaskEncoder;
static CPU_STK TaskEncoderStk[TASK3_STK_SIZE];
static OS_SEM semTest_encoder;


/* Message Queue */
static OS_Q AppQ;

/* Example semaphore */


static void TaskEncoderFunc(void *p_arg)
{
    (void)p_arg;
    OS_ERR os_err;
    CPU_TS ts;

    DRV_Init_Display();
    DRV_Init_LEDs();
    DRV_Init_Switch(&semTest_encoder, &os_err);
    printDisplay();
    while (1) {
        OSSemPend(&semTest_encoder, 0, OS_OPT_PEND_BLOCKING, &ts, &os_err);
        encoder_process();
    }
}

static void TaskLectoraFunc(void *p_arg) {
    (void)p_arg;
    OS_ERR os_err;
    CPU_TS ts;

    lectora_init(&semTest_lectora, &os_err);

    while (1) {
        OSSemPend(&semTest_lectora, 0, OS_OPT_PEND_BLOCKING, &ts, &os_err);
        lectora_process();
    }
}


static void TaskStart(void *p_arg) {
    (void)p_arg;
    OS_ERR os_err;

    /* Initialize the uC/CPU Services. */
    CPU_Init();

#if OS_CFG_STAT_TASK_EN > 0u
    /* (optional) Compute CPU capacity with no task running */
    OSStatTaskCPUUsageInit(&os_err);
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif


    /******************
     * TP1
    */
   //hw_Init();
    hw_DisableInterrupts();
    timerInit();


    DRV_FRDM_Init_LEDs();
    hw_EnableInterrupts();

    eSystemState eNextState = wait_State;
	user list[4] = {{.id = {1,0,0,0,0,0,0,0}, .password = {1,0,0,0,NAN}},
		{.id = {2,0,0,0,0,0,0,0}, .password = {2,0,0,0,0}},
		//TARJETA ITBA ique
		{.card = {';', '4','5','1','7','6','6','0','1','3','8','9','6','5','7','8','4',
				'=','2','2','0','4','1','2','1','2','3','6','0','0','4','2','1','7','7',
				'1','9','5','?','>'}, .id = {3,8,9,6,5,7,8,4}, .password = {2,1,0,0,0}},

		//SUBE joaco
		{.card = {';', '6','0','6','1','2','6','8','2','6','4','8','5','8','2','2','2',
				'=','2','5','0','6','1','0','1','0','0','0','0','0','?','2','0','0','0',
				'0','0','0','0','0'}, .id = {4,8,9,6,5,7,8,4}, .password = {0,1,0,0,NAN}}};

	loadUsers(list, 4);
	//printDisplay();
	for (;;)
	{
		FSM_Run(&eNextState); /* Program-specific loop  */
	}
   /****************/

    // while (1) {
    //     OSTimeDlyHMSM(0u, 0u, 1u, 0u, OS_OPT_TIME_HMSM_STRICT, &os_err);
    //     //LED_G_TOGGLE();
    // }
}

int main(void) {
    OS_ERR err;
    OS_ERR os_err;

#if (CPU_CFG_NAME_EN == DEF_ENABLED)
    CPU_ERR  cpu_err;
#endif

    hw_Init();

    /* RGB LED */
    SIM->SCGC5 |= (SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTE_MASK);
    LED_B_PORT->PCR[LED_B_PIN] = PORT_PCR_MUX(1);
    LED_G_PORT->PCR[LED_G_PIN] = PORT_PCR_MUX(1);
    LED_R_PORT->PCR[LED_R_PIN] = PORT_PCR_MUX(1);
    LED_B_GPIO->PDDR |= (1 << LED_B_PIN);
    LED_G_GPIO->PDDR |= (1 << LED_G_PIN);
    LED_R_GPIO->PDDR |= (1 << LED_R_PIN);
    LED_B_GPIO->PSOR |= (1 << LED_B_PIN);
    LED_G_GPIO->PSOR |= (1 << LED_G_PIN);
    LED_R_GPIO->PSOR |= (1 << LED_R_PIN);

    OSInit(&err);
 #if OS_CFG_SCHED_ROUND_ROBIN_EN > 0u
	 /* Enable task round robin. */
	 OSSchedRoundRobinCfg((CPU_BOOLEAN)1, 0, &err);
 #endif
    OS_CPU_SysTickInit(SystemCoreClock / (uint32_t)OSCfg_TickRate_Hz);

    OSQCreate(&AppQ, 
            "Example Q", 
            10, 
            &err);

    OSTaskCreate(&TaskStartTCB,
                 "App Task Start",
                  TaskStart,
                  0u,
                  TASKSTART_PRIO,
                 &TaskStartStk[0u],
                 (TASKSTART_STK_SIZE / 10u),
                  TASKSTART_STK_SIZE,
                  0u,
                  0u,
                  0u,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 &err);

    /* Create semaphore pal task_lectora*/
    OSSemCreate(&semTest_lectora, "Sem Test_lectora", 0u, &os_err);
    OSSemCreate(&semTest_encoder, "Sem Test_encoder", 0, &os_err);
    

    // /* Create Task_lectora*/
    OSTaskCreate(&Task_lectora_TCB, 			//tcb
                 "Task_lectora",				//name
                   TaskLectoraFunc,				//func
                   0u,					//arg
                   TASK_lectora_PRIO ,			//prio
                  &TaskLectoraStk[0u],			//stack
                   TASK_lectora_STK_SIZE_LIMIT,	//stack limit
                   TASK_lectora_STK_SIZE,		//stack size
                   0u,
                   0u,
                   0u,
                  (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                  &os_err);

      /* Create Task3 */
     OSTaskCreate(&TaskEncoder, 			//tcb
                  "Task Encoder",				//name
                  TaskEncoderFunc,				//func
                  0u,					//arg
                  TASK3_PRIO,			//prio
                  &TaskEncoderStk[0u],			//stack
                  TASK3_STK_SIZE_LIMIT,	//stack limit
                  TASK3_STK_SIZE,		//stack size
                  0u,
                  0u,
                  0u,
                  (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                  &os_err);


    OSStart(&err);

	/* Should Never Get Here */
    while (1) {

    }
}
