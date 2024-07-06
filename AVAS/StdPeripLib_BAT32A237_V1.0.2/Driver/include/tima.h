#ifndef __TIMA_H__
#define __TIMA_H__
#include "common.h"

#define TMA_COUNT_START                          (0x01U) /* count starts */
/* TAIO input polarity select bit (TEDGPL)输入边沿极性选择 */
#define TMA_TAIO_POLARITY_ONE                    (0x00U) /* one edge */
#define TMA_TAIO_POLARITY_BOTH                   (0x08U) /* both edges */
/* TAIO event input control bit (TIOGT1,TIOGT0) */
#define TMA_EVENT_ENABLE_ALWAYS                  (0x00U) /* event is counted */
#define TMA_EVENT_ENABLE_INTP4                   (0x40U) /* event is counted during INTP4 specified period */
#define TMA_EVENT_ENABLE_TIMEROUTSIGNAL          (0x80U) /* event is counted during polarity period specified for timer output signal */
/* PWM signal and INTP4 polarity selection (RCCPSEL2) */
#define TMA_PWM_POLARITY_L                       (0x00U) /* L period is counted */
#define TMA_PWM_POLARITY_H                       (0x04U) /* H period is counted */
/* PWM signal selection (RCCPSEL1,RCCPSEL0) */
#define TMA_PWM_TMIOD1                           (0x00U) /* TRDIOD1 */
#define TMA_PWM_TMIOC1                           (0x01U) /* TRDIOC1 */
#define TMA_PWM_TO02                             (0x02U) /* TO02 */
#define TMA_PWM_TO03                             (0x03U) /* TO03 */
/* Timer A underflow flag (TUNDF) */
#define TMA_FLAG_NOT_UNDERFLOW            (0x00U) /* no underflow */
#define TMA_FLAG_UNDERFLOW                        (0x20U) /* underflow */
/* Active edge judgment flag (TEDGF) */
#define TMA_FLAG_ACTIVE_EDGE_UNRECEIVED               (0x00U) /* active edge not received */
#define TMA_FLAG_ACTIVE_EDGE_RECEIVED                 (0x10U) /* active edge received (end of measurement period) */
/* Timer A count status flag (TCSTF) */
#define TMA_FLAG_STATUS_STOP                          (0x00U) /* count stops */
#define TMA_FLAG_STATUS_COUNT                         (0x02U) /* during count */
#define IS_TMA_FLAG(FLAG)	((FLAG == TMA_FLAG_UNDERFLOW) || (FLAG == TMA_FLAG_ACTIVE_EDGE_RECEIVED) || (FLAG ==TMA_FLAG_STATUS_COUNT))


/** @defgroup TIMA_Edge_t 
  * @{
  */
#define TMA_RISING_EDGE         0x0001U
#define TMA_FALLING_EDGE        0x0002U
#define TMA_BOTH_EDGE        	(TMA_FALLING_EDGE | TMA_RISING_EDGE) 
#define IS_TMA_EDGE(edge)		((((edge) & (uint16_t)0xFFFC) == 0x0000U) && ((edge) != (uint16_t)0x0000U))

/** @defgroup TMA_Ctrl_t 
  * @{
  */
#define TMA_COUNT_AWLAYS		0x0000U
#define TMA_INTP4_LOW_LEVEL		0x0001U
#define TMA_INTP4_HIGH_LEVEL	0x0002U

#define TMM_TMIOD1_LOW_LEVEL	0x0010U
#define TMM_TMIOD1_HIGH_LEVEL	0x0020U
#define TMM_TMIOC1_LOW_LEVEL	0x0040U
#define TMM_TMIOC1_HIGH_LEVEL	0x0080U

#define TIM_TO02_LOW_LEVEL		0x0100U
#define TIM_TO02_HIGH_LEVEL		0x0200U
#define TIM_TO03_LOW_LEVEL		0x0400U
#define TIM_TO03_HIGH_LEVEL		0x0800U
#define IS_TMA_CTRL(ctrl)		(((ctrl) & (uint16_t)0xF00CU) == 0x0000U)


typedef enum 
{
    TMA_FCLK_Div1		= 0x00U,
    TMA_FCLK_Div8		= 0x10U,
    TMA_FCLK_Div2		= 0x30U,
    TMA_FIL				= 0x40U,
    TMA_EventCount		= 0x50U,
    TMA_FSUB			= 0x60U,
}TMA_Clk_t;

typedef enum
{
	TMA_Mode_Interval   = 0x00U,
	TMA_Mode_Square     = 0x01U,
	TMA_Mode_EventCount = 0x02U,
	TMA_Mode_PluseWidth = 0x03U,
	TMA_Mode_PlusePeriod= 0x04U,
}TMA_Mode_t;

typedef enum
{
	TMA_Polarity_0    =  0x00U,  //事件计数时：上升沿计数； 方波模式：初始电平为高电平
	TMA_Polarity_1    = 0x01U,
}TMA_Polarity_t;

typedef enum
{
	TMA_Pulse_Width_High= 0x00U, // measure high level width of TI 
	TMA_Pulse_Width_Low = 0x01U, // measure low level width of TI
	TMA_Pulse_Rising 	= 0x02U, // measure rising edge interval of TI  	
	TMA_Pulse_Falling   = 0x03U, // measure falling edge interval of TI	
}TMA_Pulse_t;

typedef struct
{
    uint16_t TIMA_Edge;	   /*!< Specifies the valid edge when eventcount
									 This parameter can be a value of @ref TIMA_Edge_t */
	
	uint16_t TMA_Ctrl;		 /*!< TMA counter events under the condition of the paramters of TMA_Ctrl_t
									 This parameter can be a value of @ref TMA_Ctrl_t */
}TMA_EVNT_t;

typedef struct
{		

	uint16_t TMA_Period;            /*!< Specifies the period value to be loaded into the active
	                                   Auto-Reload Register at the next update event.This parameter must be a number between 0x0000 and 0xFFFF.	*/ 
									   	
	uint16_t TMA_Clk;     /*!< Specifies the count clock source of TIMA
	                                  This parameter can be a value of @ref TMA_Clk_t */
																	  
	TMA_Mode_t TMA_Mode; 	         /*!< Specifies the TIM work mode of different TIM channel
									      This parameter can be a value of @ref TIMA_Mode_t */

	uint8_t TMA_Polarity;	      /*!< TAIO polarity switch bit (TEDGSEL) .*/
	
	TMA_EVNT_t TMA_EventPara;		 /*!< Specifies the TMA counter condition  
										 This parameter can be a value of @ref TMA_EVNT_t */
	TMA_Pulse_t TMA_Pulse;	
}TMA_InitTypeDef; 

void TMA_Init(TMA_InitTypeDef *TIMA_InitStruct);
void TMA_Start(void);
void TMA_Stop(void);
void TMA_ClearFlag(uint8_t TMA_FLAG);
FlagStatus TMA_GetStaus(uint8_t TMA_FLAG);
void TMA_Init(TMA_InitTypeDef *TIMA_InitStruct);
uint32_t TMA_GetPulseWidth(void);
uint32_t TMA_GetPulsePeriod(void);

#endif
