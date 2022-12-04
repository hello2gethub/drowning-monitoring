#include "Read_Data.h"


/*--------------------------------  MPU6050  ----------------------------------------*/
struct MPU6050										//MPU6050�ṹ��
{
	u8 flag;										//�ɼ��ɹ���־λ
	u8 speed;										//�ϱ��ٶ�
}mpu6050;											//Ψһ�ṹ�����


char tmp_buf[33];									//�ַ�������
float pitch,roll,yaw; 								//ŷ����:�����ǣ�����ǣ������
float fy,hg,hx;
short aacx,aacy,aacz;								//���ٶȴ�����ԭʼ����  �Ǽ��ٶ�
short gyrox,gyroy,gyroz;							//������ԭʼ����  ���ٶ�
short temp;	
float temperature;


void MPU_Read(void)
{
	
	if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)		//dmp����õ����ݣ��Է���ֵ�����ж�
	{ 
		temp=MPU_Get_Temperature();	                //�õ��¶�ֵ
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
		mpu6050.speed++;                            //�ϱ��ٶ��Լ�
		if(mpu6050.speed == 1)						//�ϱ��ٶ���ֵ����
		{
			mpu6050.flag = 1;						//�ɼ��ɹ���־λ����Ϊ��Ч
			mpu6050.speed = 0;						//�ϱ��ٶȹ���
		}	
	}
	else 											//�ɼ����ɹ�										
	{
		mpu6050.flag = 0;							//�ɼ��ɹ���־λ����Ϊ��Ч
	}	
}

void DATA_Report(void)
{
	if(mpu6050.flag == 1)							//�ɼ��ɹ�ʱ
	{ 
		if(temp<0)									//�����������жϣ��ж�Ϊ��ʱ
		{
			temp=-temp;								//�Ը�����ȡ��
		}
		else                                  		//�ж�Ϊ��ʱ
		{
		}
		temperature = temp/100 + (float)(temp%10)/10;//�¶ȸ�ֵ��temprory
		
		temp=pitch*10;							 	//��tempΪpitch
		if(temp<0)									//�����������жϣ��ж�Ϊ��ʱ
		{
			temp=-temp;						   		//�Ը�����ȡ��		
		}
		else                                    	//�ж�Ϊ��ʱ 
		{
		}
		fy = (float)(temp/10 + temp%10)/10; 		//�����Ǹ�ֵ��fy	
		
		temp=yaw*10;                          		//��tempΪyaw
		if(temp<0)									//�����������жϣ��ж�Ϊ��ʱ
		{
			temp=-temp;						    	//�Ը�����ȡ��
		}
		else                                    	//�ж�Ϊ��ʱ
		{
		}
		hx = (float)(temp/10 + temp%10)/10; 		//��ת�ǽǸ�ֵ��hx
		
		temp=roll*10;                            	//��tempΪroll
		if(temp<0)									//�����������жϣ��ж�Ϊ��ʱ
		{
			temp=-temp;						    	//�Ը�����ȡ��	
		}
		else                                    	//�ж�Ϊ��ʱ
		{
		}
		hg = (float)(temp/10 + temp%10)/10; 		//ƫ���Ǹ�ֵ��hg
		mpu6050.flag = 0;							//�ɼ��ɹ���־λ����Ϊ��Ч
	}
	else ;											//������
}



/*--------------------------------  FSR  ----------------------------------------*/
#define VOLTAGE_MIN 100								//������֪��
#define VOLTAGE_MAX 3300

#define PRESS_MIN	0     							//��С���� 
#define PRESS_MAX	6000    						//�������

#define PRESS_LIMIT 10
u8 state = 0;
u16 val = 0;
u16 value_AD;
long PRESS_AO;
int VOLTAGE_AO;


long map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void DATA_Press(void)
{
	value_AD = Get_Adc_Average(1,10);				//10��ѹ����ƽ��ֵ   50ms
	VOLTAGE_AO = map(value_AD, 0, 4095, 0, 3300);

	if(VOLTAGE_AO < VOLTAGE_MIN)
	{
		PRESS_AO = 0;
	}
	else if(VOLTAGE_AO > VOLTAGE_MAX)
	{
		PRESS_AO = PRESS_MAX;
	}
	else
	{
		PRESS_AO = map(VOLTAGE_AO, VOLTAGE_MIN, VOLTAGE_MAX, PRESS_MIN, PRESS_MAX);
	}
}


/*------------------------------  ����/GPS  --------------------------------------*/



