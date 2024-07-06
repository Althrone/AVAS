#ifndef __TIM_H__
#define __TIM_H__
#include <stdbool.h>
#include "common.h"

#define TIM_TDR_OFFSET		2
#define TIMER_CHAN_MAX_NUM   4
#define	TIM_SUCC             0
#define	TIM_ERR              1

#define TIM_Index_Channel0     0x00    
#define TIM_Index_Channel1     0x01    
#define TIM_Index_Channel2     0x02    
#define TIM_Index_Channel3     0x03    
#define IS_TIM_CHANIDEX(CHANIDX)		((CHANIDX == 0x00) || (CHANIDX == 0x01) || (CHANIDX == 0x02)|| (CHANIDX == 0x03))

#define TIM_Channel_0     0x01    //TO10 :P14
#define TIM_Channel_1     0x02    //TO11 :P15
#define TIM_Channel_2     0x04    //TO12 :P50
#define TIM_Channel_3     0x08    //TO13 :P51
#define IS_TIM_CHANNEL(CHAN)		(((CHAN & 0xf0) == 0x00) && ( CHAN != 0x00))
#define IS_TIM_CHANNEL0(CHAN)		(CHAN == TIM_Channel_0)

/** @defgroup TTM_Polarity 
  * @{
  */
#define TTM_Polarity_High(CHAN)		(0x00)  
#define TTM_Polarity_Low(CHAN)		(CHAN)  

typedef enum
{
	TTM_Chan_None = 0x00, //when choose it will not use multi-tim combination
	TTM_Chan_0 = 0x01,
	TTM_Chan_2 = 0x04, 
}TTM_Master_Chan_t; //multi-channels function, even channel must be setted as master channel,it can choose one or more even channel as master channels
#define IS_TIM_Master_Chan(Master_Chan) (((Master_Chan) == TIM_Channel_0 || (Master_Chan) == TIM_Channel_2 ||  \
											(Master_Chan) == (TIM_Channel_0|TIM_Channel_2) )? 1:0)   //239需要增加


typedef enum
{
	TIM_NEGEDGE = 0x00,  // falling edge of TI
	TIM_POSEDGE = 0x01,  // rising edge of TI 
	TIM_BOTHEDGE = 0x02, //  both edge of TI 
}TIM_Edge_t;

#define TIM_CLK0_MASK	0x000F
#define TIM_CLK1_MASK	0x00F0
#define TIM_CLK2_MASK	0x0300
#define TIM_CLK3_MASK	0x3000

typedef enum
{
	TIM_CLK0_Div1     = 0x0000,  //Operation clock slection FCLK/ 2^0
	TIM_CLK0_Div2     = 0x0001,	//Operation clock slection FCLK/ 2^1		
	TIM_CLK0_Div4     = 0x0002,  //Operation clock slection FCLK/2^2
	TIM_CLK0_Div8     = 0x0003,  //Operation clock slection FCLK/2^3
	TIM_CLK0_Div16    = 0x0004,  //Operation clock slection FCLK/2^4
	TIM_CLK0_Div32    = 0x0005,  //Operation clock slection FCLK/2^5
	TIM_CLK0_Div64    = 0x0006,  //Operation clock slection FCLK/2^6
	TIM_CLK0_Div128   = 0x0007,  //Operation clock slection FCLK/2^7
	TIM_CLK0_Div256   = 0x0008,  //Operation clock slection FCLK/2^8
	TIM_CLK0_Div512   = 0x0009,  //Operation clock slection FCLK/2^9
	TIM_CLK0_Div1024  = 0x000A,  //Operation clock slection FCLK/2^10
	TIM_CLK0_Div2048  = 0x000B,  //Operation clock slection FCLK/2^11
	TIM_CLK0_Div4096  = 0x000C,  //Operation clock slection FCLK/2^12
	TIM_CLK0_Div8192  = 0x000D,  //Operation clock slection FCLK/2^13
	TIM_CLK0_Div16384 = 0x000E,	//Operation clock slection FCLK/2^14
	TIM_CLK0_Div32768 = 0x000F,  //Operation clock slection FCLK/2^15
}TIM_Clk0_t;
#define IS_TIM_CLK0(DIV) ((DIV & ~TIM_CLK0_MASK) == 0)
typedef enum
{
	TIM_CLK1_Div1     = 0x0000,  //Operation clock slection FCLK/ 2^0
	TIM_CLK1_Div2     = 0x0010,	//Operation clock slection FCLK/ 2^1		
	TIM_CLK1_Div4     = 0x0020,  //Operation clock slection FCLK/2^2
	TIM_CLK1_Div8     = 0x0030,  //Operation clock slection FCLK/2^3
	TIM_CLK1_Div16    = 0x0040,  //Operation clock slection FCLK/2^4
	TIM_CLK1_Div32    = 0x0050,  //Operation clock slection FCLK/2^5
	TIM_CLK1_Div64    = 0x0060,  //Operation clock slection FCLK/2^6
	TIM_CLK1_Div128   = 0x0070,  //Operation clock slection FCLK/2^7
	TIM_CLK1_Div256   = 0x0080,  //Operation clock slection FCLK/2^8
	TIM_CLK1_Div512   = 0x0090,  //Operation clock slection FCLK/2^9
	TIM_CLK1_Div1024  = 0x00A0,  //Operation clock slection FCLK/2^10
	TIM_CLK1_Div2048  = 0x00B0,  //Operation clock slection FCLK/2^11
	TIM_CLK1_Div4096  = 0x00C0,  //Operation clock slection FCLK/2^12
	TIM_CLK1_Div8192  = 0x00D0,  //Operation clock slection FCLK/2^13
	TIM_CLK1_Div16384 = 0x00E0,	//Operation clock slection FCLK/2^14
	TIM_CLK1_Div32768 = 0x00F0,  //Operation clock slection FCLK/2^15
}TIM_Clk1_t;
#define IS_TIM_CLK1(DIV) ((DIV & ~TIM_CLK1_MASK) == 0)

typedef enum
{
	TIM_CLK2_Div2  = 0x0000,
	TIM_CLK2_Div4  = 0x0100,
	TIM_CLK2_Div16 = 0x0200,
	TIM_CLK2_Div64 = 0x0300,
}TIM_Clk2_t;
#define IS_TIM_CLK2(DIV) ((DIV & ~TIM_CLK2_MASK) == 0)

typedef enum
{
	TIM_CLK3_Div256    = 0x0000,
	TIM_CLK3_Div1024   = 0x1000,
	TIM_CLK3_Div4096   = 0x2000,
	TIM_CLK3_Div16384  = 0x3000,
}TIM_Clk3_t;
#define IS_TIM_CLK3(DIV) ((DIV & ~TIM_CLK3_MASK) == 0)

typedef enum
{
	TIM_Selection_Slave       = 0x0000,  //config for channel2 4 or 6
	TIM_Selection_Bits16      = 0x0000,  //config for channel 1 or 3
	TIM_Selection_Bits8       = 0x0800,  //config for channel 1 or 3
	TIM_Selection_Master      = 0x0800,   //config for channel 2 4 or 6
}TIM_Selection_t;

typedef enum
{
	TIM_Trigger_Software = 0x0000,     // only software trigger start is valid
	TIM_Trigger_IputEdge = 0x0100,     // TImn valid input edge is used as a start or capture trigger 
	TIM_Trigger_BothEdge = 0x0200,     //TImn input edges are used as a start or capture trigger 
	TIM_Trigger_UseMaster_Int = 0x0400,  //when slave channel use master channel's interrupt
}TIM_Trigger_t;
typedef enum
{
	TIM_Pulse_Falling   = 0x0000,      // measure falling edge interval of TI
	TIM_Pulse_Rising 	= 0x0040,      // measure rising edge interval of TI 
	TIM_Pulse_Both 	 	= 0x0080,      // measure both edge interval of TI 
	TIM_Pulse_Both_Width_Low = 0x0080, // measure low level width of TI 
	TIM_Pulse_Both_Width_High= 0x00C0, // measure high level width of TI 
}TIM_Pulse_t;

typedef enum
{
	TIM_Mode_PWM_Master = 0x0000U,
	TIM_Mode_Interval   = 0x0001U,
	TIM_Mode_Square     = 0x0002U,
	TIM_Mode_DivFreq    = 0x0003U,
	TIM_Mode_PluseInterval = 0x0004,
	TIM_Mode_EventCount    = 0x0006,
	TIM_Mode_DelayCount    = 0x0008,
	TIM_Mode_SinglePulse   = 0x0008,
	TIM_Mode_Measure       = 0x000C,
}TIM_Mode_t;


#define IS_TIM(TIMx)		(TIMx == TM40) 

#define TIM_StartInt_Disable                 ((uint8_t)0x00) //Interrupt disable
#define TIM_StartInt_Enable         		 ((uint8_t)0x01) //Interrupt enable

typedef enum
{
	TIM4_CH0_Input_TI00 = 0x00,  // intput signal of timeer input pin (TI00) 
	TIM4_CH0_Input_ELC  = 0x10,  // event input signal from ELC 
	TIM4_CH1_Input_TI01 = 0x00,  // input signal of timer input pin (TI01)
	TIM4_CH1_Input_ELC  = 0x01,  //event input signal from ELC 
	TIM4_CH1_Input_fIL 	= 0x04,  // low-speed on-chip oscillator clock (fIL) 
	TIM4_CH1_Input_fSUB	= 0x05,  // subsystem clock (fSUB) 
}TIM4_Input_t;

typedef struct
{	
	uint8_t TIM_Channel;       /*!< Specifies the TIM channel. This parameter can be a value of @ref TTM_Channel_t */
	
	TIM_Pulse_t TIM_Pulse_Edge;	   /*!< Specifies the TIM pluse edge This parameter can be a value of @ref TIM_Pulse_t */

	uint16_t TIM_Period[TIMER_CHAN_MAX_NUM];  /*!< Specifies the period value to be loaded into the active
	                                   Auto-Reload Register at the next update event.This parameter must be a number between 0x0000 and 0xFFFF.	*/ 
									   	
	uint16_t TIM_ClkDivision;     /*!< Specifies the clock division of different TIM channel
	                                  This parameter can be a value of @ref TIM_Clkx_t */

	uint16_t TIM_Selection_Master; /*!< Selection of  master of channle n or selection of 16 bits timer or 8bits timer
								  This parameter can be a value of @ref TTM_Master_Chan_t */

	uint16_t TIM_Slave_Polarity; /*!< Selection polarity of  slave channle ,it decides the valid level of PWM waves
								  This parameter can be a value of @ref TTM_Polarity */
								  	
	uint16_t TIM_Trigger; 	     /*!< setting of start trigger or capture trigger of channle n
									This parameter can be a value of @ref TIM_Trigger_t */
									  
	uint16_t TIM_Mode; 	         /*!< Specifies the TIM work mode of different TIM channel
									      This parameter can be a value of @ref TIM_Mode_t */

	uint8_t TIM_StartInt;	      /*!< whether interrupt is generated when counting is started .*/

	TIM4_Input_t TIM4_Input;	  /*!< Specifies the input signal for channel of timer  
									  This parameter can be a value of @ref TIM_Input_t */
											
}TIM_InitTypeDef;       


#define TM_CLOCK_SELECT_CKM0             (0x0000U) // operation clock CK0 set by PRS register 
#define TM_CLOCK_SELECT_CKM1             (0x8000U) // operation clock CK1 set by PRS register 
#define TM_CLOCK_SELECT_CKM2             (0x4000U) // operation clock CK2 set by PRS register 
#define TM_CLOCK_SELECT_CKM3             (0xC000U) // operation clock CK3 set by PRS register 
/* Counter overflow status of channel n (OVF) */
#define _0000_TM4_OVERFLOW_NOT_OCCURS     (0x0000U) // overflow does not occur 
#define _0001_TM4_OVERFLOW_OCCURS         (0x0001U) // overflow occurs 



void TIM_Start(TM40_Type* TM4x, uint8_t ChxIdx);
void TIM_Stop(TM40_Type* TM4x, uint8_t ChxIdx);
void TIM_Cmd(TM40_Type* TM4x, uint8_t ChxIdx, FunctionalState state);
void TIM_SetCounter(TM40_Type* TM4x, uint8_t ChxIdx,uint16_t counter);
bool TIM_GetStatus(TM40_Type* TM4x, uint8_t ChxIdx);
int TIM_Init(TM40_Type* TM4x,TIM_InitTypeDef *TIM_InitStruct);
uint32_t TIM_GetPulsePeriod(TM40_Type* TM4x, uint8_t ChxIdx);
uint32_t TIM_GetPulseWidth(TM40_Type* TM4x,  uint8_t ChxIdx);

#endif
