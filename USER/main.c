/*
 * @Descripttion: 
 * @Version: V1.0
 * @Author: YURI
 * @Email: 1700695611@qq.com
 * @Website: www.heredifferent.com.cn
 * @Date: 2021-08-19 13:48:38
 * @LastEditors: YURI
 * @LastEditTime: 2021-08-22 15:52:25
 */
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "LED.h"
#include "KEY.h"
#include "TIM.h"
#include "MPU.h"
#include "LTDC.h"
#include "usmart.h"
#include "RTC.h"
#include "ADC.h"
#include "TOUCH.h"
#include "W25Q64.h"
#include "malloc.h"
#include "sdmmc_sdcard.h"
#include "ff.h" 
#include "diskio.h"
#include "exfuns.h"  
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_msc.h"
#include "usbd_storage.h" 
#include "FreeRTOS.h"
#include "task.h"
//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		128  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

//�������ȼ�
#define LED0_TASK_PRIO		2
//�����ջ��С	
#define LED0_STK_SIZE 		50  
//������
TaskHandle_t LED0Task_Handler;
//������
void led0_task(void *pvParameters);

//�������ȼ�
#define RTC_TASK_PRIO		3
//�����ջ��С	
#define RTC_STK_SIZE 		128  
//������
TaskHandle_t RTCTask_Handler;
//������
void rtc_task(void *pvParameters);

//�������ȼ�
#define FLOAT_TASK_PRIO		4
//�����ջ��С	
#define FLOAT_STK_SIZE 		128
//������
TaskHandle_t FLOATTask_Handler;
//������
void float_task(void *pvParameters);

int main()
{
    //�ⲿ����Ϊ25M��ʱ��,�Ƽ�ֵ:plln=160,pllm=5,pllp=2,pllq=4.
    //�õ�:Fvco=25*(160/5)=800Mhz
    //     Fsys=pll1_p_ck=800/2=400Mhz
    //     Fq=pll1_q_ck=800/4=200Mhz	
	u8 t=0;	
	u8 res=0;
    Stm32_Clock_Init(160,5,2,4);
    delay_init(400);
    uart1_init(100,115200);
    LED_Init();
    KEY_Init();
    LTDC_Init();
    Adc3_Init();
    usmart_dev.init(200);
    RTC_Init();		 				//��ʼ��RTC
    Touch_Init();	
    W25Q64_Init();
    //������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������
    vTaskStartScheduler();          //�����������
}


//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
    //����LED0����
    xTaskCreate((TaskFunction_t )led0_task,     	
                (const char*    )"led0_task",   	
                (uint16_t       )LED0_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED0_TASK_PRIO,	
                (TaskHandle_t*  )&LED0Task_Handler);   
    //����LED1����
    xTaskCreate((TaskFunction_t )rtc_task,     
                (const char*    )"rtc_task",   
                (uint16_t       )RTC_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )RTC_TASK_PRIO,
                (TaskHandle_t*  )&RTCTask_Handler);        
    //�����������
    xTaskCreate((TaskFunction_t )float_task,     
                (const char*    )"float_task",   
                (uint16_t       )FLOAT_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )FLOAT_TASK_PRIO,
                (TaskHandle_t*  )&FLOATTask_Handler);  
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

//LED0������ 
void led0_task(void *pvParameters)
{
    while(1)
    {
        LED_TOGGLE(0);
        vTaskDelay(500);
    }
}   

//LED1������
void rtc_task(void *pvParameters)
{
    u8 hour,min,sec,ampm;
	u8 year,month,date,week;
	u8 tbuf[40];
    while(1)
    {
			RTC_Get_Time(&hour,&min,&sec,&ampm);
			sprintf((char*)tbuf,"Time:%02d:%02d:%02d",hour,min,sec); 
			LCD_DisplayText(30,40,tbuf);	
			RTC_Get_Date(&year,&month,&date,&week);
			sprintf((char*)tbuf,"Date:20%02d-%02d-%02d",year,month,date); 
            LCD_DisplayText(30,80,tbuf);	
			sprintf((char*)tbuf,"Week:%d",week); 
            LCD_DisplayText(30,120,tbuf);
            vTaskDelay(1000);        
    }
}

//�����������
void float_task(void *pvParameters)
{
	static double float_num=0.00;
	while(1)
	{
		float_num+=0.01f;
		printf("float_num��ֵΪ: %.4f\r\n",float_num);
        vTaskDelay(1000);
	}
}

