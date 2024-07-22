#ifndef _USART_H_
#define _USART_H_


#include "stm32f10x.h"
#define SERIAL_DATA_LEN                     256
#define USART_DEBUG		USART1		//调试打印所使用的串口组


extern u8 USART3_RX_STA;         		//接收状态标记
extern u8 USART3_RX_CMD;
extern int su03_flag;
extern unsigned char Serial_Head;
extern unsigned char Serial_Tead;
extern unsigned char Serial_Data_Buff[SERIAL_DATA_LEN];


void Usart1_Init(unsigned int baud);
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);


void Usart2_Init(unsigned int baud);


void Usart3_Init(unsigned int baud);
void USART3_SendByte(uint8_t  Data) ;
void Uart3_SU03T_SendCMD2(int dat1);
void Uart3_SU03T_SendCMD1(int dat1, int dat2);
void Uart3_SU03T_SendCmd(int len);
void SU03T_DHT11_Play(void);
void Uart3_SU03T_alarm(void);
#endif
