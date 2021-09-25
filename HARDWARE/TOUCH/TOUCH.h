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

#define Touch_IIC_SCL_CLK_ENABLE         RCC->AHB4ENR|=1<<4		        // SCL ����ʱ��
#define Touch_IIC_SCL_PORT   			 GPIOE                 			// SCL ���Ŷ˿�
#define Touch_IIC_SCL_PIN     		     PIN5  							// SCL ����
         
#define Touch_IIC_SDA_CLK_ENABLE         RCC->AHB4ENR|=1<<4 	        // SDA ����ʱ��
#define Touch_IIC_SDA_PORT   			 GPIOE                   		// SDA ���Ŷ˿�
#define Touch_IIC_SDA_PIN    			 PIN6              			    // SDA ����

#define Touch_INT_CLK_ENABLE    		 RCC->AHB4ENR|=1<<4		        // INT ����ʱ��
#define Touch_INT_PORT   				 GPIOE                   		// INT ���Ŷ˿�
#define Touch_INT_PIN    				 PIN4             			    // INT ����

#define Touch_RST_CLK_ENABLE   		     RCC->AHB4ENR|=1<<4		        // RST ����ʱ��
#define Touch_RST_PORT   				 GPIOE                   	    // RST ���Ŷ˿�
#define Touch_RST_PIN    				 PIN3           				// RST ����

/*------------------------------------------ IIC��ض��� -------------------------------------------------*/

#define ACK_OK  	1  			// ��Ӧ����
#define ACK_ERR 	0			// ��Ӧ����

// IICͨ����ʱ��Touch_IIC_Delay()����ʹ�ã�
//	ͨ���ٶ���100KHz����
#define IIC_DelayVaule  20  	

/*-------------------------------------------- IO�ڲ��� ---------------------------------------------------*/   

#define Touch_IIC_SCL(a)	if (a)	\
										GPIO_Pin_Set(Touch_IIC_SCL_PORT,Touch_IIC_SCL_PIN,1); \
									else		\
										GPIO_Pin_Set(Touch_IIC_SCL_PORT,Touch_IIC_SCL_PIN,0);	

#define Touch_IIC_SDA(a)	if (a)	\
										GPIO_Pin_Set(Touch_IIC_SDA_PORT, Touch_IIC_SDA_PIN, 1); \
									else		\
										GPIO_Pin_Set(Touch_IIC_SDA_PORT, Touch_IIC_SDA_PIN, 0);		

/*--------------------------------------------- �������� --------------------------------------------------*/  		
					
void 		Touch_IIC_GPIO_Config (void);					// IIC���ų�ʼ��
void 		Touch_IIC_Delay(uint32_t a);					// IIC��ʱ����							
void 		Touch_IIC_Start(void);							// ����IICͨ��
void 		Touch_IIC_Stop(void);							// IICֹͣ�ź�
void 		Touch_IIC_ACK(void);							//	������Ӧ�ź�
void 		Touch_IIC_NoACK(void);							// ���ͷ�Ӧ���ź�
uint8_t 	Touch_IIC_WaitACK(void);						//	�ȴ�Ӧ���ź�
uint8_t	    Touch_IIC_WriteByte(uint8_t IIC_Data); 	        // д�ֽں���
uint8_t 	Touch_IIC_ReadByte(uint8_t ACK_Mode);		    // ���ֽں���
		
#define  Touch_INT_Out(void) GPIO_Set(Touch_INT_PORT,Touch_INT_PIN,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_LOW,GPIO_PUPD_PU)
#define Touch_INT_In(void)  GPIO_Set(Touch_INT_PORT,Touch_INT_PIN,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_LOW,GPIO_PUPD_NONE)
#define  Touch_SDA_Out(void) GPIO_Set(Touch_IIC_SDA_PORT,Touch_IIC_SDA_PIN,GPIO_MODE_OUT,GPIO_OTYPE_OD,GPIO_SPEED_LOW,GPIO_PUPD_PU)
#define Touch_SDA_In(void)  GPIO_Set(Touch_IIC_SDA_PORT,Touch_IIC_SDA_PIN,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_LOW,GPIO_PUPD_NONE)


/*------------------------------------ ��ض��� -----------------------------------*/  	

#define TOUCH_MAX   5	//���������

typedef struct 
{
	uint8_t  flag;			//	������־λ��Ϊ1ʱ��ʾ�д�������
	uint8_t  num;				//	��������
	uint16_t x[TOUCH_MAX];	//	x����
	uint16_t y[TOUCH_MAX];	//	y����
}TouchStructure;

extern volatile TouchStructure touchInfo;	// �������ݽṹ������	

/*------------------------------------ �Ĵ涨�� -----------------------------------*/  		


#define GT9XX_IIC_RADDR_TEMP 0X29    			// IIC��ʼ����ַ
#define GT9XX_IIC_WADDR_TEMP 0X28		


#define GT9XX_CFG_ADDR 	0x8047		// �̼�������Ϣ�Ĵ�����������ʼ��ַ
#define GT9XX_READ_ADDR 0x814E		// ������Ϣ�Ĵ���
#define GT9XX_ID_ADDR 	0x8140		// �������ID�Ĵ���

/*------------------------------------ �������� -----------------------------------*/  		

uint8_t 	Touch_Init(void);		// ��������ʼ��
void 		Touch_Scan(void);		// ����ɨ��



#endif //__IIC_H
