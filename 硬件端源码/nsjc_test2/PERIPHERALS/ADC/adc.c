#include "adc.h"
#include "delay.h"
#include "stm32f10x_dma.h"


__IO uint32_t ADC_ConvertedValue[2] = {0,0};	   
		   
//��ʼ��ADC
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ͨ��0~3		


static void ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1,ENABLE );	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//PA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);////��ʼ�� ADC1 IO

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC2,ENABLE );	  //ʹ��ADC2ͨ��ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PC0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);////��ʼ�� ADC2 IO
	
}


void  Adc_Init(void)
{ 	
	
	
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
	
	//-------------------��ʼ��DMA-----------------------------------/
	DMA_DeInit(DMA1_Channel1);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(& (ADC1->DR));  //DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t)ADC_ConvertedValue;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݴ��䷽�򣬴������ȡ���͵��ڴ�
	DMA_InitStructure.DMA_BufferSize = 1;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;  //����Ϊ����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Word; //���ݰ���
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //�����ݾʹ���
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ�� xӵ�и����ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
	DMA_Cmd(DMA1_Channel1,ENABLE);//ʹ��
	
	
	/*----------------------------------ADC1 ����----------------------------------------------*/
	ADC_InitTypeDef ADC_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1,ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	//ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
	

	ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;	//ADC����ģʽ:ADC1��ADC2������ͬ���ɼ�ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ADC1�����������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);//adcת��ʱ��21us 
	
	ADC_DMACmd(ADC1,ENABLE);//ʹ��ADC  DMA����
	/*----------------------------------------------------------------------------------------*/
	

	/*------------------------------------ADC2 ����---------------------------------------*/

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC2,ENABLE );	  //ʹ��ADC2ͨ��ʱ��

	
	//ADC_DeInit(ADC2);  //��λADC2,������ ADC2 ��ȫ���Ĵ�������Ϊȱʡֵ
	

	ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;	//ADC����ģʽ:ADC1��ADC2������ͬ���ɼ�ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;	//ADC2���ö�ʱ����TRGOģʽ����
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC2, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

	ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);//adcת��ʱ��21us 
	
	/*------------------------------------------------------------------------------------------*/
	
	ADC_GPIO_Config();//���ų�ʼ��
	
	/*-----------------ADC1��λУ׼----------------------------*/
	ADC_Cmd(ADC1,ENABLE);//����ADC1,����ʼת��
	ADC_ResetCalibration(ADC1);	//��ʼ��ADCУ׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���ʼ�����
	ADC_StartCalibration(ADC1);	 //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
	/*--------------------------------------------------------*/
	
	
	
	/*-----------------ADC2��λУ׼----------------------------*/
	ADC_Cmd(ADC2,ENABLE);//����ADC2,����ʼת��
	ADC_ResetCalibration(ADC2);	////��ʼ��ADCУ׼�Ĵ��� 
	while(ADC_GetResetCalibrationStatus(ADC2));	//�ȴ���ʼ�����
	ADC_StartCalibration(ADC2);	 //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC2));	 //�ȴ�У׼����
	/*--------------------------------------------------------*/
	
	
	ADC_ExternalTrigConvCmd(ADC2,ENABLE);//ADC2�����ⲿ����
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//ADC1�����������

}				  
//���ADC1ֵ
u16 Get_Adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
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



























