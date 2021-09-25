#include "usbd_storage.h"
#include "sdmmc_sdcard.h"
#include "W25Q64.h"

#define STORAGE_BLK_NBR                  2048	// ������
#define STORAGE_BLK_SIZ                  4096	// ÿ�����С4096

////////////////////////////�Լ������һ�����USB״̬�ļĴ���///////////////////
//bit0:��ʾ����������SD��д������
//bit1:��ʾ��������SD����������
//bit2:SD��д���ݴ����־λ
//bit3:SD�������ݴ����־λ
//bit4:1,��ʾ��������ѯ����(�������ӻ�������)
vu8 USB_STATUS_REG=0;
////////////////////////////////////////////////////////////////////////////////
 
//USB Mass storage ��׼��ѯ����(ÿ��lunռ36�ֽ�)
const int8_t  STORAGE_Inquirydata[] = { 
  
	/* LUN 0 */ 
	0x00,		
	0x80,		
	0x02,		
	0x02,
	(STANDARD_INQUIRY_DATA_LEN - 4),
	0x00,
	0x00,	
	0x00,
    /* Vendor Identification */
    'A', 'L', 'I', 'E', 'N', 'T', 'E', 'K', ' ',//9�ֽ�
    /* Product Identification */
    'S', 'P', 'I', ' ', 'F', 'l', 'a', 's', 'h',//15�ֽ�
    ' ','D', 'i', 's', 'k', ' ',
    /* Product Revision Level */	
    '1', '.', '0', ' ',							//4�ֽ�		
	
	/* LUN 1 */
	0x00,
	0x80,		
	0x02,		
	0x02,
	(STANDARD_INQUIRY_DATA_LEN - 4),
	0x00,
	0x00,	
	0x00,
	/* Vendor Identification */
	'A', 'L', 'I', 'E', 'N', 'T', 'E', 'K',' ',	//9�ֽ�
	/* Product Identification */				
    'N', 'A', 'N', 'D', ' ', 'F', 'l', 'a', 's', 'h',//15�ֽ�
	' ','D', 'i', 's', 'k', 
    /* Product Revision Level */
	'1', '.', '0' ,' ',                      	//4�ֽ�
	
	/* LUN 2 */
	0x00,
	0x80,		
	0x02,		
	0x02,
	(STANDARD_INQUIRY_DATA_LEN - 4),
	0x00,
	0x00,	
	0x00,
	/* Vendor Identification */
	'A', 'L', 'I', 'E', 'N', 'T', 'E', 'K',' ',	//9�ֽ�
	/* Product Identification */				
	'S', 'D', ' ', 'F', 'l', 'a', 's', 'h', ' ',//15�ֽ�
	'D', 'i', 's', 'k', ' ', ' ',  
    /* Product Revision Level */
	'1', '.', '0' ,' ',                      	//4�ֽ�
}; 
  
/* Private function prototypes ----------------------------------------------- */
int8_t STORAGE_Init(uint8_t lun);
int8_t STORAGE_GetCapacity(uint8_t lun, uint32_t * block_num,uint16_t * block_size);
int8_t STORAGE_IsReady(uint8_t lun);
int8_t STORAGE_IsWriteProtected(uint8_t lun);
int8_t STORAGE_Read(uint8_t lun, uint8_t * buf, uint32_t blk_addr,uint16_t blk_len);
int8_t STORAGE_Write(uint8_t lun, uint8_t * buf, uint32_t blk_addr,uint16_t blk_len);
int8_t STORAGE_GetMaxLun(void);

USBD_StorageTypeDef USBD_DISK_fops = {
  STORAGE_Init,
  STORAGE_GetCapacity,
  STORAGE_IsReady,
  STORAGE_IsWriteProtected,
  STORAGE_Read,
  STORAGE_Write,
  STORAGE_GetMaxLun,
  (int8_t *)STORAGE_Inquirydata,
};

//��ʼ���洢�豸
//lun:�߼���Ԫ���,0,SD��;1,SPI FLASH
//����ֵ:0,�ɹ�;
//    ����,�������
int8_t STORAGE_Init (uint8_t lun)
{
	u8 res=0;
	switch(lun)
	{
		case 0://SPI FLASH
			W25Q64_Init();
			break;
		case 1://NAND FLASH
			SD_Init();
			break; 
	}  
	return res; 
} 

//��ȡ�洢�豸�������Ϳ��С
//lun:�߼���Ԫ���,0,SD��;1,SPI FLASH
//block_num:������(������)
//block_size:���С(������С)
//����ֵ:0,�ɹ�;
//    ����,�������
int8_t STORAGE_GetCapacity (uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{     
	switch(lun)
	{
		case 0://SPI FLASH
            *block_num  = STORAGE_BLK_NBR;
            *block_size = STORAGE_BLK_SIZ;	//SPI FLASH��ǰ��25M�ֽ�,�ļ�ϵͳ��
			break;
		case 1://SD��
			*block_size=512;  
			*block_num=SDCardInfo.CardCapacity/512; 
			break; 

	}  	
	return 0; 
} 

//�鿴�洢�豸�Ƿ����
//lun:�߼���Ԫ���,0,SD��;1,SPI FLASH
//����ֵ:0,����;
//    ����,δ����
int8_t  STORAGE_IsReady (uint8_t lun)
{ 
	USB_STATUS_REG|=0X10;//�����ѯ
	return 0;
} 

//�鿴�洢�豸�Ƿ�д����
//lun:�߼���Ԫ���,0,SD��;1,SPI FLASH
//����ֵ:0,û��д����;
//    ����,д����(ֻ��)
int8_t  STORAGE_IsWriteProtected (uint8_t lun)
{
	return  0;
} 

//�Ӵ洢�豸��ȡ����
//lun:�߼���Ԫ���,0,SD��;1,SPI FLASH
//buf:���ݴ洢���׵�ַָ��
//blk_addr:Ҫ��ȡ�ĵ�ַ(������ַ)
//blk_len:Ҫ��ȡ�Ŀ���(������) 
//����ֵ:0,�ɹ�;
//    ����,������� 
int8_t STORAGE_Read (uint8_t lun,uint8_t *buf,uint32_t blk_addr,uint16_t blk_len)
{
	int8_t res=0;
	USB_STATUS_REG|=0X02;//������ڶ����� 
	switch(lun)
	{
		case 0://SPI FLASH
            SPI_W25Qxx_ReadBuffer( (uint8_t *)buf, blk_addr<<12, blk_len<<12 );   // ��ȡ���ݣ�����ܳ���flashоƬ�Ĵ�С
			break;
		case 1://SD��
			res=SD_ReadDisk(buf,blk_addr,blk_len);
			break; 

	} 
	if(res)
	{
		USB_STATUS_REG|=0X08;//������! 
	}  
	return res;
}

//��洢�豸д����
//lun:�߼���Ԫ���,0,SD��;1,SPI FLASH
//buf:���ݴ洢���׵�ַָ��
//blk_addr:Ҫд��ĵ�ַ(������ַ)
//blk_len:Ҫд��Ŀ���(������) 
//����ֵ:0,�ɹ�;
//    ����,������� 
int8_t STORAGE_Write (uint8_t lun,uint8_t *buf,uint32_t blk_addr,uint16_t blk_len) 
{
	int8_t res=0;
	USB_STATUS_REG|=0X01;//�������д���� 
	switch(lun)
	{
		case 0://SPI FLASH
            SPI_W25Qxx_SectorErase( blk_addr<<12);
            SPI_W25Qxx_WriteBuffer( (uint8_t *)buf, blk_addr<< 12, blk_len<<12);	 
			break;
		case 1://SD��
			res=SD_WriteDisk(buf,blk_addr,blk_len);
			break;  
	}  
	if(res)
	{
		USB_STATUS_REG|=0X04;//д����!	 
	} 
	return res; 
}

//��ȡ֧�ֵ�����߼���Ԫ����
//����ֵ:֧�ֵ��߼���Ԫ����-1
int8_t STORAGE_GetMaxLun (void)
{
	if(SDCardInfo.CardCapacity)return STORAGE_LUN_NBR-1;
	else return STORAGE_LUN_NBR-2;
}






















