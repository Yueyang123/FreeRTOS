/*
 * @Descripttion: W25Q64����������ʹ��QSPI,��Ӳ��SPI����ģʽ
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
    uint32_t	Device_ID;	// ����ID
#if  USE_W25Q64_NO1 
    SPIX_HARD_Init(SPI6);
    Device_ID =  SPI_W25Qxx_ReadID();
    
	if( Device_ID == SPI_W25Qxx_FLASH_ID )		// ����ƥ��
	{
		printf ("W25Q64 OK,flash ID:%X\r\n",Device_ID);		// ��ʼ���ɹ�	
	}
	else
	{
		printf ("W25Q64 ERROR!!!!!  ID:%X\r\n",Device_ID);	// ��ʼ��ʧ��	
	}
#endif

#if USE_W25Q64_NO2
    QSPI_Init();
    Device_ID =  QSPI_W25Qxx_ReadID();
	if( Device_ID == SPI_W25Qxx_FLASH_ID )		// ����ƥ��
	{
		printf ("QSPI W25Q64 OK,flash ID:%X\r\n",Device_ID);		// ��ʼ���ɹ�	
	}
	else
	{
		printf ("QSPI W25Q64 ERROR!!!!!  ID:%X\r\n",Device_ID);	// ��ʼ��ʧ��	
	}
#endif
}


#if  USE_W25Q64_NO1 
/*************************************************************************************************
*	�� �� ��: SPI_W25Qxx_ReadID
*	��ڲ���: ��
*	�� �� ֵ: W25Qxx_ID - ��ȡ��������ID��W25Qxx_ERROR_INIT - ͨ�š���ʼ������
*	��������: ��ȡ����ID
*	˵    ��: ��	
**************************************************************************************************/

uint32_t SPI_W25Qxx_ReadID(void)
{
   uint8_t	SPI_TX_Buff[1];      // Ҫ���͵�ָ��
   uint8_t	SPI_RX_Buff[4];		// �洢SPI����������
   uint32_t W25Qxx_ID;           // ID

	SPI_TX_Buff[0] = SPI_W25Qxx_CMD_JedecID;		// ���Ͷ�JedecID���� 
	if(BSP_SPI_TransmitReceive(SPI6, (uint8_t *)SPI_TX_Buff, (uint8_t *)SPI_RX_Buff, 4, 1000) != 0)	
	{
		return ERROR;
	}	
   
// ��Ҫע��ģ�ʹ�� SPI_TransmitReceive() ����ͨ�ţ����͵�һ����ID����ʱ���ͻ�������ȡһ���ֽ�
// ��ˣ�Ҫ��õ�ʵ�ʵ�������Ҫƫ��һ���ֽ�
   W25Qxx_ID = (SPI_RX_Buff[1] << 16) | (SPI_RX_Buff[2] << 8) | SPI_RX_Buff[3];    // ���������������ΪID
   return W25Qxx_ID; // ����IDֵ
}

/*************************************************************************************************
*	�� �� ��: SPI_W25Qxx_WaitForWriteEnd  
*	��ڲ���: ��
*	�� �� ֵ: SPI_W25Qxx_OK - ͨ������������W25Qxx_ERROR_Wait - �ȴ�����
*	��������: �ȴ�W25Qxx�������
*	˵    ��: ����/д������ʱ����Ҫ���øú����ȴ��������	
**************************************************************************************************/

int8_t SPI_W25Qxx_WaitForWriteEnd(void)
{
    uint8_t	SPI_TX_Buff[1];   // Ҫ���͵�ָ��
	uint8_t	SPI_RX_Buff[2];	// �洢SPI���������� 

   while (1)
   {
	  SPI_TX_Buff[0] = SPI_W25Qxx_CMD_ReadStatus_REG1;		// ���Ͷ� ��״̬�Ĵ��� ���� 
      SPI_RX_Buff[1] = 0;     // ���洢������

      if(BSP_SPI_TransmitReceive(SPI6, (uint8_t *)SPI_TX_Buff, (uint8_t *)SPI_RX_Buff, 2, 1000) != 0)	
      {
         return W25Qxx_ERROR_Wait;
      }	  
      // ��״̬�Ĵ���1�ĵ�0λ��ֻ������Busy��־λ�������ڲ���/д������/д����ʱ�ᱻ��1
      if( (SPI_RX_Buff[1] & SPI_W25Qxx_CMD_Status_REG1_BUSY)  != SET )  // �Ƚϱ�־λ
      {
         return SPI_W25Qxx_OK;
      }   
   }
}

/*************************************************************************************************
*	�� �� ��: SPI_W25Qxx_WriteEnable
*	��ڲ���: ��
*	�� �� ֵ: QSPI_W25Qxx_OK - дʹ�ܳɹ���W25Qxx_ERROR_WriteEnable - дʹ��ʧ��
*	��������: ����дʹ������
*	˵    ��: �ڲ�������д������֮ǰ����Ҫ���øú�������дʹ��
**************************************************************************************************/

int8_t SPI_W25Qxx_WriteEnable(void)
{
   uint8_t	SPI_TX_Buff[1];   // Ҫ���͵�ָ��
	uint8_t	SPI_RX_Buff[1];	// �洢SPI���������ݣ��˴��ò��� 

	SPI_TX_Buff[0] = SPI_W25Qxx_CMD_WriteEnable;		// ���Ͷ� дʹ�� ���� 

	if(BSP_SPI_TransmitReceive(SPI6, (uint8_t *)SPI_TX_Buff, (uint8_t *)SPI_RX_Buff, 1, 1000) != 0)	
	{
		return W25Qxx_ERROR_WriteEnable;
	}	
    return SPI_W25Qxx_OK;
}

/*************************************************************************************************
*
*	�� �� ��: SPI_W25Qxx_SectorErase
*
*	��ڲ���: SectorAddress - Ҫ�����ĵ�ַ
*
*	�� �� ֵ: SPI_W25Qxx_OK - �����ɹ�
*			    W25Qxx_ERROR_Erase - ����ʧ��
*
*	��������: ������������������ÿ�β���4K�ֽ�
*
*	˵    ��: 1.���� W25Q64JV �����ֲ�����Ĳ����ο�ʱ�䣬����ֵΪ 45ms�����ֵΪ400ms
*				 2.ʵ�ʵĲ����ٶȿ��ܴ���45ms��Ҳ����С��45ms
*				 3.flashʹ�õ�ʱ��Խ������������ʱ��Ҳ��Խ��
*
**************************************************************************************************/

int8_t SPI_W25Qxx_SectorErase(uint32_t SectorAddress)	
{
   uint8_t	SPI_TX_Buff[4];   // Ҫ���͵�ָ��
	uint8_t	SPI_RX_Buff[1];	// �洢SPI���������ݣ��˴��ò��� 

   SPI_W25Qxx_WriteEnable();  // дʹ��

   SPI_TX_Buff[0] = SPI_W25Qxx_CMD_SectorErase;          // 4K��������ָ��
   SPI_TX_Buff[1] = ( SectorAddress & 0xFF0000 ) >> 16;  // ����Ҫ�����ĵ�ַ
   SPI_TX_Buff[2] = ( SectorAddress & 0xFF00 ) >> 8;;
   SPI_TX_Buff[3] =   SectorAddress & 0xFF;

	if(BSP_SPI_TransmitReceive(SPI6, (uint8_t *)SPI_TX_Buff, (uint8_t *)SPI_RX_Buff, 4, 1000) != 0)	
	{
		return W25Qxx_ERROR_Erase;
	}	 
   SPI_W25Qxx_WaitForWriteEnd(); // �ȴ���������

	return SPI_W25Qxx_OK; // �����ɹ�
}

/*************************************************************************************************
*
*	�� �� ��: SPI_W25Qxx_BlockErase_32K
*
*	��ڲ���: SectorAddress - Ҫ�����ĵ�ַ
*
*	�� �� ֵ: SPI_W25Qxx_OK - �����ɹ�
*			    W25Qxx_ERROR_Erase - ����ʧ��
*
*	��������: ���п����������ÿ�β���32K�ֽ�
*
*	˵    ��: 1.���� W25Q64JV �����ֲ�����Ĳ����ο�ʱ�䣬����ֵΪ 120ms�����ֵΪ1600ms
*				 2.ʵ�ʵĲ����ٶȿ��ܴ���120ms��Ҳ����С��120ms
*				 3.flashʹ�õ�ʱ��Խ������������ʱ��Ҳ��Խ��
*
*************************************************************************************************/

int8_t SPI_W25Qxx_BlockErase_32K(uint32_t SectorAddress)	
{
   uint8_t	SPI_TX_Buff[4];   // Ҫ���͵�ָ��
	uint8_t	SPI_RX_Buff[1];	// �洢SPI���������ݣ��˴��ò��� 

   SPI_W25Qxx_WriteEnable();  // дʹ��

   SPI_TX_Buff[0] = SPI_W25Qxx_CMD_BlockErase_32K;       // 32K�����ָ��
   SPI_TX_Buff[1] = ( SectorAddress & 0xFF0000 ) >> 16;  // ����Ҫ�����ĵ�ַ
   SPI_TX_Buff[2] = ( SectorAddress & 0xFF00 ) >> 8;;
   SPI_TX_Buff[3] =   SectorAddress & 0xFF;
	if(BSP_SPI_TransmitReceive(SPI6, (uint8_t *)SPI_TX_Buff, (uint8_t *)SPI_RX_Buff, 4, 1000) != 0)	
	{
		return W25Qxx_ERROR_Erase;
	}	 
    SPI_W25Qxx_WaitForWriteEnd(); // �ȴ��������

	return SPI_W25Qxx_OK; // �����ɹ�
}

/*************************************************************************************************
*
*	�� �� ��: SPI_W25Qxx_BlockErase_64K
*
*	��ڲ���: SectorAddress - Ҫ�����ĵ�ַ
*
*	�� �� ֵ: SPI_W25Qxx_OK - �����ɹ�
*			    W25Qxx_ERROR_Erase - ����ʧ��
*
*	��������: ���п����������ÿ�β���64K�ֽ�
*
*	˵    ��: 1.���� W25Q64JV �����ֲ�����Ĳ����ο�ʱ�䣬����ֵΪ 150ms�����ֵΪ2000ms
*				 2.ʵ�ʵĲ����ٶȿ��ܴ���150ms��Ҳ����С��150ms
*				 3.flashʹ�õ�ʱ��Խ������������ʱ��Ҳ��Խ��
*				 4.ʵ��ʹ�ý���ʹ��64K������������ʱ�����
*
**************************************************************************************************/

int8_t SPI_W25Qxx_BlockErase_64K(uint32_t SectorAddress)	
{
   uint8_t	SPI_TX_Buff[4];   // Ҫ���͵�ָ��
	uint8_t	SPI_RX_Buff[1];	// �洢SPI���������ݣ��˴��ò��� 

   SPI_W25Qxx_WriteEnable();  // дʹ��   

   SPI_TX_Buff[0] = SPI_W25Qxx_CMD_BlockErase_64K;       // 64K�����ָ��
   SPI_TX_Buff[1] = ( SectorAddress & 0xFF0000 ) >> 16;  // ����Ҫ�����ĵ�ַ
   SPI_TX_Buff[2] = ( SectorAddress & 0xFF00 ) >> 8;;
   SPI_TX_Buff[3] =   SectorAddress & 0xFF;

	if(BSP_SPI_TransmitReceive(SPI6, (uint8_t *)SPI_TX_Buff, (uint8_t *)SPI_RX_Buff, 4, 1000) != 0)	
	{
		return W25Qxx_ERROR_Erase;
	} 
   SPI_W25Qxx_WaitForWriteEnd();    // �ȴ���������

	return SPI_W25Qxx_OK; // �����ɹ�
}

/*************************************************************************************************
*
*	�� �� ��: SPI_W25Qxx_ChipErase
*
*	��ڲ���: ��
*
*	�� �� ֵ: SPI_W25Qxx_OK - �����ɹ�
*			    W25Qxx_ERROR_Erase - ����ʧ��
*
*	��������: ������Ƭ��������
*
*	˵    ��: 1.���� W25Q64JV �����ֲ�����Ĳ����ο�ʱ�䣬����ֵΪ 20s�����ֵΪ100s
*				 2.ʵ�ʵĲ����ٶȿ��ܴ���20s��Ҳ����С��20s
*				 3.flashʹ�õ�ʱ��Խ������������ʱ��Ҳ��Խ��
*
*************************************************************************************************/

int8_t SPI_W25Qxx_ChipErase(void)	
{
   uint8_t	SPI_TX_Buff[1];   // Ҫ���͵�ָ��
   uint8_t	SPI_RX_Buff[1];	// �洢SPI���������ݣ��˴��ò��� 

   SPI_W25Qxx_WriteEnable();  // дʹ��

   SPI_TX_Buff[0] = SPI_W25Qxx_CMD_ChipErase;   // ȫƬ����ָ��

	if(BSP_SPI_TransmitReceive(SPI6, (uint8_t *)SPI_TX_Buff, (uint8_t *)SPI_RX_Buff, 1, 1000) != 0)	
	{
		return W25Qxx_ERROR_Erase;
	}	
   
   SPI_W25Qxx_WaitForWriteEnd();

	return SPI_W25Qxx_OK; // �����ɹ�
}

/**********************************************************************************************************
*
*	�� �� ��: SPI_W25Qxx_WritePage
*
*	��ڲ���: pBuffer 		 - Ҫд�������
*				 WriteAddr 		 - Ҫд�� W25Qxx �ĵ�ַ
*				 NumByteToWrite - ���ݳ��ȣ����ֻ��256�ֽ�
*
*	�� �� ֵ: SPI_W25Qxx_OK 		     - д���ݳɹ�
*				 W25Qxx_ERROR_TRANSMIT	  - ����ʧ��
*
*	��������: ��ҳд�룬���ֻ��256�ֽڣ�������д��֮ǰ���������ɲ�������
*
*	˵    ��: 1.Flash��д��ʱ�����޶���
*				 2.���� W25Q64JV �����ֲ������ ҳ(256�ֽ�) д��ο�ʱ�䣬����ֵΪ 0.4ms�����ֵΪ3ms
*				 3.ʵ�ʵ�д���ٶȿ��ܴ���0.4ms��Ҳ����С��0.4ms
*				 4.Flashʹ�õ�ʱ��Խ����д������ʱ��Ҳ��Խ��
*				 5.������д��֮ǰ���������ɲ�������
*
***********************************************************************************************************/

int8_t SPI_W25Qxx_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint8_t	SPI_TX_Buff[260];    // Ҫ���͵�ָ��
    uint8_t	SPI_RX_Buff[1]; 	
	uint16_t i = 0;               // ��������

   SPI_W25Qxx_WriteEnable();     // дʹ��

   SPI_TX_Buff[0] = W25Qxx_CMD_PageProgram;           // ҳ���ָ��
   SPI_TX_Buff[1] = ( WriteAddr & 0xFF0000 ) >> 16;   // ��ַ
   SPI_TX_Buff[2] = ( WriteAddr & 0xFF00 ) >> 8;;
   SPI_TX_Buff[3] =   WriteAddr & 0xFF;

   for ( i = 0; i < NumByteToWrite; i++)  // ����Ҫд�������
   {
      SPI_TX_Buff[4+i] = * pBuffer;
      pBuffer++;
   }

	if( BSP_SPI_TransmitReceive(SPI6, (uint8_t *)SPI_TX_Buff,(uint8_t *)SPI_RX_Buff, NumByteToWrite+4, 1000)!= 0 )	// ��ʼ����
	{
		return W25Qxx_ERROR_TRANSMIT;
	}	
    SPI_W25Qxx_WaitForWriteEnd(); // �ȴ�д�����

	return SPI_W25Qxx_OK; // д��ɹ�
}

/**********************************************************************************************************
*
*	�� �� ��: SPI_W25Qxx_WriteBuffer
*
*	��ڲ���: pBuffer 		 - Ҫд�������
*				 WriteAddr 		 - Ҫд�� W25Qxx �ĵ�ַ
*				 NumByteToWrite - ���ݳ��ȣ�����ܳ���flashоƬ�Ĵ�С
*
*	�� �� ֵ: QSPI_W25Qxx_OK 		     - д���ݳɹ�
*			    W25Qxx_ERROR_WriteEnable - дʹ��ʧ��
*				 W25Qxx_ERROR_TRANSMIT	  - ����ʧ��
*
*	��������: д�����ݣ�����ܳ���flashоƬ�Ĵ�С��д����ǰ�������ɲ�������
*
*	˵    ��: 1.Flash��д��ʱ�������޶���
*				 2.���� W25Q64JV �����ֲ������ ҳ д��ο�ʱ�䣬����ֵΪ 0.4ms�����ֵΪ3ms
*				 3.ʵ�ʵ�д���ٶȿ��ܴ���0.4ms��Ҳ����С��0.4ms
*				 4.Flashʹ�õ�ʱ��Խ����д������ʱ��Ҳ��Խ��
*				 5.������д��֮ǰ���������ɲ�������
*				 6.�ú�����ֲ�� stm32h743i_eval_qspi.c
*
**********************************************************************************************************/

int8_t SPI_W25Qxx_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t Size)
{	
	uint32_t end_addr=0, current_size=0, current_addr=0;
	uint8_t *write_data;  // Ҫд�������

	current_size = SPI_W25Qxx_PageSize - (WriteAddr % SPI_W25Qxx_PageSize); // ���㵱ǰҳ��ʣ��Ŀռ�

	if (current_size > Size)	// �жϵ�ǰҳʣ��Ŀռ��Ƿ��㹻д����������
	{
		current_size = Size;		// ����㹻����ֱ�ӻ�ȡ��ǰ����
	}

	current_addr = WriteAddr;		// ��ȡҪд��ĵ�ַ
	end_addr = WriteAddr + Size;	// ���������ַ
	write_data = pBuffer;			// ��ȡҪд�������

	do
	{
		// ��ҳд������
		if( SPI_W25Qxx_WritePage(write_data, current_addr, current_size) != SPI_W25Qxx_OK )
		{
			return W25Qxx_ERROR_TRANSMIT;
		}

		else // ��ҳд�����ݳɹ���������һ��д���ݵ�׼������
		{
			current_addr += current_size;	// ������һ��Ҫд��ĵ�ַ
			write_data += current_size;	// ��ȡ��һ��Ҫд������ݴ洢����ַ
			// ������һ��д���ݵĳ���
			current_size = ((current_addr + SPI_W25Qxx_PageSize) > end_addr) ? (end_addr - current_addr) : SPI_W25Qxx_PageSize;
		}
	}
	while (current_addr < end_addr) ; // �ж������Ƿ�ȫ��д�����

	return SPI_W25Qxx_OK;	// д�����ݳɹ�
}

/**********************************************************************************************************************************
*
*	�� �� ��: SPI_W25Qxx_ReadBuffer
*
*	��ڲ���:  pBuffer 		  - Ҫ��ȡ�����ݴ洢��
*				 ReadAddr 		 - Ҫ��ȡ W25Qxx �ĵ�ַ
*				 NumByteToRead  - ���ݳ��ȣ�����ܳ���flashоƬ�Ĵ�С
*
*	�� �� ֵ: SPI_W25Qxx_OK 		     - �����ݳɹ�
*				 W25Qxx_ERROR_TRANSMIT	  - ����ʧ��
*
*	��������: ��ȡ���ݣ�����ܳ���flashоƬ�Ĵ�С
*
*	˵    ��: 1.������ʹ�õĶ�ָ��Ϊ 0x03��SCK����ʱ������ܳ���50M
*
*            2.����ʹ����HAL�⺯�� SPI_TransmitReceive() �޸Ķ��õ� MY_SPI_TransmitReceive() ���ж�ȡ��
*              Ŀ����Ϊ��SPI��ȡ���ݲ��ü���ƫ���Լ��������ݳ��ȵĶ�ȡ
*
*****************************************************************************************************************FANKE************/

int8_t SPI_W25Qxx_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{  
    uint8_t	SPI_TX_Buff[4];   // Ҫ���͵�ָ��

    SPI_TX_Buff[0] = W25Qxx_CMD_Read;                  //  ������ 
    SPI_TX_Buff[1] = ( ReadAddr & 0xFF0000 ) >> 16;    //  ��ַ
    SPI_TX_Buff[2] = ( ReadAddr & 0xFF00 ) >> 8;;
    SPI_TX_Buff[3] =   ReadAddr & 0xFF;

	if(BSP_SPI_TransmitReceive_STA(SPI6, (uint8_t *)SPI_TX_Buff, (uint8_t *)pBuffer,4,NumByteToRead) != 0)	
	{
	 	return W25Qxx_ERROR_TRANSMIT;
	}	
	return SPI_W25Qxx_OK; // д��ɹ�
}
#endif

#if USE_W25Q64_NO2
/**
 * @name: QSPI_W25Qxx_ReadID
 * @msg: ��֤ID
 * @param {*}
 * @return {*}
 * 
 * 	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;    // 1��ָ��ģʽ
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;     	 // 24λ��ַ
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;  // �޽����ֽ� 
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;      // ��ֹDDRģʽ
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  // DDRģʽ�������ӳ٣������ò���
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;	 // ÿ�δ������ݶ�����ָ��
	s_command.AddressMode		 = QSPI_ADDRESS_NONE;   		 // �޵�ַģʽ
	s_command.DataMode			 = QSPI_DATA_1_LINE;       	 // 1������ģʽ
	s_command.DummyCycles 		 = 0;                   		 // �����ڸ���
	s_command.NbData 				 = 3;                          // �������ݵĳ���
	s_command.Instruction 		 = W25Qxx_CMD_JedecID;         // ִ�ж�����ID����

 */
//	mode[1:0]:ָ��ģʽ;00,��ָ��;01,���ߴ���ָ��;10,˫�ߴ���ָ��;11,���ߴ���ָ��.
//	mode[3:2]:��ַģʽ;00,�޵�ַ;01,���ߴ����ַ;10,˫�ߴ����ַ;11,���ߴ����ַ.
//	mode[5:4]:��ַ����;00,8λ��ַ;01,16λ��ַ;10,24λ��ַ;11,32λ��ַ.
//	mode[7:6]:����ģʽ;00,������;01,���ߴ�������;10,˫�ߴ�������;11,���ߴ�������.
//dmcycle:��ָ��������
uint32_t QSPI_W25Qxx_ReadID(void)	
{
	uint8_t	QSPI_ReceiveBuff[3];		// �洢QSPI����������
	uint32_t	W25Qxx_ID;					// ������ID
    QSPI_Send_CMD(SPI_W25Qxx_CMD_JedecID,0,(1<<6)|(2<<4)|(0<<2)|(1<<0),0);
	//QPI,��id,�������ݣ�24λ��ַ���޵�ַ�ߣ�����ָ�0������
	QSPI_Receive(QSPI_ReceiveBuff,3);
    W25Qxx_ID=(QSPI_ReceiveBuff[0]<<16)|(QSPI_ReceiveBuff[1]<<8)|(QSPI_ReceiveBuff[2]);
    return W25Qxx_ID; // ����ID
}

#endif
