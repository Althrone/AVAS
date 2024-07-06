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
#include "timb.h"
#include "cgc.h"



/**
  * @brief  This function stops the clock supplied for TMB.
  * @param  None
  * @retval None
  */
void TMB0_Set_PowerOff(void)
{
	CGC_PER1PeriphClockCmd(CGC_PER1Periph_TMB,DISABLE);
}

/**
  * @brief  This function starts TMA counter.
  * @param  None
  * @retval None
  */
void TMB_Start(void)
{
    volatile uint8_t tbsr_dummy;

    tbsr_dummy = TMB->TBSR; /* read TBSR before write 0 */
    TMB->TBSR = 0x00U;
	
    TMB->TBMR |= TMB_COUNT_START;	
}

/**
  * @brief  This function stops TMA counter.
  * @param  None
  * @retval None
  */
void TMB_Stop(void)
{
    volatile uint8_t tbsr_dummy;

    TMB->TBMR &= (uint8_t)~TMB_COUNT_START;

    tbsr_dummy = TMB->TBSR; /* read TBSR before write 0 */
    TMB->TBSR = 0x00U;
}

/**
  * @brief  Checks whether the specified TMB flag is set or not.
  * @param  TMB_FLAG: specifies the flag to check.
  *          This parameter can be one of the following values:
  *            @arg TMB_FLAG_TB_OVERFLOW: TB counter is overflow
  *            @arg TMB_FLAG_TB_UNDERFLOW: TB counter is underflow
  *            @arg TMB_FLAG_CHB_MATCHED: TB is matched with TBGRB
  *            @arg TMB_FLAG_CHA_MATCHED: TB is matched with TBGRA
  * @retval The new state of USART_FLAG (SET or RESET).
  */
FlagStatus TMB_GetStaus(uint8_t TMB_FLAG)
{
	FlagStatus bitstatus = RESET;	
	/* Check the parameters */
	assert_param(IS_TMB_FLAG(TMB_FLAG));

	
	if ((TMB->TBSR & TMB_FLAG) != (uint8_t)RESET)
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
  * @brief  Initializes the TIEMRB peripheral according to the specified
  *         parameters in the TIMB_InitStruct .
  * @param  TIMB_InitStruct: pointer to a TMB_InitTypeDef structure that contains
  *         the configuration information for the specified TIEMRB peripheral.
  * @retval none
  */
void TMB_Init(TMB_InitTypeDef *TIMB_InitStruct)
{
	assert_param(IS_TMB_CLOCK(TIMB_InitStruct->TMB_Clk));	
	assert_param(IS_TMB_CHANNEL(TIMB_InitStruct->TMB_Channel));	

	CGC_PER1PeriphClockCmd(CGC_PER1Periph_TMB,ENABLE);
	
	TMB->TBMR &= (uint8_t)~TMB_COUNT_START;	//TMB STOP
	TMB->TBMR = 0x00;

    TMB->TBCR = TIMB_InitStruct->TMB_CounterClear | TIMB_InitStruct->TMB_Clk;  

	if(TIMB_InitStruct->TMB_Mode == TMB_Mode_Capture)
	{		
		if((TIMB_InitStruct->TMB_Channel & TMB_Channel_A) == TMB_Channel_A)
		{
			TMB->TBIOR = 0x08 | 0x04 | TIMB_InitStruct->TMB_IC.TMB_CHA.TMB_ICPolarity;
			if(TIMB_InitStruct->TMB_IC.TMB_CHA.TMB_ICFilter == 1) //使用滤波器功能
			{		
				TMB->TBMR |= 0x04;
				TMB->TBMR |= (TIMB_InitStruct->TMB_IC.TMB_CHA.TMB_Filter_Clk << 4);
			}
		}
		if((TIMB_InitStruct->TMB_Channel & TMB_Channel_B) == TMB_Channel_B)
		{
			TMB->TBIOR = (0x08 << 4) | 0x40 |( TIMB_InitStruct->TMB_IC.TMB_CHB.TMB_ICPolarity << 4);	
			if(TIMB_InitStruct->TMB_IC.TMB_CHB.TMB_ICFilter == 1) //使用滤波器功能
			{		
				TMB->TBMR |= 0x08;
				TMB->TBMR |= (TIMB_InitStruct->TMB_IC.TMB_CHB.TMB_Filter_Clk << 4);				
			}
			
		}	
		if((TIMB_InitStruct->TMB_Channel & TMB_Channel_ELC) == TMB_Channel_ELC)
		{
			TMB->TBMR |= 0x40;		
		}

	}
	
	if(TIMB_InitStruct->TMB_Mode == TMB_Mode_Compare)
	{
		if((TIMB_InitStruct->TMB_Channel & TMB_Channel_A) == TMB_Channel_A)
		{
			TMB->TBIOR |= 0x08 | 0x00 | TIMB_InitStruct->TMB_OC.TMB_CHA.TMB_OCPolarity;
			TMB->TBGRA = TIMB_InitStruct->TMB_CHA_Pulse -1;
		}
		if((TIMB_InitStruct->TMB_Channel & TMB_Channel_B)== TMB_Channel_B)
		{
			TMB->TBIOR |= (0x08 << 4) | 0x00 |( TIMB_InitStruct->TMB_OC.TMB_CHB.TMB_OCPolarity << 4);
			TMB->TBGRB = TIMB_InitStruct->TMB_CHB_Pulse -1;				
		}
		TMB->TBGRC = TMB->TBGRA;
		TMB->TBGRD = TMB->TBGRB;	
	}
	if(TIMB_InitStruct->TMB_Mode == TMB_Mode_PWM)
	{
		TMB->TBMR |= 0x01;
		/* Please note that TBBUFB and TBBUFA bit of TBIOR register are still valid in PWM mode. */
		/* TBGRD/TBGRC is used as buffer register for TBGRB/TBGRA register */
		TMB->TBIOR = (0x08 << 4) | 0x08;
		TMB->TBGRA = TIMB_InitStruct->TMB_CHA_Pulse -1;
		TMB->TBGRB = TIMB_InitStruct->TMB_CHB_Pulse -1;				
		TMB->TBGRC = TMB->TBGRA;
		TMB->TBGRD = TMB->TBGRB;			
	}
	TMB->TBIER = 0x08 | 0x02 | 0x01;  //开启输入/比较允许中断

}


/**
  * @brief  Set the specified TMB PWM duty and period .
  * @param  period - the period of TBIO0 output wave
  * @param  duty - the duty of TBIO0 output wave      
  * @retval None
  */

void TMB_SetCounter(uint16_t period, uint16_t duty)
{
	TMB->TBGRA = period  -1;
	TMB->TBGRB = duty -1;				
	TMB->TBGRC = TMB->TBGRA;
	TMB->TBGRD = TMB->TBGRB;			
}

volatile uint16_t tmb_overflow_count_a = 0U;
volatile uint16_t tmb_overflow_count_b = 0U;

uint32_t TMB_GetPulseWidth(uint8_t channel)
{
	int32_t width=0;
	
    uint8_t tbsr_temp = TMB->TBSR;
    uint8_t tbier_temp = TMB->TBIER;
	
	if (INTC_GetPendingIRQ(TMB_IRQn) == 1)
	{
		INTC_ClearPendingIRQ(TMB_IRQn); /* clear INTTMA interrupt flag */
	}
	else
	{
		return 0;	
	}

    if (TMB_GetStaus(TMB_FLAG_TB_OVERFLOW) == SET) // count overflow
    {
        TMB->TBSR = tbsr_temp & (uint8_t)~TMB_FLAG_TB_OVERFLOW;
//        tmb_overflow_count_a += 1UL;
//        tmb_overflow_count_b += 1UL;
    }

	if(channel == TMB_Channel_A)
	{
		if (TMB_GetStaus(TMB_FLAG_CHA_MATCHED) == SET)
		{
			TMB->TBSR = tbsr_temp & ~TMB_FLAG_CHA_MATCHED;

			if (tmb_overflow_count_a == 0UL)
			{
				width = (TMB->TBGRA - TMB->TBGRC);
				tmb_overflow_count_a = 0;
				if(width < 0)
				{
					width += 0xffff;
				}
			}
			else
			{
				width = (TMB->TBGRA - TMB->TBGRC) + 0xffff;
				tmb_overflow_count_a = 0UL;
			}
		}
	}
	if(channel == TMB_Channel_B)
	{
		if (TMB_GetStaus(TMB_FLAG_CHB_MATCHED) == SET)
		{
			TMB->TBSR = tbsr_temp & ~TMB_FLAG_CHB_MATCHED;

			if (tmb_overflow_count_b == 0UL)
			{
				width = (TMB->TBGRB - TMB->TBGRD);
			}
			else
			{
				width = (TMB->TBGRB - TMB->TBGRD) + (0x10000UL * tmb_overflow_count_b);
				tmb_overflow_count_b = 0UL;
			}
		}
	}

    TMB->TBIER = tbier_temp;

	return width;
}
