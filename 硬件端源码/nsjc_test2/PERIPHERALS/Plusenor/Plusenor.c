#include "Plusenor.h"
#include "led.h"
#include "usart.h"
#include "adc.h"

// these variables are volatile because they are used during the interrupt service routine!
int BPM;                   			 // used to hold the pulse rate
int Signal;               			 // holds the incoming raw data
int IBI = 600;            			 // holds the time between beats, must be seeded! 
unsigned char Pulse = false;     // true when pulse wave is high, false when it's low
unsigned char QS = false;        // becomes true when Arduoino finds a beat.
int rate[10];                    // array to hold last ten IBI values
unsigned long sampleCounter = 0; // used to determine pulse timing
unsigned long lastBeatTime = 0;  // used to find IBI
int P =512;                      // used to find peak in pulse wave, seeded
int T = 512;                     // used to find trough in pulse wave, seeded
int thresh = 512;                // used to find instant moment of heart beat, seeded
int amp = 100;                   // used to hold amplitude of pulse waveform, seeded
unsigned char firstBeat = true;  // used to seed rate array so we startup with reasonable BPM
unsigned char secondBeat = false;// used to seed rate array so we startup with reaso

/*
 * 函数名：TIM3_Int_Init
 * 描述  ：配置TIM3
 * 输入  ：arr, psc
 * 输出  ：无
 * 调用  ：外部调用
 */
extern __IO uint16_t ADC_ConvertedValue[2];

void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE); 
	 
  TIM_TimeBaseStructure.TIM_Period = arr;      
  TIM_TimeBaseStructure.TIM_Prescaler =psc;	    
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	
  TIM_TimeBaseStructure.TIM_CounterMode =TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_Update);
	/*选择update event作为TRGO,利用TIM3触发ADC通道 */
	//每个定时周期结束后触发一次
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
  TIM_Cmd(TIM3, ENABLE);                 
	
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , DISABLE); 
	/*先关闭等待使用*/ 
}

//定时器3中断服务函数
 /*该源码由开源硬件提供*/
void TIM3_IRQHandler(void)
{
	uint16_t runningTotal=0;
	uint8_t i;
	uint16_t Num;
	
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{
		uint16_t temp = 0;
		temp = ADC_GetConversionValue(ADC2);
		//读取到的值右移2位，12位-->10位
		Signal = temp/4;     // read the Pulse Senso
		//Signal=Get_Adc_Average(ADC_Channel_1,1)>>2; 
		sampleCounter += 2;                          // keep track of the time in mS with this variable
		Num = sampleCounter - lastBeatTime;          // monitor the time since the last beat to avoid noise	
		
		//发现脉冲波的波峰和波谷
		//  find the peak and trough of the pulse wave
		if(Signal < thresh && Num > (IBI/5)*3){   // avoid dichrotic noise by waiting 3/5 of last IBI
			if (Signal < T){                        // T is the trough
				T = Signal;                           // keep track of lowest point in pulse wave 
			}
		}

		if(Signal > thresh && Signal > P){        // thresh condition helps avoid noise
			P = Signal;                             // P is the peak
		}                                         // keep track of highest point in pulse wave

  //开始寻找心跳
	//当脉冲来临的时候，signal的值会上升
	//  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
  // signal surges up in value every time there is a pulse
  if (Num > 250){                                 // avoid high frequency noise
    if ( (Signal > thresh) && (Pulse == false) && (Num > (IBI/5)*3) ){        
      Pulse = true;                               // set the Pulse flag when we think there is a pulse
//      LED0=0; 
      IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS
      lastBeatTime = sampleCounter;               // keep track of time for next pulse

      if(secondBeat){                        // if this is the second beat, if secondBeat == TRUE
        secondBeat = false;                  // clear secondBeat flag
        for(i=0; i<=9; i++){                 // seed the running total to get a realisitic BPM at startup
          rate[i] = IBI;                     //rate[0]-rate[9]的值都一样吗？？？？
        }
      }

      if(firstBeat){                         // if it's the first time we found a beat, if firstBeat == TRUE
        firstBeat = false;                   // clear firstBeat flag
        secondBeat = true;                   // set the second beat flag
        return;                              // IBI value is unreliable so discard it
      }   

      // keep a running total of the last 10 IBI values
     // runningTotal = 0;                  		// clear the runningTotal variable    

      for(i=0; i<=8; i++){                    // shift data in the rate array
        rate[i] = rate[i+1];                  // and drop the oldest IBI value 
        runningTotal += rate[i];              // add up the 9 oldest IBI values
      }

      rate[9] = IBI;                          // add the latest IBI to the rate array
      runningTotal += rate[9];                // add the latest IBI to runningTotal
      runningTotal /= 10;                     // average the last 10 IBI values 
      BPM = 60000/runningTotal;               // how many beats can fit into a minute? that's BPM!
      QS = true;                              // set Quantified Self flag 
      // QS FLAG IS NOT CLEARED INSIDE THIS ISR
    }                       
  }

	//脉冲开始下降
  if (Signal < thresh && Pulse == true){   // when the values are going down, the beat is over
//    LED0=1; 															 //灯灭
    Pulse = false;                         // reset the Pulse flag so we can do it again
    amp = P - T;                           // get amplitude of the pulse wave
    thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
    P = thresh;                            // reset these for next time
    T = thresh;
  }

  //没有检测到脉冲，设置默认值
	if (Num > 2500){                         // if 2.5 seconds go by without a beat
    thresh = 512;                          // set thresh default
    P = 512;                               // set P default
    T = 512;                               // set T default
    lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date        
    firstBeat = true;                      // set these to avoid noise
    secondBeat = false;                    // when we get the heartbeat back
	QS =false;
  }

	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}
