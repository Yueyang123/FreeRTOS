/*
 * @Descripttion: 
 * @Version: V1.0
 * @Author: YURI
 * @Email: 1700695611@qq.com
 * @Website: www.heredifferent.com.cn
 * @Date: 2021-08-21 16:58:27
 * @LastEditors: YURI
 * @LastEditTime: 2021-08-21 17:20:39
 */
#ifndef QSPI_H
#define QSPI_H

#include "sys.h"

#define  QUADSPI_CLK_PIN							PIN2								// QUADSPI_CLK 引脚
#define	QUADSPI_CLK_PORT							GPIOB										// QUADSPI_CLK 引脚端口
#define	QUADSPI_CLK_AF								9						// QUADSPI_CLK IO口复用
#define 	GPIO_QUADSPI_CLK_ENABLE      			RCC->AHB4ENR|=(1<<1)	 	// QUADSPI_CLK 引脚时钟

#define  QUADSPI_BK1_NCS_PIN						PIN6								// QUADSPI_BK1_NCS 引脚
#define	QUADSPI_BK1_NCS_PORT						GPIOB										// QUADSPI_BK1_NCS 引脚端口
#define	QUADSPI_BK1_NCS_AF						    10						// QUADSPI_BK1_NCS IO口复用
#define 	GPIO_QUADSPI_BK1_NCS_ENABLE        	    RCC->AHB4ENR|=(1<<1)	 	// QUADSPI_BK1_NCS 引脚时钟

#define  QUADSPI_BK1_IO0_PIN						PIN11								// QUADSPI_BK1_IO0 引脚
#define	QUADSPI_BK1_IO0_PORT						GPIOD										// QUADSPI_BK1_IO0 引脚端口
#define	QUADSPI_BK1_IO0_AF						    9						// QUADSPI_BK1_IO0 IO口复用
#define 	GPIO_QUADSPI_BK1_IO0_ENABLE        	    RCC->AHB4ENR|=(1<<3)	 	// QUADSPI_BK1_IO0 引脚时钟

#define  QUADSPI_BK1_IO1_PIN						PIN12								// QUADSPI_BK1_IO1 引脚
#define	QUADSPI_BK1_IO1_PORT						GPIOD										// QUADSPI_BK1_IO1 引脚端口
#define	QUADSPI_BK1_IO1_AF						    9						// QUADSPI_BK1_IO1 IO口复用
#define 	GPIO_QUADSPI_BK1_IO1_ENABLE        	    RCC->AHB4ENR|=(1<<3)	 	// QUADSPI_BK1_IO1 引脚时钟

#define  QUADSPI_BK1_IO2_PIN						PIN2								// QUADSPI_BK1_IO2 引脚
#define	QUADSPI_BK1_IO2_PORT						GPIOE										// QUADSPI_BK1_IO2 引脚端口
#define	QUADSPI_BK1_IO2_AF						    9						// QUADSPI_BK1_IO2 IO口复用
#define 	GPIO_QUADSPI_BK1_IO2_ENABLE         	RCC->AHB4ENR|=(1<<4)	 	// QUADSPI_BK1_IO2 引脚时钟

#define  QUADSPI_BK1_IO3_PIN						PIN13								// QUADSPI_BK1_IO3 引脚
#define	QUADSPI_BK1_IO3_PORT						GPIOD										// QUADSPI_BK1_IO3 引脚端口
#define	QUADSPI_BK1_IO3_AF						    9						// QUADSPI_BK1_IO3 IO口复用
#define 	GPIO_QUADSPI_BK1_IO3_ENABLE      	    RCC->AHB4ENR|=(1<<3)	 	// QUADSPI_BK1_IO3 引脚时钟
u8 QSPI_Init(void);
void QSPI_Send_CMD(u8 cmd,u32 addr,u8 mode,u8 dmcycle);
u8 QSPI_Receive(u8* buf,u32 datalen);
u8 QSPI_Transmit(u8* buf,u32 datalen);


#endif