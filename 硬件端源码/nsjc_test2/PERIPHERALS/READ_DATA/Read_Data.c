#include "Read_Data.h"


/*--------------------------------  MPU6050  ----------------------------------------*/
struct MPU6050										//MPU6050结构体
{
	u8 flag;										//采集成功标志位
	u8 speed;										//上报速度
}mpu6050;											//唯一结构体变量


char tmp_buf[33];									//字符串数组
float pitch,roll,yaw; 								//欧拉角:俯仰角，横滚角，航向角
float fy,hg,hx;
short aacx,aacy,aacz;								//加速度传感器原始数据  角加速度
short gyrox,gyroy,gyroz;							//陀螺仪原始数据  角速度
short temp;	
float temperature;


void MPU_Read(void)
{
	
	if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)		//dmp处理得到数据，对返回值进行判断
	{ 
		temp=MPU_Get_Temperature();	                //得到温度值
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
		mpu6050.speed++;                            //上报速度自加
		if(mpu6050.speed == 1)						//上报速度阈值设置
		{
			mpu6050.flag = 1;						//采集成功标志位设置为有效
			mpu6050.speed = 0;						//上报速度归零
		}	
	}
	else 											//采集不成功										
	{
		mpu6050.flag = 0;							//采集成功标志位设置为无效
	}	
}

void DATA_Report(void)
{
	if(mpu6050.flag == 1)							//采集成功时
	{ 
		if(temp<0)									//对数据正负判断，判断为负时
		{
			temp=-temp;								//对负数据取反
		}
		else                                  		//判断为正时
		{
		}
		temperature = temp/100 + (float)(temp%10)/10;//温度赋值给temprory
		
		temp=pitch*10;							 	//赋temp为pitch
		if(temp<0)									//对数据正负判断，判断为负时
		{
			temp=-temp;						   		//对负数据取反		
		}
		else                                    	//判断为正时 
		{
		}
		fy = (float)(temp/10 + temp%10)/10; 		//俯仰角赋值给fy	
		
		temp=yaw*10;                          		//赋temp为yaw
		if(temp<0)									//对数据正负判断，判断为负时
		{
			temp=-temp;						    	//对负数据取反
		}
		else                                    	//判断为正时
		{
		}
		hx = (float)(temp/10 + temp%10)/10; 		//滚转角角赋值给hx
		
		temp=roll*10;                            	//赋temp为roll
		if(temp<0)									//对数据正负判断，判断为负时
		{
			temp=-temp;						    	//对负数据取反	
		}
		else                                    	//判断为正时
		{
		}
		hg = (float)(temp/10 + temp%10)/10; 		//偏航角赋值给hg
		mpu6050.flag = 0;							//采集成功标志位设置为无效
	}
	else ;											//防卡死
}



/*--------------------------------  FSR  ----------------------------------------*/
#define VOLTAGE_MIN 100								//给定已知力
#define VOLTAGE_MAX 3300

#define PRESS_MIN	0     							//最小量程 
#define PRESS_MAX	6000    						//最大量程

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
	value_AD = Get_Adc_Average(1,10);				//10次压力的平均值   50ms
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


/*------------------------------  北斗/GPS  --------------------------------------*/



