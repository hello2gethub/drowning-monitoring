#ifndef __FSR_H
#define __FSR_H	 
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////   	 


//#define KEY0 PEin(4)   	//PE4
//#define KEY1 PEin(3)	//PE3 
//#define KEY2 PEin(2)	//PE2
//#define KEY3 PAin(0)	//PA0  WK_UP

#define FSR_GPIO  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15)//��ȡ



#define KEY_PRESS		1


void FSR_IO_Init(void);//IO��ʼ��
u8 FSR_Scan(u8);  	//����ɨ�躯��					    
#endif
