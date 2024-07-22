#ifndef __BEEP_H
#define __BEEP_H



typedef struct
{
  _Bool Beep_Status;


}BEEP_INFO;

#define BEEP_ON		1

#define BEEP_OFF	0

extern BEEP_INFO beep_info;

void Beep_Init(void);   /* ³õÊ¼»¯·äÃùÆ÷ */
void Beep_Set(_Bool status);


#endif

















