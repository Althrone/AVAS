#ifndef __TIM_DEMO_H__
#define __TIM_DEMO_H__
#include "gpio.h"
#include "tim.h"
#include "isr.h"

typedef enum
{
	INT_IDLE = 0,
	INT_RX = 1,
	INT_TX = 2,
}TIM_DIR_t;

typedef struct 
{
	TIM_DIR_t flag;
	uint32_t len;
	volatile uint8_t *data;
}TIM_FRAME_t;

void Tim40_SquareOutput(void);
void Tim40_DelayCounter(void);
void Tim40_PwmOutput1_1(uint16_t period, uint16_t duty);
void Tim40_PwmOutput1_2(uint16_t period, uint16_t duty, uint16_t duty1);
void Tim40_PwmOutput1_3(uint16_t period, uint16_t duty, uint16_t duty1, uint16_t duty2);
void Tim40_PwmOutput2_2(uint16_t period, uint16_t duty, uint16_t period1, uint16_t duty1);
void Tim40_EventCount(void);
void Tim40_GetPulseWidth_Init(void);
void Tim40_GetPulsePeriod_Init(void);
void tim40_channel0_interrupt(void);
void tim40_channel1_interrupt(void);
void Tim40_IntervalTimer(void);



#endif
