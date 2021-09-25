/*
 * @Descripttion: 
 * @Version: V1.0
 * @Author: YURI
 * @Email: 1700695611@qq.com
 * @Website: www.heredifferent.com.cn
 * @Date: 2021-08-21 16:47:04
 * @LastEditors: YURI
 * @LastEditTime: 2021-08-21 16:47:26
 */
#include "SPI.h"
#include "delay.h"
/******************************************************  
      PA15    ------> SPI6_NSS ，使用硬件片选
      PB3     ------> SPI6_SCK
      PB4     ------> SPI6_MISO
      PB5     ------> SPI6_MOSI
    // 这里将 pll2_q_ck 设置为 100M 作为 SPI6 的内核时钟，然后再经过2分频得到 50M 的SCK驱动时钟
    CFG1:
    hspi6.Init.BaudRatePrescaler 		= SPI_BAUDRATEPRESCALER_2;          //	2分频
    0<<28
    hspi6.Init.CRCCalculation 			= SPI_CRCCALCULATION_DISABLE;       //	禁止CRC
	0<<22
    hspi6.Init.FifoThreshold 		  	= SPI_FIFO_THRESHOLD_02DATA;        //	FIFO设置为2个数据，设置成其他值运行在外部flash会出错
    1<<5
    hspi6.Init.DataSize 				  	= SPI_DATASIZE_8BIT;                //	8位数据宽度
    7<<0
	
    CFG2:			    
	hspi6.Init.Mode						= SPI_MODE_MASTER;  
    1<<22             //	主机模式
	hspi6.Init.Direction 			 	= SPI_DIRECTION_2LINES;             //	双线全双工
	0<<17  
    hspi6.Init.CLKPolarity 			  	= SPI_POLARITY_LOW;                 //	CLK空闲时保持低电平
	0<<25
    hspi6.Init.CLKPhase 				  	= SPI_PHASE_1EDGE; 
    0<<24                 //	数据在CLK第一个边沿有效
	hspi6.Init.NSS 					   = SPI_NSS_HARD_OUTPUT;    //	使用硬件片选   
    1<<29
	hspi6.Init.FirstBit 				  	= SPI_FIRSTBIT_MSB;                 //	高位在先
	0<<23
    hspi6.Init.TIMode 			  	  	= SPI_TIMODE_DISABLE;               //	禁止TI模式
	0<<19
    hspi6.Init.NSSPMode 				  	= SPI_NSS_PULSE_DISABLE;            //	不使用片选脉冲模式
	0<<30
    hspi6.Init.NSSPolarity 			  	= SPI_NSS_POLARITY_LOW;             //	片选低电平有效
	0<<28
    hspi6.Init.MasterKeepIOState 		= SPI_MASTER_KEEP_IO_STATE_ENABLE;  //	主机模式下，禁止SPI后保持当前引脚状态
    CR1 :
    1<<0
*******************************************************/

/**
 * @name: SPIX_HARD_Init
 * @msg: 硬件SPI底层初始化
 * @param {SPI_TypeDef*} SPIX
 * @return {*}
 */
void SPIX_HARD_Init(SPI_TypeDef* SPIX)
{
    RCC->APB4ENR|=1<<5;//使能SPI6时钟
    RCC->AHB4ENR|=(1<<0)|(1<<1);//PA PB使能
    SPIX->CFG1|=(1<<5)|(7<<0);
    SPIX->CFG1&=~((1<<28)|(1<<22));
    SPIX->CFG2|=(1<<22)|(1<<29);
    SPIX->CFG2&=~((1<<17)|(1<<25)|(1<<24)|(1<<23)|(1<<19)|(1<<30)|(1<<28));
    SPIX->CR1|=1<<0;
    SPIX->I2SCFGR&=~(1<<0);
    GPIO_Set(GPIOA,PIN15,GPIO_MODE_AF,GPIO_OTYPE_PP, GPIO_SPEED_FAST,GPIO_PUPD_NONE);
    GPIO_AF_Set(GPIOA,15,7);
    GPIO_Set(GPIOB,PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP, GPIO_SPEED_FAST,GPIO_PUPD_NONE);
    GPIO_Set(GPIOB,PIN4,GPIO_MODE_AF,GPIO_OTYPE_PP, GPIO_SPEED_FAST,GPIO_PUPD_NONE);
    GPIO_Set(GPIOB,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP, GPIO_SPEED_FAST,GPIO_PUPD_NONE);
    GPIO_AF_Set(GPIOB,3,8);
    GPIO_AF_Set(GPIOB,4,8);
    GPIO_AF_Set(GPIOB,5,8);
}
/**
 * @name: BSP_SPI_CloseTransfer
 * @msg: 硬件SPI在关闭传输后的处理
 * @param {SPI_TypeDef*} SPIX
 * @return {*}
 */
void BSP_SPI_CloseTransfer(SPI_TypeDef* SPIX)
{
    uint32_t itflag = SPIX->SR;
    SPIX->IFCR|=1<<3;
    SPIX->IFCR|=1<<4;
    SPIX->CR1&=~(1<<0);
    SPIX->IER &= (~(((0x1UL << (3U)) | (0x1UL << (1U)) | (0x1UL << (0U)) | (0x1UL << (2U)) | (0x1UL << (5U)) | (0x1UL << (6U)) | (0x1UL << (8U)) | (0x1UL << (9U)))));
    SPIX->CFG1&= ~((0x1UL << (15U)) | (0x1UL << (14U)));
     if ((itflag & (1<<5)) != 0UL)
    {
      SPIX->IFCR|=(1<<5);
    }
      if ((itflag & (1<<6)) != 0UL)
    {
      SPIX->IFCR|=(1<<6);
    }   
      if ((itflag & (1<<9)) != 0UL)
    {
      SPIX->IFCR|=(1<<9);
    } 
      if ((itflag & (1<<8)) != 0UL)
    {
      SPIX->IFCR|=(1<<8);
    } 
}
/**
 * @name: BSP_SPI_WaitOnFlagUntilTimeout
 * @msg: 硬件SPI等待传输完成
 * @param {*}
 * @return {*}
 */
u8 BSP_SPI_WaitOnFlagUntilTimeout(SPI_TypeDef* SPIX, uint32_t Flag, FlagStatus Status,
                                  uint32_t Tickstart, uint32_t Timeout)
{
   while( ((((SPIX->SR) &Flag) ==Flag) ? SET : RESET) == Status)
   {
      if ((((Tick_Times() - Tickstart) >=  Timeout)))
      {
         return 3;
      }
   }
   return 0;  
}
/**
 * @name: BSP_SPI_TransmitReceive
 * @msg: 硬件SPI带延时的，但是收发长度必须一致的函数
 * @param {SPI_TypeDef*} SPIX
 * @param {uint8_t} *pTxData
 * @param {uint8_t} *pRxData
 * @param {uint32_t} Size
 * @param {uint32_t} Timeout
 * @return {*}
 */
u8 BSP_SPI_TransmitReceive(SPI_TypeDef* SPIX, uint8_t *pTxData, uint8_t *pRxData, uint32_t Size,uint32_t Timeout)
{
    uint32_t    tickstart;
    uint32_t    initial_TxXferCount;
    uint32_t    initial_RxXferCount;
    uint8_t* pRxBuffPtr,*pTxBuffPtr;
    tickstart=Tick_Times();

    SPIX->CR1|=1<<0;    
    //开始传输
    SPIX->CR1|=1<<9;
    initial_TxXferCount = Size;
    initial_RxXferCount = Size; 
    pRxBuffPtr  = (uint8_t *)pRxData;
    pTxBuffPtr  = (uint8_t *)pTxData;
    while ((initial_TxXferCount > 0UL) || (initial_RxXferCount > 0UL))
    {
        /* check TXP flag */
        if (((SPIX->SR & (1<<1))!=0) && (initial_TxXferCount > 0UL))
        {
            if(initial_TxXferCount > 1UL){
                *((__IO uint16_t *)&SPIX->TXDR) = *((uint16_t *)pTxBuffPtr);
                pTxBuffPtr += sizeof(uint16_t);
                initial_TxXferCount-=(uint16_t)2UL;   
            }else{
                *((__IO uint8_t *)&SPIX->TXDR) = *((uint8_t *)pTxBuffPtr);
                pTxBuffPtr += sizeof(uint8_t);
                initial_TxXferCount--;                 
            }              
        }
           /* Wait until RXWNE/FRLVL flag is reset */
          if ((((SPIX->SR & ((1<<15)|(3<<13))) != 0UL) && (initial_RxXferCount > 0UL)))
          {
            if ((SPIX->SR & (1<<15)) != 0UL)
            {
                *((uint32_t *)pRxBuffPtr) = *((__IO uint32_t *)&SPIX->RXDR);
                pRxBuffPtr += sizeof(uint32_t);  
                initial_RxXferCount -= (uint16_t)4UL;		          
            }
            else if ((SPIX->SR & (3<<13)) > (1<<13))
            {
                *((uint16_t *)pRxBuffPtr) = *((__IO uint16_t *)&SPIX->RXDR);
                pRxBuffPtr += sizeof(uint16_t);
                initial_RxXferCount -= (uint16_t)2UL;	         
            }
            else
            {
                *((uint8_t *)pRxBuffPtr) = *((__IO uint8_t *)&SPIX->RXDR);
                pRxBuffPtr += sizeof(uint8_t);
                initial_RxXferCount --;         
            }
          }
          /* Timeout management */
          if ((((Tick_Times() - tickstart) >=  Timeout)) || (Timeout == 0U))
          {	
             BSP_SPI_CloseTransfer(SPIX);
             return 1;
          }
    }
    
    SPIX->CR1|= ((0x1UL << (10U)));
   /* 等待SPI挂起 */
   if (BSP_SPI_WaitOnFlagUntilTimeout(SPIX, (1<<11), RESET, tickstart, Timeout) != 0)
   {
   }
    BSP_SPI_CloseTransfer(SPIX);
    SPIX->IFCR|= ((0x1UL << (11U)));
    
    return 0;    
}


/**
 * @name: BSP_SPI_TransmitReceive_STA
 * @msg: 发送接收长度可以不一致
 * @param {SPI_TypeDef*} SPIX
 * @param {uint8_t} *pTxData
 * @param {uint8_t} *pRxData
 * @param {uint32_t} TxSize
 * @param {uint32_t} RxSize
 * @return {*}
 */
u8 BSP_SPI_TransmitReceive_STA(SPI_TypeDef* SPIX, uint8_t *pTxData, uint8_t *pRxData, uint32_t TxSize,uint32_t RxSize)
{
    uint32_t    tickstart;
    uint32_t    Timeout=1000;
    uint32_t    initial_TxXferCount;
    uint32_t    initial_RxXferCount;
    uint8_t* pRxBuffPtr,*pTxBuffPtr;
    tickstart=Tick_Times();

    SPIX->CR1|=1<<0;    
    //开始传输
    SPIX->CR1|=1<<9;
    //先传输，排出在发送中收到的数据
    initial_TxXferCount = TxSize;
    initial_RxXferCount = TxSize; 
    pRxBuffPtr  = (uint8_t *)pRxData;
    pTxBuffPtr  = (uint8_t *)pTxData;
    while ((initial_TxXferCount > 0UL) || (initial_RxXferCount > 0UL))
    {
        /* check TXP flag */
        if (((SPIX->SR & (1<<1))!=0) && (initial_TxXferCount > 0UL))
        {
            if(initial_TxXferCount > 1UL){
                *((__IO uint16_t *)&SPIX->TXDR) = *((uint16_t *)pTxBuffPtr);
                pTxBuffPtr += sizeof(uint16_t);
                initial_TxXferCount-=(uint16_t)2UL;   
            }else{
                *((__IO uint8_t *)&SPIX->TXDR) = *((uint8_t *)pTxBuffPtr);
                pTxBuffPtr += sizeof(uint8_t);
                initial_TxXferCount--;                 
            }              
        }
           /* Wait until RXWNE/FRLVL flag is reset */
          if ((((SPIX->SR & ((1<<15)|(3<<13))) != 0UL) && (initial_RxXferCount > 0UL)))
          {
            if ((SPIX->SR & (1<<15)) != 0UL)
            {
                *((uint32_t *)pRxBuffPtr) = *((__IO uint32_t *)&SPIX->RXDR);
                pRxBuffPtr += sizeof(uint32_t);  
                initial_RxXferCount -= (uint16_t)4UL;		          
            }
            else if ((SPIX->SR & (3<<13)) > (1<<13))
            {
                *((uint16_t *)pRxBuffPtr) = *((__IO uint16_t *)&SPIX->RXDR);
                pRxBuffPtr += sizeof(uint16_t);
                initial_RxXferCount -= (uint16_t)2UL;	         
            }
            else
            {
                *((uint8_t *)pRxBuffPtr) = *((__IO uint8_t *)&SPIX->RXDR);
                pRxBuffPtr += sizeof(uint8_t);
                initial_RxXferCount --;         
            }
          }
          /* Timeout management */
          if ((((Tick_Times() - tickstart) >=  Timeout)) || (Timeout == 0U))
          {	
             BSP_SPI_CloseTransfer(SPIX);
             return 1;
          }
    }
    

   //先传输，排出在发送中收到的数据
    initial_TxXferCount = RxSize;
    initial_RxXferCount = RxSize; 
    pRxBuffPtr  = (uint8_t *)pRxData;
    pTxBuffPtr  = (uint8_t *)pTxData;
    while ((initial_TxXferCount > 0UL) || (initial_RxXferCount > 0UL))
    {
        /* check TXP flag */
        if (((SPIX->SR & (1<<1))!=0) && (initial_TxXferCount > 0UL))
        {
            if(initial_TxXferCount > 1UL){
                *((__IO uint16_t *)&SPIX->TXDR) = *((uint16_t *)pTxBuffPtr);
                //pTxBuffPtr += sizeof(uint16_t);
                initial_TxXferCount-=(uint16_t)2UL;   
            }else{
                *((__IO uint8_t *)&SPIX->TXDR) = *((uint8_t *)pTxBuffPtr);
                //pTxBuffPtr += sizeof(uint8_t);
                initial_TxXferCount--;                 
            }              
        }
           /* Wait until RXWNE/FRLVL flag is reset */
          if ((((SPIX->SR & ((1<<15)|(3<<13))) != 0UL) && (initial_RxXferCount > 0UL)))
          {
            if ((SPIX->SR & (1<<15)) != 0UL)
            {
                *((uint32_t *)pRxBuffPtr) = *((__IO uint32_t *)&SPIX->RXDR);
                pRxBuffPtr += sizeof(uint32_t);  
                initial_RxXferCount -= (uint16_t)4UL;		          
            }
            else if ((SPIX->SR & (3<<13)) > (1<<13))
            {
                *((uint16_t *)pRxBuffPtr) = *((__IO uint16_t *)&SPIX->RXDR);
                pRxBuffPtr += sizeof(uint16_t);
                initial_RxXferCount -= (uint16_t)2UL;	         
            }
            else
            {
                *((uint8_t *)pRxBuffPtr) = *((__IO uint8_t *)&SPIX->RXDR);
                pRxBuffPtr += sizeof(uint8_t);
                initial_RxXferCount --;         
            }
          }
          /* Timeout management */
          if ((((Tick_Times() - tickstart) >=  Timeout)) || (Timeout == 0U))
          {	
             BSP_SPI_CloseTransfer(SPIX);
             return 1;
          }
    }
    
    SPIX->CR1|= ((0x1UL << (10U)));
   /* 等待SPI挂起 */
   if (BSP_SPI_WaitOnFlagUntilTimeout(SPIX, (1<<11), RESET, tickstart, Timeout) != 0)
   {
   }
    BSP_SPI_CloseTransfer(SPIX);
    SPIX->IFCR|= ((0x1UL << (11U)));
    
    return 0;    
}


