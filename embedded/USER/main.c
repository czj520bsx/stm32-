#include "stm32f10x.h"
#include "delay.h"
#include "adc.h"
#include "oled.h"
#include "usart.h"
#include "math.h"
#include "timer.h"
#include "beep.h"
#include "led.h"
#include "esp8266.h"
#include "JW.h"
//网络协议层
#include "onenet.h"


void Hardware_Init(void);//硬件初始化
void Display1_Init(void);//菜单一，登陆初始化
void Display2_Init(void);//菜单二，主菜单
void Refresh_Data(void);//刷新监测值
void SNR8016_Handle(void);






#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"

int main(void)
{
  unsigned short timeCount = 0;	//发送间隔变量
	
	unsigned char *dataPtr = NULL;
  
	Hardware_Init();				//初始化外围硬件
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  Usart1_Init(9600);
  ESP8266_Init();					//初始化ESP8266
  
  Display1_Init();       //初始化界面1
  
  OneNET_Subscribe();     //订阅云服务器

	while(1)
	{
      if(Sys_Tick_Timer >= 300)
      {
        JW_Deal_Timer_Count();
        Sys_Tick_Timer = 0;
      }   
      JW_Data_Receive();
      JW_Pross();
    timeCount++;
//      System_Flag=93;
      
		if(System_Flag <= 90)
		{
			OLED_ShowNum(80,5,Sec,2,16);//电压值
		}
		else
		{
			if(System_Flag == 91)
			{
				OLED_Clear(0);
        Uart3_SU03T_SendCMD2(0x04);

				System_Flag = 92;
			}
			else if(System_Flag == 92)
			{
			 Display2_Init();//主系统菜单显示
			
			}
			else if(System_Flag == 93)
			{
        
//      	UsartPrintf(USART_DEBUG, "CH4:%d\r\n",MQ4.CH4);delay_ms(10);
//        UsartPrintf(USART_DEBUG, "发送时间间隔time:%d\r\n",timeCount);delay_ms(500);
        if(timeCount >= 50)								//发送间隔2s
          {
//            UsartPrintf(USART_DEBUG, "发送消息");delay_ms(10);
             OneNet_SendData();									//发送数据		
             timeCount = 0;
             ESP8266_Clear();
             UsartPrintf(USART_DEBUG, "完成发送");delay_ms(10);
           }
         dataPtr = ESP8266_GetIPD(0);
          if(dataPtr != NULL)
          OneNet_RevPro(dataPtr);//  ok
          
           Refresh_Data();//在主菜单持续显示测量值
           SNR8016_Handle();
//          UsartPrintf(USART_DEBUG, "OneNet_SendData\r\n");delay_ms(500);
          delay_ms(5);
			}
		}
	}
}

 void Hardware_Init(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断控制器分组设置

  TIM4_Init();
  delay_init();
  
	Usart1_Init(9600);							//串口1，打印信息用以及初始化CO2传感器
	
	Usart2_Init(115200);							//串口2，驱动ESP8266用
	
  Usart3_Init(9600);
  initialize_MQ4(&MQ4, MQ4_ADC_Value,CH4);
  
  initialize_MQ8(&MQ8, MQ8_ADC_Value,H2);
  
  initialize_MQ7(&MQ7, MQ7_ADC_Value,CO);
  
	LED_Init();

		Beep_Init();
		Adc_Init();		  		//ADC初始化
  
		OLED_Init();
       OLED_ShowString(0,0,"Hardware init ...",16); 
     delay_ms(5000);
		OLED_Clear(0); 
    Uart3_SU03T_SendCMD2(0x01);//播报硬件初始化成功
    OLED_ShowString(0,0,"Hardware init OK",16); 
    delay_ms(100);
		
//	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	
}
void Display1_Init(void)
{
  Uart3_SU03T_SendCMD2(0x02);//播报正在一件配网
	OLED_Clear(0); OLED_ShowString(0,0,"Connect MQTTs Server...",16);
	while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
		delay_ms(500);
//	UsartPrintf(USART_DEBUG, "Connect MQTT Server Success\r\n");
	OLED_ShowString(0,4,"Connect MQTT Server Success",16); delay_ms(5000);

	OLED_Clear(0); OLED_ShowString(0,0,"Device login ...",16);delay_ms(5000);
    Uart3_SU03T_SendCMD2(0x03);//播报配网成功，欢迎使用本系统
    OLED_Clear(0); OLED_ShowString(0,0,"Device Success",16);delay_ms(5000);
	while(OneNet_DevLink())			//接入OneNET
	{
		ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT");
		delay_ms(500);
	}
		
  
  OLED_Clear(0);
  delay_ms(2000);
	OLED_ShowCHinese(16-8,2,0);//传
	OLED_ShowCHinese(32-8,2,1);//感
	OLED_ShowCHinese(48-8,2,2);//器
	OLED_ShowCHinese(64-8,2,3);//正
	OLED_ShowCHinese(80-8,2,4);//在
	OLED_ShowCHinese(96-8,2,5);//预
	OLED_ShowCHinese(112-8,2,6);//热
	OLED_ShowCHinese(16,5,7);//请
	OLED_ShowCHinese(32,5,8);//等
	OLED_ShowCHinese(48,5,9);//待
	OLED_ShowCHinese(64,5,10);//：
	OLED_ShowCHinese(96,5,11);//秒
  
}
void Display2_Init(void)
{
  	//第一行
				OLED_ShowCHinese(0,0,12);//气
				OLED_ShowCHinese(16,0,13);//体
				OLED_ShowCHinese(32,0,14);//监
				OLED_ShowCHinese(48,0,15);//测
				OLED_ShowCHinese(64,0,16);//控
				OLED_ShowCHinese(80,0,17);//制
				OLED_ShowCHinese(96,0,18);//系
			  OLED_ShowCHinese(112,0,19);//统
					//第二行
				
	      OLED_ShowCHinese(0,2,20);//甲
		  	OLED_ShowCHinese(16,2,21);//烷
				OLED_ShowChar(32,2,':',16);
        
        if(CH4_flag==1)
        {
          MQ4.ADC_Value = Get_Adc_Average(ADC_Channel_0,10);//甲烷传感器ADC的值（模拟量）
          MQ4_PPM_Calibration();
          MQ4.CH4 = MQ4_GetPPM();//甲烷的浓度ppm的值
        }else MQ4.CH4=0;
				
			  
				OLED_ShowNum(40,2,MQ4.CH4,3,16);//甲烷浓度
				
//        UsartPrintf(USART_DEBUG, " CH4: %d，MQ4.CH4： %d\r\n",CH4,MQ4.CH4);
        
				OLED_ShowCHinese(64,2,23);//氢
				OLED_ShowCHinese(80,2,24);//气
				OLED_ShowChar(96,2,':',16);
				
				MQ8.ADC_Value = Get_Adc_Average(ADC_Channel_1,10);//氢气传感器ADC的值（模拟量）
				MQ8_PPM_Calibration();
				MQ8.H2 = MQ8_GetPPM();//氢气的浓度ppm的值
				OLED_ShowNum(104,2,MQ8.H2,3,16);//氢气浓度
						
					//第三行
				OLED_ShowCHinese(0,4,26);//二
				OLED_ShowCHinese(16,4,27);//氧
				OLED_ShowCHinese(32,4,28);//化
				OLED_ShowCHinese(48,4,29);//碳
				OLED_ShowCHinese(64,4,30);//：
				OLED_ShowNum(72,4,JW_Value,3,16);//电压值
				
		
				
				OLED_ShowString(96,5,"PPM",3);
				//第四行
				OLED_ShowCHinese(0,6,31);//一
				OLED_ShowCHinese(16,6,32);//氧
				OLED_ShowCHinese(32,6,33);//化
				OLED_ShowCHinese(48,6,34);//碳
				OLED_ShowCHinese(64,6,35);//：
				
        if(CO_flag==1)
        {
         	MQ7.ADC_Value = Get_Adc_Average(ADC_Channel_4,10);//烟雾传感器ADC的值（模拟量）
          MQ7_PPM_Calibration();
          MQ7.CO= MQ7_GetPPM();//一氧化碳的浓度ppm的值
        }else MQ7.CO=0;
				OLED_ShowNum(72,6,MQ7.CO,3,16);//一氧化碳浓度

      

				OLED_ShowString(96,7,"PPM",3);
				
				
				
//				OLED_ShowNum(48,6,ADC_Value,4,16);//电压值（调试用）
				System_Flag = 93;
  
}

// 在全局变量定义处定义一个计时器变量
uint32_t voice_timer = 49;

void Refresh_Data(void)
{
    if(CH4_flag==1)
    {
       MQ4.ADC_Value =  Get_Adc_Average(ADC_Channel_0,10);
       MQ4.CH4 = MQ4_GetPPM();
    }
    else MQ4.CH4=0;
      
    if(CO_flag==1)
    {
       MQ7.ADC_Value =  Get_Adc_Average(ADC_Channel_4,10);
       MQ7.CO = MQ7_GetPPM();
    }
    else MQ7.CO=0;
    
    if(H2_flag==1)
    {
        MQ8.ADC_Value =  Get_Adc_Average(ADC_Channel_1,10);
        MQ8.H2 = MQ8_GetPPM();
    }
    else MQ8.H2=0;
    
    OLED_ShowNum(40,2,MQ4.CH4,3,16);
    OLED_ShowNum(104,2,MQ8.H2,3,16);
    OLED_ShowNum(72,4,JW_Value,3,16);
    OLED_ShowNum(72,6,MQ7.CO,3,16);
    
    // 计时器递增
    voice_timer++;
  
    // 每20秒触发一次语音播报
    if(voice_timer == 60)
    {
      

       Uart3_SU03T_alarm();
        voice_timer = 0;
    }
}
void SNR8016_Handle(void)
{
  if(USART3_RX_STA == 1)
   {
      switch(USART3_RX_CMD)
      {
        //关闭警报
        case 0x01:
           flag=0;
           Beep_Set(BEEP_OFF);
           Led_Set(LED_OFF);
          break ;
        //打开报警功能
        case 0x02:
          flag=1;
           break;
          //更改甲烷阈值 
        case 0x03:  
          CH4_max=21;
           break;
        case 0x04:
          CH4_flag=0;
           break;
        case 0x05:
          CO_max=27;
          break;
        case 0x06:
          CO_flag=0;
          break;
        case 0x07:
          CO2_max=55;
          break;
         case 0x08:
          CO2_flag=0;
          break;
         case 0x09:
          H2_max=24;
          break;
         case 0x00:
          H2_flag=0;
          break;
         case 0x0B:
          CH4_flag=1;
          break;
         case 0x0C:
          CO2_flag=1;
          break;
         case 0x0D:
          CO_flag=1;
          break;
         case 0x0E:
          H2_flag=1;
          break;
         case 0x10:
         alarm_CH4=1;
          break;
         case 0x11:
         alarm_H2=1;
          break;
         case 0x12:
         alarm_CO=1;
          break;
         case 0x13:
         alarm_CO2=1;
          break;
         case 0x14:
         alarm_CH4=0;
          break;
         case 0x15:
         alarm_H2=0;
          break;
         case 0x16:
         alarm_CO=0;
          break;
         case 0x17:
         alarm_CO2=0;
          break;
      
   
     //发送传感器浓度,阈值
        case 0x0A:
          SU03T_DHT11_Play();
     


           break;
       

     }	
   }  

}



