#include "can_demo.h"
#include "gpio.h"
#include "can.h"
#include "isr.h"

CanTxRxMsg CAN_RecvMsg;

CANBuffList_t CanBufList = {0}; /*用于存放接收的rxmsg*/


uint8_t	Read_RingBuff(CanTxRxMsg *data)
{
	if(CanBufList.length == 0)
	{
		return 0;
	}
	*data = CanBufList.data[CanBufList.Head];
	CanBufList.Head = (CanBufList.Head+1)%LIST_BUF_MAX_NUM;
	CanBufList.length--;
	return 1;
}



static void can_recv_handler()
{
	INTC_ClearPendingIRQ(CAN0REC_IRQn);
	
	if (CAN_GetFlagStatus(CAN0, CAN_FLAG_REC) != RESET)
	{
		CAN_ClearFlag(CAN0, CAN_FLAG_REC);

		CAN_Receive_IT(CAN0,&CanBufList);		
	}
}

static void can_tx_handler()
{
	INTC_ClearPendingIRQ(CAN0TRX_IRQn);
	
	if (CAN_GetFlagStatus(CAN0, CAN_FLAG_TRX) != RESET)
	{
		CAN_ClearFlag(CAN0, CAN_FLAG_TRX);
	}
}

static void can_error_handler()
{
	INTC_ClearPendingIRQ(CAN0ERR_IRQn);
	
	CANErr_Recover(CAN0);
}

void CAN_Port_Config(void)
{
	GPIO_InitTypeDef	GPIO_InitStruct;
	CAN_InitTypeDef		CAN_InitStruct;
	

	/* GPIO AF set for CAN bus */
	GPIO_PinAFConfig(GPIO_PORT5, GPIO_Pin_0, GPIO_P50, GROUP_AF_CTXD);
	GPIO_PinAFConfig(GPIO_PORT5, GPIO_Pin_1, GPIO_P51, GROUP_AF_CRXD);

	
	/****** GPIO for CAN bus init ******/
	/* CTXD pin init */
    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_0 ;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Level  = GPIO_Level_HIGH;
	GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT5, &GPIO_InitStruct);
	
	/* CRXD pin init */
    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_1 ;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT5, &GPIO_InitStruct);
#endif
	
	// /****** GPIO for STB init ******/
	// GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_0;
	// GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;
	// GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
	// GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_DOWN;
	// GPIO_InitStruct.GPIO_Level  = GPIO_Level_LOW;
	// GPIO_Init(GPIO_PORT12, &GPIO_InitStruct);
	
	/****** CAN bus periphal init ******/
	//原时钟48M
	CAN_InitStruct.CAN_Prescaler = 2;
	CAN_InitStruct.CAN_BitRatePrescaler = 3;
	CAN_InitStruct.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStruct.CAN_BS1 = CAN_BS1_13tq;
	CAN_InitStruct.CAN_BS2 = CAN_BS2_2tq;
	CAN_InitStruct.CAN_OperateMode = CAN_OpMode_Initial;
	CAN_InitStruct.MASK1 = 0x1fffffffU;
	CAN_InitStruct.MASK2 = 0x1fffffffU;
	CAN_InitStruct.MASK3 = 0x1fffffffU;
	CAN_InitStruct.MASK4 = 0x1fffffffU;
	CAN_Init(CAN0, &CAN_InitStruct);
	
	CAN_OperatingModeRequest(CAN0, CAN_OpMode_Normal);
	
	CAN_ITConfig(CAN0, CAN_IT_REC |CAN_IT_TRX | CAN_IT_ERR_STATE | CAN_IT_ERR_PROTO, ENABLE);//使能接收和发送中断

	
	ISR_Register(CAN0REC_IRQn, can_recv_handler);
	
	ISR_Register(CAN0TRX_IRQn, can_tx_handler);
	
	ISR_Register(CAN0ERR_IRQn, can_error_handler);
}
