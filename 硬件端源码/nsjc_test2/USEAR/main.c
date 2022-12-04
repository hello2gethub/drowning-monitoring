/**
  ************************************************************************************************
  * @file    main.c
  * @author  �����
  * @version V1.0
  * @date    2021-11-7
  * @brief   ������ˮ���ϵͳ
  *************************************************************************************************
  * Ӳ��ƽ̨:LORA��վ + STM32F103ZET6ϵͳ�� + LORAģ�� + MPU6050ģ�� + PluSensorģ�� + FSRģ�� + GPS/����ģ��
  * ����QQ   :1599077792
  * �����������ޣ���������������©������ӭ���ָ����
  *************************************************************************************************
  */ 

/*----------------------ͷ�ļ�--------------------*/
#include "Read_Data.h"

/*----------------	����ӿ� ---------------------*/

/*----------------	 FSR  ------------------------*
FSR--AO�ӿ�
VCC  ->     5V
GND ->      GND
AO ->       PA1

FSR--DO�ӿ�
VCC  ->     5V
GND ->      GND
DO ->     	PC15

*----------------	Plusenor ---------------------*
S   ->      PC0
+	->		3V3
-	->		GND

*----------------	MPU6050  ---------------------*
SCL ->		PC12
SDA ->		PC11
+	->		5V5
-	->		GND

*----------------	LORA    -----------------------*

+	->		3V3
-	->		GND
TX	->		PA10
RX	->		PA9

*----------------   ����/GPS ----------------------*
PSS ->		PA4
TX	->		PA3
RX	->		PA2
GND ->		GND
VCC ->		5V 



*-------------------	 �������   -------------------------*/

/*----------------	FSR��Ĥѹ�������� ------------------*/
extern long PRESS_AO;


/*----------------	MPU6050 ------------------*/
extern float fy,hg,hx;
extern short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����  �Ǽ��ٶ�
extern short gyrox,gyroy,gyroz;		//������ԭʼ����  ���ٶ�
extern float temperature;

/*--------------------- PluseSenor���ʴ����� -------------------------*/

extern int BPM;         		 //����ֵ                        
extern unsigned char QS; 		//����������־

/*----------------	����/GPS ------------------*/
float latitude = 28.11270;
float longitude = 112.98991;


/*----------------- ��ˮ����߼� ---------------------*/
unsigned int Flag = 0;				//�Ƿ񱨾�

/*-----------�������--------------*/
bool  Flag_critical = false;	//����ˮ�³���8s�ı�������
bool  Flag_P_H = false;			//���ʳ�������5s�����ϱ�������



/*----------������������---------*/
unsigned int Count_FSR = 0;				//����������FSR�Ĵ���
unsigned int Count_Ph = 0;				//����������Plusensor����
unsigned int temporary = 120;			//��ʱ�洢BMPֵ



/*--------------------	ͨ�� ---------------------------*
����������:emqx
����������:www.nsjc.xyz
�˿ں�:1883
��������:Topic/wx
��������:Topic/stm32

*-----------------------------------------------*/


long map(long x, long in_min, long in_max, long out_min, long out_max);

int main(void)
{
	unsigned short timeCount = 0;	//���ͼ������

	delay_init();	    //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart1_init(9600); //��ʼ������1----LORA����

	Adc_Init();		//��ģת��ͨ����ʼ��
	MPU_Init();					//��ʼ��MPU6050
	TIM3_Int_Init(1999,71);//��ʱ����2ms
	
	while(mpu_dmp_init())//ŷ����DMP��ʼ����Ӳ���Դ�����̬�ں��㷨��ʼ����
 	{
		printf("MPU6050 ERROR\r\n");
		delay_ms(2000);
	} 
	printf("Peripherals Init OK\r\n");//��ʼ�����	
	LED0=0;				//LED0����ʾ��ʼ�����
	LED1=1;
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);// TIM3 ���¿�ʱ�ӣ���ʼ��ʱ 
	while(1)
	{
		
		/*---------------------- ѹ���ɼ� -------------------------*/
		DATA_Press();	//��ȡѹ������
		
		/*-------------------------MPU6050-------------------------*/
		MPU_Read();		//��ȡ����������
		DATA_Report();	//�ϱ�����λ�����
		
		/*---------------------- PluSenor ------------------------*/
		if (QS)
		{
			if(BPM > 120)	//��Ϊ��û������ʱ���ǳ���120������ֵ��ͬʱ�������ֵ���Ѵﵽ��������Ϊ0.
			{
				BPM = 0;
				QS = false;
			}
		}
		else
		{
			BPM = 0;
		}
		/*----------------------- �Ϸ��������� -----------------------*/
		if(++timeCount%20 == 0)//��Լ1sһ��
		{
			//printf("{\"Temp\":%.1lf,\"Heart\":%d,\"Ax\":%d,\"Ay\":%d,\"Az\":%d,\"Press\":%ld,\"Beep\":%d,\"name\":\"STM32001\",\"latitude\":%f,\"longtitude\":%f}",temperature,BPM,aacx,aacy,aacz,PRESS_AO,flag,latitude,longtitude);
			//printf("Press = %ld; Heart = %d",PRESS_AO,BPM);
			printf("{\"Temp\":%.1lf,\"Heart\":%d,\"Press\":%ld,\"Beep\":%d,\"name\":\"STM32-001\",\"latitude\":%f,\"longitude\":%f}",temperature,BPM,PRESS_AO,Flag,latitude,longitude);
			//printf("press:%ld",PRESS_AO);
			timeCount = 0;
		}
		

		
		/*---------------------��ˮ����߼�-------------------------*/
		if(timeCount%20 == 0)	//1s
		{
			/*------- ���ʼ�ʱ --------*/
			if(temporary >= BPM && !BPM)			//�����ǰһ�����ʽ��ͻ򲻱�
			{
				Count_Ph++;
				temporary = BPM;
			}
			else
			{
				temporary = 120;
				Count_Ph = 0;
			}
			
			/*------- ѹ����ʱ --------*/
			if(PRESS_AO == 0)
				Count_FSR = 0;					 //���¼���
			if(++Count_FSR >= 8)				// >=8һֱ��ˮ��
			{
				Flag_critical = true;			//��ˮ��������ˮ��������
				Count_FSR = 0;
			}
			else if(Count_FSR >=5 && Count_Ph>=5) // >=5s��ˮ�²���������������
			{
				Flag_P_H = true;
			}
		}
		if(Flag_critical | Flag_P_H)
			Flag = 1;
		
		if(Flag)
		{
			
			LED0 = !LED0;
		}
	}
}
