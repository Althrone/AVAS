/***********************************************************************************************************************
* Copyright (C) . All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : main.c
* Version      :
* Device(s)    : BAT32A237
* Tool-Chain   : MDK(armcc)
* Description  : This file is a template.
* Creation Date: 2022/5/30
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro Definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include <stdio.h>
// #include "uart_demo.h"
// #include "rtc_demo.h"
#include "cgc.h"
#include "can.h"
#include "can_demo.h"
#include "gpio.h"
#include "pcbz.h"
// #include "tim_demo.h"
#include "dac_demo.h"
#include "tim4.h"
#include "tim4_demo.h"
/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/

CanTxRxMsg Can_Msg;
uint8_t speed=0;
uint8_t old_speed=0;
uint8_t switch_flag=0;
int main()
{
    //-----------------------------------------------------------------------
    // clock setting
    //-----------------------------------------------------------------------
    /*内部高速系统时钟作为cpu运行时钟以及外围电路时钟*/
    //在选项字节配置48MHz内部时钟
    //system_BAT32A237.c user_opt_data

    //初始化CAN
    CAN_Port_Config();

    CAN_RecvMsg.Id = 0x231;
	CAN_RecvMsg.IDE = CAN_Id_Standard;
	CAN_RecvMsg.CacheType = CAN_CacheType_Rx_Mask1;
	CAN_RecvMsg.RTR = CAN_RTR_Data;
	CAN_RecvMsg.Interrupt = ENABLE;
	CAN_MessageCache_DeInit(CAN0MSG07);
	CAN_MessageCache_Init(CAN0MSG07, &CAN_RecvMsg);

    //初始化IIC 用于功放配置

    //初始化DAC
    Dac_Init();

    //初始化tim4作为音频发送计时器

    Tim40_IntervalTimer();//48k

    uint32_t flag=g_inttm00Taken;
    while(1)
    {
        //can接收车速
        if(Read_RingBuff(&Can_RxMsg))
        {
            speed=Can_RxMsg.Data[0];
            if(old_speed!=speed)
            {
                switch_flag=1;
                old_speed=speed;
            }
        }

        for (size_t i = 0; i < yy1_8_44100_size; )//441 yy1_8_44100_size
        {
            if(switch_flag==1)
            {
                //看看输入值是不是等于80
                if(yy_8b_48k[i]==0x80)
                {
                    //切换
                    switch_flag=0;
                }
            }

            if(flag!=g_inttm00Taken)//更新了
            {
                //PORT->P7 ^= (1<<1); 	// Toggle P71
                //PORT->P7 ^= (1<<2); 	// Toggle P72
                //引脚反转测测频率
                //DAC_Set_Value(1,(sin_data[i]+127));
                //1实际就是ch0
                DAC_Set_Value(1,yy1_8_44100[i]);//sin_data yy1_8_44100
                DAC_Start(1);
                i++;

                flag=g_inttm00Taken;
            }
        }
    }

}



