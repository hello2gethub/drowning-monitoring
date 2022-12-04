#include "stm32f10x.h"
#include "FSR.h"
#include "sys.h" 
#include "delay.h"

								    
//������ʼ������
void FSR_IO_Init(void) //IO��ʼ��
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PC�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 // �˿����� PC15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 //Ĭ������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC
	
	GPIO_SetBits(GPIOC,GPIO_Pin_15);					//��ʼ������Ϊ0

}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����

u8 FSR_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&FSR_GPIO==0)
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(FSR_GPIO==0)return KEY_PRESS;
	}else if(FSR_GPIO==1)key_up=1; 	    
 	return 0;// �ް�������
}
