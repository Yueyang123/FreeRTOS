/*
 * @Descripttion: 延时功能
 * @version: V1.0
 * @Author: Yueyang
 * @email: 1700695611@qq.com
 * @Date: 2021-08-19 13:48:38
 * @LastEditors: YURI
 * @LastEditTime: 2021-08-21 08:47:29
 */
#include "delay.h"
#include "TOUCH.h"
//如果使用OS,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOS使用	 
#include "task.h"
#endif

static u32 fac_us;//延时1us所代表的时钟
static u32 Times=0;//总共经历了多少ms


void SysTick_Handler(void)
{
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
    {
        xPortSysTickHandler();	
    }
    
    if(Times%20==0)Touch_Scan();
	Times++;//每加一就是1ms
}


/**
 * @name: Tick_Times
 * @msg: 总共经历了多少ms
 * @param {*}
 * @return {*}
 */
u32 Tick_Times(void)
{
	return Times;
}
/**
 * @name: Tick_Value
 * @msg: 在对应的ms下经历了多少微秒
 * @param {*}
 * @return {*}
 */
u32 Tick_Value(void)
{
	return SysTick->VAL/fac_us;
}
/**
 * @name: delay_init
 * @msg: 延时初始化 当前情况下SYSCLK应该是400
 * @param {u16 SYSCLK}
 * @return {*}
 */
void delay_init(u16 SYSCLK)
{
    SysTick->CTRL|=1<<2;		//选择内核时钟源就是400M
    fac_us=SYSCLK;      		//代表着SysTick每增加400就是1us
	SysTick->LOAD=1000*fac_us; 	//时间加载	  		 
	SysTick->CTRL|=3<<0 ;
}
/**
 * @name: delay_ms
 * @msg: 毫秒延时
 * @param {u16} nms
 * @return {*}
 */
void delay_ms(u32 nms)
{
	u32 oldtime=0;
	oldtime=Tick_Times();
	while(Tick_Times()-oldtime<nms);
}
/**
 * @name: delay_us
 * @msg: 微秒延时
 * @param {u32} nus
 * @return {*}
 */
void delay_us(u32 nus)
{
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD的值	
   	ticks=nus*fac_us; 						//需要的节拍数 
 	told=SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	}
}
