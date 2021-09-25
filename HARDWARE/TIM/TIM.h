/*
 * @Descripttion: 
 * @Version: V1.0
 * @Author: YURI
 * @Email: 1700695611@qq.com
 * @Website: www.heredifferent.com.cn
 * @Date: 2021-08-19 14:56:48
 * @LastEditors: YURI
 * @LastEditTime: 2021-08-19 15:42:56
 */
#ifndef  TIM_H
#define  TIM_H

#include "sys.h"

/**
 * @name: TIMx_Int_Init
 * @msg: 通用定时器中断模式初始化
 * @param {TIM_TypeDef*} TIMX
 * @param {uint16_t} arr
 * @param {uint16_t} psc
 * @return {*}
 */
void TIMx_Int_Init(TIM_TypeDef* TIMX,uint16_t arr,uint16_t psc);

/**
 * @name: TIM5_PWM_Init
 * @msg: 开启定时器5的PWM模式 PA0 CH1 PA2 CH3
 * @param {u32} arr
 * @param {u32} psc
 * @return {*}
 */
void TIM5_PWM_Init(u32 arr,u32 psc);
#define TIM5_CH1_CCR        TIM5->CCR1
#define TIM5_CH3_CCR        TIM5->CCR3


#endif // ! TIM_H
