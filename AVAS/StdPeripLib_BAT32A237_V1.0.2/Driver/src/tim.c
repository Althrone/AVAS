/*******************************************************************************
*                  COPYRIGHT (C) 2021 CMS Technologies Ltd.                    *
*                                                                              *
********************************************************************************
* FileName      : tim.c                                                  *
* Author        :                                                              *
* Version       : 1.0                                                          *
* Date          : 2021.08.13                                                   *
* Description   :                                                              *
* Function List :                                                              *
********************************************************************************/
#include "tim.h"
#include "cgc.h"

#define TIM_CHAN_MAX_NUM    4u
#define TIM_MAX_NUM    		1u
/* Private variables ---------------------------------------------------------*/
static IRQn_Type TIM_IRQTable[TIM_CHAN_MAX_NUM] = 
{TM00_IRQn, TM01_IRQn, TM02_IRQn, TM03_IRQn};


/**
  * @brief  Enables the specified TIMER channel.
  * @param  TIMx: the specified tim, it can be TIM40 TIM41
  * @param  ChxIdx where ChxIdx can be 0, 1,2,3.
  *         @arg TTM_Index_Channel0
  *         @arg TTM_Index_Channel1
  *         @arg TTM_Index_Channel2
  *         @arg TTM_Index_Channel3
  * @retval None
  */
void TIM_Start(TM40_Type* TM4x, uint8_t ChxIdx)
{
	assert_param(IS_TIM(TM4x));	
	assert_param(IS_TIM_CHANIDEX(ChxIdx));
	
	TM4x->TS = (1 << ChxIdx);
}


/**
  * @brief  Disable the specified TIMER channel count.
  * @param  tim: the specified tim, it can be TIM40 TIM41
  * @param  ChxIdx where ChxIdx can be 0, 1,2,3.
  *         @arg TIM_Index_Channel0
  *         @arg TIM_Index_Channel1
  *         @arg TIM_Index_Channel2
  *         @arg TIM_Index_Channel3
  * @retval None
  */
void TIM_Stop(TM40_Type* TM4x, uint8_t ChxIdx)
{
	assert_param(IS_TIM(TM4x));	
	assert_param(IS_TIM_CHANIDEX(ChxIdx));
	
	TM4x->TT = (1 << ChxIdx);
}

/**
  * @brief  Enables or disables the specified TIMER channel.
  * @param  TIMx: where x can be 0, 1, select TIEMR peripheral.
  * @param  ChxIdx can be 0, 1,2,3.
  * @param  NewState: new state of the TIEMR channel.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TIM_Cmd(TM40_Type* TM4x, uint8_t Chx, FunctionalState NewState)
{

	if(NewState == ENABLE)
		TIM_Start(TM4x,Chx);
	else
		TIM_Stop(TM4x,Chx);
}

/**
  * @brief  Set the specified TIMER channel's count num.
  * @param  TIMx: where x can be 0, 1, select TIEMR peripheral.
  * @param  ChxIdx can be 0, 1,2,3.
  * @param  counter: the count num of the TIEMR channel.          
  * @retval None
  */
void TIM_SetCounter(TM40_Type* TM4x, uint8_t ChxIdx, uint16_t counter)
{
	assert_param(IS_TIM(TM4x));	
	assert_param(IS_TIM_CHANIDEX(ChxIdx));
	
	if(ChxIdx == TIM_Index_Channel0 || ChxIdx == TIM_Index_Channel1)
	{
		TM4x->TDR[ChxIdx] = counter;
	}	
	if(ChxIdx == TIM_Index_Channel2 || ChxIdx == TIM_Index_Channel3)
	{
		TM4x->TDR1[ChxIdx - TIM_TDR_OFFSET] = counter;
	}		
}

/**
  * @brief  Get the specified TIMER channel's status.
  * @param  TIMx: where x can be 0, 1, select TIEMR peripheral.
  * @param  ChxIdx: where ChxIdx can be 0, 1,2,3.       
  * @retval bool: 1 it occurs overflow 
  *               0 it does not occur overflow
  */
bool TIM_GetStatus(TM40_Type* TM4x, uint8_t ChxIdx)
{	
	assert_param(IS_TIM(TM4x));	
	assert_param(IS_TIM_CHANIDEX(ChxIdx));
		
	
	return (bool)(TM4x->TSR[ChxIdx] & _0001_TM4_OVERFLOW_OCCURS);
}

/**
  * @brief  Initializes the TIEMR peripheral according to the specified
  *         parameters in the TIM_InitStruct .
  * @param  TIM_InitStruct: pointer to a TIM_InitTypeDef structure that contains
  *         the configuration information for the specified TIEMR peripheral.
  * @retval initial result
  */
int TIM_Init(TM40_Type* TM4x,TIM_InitTypeDef *TIM_InitStruct)
{
	int ret = TIM_SUCC;
	uint16_t clkselect = TM_CLOCK_SELECT_CKM0;
	uint8_t chanPos =0,chanNum=0,pos=0;
	uint8_t masterPos =0,masterNum=0,posm=0;
	
	assert_param(IS_TIM(TM4x));	
	assert_param(IS_TIM_CHANNEL(TIM_InitStruct->TIM_Channel));
	
	if(TM4x == TM40)
	{
		CGC_PER0PeriphClockCmd(CGC_PER0Periph_TIM40,ENABLE);
	}

	if(IS_TIM_CLK0(TIM_InitStruct->TIM_ClkDivision)){
		TM4x->TPS &= ~TIM_CLK0_MASK;
		TM4x->TPS |= TIM_InitStruct->TIM_ClkDivision;
		clkselect = TM_CLOCK_SELECT_CKM0;
	}else if(IS_TIM_CLK1(TIM_InitStruct->TIM_ClkDivision)){
		TM4x->TPS &= ~TIM_CLK1_MASK;
		TM4x->TPS |= TIM_InitStruct->TIM_ClkDivision;
		clkselect = TM_CLOCK_SELECT_CKM1;
	}else if(IS_TIM_CLK2(TIM_InitStruct->TIM_ClkDivision)){
		TM4x->TPS &= ~TIM_CLK2_MASK;
		TM4x->TPS |= TIM_InitStruct->TIM_ClkDivision;
		clkselect = TM_CLOCK_SELECT_CKM2;	
	}else if(IS_TIM_CLK3(TIM_InitStruct->TIM_ClkDivision)){
		TM4x->TPS &= ~TIM_CLK3_MASK;
		TM4x->TPS |= TIM_InitStruct->TIM_ClkDivision;
		clkselect = TM_CLOCK_SELECT_CKM3;		
	}else{
		return TIM_ERR;
	}
	
	if(IS_TIM_Master_Chan(TIM_InitStruct->TIM_Selection_Master))//multi-tim combination
	{
		for(masterPos = 0; masterPos < TIM_CHAN_MAX_NUM; masterPos++)
		{
			posm = ((uint8_t)0x01) << masterPos;
			masterNum = TIM_InitStruct->TIM_Selection_Master & posm;
			if(masterNum == posm)
			{	
				if(masterPos == TIM_Index_Channel0)  //�Ƿ���Ҫ�����ж�
				{
					TM4x->TMR[masterPos] = clkselect | TIM_InitStruct->TIM_Trigger | TIM_InitStruct->TIM_Pulse_Edge | TIM_Mode_PWM_Master | TIM_InitStruct->TIM_StartInt;								
					TM4x->TDR[masterPos] = TIM_InitStruct->TIM_Period[masterPos] -1;				 
				}
				else
				{
					TM4x->TMR[masterPos] = clkselect | 0x0800 |TIM_InitStruct->TIM_Trigger | TIM_InitStruct->TIM_Pulse_Edge | TIM_Mode_PWM_Master | TIM_InitStruct->TIM_StartInt;								
					TM4x->TDR1[masterPos - TIM_TDR_OFFSET] = TIM_InitStruct->TIM_Period[masterPos] -1;	/*���ڼĴ����ֽ��޷����룬BAT32A237.h�Ĵ����м䱣��reserved[36]��ʹ��TDR�Ĵ����޷�����ΪTDR[4]�����ȥƫ��*/			 				
				}
				TM4x->TOE &= ~(1 << masterPos);	
				TM4x->TO &= ~(1 << masterPos);
				MISC->NFEN1 |=  (1<<masterPos);	 //���			
			}			
		}
		for(chanPos = 0; chanPos < TIM_CHAN_MAX_NUM; chanPos++)   //����ͨ������
		{
			pos = ((uint8_t)0x01) << chanPos;
			chanNum = TIM_InitStruct->TIM_Channel & pos;
			if(chanNum == pos)
			{
			    TM4x->TMR[chanPos] = clkselect | TIM_Trigger_UseMaster_Int | TIM_Mode_SinglePulse |TIM_InitStruct->TIM_StartInt;
				if(chanPos == TIM_Index_Channel3 || chanPos == TIM_Index_Channel2)  /*���ڼĴ����ֽ��޷�����,�м䱣��reserved[36]��ʹ��TDR�Ĵ����޷�����ΪTDR[4]�����ȥƫ��*/
				{
					TM4x->TDR1[chanPos - TIM_TDR_OFFSET] = TIM_InitStruct->TIM_Period[chanPos];				
				}
				else
				{
					TM4x->TDR[chanPos] = TIM_InitStruct->TIM_Period[chanPos];
				}
				TM4x->TO &= ~(1 << chanPos);
				TM4x->TOE |= (1 << chanPos);
				TM4x->TOM |= (1 << chanPos);
			}
		}
		TM4x->TOL|= TIM_InitStruct->TIM_Slave_Polarity;
	}
	else
	{
		for(chanPos = 0; chanPos < TIM_CHAN_MAX_NUM; chanPos++)
		{
			pos = ((uint8_t)0x01) << chanPos;
			chanNum = TIM_InitStruct->TIM_Channel & pos;
			if(chanNum == pos)
			{				
				if(chanPos == TIM_Index_Channel3 || chanPos == TIM_Index_Channel2)  /*���ڼĴ����ֽ��޷�����,�м䱣��reserved[36]��ʹ��TDR�Ĵ����޷�����ΪTDR[4]�����ȥƫ��*/
				{
					TM4x->TDR1[chanPos - TIM_TDR_OFFSET] = TIM_InitStruct->TIM_Period[chanPos] -1;				
				}
				else
				{
					TM4x->TDR[chanPos] = TIM_InitStruct->TIM_Period[chanPos] -1;				
				}
				TM4x->TT  =  TIM_InitStruct->TIM_Channel;   //TIM stop

				TM4x->TMR[chanPos] = clkselect | TIM_InitStruct->TIM_Trigger | TIM_InitStruct->TIM_Pulse_Edge | TIM_InitStruct->TIM_StartInt;

				TM4x->TO &= ~ TIM_InitStruct->TIM_Channel;
				/*ģʽΪTIM_Mode_Interval/TIM_Mode_Square/TIM_Mode_DivFreq TMR�Ĵ���ģʽ����Ϊ0x0000*/
				if(TIM_InitStruct->TIM_Mode == TIM_Mode_Interval)    //��ʱ��ģʽ���ر�TO�������
				{
					TM4x->TOE &= ~TIM_InitStruct->TIM_Channel;
				}
				else if(TIM_InitStruct->TIM_Mode == TIM_Mode_Square)
				{
					TM4x->TOE |= TIM_InitStruct->TIM_Channel;				
				}
				else if(TIM_InitStruct->TIM_Mode == TIM_Mode_DivFreq) //��Ƶģʽһ����Ҫ��channel0 ���
				{
					assert_param(IS_TIM_CHANNEL0(TIM_InitStruct->TIM_Channel));
					if(IS_TIM_CHANNEL0(TIM_InitStruct->TIM_Channel))
					{
						TM4x->TOE |= TIM_InitStruct->TIM_Channel;
					}						
				}				
				else   //input event for tim 
				{
					TM4x->TOE &= ~ TIM_InitStruct->TIM_Channel;
					TM4x->TMR[chanPos] |= TIM_InitStruct->TIM_Mode;
					if(TIM_InitStruct->TIM_Mode == TIM_Mode_EventCount)
					{
						TM4x->TMR[chanPos] |= 0x1000;	//ѡ�������ŵ�ʱ��	
					}
					if(TM4x == TM40)
					{
						 MISC->NFEN1 |=  TIM_InitStruct->TIM_Channel;
					}
							
					if((TIM_InitStruct->TIM_Channel == TIM_Channel_0 || TIM_InitStruct->TIM_Channel == TIM_Channel_1) && (TM4x == TM40))
					{
						MISC->TIOS0 |=  TIM_InitStruct->TIM4_Input;
						if(TIM_InitStruct->TIM4_Input)
						{
							MISC->NFEN1 &=  ~(TIM_InitStruct->TIM_Channel);					
						}	
					}		
				}	
			}
		}
	}
	
	return ret;
}

/**
  * @brief  The Timer operates as pulse width measure.
  * @param  tim - choose timer unit it can choose TIM40 TIM41
  * @param  chxIdx - choose 0~3   
  * @retval period - return the pulse period at specified edge
  */
uint32_t TIM_GetPulsePeriod(TM40_Type* TM4x, uint8_t ChxIdx)
{
	assert_param(IS_TIM_CHANIDEX(ChxIdx));
	assert_param(IS_TIM(TM4x));
	
	uint32_t width = 0;
	
	if(INTC_GetPendingIRQ(TIM_IRQTable[ChxIdx]))
	{
		INTC_ClearPendingIRQ(TIM_IRQTable[ChxIdx]); /* clear INTTMA interrupt flag */
	}

	if (1U == (TM4x->TSR[ChxIdx] & _0001_TM4_OVERFLOW_OCCURS))
	{
		width= (uint32_t)(TM4x->TDR[ChxIdx] + 1UL) + 0x10000UL;
	}
	else
	{
		width= (uint32_t)(TM4x->TDR[ChxIdx] + 1UL);
	}

    return (width);
}

/**
  * @brief  The Timer operates as pulse width measure.
  * @param  tim - choose timer unit it can choose TIM40 TIM41
  * @param  chxIdx - choose 0~3   
  * @retval pulse width - return the width of pulse
  */
uint32_t TIM_GetPulseWidth(TM40_Type* TM4x, uint8_t ChxIdx)
{
	assert_param(IS_TIM_CHANIDEX(ChxIdx));
	assert_param(IS_TIM(TM4x));
	
	uint32_t width = 0;

	if(INTC_GetPendingIRQ((TIM_IRQTable[ChxIdx])))
	{
		INTC_ClearPendingIRQ(TIM_IRQTable[ChxIdx]); /* clear INTTMA interrupt flag */
	}
	else
	{
		return (width);
	}


	if (1U == (TM4x->TSR[ChxIdx] & _0001_TM4_OVERFLOW_OCCURS))
	{
		width= (uint32_t)(TM4x->TDR[ChxIdx] + 1UL) + 0x10000UL;
	}
	else
	{
		width= (uint32_t)(TM4x->TDR[ChxIdx] + 1UL);
	}


    return (width);
}
