/*
 * @Descripttion: ��ʱ����
 * @version: V1.0
 * @Author: Yueyang
 * @email: 1700695611@qq.com
 * @Date: 2021-08-19 13:48:38
 * @LastEditors: YURI
 * @LastEditTime: 2021-08-21 11:54:58
 */
#ifndef __DELAY_H
#define __DELAY_H 			   
#include <sys.h>	  
/**
 * @name: delay_init
 * @msg: ��ʱ��ʼ�� ��ǰ�����SYSCLKӦ����400
 * @param {u16 SYSCLK}
 * @return {*}
 */
void delay_init(u16 SYSCLK);
/**
 * @name: delay_ms
 * @msg: ������ʱ
 * @param {u16} nms
 * @return {*}
 */
void delay_ms(u32 nms);
/**
 * @name: delay_us
 * @msg: ΢����ʱ
 * @param {u32} nus
 * @return {*}
 */
void delay_us(u32 nus);
/**
 * @name: Tick_Times
 * @msg: �ܹ������˶���ms
 * @param {*}
 * @return {*}
 */
u32 Tick_Times(void);
#endif
