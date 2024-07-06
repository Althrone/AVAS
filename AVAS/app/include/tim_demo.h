#ifndef __TIM_DEMO_H__
#define __TIM_DEMO_H__
#include "gpio.h"
#include "tim.h"
#include "isr.h"
#include "isr.h"
void Tim40_SquareOutput(void);
void Tim40_DelayCounter(void);
void Tim40_PwmOutput1_1(uint16_t period, uint16_t duty);
void Tim40_PwmOutput1_2(uint16_t period, uint16_t duty, uint16_t duty1);
void Tim40_PwmOutput1_3(uint16_t period, uint16_t duty, uint16_t duty1, uint16_t duty2);
void Tim40_PwmOutput2_2(uint16_t period, uint16_t duty, uint16_t period1, uint16_t duty1);
void Tim40_EventCount(void);
uint32_t Tim40_GetPulseWidth(void);
uint32_t Tim40_GetPulsePeriod(void);
void tim40_channel0_interrupt(void *msg);
void tim40_channel1_interrupt(void *msg);
void tim41_channel0_interrupt(void *msg);
void Tim41_PwmOutput1_1(uint16_t period, uint16_t duty);
void Tim41_IntervalTimer(void);
void Tim41_SquareOutput(void);
void Tim41_EventCount(void);
void tim41_channel1_interrupt(void *msg);
void tim41_channel2_interrupt(void *msg);
uint32_t Tim41_Get_PulseWidth(TIM_Pulse_t PulseEdge);


#endif
