#include "dac_demo.h"

void Dac_Init()
{
	GPIO_InitTypeDef      GPIO_InitStructure ={0};
	DAC_InitTypeDef       DAC_InitStructure ={0};
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;  //����adc ����ͨ��
	GPIO_InitStructure.GPIO_Ctrl = GPIO_Control_ANA; //ģ������
	GPIO_Init(GPIO_PORT2, &GPIO_InitStructure);//��ʼ��  
	
	DAC_InitStructure.DAC_Channel = DAC_Channel_0;
	DAC_InitStructure.DAC_Mode = DAC_Mode_Normal;
	
	DAC_Init(&DAC_InitStructure);
}
