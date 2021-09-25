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
 * @msg: ͨ�ö�ʱ���ж�ģʽ��ʼ��
 * ��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
 * Ft=��ʱ������Ƶ��,��λ:Mhz
 * STM32��ͨ��TIMx (TIM2~TIM5��TIM12~TIM17)��  
   16λ/32λ(��TIM2��TIM5)���ϡ����¡�����/�����Զ�װ�ؼ�������TIMx_CNT����ע 
   �⣺TIM12~TIM17ֻ֧�����ϣ�������������ʽ��
 * @param {TIM_TypeDef*} TIMX
 * @param {uint16_t} arr
 * @param {uint16_t} psc
 * @return {*}
 */
void TIMx_Int_Init(TIM_TypeDef* TIMX,uint16_t arr,uint16_t psc)
{
    //��ʱ��3ʱ��
    //RCC->APB1LENR|=1<<1;//��ʱ��ʱ�ӿ���
    //while((RCC->APB1LENR&(1<<1))==0);	//�ȴ�ʱ������OK  
    
    //��ʱ��2ʱ��
    RCC->APB1LENR|=1<<0;//��ʱ��ʱ�ӿ���
	while((RCC->APB1LENR&(1<<0))==0);	//�ȴ�ʱ������OK  
       
 	TIMX->ARR=arr;  				//�趨�������Զ���װֵ 
	TIMX->PSC=psc;  				//Ԥ��Ƶ��
	TIMX->SMCR=0;					//SMS[3:0]=0000,CK_INT��Ϊʱ��Դ
	TIMX->DIER|=1<<0;  				//��������ж�	  
	TIMX->CR1&=~(3<<5);				//CMS[1:0]=00,���ض���ģʽ
	TIMX->CR1&=~(1<<4);				//DIR=0,���ϼ���ģʽ
	TIMX->CR1|=0x01;				//ʹ�ܶ�ʱ��

    //��ʱ��3�ж�
  	//MY_NVIC_Init(3,0,TIM3_IRQn,NVICPRIORITYGROUP);	//��ռ1�������ȼ�3����2	
    //��ʱ��2�ж�
  	MY_NVIC_Init(3,0,TIM2_IRQn,NVICPRIORITYGROUP);	//��ռ1�������ȼ�3����2	
    
}


/**
 * @name: TIM5_PWM_Init
 * @msg: ������ʱ��5��PWMģʽ PA0 CH1 PA2 CH3
 * @param {u32} arr
 * @param {u32} psc
 * @return {*}
 */
void TIM5_PWM_Init(u32 arr,u32 psc)
{		 					 
	//�˲������ֶ��޸�IO������
	RCC->APB1LENR|=1<<3;	//TIM5ʱ��ʹ��    
	RCC->AHB4ENR|=1<<0;   	//ʹ��PORTAʱ��	
	GPIO_Set(GPIOA,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_MID,GPIO_PUPD_PU);//���ù���,�������
    GPIO_Set(GPIOA,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_MID,GPIO_PUPD_PU);
	GPIO_AF_Set(GPIOA,0,2);
	GPIO_AF_Set(GPIOA,2,2);

	TIM5->ARR=arr;			//�趨�������Զ���װֵ 
	TIM5->PSC=psc;			//Ԥ��Ƶ������Ƶ 
    
    TIM5->CCMR1|=6<<4;  	//CH3 PWM1ģʽ		 
	TIM5->CCMR1|=1<<3; 	    //CH3 Ԥװ��ʹ��	 
	TIM5->CCMR2|=6<<4;  	//CH3 PWM1ģʽ		 
	TIM5->CCMR2|=1<<3; 	    //CH3 Ԥװ��ʹ��	   
	
    TIM5->CCER|=1<<8;   	//OCC ���ʹ��	
	TIM5->CCER|=1<<9;   	//OC3 �͵�ƽ��Ч	 
    TIM5->CCER|=1<<0;   	//OCC ���ʹ��	
	TIM5->CCER|=1<<1;   	//OC3 �͵�ƽ��Ч	 
    
	TIM5->CR1|=1<<7;   		//ARPEʹ�� 
	TIM5->CR1|=1<<0;    	//ʹ�ܶ�ʱ��5
	TIM5->DIER|=1<<0;  		//��������ж�
    MY_NVIC_Init(3,0,TIM5_IRQn,NVICPRIORITYGROUP);
    TIM5_CH1_CCR=arr/2;
    TIM5_CH3_CCR=arr/3;											  
} 

//��ʱ��3�жϷ������	 
void TIM2_IRQHandler(void)
{
	if(TIM2->SR&0X0001)//����ж�
	{   
        //LED_TOGGLE(0);
	}				   
	TIM2->SR&=~(1<<0);//����жϱ�־λ 	
}

//��ʱ��3�жϷ������	 
void TIM3_IRQHandler(void)
{
	if(TIM3->SR&0X0001)//����ж�
	{   
        LED_TOGGLE(0);
	}				   
	TIM3->SR&=~(1<<0);//����жϱ�־λ 	
}

void TIM5_IRQHandler(void)
{
    static uint8_t times=0;
	if(TIM5->SR&0X0001)//����ж�
	{   
        times++;
        if(times%50==0){
            LED_TOGGLE(0);
        }
	}				   
	TIM5->SR&=~(1<<0);//����жϱ�־λ 	
}