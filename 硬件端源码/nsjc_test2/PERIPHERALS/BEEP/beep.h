#ifndef beep_h
#define beep_h

#define Beep_off() GPIO_SetBits(GPIOB,GPIO_Pin_12);
#define Beep_on() GPIO_ResetBits(GPIOB,GPIO_Pin_12);

void Beep_config(void);//·äÃùÆ÷ÅäÖÃº¯Êı
#endif
