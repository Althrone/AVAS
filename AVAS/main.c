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
    /*�ڲ�����ϵͳʱ����Ϊcpu����ʱ���Լ���Χ��·ʱ��*/
    //��ѡ���ֽ�����48MHz�ڲ�ʱ��
    //system_BAT32A237.c user_opt_data

    //��ʼ��CAN
    CAN_Port_Config();

    CAN_RecvMsg.Id = 0x231;
	CAN_RecvMsg.IDE = CAN_Id_Standard;
	CAN_RecvMsg.CacheType = CAN_CacheType_Rx_Mask1;
	CAN_RecvMsg.RTR = CAN_RTR_Data;
	CAN_RecvMsg.Interrupt = ENABLE;
	CAN_MessageCache_DeInit(CAN0MSG07);
	CAN_MessageCache_Init(CAN0MSG07, &CAN_RecvMsg);

    //��ʼ��IIC ���ڹ�������

    //��ʼ��DAC
    Dac_Init();

    //��ʼ��tim4��Ϊ��Ƶ���ͼ�ʱ��

    Tim40_IntervalTimer();//48k

    uint32_t flag=g_inttm00Taken;
    while(1)
    {
        //can���ճ���
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
                //��������ֵ�ǲ��ǵ���80
                if(yy_8b_48k[i]==0x80)
                {
                    //�л�
                    switch_flag=0;
                }
            }

            if(flag!=g_inttm00Taken)//������
            {
                //PORT->P7 ^= (1<<1); 	// Toggle P71
                //PORT->P7 ^= (1<<2); 	// Toggle P72
                //���ŷ�ת���Ƶ��
                //DAC_Set_Value(1,(sin_data[i]+127));
                //1ʵ�ʾ���ch0
                DAC_Set_Value(1,yy1_8_44100[i]);//sin_data yy1_8_44100
                DAC_Start(1);
                i++;

                flag=g_inttm00Taken;
            }
        }
    }

}



