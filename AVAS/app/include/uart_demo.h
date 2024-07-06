#ifndef __UART_DEMO_H__
#define __UART_DEMO_H__
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "uart.h"
#include "gpio.h"
#include "isr.h"
#define UART_MAX_RECV_LEN       64

typedef enum
{
	UART_INT_IDLE = 0,
	UART_INT_RX = 1,
	UART_INT_TX = 2,
	UART_INT_DMA =3,
}UART_DIR_t;

typedef struct 
{
	UART_DIR_t flag;
	uint32_t len;
	volatile uint8_t *data;
}UART_FRAME_t;

extern unsigned char UART0_RX_BUF[UART_MAX_RECV_LEN];
extern unsigned short UART0_RX_STA ;


int8_t Uart0_Init(uint32_t bound);


void Uart0_Send(uint8_t ch);
char Uart0_Receive(void);

void Uart0_IntSend(uint8_t *tx_buf, uint16_t tx_num);
void uart0_interrupt_send(void);
void uart0_interrupt_receive(void);



#endif
