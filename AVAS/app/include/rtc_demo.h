#ifndef __RTC_DEMO_H__
#define __RTC_DEMO_H__
#include "rtc.h"
#include "isr.h"
void it_interrupt(void *msg);
void rtc_interrupt(void *msg);
void Rtc_Init(void);
void IT_Start(void);
void IT_Stop(void);

#endif
