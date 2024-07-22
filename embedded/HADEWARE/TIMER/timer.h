#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

extern int Sec,Waring_Time;//倒计时参数
extern unsigned int System_Flag,System_Time;
extern int flag;



	  
void TIM4_alarm(void);

void TIM4_Init(void);
extern unsigned int Sys_Tick_Timer;
#endif
