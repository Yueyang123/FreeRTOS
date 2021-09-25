/*
 * @Descripttion: Ӳ��SPI
 * @Version: V1.0
 * @Author: YURI
 * @Email: 1700695611@qq.com
 * @Website: www.heredifferent.com.cn
 * @Date: 2021-08-21 16:47:09
 * @LastEditors: YURI
 * @LastEditTime: 2021-08-21 16:50:40
 */
#ifndef _SPI_
#define _SPI_

#include "sys.h"


/**
 * @name: SPIX_HARD_Init
 * @msg: Ӳ��SPI�ײ��ʼ��
 * @param {SPI_TypeDef*} SPIX
 * @return {*}
 */
void SPIX_HARD_Init(SPI_TypeDef* SPIX);

/**
 * @name: BSP_SPI_CloseTransfer
 * @msg: Ӳ��SPI�ڹرմ����Ĵ���
 * @param {SPI_TypeDef*} SPIX
 * @return {*}
 */
void BSP_SPI_CloseTransfer(SPI_TypeDef* SPIX);

/**
 * @name: BSP_SPI_WaitOnFlagUntilTimeout
 * @msg: Ӳ��SPI�ȴ��������
 * @param {*}
 * @return {*}
 */
u8 BSP_SPI_WaitOnFlagUntilTimeout(SPI_TypeDef* SPIX, uint32_t Flag, FlagStatus Status,
                                  uint32_t Tickstart, uint32_t Timeout);

                            
/**
 * @name: BSP_SPI_TransmitReceive
 * @msg: Ӳ��SPI����ʱ�ģ������շ����ȱ���һ�µĺ���
 * @param {SPI_TypeDef*} SPIX
 * @param {uint8_t} *pTxData
 * @param {uint8_t} *pRxData
 * @param {uint32_t} Size
 * @param {uint32_t} Timeout
 * @return {*}
 */
u8 BSP_SPI_TransmitReceive(SPI_TypeDef* SPIX, uint8_t *pTxData, uint8_t *pRxData, uint32_t Size,uint32_t Timeout);


/**
 * @name: BSP_SPI_TransmitReceive_STA
 * @msg: ���ͽ��ճ��ȿ��Բ�һ��
 * @param {SPI_TypeDef*} SPIX
 * @param {uint8_t} *pTxData
 * @param {uint8_t} *pRxData
 * @param {uint32_t} TxSize
 * @param {uint32_t} RxSize
 * @return {*}
 */
u8 BSP_SPI_TransmitReceive_STA(SPI_TypeDef* SPIX, uint8_t *pTxData, uint8_t *pRxData, uint32_t TxSize,uint32_t RxSize);

#endif