#ifndef _ISR_H__
#define _ISR_H__
#include <stdbool.h>
#include <stdlib.h>
#include "common.h"


#define	ISR_SUCC   0
#define	ISR_ERR    1
#define TOTAL_IRQ_NUM	  58

typedef void(*isr_handler_t)(void);








/** @defgroup device_isr_handler_t 
  * @{
  */ 
#pragma pack(1)
typedef struct
{	
	bool IRQ_Flag; 
	isr_handler_t isrHandler;
}device_isr_handler_t;
#pragma pack()

int ISR_Register(IRQn_Type irqnum, isr_handler_t cb);
int ISR_DeRegister(IRQn_Type irqnum);

#endif
