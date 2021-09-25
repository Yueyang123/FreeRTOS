/*
 * @Descripttion: ��ʱ����
 * @version: V1.0
 * @Author: Yueyang
 * @email: 1700695611@qq.com
 * @Date: 2021-08-19 13:48:38
 * @LastEditors: YURI
 * @LastEditTime: 2021-08-21 08:47:29
 */
#include "delay.h"
#include "TOUCH.h"
//���ʹ��OS,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOSʹ��	 
#include "task.h"
#endif

static u32 fac_us;//��ʱ1us�������ʱ��
static u32 Times=0;//�ܹ������˶���ms


void SysTick_Handler(void)
{
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//ϵͳ�Ѿ�����
    {
        xPortSysTickHandler();	
    }
    
    if(Times%20==0)Touch_Scan();
	Times++;//ÿ��һ����1ms
}


/**
 * @name: Tick_Times
 * @msg: �ܹ������˶���ms
 * @param {*}
 * @return {*}
 */
u32 Tick_Times(void)
{
	return Times;
}
/**
 * @name: Tick_Value
 * @msg: �ڶ�Ӧ��ms�¾����˶���΢��
 * @param {*}
 * @return {*}
 */
u32 Tick_Value(void)
{
	return SysTick->VAL/fac_us;
}
/**
 * @name: delay_init
 * @msg: ��ʱ��ʼ�� ��ǰ�����SYSCLKӦ����400
 * @param {u16 SYSCLK}
 * @return {*}
 */
void delay_init(u16 SYSCLK)
{
    SysTick->CTRL|=1<<2;		//ѡ���ں�ʱ��Դ����400M
    fac_us=SYSCLK;      		//������SysTickÿ����400����1us
	SysTick->LOAD=1000*fac_us; 	//ʱ�����	  		 
	SysTick->CTRL|=3<<0 ;
}
/**
 * @name: delay_ms
 * @msg: ������ʱ
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
 * @msg: ΢����ʱ
 * @param {u32} nus
 * @return {*}
 */
void delay_us(u32 nus)
{
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD��ֵ	
   	ticks=nus*fac_us; 						//��Ҫ�Ľ����� 
 	told=SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	}
}
