/*
 * @Descripttion: 
 * @Version: V1.0
 * @Author: YURI
 * @Email: 1700695611@qq.com
 * @Website: www.heredifferent.com.cn
 * @Date: 2021-08-21 07:04:37
 * @LastEditors: YURI
 * @LastEditTime: 2021-08-21 07:07:07
 */
#ifndef __RTC_H
#define __RTC_H	 
#include "sys.h" 
		
u8 RTC_Init(void);										//RTC��ʼ��
u8 RTC_Wait_Synchro(void);								//�ȴ�ͬ��
u8 RTC_Init_Mode(void);									//�����ʼ��ģʽ
void RTC_Write_BKR(u32 BKRx,u32 data);					//д������SRAM
u32 RTC_Read_BKR(u32 BKRx);								//��������SRAM
u8 RTC_DEC2BCD(u8 val);									//ʮ����ת��ΪBCD��
u8 RTC_BCD2DEC(u8 val);									//BCD��ת��Ϊʮ��������
u8 RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);			//RTCʱ������
u8 RTC_Set_Date(u8 year,u8 month,u8 date,u8 week); 		//RTC��������
void RTC_Get_Time(u8 *hour,u8 *min,u8 *sec,u8 *ampm);	//��ȡRTCʱ��
void RTC_Get_Date(u8 *year,u8 *month,u8 *date,u8 *week);//��ȡRTC����
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec);		//��������ʱ��(����������,24Сʱ��)
void RTC_Set_WakeUp(u8 wksel,u16 cnt);					//�����Ի��Ѷ�ʱ������
u8 RTC_Get_Week(u16 year,u8 month,u8 day);				//���������������,���㵱���������ڼ�
#endif

















