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
#include "uart_demo.h"

uint32_t g_Int_Taken;
uint8_t interval_time_flag;
/***********************************************************************************************************************
* Function Name: Tim4_SquareOutput
* @brief  TIM4 output 50% square wave
* @note   square period calculation£º 1/(fclk/32) *(TIM_Period*2) = timeValue (s)
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
    TIM_InitStructure.TIM_Period[TIM_Index_Channel0]   = 1500;            //specify the number of count clock
    TIM_InitStructure.TIM_Trigger     = TIM_Trigger_Software; //specify the software trigger
    TIM_InitStructure.TIM_Mode        = TIM_Mode_Square;      // square count mode
    TIM_InitStructure.TIM_StartInt    = TIM_StartInt_Disable;
    TIM_Init(TM40, &TIM_InitStructure);
}



















