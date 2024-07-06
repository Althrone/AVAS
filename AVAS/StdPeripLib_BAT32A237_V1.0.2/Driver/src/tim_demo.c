/*******************************************************************************
*                  COPYRIGHT (C) 2021 CMS Technologies Ltd.                    *
*                                                                              *
********************************************************************************
* FileName      : tim_demo.c                                                  *
* Author        :                                                              *
* Version       : 1.0                                                          *
* Date          : 2021.08.13                                                   *
* Description   :                                                              *
* Function List :                                                              *
********************************************************************************/
#include "tim_demo.h"
#include "tim4_user.h"
#include "gpio_demo.h"
#include "uart_demo.h"

uint32_t g_Int_Taken;
uint8_t interval_time_flag;
static TIM_FRAME_t pData;

/***********************************************************************************************************************
* Function Name: Tim4_SquareOutput
* @brief  TIM4 output 50% square wave
* @note   square period calculation： 1/(fclk/32) *(TIM_Period*2) = timeValue (s)
*     for fclk = 32MHz ,timer frequence division is 32, period calculation: 1/1MHz *(TIM_Period*2)
* @return none
***********************************************************************************************************************/
void Tim40_SquareOutput()
{
    TIM_InitTypeDef TIM_InitStructure={};
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_PinAFConfig(GPIO_PORT0, GPIO_Pin_1, GPIO_P01, GROUP_AF_ODEFAULT); //P01 used as TO00 output(P01 default function)

    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Level  = GPIO_Level_LOW;
	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;		
    GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT0, &GPIO_InitStruct);


    TIM_InitStructure.TIM_Channel     = TIM_Channel_0;
    TIM_InitStructure.TIM_ClkDivision = TIM_CLK0_Div1;   //specify the operation clk of tim 
    TIM_InitStructure.TIM_Period[TIM_Index_Channel0]   = 2000;            //specify the number of count clock
    TIM_InitStructure.TIM_Trigger     = TIM_Trigger_Software; //specify the software trigger
    TIM_InitStructure.TIM_Mode        = TIM_Mode_Square;      // square count mode
    TIM_InitStructure.TIM_StartInt    = TIM_StartInt_Disable;
    TIM_Init(TM40,&TIM_InitStructure);
    ISR_Register(TM00_IRQn, tim40_channel0_interrupt);     //TIM40 通道0中断服务路径注册

}

/***********************************************************************************************************************
* Function Name: Tim40_DelayCounter
* @brief  
* @param
* @return none
***********************************************************************************************************************/
void Tim40_DelayCounter()
{
    TIM_InitTypeDef TIM_InitStructure={0};
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_PinAFConfig(GPIO_PORT1, GPIO_Pin_6, GPIO_P16, GROUP_AF_ODEFAULT); //P16 is used as TI01 output

    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Level  = GPIO_Level_LOW;
	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT1, &GPIO_InitStruct);


    TIM_InitStructure.TIM_Channel     = TIM_Channel_1;
    TIM_InitStructure.TIM_ClkDivision = TIM_CLK0_Div1;   //specify the operation clk of tim
    TIM_InitStructure.TIM_Period[TIM_Index_Channel1]  = 3000;                 //specify the number of count clock
    TIM_InitStructure.TIM_Trigger    = TIM_Trigger_IputEdge; //TImn valid input edge is used as a start or capture trigger
    TIM_InitStructure.TIM4_Input     = TIM4_CH1_Input_ELC; // choose ELC event as Timer input
    TIM_InitStructure.TIM_Pulse_Edge = TIM_Pulse_Rising; // Timer valid edge choose
    TIM_InitStructure.TIM_Mode       = TIM_Mode_DelayCount;      // DelayCount mode
    TIM_InitStructure.TIM_StartInt   = TIM_StartInt_Disable;
    TIM_Init(TM40,&TIM_InitStructure);

    ISR_Register(TM01_IRQn, tim40_channel1_interrupt);     //TIM40 通道1中断服务路径注册
}

void tim40_channel1_interrupt(void)
{
    INTC_ClearPendingIRQ(TM01_IRQn);    // clear INTTM01 interrupt flag
    PORT->P1  ^= (1 << 6);
	interval_time_flag = 2;
}

/***********************************************************************************************************************
* Function Name: Tim40_Single_PWM
* @brief  TIM4 output single pwm according master's  delay
* @param  delay - the delay of master channel
* @param  period - the period of slave channel
* @return none
***********************************************************************************************************************/
void Tim40_Single_PWM(uint16_t delay, uint16_t period)
{
    TIM_InitTypeDef TIM_InitStructure = {0};
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_PinAFConfig(GPIO_PORT1, GPIO_Pin_6, GPIO_P16, GROUP_AF_ODEFAULT); //P16 default used as TO01 output

    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Level  = GPIO_Level_LOW;
	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;	
    GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT1, &GPIO_InitStruct);

    TIM_InitStructure.TIM_Selection_Master = TIM_Channel_0;//when multi-tim combination,it can generate pwm wave
    TIM_InitStructure.TIM_Channel     = TIM_Channel_1;
    TIM_InitStructure.TIM_ClkDivision = TIM_CLK0_Div32768;   //specify the operation clk of tim
    TIM_InitStructure.TIM_Period[TIM_Index_Channel0]   = delay;                 //specify the number of count clock
    TIM_InitStructure.TIM_Period[TIM_Index_Channel1]   = period;                 //specify duty

    TIM_InitStructure.TIM_Trigger     = TIM_Trigger_IputEdge; //specify the software trigger
    TIM_InitStructure.TIM_Mode        = TIM_Mode_SinglePulse;   // PWM_Master mode
    TIM_InitStructure.TIM_Pulse_Edge  = TIM_Pulse_Rising;      // Timer valid edge choose
    TIM_InitStructure.TIM_StartInt    = TIM_StartInt_Disable;   //the relationship between startCount and interrupt setting
    TIM_Init(TM40,&TIM_InitStructure);

    ISR_Register(TM00_IRQn, tim40_channel0_interrupt);
}
/***********************************************************************************************************************
* Function Name: Tim40_PwmOutput1_1
* @brief  TIM4 output 1 PWM which can select TIMER40 channel0 as master channel
* @param  period - the period of PWM waveform
* @note   Pwm period calculation： 1/(fclk/32) *(MasterChan_TIM_Period*2) = periodValue (s)
*         for fclk = 32MHz ,timer frequence division is 32, when select TTM_Channel_0 as master channel
*         Pwm period calculation: 1/1MHz *(TIM_Period[0]*2)  
* @param  duty - the duty factor of PWM waveform
* @note   duty time calculation： 1/(fclk/32) *(TIM_Period) = dutyValue (s)
*         for fclk = 32MHz ,timer frequence division is 32, when select TTM_Channel_1 as normal channel
*         Pwm period calculation: 1/1MHz *TIM_Period[1] 
* @return none
***********************************************************************************************************************/
void Tim40_PwmOutput1_1(uint16_t period, uint16_t duty)
{
    TIM_InitTypeDef TIM_InitStructure = {0};
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_PinAFConfig(GPIO_PORT1, GPIO_Pin_6, GPIO_P16, GROUP_AF_ODEFAULT); //P16 default used as TO01 output

    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Level  = GPIO_Level_LOW;
	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;	
    GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT1, &GPIO_InitStruct);

    TIM_InitStructure.TIM_Selection_Master = TIM_Channel_0;//when multi-tim combination,it can generate pwm wave
    TIM_InitStructure.TIM_Channel     = TIM_Channel_1;  //|TTM_Channel_1
    TIM_InitStructure.TIM_ClkDivision = TIM_CLK0_Div1;   //specify the operation clk of tim
    TIM_InitStructure.TIM_Period[TIM_Index_Channel0] = period;            //specify the number of count clock
    TIM_InitStructure.TIM_Period[TIM_Index_Channel1] = duty;              //specify duty

    TIM_InitStructure.TIM_Trigger   = TIM_Trigger_Software; //specify the software trigger
    TIM_InitStructure.TIM_Mode      = TIM_Mode_PWM_Master;   // PWM_Master mode
    TIM_InitStructure.TIM_StartInt  = TIM_StartInt_Disable; //the relationship between startCount and interrupt setting
    TIM_Init(TM40,&TIM_InitStructure);
}

/***********************************************************************************************************************
* Function Name: Tim40_PwmOutput1_2
* @brief  TIM4 output 2 PWM which can select TIMER40 channel0 as master channel,channel1/channel2 as PWM output
* @param  period - the period of PWM waveform
* @note   Pwm period calculation： 1/(fclk/32) *(MasterChan_TIM_Period*2) = periodValue (s)
*         for fclk = 32MHz ,timer frequence division is 32, when select TTM_Channel_0 as master channel
*         Pwm period calculation: 1/1MHz *(TIM_Period[0]*2)  
* @param  duty - the duty factor of PWM waveform
* @note   duty time calculation： 1/(fclk/32) *(TIM_Period) = dutyValue (s)
*         for fclk = 32MHz ,timer frequence division is 32, when select TTM_Channel_1 as normal channel
*         Pwm period calculation: 1/1MHz *TIM_Period[1] 
* @return none
***********************************************************************************************************************/
void Tim40_PwmOutput1_2(uint16_t period, uint16_t duty, uint16_t duty1)
{

    TIM_InitTypeDef TIM_InitStructure = {};
    GPIO_InitTypeDef GPIO_InitStruct = {};

    GPIO_PinAFConfig(GPIO_PORT1, GPIO_Pin_6, GPIO_P16, GROUP_AF_ODEFAULT); //P16 default used as TO01 output
    GPIO_PinAFConfig(GPIO_PORT1, GPIO_Pin_7, GPIO_P17, GROUP_AF_ODEFAULT); //P17 default used as TO02 output

    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Level  = GPIO_Level_LOW;
	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;		
    GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT1, &GPIO_InitStruct);

    TIM_InitStructure.TIM_Selection_Master = TIM_Channel_0;//when multi-tim combination,it used as master channel
    TIM_InitStructure.TIM_Channel     = TIM_Channel_1 | TIM_Channel_2; //|TTM_Channel_2|TTM_Channel_3
    TIM_InitStructure.TIM_ClkDivision = TIM_CLK0_Div1;   //specify the operation clk of tim
    TIM_InitStructure.TIM_Period[TIM_Index_Channel0] = period;                 //specify the number of count clock
    TIM_InitStructure.TIM_Period[TIM_Index_Channel1] = duty;                 //specify duty
    TIM_InitStructure.TIM_Period[TIM_Index_Channel2] = duty1;                   //specify duty1

    TIM_InitStructure.TIM_Trigger   = TIM_Trigger_Software; //specify the software trigger
    TIM_InitStructure.TIM_Mode      = TIM_Mode_PWM_Master;      // PWM_Master mode
    TIM_InitStructure.TIM_StartInt  = TIM_StartInt_Enable;  //the relationship between startCount and interrupt setting
    TIM_Init(TM40,&TIM_InitStructure);
}

/***********************************************************************************************************************
* Function Name: Tim40_PwmOutput1_3
* @brief  TIM4 output 3 PWM which can select TIMER40 channel0 as master channel,channel1/channel2/channel3 as PWM output
* @param  period - the period of PWM waveform
* @note   Pwm period calculation： 1/(fclk/32) *(MasterChan_TIM_Period*2) = periodValue (s)
*         for fclk = 32MHz ,timer frequence division is 32, when select TTM_Channel_0 as master channel
*         Pwm period calculation: 1/1MHz *(TIM_Period[0]*2)  
* @param  duty - the duty factor of PWM waveform
* @note   duty time calculation： 1/(fclk/32) *(TIM_Period) = dutyValue (s)
*         for fclk = 32MHz ,timer frequence division is 32, when select TTM_Channel_1 as normal channel
*         Pwm period calculation: 1/1MHz *TIM_Period[1] 
* @return none
***********************************************************************************************************************/
void Tim40_PwmOutput1_3(uint16_t period, uint16_t duty, uint16_t duty1, uint16_t duty2)
{
    TIM_InitTypeDef TIM_InitStructure = {};
    GPIO_InitTypeDef GPIO_InitStruct = {};

    GPIO_PinAFConfig(GPIO_PORT1, GPIO_Pin_6, GPIO_P16, GROUP_AF_ODEFAULT); //P16 default used as TO01 output
    GPIO_PinAFConfig(GPIO_PORT1, GPIO_Pin_7, GPIO_P17, GROUP_AF_ODEFAULT); //P17 default used as TO02 output
    GPIO_PinAFConfig(GPIO_PORT3, GPIO_Pin_1, GPIO_P31, GROUP_AF_ODEFAULT); //P31 default used as TO03 output

    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Level  = GPIO_Level_LOW;
	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT1, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Level  = GPIO_Level_LOW;
	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT3, &GPIO_InitStruct);

    TIM_InitStructure.TIM_Selection_Master = TIM_Channel_0;//when multi-tim combination,it can generate pwm wave
    TIM_InitStructure.TIM_Channel     = TIM_Channel_1 | TIM_Channel_2 | TIM_Channel_3; //|TTM_Channel_2
    TIM_InitStructure.TIM_ClkDivision = TIM_CLK0_Div1;   //specify the operation clk of tim
    TIM_InitStructure.TIM_Period[TIM_Index_Channel0]   = period;                 //specify the number of count clock
    TIM_InitStructure.TIM_Period[TIM_Index_Channel1]   = duty;                 //specify duty
    TIM_InitStructure.TIM_Period[TIM_Index_Channel2]   = duty1;                   //specify duty1
    TIM_InitStructure.TIM_Period[TIM_Index_Channel3]   = duty2;                   //specify duty2
    TIM_InitStructure.TIM_Trigger     = TIM_Trigger_Software; //specify the software trigger
    TIM_InitStructure.TIM_Mode        = TIM_Mode_PWM_Master;      // PWM_Master mode
    TIM_InitStructure.TIM_StartInt    = TIM_StartInt_Enable;    //the relationship between startCount and interrupt setting
    TIM_Init(TM40,&TIM_InitStructure);
}

/***********************************************************************************************************************
* Function Name: Tim40_PwmOutput2_2
* @brief  TIM4 output 2 PWM which  selects TIMER40 channel0 as master channel for channel1,channel1 output PWM
*			 channel2 as master channel for channel3,channel3 output PWM
* @param  period - the period of PWM waveform
* @note   Pwm period calculation： 1/(fclk/32) *(MasterChan_TIM_Period*2) = periodValue (s)
*         for fclk = 32MHz ,timer frequence division is 32, when select TTM_Channel_0 as master channel
*         Pwm period calculation: 1/1MHz *(TIM_Period[0]*2)  
* @param  duty - the duty factor of PWM waveform
* @note   duty time calculation： 1/(fclk/32) *(TIM_Period) = dutyValue (s)
*         for fclk = 32MHz ,timer frequence division is 32, when select TTM_Channel_1 as normal channel
*         Pwm period calculation: 1/1MHz *TIM_Period[1] 
* @return none
***********************************************************************************************************************/

void Tim40_PwmOutput2_2(uint16_t period, uint16_t duty, uint16_t period1, uint16_t duty1)
{
    TIM_InitTypeDef TIM_InitStructure = {0};
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_PinAFConfig(GPIO_PORT1, GPIO_Pin_6, GPIO_P16, GROUP_AF_ODEFAULT); //P16 default used as TO01 output

    GPIO_PinAFConfig(GPIO_PORT3, GPIO_Pin_1, GPIO_P31, GROUP_AF_ODEFAULT); //P31 default used as TO03 output

    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_6 ;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Level  = GPIO_Level_LOW;
	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT1, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Level  = GPIO_Level_LOW;
	GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT3, &GPIO_InitStruct);

    TIM_InitStructure.TIM_Selection_Master = TIM_Channel_0 | TIM_Channel_2; //when multi-tim combination,it can generate pwm wave
    TIM_InitStructure.TIM_Channel = TIM_Channel_1 | TIM_Channel_3; //|TTM_Channel_2|TTM_Channel_3
    TIM_InitStructure.TIM_ClkDivision = TIM_CLK0_Div1;   //specify the operation clk of tim
    TIM_InitStructure.TIM_Period[TIM_Index_Channel0]   = period;                 //specify the number of count clock
    TIM_InitStructure.TIM_Period[TIM_Index_Channel1]   = duty;                 //specify duty
    TIM_InitStructure.TIM_Period[TIM_Index_Channel2]   = period1;                   //specify duty1
    TIM_InitStructure.TIM_Period[TIM_Index_Channel3]   = duty1;                   //specify duty2
    TIM_InitStructure.TIM_Trigger     = TIM_Trigger_Software; //specify the software trigger
    TIM_InitStructure.TIM_Mode        = TIM_Mode_PWM_Master;      // PWM_Master mode
    TIM_InitStructure.TIM_StartInt    = TIM_StartInt_Enable;    //the relationship between startCount and interrupt setting
    TIM_Init(TM40,&TIM_InitStructure);
}
/***********************************************************************************************************************
* Function Name: Tim4_EventCount
* @brief  TIM4 specified channel operates as external event counter.
* @param
* @return none
***********************************************************************************************************************/
void Tim40_EventCount()
{
    TIM_InitTypeDef TIM_InitStructure= {};
    GPIO_InitTypeDef GPIO_InitStruct = {};

    GPIO_PinAFConfig(GPIO_PORT0, GPIO_Pin_0, GPIO_P00, GROUP_AF_ODEFAULT); //P00 used as TI00 input

    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT0, &GPIO_InitStruct);

    TIM_InitStructure.TIM_Channel = TIM_Channel_0 ;
    TIM_InitStructure.TIM_ClkDivision = TIM_CLK0_Div1;   //specify the operation clk of tim
    TIM_InitStructure.TIM_Period[TIM_Index_Channel0]   = 10;                 //specify the count num of event
    TIM_InitStructure.TIM_Trigger     = TIM_Trigger_Software; //specify the software trigger
    TIM_InitStructure.TIM_Pulse_Edge  = TIM_Pulse_Rising;  // TIM effective edge choose
    TIM_InitStructure.TIM_Mode        = TIM_Mode_EventCount;      // event count
    TIM_InitStructure.TIM_StartInt    = TIM_StartInt_Disable;
    TIM_InitStructure.TIM4_Input      = TIM4_CH0_Input_TI00;  // specify the input of TI
    TIM_Init(TM40,&TIM_InitStructure);

    ISR_Register(TM00_IRQn, tim40_channel0_interrupt);     //TIM40 通道0中断服务路径注册
}

/***********************************************************************************************************************
* Function Name: Tim4_GetPulseWidth
* @brief This function measures the high level pulse width of P00.
* @param
* @return none
***********************************************************************************************************************/
void Tim40_GetPulseWidth_Init(void)
{
    TIM_InitTypeDef TIM_InitStructure = {};
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_PinAFConfig(GPIO_PORT0, GPIO_Pin_0, GPIO_P00, GROUP_AF_ODEFAULT); //P00 used as TI00 input

    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT0, &GPIO_InitStruct);

    TIM_InitStructure.TIM_Channel = TIM_Channel_0;
    TIM_InitStructure.TIM_ClkDivision = TIM_CLK0_Div1;   //specify the operation clk of tim
    TIM_InitStructure.TIM_Trigger     = TIM_Trigger_BothEdge; //bothedge :start trigger and capture trigger
    TIM_InitStructure.TIM_Pulse_Edge  = TIM_Pulse_Both_Width_High;  // Measure the high or low level pulse width of P00/TI00
    TIM_InitStructure.TIM_Mode        = TIM_Mode_Measure;      // pulse width measure
    TIM_InitStructure.TIM_StartInt    = TIM_StartInt_Disable;
    TIM_InitStructure.TIM4_Input      = TIM4_CH0_Input_TI00;  // specify the input of TI

    TIM_Init(TM40,&TIM_InitStructure);
}

/***********************************************************************************************************************
* Function Name: TM40_Get_PulsePeriod
* @brief  This function measures the pulse period with polling mode for TM4 module.
* @return period - return the pulse period at specified edge
***********************************************************************************************************************/
void Tim40_GetPulsePeriod_Init(void)
{
    TIM_InitTypeDef TIM_InitStructure = {};
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_PinAFConfig(GPIO_PORT0, GPIO_Pin_0, GPIO_P00, GROUP_AF_ODEFAULT); //P00 used as TI00 input

    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT0, &GPIO_InitStruct);

    TIM_InitStructure.TIM_Channel     = TIM_Channel_0;
    TIM_InitStructure.TIM_ClkDivision = TIM_CLK0_Div1;   //specify the operation clk of tim
    TIM_InitStructure.TIM_Trigger     = TIM_Trigger_IputEdge; //IputEdge is used to start trigger and capture trigger
    TIM_InitStructure.TIM_Pulse_Edge  = TIM_Pulse_Rising;  // Measure the high or low level pulse width of P00/TI00  /TIM_Pulse_Both/TIM_Pulse_Rising

    TIM_InitStructure.TIM_Mode        = TIM_Mode_PluseInterval;  // pulse interval measure: use capture function
    TIM_InitStructure.TIM_StartInt    = TIM_StartInt_Disable;
    TIM_InitStructure.TIM4_Input      = TIM4_CH0_Input_TI00;  // specify the input of TI
    TIM_Init(TM40,&TIM_InitStructure);
}


/***********************************************************************************************************************
* Function Name: Tim40_IntervalTimer
* @brief This function interval timer and generate interval interrupt.
* @note   Interval timer period calculation： 1/(fclk/32768) *TIM_Period = timerValue (s)
*         for fclk = 32MHz ,timer frequence division is 32768, TTM_Channel_0 as interval timer， 
*         TIM_Period is 20；   timerValue = 1/1000* 20
* @return none
***********************************************************************************************************************/
void Tim40_IntervalTimer()
{
    TIM_InitTypeDef TIM_InitStructure = {};

    TIM_InitStructure.TIM_Channel = TIM_Channel_0 ;
    // TIM_InitStructure.TIM_Channel = TIM_Channel_0 | TIM_Channel_1 ;
    TIM_InitStructure.TIM_ClkDivision = TIM_CLK0_Div1;  // 48M输入
    // TIM_InitStructure.TIM_ClkDivision = TIM_CLK0_Div32768;  // specify the operation clk=1khz of tim for 32Mhz fclk
    TIM_InitStructure.TIM_Period[TIM_Index_Channel0]   = 1000;   //specify the  time =20*(1/1000) =20ms
	// TIM_InitStructure.TIM_Period[TIM_Index_Channel1]   = 5000;   //specify the  time =20*(1/1000) =20ms
		
    TIM_InitStructure.TIM_Trigger     = TIM_Trigger_Software; //specify the software trigger
    TIM_InitStructure.TIM_Mode        = TIM_Mode_Interval;  // external event
    TIM_InitStructure.TIM_StartInt    = TIM_StartInt_Disable;
    TIM_Init(TM40,&TIM_InitStructure);

    ISR_Register(TM00_IRQn, tm40_channel0_interrupt); //TIM40 通道0 中断服务注册
    // ISR_Register(TM01_IRQn, tim40_channel1_interrupt); //TIM40 通道1 中断服务注册
}

/***********************************************************************************************************************
* Function Name: tim40_channel0_interrupt
* @brief  TIM4 Channel interrupt service routine
* @param  msg
* @return None
***********************************************************************************************************************/
void tim40_channel0_interrupt(void)
{
    TIM_FRAME_t *pFrame = &pData;

    INTC_ClearPendingIRQ(TM00_IRQn);    // clear INTTM00 interrupt flag
    g_Int_Taken++;
    pFrame->flag = INT_RX;              //interrupt is generated
	interval_time_flag = 1;
    if (1U == (TM40->TSR[0] & _0001_TM4_OVERFLOW_OCCURS))
    {
        pFrame->len = (uint32_t)(TM40->TDR[0] + 1UL) + 0x10000UL;
    }
    else
    {
        pFrame->len = (uint32_t)(TM40->TDR[0] + 1UL);
    }
}
