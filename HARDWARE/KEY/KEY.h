/*
 * @Descripttion: 按键控制
 * @Version: V1.0
 * @Author: Yueyang
 * @Email: 1700695611@qq.com
 * @Website: www.heredifferent.com.cn
 * @Date: 2021-08-19 14:04:48
 * @LastEditors: YURI
 * @LastEditTime: 2021-08-19 16:18:19
 */
#ifndef  KEY_H
#define KEY_H

#include "sys.h"

#define ENABLE_KEY_IRQ  1

/**
 * @name: __KEY_CTL
 * @msg: 按键控制句柄
 * @param {*}
 * @return {*}
 */
typedef struct __KEY_CTL
{   
    GPIO_TypeDef* GPIOX;
    uint16_t      PINX;
    uint8_t       status;//上下拉状态
    uint16_t      times; //一共按下的次数
}KEY_CTL;
 
/**
 * @name: KEY_Init
 * @msg: 所有按键初始化
 * @param {*}
 * @return {*}
 */
void KEY_Init(void);
/**
 * @name: KEY_Scan
 * @msg: 按键信息扫描
 * @param {uint8_t} index
 * @return {*}
 */
uint8_t KEY_Scan(uint8_t index);
/**
 * @name: KEY_PressedTimes
 * @msg: 返回已经按下的次数
 * @param {uint8_t} index
 * @return {*}
 */
uint16_t KEY_PressedTimes(uint8_t index);
/**
 * @name: KEY_IRQInit
 * @msg: 初始化按键的中断模式可以不使用
 * @param {*}
 * @return {*}
 */
void KEY_IRQInit(uint8_t index);


#endif // ! KEY_H

