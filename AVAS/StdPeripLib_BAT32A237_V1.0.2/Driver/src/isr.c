#include "isr.h"
#define MK_IRQ_OFF	0xFF
void IntDefaultHandler(void);
volatile uint32_t g_ticks = 0xFFFFFFFFU;

device_isr_handler_t isrObj[TOTAL_IRQ_NUM]={
{0,IntDefaultHandler},/*  LVI_IRQn         !< 0  Low Voltage detection interrupt                                        */
{0,IntDefaultHandler},/*  INTP0_IRQn       !< 1  INTP0 External interrupt request input is valid                        */
{0,IntDefaultHandler},/*  INTP1_IRQn       !< 2  INTP1 External interrupt request input is valid                        */
{0,IntDefaultHandler},/*  INTP2_IRQn       !< 3  INTP2 External interrupt request input is valid                        */
{0,IntDefaultHandler}, /*  INTP3_IRQn       !< 4  INTP3 External interrupt request input is valid                       */
{0,IntDefaultHandler},/*  INTP4_IRQn       !< 5  INTP4 External interrupt request input is valid                        */
{0,IntDefaultHandler},/*  INTP5_IRQn       !< 6  INTP5 External interrupt request input is valid                        */
{0,IntDefaultHandler},/*  ST2_IRQn /SPI20_IRQn/IIC20_IRQn !< 7  UART2 transmission transfer end or buffer empty       */
{0,IntDefaultHandler},/*  SR2_IRQn /SPI21_IRQn/IIC21_IRQn !< 8  UART2 rerception transfer                               */
{0,IntDefaultHandler},/*  SRE2_IRQn        !< 9  UART2 rerception communication error occurrence                        */
{0,IntDefaultHandler},/*  ST0_IRQn /SPI00_IRQn/IIC00_IRQn !< 10 UART0 transmission transfer end or buffer empty         */
{0,IntDefaultHandler},/*  SR0_IRQn  /SPI01_IRQn/IIC01_IRQn !< 11 UART0 rerception transfer                              */
{0,IntDefaultHandler},/*  SRE0_IRQn        !< 12 UART0 rerception communication error occurrence                        */
{0,IntDefaultHandler},/*  ST1_IRQn /SPI10_IRQn/IIC10_IRQn  !< 13 UART1 transmission transfer end or buffer empty        */
{0,IntDefaultHandler},/*  SR1_IRQn /SPI11_IRQn/IIC11_IRQn  !< 14 UART1 rerception transfer                              */
{0,IntDefaultHandler},/*  SRE1_IRQn        !< 15 UART1 rerception communication error occurrence                        */
{0,IntDefaultHandler},/*  IICA_IRQn        !< 16 IICA interrupt request                                                 */
{0,IntDefaultHandler},/*  TM00_IRQn        !< 17 TM4 channel 0 interrupt request                                        */
{0,IntDefaultHandler},/*  TM01_IRQn        !< 18 TM4 channel 1 interrupt request                                        */
{0,IntDefaultHandler},/*  TM02_IRQn        !< 19 TM4 channel 2 interrupt request                                        */
{0,IntDefaultHandler},/*  TM03_IRQn        !< 20 TM4 channel 3 interrupt request                                        */
{0,IntDefaultHandler},/*  ADC_IRQn         !< 21 ADC interrupt request                                                  */
{0,IntDefaultHandler},/*  RTC_IRQn         !< 22 Real-Time Clock interrupt request                                      */
{0,IntDefaultHandler},/*  KEY_IRQn         !< 23 KEY return interrupt request                                           */
{0,IntDefaultHandler},/*  CMP0_IRQn        !< 24 CMP0 interrupt request                                                 */
{0,IntDefaultHandler},/*  CMP1_IRQn        !< 25 CMP1 interrupt request                                                 */
{0,IntDefaultHandler},/*  TMA_IRQn         !< 26 TMA interrupt request                                                  */
{0,IntDefaultHandler},/*  TMM0_IRQn        !< 27 TMM channel 0 interrupt request                                        */
{0,IntDefaultHandler},/*  TMM1_IRQn        !< 28 TMM channel 1 interrupt request                                        */
{0,IntDefaultHandler},/*  TMB_IRQn         !< 29 TMB interrupt request                                                  */
{0,IntDefaultHandler},/*  TMC_IRQn         !< 30 TMC interrupt request                                                  */
{0,IntDefaultHandler},/*  FMC_IRQn         !< 31 Flash erase or write finish                                            */
{0,0},
{0,IntDefaultHandler},/*  INTP6_IRQn       !< 33 INTP6 External interrupt request input is valid                        */
{0,IntDefaultHandler},/*  INTP7_IRQn       !< 34 INTP7 External interrupt request input is valid                        */
{0,IntDefaultHandler},/*  INTP8_IRQn       !< 35 INTP8 External interrupt request input is valid                        */
{0,IntDefaultHandler},/*  INTP9_IRQn       !< 36 INTP9 External interrupt request input is valid                        */
{0,IntDefaultHandler},/*  INTP10_IRQn      !< 37 INTP10 External interrupt request input is valid                       */
{0,IntDefaultHandler},/*  INTP11_IRQn      !< 38 INTP11 External interrupt request input is valid                       */
{0,0},
{0,0},
{0,IntDefaultHandler},/*  CAN0ERR_IRQn     !< 41 CAN0 error interrupt                                                   */
{0,0},
{0,0},
{0,IntDefaultHandler},/*  TM01H_IRQn       !< 44 TM4 channel 0 (8bit) interrupt request                                 */
{0,0},
{0,0},
{0,IntDefaultHandler},/*  TM03H_IRQn       !< 47 TM4 channel 3 (8bit) interrupt request                                 */
{0,IntDefaultHandler},/*  DIV_IRQn         !< 48 Hardware divider interrupt                                             */
{0,0},
{0,0},
{0,0},
{0,0},
{0,0},
{0,IntDefaultHandler},/*  IT_IRQn          !< 54 15-bit interval timer interrupt request                                */
{0,IntDefaultHandler},/*  CAN0REC_IRQn     !< 55 CAN0 reception completion interrupt                                    */
{0,IntDefaultHandler},/*  CAN0WUP_IRQn     !< 56 CAN0 wakeup interrupt                                                  */
{0,IntDefaultHandler},/*  CAN0TRX_IRQn     !< 57 CAN0 transmission completion interrupt                                 */
};

void IntDefaultHandler(void)
{ 
	while(1) {}
}
/**
  * @brief  SysTick Handler Decreament the g_ticks value
  * @param  TM13 IRQ
  * @retval None
  */

void SysTick_Handler(void)
{
	g_ticks--;
}



/**
  * @brief  IRQ00_Handler
  * @param  LVI IRQ
  * @retval None
  */
void IRQ00_Handler(void)
{
	(void)isrObj[LVI_IRQn].isrHandler();
}

/**
  * @brief  IRQ01_Handler
  * @param  INTP0_IRQn /INTP6_IRQn
  * @retval None
  */
void IRQ01_Handler(void)
{
	if(INTC_GetPendingIRQ(INTP0_IRQn) && (INTC_GetMKIRQ(INTP0_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[INTP0_IRQn].isrHandler();
	}

	if(INTC_GetPendingIRQ(INTP6_IRQn) && (INTC_GetMKIRQ(INTP6_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[INTP6_IRQn].isrHandler();
	}
}

/**
  * @brief  IRQ02_Handler
  * @param  INTP1 IRQ /INTP7_IRQn
  * @retval None
  */
void IRQ02_Handler(void)
{
	if(INTC_GetPendingIRQ(INTP1_IRQn) && (INTC_GetMKIRQ(INTP1_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[INTP1_IRQn].isrHandler();
	}

	if(INTC_GetPendingIRQ(INTP7_IRQn) && (INTC_GetMKIRQ(INTP7_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[INTP7_IRQn].isrHandler();
	}
}

/**
  * @brief  IRQ03_Handler
  * @param  INTP2 IRQ /INTP8_IRQn
  * @retval None
  */
void IRQ03_Handler(void)
{
	if(INTC_GetPendingIRQ(INTP2_IRQn) && (INTC_GetMKIRQ(INTP2_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[INTP2_IRQn].isrHandler();
	}
	
	if(INTC_GetPendingIRQ(INTP8_IRQn) && (INTC_GetMKIRQ(INTP8_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[INTP8_IRQn].isrHandler();
	}	
}

/**
  * @brief  IRQ04_Handler
  * @param  INTP3 IRQ /INTP9_IRQn
  * @retval None
  */
void IRQ04_Handler(void)
{
	if(INTC_GetPendingIRQ(INTP3_IRQn) && (INTC_GetMKIRQ(INTP3_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[INTP3_IRQn].isrHandler();
	}
	
	if(INTC_GetPendingIRQ(INTP9_IRQn) && (INTC_GetMKIRQ(INTP9_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[INTP9_IRQn].isrHandler();
	}	
}

/**
  * @brief  IRQ05_Handler
  * @param  INTP4_IRQn /INTP10_IRQn
  * @retval None
  */
void IRQ05_Handler(void)
{
	if(INTC_GetPendingIRQ(INTP4_IRQn) && (INTC_GetMKIRQ(INTP4_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[INTP4_IRQn].isrHandler();
	}
	
	if(INTC_GetPendingIRQ(INTP10_IRQn) && (INTC_GetMKIRQ(INTP10_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[INTP10_IRQn].isrHandler();
	}	

}

/**
  * @brief  IRQ06_Handler
  * @param  INTP5_IRQn /INTP11_IRQn
  * @retval None
  */
void IRQ06_Handler(void)
{
	if(INTC_GetPendingIRQ(INTP5_IRQn) && (INTC_GetMKIRQ(INTP5_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[INTP5_IRQn].isrHandler();
	}
	
	if(INTC_GetPendingIRQ(INTP11_IRQn) && (INTC_GetMKIRQ(INTP11_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[INTP11_IRQn].isrHandler();
	}
}

/**
  * @brief  IRQ07_Handler
  * @param  ST2/SPI20/IIC20 IRQ 
  * @retval None
  */
void IRQ07_Handler(void)
{
	(void)isrObj[7].isrHandler();
}

/**
  * @brief  IRQ08_Handler
  * @param  SR2/SPI21/IIC21 IRQ
  * @retval None
  */
void IRQ08_Handler(void)
{	
	(void)isrObj[8].isrHandler();
}

/**
  * @brief  IRQ09_Handler
  * @param  SRE2 IRQ /CAN0ERR_IRQn
  * @retval None
  */
void IRQ09_Handler(void)
{
	if(INTC_GetPendingIRQ(SRE2_IRQn) && (INTC_GetMKIRQ(SRE2_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[SRE2_IRQn].isrHandler();
	}
	
	if(INTC_GetPendingIRQ(CAN0ERR_IRQn) && (INTC_GetMKIRQ(CAN0ERR_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[CAN0ERR_IRQn].isrHandler();
	}
}

/**
  * @brief  IRQ10_Handler
  * @param  ST0/SPI00/IIC00 IRQ  
  * @note   the num of ST0_IRQn is same as SPI00_IRQn and IIC00_IRQ, so SPI00_IRQn or IIC00_IRQ
  *			interrupt quote this function handler IRQ10_Handler
  * @retval None
  */
void IRQ10_Handler(void)
{
	(void)isrObj[10].isrHandler();
}

/**
  * @brief  IRQ11_Handler
  * @param  SR0/SPI01/IIC01 IRQ 
  * @note   the num of SR0_IRQn is same as SPI01_IRQn and IIC01_IRQ, so SPI01_IRQn or IIC01_IRQ
  *			interrupt quote this function handler IRQ11_Handler
  * @retval None
  */
void IRQ11_Handler(void)
{
	(void)isrObj[11].isrHandler();
}

/**
  * @brief  IRQ12_Handler
  * @param   SRE0 or TM01H IRQ 
  * @note   the num of SRE0_IRQn is same as TM01H_IRQn, so TM01H_IRQn 
  *			interrupt quotes this function handler IRQ12_Handler
  * @retval None
  */
void IRQ12_Handler(void)
{
	if(INTC_GetPendingIRQ(SRE0_IRQn) && (INTC_GetMKIRQ(SRE0_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[SRE0_IRQn].isrHandler();
	}
	
	if(INTC_GetPendingIRQ(TM01H_IRQn) && (INTC_GetMKIRQ(TM01H_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[TM01H_IRQn].isrHandler();
	}	
}

/**
  * @brief  IRQ13_Handler
  * @param  ST1/SPI10/IIC10 IRQ  
  * @note   the num of ST1_IRQn is same as SPI10_IRQn and IIC10_IRQ, so SPI10_IRQn or IIC10_IRQ
  *			interrupt quote this function handler IRQ13_Handler
  * @retval None
  */
void IRQ13_Handler(void)
{
	(void)isrObj[13].isrHandler();
}

/**
  * @brief  IRQ14_Handler
  * @param  SR1/SPI11/IIC11 IRQ
  * @note   the num of SR1_IRQn is same as SPI11_IRQn and IIC11_IRQ, so SPI11_IRQn or IIC11_IRQ
  *			interrupt quote this function handler IRQ14_Handler
  * @retval None
  */
void IRQ14_Handler(void)
{
	(void)isrObj[14].isrHandler();
}

/**
  * @brief  IRQ15_Handler
  * @param  SRE1 IRQ /TM03H_IRQn
  * @note   the num of SRE1_IRQn is same as TM03H_IRQn, so TM03H_IRQn 
  *			interrupt quotes this function handler IRQ15_Handler
  * @retval None
  */
void IRQ15_Handler(void)
{
	if(INTC_GetPendingIRQ(SRE1_IRQn) && (INTC_GetMKIRQ(SRE1_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[SRE1_IRQn].isrHandler();
	}
	
	if(INTC_GetPendingIRQ(TM03H_IRQn) && (INTC_GetMKIRQ(TM03H_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[TM03H_IRQn].isrHandler();
	}	
}

/**
  * @brief  IRQ16_Handler
  * @param  IICA IRQ /DIV_IRQn
  * @note   the num of IICA_IRQn is same as DIV_IRQn, so DIV_IRQn 
  *			interrupt quotes this function handler IRQ16_Handler
  * @retval None
  */
void IRQ16_Handler(void)
{
	if(INTC_GetPendingIRQ(IICA_IRQn) && (INTC_GetMKIRQ(IICA_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[IICA_IRQn].isrHandler();
	}
	
	if(INTC_GetPendingIRQ(DIV_IRQn) && (INTC_GetMKIRQ(DIV_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[DIV_IRQn].isrHandler();
	}
}

/**
  * @brief  IRQ17_Handler
  * @param  TM00 IRQ
  * @retval None
  */
void IRQ17_Handler(void)
{
	(void)isrObj[TM00_IRQn].isrHandler();
}

/**
  * @brief  IRQ18_Handler
  * @param  TM01 IRQ
  * @retval None
  */
void IRQ18_Handler(void)
{
	(void)isrObj[TM01_IRQn].isrHandler();
}

/**
  * @brief  IRQ19_Handler
  * @param  TM02 IRQ
  * @retval None
  */
void IRQ19_Handler(void)
{
	(void)isrObj[TM02_IRQn].isrHandler();
}

/**
  * @brief  IRQ20_Handler
  * @param  TM03 IRQ
  * @retval None
  */
void IRQ20_Handler(void)
{
	(void)isrObj[TM03_IRQn].isrHandler();
}

/**
  * @brief  IRQ21_Handler
  * @param  ADC IRQ
  * @retval None
  */
void IRQ21_Handler(void)
{
	(void)isrObj[ADC_IRQn].isrHandler();
}

/**
  * @brief  IRQ22_Handler
  * @param  RTC IRQ /IT_IRQn
  * @retval None
  */
void IRQ22_Handler(void)
{
	if(INTC_GetPendingIRQ(RTC_IRQn) && (INTC_GetMKIRQ(RTC_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[RTC_IRQn].isrHandler();
	}
	
	if(INTC_GetPendingIRQ(IT_IRQn) && (INTC_GetMKIRQ(IT_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[IT_IRQn].isrHandler();
	}	
}

/**
  * @brief  IRQ23_Handler
  * @param  KEY_IRQn /CAN0REC_IRQn
  * @retval None
  */
void IRQ23_Handler(void)
{
	if(INTC_GetPendingIRQ(KEY_IRQn) && (INTC_GetMKIRQ(KEY_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[KEY_IRQn].isrHandler();
	}
	
	if(INTC_GetPendingIRQ(CAN0REC_IRQn) && (INTC_GetMKIRQ(CAN0REC_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[CAN0REC_IRQn].isrHandler();
	}	
}

/**
  * @brief  IRQ24_Handler
  * @param  CMP0_IRQn /CAN0WUP_IRQn
  * @retval None
  */
void IRQ24_Handler(void)
{
	if(INTC_GetPendingIRQ(CMP0_IRQn) && (INTC_GetMKIRQ(CMP0_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[CMP0_IRQn].isrHandler();
	}
	
	if(INTC_GetPendingIRQ(CAN0WUP_IRQn) && (INTC_GetMKIRQ(CAN0WUP_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[CAN0WUP_IRQn].isrHandler();
	}
}

void IRQ25_Handler(void)
{
	if(INTC_GetPendingIRQ(CMP1_IRQn) && (INTC_GetMKIRQ(CMP1_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[CMP1_IRQn].isrHandler();
	}
	
	if(INTC_GetPendingIRQ(CAN0TRX_IRQn) && (INTC_GetMKIRQ(CAN0TRX_IRQn) != MK_IRQ_OFF))
	{
		(void)isrObj[CAN0TRX_IRQn].isrHandler();
	}
}

void IRQ26_Handler(void)
{
	(void)isrObj[TMA_IRQn].isrHandler();
}
/**
  * @brief  IRQ27_Handler
  * @param  TMM0 IRQ
  * @retval None
  */
void IRQ27_Handler(void)
{
	(void)isrObj[TMM0_IRQn].isrHandler();
}

/**
  * @brief  IRQ28_Handler
  * @param  TM13 IRQ
  * @retval None
  */
void IRQ28_Handler(void)
{
	(void)isrObj[TMM1_IRQn].isrHandler();
}

/**
  * @brief  IRQ29_Handler
  * @param  TM12 IRQ
  * @retval None
  */
void IRQ29_Handler(void)
{
	(void)isrObj[TMB_IRQn].isrHandler();
}

/**
  * @brief  IRQ30_Handler
  * @param  TM13 IRQ
  * @retval None
  */
void IRQ30_Handler(void)
{
	(void)isrObj[TMC_IRQn].isrHandler();
}

/**
  * @brief  IRQ31_Handler
  * @param  FMC IRQ 
  * @retval None
  */
void IRQ31_Handler(void)
{
	(void)isrObj[FMC_IRQn].isrHandler();
}

/**
  * @brief  register interrupt function according to  IRQn num.
  * @param  irq_num: it relates to irq handler 
  * @param	interrupt:interrupt server function
  * @retval regiseter status 
  */
int ISR_Register(IRQn_Type irqnum, isr_handler_t cb)
{
	int ret = ISR_SUCC;	
	uint8_t irq_num;
	
	irq_num = (uint8_t)irqnum;
	if(irq_num >= TOTAL_IRQ_NUM)
	{
		return ISR_ERR;
	}
	
	if(isrObj[irq_num].IRQ_Flag)
	{
		return ISR_ERR;
	}
	else
	{		
		isrObj[irq_num].isrHandler = cb;		
		isrObj[irq_num].IRQ_Flag = true;
		INTC_ClearPendingIRQ(irqnum); // clear  interrupt flag 
		NVIC_ClearPendingIRQ(irqnum); // clear  interrupt flag 
		INTC_EnableIRQ(irq_num);
	}

	return ret;
}

/**
  * @brief  cancel register interrupt function according to  IRQn num.
  * @param  irq_num: it relates to irq handler 
  * @param	interrupt:interrupt server function
  * @retval cancel regiseter status 
  */
int ISR_DeRegister(IRQn_Type irqnum)
{
	int ret = ISR_SUCC;	
	uint8_t irq_num;
	
	irq_num = (uint8_t)irqnum;
	if(irq_num >= TOTAL_IRQ_NUM)
	{
		return ISR_ERR;
	}
	
	isrObj[irq_num].isrHandler = NULL;		
	isrObj[irq_num].IRQ_Flag = false;
	INTC_ClearPendingIRQ(irqnum); // clear  interrupt flag 
	NVIC_ClearPendingIRQ(irqnum); // clear  interrupt flag 


	return ret;
}
