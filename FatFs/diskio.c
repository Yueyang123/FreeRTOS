/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "W25Q64.h"
#include "sdmmc_sdcard.h"

#define SD_Card		0	//SD卡 
#define SPI_Flash		1	//SPI Flash


#define FILE_MAX_TYPE_NUM		7	//最多FILE_MAX_TYPE_NUM个大类
#define FILE_MAX_SUBT_NUM		4	//最多FILE_MAX_SUBT_NUM个小类


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;

	switch (pdrv) {
		
		case SD_Card :
		return RES_OK;

		case SPI_Flash :
			if(SPI_W25Qxx_FLASH_ID == SPI_W25Qxx_ReadID())
			{
			  stat &= ~STA_NOINIT;
			}	
			else
			{
			  stat = STA_NOINIT;;
			}			
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;

	switch (pdrv) {
		
		case SD_Card :
            SD_Init();     
            stat &= ~STA_NOINIT;

		case SPI_Flash :
			W25Q64_Init();
			stat &= ~STA_NOINIT;			
		return stat;

	}
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
    u8 res=0; 
	switch (pdrv) {
		
		case SD_Card:
           	res=SD_ReadDisk(buff,sector,count);	 
			while(res)//读出错
			{
				if(res!=2)SD_Init();	//重新初始化SD卡
				res=SD_ReadDisk(buff,sector,count);	
				//printf("sd rd error:%d\r\n",res);
			} 
            return RES_OK;			

		case SPI_Flash :
			SPI_W25Qxx_ReadBuffer(buff, sector <<12, count<<12);	// 以4K字节为单位		
            return RES_OK;
	}
	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	uint32_t write_addr;
    u8 res=0;     
	switch (pdrv) {
		
		case SD_Card:						
			res=SD_WriteDisk((u8*)buff,sector,count);
			while(res)//写出错
			{
				SD_Init();	//重新初始化SD卡
				res=SD_WriteDisk((u8*)buff,sector,count);	
			}	
            return RES_OK;            
		case SPI_Flash :
			write_addr = sector<<12; 			// 以4K字节为单位		
			SPI_W25Qxx_SectorErase(write_addr);	
			SPI_W25Qxx_WriteBuffer((uint8_t *)buff,write_addr,count<<12);
            return RES_OK;
	}
	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
    DRESULT res;	
	switch (pdrv) {
		case SD_Card :
            switch(cmd)
            {
                case CTRL_SYNC:
                    res = RES_OK; 
                    break;	 
                case GET_SECTOR_SIZE:
                    *(DWORD*)buff = 512; 
                    res = RES_OK;
                    break;	 
                case GET_BLOCK_SIZE:
                    *(WORD*)buff = SDCardInfo.CardBlockSize;
                    res = RES_OK;
                    break;	 
                case GET_SECTOR_COUNT:
                    *(DWORD*)buff = SDCardInfo.CardCapacity/512;
                    res = RES_OK;
                    break;
                default:
                    res = RES_PARERR;
                    break;
            }
            return res;
		case SPI_Flash :
			switch (cmd) {
				case GET_SECTOR_COUNT:
					*(DWORD * )buff = 2048;		// 总的扇区数
				break;
				
				case GET_SECTOR_SIZE :
					*(WORD * )buff = 4096;		// 定义一个扇区大小为4K
				break;
				
				case GET_BLOCK_SIZE :
					*(DWORD * )buff = 65536;	// 定义一个块大小为64K
				break;        
			}	
		return RES_OK;
	}
	return RES_PARERR;
}
/*-----------------------------------------------------------------------*/
/* 时间获取函数，在这里不使用，返回0                                      */
/*-----------------------------------------------------------------------*/
DWORD get_fattime(void) {

	return 0;
}
