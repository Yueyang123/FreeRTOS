/*
 * @Descripttion: 
 * @Version: V1.0
 * @Author: YURI
 * @Email: 1700695611@qq.com
 * @Website: www.heredifferent.com.cn
 * @Date: 2021-08-22 13:35:11
 * @LastEditors: YURI
 * @LastEditTime: 2021-08-22 13:44:35
 */
#ifndef __MALLOC_H
#define __MALLOC_H
#include "sys.h" 

 
#ifndef NULL
#define NULL 0
#endif

//定义六个内存池
#define SRAMIN	 	0		//AXI内存池,AXI共512KB      //大约256K 480*272*2给LTDC做显存
#define SRAM12		1		//SRAM1/2内存池,SRAM1+SRAM2,共256KB
#define SRAM4		2		//SRAM4内存池,SRAM4共64KB
#define SRAMITCM 	3		//ITCM内存池,DTCM共64 KB,此部分内存仅CPU和MDMA(通过AHBS)可以访问!!!!


#define SRAMBANK 	4		//定义支持的SRAM块数.	


//mem1内存参数设定.mem1是H7内部的AXI内存.
#define MEM1_BLOCK_SIZE			64  	  						//内存块大小为64字节
#define MEM1_MAX_SIZE			190*1024  						//最大管理内存 192K,H7的AXI内存总共512KB 从0x240000000+480*272*2
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//内存表大小
 
//mem3内存参数设定.mem3是H7内部的SRAM1+SRAM2内存
#define MEM2_BLOCK_SIZE			64  	  						//内存块大小为64字节
#define MEM2_MAX_SIZE			240 *1024  						//最大管理内存240K,H7的SRAM1+SRAM2共256KB
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	//内存表大小
		 
//mem4内存参数设定.mem4是H7内部的SRAM4内存
#define MEM3_BLOCK_SIZE			64  	  						//内存块大小为64字节
#define MEM3_MAX_SIZE			60 *1024  						//最大管理内存60K,H7的SRAM4共64KB
#define MEM3_ALLOC_TABLE_SIZE	MEM3_MAX_SIZE/MEM3_BLOCK_SIZE 	//内存表大小

		 
//mem6内存参数设定.mem6是H7内部的ITCM内存,此部分内存仅CPU和MDMA可以访问!!!!!!
#define MEM4_BLOCK_SIZE			64  	  						//内存块大小为64字节
#define MEM4_MAX_SIZE			60 *1024  						//最大管理内存60K,H7的ITCM共64KB
#define MEM4_ALLOC_TABLE_SIZE	MEM4_MAX_SIZE/MEM4_BLOCK_SIZE 	//内存表大小
		 


//内存管理控制器
struct _m_mallco_dev
{
	void (*init)(u8);					//初始化
	u16 (*perused)(u8);		  	    	//内存使用率
	u8 	*membase[SRAMBANK];				//内存池 管理SRAMBANK个区域的内存
	u32 *memmap[SRAMBANK]; 				//内存管理状态表
	u8  memrdy[SRAMBANK]; 				//内存管理是否就绪
};
extern struct _m_mallco_dev mallco_dev;	 //在mallco.c里面定义

void mymemset(void *s,u8 c,u32 count);	//设置内存
void mymemcpy(void *des,void *src,u32 n);//复制内存     
void my_mem_init(u8 memx);				//内存管理初始化函数(外/内部调用)
u32 my_mem_malloc(u8 memx,u32 size);	//内存分配(内部调用)
u8 my_mem_free(u8 memx,u32 offset);		//内存释放(内部调用)
u16 my_mem_perused(u8 memx) ;			//获得内存使用率(外/内部调用) 
////////////////////////////////////////////////////////////////////////////////
//用户调用函数
void myfree(u8 memx,void *ptr);  			//内存释放(外部调用)
void *mymalloc(u8 memx,u32 size);			//内存分配(外部调用)
void *myrealloc(u8 memx,void *ptr,u32 size);//重新分配内存(外部调用)
#endif













