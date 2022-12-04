#include "beep.h"
#include "stm32f10x.h"

void Beep_config(void)//蜂鸣器配置函数  低电平开启蜂鸣器
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能GPIOB的时钟信号
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//上电默认关闭
}









