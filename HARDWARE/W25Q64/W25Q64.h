/*
 * @Descripttion: 
 * @Version: V1.0
 * @Author: YURI
 * @Email: 1700695611@qq.com
 * @Website: www.heredifferent.com.cn
 * @Date: 2021-08-21 10:29:51
 * @LastEditors: YURI
 * @LastEditTime: 2021-08-22 12:55:25
 */
#ifndef _SPI_W24Q64
#define _SPI_W24Q64

#include "sys.h"


//0��ʹ�� ��1ʹ��
#define  USE_W25Q64_NO1  1
#define  USE_W25Q64_NO2  0
//0 ��ͨģʽ
//1 QSPI��ַӳ�䵽0x90000000
#define W25Q64_NO2_MODE  0
/*----------------------------------------------- ���������� -------------------------------------------*/
#define QSPI_W25Qxx_OK           		0		// W25Qxxͨ������
#define SPI_W25Qxx_OK           		    0		// W25Qxxͨ������
#define W25Qxx_ERROR_INIT         		-1		// ��ʼ������
#define W25Qxx_ERROR_WriteEnable       -2		// дʹ�ܴ���
#define W25Qxx_ERROR_Wait              -3		// ��ѯ�ȴ���������Ӧ
#define W25Qxx_ERROR_Erase         		-4		// ��������
#define W25Qxx_ERROR_TRANSMIT         	-5		// �������
#define W25Qxx_ERROR_MemoryMapped		-6    // �ڴ�ӳ��ģʽ����


#define SPI_W25Qxx_CMD_JedecID 			      0x9F		   // JEDEC ID  
#define SPI_W25Qxx_CMD_WriteEnable		      0X06		   // дʹ��

#define SPI_W25Qxx_CMD_SectorErase 		      0x20		   // ����������4K�ֽڣ� �ο�����ʱ�� 45ms
#define SPI_W25Qxx_CMD_BlockErase_32K 	      0x52		   // �������  32K�ֽڣ��ο�����ʱ�� 120ms
#define SPI_W25Qxx_CMD_BlockErase_64K 	      0xD8		   // �������  64K�ֽڣ��ο�����ʱ�� 150ms
#define SPI_W25Qxx_CMD_ChipErase 		      0xC7		   // ��Ƭ�������ο�����ʱ�� 20S

#define W25Qxx_CMD_PageProgram  	            0x02  		// ҳ���ָ��ο�д��ʱ�� 0.4ms 
#define W25Qxx_CMD_Read       	         	0x03  		// ������ָ�ʹ�ø�ָ��ʱ�����ֻ��50MHz

#define SPI_W25Qxx_CMD_ReadStatus_REG1			0X05			// ��״̬�Ĵ���1
#define SPI_W25Qxx_CMD_Status_REG1_BUSY  		0x01			// ��״̬�Ĵ���1�ĵ�0λ��ֻ������Busy��־λ�������ڲ���/д������/д����ʱ�ᱻ��1
#define SPI_W25Qxx_CMD_Status_REG1_WEL  		0x02			// ��״̬�Ĵ���1�ĵ�1λ��ֻ������WELдʹ�ܱ�־λ���ñ�־λΪ1ʱ��������Խ���д����

#define SPI_W25Qxx_PageSize       				256			// ҳ��С��256�ֽ�
#define SPI_W25Qxx_FlashSize       				0x800000		// W25Q64��С��8M�ֽ�
#define SPI_W25Qxx_FLASH_ID           			0Xef4017    // W25Q64 JEDEC ID

/*------------------------------------------------ �������� ----------------------------------------------*/


void W25Q64_Init(void);
uint32_t SPI_W25Qxx_ReadID(void);   // ��ȡ����ID

int8_t SPI_W25Qxx_WaitForWriteEnd(void);  // �ȴ� ����/д������/д���� ���
int8_t SPI_W25Qxx_WriteEnable(void);      // дʹ��

int8_t SPI_W25Qxx_SectorErase(uint32_t SectorAddress)	;     // ����������4K�ֽڣ� �ο�����ʱ�� 45ms
int8_t SPI_W25Qxx_BlockErase_32K(uint32_t SectorAddress)	;  // �������  32K�ֽڣ��ο�����ʱ�� 120ms
int8_t SPI_W25Qxx_BlockErase_64K(uint32_t SectorAddress)	;  // �������  64K�ֽڣ��ο�����ʱ�� 150ms
int8_t SPI_W25Qxx_ChipErase(void);                          // ��Ƭ�������ο�����ʱ�� 20S

int8_t SPI_W25Qxx_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);  // ҳд�룬���256�ֽڣ��ο�д��ʱ�� 0.4ms 
int8_t SPI_W25Qxx_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t Size);          // д�����ݣ�����ܳ���flashоƬ�Ĵ�С��д����ǰ�������ɲ�������
int8_t SPI_W25Qxx_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);   // ��ȡ���ݣ�����ܳ���flashоƬ�Ĵ�С


/*----------------------------------------------- �������� ---------------------------------------------------*/
int8_t 	QSPI_W25Qxx_Reset(void);					// ��λ����
uint32_t QSPI_W25Qxx_ReadID(void);					// ��ȡ����ID
int8_t 	QSPI_W25Qxx_MemoryMappedMode(void);		// �����ڴ�ӳ��ģʽ
	
int8_t 	QSPI_W25Qxx_SectorErase(uint32_t SectorAddress);		// ����������4K�ֽڣ� �ο�����ʱ�� 45ms
int8_t 	QSPI_W25Qxx_BlockErase_32K (uint32_t SectorAddress);	// �������  32K�ֽڣ��ο�����ʱ�� 120ms
int8_t 	QSPI_W25Qxx_BlockErase_64K (uint32_t SectorAddress);	// �������  64K�ֽڣ��ο�����ʱ�� 150ms��ʵ��ʹ�ý���ʹ��64K������������ʱ�����
int8_t 	QSPI_W25Qxx_ChipErase (void);                         // ��Ƭ�������ο�����ʱ�� 20S

int8_t	QSPI_W25Qxx_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);	// ��ҳд�룬���256�ֽ�
int8_t	QSPI_W25Qxx_WriteBuffer(uint8_t* pData, uint32_t WriteAddr, uint32_t Size);				// д�����ݣ�����ܳ���flashоƬ�Ĵ�С
int8_t 	QSPI_W25Qxx_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);	// ��ȡ���ݣ�����ܳ���flashоƬ�Ĵ�С




#endif //spi_w25q64