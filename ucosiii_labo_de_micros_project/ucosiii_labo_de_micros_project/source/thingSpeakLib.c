#include "uart_drv.h"
#include "thingSpeakLib.h"


// Arrays.
static char result[6]={0,0,0,0,0,0};
static char keepAliveBuffer[6]={0xAA,0x55,0xC3,0x3C,0x01,0x02};
static char sendDataBuffer[12]={0xAA,0x55,0xC3,0x3C,0x07,0x01,0x0,0x0,0x0,0x0,0x0,0x0};


// Headers.


void sendData(int p1,int p2,int p3){
    int2uint16LE(p1,p2,p3,result);
    writeSendDataBuff();
    UART_Send_Data(sendDataBuffer,12);
}

//Functions.
void int2uint16LE(int p1,int p2,int p3,char *result){
	if(p1>4095){
		result[0]=0;
		result[1]=0;
	}
	else{
		result[0]=(p1&0xFF);
		result[1]=((p1>>8)&0xFF);
	}

	if(p2>4095){
		result[2]=0;
		result[3]=0;
	}
	else{
		result[2]=(p2&0xFF);
		result[3]=((p2>>8)&0xFF);
	}

	if(p3>4095){
			result[4]=0;
			result[5]=0;
		}
		else{
			result[4]=(p3&0xFF);
			result[5]=((p3>>8)&0xFF);
		}

}

void writeSendDataBuff(void){

	int m = 0;
	for(m=0;m<6;m++){
		sendDataBuffer[6+m]=result[m];
	}
}