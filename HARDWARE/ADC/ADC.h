/*
 * @Descripttion: ADC�ɼ������Բɼ��ڲ��¶ȣ��Լ�VBAT��ѹ
 * @Version: V1.0
 * @Author: YURI
 * @Email: 1700695611@qq.com
 * @Website: www.heredifferent.com.cn
 * @Date: 2021-08-21 07:42:47
 * @LastEditors: YURI
 * @LastEditTime: 2021-08-21 08:17:22
 */
#ifndef __ADC_H
#define __ADC_H	
#include "sys.h" 


#define ADC1_CH19	  		19			//ͨ��19  	      	    
#define ADC3_TEMPSENSOR	  	18			//ͨ��18,�¶ȴ�����ͨ��  	      	    
#define ADC3_BATTARY	  	17	   	    //ͨ��17,VBAT/4 

void Adc_Init(void); 					//ADC��ʼ��
u16  Get_Adc(u8 ch); 					//���ĳ��ͨ��ֵ 
u16 Get_Adc_Average(u8 ch,u8 times);	//�õ�ĳ��ͨ����������������ƽ��ֵ  
void Adc3_Init(void); 					//ADC3��ʼ��
u16  Get_Adc3(u8 ch); 					//���ADC3ĳ��ͨ��ֵ 
u16 Get_Adc3_Average(u8 ch,u8 times);	//�õ�ADC3ĳ��ͨ����������������ƽ��ֵ  
short Get_Temprate(void);				//��ȡ��ǰ�¶�
float Get_Battery(void);
#endif 


