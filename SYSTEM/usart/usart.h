#ifndef __USART_H
#define __USART_H 
#include "sys.h"
#include "stdio.h"	  

#define USART1_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART1_RX_BUF[USART1_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART1_RX_STA;         		//接收状态标记	

void uart1_init(u32 pclk2,u32 bound); 
u8 USART1_SendByte(u8 data);
#endif	   
