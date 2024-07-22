#include "JW.h"
#include "stdbool.h"
#include "usart.h"

#define    JW_ADDR                        0x2C
#define    JW_DATA_LEN                    0x06
uint16_t CO2_max = 75;
int CO2_flag =1 ;
int alarm_CO2=1 ;
unsigned char Jw_Data_Start    =      false;
unsigned char JW_Pos            =        0;
unsigned int JW_Value          =        0;
unsigned char JW_Data_Buffer[SERIAL_DATA_LEN];
unsigned char Check_Jw_Data;

unsigned int JW_Timer_Value = 0;

void JW_Deal_Timer_Count(void)
{
    if(JW_Timer_Value)
        JW_Timer_Value--;
}

void JW_Set_Timer_Value(unsigned int value)
{
   JW_Timer_Value = value;
}

unsigned char JW_Get_Timer(void)
{
    if(JW_Timer_Value)
      return 1;
    else
      return 0;
}
unsigned char  Serial_PopRece(unsigned char* c)
{
  if(Serial_Head == Serial_Tead)
  {
      return 0;
  }
  else
  {
      *c = Serial_Data_Buff[ Serial_Tead];
      Serial_Tead = ( Serial_Tead +1)% SERIAL_DATA_LEN;
      return 1;
  }
}


void JW_Data_Analyze(void)
{
    unsigned char Temp = 0;
    unsigned char i;
    
    for(i = 0;i < 5; i++)
    {
        Temp += JW_Data_Buffer[i];
    }
    if(Temp != JW_Data_Buffer[5])
    {
//      UsartPrintf(USART_DEBUG,"JW_Receive_Data_Error: %02X,%02X,%02X,%02X%,%02X,%02X  Temp:%02X..\r\n",JW_Data_Buffer[0],JW_Data_Buffer[1],JW_Data_Buffer[2],JW_Data_Buffer[3],JW_Data_Buffer[4],JW_Data_Buffer[5],Temp);
        return;
    }
    if(CO2_flag==1)
    {
      JW_Value = JW_Data_Buffer[1]*256 + JW_Data_Buffer[2];
      JW_Value /=10;
    }else JW_Value=0;
    
//    UsartPrintf(USART_DEBUG,"JW_Receive_Data_success:%02X,%02X,%02X,%02X%,%02X,%02X  Temp:%02X..\r\n",JW_Data_Buffer[0],JW_Data_Buffer[1],JW_Data_Buffer[2],JW_Data_Buffer[3],JW_Data_Buffer[4],JW_Data_Buffer[5],Temp);

//    UsartPrintf(USART_DEBUG,"JW_Receive_Data_Value:%d ppm\r\n",JW_Value);
    
    
}
void JW_Data_Receive(void)
{
    unsigned char c;
    
    while(Serial_PopRece(&c))
    {
        JW_Data_Buffer[JW_Pos]=c;
        JW_Pos=(JW_Pos+1)  % SERIAL_DATA_LEN;
         Check_Jw_Data  = 10;
       // UsartPrintf(USART_DEBUG,"%02X\r\n",Serial_Data_Buff[Serial_Head]);
          if(Jw_Data_Start == true)
          {
            if(JW_Pos == JW_DATA_LEN )
            {
                JW_Pos = 0;
                JW_Data_Analyze();
                Jw_Data_Start = false;
                return;
            }
            if(JW_Pos  > JW_DATA_LEN )
            {
                JW_Pos = 0;
                Jw_Data_Start = false;
            }
            continue;
          }
        if((c == JW_ADDR )&&  (!Jw_Data_Start))
        {
            JW_Data_Buffer[0] = JW_ADDR;
           Jw_Data_Start = true;
            JW_Pos = 1;
        }
        
    }
}

void JW_Pross(void)
{
  if(JW_Get_Timer())
    return;
  
  JW_Set_Timer_Value(10);
  
  if(Check_Jw_Data)
  {
    Check_Jw_Data--;
    if(Check_Jw_Data  == 0)
    {
      Jw_Data_Start = false;
      JW_Pos = 0;
    }
  }
  
}

