/*
 * @Descripttion: 根据HAL库修改的LTDC驱动文件，适用于H7平台
 * @Version: V1.0
 * @Author: YURI
 * @Email: 1700695611@qq.com
 * @Website: www.heredifferent.com.cn
 * @Date: 2021-08-20 12:31:05
 * @LastEditors: YURI
 * @LastEditTime: 2021-08-21 06:29:27
 */
#ifndef _LTDC_H
#define _LTDC_H

#include "sys.h"

#define DMA2D_CR_MODE_Pos          (16U)
#define DMA2D_CR_MODE_0            (0x1UL << DMA2D_CR_MODE_Pos)                 /*!< 0x00010000 */
#define DMA2D_CR_MODE_1            (0x2UL << DMA2D_CR_MODE_Pos)                 /*!< 0x00020000 */
#define DMA2D_CR_MODE_2            (0x4UL << DMA2D_CR_MODE_Pos)                 /*!< 0x00040000 */
#define DMA2D_R2M                   (DMA2D_CR_MODE_1 | DMA2D_CR_MODE_0) 

/**
  * @brief  HAL Lock structures definition
  */
typedef enum
{
  BSP_UNLOCKED = 0x00,
  BSP_LOCKED   = 0x01
} BSP_LockTypeDef;

#define MAX_LAYER  2U

typedef struct
{
  uint8_t Blue;                    /*!< Configures the blue value.
                                        This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF. */
  uint8_t Green;                   /*!< Configures the green value.
                                        This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF. */
  uint8_t Red;                     /*!< Configures the red value.
                                        This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF. */
  uint8_t Reserved;                /*!< Reserved 0xFF */
} BSP_LTDC_ColorTypeDef;

/**
  * @brief  LTDC Init structure definition
  */
typedef struct
{
  uint32_t            HSPolarity;                /*!< configures the horizontal synchronization polarity.
                                                      This parameter can be one value of @ref LTDC_HS_POLARITY */

  uint32_t            VSPolarity;                /*!< configures the vertical synchronization polarity.
                                                      This parameter can be one value of @ref LTDC_VS_POLARITY */

  uint32_t            DEPolarity;                /*!< configures the data enable polarity.
                                                      This parameter can be one of value of @ref LTDC_DE_POLARITY */

  uint32_t            PCPolarity;                /*!< configures the pixel clock polarity.
                                                      This parameter can be one of value of @ref LTDC_PC_POLARITY */

  uint32_t            HorizontalSync;            /*!< configures the number of Horizontal synchronization width.
                                                      This parameter must be a number between Min_Data = 0x000 and Max_Data = 0xFFF. */

  uint32_t            VerticalSync;              /*!< configures the number of Vertical synchronization height.
                                                      This parameter must be a number between Min_Data = 0x000 and Max_Data = 0x7FF. */

  uint32_t            AccumulatedHBP;            /*!< configures the accumulated horizontal back porch width.
                                                      This parameter must be a number between Min_Data = LTDC_HorizontalSync and Max_Data = 0xFFF. */

  uint32_t            AccumulatedVBP;            /*!< configures the accumulated vertical back porch height.
                                                      This parameter must be a number between Min_Data = LTDC_VerticalSync and Max_Data = 0x7FF. */

  uint32_t            AccumulatedActiveW;        /*!< configures the accumulated active width.
                                                      This parameter must be a number between Min_Data = LTDC_AccumulatedHBP and Max_Data = 0xFFF. */

  uint32_t            AccumulatedActiveH;        /*!< configures the accumulated active height.
                                                      This parameter must be a number between Min_Data = LTDC_AccumulatedVBP and Max_Data = 0x7FF. */

  uint32_t            TotalWidth;                /*!< configures the total width.
                                                      This parameter must be a number between Min_Data = LTDC_AccumulatedActiveW and Max_Data = 0xFFF. */

  uint32_t            TotalHeigh;                /*!< configures the total height.
                                                      This parameter must be a number between Min_Data = LTDC_AccumulatedActiveH and Max_Data = 0x7FF. */

  BSP_LTDC_ColorTypeDef   Backcolor;                 /*!< Configures the background color. */
} BSP_LTDC_InitTypeDef;

/**
  * @brief  LTDC Layer structure definition
  */
typedef struct
{
  uint32_t WindowX0;                   /*!< Configures the Window Horizontal Start Position.
                                            This parameter must be a number between Min_Data = 0x000 and Max_Data = 0xFFF. */

  uint32_t WindowX1;                   /*!< Configures the Window Horizontal Stop Position.
                                            This parameter must be a number between Min_Data = 0x000 and Max_Data = 0xFFF. */

  uint32_t WindowY0;                   /*!< Configures the Window vertical Start Position.
                                            This parameter must be a number between Min_Data = 0x000 and Max_Data = 0x7FF. */

  uint32_t WindowY1;                   /*!< Configures the Window vertical Stop Position.
                                            This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0x7FF. */

  uint32_t PixelFormat;                /*!< Specifies the pixel format.
                                            This parameter can be one of value of @ref LTDC_Pixelformat */

  uint32_t Alpha;                      /*!< Specifies the constant alpha used for blending.
                                            This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF. */

  uint32_t Alpha0;                     /*!< Configures the default alpha value.
                                            This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF. */

  uint32_t BlendingFactor1;            /*!< Select the blending factor 1.
                                            This parameter can be one of value of @ref LTDC_BlendingFactor1 */

  uint32_t BlendingFactor2;            /*!< Select the blending factor 2.
                                            This parameter can be one of value of @ref LTDC_BlendingFactor2 */

  uint32_t FBStartAdress;              /*!< Configures the color frame buffer address */

  uint32_t ImageWidth;                 /*!< Configures the color frame buffer line length.
                                            This parameter must be a number between Min_Data = 0x0000 and Max_Data = 0x1FFF. */

  uint32_t ImageHeight;                /*!< Specifies the number of line in frame buffer.
                                            This parameter must be a number between Min_Data = 0x000 and Max_Data = 0x7FF. */

  BSP_LTDC_ColorTypeDef   Backcolor;       /*!< Configures the layer background color. */
} BSP_LTDC_LayerCfgTypeDef;

typedef enum
{
  BSP_LTDC_STATE_RESET             = 0x00U,    /*!< LTDC not yet initialized or disabled */
  BSP_LTDC_STATE_READY             = 0x01U,    /*!< LTDC initialized and ready for use   */
  BSP_LTDC_STATE_BUSY              = 0x02U,    /*!< LTDC internal process is ongoing     */
  BSP_LTDC_STATE_TIMEOUT           = 0x03U,    /*!< LTDC Timeout state                   */
  BSP_LTDC_STATE_ERROR             = 0x04U     /*!< LTDC state error                     */
} BSP_LTDC_StateTypeDef;

typedef struct
{
  LTDC_TypeDef                *Instance;                /*!< LTDC Register base address                */
  BSP_LTDC_InitTypeDef            Init;                     /*!< LTDC parameters                           */
  BSP_LTDC_LayerCfgTypeDef        LayerCfg[MAX_LAYER];      /*!< LTDC Layers parameters                    */
  BSP_LockTypeDef             Lock;                     /*!< LTDC Lock                                 */
  __IO BSP_LTDC_StateTypeDef  State;                    /*!< LTDC state                                */
  __IO uint32_t               ErrorCode;                /*!< LTDC Error code                           */
} BSP_LTDC_HandleTypeDef;

#define BSP_LTDC_ERROR_NONE               0x00000000U   /*!< LTDC No error             */
#define BSP_LTDC_ERROR_TE                 0x00000001U   /*!< LTDC Transfer error       */
#define BSP_LTDC_ERROR_FU                 0x00000002U   /*!< LTDC FIFO Underrun        */
#define BSP_LTDC_ERROR_TIMEOUT            0x00000020U   /*!< LTDC Timeout error        */

#define LTDC_LAYER_1                      0x00000000U   /*!< LTDC Layer 1 */
#define LTDC_LAYER_2                      0x00000001U   /*!< LTDC Layer 2 */

#define LTDC_HSPOLARITY_AL                0x00000000U   /*!< Horizontal Synchronization is active low. */
#define LTDC_HSPOLARITY_AH                LTDC_GCR_HSPOL            /*!< Horizontal Synchronization is active high. */

#define LTDC_VSPOLARITY_AL                0x00000000U   /*!< Vertical Synchronization is active low. */
#define LTDC_VSPOLARITY_AH                LTDC_GCR_VSPOL            /*!< Vertical Synchronization is active high. */

#define LTDC_DEPOLARITY_AL                0x00000000U   /*!< Data Enable, is active low. */
#define LTDC_DEPOLARITY_AH                LTDC_GCR_DEPOL            /*!< Data Enable, is active high. */

#define LTDC_PCPOLARITY_IPC               0x00000000U   /*!< input pixel clock. */
#define LTDC_PCPOLARITY_IIPC              LTDC_GCR_PCPOL            /*!< inverted input pixel clock. */

#define LTDC_HORIZONTALSYNC               (LTDC_SSCR_HSW >> 16U)    /*!< Horizontal synchronization width. */
#define LTDC_VERTICALSYNC                 LTDC_SSCR_VSH             /*!< Vertical synchronization height. */

#define LTDC_COLOR                        0x000000FFU   /*!< Color mask */

#define LTDC_BLENDING_FACTOR1_CA          0x00000400U   /*!< Blending factor : Cte Alpha */
#define LTDC_BLENDING_FACTOR1_PAxCA       0x00000600U   /*!< Blending factor : Cte Alpha x Pixel Alpha*/

#define LTDC_BLENDING_FACTOR2_CA          0x00000005U   /*!< Blending factor : Cte Alpha */
#define LTDC_BLENDING_FACTOR2_PAxCA       0x00000007U   /*!< Blending factor : Cte Alpha x Pixel Alpha*/

#define LTDC_PIXEL_FORMAT_ARGB8888        0x00000000U   /*!< ARGB8888 LTDC pixel format */
#define LTDC_PIXEL_FORMAT_RGB888          0x00000001U   /*!< RGB888 LTDC pixel format   */
#define LTDC_PIXEL_FORMAT_RGB565          0x00000002U   /*!< RGB565 LTDC pixel format   */
#define LTDC_PIXEL_FORMAT_ARGB1555        0x00000003U   /*!< ARGB1555 LTDC pixel format */
#define LTDC_PIXEL_FORMAT_ARGB4444        0x00000004U   /*!< ARGB4444 LTDC pixel format */
#define LTDC_PIXEL_FORMAT_L8              0x00000005U   /*!< L8 LTDC pixel format       */
#define LTDC_PIXEL_FORMAT_AL44            0x00000006U   /*!< AL44 LTDC pixel format     */
#define LTDC_PIXEL_FORMAT_AL88            0x00000007U   /*!< AL88 LTDC pixel format     */

#define LTDC_ALPHA                        LTDC_LxCACR_CONSTA        /*!< LTDC Constant Alpha mask */

#define LTDC_STOPPOSITION                 (LTDC_LxWHPCR_WHSPPOS >> 16U) /*!< LTDC Layer stop position  */
#define LTDC_STARTPOSITION                LTDC_LxWHPCR_WHSTPOS          /*!< LTDC Layer start position */

#define LTDC_COLOR_FRAME_BUFFER           LTDC_LxCFBLR_CFBLL            /*!< LTDC Layer Line length    */
#define LTDC_LINE_NUMBER                  LTDC_LxCFBLNR_CFBLNBR         /*!< LTDC Layer Line number    */

#define LTDC_IT_LI                        LTDC_IER_LIE              /*!< LTDC Line Interrupt            */
#define LTDC_IT_FU                        LTDC_IER_FUIE             /*!< LTDC FIFO Underrun Interrupt   */
#define LTDC_IT_TE                        LTDC_IER_TERRIE           /*!< LTDC Transfer Error Interrupt  */
#define LTDC_IT_RR                        LTDC_IER_RRIE             /*!< LTDC Register Reload Interrupt */

#define LTDC_FLAG_LI                      LTDC_ISR_LIF              /*!< LTDC Line Interrupt Flag            */
#define LTDC_FLAG_FU                      LTDC_ISR_FUIF             /*!< LTDC FIFO Underrun interrupt Flag   */
#define LTDC_FLAG_TE                      LTDC_ISR_TERRIF           /*!< LTDC Transfer Error interrupt Flag  */
#define LTDC_FLAG_RR                      LTDC_ISR_RRIF             /*!< LTDC Register Reload interrupt Flag */

#define LTDC_RELOAD_IMMEDIATE             LTDC_SRCR_IMR             /*!< Immediate Reload */
#define LTDC_RELOAD_VERTICAL_BLANKING     LTDC_SRCR_VBR             /*!< Vertical Blanking Reload */

#define __BSP_LTDC_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = BSP_LTDC_STATE_RESET)
#define __BSP_LTDC_ENABLE(__HANDLE__)    ((__HANDLE__)->Instance->GCR |= LTDC_GCR_LTDCEN)
#define __BSP_LTDC_DISABLE(__HANDLE__)   ((__HANDLE__)->Instance->GCR &= ~(LTDC_GCR_LTDCEN))
#define __BSP_LTDC_LAYER_ENABLE(__HANDLE__, __LAYER__)  ((LTDC_LAYER((__HANDLE__), (__LAYER__)))->CR |= (uint32_t)LTDC_LxCR_LEN)
#define __BSP_LTDC_LAYER_DISABLE(__HANDLE__, __LAYER__) ((LTDC_LAYER((__HANDLE__), (__LAYER__)))->CR &= ~(uint32_t)LTDC_LxCR_LEN)
#define __BSP_LTDC_RELOAD_IMMEDIATE_CONFIG(__HANDLE__)  ((__HANDLE__)->Instance->SRCR |= LTDC_SRCR_IMR)
#define __BSP_LTDC_VERTICAL_BLANKING_RELOAD_CONFIG(__HANDLE__)  ((__HANDLE__)->Instance->SRCR |= LTDC_SRCR_VBR)
#define __BSP_LTDC_GET_FLAG(__HANDLE__, __FLAG__) ((__HANDLE__)->Instance->ISR & (__FLAG__))
#define __BSP_LTDC_CLEAR_FLAG(__HANDLE__, __FLAG__) ((__HANDLE__)->Instance->ICR = (__FLAG__))
#define __BSP_LTDC_ENABLE_IT(__HANDLE__, __INTERRUPT__) ((__HANDLE__)->Instance->IER |= (__INTERRUPT__))
#define __BSP_LTDC_DISABLE_IT(__HANDLE__, __INTERRUPT__) ((__HANDLE__)->Instance->IER &= ~(__INTERRUPT__))
#define __BSP_LTDC_GET_IT_SOURCE(__HANDLE__, __INTERRUPT__) ((__HANDLE__)->Instance->IER & (__INTERRUPT__))

uint8_t BSP_LTDC_Init(BSP_LTDC_HandleTypeDef *hltdc);
uint8_t BSP_LTDC_DeInit(BSP_LTDC_HandleTypeDef *hltdc);
void BSP_LTDC_MspInit(BSP_LTDC_HandleTypeDef *hltdc);
void BSP_LTDC_MspDeInit(BSP_LTDC_HandleTypeDef *hltdc);
void BSP_LTDC_ErrorCallback(BSP_LTDC_HandleTypeDef *hltdc);
void BSP_LTDC_LineEventCallback(BSP_LTDC_HandleTypeDef *hltdc);
void BSP_LTDC_ReloadEventCallback(BSP_LTDC_HandleTypeDef *hltdc);
uint8_t BSP_LTDC_Clk_Set(u32 pll3n,u32 pll3m,u32 pll3r);
uint8_t BSP_LTDC_ConfigLayer(BSP_LTDC_HandleTypeDef *hltdc, BSP_LTDC_LayerCfgTypeDef *pLayerCfg, uint32_t LayerIdx);

#define UNUSED(x) ((void)(x))
BSP_LTDC_StateTypeDef BSP_LTDC_GetState(BSP_LTDC_HandleTypeDef *hltdc);
uint32_t              BSP_LTDC_GetError(BSP_LTDC_HandleTypeDef *hltdc);
#define LTDC_LAYER(__HANDLE__, __LAYER__)              ((LTDC_Layer_TypeDef *)((uint32_t)(((uint32_t)((__HANDLE__)->Instance)) + 0x84U + (0x80U*(__LAYER__)))))


#endif
