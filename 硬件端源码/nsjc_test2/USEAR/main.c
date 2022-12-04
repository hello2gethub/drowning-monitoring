/**
  ************************************************************************************************
  * @file    main.c
  * @author  李佳钰
  * @version V1.0
  * @date    2021-11-7
  * @brief   智能溺水监测系统
  *************************************************************************************************
  * 硬件平台:LORA基站 + STM32F103ZET6系统板 + LORA模块 + MPU6050模块 + PluSensor模块 + FSR模块 + GPS/北斗模块
  * 本人QQ   :1599077792
  * 本人能力有限，设计难免有问题和漏洞，欢迎大家指正。
  *************************************************************************************************
  */ 

/*----------------------头文件--------------------*/
#include "Read_Data.h"

/*----------------	外设接口 ---------------------*/

/*----------------	 FSR  ------------------------*
FSR--AO接口
VCC  ->     5V
GND ->      GND
AO ->       PA1

FSR--DO接口
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

*----------------   北斗/GPS ----------------------*
PSS ->		PA4
TX	->		PA3
RX	->		PA2
GND ->		GND
VCC ->		5V 



*-------------------	 外设参数   -------------------------*/

/*----------------	FSR薄膜压力传感器 ------------------*/
extern long PRESS_AO;


/*----------------	MPU6050 ------------------*/
extern float fy,hg,hx;
extern short aacx,aacy,aacz;		//加速度传感器原始数据  角加速度
extern short gyrox,gyroy,gyroz;		//陀螺仪原始数据  角速度
extern float temperature;

/*--------------------- PluseSenor心率传感器 -------------------------*/

extern int BPM;         		 //心率值                        
extern unsigned char QS; 		//发现心跳标志

/*----------------	北斗/GPS ------------------*/
float latitude = 28.11270;
float longitude = 112.98991;


/*----------------- 溺水监测逻辑 ---------------------*/
unsigned int Flag = 0;				//是否报警

/*-----------报警类别--------------*/
bool  Flag_critical = false;	//沉入水下超过8s的报警条件
bool  Flag_P_H = false;			//心率持续降低5s。符合闭气条件



/*----------报警辅助参数---------*/
unsigned int Count_FSR = 0;				//满足条件的FSR的次数
unsigned int Count_Ph = 0;				//满足条件的Plusensor计数
unsigned int temporary = 120;			//临时存储BMP值



/*--------------------	通信 ---------------------------*
服务器类型:emqx
服务器域名:www.nsjc.xyz
端口号:1883
订阅主题:Topic/wx
发布主题:Topic/stm32

*-----------------------------------------------*/


long map(long x, long in_min, long in_max, long out_min, long out_max);

int main(void)
{
	unsigned short timeCount = 0;	//发送间隔变量

	delay_init();	    //延时函数初始化	  
	LED_Init();		  	//初始化与LED连接的硬件接口
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart1_init(9600); //初始化串口1----LORA串口

	Adc_Init();		//数模转换通道初始化
	MPU_Init();					//初始化MPU6050
	TIM3_Int_Init(1999,71);//定时周期2ms
	
	while(mpu_dmp_init())//欧拉角DMP初始化（硬件自带多姿态融合算法初始化）
 	{
		printf("MPU6050 ERROR\r\n");
		delay_ms(2000);
	} 
	printf("Peripherals Init OK\r\n");//初始化完成	
	LED0=0;				//LED0亮表示初始化完成
	LED1=1;
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);// TIM3 重新开时钟，开始计时 
	while(1)
	{
		
		/*---------------------- 压力采集 -------------------------*/
		DATA_Press();	//读取压力数据
		
		/*-------------------------MPU6050-------------------------*/
		MPU_Read();		//读取陀螺仪数据
		DATA_Report();	//上报至上位机软件
		
		/*---------------------- PluSenor ------------------------*/
		if (QS)
		{
			if(BPM > 120)	//因为当没有心率时大都是超过120的心率值，同时这个心率值较难达到，所以置为0.
			{
				BPM = 0;
				QS = false;
			}
		}
		else
		{
			BPM = 0;
		}
		/*----------------------- 上发至服务器 -----------------------*/
		if(++timeCount%20 == 0)//大约1s一次
		{
			//printf("{\"Temp\":%.1lf,\"Heart\":%d,\"Ax\":%d,\"Ay\":%d,\"Az\":%d,\"Press\":%ld,\"Beep\":%d,\"name\":\"STM32001\",\"latitude\":%f,\"longtitude\":%f}",temperature,BPM,aacx,aacy,aacz,PRESS_AO,flag,latitude,longtitude);
			//printf("Press = %ld; Heart = %d",PRESS_AO,BPM);
			printf("{\"Temp\":%.1lf,\"Heart\":%d,\"Press\":%ld,\"Beep\":%d,\"name\":\"STM32-001\",\"latitude\":%f,\"longitude\":%f}",temperature,BPM,PRESS_AO,Flag,latitude,longitude);
			//printf("press:%ld",PRESS_AO);
			timeCount = 0;
		}
		

		
		/*---------------------溺水监测逻辑-------------------------*/
		if(timeCount%20 == 0)	//1s
		{
			/*------- 心率计时 --------*/
			if(temporary >= BPM && !BPM)			//相较于前一次心率降低或不变
			{
				Count_Ph++;
				temporary = BPM;
			}
			else
			{
				temporary = 120;
				Count_Ph = 0;
			}
			
			/*------- 压力计时 --------*/
			if(PRESS_AO == 0)
				Count_FSR = 0;					 //重新计数
			if(++Count_FSR >= 8)				// >=8一直在水下
			{
				Flag_critical = true;			//在水下起不来溺水条件满足
				Count_FSR = 0;
			}
			else if(Count_FSR >=5 && Count_Ph>=5) // >=5s在水下并且心率慢慢降低
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
