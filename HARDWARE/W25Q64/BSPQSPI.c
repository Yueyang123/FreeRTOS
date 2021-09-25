/*
 * @Descripttion: 
 * @Version: V1.0
 * @Author: YURI
 * @Email: 1700695611@qq.com
 * @Website: www.heredifferent.com.cn
 * @Date: 2021-08-21 16:58:17
 * @LastEditors: YURI
 * @LastEditTime: 2021-08-22 12:35:22
 */
#include "BSPQSPI.h"
//�ȴ�״̬��־
//flag:��Ҫ�ȴ��ı�־λ
//sta:��Ҫ�ȴ���״̬
//wtime:�ȴ�ʱ��
//����ֵ:0,�ȴ��ɹ�.
//	     1,�ȴ�ʧ��.
u8 QSPI_Wait_Flag(u32 flag,u8 sta,u32 wtime)
{
	u8 flagsta=0;
	while(wtime)
	{
		flagsta=(QUADSPI->SR&flag)?1:0; 
		if(flagsta==sta)break;
		wtime--;
	}
	if(wtime)return 0;
	else return 1;
}

/**
 * 	hqspi.Instance 					= QUADSPI;									// QSPI����
	hqspi.Init.ClockPrescaler 		= 1;											// ʱ�ӷ�Ƶֵ����QSPI�ں�ʱ�ӽ��� 1+1 ��Ƶ�õ�QSPIͨ������ʱ��
	hqspi.Init.FifoThreshold 		= 32;											// FIFO��ֵ
	hqspi.Init.SampleShifting		= QSPI_SAMPLE_SHIFTING_HALFCYCLE;	// ���CLK����֮����в���
	hqspi.Init.FlashSize 			= 22;											// flash��С��FLASH �е��ֽ��� = 2^[FSIZE+1]�����İ������8M�ֽڵ�W25Q64����������Ϊ22
	hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;			// Ƭѡ���ָߵ�ƽ��ʱ��
	hqspi.Init.ClockMode 			= QSPI_CLOCK_MODE_3;						// ģʽ3
	hqspi.Init.FlashID 				= QSPI_FLASH_ID_1;						// ʹ��QSPI1
	hqspi.Init.DualFlash 			= QSPI_DUALFLASH_DISABLE;				// ��ֹ˫����ģʽ
	HAL_QSPI_Init(&hqspi); // ��ʼ������
 * 
*/
//��ʼ��QSPI�ӿ�
//����ֵ:0,�ɹ�;
//       1,ʧ��;
u8 QSPI_Init(void)
{
	u32 tempreg=0;
		
	RCC->AHB3ENR|=1<<14;   		//QSPIʱ��ʹ��
	GPIO_QUADSPI_CLK_ENABLE;		// ʹ�� QUADSPI_CLK IO��ʱ��
	GPIO_QUADSPI_BK1_NCS_ENABLE;	// ʹ�� QUADSPI_BK1_NCS IO��ʱ��
	GPIO_QUADSPI_BK1_IO0_ENABLE;	// ʹ�� QUADSPI_BK1_IO0 IO��ʱ��
	GPIO_QUADSPI_BK1_IO1_ENABLE;	// ʹ�� QUADSPI_BK1_IO1 IO��ʱ��
	GPIO_QUADSPI_BK1_IO2_ENABLE;	// ʹ�� QUADSPI_BK1_IO2 IO��ʱ��
	GPIO_QUADSPI_BK1_IO3_ENABLE;	// ʹ�� QUADSPI_BK1_IO3 IO��ʱ��

	GPIO_Set(QUADSPI_CLK_PORT,QUADSPI_CLK_PIN,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_NONE);	
	GPIO_Set(QUADSPI_BK1_NCS_PORT,QUADSPI_BK1_NCS_PIN,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_NONE);	
	GPIO_Set(QUADSPI_BK1_IO0_PORT,QUADSPI_BK1_IO0_PIN,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_NONE);	
	GPIO_Set(QUADSPI_BK1_IO1_PORT,QUADSPI_BK1_IO1_PIN,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_NONE);	
	GPIO_Set(QUADSPI_BK1_IO2_PORT,QUADSPI_BK1_IO2_PIN,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_NONE);	
	GPIO_Set(QUADSPI_BK1_IO3_PORT,QUADSPI_BK1_IO3_PIN,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_HIGH,GPIO_PUPD_NONE);	

    GPIO_AF_Set(QUADSPI_CLK_PORT,2,QUADSPI_CLK_AF);
 	GPIO_AF_Set(QUADSPI_BK1_NCS_PORT,6,QUADSPI_BK1_NCS_AF);	
 	GPIO_AF_Set(QUADSPI_BK1_IO0_PORT,11,QUADSPI_BK1_IO0_AF);		
 	GPIO_AF_Set(QUADSPI_BK1_IO1_PORT,12,QUADSPI_BK1_IO1_AF);	
 	GPIO_AF_Set(QUADSPI_BK1_IO2_PORT,2,QUADSPI_BK1_IO2_AF);	    
 	GPIO_AF_Set(QUADSPI_BK1_IO3_PORT,13,QUADSPI_BK1_IO3_AF);	
	
	RCC->AHB3RSTR|=1<<14;		//��λQSPI
	RCC->AHB3RSTR&=~(1<<14);	//ֹͣ��λQSPI
/**
 * 
 * 	hqspi.Init.ClockPrescaler 		= 1;											// ʱ�ӷ�Ƶֵ����QSPI�ں�ʱ�ӽ��� 1+1 ��Ƶ�õ�QSPIͨ������ʱ��
	hqspi.Init.FifoThreshold 		= 32;											// FIFO��ֵ
	hqspi.Init.SampleShifting		= QSPI_SAMPLE_SHIFTING_HALFCYCLE;	// ���CLK����֮����в���
	hqspi.Init.FlashSize 			= 22;											// flash��С��FLASH �е��ֽ��� = 2^[FSIZE+1]�����İ������8M�ֽڵ�W25Q64����������Ϊ22
	hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;			// Ƭѡ���ָߵ�ƽ��ʱ��
	hqspi.Init.ClockMode 			= QSPI_CLOCK_MODE_3;						// ģʽ3
	hqspi.Init.FlashID 				= QSPI_FLASH_ID_1;						// ʹ��QSPI1
	hqspi.Init.DualFlash 			= QSPI_DUALFLASH_DISABLE;	
 * 
 * */		
	if(QSPI_Wait_Flag(1<<5,0,0XFFFF)==0)//�ȴ�BUSY����
	{
		//QSPIʱ��Ĭ������rcc_hclk3(��RCC_D1CCIPR��QSPISEL[1:0]ѡ��)
		tempreg=(2-1)<<24;		//����QSPIʱ��ΪAHBʱ�ӵ�1/2,��200M/2=100Mhz,10ns
		tempreg|=(4-1)<<8;		//����FIFO��ֵΪ4���ֽ�(���Ϊ31,��ʾ32���ֽ�)
		tempreg|=0<<7;			//ѡ��FLASH1
		tempreg|=0<<6;			//��ֹ˫����ģʽ
		tempreg|=1<<4;			//������λ�������(DDRģʽ��,��������Ϊ0)
		QUADSPI->CR=tempreg;	//����CR�Ĵ���
		tempreg=(23-1)<<16;		//����FLASH��СΪ2^23=8MB
		tempreg|=(1-1)<<8;		//Ƭѡ�ߵ�ƽʱ��Ϊ1��ʱ��(10*1=10ns),���ֲ������tSHSL����
		tempreg|=1<<0;			//Mode3,����ʱCLKΪ�ߵ�ƽ
		QUADSPI->DCR=tempreg;	//����DCR�Ĵ���
		QUADSPI->CR|=1<<0;		//ʹ��QSPI
	}else return 1;
	return 0;
}

//QSPI��������
//cmd:Ҫ���͵�ָ��
//addr:���͵���Ŀ�ĵ�ַ
//mode:ģʽ,��ϸλ��������:
//	mode[1:0]:ָ��ģʽ;00,��ָ��;01,���ߴ���ָ��;10,˫�ߴ���ָ��;11,���ߴ���ָ��.
//	mode[3:2]:��ַģʽ;00,�޵�ַ;01,���ߴ����ַ;10,˫�ߴ����ַ;11,���ߴ����ַ.
//	mode[5:4]:��ַ����;00,8λ��ַ;01,16λ��ַ;10,24λ��ַ;11,32λ��ַ.
//	mode[7:6]:����ģʽ;00,������;01,���ߴ�������;10,˫�ߴ�������;11,���ߴ�������.
//dmcycle:��ָ��������
void QSPI_Send_CMD(u8 cmd,u32 addr,u8 mode,u8 dmcycle)
{
	u32 tempreg=0;	
	u8 status;
	if(QSPI_Wait_Flag(1<<5,0,0XFFFF)==0)	//�ȴ�BUSY����
	{
		tempreg=0<<31;						//��ֹDDRģʽ
		tempreg|=0<<28;						//ÿ�ζ�����ָ��
		tempreg|=0<<26;						//���дģʽ
		tempreg|=((u32)mode>>6)<<24;		//��������ģʽ
		tempreg|=(u32)dmcycle<<18;			//���ÿ�ָ��������
		tempreg|=((u32)(mode>>4)&0X03)<<12;	//���õ�ַ����
		tempreg|=((u32)(mode>>2)&0X03)<<10;	//���õ�ַģʽ
		tempreg|=((u32)(mode>>0)&0X03)<<8;	//����ָ��ģʽ
		tempreg|=cmd;						//����ָ��
		QUADSPI->CCR=tempreg;				//����CCR�Ĵ���
		if(mode&0X0C)						//��ָ��+��ַҪ����
		{
			QUADSPI->AR=addr;				//���õ�ַ�Ĵ���
		} 
		if((mode&0XC0)==0)					//�����ݴ���,�ȴ�ָ������
		{
			status=QSPI_Wait_Flag(1<<1,1,0XFFFF);//�ȴ�TCF,���������
			if(status==0)
			{
				QUADSPI->FCR|=1<<1;			//���TCF��־λ 
			}
		}
	}	
}

//QSPI����ָ�����ȵ�����
//buf:�������ݻ������׵�ַ
//datalen:Ҫ��������ݳ���
//����ֵ:0,����
//    ����,�������
u8 QSPI_Receive(u8* buf,u32 datalen)
{
	u32 tempreg=QUADSPI->CCR;
	u32 addrreg=QUADSPI->AR; 	
	u8 status;
	vu32 *data_reg=&QUADSPI->DR;
	QUADSPI->DLR=datalen-1;					//�������ݴ��䳤��
	tempreg&=~(3<<26);						//���FMODEԭ��������
	tempreg|=1<<26;							//����FMODEΪ��Ӷ�ȡģʽ
	QUADSPI->FCR|=1<<1;						//���TCF��־λ
	QUADSPI->CCR=tempreg;					//��дCCR�Ĵ���
	QUADSPI->AR=addrreg;					//��дAR�Ĵ���,��������
	while(datalen)
	{
		status=QSPI_Wait_Flag(3<<1,1,0XFFFF);//�ȵ�FTF��TCF,�����յ�������
		if(status==0)						//�ȴ��ɹ�
		{
			*buf++=*(vu8 *)data_reg;
			datalen--;
		}else break;
	}
	if(status==0)
	{
		QUADSPI->CR|=1<<2;							//��ֹ���� 
		status=QSPI_Wait_Flag(1<<1,1,0XFFFF);		//�ȴ�TCF,�����ݴ������
		if(status==0)
		{
			QUADSPI->FCR|=1<<1;						//���TCF��־λ 
			status=QSPI_Wait_Flag(1<<5,0,0XFFFF);	//�ȴ�BUSYλ����
		}
	}
	return status;
} 

//QSPI����ָ�����ȵ�����
//buf:�������ݻ������׵�ַ
//datalen:Ҫ��������ݳ���
//����ֵ:0,����
//    ����,�������
u8 QSPI_Transmit(u8* buf,u32 datalen)
{
	u32 tempreg=QUADSPI->CCR;
	u32 addrreg=QUADSPI->AR; 
	u8 status;
	vu32 *data_reg=&QUADSPI->DR;
	QUADSPI->DLR=datalen-1;					//�������ݴ��䳤��
	tempreg&=~(3<<26);						//���FMODEԭ��������
	tempreg|=0<<26;							//����FMODEΪ���д��ģʽ
	QUADSPI->FCR|=1<<1;						//���TCF��־λ
	QUADSPI->CCR=tempreg;					//��дCCR�Ĵ��� 
	while(datalen)
	{
		status=QSPI_Wait_Flag(1<<2,1,0XFFFF);//�ȵ�FTF
		if(status!=0)						//�ȴ��ɹ�
		{
			break;
		}
		*(vu8 *)data_reg=*buf++;
		datalen--;
	}
	if(status==0)
	{
		QUADSPI->CR|=1<<2;							//��ֹ���� 
		status=QSPI_Wait_Flag(1<<1,1,0XFFFF);		//�ȴ�TCF,�����ݴ������
		if(status==0)
		{
			QUADSPI->FCR|=1<<1;						//���TCF��־λ 
			status=QSPI_Wait_Flag(1<<5,0,0XFFFF);	//�ȴ�BUSYλ����
		}
	}
	return status;
}

