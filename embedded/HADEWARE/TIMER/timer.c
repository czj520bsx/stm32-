#include "timer.h"
#include "adc.h"
#include "led.h"
#include "beep.h"
#include "JW.h"
#include "usart.h"
int Sec = 90;//倒计时参数
int Waring_Time ;//DAC输出的值。
unsigned int System_Flag,System_Time;//系统标志位，系统运行时间
int flag=1;
unsigned int Sys_Tick_Timer;



void TIM4_Init(void)//0.1ms定时当作整个系统的呼吸
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStrue;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	TIM_InternalClockConfig(TIM4);
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//指定时钟频率
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period = 7200-1;	   //ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1-1; //PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);//开启定时器中断
	
	NVIC_InitStrue.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStrue);//初始化中断，设置中断的优先级
	
	TIM_Cmd(TIM4,ENABLE);//使能定时器
}

void TIM4_IRQHandler(void)//系统逻辑执行中断
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
	{
		if(++System_Time % 10000 == 0 && System_Flag <= 90){System_Flag++;Sec--;}//一氧化碳传感器上电需要预热一分半钟（5V电是1分钟）
    
		if(System_Flag== 93&&flag==1)
		{
		
      if((MQ4.CH4 >= CH4_max&&alarm_CH4==1)||(MQ7.CO >= CO_max&&alarm_CO==1)||(MQ8.H2 >= H2_max&&alarm_H2==1))//CO值大于50则有危险，所以选在40-50为预警阶段
			{
				Waring_Time++;
				if(Waring_Time<=500)
				{
					Beep_Set(BEEP_ON); //危险间隙报警

					Led_Set(LED_ON);
				}
				else if(Waring_Time<=1000)
				{

          Beep_Set(BEEP_OFF);
					Led_Set(LED_OFF);
				}
				else if(Waring_Time>1000)
					Waring_Time = 0;
			}
			else if(1)//CO浓度处于危险阶段，报警
			{
				Waring_Time = 0;
//        Beep_Set(BEEP_ON);

//				Led_Set(LED_ON);
			}
		}
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//清除中断标志位
	}
}
