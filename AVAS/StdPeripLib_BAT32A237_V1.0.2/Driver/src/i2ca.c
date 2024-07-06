/**
  ******************************************************************************
  * @file    i2ca.c
  * @author  CMS Application Team
  * @version Vx.x.x
  * @date    24-April-2022
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Universal Inter Integrated Circuit (I2CA):           
  @verbatim       
 ===============================================================================
                        ##### How to use this driver #####
 ===============================================================================
    [..]
            
    @endverbatim        
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "i2ca.h"
#include "cgc.h"

/** @addtogroup bat32g135_StdPeriph_Driver
  * @{
  */

/** @defgroup I2CA 
  * @brief I2CA driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I2CA_MAX_CLOCK  (20000000U)
#define I2CA_DIR_TRANSMIT   0x01
#define I2CA_DIR_RECEIVE    0x02

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  I2CA module Deinit.
  * @param  None
  * @retval None
  */
void I2CA_DeInit(void)
{
    CGC_PER0PeriphClockCmd(CGC_PER0Periph_I2CA, DISABLE);
}

/**
  * @brief  Initializes the I2CA peripheral according to the specified
  *         parameters in the I2CA_InitStruct .
  * @param  I2CA_InitStruct: pointer to a I2CA_InitTypeDef structure that contains
  *         the configuration information for the specified I2CA peripheral.
  * @retval None
  */
void I2CA_Init(I2CA_InitTypeDef* I2CA_InitStruct)
{
    uint8_t tmpreg = 0;
    uint8_t tmp_wl, tmp_wh;
    uint32_t fmck;

    /* Check the parameters */
    assert_param(IS_I2CA_CLOCK_SPEED(I2CA_InitStruct->I2C_ClockSpeed));
    assert_param(IS_I2CA_MODE(I2CA_InitStruct->I2C_Mode));
    assert_param(IS_I2CA_DUTY_CYCLE(I2CA_InitStruct->I2C_DutyCycle));
    assert_param(IS_I2CA_OWN_ADDRESS(I2CA_InitStruct->I2C_OwnAddress));
    assert_param(IS_I2CA_ACK_STATE(I2CA_InitStruct->I2C_Ack));

    CGC_PER0PeriphClockCmd(CGC_PER0Periph_I2CA, ENABLE);

    /* I2CA register setting for control register n1 */
    tmpreg = IICA->IICCTL01;
    if (I2CA_InitStruct->I2C_ClockSpeed <= 100000)
    {
        /* operates in standard mode */
        tmpreg &= ~(1 << I2CA_OPERATE_BIT);
        tmpreg &= ~(1 << I2CA_DIGITAL_FILTER_BIT);
    }
    else
    {
        /* operates in fast mode and open filter */
        tmpreg |=  (1 << I2CA_OPERATE_BIT);
        tmpreg |=  (1 << I2CA_DIGITAL_FILTER_BIT);
    }

    /* clock for I2CA division select */
    if (SystemCoreClock >= 1000000 && SystemCoreClock <= I2CA_MAX_CLOCK) 
    {
        tmpreg &= ~(1 << I2CA_FCLK_DIV_BIT);
        fmck = SystemCoreClock;
    }
    else if (SystemCoreClock > I2CA_MAX_CLOCK)
    {
        tmpreg |=  (1 << I2CA_FCLK_DIV_BIT);
		fmck = SystemCoreClock/2;
    }

    IICA->IICCTL01 = tmpreg;

    /* Own address setting for I2C bus */
    IICA->SVA0 = I2CA_InitStruct->I2C_OwnAddress;

    /* I2CA register setting for flag register n */
    tmpreg = IICA->IICF0;

    /* enable generation of a start condition without detecting a stop condition */
    tmpreg |=  (1 << I2CA_STOP_DETECTION_BIT);

    /* enable  Communication reservation */
    tmpreg |=  (1 << I2CA_RESERVATION_BIT);

    IICA->IICF0 = tmpreg;

    /* I2CA bus clock option setting */
    if (I2CA_InitStruct->I2C_ClockSpeed <= 100000)
    {
        if (I2CA_InitStruct->I2C_Mode == I2CA_Mode_SMBusSlave)
        {
            tmp_wl = 4.7 * fmck / 1000000;
            tmp_wh = 5.3 * fmck / 1000000;
        }
        else if (I2CA_InitStruct->I2C_Mode == I2CA_Mode_SMBusMaster)
        {
            tmp_wl = (fmck) * 0.47 / (I2CA_InitStruct->I2C_ClockSpeed);
            tmp_wh = (fmck) * 0.53 / (I2CA_InitStruct->I2C_ClockSpeed);
        }
    }
    else if (I2CA_InitStruct->I2C_ClockSpeed <= 400000)
    {
        if (I2CA_InitStruct->I2C_Mode == I2CA_Mode_SMBusSlave)
        {
            tmp_wl = 1.3 * fmck / 1000000;
            tmp_wh = 1.2 * fmck / 1000000;
        }
        else if (I2CA_InitStruct->I2C_Mode == I2CA_Mode_SMBusMaster)
        {
            tmp_wl = (fmck) * 0.52 / (I2CA_InitStruct->I2C_ClockSpeed);
            tmp_wh = (fmck) * 0.48 / (I2CA_InitStruct->I2C_ClockSpeed);
        }
    }
    else
    {
        if (I2CA_InitStruct->I2C_Mode == I2CA_Mode_SMBusSlave)
        {
            tmp_wl = 0.5 * fmck / 1000000;
            tmp_wh = 0.5 * fmck / 1000000;
        }
        else if (I2CA_InitStruct->I2C_Mode == I2CA_Mode_SMBusMaster)
        {
            tmp_wl = (fmck) * 0.5 / (I2CA_InitStruct->I2C_ClockSpeed);
            tmp_wh = (fmck) * 0.5 / (I2CA_InitStruct->I2C_ClockSpeed);
        }
    }

    /* IICWL0 and IICWH0 register setting to config clock square option */
    IICA->IICWL0 = tmp_wl;
    IICA->IICWH0 = tmp_wh;

    /* I2CA bus option setting for control register n0 */
    /* interrupt request is generated at the ninth clock's falling edge */
    IICA->IICCTL00 |=  (1 << I2CA_WTIM_BIT);

    /* acknowledgment config */
    if (I2CA_InitStruct->I2C_Ack)
        IICA->IICCTL00 |=  (1 << I2CA_ACK_BIT);
    else
        IICA->IICCTL00 &= ~(1 << I2CA_ACK_BIT);

    /* enable operation */
    IICA->IICCTL00 |=  (1 << I2CA_EN_BIT);

    /* this exits from the current communications and sets standby mode */
    IICA->IICCTL00 |=  (1 << I2CA_LREL_BIT);
}

/**
  * @brief  Enables or disables the specified I2CA peripheral.
  * @param  NewState: new state of the I2CA peripheral.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2CA_Cmd(FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if (NewState != DISABLE)
	{
		/* Enable the I2CA periphal by setting the SPIE register */
		IICA->IICCTL00 |= IICA_IICCTL00_SPIE_Msk;
	}
	else
	{
		/* Disable the I2CA periphal by setting the SPIE register */
		IICA->IICCTL00 &= ~IICA_IICCTL00_SPIE_Msk;
	}
}

/**
  * @brief  Enables or disables the specified I2CA acknowledgement.
  * @param  NewState: new state of the I2CA acknowledgement.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2CA_Acknowledge_Cmd(FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if (NewState != DISABLE)
	{
		/* Enable the I2CA acknowledgement by setting the ACKE register */
		IICA->IICCTL00 |= IICA_IICCTL00_ACKE_Msk;
	}
	else
	{
		/* Disable the I2CA acknowledgement by setting the ACKE register */
		IICA->IICCTL00 &= ~IICA_IICCTL00_ACKE_Msk;
	}
}

/**
  * @brief  Configurate the Interrupt positon depend on the register WTIM bit in IICCTL00.
  * @param  Intpos: Interrupt position config.
  *          This parameter can be one of the following values:
  *            @arg I2CA_INTPOS_8CLK	: In the 8th clock to generate intterupt signal.
  *            @arg I2CA_INTPOS_9CLK	: In the 9th clock to generate intterupt signal.
  * @retval None
  */
void I2CA_InterruptPosition_Config(uint8_t Intpos)
{
    /* Check the parameters */
    assert_param(IS_I2CA_INTPOS(Intpos));

    /* 
    Generate the interrupt signal int the 8th clock falling edge.
    Master role : when the Nth clock is completed, set the clock is low to wait.
    Slave role  : when the Nth clock is completed, set the clock is low to wait master.
    Note : N is depend on param intpos.
    */
    if (Intpos == I2CA_INTPOS_8CLK)
    {
        IICA->IICCTL00 &= ~IICA_IICCTL00_WTIM_Msk;
    }
    else if (Intpos == I2CA_INTPOS_9CLK)
    {
        IICA->IICCTL00 |= IICA_IICCTL00_WTIM_Msk;
    }
}

/**
  * @brief  To relieve the wait for I2CA bus state
  * @param  NewState: new state of the I2CA waiting relieve.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  * @Note   the WREL bit will auto reset when the relieve is completed.
  */
void I2CA_WaitRelieve_Cmd(FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if (NewState != DISABLE)
	{
		/* Relieve the wait for bus state */
		IICA->IICCTL00 |=  IICA_IICCTL00_WREL_Msk;
	}
	else
	{
		IICA->IICCTL00 &= ~IICA_IICCTL00_WREL_Msk;
	}
}

/**
  * @brief  Configurate the I2CA own address to SVA0 register.
  * @param  Address: Set the device address to register.
  * @retval None
  */
void I2CA_OwnAddress_Config(uint8_t Address)
{
	/* Check the parameters */
	assert_param(IS_I2CA_OWN_ADDRESS(Address));

    IICA->SVA0 = Address;
}

/**
  * @}
  */

/** @defgroup I2CA Data transfers functions
 *  @brief   Data transfers functions
 *
@verbatim   
 ===============================================================================
                      ##### Data transfers functions #####
 ===============================================================================  

 [..] This section provides a set of functions allowing to manage the I2CA data 
      transfers. In reception, data are received and then stored into an internal 
      buffer while. In transmission, data are first stored into an internal send
      buffer before being transmitted.

 [..] The read access of the IICA0 register can be done using the I2CA_ReadByte()
      function and returns the Rx register value. Whereas a write access to the IICA0 
      can be done using I2CA_WriteByte() function and stores the written data into 
      Tx register.

@endverbatim
  * @{
  */

/**
  * @brief  Read the register data for IICA0.
  * @param  None.
  * @retval The data value for IICA0.
  */
uint8_t I2CA_ReadByte(void)
{
    return (IICA->IICA0);
}

/**
  * @brief  Write the data to IICA0 register.
  * @param  The data which to be write.
  * @retval None.
  */
void I2CA_WriteByte(uint8_t Data)
{
    IICA->IICA0 = Data;
}

/**
  * @brief  Generates I2CA communication START condition.
  * @param  None
  * @retval None.
  */
void I2CA_GenerateSTART(void)
{
    /* Generate a START condition */
    IICA->IICCTL00 |=  (1 << I2CA_START_BIT);
}

/**
  * @brief  Generates I2CA communication STOP condition.
  * @param  None
  * @retval None.
  */
void I2CA_GenerateSTOP(void)
{
    /* Generate a STOP condition */
    IICA->IICCTL00 |=  (1 << I2CA_STOP_BIT);
}

/**
  * @brief  Checks whether the specified I2CA flag is set or not.
  * @param  I2CA_FLAG: specifies the flag to check.
  *          This parameter can be one of the following values:
  *            @arg I2CA_STATUS_SPD	: the STOP condition check status.
  *            @arg I2CA_STATUS_STD	: the START condition check status.
  *            @arg I2CA_STATUS_ACK	: the ACK condition check status.
  *            @arg I2CA_STATUS_TRC	: bus status in transmit(is 1) or receive(is 0) mode.
  *            @arg I2CA_STATUS_COI	: check the address is matched.
  *            @arg I2CA_STATUS_EXC	: the extend code received or not status.
  *            @arg I2CA_STATUS_ALD	: the arbitrate success(is 0) or fail(is 1).
  *            @arg I2CA_STATUS_MSTS: to display the communication role is master(is 1) or slave(is 0).
  *            @arg I2CA_STATUS_BUSBSY: I2C bus is busy or idle.
  *            @arg I2CA_STATUS_STCF: Generate START condition or not.
  * @retval The new state of I2CA_FLAG (SET or RESET).
  */
FlagStatus I2CA_GetFlagStaus(uint16_t I2CA_FLAG)
{
    FlagStatus bitstatus;
	uint8_t tmp_reg = 0;
    uint8_t tmp_flag = 0;

    /* Check the parameters */
    assert_param(IS_I2CA_STATUS(I2CA_FLAG));

    if (I2CA_FLAG == I2CA_STATUS_BUSBSY || I2CA_FLAG == I2CA_STATUS_STCF)
	{
		tmp_reg = IICA->IICF0;
		tmp_flag = (uint8_t)(I2CA_FLAG >> 8);
	}
	else
	{
		tmp_reg = IICA->IICS0;
		tmp_flag = (uint8_t)(I2CA_FLAG & 0xFF);
	}

    if ((tmp_reg & tmp_flag) != (uint16_t)RESET)
        bitstatus = SET;
    else
        bitstatus = RESET;

    return bitstatus;
}

/**
  * @brief  Send a data buffer through the I2CA peripheral to write.
  * @param  Address: Device address in the I2C bus.
  * @param  Reg: Register address in the Device.
  * @param  Data: Data buffer address.
  * @param  Len: Data buffer length need to transmit.
  * @retval None
  */
I2CA_Status I2CA_Master_WriteData(uint8_t Address, uint8_t Reg, uint8_t *Data, uint16_t Len)
{
	I2CA_Status status = OK;
	
	if (I2CA_GetFlagStaus(I2CA_STATUS_BUSBSY) != RESET)
	{
		status = BUSY;
	}
	else
	{
		I2CA_GenerateSTART();
		
		IICA->IICA0 = Address & 0xFE;
		
        while(!INTC_GetPendingIRQ(IICA_IRQn));
        INTC_ClearPendingIRQ(IICA_IRQn);
		
		IICA->IICA0 = Reg;
		
        while(!INTC_GetPendingIRQ(IICA_IRQn));
        INTC_ClearPendingIRQ(IICA_IRQn);
		
		do
		{
			IICA->IICA0 = *Data++;

			while(!INTC_GetPendingIRQ(IICA_IRQn));
			INTC_ClearPendingIRQ(IICA_IRQn);
			
		} while(--Len);
		
        /* stop condition is generated */
        I2CA_GenerateSTOP();
	}
	
	return status;
}

/**
  * @brief  Read a data buffer through the I2CA peripheral to device register address.
  * @param  Address: Device address in the I2C bus.
  * @param  Reg: Register address in the Device.
  * @param  Data: Data buffer address.
  * @param  Len: Data buffer length need to transmit read.
  * @retval None
  */
I2CA_Status I2CA_Master_ReadData(uint8_t Address, uint8_t Reg, uint8_t *Data, uint16_t Len)
{
	I2CA_Status status = OK;
	
	if (I2CA_GetFlagStaus(I2CA_STATUS_BUSBSY) != RESET)
	{
		status = BUSY;
	}
	else
	{
        /* Generate the START condition */
		I2CA_GenerateSTART();
		
		IICA->IICA0 = Address & 0xFE;
		
        /* Wait device address send succes */
        while(!INTC_GetPendingIRQ(IICA_IRQn));
        INTC_ClearPendingIRQ(IICA_IRQn);
		
		IICA->IICA0 = Reg;
		
        /* Wait device register send succes */
        while(!INTC_GetPendingIRQ(IICA_IRQn));
        INTC_ClearPendingIRQ(IICA_IRQn);

        /* Generate the RESTART condition */
        I2CA_GenerateSTART();

		IICA->IICA0 = Address | 0x01;
		
        /* Wait address send succes */
        while(!INTC_GetPendingIRQ(IICA_IRQn));
        INTC_ClearPendingIRQ(IICA_IRQn);
		
		do {
            /* When the last byte to receive, we should not output ACK */
            if(Len == 1U)
            {
                /* acknowledgment disable in the last byte to receive */
                IICA->IICCTL00 &= ~(1 << I2CA_ACK_BIT);
            }

            /* Release the bus status for receive data */
			IICA->IICCTL00 |= (1 << I2CA_WREL_BIT);
			
            /* Wait data receive succes */
			while(!INTC_GetPendingIRQ(IICA_IRQn));
			INTC_ClearPendingIRQ(IICA_IRQn);
			
            /* copy the receive data to target memory */
			*Data++ = IICA->IICA0;
			
		} while(--Len);
		
        /* STOP condition is generated */
        I2CA_GenerateSTOP();
	}
	
	return status;
}

/**
  * @brief  Receive a data buffer through the I2CA peripheral to device register address by slave mode.
  * @param  Data: Data buffer address.
  * @param  Size: Data size when the slave hope to recceive.
  * @param  Len: Data length in buffer received.
  * @retval status: This parameter can be a value of @ref I2CA_Status.
  */
I2CA_Status I2CA_Slave_ReceiveData(uint8_t *Data, uint16_t Size, uint16_t *Len)
{
	I2CA_Status status = OK;
    uint8_t Address_Match = 0;
    uint16_t Recv_Length = 0;

    while(1)
    {
        while(!INTC_GetPendingIRQ(IICA_IRQn));
        INTC_ClearPendingIRQ(IICA_IRQn);

        /* Check stop condition */
        if (IICA->IICS0 & IICA_IICS0_SPD_Msk)
        {
            /* Disable I2CA */
            IICA->IICCTL00 &= ~IICA_IICCTL00_SPIE_Msk;
            break;
        }
        else
        {
			/* When the address match is success */
            if (!Address_Match && (IICA->IICS0 & IICA_IICS0_COI_Msk))
            {
                Address_Match = 1;
				
				/* SPIE0 = 1: enable I2CA */
				IICA->IICCTL00 |= IICA_IICCTL00_SPIE_Msk;
				
				/* interrupt request is generated at the eighth clock's falling edge */
                IICA->IICCTL00 &= ~IICA_IICCTL00_WTIM_Msk;
				
				/* cancel wait */
                IICA->IICCTL00 |= IICA_IICCTL00_WREL_Msk;
            }
            else
            {
				if (Recv_Length < Size)
				{
					*Data = IICA->IICA0;
					Data++;
					Recv_Length++;

					/* WREL0 = 1U: cancel wait */
					IICA->IICCTL00 |= IICA_IICCTL00_WREL_Msk;
					if(Size == Recv_Length)
					{
						/* WTIM0 = 1:  interrupt request is generated at the ninth clock's falling edge */
						IICA->IICCTL00 |= IICA_IICCTL00_WTIM_Msk;
                        break;
					}
				}
				else
				{
					/* When the receive data length exceed the hope length,we shoule not write data to the receive buffer */
					/* WTIM0 = 1:  interrupt request is generated at the ninth clock's falling edge */
					IICA->IICCTL00 |= IICA_IICCTL00_WTIM_Msk;
					
					/* WREL0 = 1U: cancel wait */
					IICA->IICCTL00 |=  IICA_IICCTL00_WREL_Msk;
				}
            }
        }
    }
	
	/* Set the receive data length */
	*Len = Recv_Length;
	
	return status;
}

/**
  * @brief  Transmit a data buffer through the I2CA peripheral to device register address by slave mode.
  * @param  Data: Data buffer address.
  * @param  Size: Data size when the slave hope to recceive.
  * @retval status: This parameter can be a value of @ref I2CA_Status.
  */
I2CA_Status I2CA_Slave_TransmitData(uint8_t *Reg, uint8_t *Data, uint16_t Size)
{
	I2CA_Status status = OK;
	uint8_t Stop_count = 0;
    uint8_t Address_Match = 0;
    uint16_t Send_Length = 0;
	
    while(1)
    {
        while(!INTC_GetPendingIRQ(IICA_IRQn));
        INTC_ClearPendingIRQ(IICA_IRQn);

        /* Check stop condition */
        if (IICA->IICS0 & IICA_IICS0_SPD_Msk)
        {
			Stop_count++;
			
			/* To avoid the repeated stop condition */
			if (Stop_count == 2)
			{
				/* Disable I2CA */
				IICA->IICCTL00 &= ~IICA_IICCTL00_SPIE_Msk;
				break;
			}
        }
        else
        {
			/* When the address match is success */
            if ((Address_Match < 2) && (IICA->IICS0 & IICA_IICS0_COI_Msk))  //接收地址和自身从机地址一致
            {
                Address_Match ++;
				
				if (Address_Match == 1)
				{
					IICA->SVA0 |= 0x01;    //第一次先读		
				} 
				else if (Address_Match == 2)
				{
					*Reg = IICA->IICA0;
					IICA->SVA0 &= ~0x01;	
				}
				
				if (!(IICA->IICCTL00 & IICA_IICCTL00_SPIE_Msk))
				{
					/* SPIE0 = 1: enable I2CA */
					IICA->IICCTL00 |= IICA_IICCTL00_SPIE_Msk;
				}
				
				/* Sending or receiving */
				if (IICA->IICS0 & IICA_IICS0_TRC_Msk)    //send status
				{
					IICA->IICCTL00 |= IICA_IICCTL00_WTIM_Msk; //在输入9个时钟，将时钟置为低电平
					IICA->IICA0 = *Data;
					Data++;
					Send_Length++;
				}
				else
				{
					IICA->IICCTL00 |=  IICA_IICCTL00_ACKE_Msk;   /* ACKE0 = 1U: enable acknowledgment */																				
					IICA->IICCTL00 &= ~IICA_IICCTL00_WTIM_Msk;    // interrupt request is generated at the eighth clock's falling edge
					IICA->IICCTL00 |= IICA_IICCTL00_WREL_Msk;     // cancel wait 
				}
            }
            else
            {
				/* Sending status */
				if (IICA->IICS0 & IICA_IICS0_TRC_Msk)
				{
					if (Send_Length == Size)
					{
						IICA->IICA0 = 0xFF;

					}
					else
					{
						/* Write data to IICA0 register to send it */
						IICA->IICA0 = *Data;
						Data++;
						Send_Length++;
					}
				}
            }
        }
	}
	
	return status;
}

/**
  * @brief  Transmit or receive the data which can be transmitted. This function only used by ISR.
  * @param  TxBuffer:   The transmit buffer pointer for application memory.
  * @param  TxLength:   The value pointer for transmit data length.
  * @param  RxBuffer:   The receive buffer pointer for application memory.
  * @param  RxLength:   The value pointer for received data length.
  * @param  RxTotLength:The total length of the receive buffer. 
  * @retval Event: This parameter can be a value of @ref I2CA_Slave_Event.
  */
I2CA_Slave_Event I2CA_Slave_TransmitReceive_IT( uint8_t *TxBuffer, uint16_t *TxLength, 
                                                uint8_t *RxBuffer, uint16_t *RxLength, uint16_t RxTotLength)
{
    I2CA_Slave_Event Event = I2C_SLAVE_EVENT_NONE;
    static uint8_t is_AddrMatch;
	static uint8_t TransmitDir;
    static uint8_t *TxData, *RxData;
    static uint16_t Length;

    /* Control for stop condition */
    if (is_AddrMatch && I2CA_GetFlagStaus(I2CA_STATUS_SPD))
    {
        is_AddrMatch = 0;
        if (TransmitDir == I2CA_DIR_TRANSMIT)
        {
            TransmitDir = 0;
            Event = I2C_SLAVE_EVENT_TRANSMIT_COMPLETED;
        }
        else if (TransmitDir == I2CA_DIR_RECEIVE)
        {
            TransmitDir = 0;
            *RxLength = Length;
            Event = I2C_SLAVE_EVENT_RECEIVE_COMPLETED;
        }
        
        /* I2CA periphal disable and ACK disable for the next transmission */
        I2CA_Acknowledge_Cmd(DISABLE);
        I2CA_Cmd(DISABLE);
    }
    else
    {
        /* Address to match and get the operate for master */
		if (!is_AddrMatch && I2CA_GetFlagStaus(I2CA_STATUS_COI))
		{
			is_AddrMatch = 1;
			
			I2CA_Cmd(ENABLE);
			
			if (I2CA_ReadByte() & 0x01)
			{
                /* Assign the TxBuffer pointer and the data length which to be transmit. */
				TxData = TxBuffer;
                Length = *TxLength;
				
                /* Config the interrupt position and write the first byte to register */
				I2CA_InterruptPosition_Config(I2CA_INTPOS_9CLK);
				I2CA_WriteByte(*TxData);
				TxData++;
				Length--;

				/* Slave transmit operate */
				TransmitDir = I2CA_DIR_TRANSMIT;
                Event = I2C_SLAVE_EVENT_READ_ADDRESS_MATCHED;
			}
			else
			{
                /* Assign the RxBuffer pointer. */
				RxData = RxBuffer;
				Length = 0;
				
                /* Config the interrupt position and enable ACK and I2CA periphal */
				I2CA_InterruptPosition_Config(I2CA_INTPOS_8CLK);
                I2CA_Acknowledge_Cmd(ENABLE);
				I2CA_WaitRelieve_Cmd(ENABLE);

				/* Slave receive operate */
				TransmitDir = I2CA_DIR_RECEIVE;
                Event = I2C_SLAVE_EVENT_WRITE_ADDRESS_MATCHED;
			}
		}
		else if (is_AddrMatch)
		{
			if (TransmitDir == I2CA_DIR_TRANSMIT)
			{
                /* The transmission direction is transmit. */
				if (Length > 0U)
				{
                    /* Write send data one by one byte to the register. */
					I2CA_WriteByte(*TxData);
					TxData++;
					Length--;
				}
				else
				{
                    /* When the data transmit is completed, we shoule write 0xFF to pull-up the bus. */
                    /* To wait the stop condition */
					I2CA_WriteByte(0xFF);
				}
                Event = I2C_SLAVE_EVENT_TRANSMIT_RUNNING;
			}
			else if (TransmitDir == I2CA_DIR_RECEIVE)
			{
                /* The transmission direction is receive. */
				I2CA_WaitRelieve_Cmd(ENABLE);
				if (Length < RxTotLength)
				{
					*RxData = I2CA_ReadByte();
					RxData++;
					Length++;
				}
				else
				{
                    /* Receive data operate finished and config the interrupt position to wait the stop condition. */
					I2CA_InterruptPosition_Config(I2CA_INTPOS_9CLK);
				}
                Event = I2C_SLAVE_EVENT_RECEIVE_RUNNING;
			}
		}
		else
		{
			I2CA_WaitRelieve_Cmd(ENABLE);
            Event = I2C_SLAVE_EVENT_ERROR;
		}
    }

    return Event;
}
