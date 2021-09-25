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
//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define LED0_TASK_PRIO		2
//任务堆栈大小	
#define LED0_STK_SIZE 		50  
//任务句柄
TaskHandle_t LED0Task_Handler;
//任务函数
void led0_task(void *pvParameters);

//任务优先级
#define RTC_TASK_PRIO		3
//任务堆栈大小	
#define RTC_STK_SIZE 		128  
//任务句柄
TaskHandle_t RTCTask_Handler;
//任务函数
void rtc_task(void *pvParameters);

//任务优先级
#define FLOAT_TASK_PRIO		4
//任务堆栈大小	
#define FLOAT_STK_SIZE 		128
//任务句柄
TaskHandle_t FLOATTask_Handler;
//任务函数
void float_task(void *pvParameters);

int main()
{
    //外部晶振为25M的时候,推荐值:plln=160,pllm=5,pllp=2,pllq=4.
    //得到:Fvco=25*(160/5)=800Mhz
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
    RTC_Init();		 				//初始化RTC
    Touch_Init();	
    W25Q64_Init();
    //创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄
    vTaskStartScheduler();          //开启任务调度
}


//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
    //创建LED0任务
    xTaskCreate((TaskFunction_t )led0_task,     	
                (const char*    )"led0_task",   	
                (uint16_t       )LED0_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED0_TASK_PRIO,	
                (TaskHandle_t*  )&LED0Task_Handler);   
    //创建LED1任务
    xTaskCreate((TaskFunction_t )rtc_task,     
                (const char*    )"rtc_task",   
                (uint16_t       )RTC_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )RTC_TASK_PRIO,
                (TaskHandle_t*  )&RTCTask_Handler);        
    //浮点测试任务
    xTaskCreate((TaskFunction_t )float_task,     
                (const char*    )"float_task",   
                (uint16_t       )FLOAT_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )FLOAT_TASK_PRIO,
                (TaskHandle_t*  )&FLOATTask_Handler);  
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

//LED0任务函数 
void led0_task(void *pvParameters)
{
    while(1)
    {
        LED_TOGGLE(0);
        vTaskDelay(500);
    }
}   

//LED1任务函数
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

//浮点测试任务
void float_task(void *pvParameters)
{
	static double float_num=0.00;
	while(1)
	{
		float_num+=0.01f;
		printf("float_num的值为: %.4f\r\n",float_num);
        vTaskDelay(1000);
	}
}

