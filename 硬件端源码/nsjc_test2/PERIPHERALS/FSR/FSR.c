#include "stm32f10x.h"
#include "FSR.h"
#include "sys.h" 
#include "delay.h"

								    
//按键初始化函数
void FSR_IO_Init(void) //IO初始化
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PC端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 // 端口配置 PC15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 //默认下拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC
	
	GPIO_SetBits(GPIOC,GPIO_Pin_15);					//初始化设置为0

}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下

u8 FSR_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&FSR_GPIO==0)
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(FSR_GPIO==0)return KEY_PRESS;
	}else if(FSR_GPIO==1)key_up=1; 	    
 	return 0;// 无按键按下
}
