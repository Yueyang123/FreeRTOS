/*
 * @Descripttion: ADC采集，可以采集内部温度，以及VBAT电压
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


#define ADC1_CH19	  		19			//通道19  	      	    
#define ADC3_TEMPSENSOR	  	18			//通道18,温度传感器通道  	      	    
#define ADC3_BATTARY	  	17	   	    //通道17,VBAT/4 

void Adc_Init(void); 					//ADC初始化
u16  Get_Adc(u8 ch); 					//获得某个通道值 
u16 Get_Adc_Average(u8 ch,u8 times);	//得到某个通道给定次数采样的平均值  
void Adc3_Init(void); 					//ADC3初始化
u16  Get_Adc3(u8 ch); 					//获得ADC3某个通道值 
u16 Get_Adc3_Average(u8 ch,u8 times);	//得到ADC3某个通道给定次数采样的平均值  
short Get_Temprate(void);				//获取当前温度
float Get_Battery(void);
#endif 


