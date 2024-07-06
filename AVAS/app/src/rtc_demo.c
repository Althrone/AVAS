#include "rtc_demo.h"
volatile uint32_t g_ItIntTaken;

void Rtc_Init()
{
    RTC_InitTypeDef RTC_InitStructure = {0};

    RTC_InitStructure.RTC_Clk = RTC_FSUB; //选择副系统时钟
    RTC_InitStructure.RTC_Time.RTC_Seconds = 0x00;
    RTC_InitStructure.RTC_Time.RTC_Minutes = 0x30;
    RTC_InitStructure.RTC_Time.RTC_Hours = 0x08;
    RTC_InitStructure.RTC_Time.RTC_HourFormat = RTC_HourFormat_12;//RTC_HourFormat_24

    RTC_InitStructure.RTC_Date.RTC_Day = 0x31;// 31天
    RTC_InitStructure.RTC_Date.RTC_WeekDay = SUNDAY;
    RTC_InitStructure.RTC_Date.RTC_Month = 0x08;
    RTC_InitStructure.RTC_Date.RTC_Year = 0x19;
    RTC_InitStructure.RTC_Period = One_Second; //RTC 计数每隔1s产生固定周期中断

    RTC_InitStructure.RTC_Alarm_Onoff = RTC_Alarm_On;//闹钟中断关闭
    RTC_InitStructure.RTC_Alarm.Alarm_Minute = 0x35; //闹钟分钟计数
    RTC_InitStructure.RTC_Alarm.Alarm_Hour  = 0x21;//闹钟小时计数 上午11点0x11
    RTC_InitStructure.RTC_Alarm.Alarm_Week  = 0x7F;//闹钟星期计数
    RTC_InitStructure.RTC_1HZ_Output = RTC_1HZ_Output_Disable; //RTC1HZ diable output

    ISR_Register(RTC_IRQn, rtc_interrupt);     //RTC中断服务路径注册

    RTC_Start();
    RTC_Set_AlarmOn();
}


static void rtc_callback_alarm(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

RTC_Counter_Value_t date_time;
static void rtc_callback_constperiod(void)
{
    /* Start user code. Do not edit comment generated here */
    RTC_Get_CounterValue(&date_time);
    printf("20%02x/%02x/%02x %02x:%02x:%02x\n", date_time.year, date_time.month, date_time.day, date_time.hour, date_time.min, date_time.sec);
    /* End user code. Do not edit comment generated here */
}
void rtc_interrupt(void *msg)
{
    if (RTC->RTCC1 & RTC_RTCC1_WAFG_Msk)       //if generate alarm interrupt
    {
        RTC->RTCC1 &= (uint8_t)~0x10;        // clear WAFG
        INTC_ClearPendingIRQ(RTC_IRQn);                     //clear INTRTC flag
        rtc_callback_alarm();
    }

    if (RTC->RTCC1 & RTC_RTCC1_RIFG_Msk)         //if generate consant period interrupt
    {
        RTC->RTCC1 &= (uint8_t)~0x08; // clear RIFG
        INTC_ClearPendingIRQ(RTC_IRQn);                     // clear INTRTC flag
        rtc_callback_constperiod();
    }
}


/***********************************************************************************************************************
* Function Name: IT_Init
* @brief  This function initializes the IntervalTimer module.
* @param  rtccks - select the operaton clock of RTC/IT
* @param  counter_val - the expected interval timer value
* @return None
***********************************************************************************************************************/
void IT_Init(RTC_Clk_t rtccks, uint16_t counter_val)
{
    PeripheralClock(PER0, CGC_PER0_RTCEN_Msk);

    if (rtccks == RTC_FIL)
    {
        CGC->OSMC  |= 1 << 4;
    }

    MISC->RTCCL |= rtccks;

    RTC->ITMC = 0x0000;//_0000_IT_OPERATION_DISABLE
    INTC_DisableIRQ(IT_IRQn);       // disable INTIT interrupt
    INTC_ClearPendingIRQ(IT_IRQn);  // clear INTIT interrupt flag
    RTC->ITMC = counter_val;

    ISR_Register(IT_IRQn, it_interrupt);     //RTC中断服务路径注册
}
/***********************************************************************************************************************
* Function Name: IT_Start
* @brief This function starts IntervalTimer module operation.
* @param  None
* @return None
***********************************************************************************************************************/
void IT_Start(void)
{
    INTC_ClearPendingIRQ(IT_IRQn); // clear INTIT interrupt flag
    NVIC_ClearPendingIRQ(IT_IRQn); // clear INTIT interrupt flag
    INTC_EnableIRQ(IT_IRQn);       // enable INTIT interrupt
    RTC->ITMC |= 0x8000;//_8000_IT_OPERATION_ENABLE

    /* confirm the IntervalTimer is running */
    while ((RTC->ITMC & 0x8000) == 0U); /* wait ITMC.RINTE == 1 */
}
/***********************************************************************************************************************
* Function Name: IT_Stop
* @brief This function stops IntervalTimer module operation.
* @return None
***********************************************************************************************************************/
void IT_Stop(void)
{
    INTC_DisableIRQ(IT_IRQn);      // disable INTIT interrupt
    INTC_ClearPendingIRQ(IT_IRQn); // clear INTIT interrupt flag
    NVIC_ClearPendingIRQ(IT_IRQn); // clear INTIT interrupt flag
    RTC->ITMC &= (uint16_t)~0x8000;
}

/***********************************************************************************************************************
* Function Name: adc_interrupt
* @brief  UART1 Receive interrupt service routine
* @param  None
* @return None
***********************************************************************************************************************/
void it_interrupt(void *msg)
{
    INTC_ClearPendingIRQ(IT_IRQn);     //clear INTAD interrupt flag
    g_ItIntTaken++;
}
