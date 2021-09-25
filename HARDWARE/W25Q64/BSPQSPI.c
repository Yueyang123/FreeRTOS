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
//等待状态标志
//flag:需要等待的标志位
//sta:需要等待的状态
//wtime:等待时间
//返回值:0,等待成功.
//	     1,等待失败.
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
 * 	hqspi.Instance 					= QUADSPI;									// QSPI外设
	hqspi.Init.ClockPrescaler 		= 1;											// 时钟分频值，将QSPI内核时钟进行 1+1 分频得到QSPI通信驱动时钟
	hqspi.Init.FifoThreshold 		= 32;											// FIFO阈值
	hqspi.Init.SampleShifting		= QSPI_SAMPLE_SHIFTING_HALFCYCLE;	// 半个CLK周期之后进行采样
	hqspi.Init.FlashSize 			= 22;											// flash大小，FLASH 中的字节数 = 2^[FSIZE+1]，核心板采用是8M字节的W25Q64，这里设置为22
	hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;			// 片选保持高电平的时间
	hqspi.Init.ClockMode 			= QSPI_CLOCK_MODE_3;						// 模式3
	hqspi.Init.FlashID 				= QSPI_FLASH_ID_1;						// 使用QSPI1
	hqspi.Init.DualFlash 			= QSPI_DUALFLASH_DISABLE;				// 禁止双闪存模式
	HAL_QSPI_Init(&hqspi); // 初始化配置
 * 
*/
//初始化QSPI接口
//返回值:0,成功;
//       1,失败;
u8 QSPI_Init(void)
{
	u32 tempreg=0;
		
	RCC->AHB3ENR|=1<<14;   		//QSPI时钟使能
	GPIO_QUADSPI_CLK_ENABLE;		// 使能 QUADSPI_CLK IO口时钟
	GPIO_QUADSPI_BK1_NCS_ENABLE;	// 使能 QUADSPI_BK1_NCS IO口时钟
	GPIO_QUADSPI_BK1_IO0_ENABLE;	// 使能 QUADSPI_BK1_IO0 IO口时钟
	GPIO_QUADSPI_BK1_IO1_ENABLE;	// 使能 QUADSPI_BK1_IO1 IO口时钟
	GPIO_QUADSPI_BK1_IO2_ENABLE;	// 使能 QUADSPI_BK1_IO2 IO口时钟
	GPIO_QUADSPI_BK1_IO3_ENABLE;	// 使能 QUADSPI_BK1_IO3 IO口时钟

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
	
	RCC->AHB3RSTR|=1<<14;		//复位QSPI
	RCC->AHB3RSTR&=~(1<<14);	//停止复位QSPI
/**
 * 
 * 	hqspi.Init.ClockPrescaler 		= 1;											// 时钟分频值，将QSPI内核时钟进行 1+1 分频得到QSPI通信驱动时钟
	hqspi.Init.FifoThreshold 		= 32;											// FIFO阈值
	hqspi.Init.SampleShifting		= QSPI_SAMPLE_SHIFTING_HALFCYCLE;	// 半个CLK周期之后进行采样
	hqspi.Init.FlashSize 			= 22;											// flash大小，FLASH 中的字节数 = 2^[FSIZE+1]，核心板采用是8M字节的W25Q64，这里设置为22
	hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;			// 片选保持高电平的时间
	hqspi.Init.ClockMode 			= QSPI_CLOCK_MODE_3;						// 模式3
	hqspi.Init.FlashID 				= QSPI_FLASH_ID_1;						// 使用QSPI1
	hqspi.Init.DualFlash 			= QSPI_DUALFLASH_DISABLE;	
 * 
 * */		
	if(QSPI_Wait_Flag(1<<5,0,0XFFFF)==0)//等待BUSY空闲
	{
		//QSPI时钟默认来自rcc_hclk3(由RCC_D1CCIPR的QSPISEL[1:0]选择)
		tempreg=(2-1)<<24;		//设置QSPI时钟为AHB时钟的1/2,即200M/2=100Mhz,10ns
		tempreg|=(4-1)<<8;		//设置FIFO阈值为4个字节(最大为31,表示32个字节)
		tempreg|=0<<7;			//选择FLASH1
		tempreg|=0<<6;			//禁止双闪存模式
		tempreg|=1<<4;			//采样移位半个周期(DDR模式下,必须设置为0)
		QUADSPI->CR=tempreg;	//设置CR寄存器
		tempreg=(23-1)<<16;		//设置FLASH大小为2^23=8MB
		tempreg|=(1-1)<<8;		//片选高电平时间为1个时钟(10*1=10ns),即手册里面的tSHSL参数
		tempreg|=1<<0;			//Mode3,空闲时CLK为高电平
		QUADSPI->DCR=tempreg;	//设置DCR寄存器
		QUADSPI->CR|=1<<0;		//使能QSPI
	}else return 1;
	return 0;
}

//QSPI发送命令
//cmd:要发送的指令
//addr:发送到的目的地址
//mode:模式,详细位定义如下:
//	mode[1:0]:指令模式;00,无指令;01,单线传输指令;10,双线传输指令;11,四线传输指令.
//	mode[3:2]:地址模式;00,无地址;01,单线传输地址;10,双线传输地址;11,四线传输地址.
//	mode[5:4]:地址长度;00,8位地址;01,16位地址;10,24位地址;11,32位地址.
//	mode[7:6]:数据模式;00,无数据;01,单线传输数据;10,双线传输数据;11,四线传输数据.
//dmcycle:空指令周期数
void QSPI_Send_CMD(u8 cmd,u32 addr,u8 mode,u8 dmcycle)
{
	u32 tempreg=0;	
	u8 status;
	if(QSPI_Wait_Flag(1<<5,0,0XFFFF)==0)	//等待BUSY空闲
	{
		tempreg=0<<31;						//禁止DDR模式
		tempreg|=0<<28;						//每次都发送指令
		tempreg|=0<<26;						//间接写模式
		tempreg|=((u32)mode>>6)<<24;		//设置数据模式
		tempreg|=(u32)dmcycle<<18;			//设置空指令周期数
		tempreg|=((u32)(mode>>4)&0X03)<<12;	//设置地址长度
		tempreg|=((u32)(mode>>2)&0X03)<<10;	//设置地址模式
		tempreg|=((u32)(mode>>0)&0X03)<<8;	//设置指令模式
		tempreg|=cmd;						//设置指令
		QUADSPI->CCR=tempreg;				//设置CCR寄存器
		if(mode&0X0C)						//有指令+地址要发送
		{
			QUADSPI->AR=addr;				//设置地址寄存器
		} 
		if((mode&0XC0)==0)					//无数据传输,等待指令发送完成
		{
			status=QSPI_Wait_Flag(1<<1,1,0XFFFF);//等待TCF,即传输完成
			if(status==0)
			{
				QUADSPI->FCR|=1<<1;			//清除TCF标志位 
			}
		}
	}	
}

//QSPI接收指定长度的数据
//buf:接收数据缓冲区首地址
//datalen:要传输的数据长度
//返回值:0,正常
//    其他,错误代码
u8 QSPI_Receive(u8* buf,u32 datalen)
{
	u32 tempreg=QUADSPI->CCR;
	u32 addrreg=QUADSPI->AR; 	
	u8 status;
	vu32 *data_reg=&QUADSPI->DR;
	QUADSPI->DLR=datalen-1;					//设置数据传输长度
	tempreg&=~(3<<26);						//清除FMODE原来的设置
	tempreg|=1<<26;							//设置FMODE为间接读取模式
	QUADSPI->FCR|=1<<1;						//清除TCF标志位
	QUADSPI->CCR=tempreg;					//回写CCR寄存器
	QUADSPI->AR=addrreg;					//回写AR寄存器,触发传输
	while(datalen)
	{
		status=QSPI_Wait_Flag(3<<1,1,0XFFFF);//等到FTF和TCF,即接收到了数据
		if(status==0)						//等待成功
		{
			*buf++=*(vu8 *)data_reg;
			datalen--;
		}else break;
	}
	if(status==0)
	{
		QUADSPI->CR|=1<<2;							//终止传输 
		status=QSPI_Wait_Flag(1<<1,1,0XFFFF);		//等待TCF,即数据传输完成
		if(status==0)
		{
			QUADSPI->FCR|=1<<1;						//清除TCF标志位 
			status=QSPI_Wait_Flag(1<<5,0,0XFFFF);	//等待BUSY位清零
		}
	}
	return status;
} 

//QSPI发送指定长度的数据
//buf:发送数据缓冲区首地址
//datalen:要传输的数据长度
//返回值:0,正常
//    其他,错误代码
u8 QSPI_Transmit(u8* buf,u32 datalen)
{
	u32 tempreg=QUADSPI->CCR;
	u32 addrreg=QUADSPI->AR; 
	u8 status;
	vu32 *data_reg=&QUADSPI->DR;
	QUADSPI->DLR=datalen-1;					//设置数据传输长度
	tempreg&=~(3<<26);						//清除FMODE原来的设置
	tempreg|=0<<26;							//设置FMODE为间接写入模式
	QUADSPI->FCR|=1<<1;						//清除TCF标志位
	QUADSPI->CCR=tempreg;					//回写CCR寄存器 
	while(datalen)
	{
		status=QSPI_Wait_Flag(1<<2,1,0XFFFF);//等到FTF
		if(status!=0)						//等待成功
		{
			break;
		}
		*(vu8 *)data_reg=*buf++;
		datalen--;
	}
	if(status==0)
	{
		QUADSPI->CR|=1<<2;							//终止传输 
		status=QSPI_Wait_Flag(1<<1,1,0XFFFF);		//等待TCF,即数据传输完成
		if(status==0)
		{
			QUADSPI->FCR|=1<<1;						//清除TCF标志位 
			status=QSPI_Wait_Flag(1<<5,0,0XFFFF);	//等待BUSY位清零
		}
	}
	return status;
}

