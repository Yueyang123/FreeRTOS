/*
 * @Descripttion: W25Q64驱动，可以使用QSPI,与硬件SPI两种模式
 * @Version: V1.0
 * @Author: YURI
 * @Email: 1700695611@qq.com
 * @Website: www.heredifferent.com.cn
 * @Date: 2021-08-21 10:29:39
 * @LastEditors: YURI
 * @LastEditTime: 2021-08-22 12:55:14
 */
#include "W25Q64.h"
#include "delay.h"
#include "SPI.h"
#include "BSPQSPI.h"
#include "usart.h"




void W25Q64_Init(void)
{
    uint32_t	Device_ID;	// 器件ID
#if  USE_W25Q64_NO1 
    SPIX_HARD_Init(SPI6);
    Device_ID =  SPI_W25Qxx_ReadID();
    
	if( Device_ID == SPI_W25Qxx_FLASH_ID )		// 进行匹配
	{
		printf ("W25Q64 OK,flash ID:%X\r\n",Device_ID);		// 初始化成功	
	}
	else
	{
		printf ("W25Q64 ERROR!!!!!  ID:%X\r\n",Device_ID);	// 初始化失败	
	}
#endif

#if USE_W25Q64_NO2
    QSPI_Init();
    Device_ID =  QSPI_W25Qxx_ReadID();
	if( Device_ID == SPI_W25Qxx_FLASH_ID )		// 进行匹配
	{
		printf ("QSPI W25Q64 OK,flash ID:%X\r\n",Device_ID);		// 初始化成功	
	}
	else
	{
		printf ("QSPI W25Q64 ERROR!!!!!  ID:%X\r\n",Device_ID);	// 初始化失败	
	}
#endif
}


#if  USE_W25Q64_NO1 
/*************************************************************************************************
*	函 数 名: SPI_W25Qxx_ReadID
*	入口参数: 无
*	返 回 值: W25Qxx_ID - 读取到的器件ID，W25Qxx_ERROR_INIT - 通信、初始化错误
*	函数功能: 读取器件ID
*	说    明: 无	
**************************************************************************************************/

uint32_t SPI_W25Qxx_ReadID(void)
{
   uint8_t	SPI_TX_Buff[1];      // 要发送的指令
   uint8_t	SPI_RX_Buff[4];		// 存储SPI读到的数据
   uint32_t W25Qxx_ID;           // ID

	SPI_TX_Buff[0] = SPI_W25Qxx_CMD_JedecID;		// 发送读JedecID命令 
	if(BSP_SPI_TransmitReceive(SPI6, (uint8_t *)SPI_TX_Buff, (uint8_t *)SPI_RX_Buff, 4, 1000) != 0)	
	{
		return ERROR;
	}	
   
// 需要注意的，使用 SPI_TransmitReceive() 进行通信，发送第一个读ID命令时，就会立即读取一个字节
// 因此，要想得到实际的数据需要偏移一个字节
   W25Qxx_ID = (SPI_RX_Buff[1] << 16) | (SPI_RX_Buff[2] << 8) | SPI_RX_Buff[3];    // 将读到的数据组合为ID
   return W25Qxx_ID; // 返回ID值
}

/*************************************************************************************************
*	函 数 名: SPI_W25Qxx_WaitForWriteEnd  
*	入口参数: 无
*	返 回 值: SPI_W25Qxx_OK - 通信正常结束，W25Qxx_ERROR_Wait - 等待错误
*	函数功能: 等待W25Qxx操做完毕
*	说    明: 擦除/写入数据时，需要调用该函数等待操做完成	
**************************************************************************************************/

int8_t SPI_W25Qxx_WaitForWriteEnd(void)
{
    uint8_t	SPI_TX_Buff[1];   // 要发送的指令
	uint8_t	SPI_RX_Buff[2];	// 存储SPI读到的数据 

   while (1)
   {
	  SPI_TX_Buff[0] = SPI_W25Qxx_CMD_ReadStatus_REG1;		// 发送读 读状态寄存器 命令 
      SPI_RX_Buff[1] = 0;     // 将存储区置零

      if(BSP_SPI_TransmitReceive(SPI6, (uint8_t *)SPI_TX_Buff, (uint8_t *)SPI_RX_Buff, 2, 1000) != 0)	
      {
         return W25Qxx_ERROR_Wait;
      }	  
      // 读状态寄存器1的第0位（只读），Busy标志位，当正在擦除/写入数据/写命令时会被置1
      if( (SPI_RX_Buff[1] & SPI_W25Qxx_CMD_Status_REG1_BUSY)  != SET )  // 比较标志位
      {
         return SPI_W25Qxx_OK;
      }   
   }
}

/*************************************************************************************************
*	函 数 名: SPI_W25Qxx_WriteEnable
*	入口参数: 无
*	返 回 值: QSPI_W25Qxx_OK - 写使能成功，W25Qxx_ERROR_WriteEnable - 写使能失败
*	函数功能: 发送写使能命令
*	说    明: 在擦除或者写入数据之前，需要调用该函数进行写使能
**************************************************************************************************/

int8_t SPI_W25Qxx_WriteEnable(void)
{
   uint8_t	SPI_TX_Buff[1];   // 要发送的指令
	uint8_t	SPI_RX_Buff[1];	// 存储SPI读到的数据，此处用不到 

	SPI_TX_Buff[0] = SPI_W25Qxx_CMD_WriteEnable;		// 发送读 写使能 命令 

	if(BSP_SPI_TransmitReceive(SPI6, (uint8_t *)SPI_TX_Buff, (uint8_t *)SPI_RX_Buff, 1, 1000) != 0)	
	{
		return W25Qxx_ERROR_WriteEnable;
	}	
    return SPI_W25Qxx_OK;
}

/*************************************************************************************************
*
*	函 数 名: SPI_W25Qxx_SectorErase
*
*	入口参数: SectorAddress - 要擦除的地址
*
*	返 回 值: SPI_W25Qxx_OK - 擦除成功
*			    W25Qxx_ERROR_Erase - 擦除失败
*
*	函数功能: 进行扇区擦除操作，每次擦除4K字节
*
*	说    明: 1.按照 W25Q64JV 数据手册给出的擦除参考时间，典型值为 45ms，最大值为400ms
*				 2.实际的擦除速度可能大于45ms，也可能小于45ms
*				 3.flash使用的时间越长，擦除所需时间也会越长
*
**************************************************************************************************/

int8_t SPI_W25Qxx_SectorErase(uint32_t SectorAddress)	
{
   uint8_t	SPI_TX_Buff[4];   // 要发送的指令
	uint8_t	SPI_RX_Buff[1];	// 存储SPI读到的数据，此处用不到 

   SPI_W25Qxx_WriteEnable();  // 写使能

   SPI_TX_Buff[0] = SPI_W25Qxx_CMD_SectorErase;          // 4K扇区擦除指令
   SPI_TX_Buff[1] = ( SectorAddress & 0xFF0000 ) >> 16;  // 发送要擦除的地址
   SPI_TX_Buff[2] = ( SectorAddress & 0xFF00 ) >> 8;;
   SPI_TX_Buff[3] =   SectorAddress & 0xFF;

	if(BSP_SPI_TransmitReceive(SPI6, (uint8_t *)SPI_TX_Buff, (uint8_t *)SPI_RX_Buff, 4, 1000) != 0)	
	{
		return W25Qxx_ERROR_Erase;
	}	 
   SPI_W25Qxx_WaitForWriteEnd(); // 等待擦除结束

	return SPI_W25Qxx_OK; // 擦除成功
}

/*************************************************************************************************
*
*	函 数 名: SPI_W25Qxx_BlockErase_32K
*
*	入口参数: SectorAddress - 要擦除的地址
*
*	返 回 值: SPI_W25Qxx_OK - 擦除成功
*			    W25Qxx_ERROR_Erase - 擦除失败
*
*	函数功能: 进行块擦除操作，每次擦除32K字节
*
*	说    明: 1.按照 W25Q64JV 数据手册给出的擦除参考时间，典型值为 120ms，最大值为1600ms
*				 2.实际的擦除速度可能大于120ms，也可能小于120ms
*				 3.flash使用的时间越长，擦除所需时间也会越长
*
*************************************************************************************************/

int8_t SPI_W25Qxx_BlockErase_32K(uint32_t SectorAddress)	
{
   uint8_t	SPI_TX_Buff[4];   // 要发送的指令
	uint8_t	SPI_RX_Buff[1];	// 存储SPI读到的数据，此处用不到 

   SPI_W25Qxx_WriteEnable();  // 写使能

   SPI_TX_Buff[0] = SPI_W25Qxx_CMD_BlockErase_32K;       // 32K块擦除指令
   SPI_TX_Buff[1] = ( SectorAddress & 0xFF0000 ) >> 16;  // 发送要擦除的地址
   SPI_TX_Buff[2] = ( SectorAddress & 0xFF00 ) >> 8;;
   SPI_TX_Buff[3] =   SectorAddress & 0xFF;
	if(BSP_SPI_TransmitReceive(SPI6, (uint8_t *)SPI_TX_Buff, (uint8_t *)SPI_RX_Buff, 4, 1000) != 0)	
	{
		return W25Qxx_ERROR_Erase;
	}	 
    SPI_W25Qxx_WaitForWriteEnd(); // 等待擦除完毕

	return SPI_W25Qxx_OK; // 擦除成功
}

/*************************************************************************************************
*
*	函 数 名: SPI_W25Qxx_BlockErase_64K
*
*	入口参数: SectorAddress - 要擦除的地址
*
*	返 回 值: SPI_W25Qxx_OK - 擦除成功
*			    W25Qxx_ERROR_Erase - 擦除失败
*
*	函数功能: 进行块擦除操作，每次擦除64K字节
*
*	说    明: 1.按照 W25Q64JV 数据手册给出的擦除参考时间，典型值为 150ms，最大值为2000ms
*				 2.实际的擦除速度可能大于150ms，也可能小于150ms
*				 3.flash使用的时间越长，擦除所需时间也会越长
*				 4.实际使用建议使用64K擦除，擦除的时间最快
*
**************************************************************************************************/

int8_t SPI_W25Qxx_BlockErase_64K(uint32_t SectorAddress)	
{
   uint8_t	SPI_TX_Buff[4];   // 要发送的指令
	uint8_t	SPI_RX_Buff[1];	// 存储SPI读到的数据，此处用不到 

   SPI_W25Qxx_WriteEnable();  // 写使能   

   SPI_TX_Buff[0] = SPI_W25Qxx_CMD_BlockErase_64K;       // 64K块擦除指令
   SPI_TX_Buff[1] = ( SectorAddress & 0xFF0000 ) >> 16;  // 发送要擦除的地址
   SPI_TX_Buff[2] = ( SectorAddress & 0xFF00 ) >> 8;;
   SPI_TX_Buff[3] =   SectorAddress & 0xFF;

	if(BSP_SPI_TransmitReceive(SPI6, (uint8_t *)SPI_TX_Buff, (uint8_t *)SPI_RX_Buff, 4, 1000) != 0)	
	{
		return W25Qxx_ERROR_Erase;
	} 
   SPI_W25Qxx_WaitForWriteEnd();    // 等待擦除结束

	return SPI_W25Qxx_OK; // 擦除成功
}

/*************************************************************************************************
*
*	函 数 名: SPI_W25Qxx_ChipErase
*
*	入口参数: 无
*
*	返 回 值: SPI_W25Qxx_OK - 擦除成功
*			    W25Qxx_ERROR_Erase - 擦除失败
*
*	函数功能: 进行整片擦除操作
*
*	说    明: 1.按照 W25Q64JV 数据手册给出的擦除参考时间，典型值为 20s，最大值为100s
*				 2.实际的擦除速度可能大于20s，也可能小于20s
*				 3.flash使用的时间越长，擦除所需时间也会越长
*
*************************************************************************************************/

int8_t SPI_W25Qxx_ChipErase(void)	
{
   uint8_t	SPI_TX_Buff[1];   // 要发送的指令
   uint8_t	SPI_RX_Buff[1];	// 存储SPI读到的数据，此处用不到 

   SPI_W25Qxx_WriteEnable();  // 写使能

   SPI_TX_Buff[0] = SPI_W25Qxx_CMD_ChipErase;   // 全片擦除指令

	if(BSP_SPI_TransmitReceive(SPI6, (uint8_t *)SPI_TX_Buff, (uint8_t *)SPI_RX_Buff, 1, 1000) != 0)	
	{
		return W25Qxx_ERROR_Erase;
	}	
   
   SPI_W25Qxx_WaitForWriteEnd();

	return SPI_W25Qxx_OK; // 擦除成功
}

/**********************************************************************************************************
*
*	函 数 名: SPI_W25Qxx_WritePage
*
*	入口参数: pBuffer 		 - 要写入的数据
*				 WriteAddr 		 - 要写入 W25Qxx 的地址
*				 NumByteToWrite - 数据长度，最大只能256字节
*
*	返 回 值: SPI_W25Qxx_OK 		     - 写数据成功
*				 W25Qxx_ERROR_TRANSMIT	  - 传输失败
*
*	函数功能: 按页写入，最大只能256字节，在数据写入之前，请务必完成擦除操作
*
*	说    明: 1.Flash的写入时间是限定的
*				 2.按照 W25Q64JV 数据手册给出的 页(256字节) 写入参考时间，典型值为 0.4ms，最大值为3ms
*				 3.实际的写入速度可能大于0.4ms，也可能小于0.4ms
*				 4.Flash使用的时间越长，写入所需时间也会越长
*				 5.在数据写入之前，请务必完成擦除操作
*
***********************************************************************************************************/

int8_t SPI_W25Qxx_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint8_t	SPI_TX_Buff[260];    // 要发送的指令
    uint8_t	SPI_RX_Buff[1]; 	
	uint16_t i = 0;               // 计数变量

   SPI_W25Qxx_WriteEnable();     // 写使能

   SPI_TX_Buff[0] = W25Qxx_CMD_PageProgram;           // 页编程指令
   SPI_TX_Buff[1] = ( WriteAddr & 0xFF0000 ) >> 16;   // 地址
   SPI_TX_Buff[2] = ( WriteAddr & 0xFF00 ) >> 8;;
   SPI_TX_Buff[3] =   WriteAddr & 0xFF;

   for ( i = 0; i < NumByteToWrite; i++)  // 复制要写入的数据
   {
      SPI_TX_Buff[4+i] = * pBuffer;
      pBuffer++;
   }

	if( BSP_SPI_TransmitReceive(SPI6, (uint8_t *)SPI_TX_Buff,(uint8_t *)SPI_RX_Buff, NumByteToWrite+4, 1000)!= 0 )	// 开始传输
	{
		return W25Qxx_ERROR_TRANSMIT;
	}	
    SPI_W25Qxx_WaitForWriteEnd(); // 等待写入完成

	return SPI_W25Qxx_OK; // 写入成功
}

/**********************************************************************************************************
*
*	函 数 名: SPI_W25Qxx_WriteBuffer
*
*	入口参数: pBuffer 		 - 要写入的数据
*				 WriteAddr 		 - 要写入 W25Qxx 的地址
*				 NumByteToWrite - 数据长度，最大不能超过flash芯片的大小
*
*	返 回 值: QSPI_W25Qxx_OK 		     - 写数据成功
*			    W25Qxx_ERROR_WriteEnable - 写使能失败
*				 W25Qxx_ERROR_TRANSMIT	  - 传输失败
*
*	函数功能: 写入数据，最大不能超过flash芯片的大小，写数据前请务必完成擦除操作
*
*	说    明: 1.Flash的写入时间是有限定的
*				 2.按照 W25Q64JV 数据手册给出的 页 写入参考时间，典型值为 0.4ms，最大值为3ms
*				 3.实际的写入速度可能大于0.4ms，也可能小于0.4ms
*				 4.Flash使用的时间越长，写入所需时间也会越长
*				 5.在数据写入之前，请务必完成擦除操作
*				 6.该函数移植于 stm32h743i_eval_qspi.c
*
**********************************************************************************************************/

int8_t SPI_W25Qxx_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t Size)
{	
	uint32_t end_addr=0, current_size=0, current_addr=0;
	uint8_t *write_data;  // 要写入的数据

	current_size = SPI_W25Qxx_PageSize - (WriteAddr % SPI_W25Qxx_PageSize); // 计算当前页还剩余的空间

	if (current_size > Size)	// 判断当前页剩余的空间是否足够写入所有数据
	{
		current_size = Size;		// 如果足够，则直接获取当前长度
	}

	current_addr = WriteAddr;		// 获取要写入的地址
	end_addr = WriteAddr + Size;	// 计算结束地址
	write_data = pBuffer;			// 获取要写入的数据

	do
	{
		// 按页写入数据
		if( SPI_W25Qxx_WritePage(write_data, current_addr, current_size) != SPI_W25Qxx_OK )
		{
			return W25Qxx_ERROR_TRANSMIT;
		}

		else // 按页写入数据成功，进行下一次写数据的准备工作
		{
			current_addr += current_size;	// 计算下一次要写入的地址
			write_data += current_size;	// 获取下一次要写入的数据存储区地址
			// 计算下一次写数据的长度
			current_size = ((current_addr + SPI_W25Qxx_PageSize) > end_addr) ? (end_addr - current_addr) : SPI_W25Qxx_PageSize;
		}
	}
	while (current_addr < end_addr) ; // 判断数据是否全部写入完毕

	return SPI_W25Qxx_OK;	// 写入数据成功
}

/**********************************************************************************************************************************
*
*	函 数 名: SPI_W25Qxx_ReadBuffer
*
*	入口参数:  pBuffer 		  - 要读取的数据存储区
*				 ReadAddr 		 - 要读取 W25Qxx 的地址
*				 NumByteToRead  - 数据长度，最大不能超过flash芯片的大小
*
*	返 回 值: SPI_W25Qxx_OK 		     - 读数据成功
*				 W25Qxx_ERROR_TRANSMIT	  - 传输失败
*
*	函数功能: 读取数据，最大不能超过flash芯片的大小
*
*	说    明: 1.本例程使用的读指令为 0x03，SCK驱动时钟最大不能超过50M
*
*            2.这里使用由HAL库函数 SPI_TransmitReceive() 修改而得的 MY_SPI_TransmitReceive() 进行读取，
*              目的是为了SPI读取数据不用计算偏移以及不限数据长度的读取
*
*****************************************************************************************************************FANKE************/

int8_t SPI_W25Qxx_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{  
    uint8_t	SPI_TX_Buff[4];   // 要发送的指令

    SPI_TX_Buff[0] = W25Qxx_CMD_Read;                  //  读命令 
    SPI_TX_Buff[1] = ( ReadAddr & 0xFF0000 ) >> 16;    //  地址
    SPI_TX_Buff[2] = ( ReadAddr & 0xFF00 ) >> 8;;
    SPI_TX_Buff[3] =   ReadAddr & 0xFF;

	if(BSP_SPI_TransmitReceive_STA(SPI6, (uint8_t *)SPI_TX_Buff, (uint8_t *)pBuffer,4,NumByteToRead) != 0)	
	{
	 	return W25Qxx_ERROR_TRANSMIT;
	}	
	return SPI_W25Qxx_OK; // 写入成功
}
#endif

#if USE_W25Q64_NO2
/**
 * @name: QSPI_W25Qxx_ReadID
 * @msg: 验证ID
 * @param {*}
 * @return {*}
 * 
 * 	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;    // 1线指令模式
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;     	 // 24位地址
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;  // 无交替字节 
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;      // 禁止DDR模式
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  // DDR模式中数据延迟，这里用不到
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;	 // 每次传输数据都发送指令
	s_command.AddressMode		 = QSPI_ADDRESS_NONE;   		 // 无地址模式
	s_command.DataMode			 = QSPI_DATA_1_LINE;       	 // 1线数据模式
	s_command.DummyCycles 		 = 0;                   		 // 空周期个数
	s_command.NbData 				 = 3;                          // 传输数据的长度
	s_command.Instruction 		 = W25Qxx_CMD_JedecID;         // 执行读器件ID命令

 */
//	mode[1:0]:指令模式;00,无指令;01,单线传输指令;10,双线传输指令;11,四线传输指令.
//	mode[3:2]:地址模式;00,无地址;01,单线传输地址;10,双线传输地址;11,四线传输地址.
//	mode[5:4]:地址长度;00,8位地址;01,16位地址;10,24位地址;11,32位地址.
//	mode[7:6]:数据模式;00,无数据;01,单线传输数据;10,双线传输数据;11,四线传输数据.
//dmcycle:空指令周期数
uint32_t QSPI_W25Qxx_ReadID(void)	
{
	uint8_t	QSPI_ReceiveBuff[3];		// 存储QSPI读到的数据
	uint32_t	W25Qxx_ID;					// 器件的ID
    QSPI_Send_CMD(SPI_W25Qxx_CMD_JedecID,0,(1<<6)|(2<<4)|(0<<2)|(1<<0),0);
	//QPI,读id,单线数据，24位地址，无地址线，单线指令，0空周期
	QSPI_Receive(QSPI_ReceiveBuff,3);
    W25Qxx_ID=(QSPI_ReceiveBuff[0]<<16)|(QSPI_ReceiveBuff[1]<<8)|(QSPI_ReceiveBuff[2]);
    return W25Qxx_ID; // 返回ID
}

#endif
