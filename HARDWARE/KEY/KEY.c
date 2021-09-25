/*
 * @Descripttion: 按键控制
 * @Version: V1.0
 * @Author: Yueyang
 * @Email: 1700695611@qq.com
 * @Website: www.heredifferent.com.cn
 * @Date: 2021-08-19 14:03:57
 * @LastEditors: YURI
 * @LastEditTime: 2021-08-19 16:18:08
 */
#include "KEY.h"
#include "delay.h"
#include "LED.h"


/**
 * @name: __KEY_CTL
 * @msg: 按键控制句柄
 * @param {*}
 * @return {*}
 */
KEY_CTL KEY_HANDLE[1]={
    {GPIOC,PIN13,GPIO_PUPD_PU,0}
};
/**
 * @name: KEY_Init
 * @msg: 所有按键初始化
 * @param {*}
 * @return {*}
 */
void KEY_Init(void)
{
    uint16_t t;
    RCC->AHB4ENR|=1<<2;//使能GPIOC时钟
    for(t=0;t<sizeof(KEY_HANDLE)/sizeof(KEY_CTL);t++){
        GPIO_Set(KEY_HANDLE[t].GPIOX,KEY_HANDLE[t].PINX,0,0,0,KEY_HANDLE[t].status);//设置为输入
        #if ENABLE_KEY_IRQ
            KEY_IRQInit(t);
        #endif
    }
}
/**
 * @name: KEY_Scan
 * @msg: 按键信息扫描
 * @param {uint8_t} index
 * @return {*}
 */
uint8_t KEY_Scan(uint8_t index)
{
    delay_ms(2);
    if(KEY_HANDLE[index].status==GPIO_PUPD_PU) //内部上拉，外部下拉情况
    {
        if(GPIO_Pin_Get(KEY_HANDLE[index].GPIOX,KEY_HANDLE[index].PINX))return 0;//没有按下
        else return 1;
    }else{//内部下拉，外部上拉
        if(GPIO_Pin_Get(KEY_HANDLE[index].GPIOX,KEY_HANDLE[index].PINX))return 1;//没有按下
        else return 0; 
    }
}

/**
 * @name: KEY_IRQInit
 * @msg: 初始化按键的中断模式可以不使用
 * @param {*}
 * @return {*}
 */
void KEY_IRQInit(uint8_t index)
{
    if(index==0){
        //PC13做中断输入
        Ex_NVIC_Config(GPIO_C,13,FTIR); 
        MY_NVIC_Init(2,0,EXTI15_10_IRQn,NVICPRIORITYGROUP);
    }
}
/**
 * @name: KEY_PressedTimes
 * @msg: 返回已经按下的次数
 * @param {uint8_t} index
 * @return {*}
 */
uint16_t KEY_PressedTimes(uint8_t index)
{
    return KEY_HANDLE[index].times;
}

//外部中断10~15服务程序
void EXTI15_10_IRQHandler(void)
{
	delay_ms(1);		//消抖
	EXTI_D1->PR1=1<<13;  //清除LINE13上的中断标志位  
	if(KEY_Scan(0)==1)	 
	{	

         KEY_HANDLE[0].times++;
         LED_TOGGLE(0);		
	}		  
}
