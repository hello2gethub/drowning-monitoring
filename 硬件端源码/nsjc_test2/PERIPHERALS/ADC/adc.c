#include "adc.h"
#include "delay.h"
#include "stm32f10x_dma.h"


__IO uint32_t ADC_ConvertedValue[2] = {0,0};	   
		   
//初始化ADC
//这里我们仅以规则通道为例
//我们默认将开启通道0~3		


static void ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1,ENABLE );	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//PA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);////初始化 ADC1 IO

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC2,ENABLE );	  //使能ADC2通道时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PC0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);////初始化 ADC2 IO
	
}


void  Adc_Init(void)
{ 	
	
	
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
	//-------------------初始化DMA-----------------------------------/
	DMA_DeInit(DMA1_Channel1);   //将DMA的通道1寄存器重设为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(& (ADC1->DR));  //DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t)ADC_ConvertedValue;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从外设读取发送到内存
	DMA_InitStructure.DMA_BufferSize = 1;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;  //数据为半字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Word; //数据半字
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //有数据就传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA通道 x拥有高优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器
	DMA_Cmd(DMA1_Channel1,ENABLE);//使能
	
	
	/*----------------------------------ADC1 配置----------------------------------------------*/
	ADC_InitTypeDef ADC_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1,ENABLE );	  //使能ADC1通道时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	//ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值
	

	ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;	//ADC工作模式:ADC1和ADC2工作在同步采集模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ADC1采用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);//adc转换时间21us 
	
	ADC_DMACmd(ADC1,ENABLE);//使能ADC  DMA请求
	/*----------------------------------------------------------------------------------------*/
	

	/*------------------------------------ADC2 配置---------------------------------------*/

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC2,ENABLE );	  //使能ADC2通道时钟

	
	//ADC_DeInit(ADC2);  //复位ADC2,将外设 ADC2 的全部寄存器重设为缺省值
	

	ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;	//ADC工作模式:ADC1和ADC2工作在同步采集模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;	//ADC2采用定时器三TRGO模式触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC2, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

	ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);//adc转换时间21us 
	
	/*------------------------------------------------------------------------------------------*/
	
	ADC_GPIO_Config();//引脚初始化
	
	/*-----------------ADC1复位校准----------------------------*/
	ADC_Cmd(ADC1,ENABLE);//开启ADC1,并开始转换
	ADC_ResetCalibration(ADC1);	//初始化ADC校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待初始化完成
	ADC_StartCalibration(ADC1);	 //开启AD校准
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
	/*--------------------------------------------------------*/
	
	
	
	/*-----------------ADC2复位校准----------------------------*/
	ADC_Cmd(ADC2,ENABLE);//开启ADC2,并开始转换
	ADC_ResetCalibration(ADC2);	////初始化ADC校准寄存器 
	while(ADC_GetResetCalibrationStatus(ADC2));	//等待初始化完成
	ADC_StartCalibration(ADC2);	 //开启AD校准
	while(ADC_GetCalibrationStatus(ADC2));	 //等待校准结束
	/*--------------------------------------------------------*/
	
	
	ADC_ExternalTrigConvCmd(ADC2,ENABLE);//ADC2采用外部触发
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//ADC1采用软件触发

}				  
//获得ADC1值
u16 Get_Adc(u8 ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 	 



























