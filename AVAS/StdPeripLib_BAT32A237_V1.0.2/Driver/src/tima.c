/*******************************************************************************
*                  COPYRIGHT (C) 2021 CMS Technologies Ltd.                    *
*                                                                              *
********************************************************************************
* FileName      : tima.c                                                  *
* Author        :                                                              *
* Version       : 1.0                                                          *
* Date          : 2021.08.13                                                   *
* Description   :                                                              *
* Function List :                                                              *
********************************************************************************/
#include "tima.h"
#include "cgc.h"

#define  g_ta0_value	 0xffff

/**
  * @brief  This function starts TMA counter.
  * @param  None
  * @retval None
  */
void TMA_Start(void)
{
    TMA->TACR0 |= TMA_COUNT_START;
}

/**
  * @brief  This function stops TMA counter.
  * @param  None
  * @retval None
  */
void TMA_Stop(void)
{
    TMA->TACR0 &= (uint8_t)~TMA_COUNT_START;
    INTC_DisableIRQ(TMA_IRQn);/* disable INTTMA interrupt */
    INTC_ClearPendingIRQ(TMA_IRQn);/* clear INTTMA interrupt flag */
}

/**
  * @brief  Clear the specified TMA flag is set.
  * @param  TMA_FLAG: specifies the flag to check.
  *          This parameter can be one of the following values:
  *            @arg TMA_FLAG_UNDERFLOW: Transmission running status flag
  *            @arg TMA_FLAG_ACTIVE_EDGE_RECEIVED: Transmission register save valid data flag
  *            @arg TMA_FLAG_STATUS_COUNT: Transmission frame data error flag
  * @retval None.
  */
void TMA_ClearFlag(uint8_t TMA_FLAG)
{
	/* Check the parameters */
	assert_param(IS_TMA_FLAG(TMA_FLAG));
	
	TMA->TACR0 &= ~TMA_FLAG;
}

/**
  * @brief  Checks whether the specified TMA flag is set or not.
  * @param  TMA_FLAG: specifies the flag to check.
  *          This parameter can be one of the following values:
  *            @arg TMA_FLAG_UNDERFLOW: Transmission running status flag
  *            @arg TMA_FLAG_ACTIVE_EDGE_RECEIVED: Transmission register save valid data flag
  *            @arg TMA_FLAG_STATUS_COUNT: Transmission frame data error flag
  * @retval The new state of USART_FLAG (SET or RESET).
  */
FlagStatus TMA_GetStaus(uint8_t TMA_FLAG)
{
	FlagStatus bitstatus = RESET;	
	/* Check the parameters */
	assert_param(IS_TMA_FLAG(TMA_FLAG));

	
	if ((TMA->TACR0 & TMA_FLAG) != (uint16_t)RESET)
	{
		bitstatus = SET;
	}
	else
	{
		bitstatus = RESET;
	}
	
	return bitstatus;
}


/**
  * @brief  Initializes the TIEMRA peripheral according to the specified
  *         parameters in the TIMA_InitStruct .
  * @param  TIMA_InitStruct: pointer to a TMA_InitTypeDef structure that contains
  *         the configuration information for the specified TIEMRA peripheral.
  * @retval none
  */
void TMA_Init(TMA_InitTypeDef *TIMA_InitStruct)
{
	CGC_PER1PeriphClockCmd(CGC_PER1Periph_TMA,ENABLE);
	
	TMA->TACR0 &= (uint8_t)~0x01;	//TMA STOP
    INTC_DisableIRQ(TMA_IRQn);           /* disable INTTMA interrupt */
    INTC_ClearPendingIRQ(TMA_IRQn);      /* clear INTTMA interrupt flag */
	
	TMA->TAMR0 = TIMA_InitStruct->TMA_Clk | TIMA_InitStruct->TMA_Mode;

	if(TIMA_InitStruct->TMA_Mode == TMA_Mode_Square)
	{
		TMA->TAIOC0 = TIMA_InitStruct->TMA_Polarity | 0x04; //0x00:ban TAO OUTPUT
	}
	else if(TIMA_InitStruct->TMA_Mode == TMA_Mode_PluseWidth || TIMA_InitStruct->TMA_Mode == TMA_Mode_PlusePeriod)
	{
		if(TIMA_InitStruct->TMA_Pulse ==TMA_Pulse_Width_Low || TIMA_InitStruct->TMA_Pulse ==TMA_Pulse_Rising)
			TMA->TAIOC0 = TMA_Polarity_0;
		if(TIMA_InitStruct->TMA_Pulse ==TMA_Pulse_Width_High ||TIMA_InitStruct->TMA_Pulse ==TMA_Pulse_Falling )
			TMA->TAIOC0 = TMA_Polarity_1;
	}
	else if(TIMA_InitStruct->TMA_Mode == TMA_Mode_EventCount)
	{
		assert_param(IS_TMA_EDGE(TIMA_InitStruct->TMA_EventPara.TIMA_Edge));
		assert_param(IS_TMA_CTRL(TIMA_InitStruct->TMA_EventPara.TMA_Ctrl));

		if(TIMA_InitStruct->TMA_EventPara.TMA_Ctrl & 0x000f)
		{
			TMA->TAIOC0 = TMA_EVENT_ENABLE_INTP4;		
		}
		else if(TIMA_InitStruct->TMA_EventPara.TMA_Ctrl & 0x0ff0)
		{
			TMA->TAIOC0 = TMA_EVENT_ENABLE_TIMEROUTSIGNAL;			
		}
		else
		{
			TMA->TAIOC0 = TMA_EVENT_ENABLE_ALWAYS;			
		}
		
		if(TIMA_InitStruct->TMA_EventPara.TMA_Ctrl == TMA_INTP4_LOW_LEVEL)
		{
			TMA->TAISR0 = TMA_PWM_POLARITY_L;
		}
		if(TIMA_InitStruct->TMA_EventPara.TMA_Ctrl == TMM_TMIOD1_LOW_LEVEL)
		{
			TMA->TAISR0 = TMA_PWM_POLARITY_L | TMA_PWM_TMIOD1;
		}
		if(TIMA_InitStruct->TMA_EventPara.TMA_Ctrl == TMM_TMIOC1_LOW_LEVEL)
		{
			TMA->TAISR0 = TMA_PWM_POLARITY_L | TMA_PWM_TMIOC1;
		}
		if(TIMA_InitStruct->TMA_EventPara.TMA_Ctrl == TIM_TO02_LOW_LEVEL)
		{
			TMA->TAISR0 = TMA_PWM_POLARITY_L | TMA_PWM_TO02;
		}			
		if(TIMA_InitStruct->TMA_EventPara.TMA_Ctrl == TIM_TO03_LOW_LEVEL)
		{
			TMA->TAISR0 = TMA_PWM_POLARITY_L | TMA_PWM_TO03;
		}
		
		if(TIMA_InitStruct->TMA_EventPara.TMA_Ctrl == TMA_INTP4_HIGH_LEVEL)
		{
			TMA->TAISR0 = TMA_PWM_POLARITY_H;		
		}		
		if(TIMA_InitStruct->TMA_EventPara.TMA_Ctrl == TMM_TMIOD1_HIGH_LEVEL)
		{
			TMA->TAISR0 = TMA_PWM_POLARITY_H | TMA_PWM_TMIOD1;
		}
		if(TIMA_InitStruct->TMA_EventPara.TMA_Ctrl == TMM_TMIOC1_HIGH_LEVEL)
		{
			TMA->TAISR0 = TMA_PWM_POLARITY_H | TMA_PWM_TMIOC1;
		}
		if(TIMA_InitStruct->TMA_EventPara.TMA_Ctrl == TIM_TO02_HIGH_LEVEL)
		{
			TMA->TAISR0 = TMA_PWM_POLARITY_H | TMA_PWM_TO02;
		}			
		if(TIMA_InitStruct->TMA_EventPara.TMA_Ctrl == TIM_TO03_HIGH_LEVEL)
		{
			TMA->TAISR0 = TMA_PWM_POLARITY_H | TMA_PWM_TO03;
		}
		
		if(TIMA_InitStruct->TMA_EventPara.TIMA_Edge == TMA_RISING_EDGE)
		{
			TMA->TAMR0 |= TMA_TAIO_POLARITY_ONE;
			TMA->TAIOC0 |= 	TMA_Polarity_0;
		}
		else if(TIMA_InitStruct->TMA_EventPara.TIMA_Edge == TMA_FALLING_EDGE)
		{
			TMA->TAMR0 |= TMA_TAIO_POLARITY_ONE;
			TMA->TAIOC0 |= 	TMA_Polarity_1;
		}
		else
		{
			TMA->TAMR0 |= TMA_TAIO_POLARITY_BOTH;
		}
	}
	if(TIMA_InitStruct->TMA_Clk == TMA_FIL)
	{
        CGC->OSMC = 0x10U;  /* WUTMMCK0 = 1 */	
	}
    TMA->TA0 = TIMA_InitStruct->TMA_Period - 1;	
}


uint32_t TMA_GetPulseWidth(void)
{
	uint32_t width, period;
	static uint32_t tempwidth = g_ta0_value, tma_underflow_count=0;

	while(INTC_GetPendingIRQ(TMA_IRQn) == 0);
	INTC_ClearPendingIRQ(TMA_IRQn); /* clear INTTMA interrupt flag */

	if (TMA_GetStaus(TMA_FLAG_UNDERFLOW))
	{
		TMA->TACR0 &= (uint8_t)~TMA_FLAG_UNDERFLOW;
		tma_underflow_count += 1U;
	}		
	if(TMA_GetStaus(TMA_FLAG_ACTIVE_EDGE_RECEIVED)) //active edge ocuurs
	{
		TMA->TACR0 &= ~TMA_FLAG_ACTIVE_EDGE_RECEIVED;
		if (tma_underflow_count == 0U)
		{
			width = tempwidth - TMA->TA0;
			tempwidth = TMA->TA0;
		}
		else
		{
			width = tempwidth + (0xffff + 1) * tma_underflow_count - TMA->TA0;
			tempwidth = TMA->TA0;
			tma_underflow_count = 0;
		}	
	}
	return width;
}


uint32_t TMA_GetPulsePeriod(void)   
{
	uint32_t width=0, tmp;
	static uint32_t tempwidth = g_ta0_value, tma_underflow_count=0;
	static uint8_t time =0;
	
	while(INTC_GetPendingIRQ(TMA_IRQn) == 0);
    INTC_ClearPendingIRQ(TMA_IRQn); /* clear INTTMA interrupt flag */
	
	if (TMA_GetStaus(TMA_FLAG_UNDERFLOW))
    {
		TMA_ClearFlag(TMA_FLAG_UNDERFLOW);
        tma_underflow_count += 1U;
    }	
	if(TMA_GetStaus(TMA_FLAG_ACTIVE_EDGE_RECEIVED)) //active edge ocuurs
	{
		time++;
		TMA_ClearFlag(TMA_FLAG_ACTIVE_EDGE_RECEIVED);

		if(time == 1)
		{
			tmp = TMA->TA0;    // dummy read to update readbuf	
		}
		else
		{
			if(tma_underflow_count == 0)
			{
				width = tempwidth - TMA->TA0;
			}
			else
			{
				 width = tempwidth - TMA->TA0 + 1U + (tempwidth + 1) * tma_underflow_count;
			}

			time = 0;			
		}
	}

	return width;
}
