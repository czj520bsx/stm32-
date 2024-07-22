#ifndef _JW_H_
#define _JW_H_
#include "stm32f10x.h"

extern unsigned int JW_Value;
extern uint16_t CO2_max;
extern int CO2_flag;
extern int alarm_CO2;

extern void JW_Pross(void);
extern void JW_Data_Receive(void);
extern void JW_Deal_Timer_Count(void);
#endif
