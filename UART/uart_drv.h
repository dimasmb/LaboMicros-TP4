#include "MK64F12.h"
#include <stdint.h>
#include "hardware.h"
#include <stdbool.h>

void UART_SetBaudRate(UART_Type *uart, uint32_t baudrate);
void UART_Send_Data(char *tx_data, int datalen);
void UART_Init(int baudrate, char parity); // 0= no parity, 1=even parity, 2=odd  parity
bool inputEmpty();
char retreiveInput();
