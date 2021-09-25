/*
 * @Descripttion: LED����
 * @Version: V1.0
 * @Author: YURI
 * @Email: 1700695611@qq.com
 * @Website: www.heredifferent.com.cn
 * @Date: 2021-08-19 13:48:38
 * @LastEditors: YURI
 * @LastEditTime: 2021-08-19 14:33:43
 */

#include "LED.h"
#include "usart.h"
/**
 * @name: LED_HANDLE
 * @msg: ����С�Ƶľ��
 * @param {*}
 * @return {*}
 */
LED_CTL LED_HANDLE[]={
    {GPIOC ,PIN1,1}
};

/**
 * @name: LED_Init
 * @msg: ����С�Ƴ�ʼ��
 * @param {*}
 * @return {*}
 */
void LED_Init(void)
{	
    uint8_t t=0;
	RCC->AHB4ENR|=1<<2;	//ʹ��PORTCʱ�� 
    
    for(t=0;t<sizeof(LED_HANDLE)/sizeof(LED_CTL);t++){
        GPIO_Set(LED_HANDLE[t].GPIOX,LED_HANDLE[t].PINX,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_MID,GPIO_PUPD_PU);
        GPIO_Pin_Set(LED_HANDLE[t].GPIOX,LED_HANDLE[t].PINX,LED_HANDLE[t].status);
    }
}
/**
 * @name: LED_ON
 * @msg: ��ָ��С��
 * @param {uint8_t} index
 * @return {*}
 */
void LED_ON(uint8_t index)
{
    GPIO_Pin_Set(LED_HANDLE[index].GPIOX,LED_HANDLE[index].PINX,1);
    LED_HANDLE[index].status=1;
}
/**
 * @name: LED_OFF
 * @msg: �ر�ָ��С��
 * @param {uint8_t} index
 * @return {*}
 */
void LED_OFF(uint8_t index)
{
    GPIO_Pin_Set(LED_HANDLE[index].GPIOX,LED_HANDLE[index].PINX,0);
    LED_HANDLE[index].status=0;
}
/**
 * @name: LED_TOGGLE
 * @msg: ��תָ��С��
 * @param {uint8_t} index
 * @return {*}
 */
void LED_TOGGLE(uint8_t index)
{
    LED_HANDLE[index].status=!LED_HANDLE[index].status;
    GPIO_Pin_Set(LED_HANDLE[index].GPIOX,LED_HANDLE[index].PINX,LED_HANDLE[index].status);
}
