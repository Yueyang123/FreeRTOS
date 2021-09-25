/*
 * @Descripttion: ��������
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
 * @msg: �������ƾ��
 * @param {*}
 * @return {*}
 */
typedef struct __KEY_CTL
{   
    GPIO_TypeDef* GPIOX;
    uint16_t      PINX;
    uint8_t       status;//������״̬
    uint16_t      times; //һ�����µĴ���
}KEY_CTL;
 
/**
 * @name: KEY_Init
 * @msg: ���а�����ʼ��
 * @param {*}
 * @return {*}
 */
void KEY_Init(void);
/**
 * @name: KEY_Scan
 * @msg: ������Ϣɨ��
 * @param {uint8_t} index
 * @return {*}
 */
uint8_t KEY_Scan(uint8_t index);
/**
 * @name: KEY_PressedTimes
 * @msg: �����Ѿ����µĴ���
 * @param {uint8_t} index
 * @return {*}
 */
uint16_t KEY_PressedTimes(uint8_t index);
/**
 * @name: KEY_IRQInit
 * @msg: ��ʼ���������ж�ģʽ���Բ�ʹ��
 * @param {*}
 * @return {*}
 */
void KEY_IRQInit(uint8_t index);


#endif // ! KEY_H

