/*
 * @Descripttion: 
 * @Version: V1.0
 * @Author: YURI
 * @Email: 1700695611@qq.com
 * @Website: www.heredifferent.com.cn
 * @Date: 2021-08-21 09:31:23
 * @LastEditors: YURI
 * @LastEditTime: 2021-08-21 09:55:00
 */
#ifndef __TOUCH_IIC_H
#define __TOUCH_IIC_H

#include "sys.h"

#define Touch_IIC_SCL_CLK_ENABLE         RCC->AHB4ENR|=1<<4		        // SCL 引脚时钟
#define Touch_IIC_SCL_PORT   			 GPIOE                 			// SCL 引脚端口
#define Touch_IIC_SCL_PIN     		     PIN5  							// SCL 引脚
         
#define Touch_IIC_SDA_CLK_ENABLE         RCC->AHB4ENR|=1<<4 	        // SDA 引脚时钟
#define Touch_IIC_SDA_PORT   			 GPIOE                   		// SDA 引脚端口
#define Touch_IIC_SDA_PIN    			 PIN6              			    // SDA 引脚

#define Touch_INT_CLK_ENABLE    		 RCC->AHB4ENR|=1<<4		        // INT 引脚时钟
#define Touch_INT_PORT   				 GPIOE                   		// INT 引脚端口
#define Touch_INT_PIN    				 PIN4             			    // INT 引脚

#define Touch_RST_CLK_ENABLE   		     RCC->AHB4ENR|=1<<4		        // RST 引脚时钟
#define Touch_RST_PORT   				 GPIOE                   	    // RST 引脚端口
#define Touch_RST_PIN    				 PIN3           				// RST 引脚

/*------------------------------------------ IIC相关定义 -------------------------------------------------*/

#define ACK_OK  	1  			// 响应正常
#define ACK_ERR 	0			// 响应错误

// IIC通信延时，Touch_IIC_Delay()函数使用，
//	通信速度在100KHz左右
#define IIC_DelayVaule  20  	

/*-------------------------------------------- IO口操作 ---------------------------------------------------*/   

#define Touch_IIC_SCL(a)	if (a)	\
										GPIO_Pin_Set(Touch_IIC_SCL_PORT,Touch_IIC_SCL_PIN,1); \
									else		\
										GPIO_Pin_Set(Touch_IIC_SCL_PORT,Touch_IIC_SCL_PIN,0);	

#define Touch_IIC_SDA(a)	if (a)	\
										GPIO_Pin_Set(Touch_IIC_SDA_PORT, Touch_IIC_SDA_PIN, 1); \
									else		\
										GPIO_Pin_Set(Touch_IIC_SDA_PORT, Touch_IIC_SDA_PIN, 0);		

/*--------------------------------------------- 函数声明 --------------------------------------------------*/  		
					
void 		Touch_IIC_GPIO_Config (void);					// IIC引脚初始化
void 		Touch_IIC_Delay(uint32_t a);					// IIC延时函数							
void 		Touch_IIC_Start(void);							// 启动IIC通信
void 		Touch_IIC_Stop(void);							// IIC停止信号
void 		Touch_IIC_ACK(void);							//	发送响应信号
void 		Touch_IIC_NoACK(void);							// 发送非应答信号
uint8_t 	Touch_IIC_WaitACK(void);						//	等待应答信号
uint8_t	    Touch_IIC_WriteByte(uint8_t IIC_Data); 	        // 写字节函数
uint8_t 	Touch_IIC_ReadByte(uint8_t ACK_Mode);		    // 读字节函数
		
#define  Touch_INT_Out(void) GPIO_Set(Touch_INT_PORT,Touch_INT_PIN,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_LOW,GPIO_PUPD_PU)
#define Touch_INT_In(void)  GPIO_Set(Touch_INT_PORT,Touch_INT_PIN,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_LOW,GPIO_PUPD_NONE)
#define  Touch_SDA_Out(void) GPIO_Set(Touch_IIC_SDA_PORT,Touch_IIC_SDA_PIN,GPIO_MODE_OUT,GPIO_OTYPE_OD,GPIO_SPEED_LOW,GPIO_PUPD_PU)
#define Touch_SDA_In(void)  GPIO_Set(Touch_IIC_SDA_PORT,Touch_IIC_SDA_PIN,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_LOW,GPIO_PUPD_NONE)


/*------------------------------------ 相关定义 -----------------------------------*/  	

#define TOUCH_MAX   5	//最大触摸点数

typedef struct 
{
	uint8_t  flag;			//	触摸标志位，为1时表示有触摸操作
	uint8_t  num;				//	触摸点数
	uint16_t x[TOUCH_MAX];	//	x坐标
	uint16_t y[TOUCH_MAX];	//	y坐标
}TouchStructure;

extern volatile TouchStructure touchInfo;	// 触摸数据结构体声明	

/*------------------------------------ 寄存定义 -----------------------------------*/  		


#define GT9XX_IIC_RADDR_TEMP 0X29    			// IIC初始化地址
#define GT9XX_IIC_WADDR_TEMP 0X28		


#define GT9XX_CFG_ADDR 	0x8047		// 固件配置信息寄存器和配置起始地址
#define GT9XX_READ_ADDR 0x814E		// 触摸信息寄存器
#define GT9XX_ID_ADDR 	0x8140		// 触摸面板ID寄存器

/*------------------------------------ 函数声明 -----------------------------------*/  		

uint8_t 	Touch_Init(void);		// 触摸屏初始化
void 		Touch_Scan(void);		// 触摸扫描



#endif //__IIC_H
