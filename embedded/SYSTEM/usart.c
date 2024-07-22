/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	usart.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2016-11-23
	*
	*	版本： 		V1.0
	*
	*	说明： 		单片机串口外设初始化，格式化打印
	*
	*	修改记录：	
	************************************************************
	************************************************************
	************************************************************
**/

//硬件驱动
#include "usart.h"
#include "delay.h"
#include "timer.h"

//C库
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

//硬件数据
#include "adc.h"
#include "JW.h"
unsigned char Serial_Head = 0;
unsigned char Serial_Tead = 0;
unsigned char Serial_Data_Buff[SERIAL_DATA_LEN];

static int Send_buf[10] = {0} ;
/*
************************************************************
*	函数名称：	Usart1_Init
*
*	函数功能：	串口1初始化
*
*	入口参数：	baud：设定的波特率
*
*	返回参数：	无
*
*	说明：		TX-PA9		RX-PA10
************************************************************
*/

void Usart3_Init(unsigned int baud)
{
  GPIO_InitTypeDef gpio_initstruct;
	USART_InitTypeDef usart_initstruct;
	NVIC_InitTypeDef nvic_initstruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	//PB10	TXD
	gpio_initstruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_initstruct.GPIO_Pin = GPIO_Pin_10;
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_initstruct);
	
	//PB11	RXD
	gpio_initstruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_initstruct.GPIO_Pin = GPIO_Pin_11;
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_initstruct);
	
	usart_initstruct.USART_BaudRate = baud;
	usart_initstruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控
	usart_initstruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//接收和发送
	usart_initstruct.USART_Parity = USART_Parity_No;									//无校验
	usart_initstruct.USART_StopBits = USART_StopBits_1;								//1位停止位
	usart_initstruct.USART_WordLength = USART_WordLength_8b;							//8位数据位
	USART_Init(USART3, &usart_initstruct);
	
	USART_Cmd(USART3, ENABLE);														//使能串口
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);									//使能接收中断
	
	nvic_initstruct.NVIC_IRQChannel = USART3_IRQn;
	nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
	nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_initstruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvic_initstruct);
  
}



void Usart1_Init(unsigned int baud)
{
  
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    //①串口时钟使能，GPIO 时钟使能，复用时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE); //使能 USART1,GPIOA 时钟
    //②串口复位
    USART_DeInit(USART1); //复位串口 1
    //③GPIO 端口模式设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //ISART1_TX PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化 GPIOA.9

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //USART1_RX PA.10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化 GPIOA.10
    //④串口参数初始化
    USART_InitStructure.USART_BaudRate = 9600; //波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为 8 位
    USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None; //无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式

    USART_Init(USART1, &USART_InitStructure); //初始化串口
    //⑤开启中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //开启中断
    //⑥使能串口
    USART_Cmd(USART1, ENABLE); //使能串口
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ; //抢占优先级 3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //子优先级 3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道使能
    NVIC_Init(&NVIC_InitStructure); //中断优先级初始化

}

/*
************************************************************
*	函数名称：	Usart2_Init
*
*	函数功能：	串口2初始化
*
*	入口参数：	baud：设定的波特率
*
*	返回参数：	无
*
*	说明：		TX-PA2		RX-PA3
************************************************************
*/
void Usart2_Init(unsigned int baud)
{

	GPIO_InitTypeDef gpio_initstruct;
	USART_InitTypeDef usart_initstruct;
	NVIC_InitTypeDef nvic_initstruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//PA2	TXD
	gpio_initstruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_initstruct.GPIO_Pin = GPIO_Pin_2;
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_initstruct);
	
	//PA3	RXD
	gpio_initstruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_initstruct.GPIO_Pin = GPIO_Pin_3;
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_initstruct);
	
	usart_initstruct.USART_BaudRate = baud;
	usart_initstruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控
	usart_initstruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//接收和发送
	usart_initstruct.USART_Parity = USART_Parity_No;									//无校验
	usart_initstruct.USART_StopBits = USART_StopBits_1;								//1位停止位
	usart_initstruct.USART_WordLength = USART_WordLength_8b;							//8位数据位
	USART_Init(USART2, &usart_initstruct);
	
	USART_Cmd(USART2, ENABLE);														//使能串口
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);									//使能接收中断
	
	nvic_initstruct.NVIC_IRQChannel = USART2_IRQn;
	nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
	nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_initstruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvic_initstruct);

}

/*
************************************************************
*	函数名称：	Usart_SendString
*
*	函数功能：	串口数据发送
*
*	入口参数：	USARTx：串口组
*				str：要发送的数据
*				len：数据长度
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len)
{

	unsigned short count = 0;
	
	for(; count < len; count++)
	{
		USART_SendData(USARTx, *str++);									//发送数据
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//等待发送完成
	}

}
void Usart_SendHexData(USART_TypeDef *USARTx, unsigned char *data, unsigned short len)
{
    unsigned short count = 0;
    
    for (; count < len; count++)
    {
        USART_SendData(USARTx, data[count]); // 发送数据
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET); // 等待发送完成
    }
}

/*
************************************************************
*	函数名称：	UsartPrintf
*
*	函数功能：	格式化打印
*
*	入口参数：	USARTx：串口组
*				fmt：不定长参
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...)
{

	unsigned char UsartPrintfBuf[296];
	va_list ap;
	unsigned char *pStr = UsartPrintfBuf;
	
	va_start(ap, fmt);
	vsnprintf((char *)UsartPrintfBuf, sizeof(UsartPrintfBuf), fmt, ap);							//格式化
	va_end(ap);
	
	while(*pStr != 0)
	{
		USART_SendData(USARTx, *pStr++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}

}

/*
************************************************************
*	函数名称：	USART1_IRQHandler
*
*	函数功能：	串口1收发中断
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/

void USART1_IRQHandler(void)
{
   
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收中断
    {
       
        Serial_Data_Buff[Serial_Head] = USART_ReceiveData(USART1);
        
        Serial_Head = (Serial_Head +1 ) % SERIAL_DATA_LEN;
        USART_ClearFlag(USART1, USART_FLAG_RXNE);
    }
}


u8 USART3_RX_STA = 0;     //串口3接收状态标记
u8 USART3_RX_CMD;

void USART3_IRQHandler(void)                           	 
{
  
  
    u8 Res = 0;

    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  
    {
        Res = USART_ReceiveData(USART3);	               
        USART3_RX_CMD = Res;                               
        USART3_RX_STA = 1;	
//       UsartPrintf(USART_DEBUG,"消息1：%d\r\n",USART3_RX_CMD);
//          	UsartPrintf(USART_DEBUG,"标准：%d\r\n",USART3_RX_STA);

    }
}

void USART3_SendByte(uint8_t  Data)                                 //串口发送一个字节；字节 (byte)    1byte=8bit
{
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);    //USART_FLAG_TXE发送寄存器空

    USART_SendData(USART3, Data);                                   //从串口2发送传入的数据

    while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	    //USART_FLAG_TC发送完成标志
}

/*
 * 功能描述：串口3向外(SU-03T)发送命令
 * 内在逻辑：按照智能公元平台的数据接受协议，发送数据过去
*/
void Uart3_SU03T_SendCmd(int len)
{
  int i = 0 ;

    USART3_SendByte(0xAA);       //起始
    USART3_SendByte(0x55);

    for(i = 0; i < len; i++)     //数据
    {
        USART3_SendByte(Send_buf[i]);   //len 为8 ；依次将Send_buf[0]、Send_buf[1]~Send_buf[8]  发送出来
    }

    USART3_SendByte(0x55);
    USART3_SendByte(0xAA);      //结束

}
/*
 * 功能描述：串口3向外(SU-03T)发送命令(2位数据)
 * 内在逻辑：按照智能公元平台的数据接受协议，发送数据过去
*/
void Uart3_SU03T_SendCMD1(int dat1, int dat2)
{
    Send_buf[0] = (int)(dat1);         //datal
    Send_buf[1] = (int)(dat2);         //data2
    Uart3_SU03T_SendCmd(2);            //发送此帧数据
}
/*
 * 功能描述：串口3向外(SU-03T)发送命令(1位数据)
 * 内在逻辑：按照智能公元平台的数据接受协议，发送数据过去
*/
void Uart3_SU03T_SendCMD2(int dat1)
{
    Send_buf[0] = (int)(dat1);         //datal
   
    Uart3_SU03T_SendCmd(1);            //发送此帧数据

}
//函数功能：SU-03T播报DHT11温湿度(可以播报小数和点)
void SU03T_DHT11_Play(void)
{
    u8 t = 0;
    if(t % 10 == 0) //每10ms读取一次
    {
     Uart3_SU03T_SendCMD1(0X05, MQ4.CH4); /*(甲烷)上传给SU-03T进行处理*/
     Uart3_SU03T_SendCMD1(0X06,CH4_max); /*(甲烷阈值)上传给SU-03T进行处理*/
      
     Uart3_SU03T_SendCMD1(0X07,MQ7.CO);  
     Uart3_SU03T_SendCMD1(0X08,CO_max); 
      
     Uart3_SU03T_SendCMD1(0X09,JW_Value);
     Uart3_SU03T_SendCMD1(0X0A,CO2_max);  //消息号10
      
     Uart3_SU03T_SendCMD1(0X0B,MQ8.H2);
     Uart3_SU03T_SendCMD1(0X0C,H2_max);
    }
     
    delay_ms(10);
    t++;

    if(t == 20)/*200ms最大检测时间，满则清零，重新累加计算*/
    {
        t = 0;
    }

}

void Uart3_SU03T_alarm(void)
{
     

        if(alarm_CH4==1&&MQ4.CH4>=CH4_max)
        {
          Uart3_SU03T_SendCMD1(0X05, MQ4.CH4);
          Uart3_SU03T_SendCMD2(0X0D);
         
      
        }else if(alarm_H2==1&&MQ8.H2>=H2_max)
        {
          Uart3_SU03T_SendCMD1(0X0B,MQ8.H2);
          Uart3_SU03T_SendCMD2(0X0E);
        
      
        }else if(alarm_CO==1&&MQ7.CO>=CO_max)
        {
        Uart3_SU03T_SendCMD1(0X07,MQ7.CO);
        Uart3_SU03T_SendCMD2(0X0F);
       
        
        }
    
}




