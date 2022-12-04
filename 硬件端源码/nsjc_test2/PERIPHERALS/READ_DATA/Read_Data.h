#ifndef __READ_DATA_H
#define __READ_DATA_H	


#include "stdio.h"
#include "stdlib.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"
#include "FSR.h"
#include "led.h"
#include "stdbool.h"
#include "usart.h"   
#include "Plusenor.h"
#include "mpu6050.h"
#include "mpuiic.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

/*------------ MPU6050 --------------*/
void MPU_Read(void);
void DATA_Report(void);

/*------------   FSR ----------------*/
long map(long x, long in_min, long in_max, long out_min, long out_max);
void DATA_Press(void);


/*------------ ±±¶·/GPS -------------*/
#endif 

