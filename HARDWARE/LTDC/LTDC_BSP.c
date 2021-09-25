/*
 * @Descripttion: 根据HAL库修改的LTDC驱动文件，适用于H7平台
 * @Version: V1.0
 * @Author: YURI
 * @Email: 1700695611@qq.com
 * @Website: www.heredifferent.com.cn
 * @Date: 2021-08-20 11:17:45
 * @LastEditors: YURI
 * @LastEditTime: 2021-08-21 06:28:59
 */
#include "sys.h"
#include "LTDC_BSP.h"


 //LTDC时钟(Fdclk)设置函数
 //Fvco=Fs*(pll3n/pll3m); 
 //Fr=Fvco/pll3r=Fs*(pll3n/(pll3m*pll3r));
 //Fdclk=Fr;
 //Fvco:VCO频率
 //Fr:PLL3的r分频输出时钟频率
 //Fs:PLL3输入时钟频率,可以是HSI,CSI,HSE等. 
 //Fdclk:LTDC的输入时钟

 //pll3n:PLL3倍频系数(PLL倍频),取值范围:4~512.
 //pll3m:PLL3预分频系数(进PLL之前的分频),取值范围:2~63.
 //pll3r:PLL3的r分频系数(PLL之后的分频),取值范围:1~128.

 //因为LTDC和SAI共用pll3,所以,当使用SAI的时候,会切换不同的pll3n(一般会
 //大于300),导致LTDC的plln也变化了,因此这里设置为300,得到的像素时钟在
 //后面SAI修改pll3n的时候,只会大,不会小.不会降低LCD的刷新频率.
 //因此,LTDC,我们一般设置pll3m=25,pll3n=300,这样,可以得到Fvco=300Mhz  
 //然后,只需要通过修改pll3r,来匹配不同的液晶屏时序即可.

 //例如:要得到33M的LTDC时钟,则可以设置:pll3n=300,pll3m=25,pll3r=9
 //Fdclk=300*(25/25)/9=300/9=33.33Mhz
 //返回值:0,成功;1,失败。
uint8_t BSP_LTDC_Clk_Set(u32 pll3n,u32 pll3m,u32 pll3r)
 { 
 	u16 retry=0;
 	u8 status=0; 
 	RCC->CR&=~(1<<28);				//关闭PLL3时钟 
 	while(((RCC->CR&(1<<29)))&&(retry<0X1FFF))retry++;//等待PLL3时钟失锁
  	if(retry==0X1FFF)status=1;		//PLL3时钟关闭失败
 	else   
 	{ 
 		RCC->PLLCKSELR&=~(0X3F<<20);	//清除DIVM3[5:0]原来的设置
 		RCC->PLLCKSELR|=pll3m<<20;		//DIVM3[5:0]=25,设置PLL3的预分频系数
 		RCC->PLL3DIVR&=~(0X1FF<<0);		//清除DIVN3[8:0]原来的设置
 		RCC->PLL3DIVR|=(pll3n-1)<<0;	//DIVN3[8:0]=pll3n-1,设置PLL3的倍频系数,设置值需减1
 		RCC->PLL3DIVR&=~(0X7F<<24);		//清除DIVR2[6:0]原来的设置
 		RCC->PLL3DIVR|=(pll3r-1)<<24;	//DIVR3[8:0]=pll3r-1,设置PLL3的倍频系数,设置值需减1  

 		RCC->PLLCFGR&=~(0X0F<<8);		//清除PLL3RGE[1:0]/PLL3VCOSEL/PLL3FRACEN的设置
 		RCC->PLLCFGR|=0<<10;			//PLL3RGE[1:0]=0,PLL3输入时钟频率在1~2Mhz之间(25/25=1Mhz)
 		RCC->PLLCFGR|=0<<9;				//PLL3VCOSEL=0,PLL3宽的VCO范围,192~836Mhz 
 		RCC->PLLCFGR|=1<<24;			//DIVR3EN=1,使能pll3_r_ck  
 		RCC->CR|=1<<28;					//PLL3ON=1,使能PLL3
 		while(((RCC->CR&(1<<29))==0)&&(retry<0X1FFF))retry++;	//等待PLL3时钟锁定
 		if(retry==0X1FFF)status=2;	
  	} 
 	return status;
 } 


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void BSP_LTDC_SetConfig(BSP_LTDC_HandleTypeDef *hltdc, BSP_LTDC_LayerCfgTypeDef *pLayerCfg, uint32_t LayerIdx);

uint8_t BSP_LTDC_Init(BSP_LTDC_HandleTypeDef *hltdc)
{
  uint32_t tmp, tmp1;

  /* Check the LTDC peripheral state */
  if (hltdc == 0)
  {
    return 1;
  }


  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Configure the HS, VS, DE and PC polarity */
  hltdc->Instance->GCR &= ~(LTDC_GCR_HSPOL | LTDC_GCR_VSPOL | LTDC_GCR_DEPOL | LTDC_GCR_PCPOL);
  hltdc->Instance->GCR |= (uint32_t)(hltdc->Init.HSPolarity | hltdc->Init.VSPolarity | \
                                     hltdc->Init.DEPolarity | hltdc->Init.PCPolarity);

  /* Set Synchronization size */
  hltdc->Instance->SSCR &= ~(LTDC_SSCR_VSH | LTDC_SSCR_HSW);
  tmp = (hltdc->Init.HorizontalSync << 16U);
  hltdc->Instance->SSCR |= (tmp | hltdc->Init.VerticalSync);

  /* Set Accumulated Back porch */
  hltdc->Instance->BPCR &= ~(LTDC_BPCR_AVBP | LTDC_BPCR_AHBP);
  tmp = (hltdc->Init.AccumulatedHBP << 16U);
  hltdc->Instance->BPCR |= (tmp | hltdc->Init.AccumulatedVBP);

  /* Set Accumulated Active Width */
  hltdc->Instance->AWCR &= ~(LTDC_AWCR_AAH | LTDC_AWCR_AAW);
  tmp = (hltdc->Init.AccumulatedActiveW << 16U);
  hltdc->Instance->AWCR |= (tmp | hltdc->Init.AccumulatedActiveH);

  /* Set Total Width */
  hltdc->Instance->TWCR &= ~(LTDC_TWCR_TOTALH | LTDC_TWCR_TOTALW);
  tmp = (hltdc->Init.TotalWidth << 16U);
  hltdc->Instance->TWCR |= (tmp | hltdc->Init.TotalHeigh);

  /* Set the background color value */
  tmp = ((uint32_t)(hltdc->Init.Backcolor.Green) << 8U);
  tmp1 = ((uint32_t)(hltdc->Init.Backcolor.Red) << 16U);
  hltdc->Instance->BCCR &= ~(LTDC_BCCR_BCBLUE | LTDC_BCCR_BCGREEN | LTDC_BCCR_BCRED);
  hltdc->Instance->BCCR |= (tmp1 | tmp | hltdc->Init.Backcolor.Blue);

  /* Enable the Transfer Error and FIFO underrun interrupts */
  __BSP_LTDC_ENABLE_IT(hltdc, LTDC_IT_TE | LTDC_IT_FU);

  /* Enable LTDC by setting LTDCEN bit */
  __BSP_LTDC_ENABLE(hltdc);

  /* Initialize the error code */
  hltdc->ErrorCode = BSP_LTDC_ERROR_NONE;

  /* Initialize the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  return 0;
}

uint8_t BSP_LTDC_DeInit(BSP_LTDC_HandleTypeDef *hltdc)
{

  /* Initialize the error code */
  hltdc->ErrorCode = BSP_LTDC_ERROR_NONE;

  /* Initialize the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_RESET;

  /* Release Lock */

  return 0;
}



/** @defgroup LTDC_Exported_Functions_Group2 IO operation functions
  *  @brief   IO operation functions
  *
@verbatim
 ===============================================================================
                      #####  IO operation functions  #####
 ===============================================================================
    [..]  This section provides function allowing to:
      (+) Handle LTDC interrupt request

@endverbatim
  * @{
  */
/**
  * @brief  Handle LTDC interrupt request.
  * @param  hltdc  pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval HAL status
  */
void LTDC_IRQHandler(BSP_LTDC_HandleTypeDef *hltdc)
{
  uint32_t isrflags  = READ_REG(hltdc->Instance->ISR);
  uint32_t itsources = READ_REG(hltdc->Instance->IER);

  /* Transfer Error Interrupt management ***************************************/
  if (((isrflags & LTDC_ISR_TERRIF) != 0U) && ((itsources & LTDC_IER_TERRIE) != 0U))
  {
    /* Disable the transfer Error interrupt */
    __BSP_LTDC_DISABLE_IT(hltdc, LTDC_IT_TE);

    /* Clear the transfer error flag */
    __BSP_LTDC_CLEAR_FLAG(hltdc, LTDC_FLAG_TE);

    /* Update error code */
    hltdc->ErrorCode |= BSP_LTDC_ERROR_TE;

    /* Change LTDC state */
    hltdc->State = BSP_LTDC_STATE_ERROR;

    /* Process unlocked */


    /* Transfer error Callback */
#if (USE_LTDC_REGISTER_CALLBACKS == 1)
    /*Call registered error callback*/
    hltdc->ErrorCallback(hltdc);
#else
    /* Call legacy error callback*/
    BSP_LTDC_ErrorCallback(hltdc);
#endif /* USE_LTDC_REGISTER_CALLBACKS */
  }

  /* FIFO underrun Interrupt management ***************************************/
  if (((isrflags & LTDC_ISR_FUIF) != 0U) && ((itsources & LTDC_IER_FUIE) != 0U))
  {
    /* Disable the FIFO underrun interrupt */
    __BSP_LTDC_DISABLE_IT(hltdc, LTDC_IT_FU);

    /* Clear the FIFO underrun flag */
    __BSP_LTDC_CLEAR_FLAG(hltdc, LTDC_FLAG_FU);

    /* Update error code */
    hltdc->ErrorCode |= BSP_LTDC_ERROR_FU;

    /* Change LTDC state */
    hltdc->State = BSP_LTDC_STATE_ERROR;

    /* Process unlocked */


    /* Transfer error Callback */
#if (USE_LTDC_REGISTER_CALLBACKS == 1)
    /*Call registered error callback*/
    hltdc->ErrorCallback(hltdc);
#else
    /* Call legacy error callback*/
    BSP_LTDC_ErrorCallback(hltdc);
#endif /* USE_LTDC_REGISTER_CALLBACKS */
  }

  /* Line Interrupt management ************************************************/
  if (((isrflags & LTDC_ISR_LIF) != 0U) && ((itsources & LTDC_IER_LIE) != 0U))
  {
    /* Disable the Line interrupt */
    __BSP_LTDC_DISABLE_IT(hltdc, LTDC_IT_LI);

    /* Clear the Line interrupt flag */
    __BSP_LTDC_CLEAR_FLAG(hltdc, LTDC_FLAG_LI);

    /* Change LTDC state */
    hltdc->State = BSP_LTDC_STATE_READY;

    /* Process unlocked */


    /* Line interrupt Callback */
#if (USE_LTDC_REGISTER_CALLBACKS == 1)
    /*Call registered Line Event callback */
    hltdc->LineEventCallback(hltdc);
#else
    /*Call Legacy Line Event callback */
    BSP_LTDC_LineEventCallback(hltdc);
#endif /* USE_LTDC_REGISTER_CALLBACKS */
  }

  /* Register reload Interrupt management ***************************************/
  if (((isrflags & LTDC_ISR_RRIF) != 0U) && ((itsources & LTDC_IER_RRIE) != 0U))
  {
    /* Disable the register reload interrupt */
    __BSP_LTDC_DISABLE_IT(hltdc, LTDC_IT_RR);

    /* Clear the register reload flag */
    __BSP_LTDC_CLEAR_FLAG(hltdc, LTDC_FLAG_RR);

    /* Change LTDC state */
    hltdc->State = BSP_LTDC_STATE_READY;

    /* Process unlocked */


    /* Reload interrupt Callback */
#if (USE_LTDC_REGISTER_CALLBACKS == 1)
    /*Call registered reload Event callback */
    hltdc->ReloadEventCallback(hltdc);
#else
    /*Call Legacy Reload Event callback */
    BSP_LTDC_ReloadEventCallback(hltdc);
#endif /* USE_LTDC_REGISTER_CALLBACKS */
  }
}

/**
  * @brief  Error LTDC callback.
  * @param  hltdc  pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval None
  */
__weak void BSP_LTDC_ErrorCallback(BSP_LTDC_HandleTypeDef *hltdc)
{
  /* Prevent unused argument(s) compilation warning */

  /* NOTE : This function should not be modified, when the callback is needed,
            the BSP_LTDC_ErrorCallback could be implemented in the user file
   */
}

/**
  * @brief  Line Event callback.
  * @param  hltdc  pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval None
  */
__weak void BSP_LTDC_LineEventCallback(BSP_LTDC_HandleTypeDef *hltdc)
{
  /* Prevent unused argument(s) compilation warning */

  /* NOTE : This function should not be modified, when the callback is needed,
            the BSP_LTDC_LineEventCallback could be implemented in the user file
   */
}

/**
  * @brief  Reload Event callback.
  * @param  hltdc  pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval None
  */
__weak void BSP_LTDC_ReloadEventCallback(BSP_LTDC_HandleTypeDef *hltdc)
{
  /* Prevent unused argument(s) compilation warning */

  /* NOTE : This function should not be modified, when the callback is needed,
            the LTDC_ReloadEvenCallback could be implemented in the user file
   */
}

/**
  * @}
  */

/** @defgroup LTDC_Exported_Functions_Group3 Peripheral Control functions
  *  @brief    Peripheral Control functions
  *
@verbatim
 ===============================================================================
                    ##### Peripheral Control functions #####
 ===============================================================================
    [..]  This section provides functions allowing to:
      (+) Configure the LTDC foreground or/and background parameters.
      (+) Set the active layer.
      (+) Configure the color keying.
      (+) Configure the C-LUT.
      (+) Enable / Disable the color keying.
      (+) Enable / Disable the C-LUT.
      (+) Update the layer position.
      (+) Update the layer size.
      (+) Update pixel format on the fly.
      (+) Update transparency on the fly.
      (+) Update address on the fly.

@endverbatim
  * @{
  */

/**
  * @brief  Configure the LTDC Layer according to the specified
  *         parameters in the LTDC_InitTypeDef and create the associated handle.
  * @param  hltdc      pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                    the configuration information for the LTDC.
  * @param  pLayerCfg  pointer to a BSP_LTDC_LayerCfgTypeDef structure that contains
  *                    the configuration information for the Layer.
  * @param  LayerIdx  LTDC Layer index.
  *                    This parameter can be one of the following values:
  *                    LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval HAL status
  */
uint8_t BSP_LTDC_ConfigLayer(BSP_LTDC_HandleTypeDef *hltdc, BSP_LTDC_LayerCfgTypeDef *pLayerCfg, uint32_t LayerIdx)
{
  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Copy new layer configuration into handle structure */
  hltdc->LayerCfg[LayerIdx] = *pLayerCfg;

  /* Configure the LTDC Layer */
  BSP_LTDC_SetConfig(hltdc, pLayerCfg, LayerIdx);

  /* Set the Immediate Reload type */
  hltdc->Instance->SRCR = LTDC_SRCR_IMR;

  /* Initialize the LTDC state*/
  hltdc->State  = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Configure the color keying.
  * @param  hltdc     pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  RGBValue  the color key value
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval HAL status
  */
uint8_t BSP_LTDC_ConfigColorKeying(BSP_LTDC_HandleTypeDef *hltdc, uint32_t RGBValue, uint32_t LayerIdx)
{
  /* Process locked */

  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Configure the default color values */
  LTDC_LAYER(hltdc, LayerIdx)->CKCR &=  ~(LTDC_LxCKCR_CKBLUE | LTDC_LxCKCR_CKGREEN | LTDC_LxCKCR_CKRED);
  LTDC_LAYER(hltdc, LayerIdx)->CKCR  = RGBValue;

  /* Set the Immediate Reload type */
  hltdc->Instance->SRCR = LTDC_SRCR_IMR;

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Load the color lookup table.
  * @param  hltdc     pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  pCLUT     pointer to the color lookup table address.
  * @param  CLUTSize  the color lookup table size.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval HAL status
  */
uint8_t BSP_LTDC_ConfigCLUT(BSP_LTDC_HandleTypeDef *hltdc, uint32_t *pCLUT, uint32_t CLUTSize, uint32_t LayerIdx)
{
  uint32_t tmp;
  uint32_t counter;
  uint32_t *pcolorlut = pCLUT;

  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  for (counter = 0U; (counter < CLUTSize); counter++)
  {
    if (hltdc->LayerCfg[LayerIdx].PixelFormat == LTDC_PIXEL_FORMAT_AL44)
    {
      tmp  = (((counter + (16U*counter)) << 24U) | ((uint32_t)(*pcolorlut) & 0xFFU) | ((uint32_t)(*pcolorlut) & 0xFF00U) | ((uint32_t)(*pcolorlut) & 0xFF0000U));
    }
    else
    {
      tmp  = ((counter << 24U) | ((uint32_t)(*pcolorlut) & 0xFFU) | ((uint32_t)(*pcolorlut) & 0xFF00U) | ((uint32_t)(*pcolorlut) & 0xFF0000U));
    }

    pcolorlut++;

    /* Specifies the C-LUT address and RGB value */
    LTDC_LAYER(hltdc, LayerIdx)->CLUTWR  = tmp;
  }

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Enable the color keying.
  * @param  hltdc     pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
uint8_t BSP_LTDC_EnableColorKeying(BSP_LTDC_HandleTypeDef *hltdc, uint32_t LayerIdx)
{

  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Enable LTDC color keying by setting COLKEN bit */
  LTDC_LAYER(hltdc, LayerIdx)->CR |= (uint32_t)LTDC_LxCR_COLKEN;

  /* Set the Immediate Reload type */
  hltdc->Instance->SRCR = LTDC_SRCR_IMR;

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Disable the color keying.
  * @param  hltdc     pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
uint8_t BSP_LTDC_DisableColorKeying(BSP_LTDC_HandleTypeDef *hltdc, uint32_t LayerIdx)
{

  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Disable LTDC color keying by setting COLKEN bit */
  LTDC_LAYER(hltdc, LayerIdx)->CR &= ~(uint32_t)LTDC_LxCR_COLKEN;

  /* Set the Immediate Reload type */
  hltdc->Instance->SRCR = LTDC_SRCR_IMR;

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Enable the color lookup table.
  * @param  hltdc     pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
uint8_t BSP_LTDC_EnableCLUT(BSP_LTDC_HandleTypeDef *hltdc, uint32_t LayerIdx)
{
  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Enable LTDC color lookup table by setting CLUTEN bit */
  LTDC_LAYER(hltdc, LayerIdx)->CR |= (uint32_t)LTDC_LxCR_CLUTEN;

  /* Set the Immediate Reload type */
  hltdc->Instance->SRCR = LTDC_SRCR_IMR;

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Disable the color lookup table.
  * @param  hltdc     pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
uint8_t BSP_LTDC_DisableCLUT(BSP_LTDC_HandleTypeDef *hltdc, uint32_t LayerIdx)
{
  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Disable LTDC color lookup table by setting CLUTEN bit */
  LTDC_LAYER(hltdc, LayerIdx)->CR &= ~(uint32_t)LTDC_LxCR_CLUTEN;

  /* Set the Immediate Reload type */
  hltdc->Instance->SRCR = LTDC_SRCR_IMR;

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Enable Dither.
  * @param  hltdc  pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval  HAL status
  */

uint8_t BSP_LTDC_EnableDither(BSP_LTDC_HandleTypeDef *hltdc)
{
  /* Process locked */

  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Enable Dither by setting DTEN bit */
  LTDC->GCR |= (uint32_t)LTDC_GCR_DEN;

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Disable Dither.
  * @param  hltdc  pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval  HAL status
  */

uint8_t BSP_LTDC_DisableDither(BSP_LTDC_HandleTypeDef *hltdc)
{
  /* Process locked */

  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Disable Dither by setting DTEN bit */
  LTDC->GCR &= ~(uint32_t)LTDC_GCR_DEN;

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Set the LTDC window size.
  * @param  hltdc     pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  XSize     LTDC Pixel per line
  * @param  YSize     LTDC Line number
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
uint8_t BSP_LTDC_SetWindowSize(BSP_LTDC_HandleTypeDef *hltdc, uint32_t XSize, uint32_t YSize, uint32_t LayerIdx)
{
  BSP_LTDC_LayerCfgTypeDef *pLayerCfg;

  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Get layer configuration from handle structure */
  pLayerCfg = &hltdc->LayerCfg[LayerIdx];

  /* update horizontal stop */
  pLayerCfg->WindowX1 = XSize + pLayerCfg->WindowX0;

  /* update vertical stop */
  pLayerCfg->WindowY1 = YSize + pLayerCfg->WindowY0;

  /* Reconfigures the color frame buffer pitch in byte */
  pLayerCfg->ImageWidth = XSize;

  /* Reconfigures the frame buffer line number */
  pLayerCfg->ImageHeight = YSize;

  /* Set LTDC parameters */
  BSP_LTDC_SetConfig(hltdc, pLayerCfg, LayerIdx);

  /* Set the Immediate Reload type */
  hltdc->Instance->SRCR = LTDC_SRCR_IMR;

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Set the LTDC window position.
  * @param  hltdc     pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  X0        LTDC window X offset
  * @param  Y0        LTDC window Y offset
  * @param  LayerIdx  LTDC Layer index.
  *                         This parameter can be one of the following values:
  *                         LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
uint8_t BSP_LTDC_SetWindowPosition(BSP_LTDC_HandleTypeDef *hltdc, uint32_t X0, uint32_t Y0, uint32_t LayerIdx)
{
  BSP_LTDC_LayerCfgTypeDef *pLayerCfg;

  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Get layer configuration from handle structure */
  pLayerCfg = &hltdc->LayerCfg[LayerIdx];

  /* update horizontal start/stop */
  pLayerCfg->WindowX0 = X0;
  pLayerCfg->WindowX1 = X0 + pLayerCfg->ImageWidth;

  /* update vertical start/stop */
  pLayerCfg->WindowY0 = Y0;
  pLayerCfg->WindowY1 = Y0 + pLayerCfg->ImageHeight;

  /* Set LTDC parameters */
   BSP_LTDC_SetConfig(hltdc, pLayerCfg, LayerIdx);

  /* Set the Immediate Reload type */
  hltdc->Instance->SRCR = LTDC_SRCR_IMR;

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Reconfigure the pixel format.
  * @param  hltdc        pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                      the configuration information for the LTDC.
  * @param  Pixelformat  new pixel format value.
  * @param  LayerIdx     LTDC Layer index.
  *                      This parameter can be one of the following values:
  *                      LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1).
  * @retval  HAL status
  */
uint8_t BSP_LTDC_SetPixelFormat(BSP_LTDC_HandleTypeDef *hltdc, uint32_t Pixelformat, uint32_t LayerIdx)
{
  BSP_LTDC_LayerCfgTypeDef *pLayerCfg;

  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Get layer configuration from handle structure */
  pLayerCfg = &hltdc->LayerCfg[LayerIdx];

  /* Reconfigure the pixel format */
  pLayerCfg->PixelFormat = Pixelformat;

  /* Set LTDC parameters */
   BSP_LTDC_SetConfig(hltdc, pLayerCfg, LayerIdx);

  /* Set the Immediate Reload type */
  hltdc->Instance->SRCR = LTDC_SRCR_IMR;

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Reconfigure the layer alpha value.
  * @param  hltdc     pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  Alpha     new alpha value.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
uint8_t BSP_LTDC_SetAlpha(BSP_LTDC_HandleTypeDef *hltdc, uint32_t Alpha, uint32_t LayerIdx)
{
  BSP_LTDC_LayerCfgTypeDef *pLayerCfg;


  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Get layer configuration from handle structure */
  pLayerCfg = &hltdc->LayerCfg[LayerIdx];

  /* Reconfigure the Alpha value */
  pLayerCfg->Alpha = Alpha;

  /* Set LTDC parameters */
   BSP_LTDC_SetConfig(hltdc, pLayerCfg, LayerIdx);

  /* Set the Immediate Reload type */
  hltdc->Instance->SRCR = LTDC_SRCR_IMR;

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}
/**
  * @brief  Reconfigure the frame buffer Address.
  * @param  hltdc     pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  Address   new address value.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1).
  * @retval  HAL status
  */
uint8_t BSP_LTDC_SetAddress(BSP_LTDC_HandleTypeDef *hltdc, uint32_t Address, uint32_t LayerIdx)
{
  BSP_LTDC_LayerCfgTypeDef *pLayerCfg;
  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Get layer configuration from handle structure */
  pLayerCfg = &hltdc->LayerCfg[LayerIdx];

  /* Reconfigure the Address */
  pLayerCfg->FBStartAdress = Address;

  /* Set LTDC parameters */
   BSP_LTDC_SetConfig(hltdc, pLayerCfg, LayerIdx);

  /* Set the Immediate Reload type */
  hltdc->Instance->SRCR = LTDC_SRCR_IMR;

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Function used to reconfigure the pitch for specific cases where the attached LayerIdx buffer have a width that is
  *         larger than the one intended to be displayed on screen. Example of a buffer 800x480 attached to layer for which we
  *         want to read and display on screen only a portion 320x240 taken in the center of the buffer. The pitch in pixels
  *         will be in that case 800 pixels and not 320 pixels as initially configured by previous call to LTDC_ConfigLayer().
  * @note   This function should be called only after a previous call to LTDC_ConfigLayer() to modify the default pitch
  *         configured by LTDC_ConfigLayer() when required (refer to example described just above).
  * @param  hltdc              pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                            the configuration information for the LTDC.
  * @param  LinePitchInPixels  New line pitch in pixels to configure for LTDC layer 'LayerIdx'.
  * @param  LayerIdx           LTDC layer index concerned by the modification of line pitch.
  * @retval HAL status
  */
uint8_t BSP_LTDC_SetPitch(BSP_LTDC_HandleTypeDef *hltdc, uint32_t LinePitchInPixels, uint32_t LayerIdx)
{
  uint32_t tmp;
  uint32_t pitchUpdate;
  uint32_t pixelFormat;
  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* get LayerIdx used pixel format */
  pixelFormat = hltdc->LayerCfg[LayerIdx].PixelFormat;

  if (pixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
  {
    tmp = 4U;
  }
  else if (pixelFormat == LTDC_PIXEL_FORMAT_RGB888)
  {
    tmp = 3U;
  }
  else if ((pixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) || \
           (pixelFormat == LTDC_PIXEL_FORMAT_RGB565)   || \
           (pixelFormat == LTDC_PIXEL_FORMAT_ARGB1555) || \
           (pixelFormat == LTDC_PIXEL_FORMAT_AL88))
  {
    tmp = 2U;
  }
  else
  {
    tmp = 1U;
  }

  pitchUpdate = ((LinePitchInPixels * tmp) << 16U);

  /* Clear previously set standard pitch */
  LTDC_LAYER(hltdc, LayerIdx)->CFBLR &= ~LTDC_LxCFBLR_CFBP;

  /* Set the Reload type as immediate update of LTDC pitch configured above */
  LTDC->SRCR |= LTDC_SRCR_IMR;

  /* Set new line pitch value */
  LTDC_LAYER(hltdc, LayerIdx)->CFBLR |= pitchUpdate;

  /* Set the Reload type as immediate update of LTDC pitch configured above */
  LTDC->SRCR |= LTDC_SRCR_IMR;

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Define the position of the line interrupt.
  * @param  hltdc   pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                 the configuration information for the LTDC.
  * @param  Line    Line Interrupt Position.
  * @note   User application may resort to BSP_LTDC_LineEventCallback() at line interrupt generation.
  * @retval  HAL status
  */
uint8_t BSP_LTDC_ProgramLineEvent(BSP_LTDC_HandleTypeDef *hltdc, uint32_t Line)
{
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Disable the Line interrupt */
  __BSP_LTDC_DISABLE_IT(hltdc, LTDC_IT_LI);

  /* Set the Line Interrupt position */
  LTDC->LIPCR = (uint32_t)Line;

  /* Enable the Line interrupt */
  __BSP_LTDC_ENABLE_IT(hltdc, LTDC_IT_LI);

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Reload LTDC Layers configuration.
  * @param  hltdc      pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                    the configuration information for the LTDC.
  * @param  ReloadType This parameter can be one of the following values :
  *                      LTDC_RELOAD_IMMEDIATE : Immediate Reload
  *                      LTDC_RELOAD_VERTICAL_BLANKING  : Reload in the next Vertical Blanking
  * @note   User application may resort to BSP_LTDC_ReloadEventCallback() at reload interrupt generation.
  * @retval  HAL status
  */
uint8_t  BSP_LTDC_Reload(BSP_LTDC_HandleTypeDef *hltdc, uint32_t ReloadType)
{
  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Enable the Reload interrupt */
  __BSP_LTDC_ENABLE_IT(hltdc, LTDC_IT_RR);

  /* Apply Reload type */
  hltdc->Instance->SRCR = ReloadType;

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Configure the LTDC Layer according to the specified without reloading
  *         parameters in the LTDC_InitTypeDef and create the associated handle.
  *         Variant of the function LTDC_ConfigLayer without immediate reload.
  * @param  hltdc      pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                    the configuration information for the LTDC.
  * @param  pLayerCfg  pointer to a BSP_LTDC_LayerCfgTypeDef structure that contains
  *                    the configuration information for the Layer.
  * @param  LayerIdx   LTDC Layer index.
  *                    This parameter can be one of the following values:
  *                    LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval HAL status
  */
uint8_t BSP_LTDC_ConfigLayer_NoReload(BSP_LTDC_HandleTypeDef *hltdc, BSP_LTDC_LayerCfgTypeDef *pLayerCfg, uint32_t LayerIdx)
{
  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Copy new layer configuration into handle structure */
  hltdc->LayerCfg[LayerIdx] = *pLayerCfg;

  /* Configure the LTDC Layer */
   BSP_LTDC_SetConfig(hltdc, pLayerCfg, LayerIdx);

  /* Initialize the LTDC state*/
  hltdc->State  = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Set the LTDC window size without reloading.
  *         Variant of the function LTDC_SetWindowSize without immediate reload.
  * @param  hltdc     pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  XSize     LTDC Pixel per line
  * @param  YSize     LTDC Line number
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
uint8_t BSP_LTDC_SetWindowSize_NoReload(BSP_LTDC_HandleTypeDef *hltdc, uint32_t XSize, uint32_t YSize, uint32_t LayerIdx)
{
  BSP_LTDC_LayerCfgTypeDef *pLayerCfg;


  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Get layer configuration from handle structure */
  pLayerCfg = &hltdc->LayerCfg[LayerIdx];

  /* update horizontal stop */
  pLayerCfg->WindowX1 = XSize + pLayerCfg->WindowX0;

  /* update vertical stop */
  pLayerCfg->WindowY1 = YSize + pLayerCfg->WindowY0;

  /* Reconfigures the color frame buffer pitch in byte */
  pLayerCfg->ImageWidth = XSize;

  /* Reconfigures the frame buffer line number */
  pLayerCfg->ImageHeight = YSize;

  /* Set LTDC parameters */
   BSP_LTDC_SetConfig(hltdc, pLayerCfg, LayerIdx);

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Set the LTDC window position without reloading.
  *         Variant of the function LTDC_SetWindowPosition without immediate reload.
  * @param  hltdc     pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  X0        LTDC window X offset
  * @param  Y0        LTDC window Y offset
  * @param  LayerIdx  LTDC Layer index.
  *                         This parameter can be one of the following values:
  *                         LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
uint8_t BSP_LTDC_SetWindowPosition_NoReload(BSP_LTDC_HandleTypeDef *hltdc, uint32_t X0, uint32_t Y0, uint32_t LayerIdx)
{
  BSP_LTDC_LayerCfgTypeDef *pLayerCfg;


  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Get layer configuration from handle structure */
  pLayerCfg = &hltdc->LayerCfg[LayerIdx];

  /* update horizontal start/stop */
  pLayerCfg->WindowX0 = X0;
  pLayerCfg->WindowX1 = X0 + pLayerCfg->ImageWidth;

  /* update vertical start/stop */
  pLayerCfg->WindowY0 = Y0;
  pLayerCfg->WindowY1 = Y0 + pLayerCfg->ImageHeight;

  /* Set LTDC parameters */
   BSP_LTDC_SetConfig(hltdc, pLayerCfg, LayerIdx);

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Reconfigure the pixel format without reloading.
  *         Variant of the function LTDC_SetPixelFormat without immediate reload.
  * @param  hltdc        pointer to a LTDC_HandleTypeDfef structure that contains
  *                      the configuration information for the LTDC.
  * @param  Pixelformat  new pixel format value.
  * @param  LayerIdx     LTDC Layer index.
  *                      This parameter can be one of the following values:
  *                      LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1).
  * @retval  HAL status
  */
uint8_t BSP_LTDC_SetPixelFormat_NoReload(BSP_LTDC_HandleTypeDef *hltdc, uint32_t Pixelformat, uint32_t LayerIdx)
{
  BSP_LTDC_LayerCfgTypeDef *pLayerCfg;


  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Get layer configuration from handle structure */
  pLayerCfg = &hltdc->LayerCfg[LayerIdx];

  /* Reconfigure the pixel format */
  pLayerCfg->PixelFormat = Pixelformat;

  /* Set LTDC parameters */
  BSP_LTDC_SetConfig(hltdc, pLayerCfg, LayerIdx);

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Reconfigure the layer alpha value without reloading.
  *         Variant of the function LTDC_SetAlpha without immediate reload.
  * @param  hltdc     pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  Alpha     new alpha value.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
uint8_t BSP_LTDC_SetAlpha_NoReload(BSP_LTDC_HandleTypeDef *hltdc, uint32_t Alpha, uint32_t LayerIdx)
{
  BSP_LTDC_LayerCfgTypeDef *pLayerCfg;

  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Get layer configuration from handle structure */
  pLayerCfg = &hltdc->LayerCfg[LayerIdx];

  /* Reconfigure the Alpha value */
  pLayerCfg->Alpha = Alpha;

  /* Set LTDC parameters */
  BSP_LTDC_SetConfig(hltdc, pLayerCfg, LayerIdx);

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Reconfigure the frame buffer Address without reloading.
  *         Variant of the function LTDC_SetAddress without immediate reload.
  * @param  hltdc     pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  Address   new address value.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1).
  * @retval  HAL status
  */
uint8_t BSP_LTDC_SetAddress_NoReload(BSP_LTDC_HandleTypeDef *hltdc, uint32_t Address, uint32_t LayerIdx)
{
  BSP_LTDC_LayerCfgTypeDef *pLayerCfg;


  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Get layer configuration from handle structure */
  pLayerCfg = &hltdc->LayerCfg[LayerIdx];

  /* Reconfigure the Address */
  pLayerCfg->FBStartAdress = Address;

  /* Set LTDC parameters */
  BSP_LTDC_SetConfig(hltdc, pLayerCfg, LayerIdx);

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Function used to reconfigure the pitch for specific cases where the attached LayerIdx buffer have a width that is
  *         larger than the one intended to be displayed on screen. Example of a buffer 800x480 attached to layer for which we
  *         want to read and display on screen only a portion 320x240 taken in the center of the buffer. The pitch in pixels
  *         will be in that case 800 pixels and not 320 pixels as initially configured by previous call to LTDC_ConfigLayer().
  * @note   This function should be called only after a previous call to LTDC_ConfigLayer() to modify the default pitch
  *         configured by LTDC_ConfigLayer() when required (refer to example described just above).
  *         Variant of the function LTDC_SetPitch without immediate reload.
  * @param  hltdc              pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                            the configuration information for the LTDC.
  * @param  LinePitchInPixels  New line pitch in pixels to configure for LTDC layer 'LayerIdx'.
  * @param  LayerIdx           LTDC layer index concerned by the modification of line pitch.
  * @retval HAL status
  */
uint8_t BSP_LTDC_SetPitch_NoReload(BSP_LTDC_HandleTypeDef *hltdc, uint32_t LinePitchInPixels, uint32_t LayerIdx)
{
  uint32_t tmp;
  uint32_t pitchUpdate;
  uint32_t pixelFormat;

  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* get LayerIdx used pixel format */
  pixelFormat = hltdc->LayerCfg[LayerIdx].PixelFormat;

  if (pixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
  {
    tmp = 4U;
  }
  else if (pixelFormat == LTDC_PIXEL_FORMAT_RGB888)
  {
    tmp = 3U;
  }
  else if ((pixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) || \
           (pixelFormat == LTDC_PIXEL_FORMAT_RGB565)   || \
           (pixelFormat == LTDC_PIXEL_FORMAT_ARGB1555) || \
           (pixelFormat == LTDC_PIXEL_FORMAT_AL88))
  {
    tmp = 2U;
  }
  else
  {
    tmp = 1U;
  }

  pitchUpdate = ((LinePitchInPixels * tmp) << 16U);

  /* Clear previously set standard pitch */
  LTDC_LAYER(hltdc, LayerIdx)->CFBLR &= ~LTDC_LxCFBLR_CFBP;

  /* Set new line pitch value */
  LTDC_LAYER(hltdc, LayerIdx)->CFBLR |= pitchUpdate;

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}


/**
  * @brief  Configure the color keying without reloading.
  *         Variant of the function LTDC_ConfigColorKeying without immediate reload.
  * @param  hltdc     pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  RGBValue the color key value
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval HAL status
  */
uint8_t BSP_LTDC_ConfigColorKeying_NoReload(BSP_LTDC_HandleTypeDef *hltdc, uint32_t RGBValue, uint32_t LayerIdx)
{
  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Configure the default color values */
  LTDC_LAYER(hltdc, LayerIdx)->CKCR &=  ~(LTDC_LxCKCR_CKBLUE | LTDC_LxCKCR_CKGREEN | LTDC_LxCKCR_CKRED);
  LTDC_LAYER(hltdc, LayerIdx)->CKCR  = RGBValue;

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Enable the color keying without reloading.
  *         Variant of the function LTDC_EnableColorKeying without immediate reload.
  * @param  hltdc     pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
uint8_t BSP_LTDC_EnableColorKeying_NoReload(BSP_LTDC_HandleTypeDef *hltdc, uint32_t LayerIdx)
{
  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Enable LTDC color keying by setting COLKEN bit */
  LTDC_LAYER(hltdc, LayerIdx)->CR |= (uint32_t)LTDC_LxCR_COLKEN;

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Disable the color keying without reloading.
  *         Variant of the function LTDC_DisableColorKeying without immediate reload.
  * @param  hltdc     pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
uint8_t BSP_LTDC_DisableColorKeying_NoReload(BSP_LTDC_HandleTypeDef *hltdc, uint32_t LayerIdx)
{

  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Disable LTDC color keying by setting COLKEN bit */
  LTDC_LAYER(hltdc, LayerIdx)->CR &= ~(uint32_t)LTDC_LxCR_COLKEN;

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Enable the color lookup table without reloading.
  *         Variant of the function LTDC_EnableCLUT without immediate reload.
  * @param  hltdc     pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
uint8_t BSP_LTDC_EnableCLUT_NoReload(BSP_LTDC_HandleTypeDef *hltdc, uint32_t LayerIdx)
{


  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Disable LTDC color lookup table by setting CLUTEN bit */
  LTDC_LAYER(hltdc, LayerIdx)->CR |= (uint32_t)LTDC_LxCR_CLUTEN;

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @brief  Disable the color lookup table without reloading.
  *         Variant of the function LTDC_DisableCLUT without immediate reload.
  * @param  hltdc     pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values:
  *                   LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval  HAL status
  */
uint8_t BSP_LTDC_DisableCLUT_NoReload(BSP_LTDC_HandleTypeDef *hltdc, uint32_t LayerIdx)
{
  /* Change LTDC peripheral state */
  hltdc->State = BSP_LTDC_STATE_BUSY;

  /* Disable LTDC color lookup table by setting CLUTEN bit */
  LTDC_LAYER(hltdc, LayerIdx)->CR &= ~(uint32_t)LTDC_LxCR_CLUTEN;

  /* Change the LTDC state*/
  hltdc->State = BSP_LTDC_STATE_READY;

  /* Process unlocked */

  return 0;
}

/**
  * @}
  */

/** @defgroup LTDC_Exported_Functions_Group4 Peripheral State and Errors functions
  *  @brief    Peripheral State and Errors functions
  *
@verbatim
 ===============================================================================
                  ##### Peripheral State and Errors functions #####
 ===============================================================================
    [..]
    This subsection provides functions allowing to
      (+) Check the LTDC handle state.
      (+) Get the LTDC handle error code.

@endverbatim
  * @{
  */

/**
  * @brief  Return the LTDC handle state.
  * @param  hltdc  pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                the configuration information for the LTDC.
  * @retval HAL state
  */
BSP_LTDC_StateTypeDef BSP_LTDC_GetState(BSP_LTDC_HandleTypeDef *hltdc)
{
  return hltdc->State;
}

/**
  * @brief  Return the LTDC handle error code.
  * @param  hltdc  pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *               the configuration information for the LTDC.
  * @retval LTDC Error Code
  */
uint32_t BSP_LTDC_GetError(BSP_LTDC_HandleTypeDef *hltdc)
{
  return hltdc->ErrorCode;
}

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup LTDC_Private_Functions LTDC Private Functions
  * @{
  */

/**
  * @brief  Configure the LTDC peripheral
  * @param  hltdc     Pointer to a BSP_LTDC_HandleTypeDef structure that contains
  *                   the configuration information for the LTDC.
  * @param  pLayerCfg Pointer LTDC Layer Configuration structure
  * @param  LayerIdx  LTDC Layer index.
  *                   This parameter can be one of the following values: LTDC_LAYER_1 (0) or LTDC_LAYER_2 (1)
  * @retval None
  */
static void BSP_LTDC_SetConfig(BSP_LTDC_HandleTypeDef *hltdc, BSP_LTDC_LayerCfgTypeDef *pLayerCfg, uint32_t LayerIdx)
{
  uint32_t tmp;
  uint32_t tmp1;
  uint32_t tmp2;

  /* Configure the horizontal start and stop position */
  tmp = ((pLayerCfg->WindowX1 + ((hltdc->Instance->BPCR & LTDC_BPCR_AHBP) >> 16U)) << 16U);
  LTDC_LAYER(hltdc, LayerIdx)->WHPCR &= ~(LTDC_LxWHPCR_WHSTPOS | LTDC_LxWHPCR_WHSPPOS);
  LTDC_LAYER(hltdc, LayerIdx)->WHPCR = ((pLayerCfg->WindowX0 + ((hltdc->Instance->BPCR & LTDC_BPCR_AHBP) >> 16U) + 1U) | tmp);

  /* Configure the vertical start and stop position */
  tmp = ((pLayerCfg->WindowY1 + (hltdc->Instance->BPCR & LTDC_BPCR_AVBP)) << 16U);
  LTDC_LAYER(hltdc, LayerIdx)->WVPCR &= ~(LTDC_LxWVPCR_WVSTPOS | LTDC_LxWVPCR_WVSPPOS);
  LTDC_LAYER(hltdc, LayerIdx)->WVPCR  = ((pLayerCfg->WindowY0 + (hltdc->Instance->BPCR & LTDC_BPCR_AVBP) + 1U) | tmp);

  /* Specifies the pixel format */
  LTDC_LAYER(hltdc, LayerIdx)->PFCR &= ~(LTDC_LxPFCR_PF);
  LTDC_LAYER(hltdc, LayerIdx)->PFCR = (pLayerCfg->PixelFormat);

  /* Configure the default color values */
  tmp = ((uint32_t)(pLayerCfg->Backcolor.Green) << 8U);
  tmp1 = ((uint32_t)(pLayerCfg->Backcolor.Red) << 16U);
  tmp2 = (pLayerCfg->Alpha0 << 24U);
  LTDC_LAYER(hltdc, LayerIdx)->DCCR &= ~(LTDC_LxDCCR_DCBLUE | LTDC_LxDCCR_DCGREEN | LTDC_LxDCCR_DCRED | LTDC_LxDCCR_DCALPHA);
  LTDC_LAYER(hltdc, LayerIdx)->DCCR = (pLayerCfg->Backcolor.Blue | tmp | tmp1 | tmp2);

  /* Specifies the constant alpha value */
  LTDC_LAYER(hltdc, LayerIdx)->CACR &= ~(LTDC_LxCACR_CONSTA);
  LTDC_LAYER(hltdc, LayerIdx)->CACR = (pLayerCfg->Alpha);

  /* Specifies the blending factors */
  LTDC_LAYER(hltdc, LayerIdx)->BFCR &= ~(LTDC_LxBFCR_BF2 | LTDC_LxBFCR_BF1);
  LTDC_LAYER(hltdc, LayerIdx)->BFCR = (pLayerCfg->BlendingFactor1 | pLayerCfg->BlendingFactor2);

  /* Configure the color frame buffer start address */
  LTDC_LAYER(hltdc, LayerIdx)->CFBAR &= ~(LTDC_LxCFBAR_CFBADD);
  LTDC_LAYER(hltdc, LayerIdx)->CFBAR = (pLayerCfg->FBStartAdress);

  if (pLayerCfg->PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
  {
    tmp = 4U;
  }
  else if (pLayerCfg->PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
  {
    tmp = 3U;
  }
  else if ((pLayerCfg->PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) || \
           (pLayerCfg->PixelFormat == LTDC_PIXEL_FORMAT_RGB565)   || \
           (pLayerCfg->PixelFormat == LTDC_PIXEL_FORMAT_ARGB1555) || \
           (pLayerCfg->PixelFormat == LTDC_PIXEL_FORMAT_AL88))
  {
    tmp = 2U;
  }
  else
  {
    tmp = 1U;
  }

  /* Configure the color frame buffer pitch in byte */
  LTDC_LAYER(hltdc, LayerIdx)->CFBLR  &= ~(LTDC_LxCFBLR_CFBLL | LTDC_LxCFBLR_CFBP);
  LTDC_LAYER(hltdc, LayerIdx)->CFBLR  = (((pLayerCfg->ImageWidth * tmp) << 16U) | (((pLayerCfg->WindowX1 - pLayerCfg->WindowX0) * tmp)  + 7U));
  /* Configure the frame buffer line number */
  LTDC_LAYER(hltdc, LayerIdx)->CFBLNR  &= ~(LTDC_LxCFBLNR_CFBLNBR);
  LTDC_LAYER(hltdc, LayerIdx)->CFBLNR  = (pLayerCfg->ImageHeight);

  /* Enable LTDC_Layer by setting LEN bit */
  LTDC_LAYER(hltdc, LayerIdx)->CR |= (uint32_t)LTDC_LxCR_LEN;
}

