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

//���������ڴ��
#define SRAMIN	 	0		//AXI�ڴ��,AXI��512KB      //��Լ256K 480*272*2��LTDC���Դ�
#define SRAM12		1		//SRAM1/2�ڴ��,SRAM1+SRAM2,��256KB
#define SRAM4		2		//SRAM4�ڴ��,SRAM4��64KB
#define SRAMITCM 	3		//ITCM�ڴ��,DTCM��64 KB,�˲����ڴ��CPU��MDMA(ͨ��AHBS)���Է���!!!!


#define SRAMBANK 	4		//����֧�ֵ�SRAM����.	


//mem1�ڴ�����趨.mem1��H7�ڲ���AXI�ڴ�.
#define MEM1_BLOCK_SIZE			64  	  						//�ڴ���СΪ64�ֽ�
#define MEM1_MAX_SIZE			190*1024  						//�������ڴ� 192K,H7��AXI�ڴ��ܹ�512KB ��0x240000000+480*272*2
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//�ڴ���С
 
//mem3�ڴ�����趨.mem3��H7�ڲ���SRAM1+SRAM2�ڴ�
#define MEM2_BLOCK_SIZE			64  	  						//�ڴ���СΪ64�ֽ�
#define MEM2_MAX_SIZE			240 *1024  						//�������ڴ�240K,H7��SRAM1+SRAM2��256KB
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	//�ڴ���С
		 
//mem4�ڴ�����趨.mem4��H7�ڲ���SRAM4�ڴ�
#define MEM3_BLOCK_SIZE			64  	  						//�ڴ���СΪ64�ֽ�
#define MEM3_MAX_SIZE			60 *1024  						//�������ڴ�60K,H7��SRAM4��64KB
#define MEM3_ALLOC_TABLE_SIZE	MEM3_MAX_SIZE/MEM3_BLOCK_SIZE 	//�ڴ���С

		 
//mem6�ڴ�����趨.mem6��H7�ڲ���ITCM�ڴ�,�˲����ڴ��CPU��MDMA���Է���!!!!!!
#define MEM4_BLOCK_SIZE			64  	  						//�ڴ���СΪ64�ֽ�
#define MEM4_MAX_SIZE			60 *1024  						//�������ڴ�60K,H7��ITCM��64KB
#define MEM4_ALLOC_TABLE_SIZE	MEM4_MAX_SIZE/MEM4_BLOCK_SIZE 	//�ڴ���С
		 


//�ڴ���������
struct _m_mallco_dev
{
	void (*init)(u8);					//��ʼ��
	u16 (*perused)(u8);		  	    	//�ڴ�ʹ����
	u8 	*membase[SRAMBANK];				//�ڴ�� ����SRAMBANK��������ڴ�
	u32 *memmap[SRAMBANK]; 				//�ڴ����״̬��
	u8  memrdy[SRAMBANK]; 				//�ڴ�����Ƿ����
};
extern struct _m_mallco_dev mallco_dev;	 //��mallco.c���涨��

void mymemset(void *s,u8 c,u32 count);	//�����ڴ�
void mymemcpy(void *des,void *src,u32 n);//�����ڴ�     
void my_mem_init(u8 memx);				//�ڴ�����ʼ������(��/�ڲ�����)
u32 my_mem_malloc(u8 memx,u32 size);	//�ڴ����(�ڲ�����)
u8 my_mem_free(u8 memx,u32 offset);		//�ڴ��ͷ�(�ڲ�����)
u16 my_mem_perused(u8 memx) ;			//����ڴ�ʹ����(��/�ڲ�����) 
////////////////////////////////////////////////////////////////////////////////
//�û����ú���
void myfree(u8 memx,void *ptr);  			//�ڴ��ͷ�(�ⲿ����)
void *mymalloc(u8 memx,u32 size);			//�ڴ����(�ⲿ����)
void *myrealloc(u8 memx,void *ptr,u32 size);//���·����ڴ�(�ⲿ����)
#endif













