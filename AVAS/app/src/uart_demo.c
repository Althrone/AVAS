/*******************************************************************************
*                  COPYRIGHT (C) 2021 CMS Technologies Ltd.                    *
*                                                                              *
********************************************************************************
* FileName      : uart_demo.c                                                  *
* Author        :                                                              *
* Version       : 1.0                                                          *
* Date          : 2021.08.13                                                   *
* Description   :                                                              *
* Function List :                                                              *
********************************************************************************/
#include "uart_demo.h"

unsigned char UART0_RX_BUF[UART_MAX_RECV_LEN];
unsigned short UART0_RX_STA = 0;
static UART_FRAME_t pData; /*pData used as an example,user can dinfine different paramater according to needs*/




/******************************************************************************
* Function Name: Uart0_Init
* @brief  UART0 init demo
* @param  bound
* @return init status
******************************************************************************/
int8_t Uart0_Init(uint32_t bound)
{
    int8_t ret;
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    UART_InitTypeDef UART_InitStructure = {0};

    GPIO_PinAFConfig(GPIO_PORT5, GPIO_Pin_1, GPIO_P51, GROUP_AF_TXD0);
    GPIO_PinAFConfig(GPIO_PORT5, GPIO_Pin_0, GPIO_P50, GROUP_AF_RXD0);

    /*TX GPIO CONFIG*/
    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Level   = GPIO_Level_HIGH;	
    GPIO_InitStruct.GPIO_Ctrl  = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT5, &GPIO_InitStruct);

    /*RX GPIO CONFIG*/
    GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_0 ;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Ctrl   = GPIO_Control_DIG;
    GPIO_Init(GPIO_PORT5, &GPIO_InitStruct);

    /*USART CONFIG*/
    UART_InitStructure.UART_BaudRate = bound;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;//һ��ֹͣλ
    UART_InitStructure.UART_Parity = UART_Parity_No;//����żУ��λ
    UART_InitStructure.phase = UART_Phase_Normal;
    UART_InitStructure.bitorder = UART_Bit_LSB;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx; //�շ�ģʽ

    ret = UART_Init(UART0, &UART_InitStructure); //��ʼ������

    if (ret)
    {
        SCI_ERROR_LOG(ret);
        return ret;
    }

    ISR_Register(ST0_IRQn, uart0_interrupt_send);     //����0�����жϷ���·��ע��
    ISR_Register(SR0_IRQn, uart0_interrupt_receive);  //����0�����жϷ���·��ע��


    return SCI_SUCCESS;
}


/******************************************************************************
* Function Name: Uart0_Send
* @brief  UART0 Send data
* @param  None
* @return None
*******************************************************************************/
void Uart0_Send(uint8_t ch)
{
    UART_SendByte(UART0, ch);
}
/*****************************************************************************
* Function Name: Uart0_Receive
* @brief  UART0 receive data
* @param  None
* @return rx data
*****************************************************************************/
char Uart0_Receive(void)
{
    return UART_ReceiveByte(UART0);
}

/*****************************************************************************
* Function Name: Uart0_IntSend
* @brief  UART0 Send data by interrupt
* @param  None
* @return None
*****************************************************************************/
void Uart0_IntSend(uint8_t *tx_buf, uint16_t tx_num)
{
    pData.data = tx_buf;
    pData.len = tx_num;

    INTC_SetPendingIRQ(ST0_IRQn);
}


/****************************************************************************
* Function Name: uart0_interrupt_send
* @brief  UART0 Send interrupt service routine
* @param  None
* @return None
*****************************************************************************/
void uart0_interrupt_send(void)
{
    UART_FRAME_t *pFrame = &pData;
    INTC_ClearPendingIRQ(ST0_IRQn);

    if ((pFrame->len > 0U) && pFrame->data)
    {
        UART0_TX = *pFrame->data;
        pFrame->data++;
        pFrame->len --;
    }
    else //send finished
    {
    }
}


/*****************************************************************************
* Function Name: uart_callback_error
* @brief
* @param  None
* @return None
******************************************************************************/
void uart_callback_error(uint8_t err_type)
{
    //user edit here when appear error
}

/*****************************************************************************
* Function Name: uart0_interrupt_receive
* @brief  UART0 Receive interrupt service routine
* @param  None
* @return None
*****************************************************************************/
void uart0_interrupt_receive(void)
{
    volatile uint8_t rx_data;
    volatile uint8_t err_type;

    INTC_ClearPendingIRQ(SR0_IRQn);
    err_type = UART_GetErrStaus(UART0, UART_FLAG_FEF | UART_FLAG_PEF | UART_FLAG_OVF);
    SCI_ClearFlag(UART0, err_type);

    if (err_type)
    {
        uart_callback_error(err_type);
    }

    rx_data = UART0_RX;

    if ((UART0_RX_STA & 0x8000U) == 0) //����δ���
    {
        if (UART0_RX_STA & 0x4000U) //���յ�0x0d
        {
            if (rx_data != 0x0a)
			{
				UART0_RX_BUF[UART0_RX_STA & 0x3fff] = rx_data;
                UART0_RX_STA++;					
			}
            else
            {
                UART0_RX_STA |= 0x8000;
                UART0_RX_BUF[UART0_RX_STA & 0x3fff] = rx_data;
                UART0_RX_STA ++;
            }
        }
        else                     //��δ���յ�0x0d
        {
            if (rx_data == 0x0d)
            {
                UART0_RX_STA |= 0x4000;
                UART0_RX_BUF[UART0_RX_STA & 0x3fff] = rx_data;
                UART0_RX_STA ++;
            }
            else
            {
                UART0_RX_BUF[UART0_RX_STA & 0x3fff] = rx_data;
                UART0_RX_STA ++;
            }
        }
    }
    else if ((UART0_RX_STA & 0x8000U) == 1) // receive finished
    {
    }
}


