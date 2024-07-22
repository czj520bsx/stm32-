#ifndef __LED_H
#define __LED_H	 
#include "sys.h"


typedef struct
{

	_Bool Led_Status;

} LED_INFO;

#define LED_ON		1

#define LED_OFF	0

extern LED_INFO led_info;


void LED_Init(void);//≥ı ºªØ

//void LED_Set(_Bool status); 				    

void Led_Set(_Bool status);

#endif
