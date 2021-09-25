/*
 * @Descripttion: 
 * @Version: V1.0
 * @Author: YURI
 * @Email: 1700695611@qq.com
 * @Website: www.heredifferent.com.cn
 * @Date: 2021-08-19 14:56:41
 * @LastEditors: YURI
 * @LastEditTime: 2021-08-19 15:43:30
 */
#include "TIM.h"
#include "LED.h"

/**
 * @name: TIMx_Int_Init
 * @msg: 通用定时器中断模式初始化
 * 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
 * Ft=定时器工作频率,单位:Mhz
 * STM32的通用TIMx (TIM2~TIM5和TIM12~TIM17)：  
   16位/32位(仅TIM2和TIM5)向上、向下、向上/向下自动装载计数器（TIMx_CNT），注 
   意：TIM12~TIM17只支持向上（递增）计数方式。
 * @param {TIM_TypeDef*} TIMX
 * @param {uint16_t} arr
 * @param {uint16_t} psc
 * @return {*}
 */
void TIMx_Int_Init(TIM_TypeDef* TIMX,uint16_t arr,uint16_t psc)
{
    //定时器3时钟
    //RCC->APB1LENR|=1<<1;//定时器时钟开启
    //while((RCC->APB1LENR&(1<<1))==0);	//等待时钟设置OK  
    
    //定时器2时钟
    RCC->APB1LENR|=1<<0;//定时器时钟开启
	while((RCC->APB1LENR&(1<<0))==0);	//等待时钟设置OK  
       
 	TIMX->ARR=arr;  				//设定计数器自动重装值 
	TIMX->PSC=psc;  				//预分频器
	TIMX->SMCR=0;					//SMS[3:0]=0000,CK_INT作为时钟源
	TIMX->DIER|=1<<0;  				//允许更新中断	  
	TIMX->CR1&=~(3<<5);				//CMS[1:0]=00,边沿对齐模式
	TIMX->CR1&=~(1<<4);				//DIR=0,向上计数模式
	TIMX->CR1|=0x01;				//使能定时器

    //定时器3中断
  	//MY_NVIC_Init(3,0,TIM3_IRQn,NVICPRIORITYGROUP);	//抢占1，子优先级3，组2	
    //定时器2中断
  	MY_NVIC_Init(3,0,TIM2_IRQn,NVICPRIORITYGROUP);	//抢占1，子优先级3，组2	
    
}


/**
 * @name: TIM5_PWM_Init
 * @msg: 开启定时器5的PWM模式 PA0 CH1 PA2 CH3
 * @param {u32} arr
 * @param {u32} psc
 * @return {*}
 */
void TIM5_PWM_Init(u32 arr,u32 psc)
{		 					 
	//此部分需手动修改IO口设置
	RCC->APB1LENR|=1<<3;	//TIM5时钟使能    
	RCC->AHB4ENR|=1<<0;   	//使能PORTA时钟	
	GPIO_Set(GPIOA,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_MID,GPIO_PUPD_PU);//复用功能,上拉输出
    GPIO_Set(GPIOA,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_MID,GPIO_PUPD_PU);
	GPIO_AF_Set(GPIOA,0,2);
	GPIO_AF_Set(GPIOA,2,2);

	TIM5->ARR=arr;			//设定计数器自动重装值 
	TIM5->PSC=psc;			//预分频器不分频 
    
    TIM5->CCMR1|=6<<4;  	//CH3 PWM1模式		 
	TIM5->CCMR1|=1<<3; 	    //CH3 预装载使能	 
	TIM5->CCMR2|=6<<4;  	//CH3 PWM1模式		 
	TIM5->CCMR2|=1<<3; 	    //CH3 预装载使能	   
	
    TIM5->CCER|=1<<8;   	//OCC 输出使能	
	TIM5->CCER|=1<<9;   	//OC3 低电平有效	 
    TIM5->CCER|=1<<0;   	//OCC 输出使能	
	TIM5->CCER|=1<<1;   	//OC3 低电平有效	 
    
	TIM5->CR1|=1<<7;   		//ARPE使能 
	TIM5->CR1|=1<<0;    	//使能定时器5
	TIM5->DIER|=1<<0;  		//允许更新中断
    MY_NVIC_Init(3,0,TIM5_IRQn,NVICPRIORITYGROUP);
    TIM5_CH1_CCR=arr/2;
    TIM5_CH3_CCR=arr/3;											  
} 

//定时器3中断服务程序	 
void TIM2_IRQHandler(void)
{
	if(TIM2->SR&0X0001)//溢出中断
	{   
        //LED_TOGGLE(0);
	}				   
	TIM2->SR&=~(1<<0);//清除中断标志位 	
}

//定时器3中断服务程序	 
void TIM3_IRQHandler(void)
{
	if(TIM3->SR&0X0001)//溢出中断
	{   
        LED_TOGGLE(0);
	}				   
	TIM3->SR&=~(1<<0);//清除中断标志位 	
}

void TIM5_IRQHandler(void)
{
    static uint8_t times=0;
	if(TIM5->SR&0X0001)//溢出中断
	{   
        times++;
        if(times%50==0){
            LED_TOGGLE(0);
        }
	}				   
	TIM5->SR&=~(1<<0);//清除中断标志位 	
}