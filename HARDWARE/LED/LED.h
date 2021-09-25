/*
 * @Descripttion: LED灯控制
 * @Version: V1.0
 * @Author: YURI
 * @Email: 1700695611@qq.com
 * @Website: www.heredifferent.com.cn
 * @Date: 2021-08-19 13:48:38
 * @LastEditors: YURI
 * @LastEditTime: 2021-08-19 14:13:52
 */
#ifndef _LED_H
#define _LED_H
#include "sys.h"

/**
 * @name: __LED_CTL
 * @msg: 控制小灯的句柄
 * @param {*}
 * @return {*}
 */
typedef struct __LED_CTL{
    GPIO_TypeDef* GPIOX;
    uint16_t  PINX;
    uint8_t   status;//当前状态 
}LED_CTL;
/**
 * @name: LED_Init
 * @msg: 所有小灯初始化
 * @param {*}
 * @return {*}
 */
void LED_Init(void);
/**
 * @name: LED_ON
 * @msg: 打开指定小灯
 * @param {uint8_t} index
 * @return {*}
 */
void LED_ON(uint8_t index);
/**
 * @name: LED_OFF
 * @msg: 关闭指定小灯
 * @param {uint8_t} index
 * @return {*}
 */
void LED_OFF(uint8_t index);
/**
 * @name: LED_TOGGLE
 * @msg: 反转指定小灯
 * @param {uint8_t} index
 * @return {*}
 */
void LED_TOGGLE(uint8_t index);
#endif
