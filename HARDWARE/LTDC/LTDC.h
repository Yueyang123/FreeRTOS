/*
 * @Descripttion: LTDC�����ļ�
 * @Version: V1.0
 * @Author: YURI
 * @Email: 1700695611@qq.com
 * @Website: www.heredifferent.com.cn
 * @Date: 2021-08-19 22:08:59
 * @LastEditors: YURI
 * @LastEditTime: 2021-08-21 06:31:16
 */
#ifndef  LTDC_H
#define  LTDC_H

#include "sys.h"

// 1. ���ֻ�õ��㣬�ò�������Ϊ1���ɣ�ʹ��˫��Ļ�����Ҫ�޸�Ϊ 2
// 2. ʹ�õ����ڲ�AXI SRAM��Ϊ�Դ棬��ʼ��ַ0x24000000����СΪ512K
// 3. �Դ�����ռ� = �ֱ��� * ÿ��������ռ�ֽ��������� 480*272������ʹ��16λɫ��RGB565����AEGB1555������Ҫ�Դ� 480*272*2 = 261120 �ֽ�
// 4. �����ǵ�����ʾ����˫����ʾ�������ܳ��� AXI SRAM �Ĵ�С
// 5. ����û���Ҫ˫����ʾ����ֻ������ ARGB1555 + RGB565 ���� ARGB4444 + RGB565 �ĸ�ʽ�����������Դ�Ϊ 480*272*2*2 = 522240 �ֽ�
//
#define 	LCD_NUM_LAYERS  1	//	������ʾ�Ĳ�����750������������ʾ


#define	ColorMode_0   LTDC_PIXEL_FORMAT_RGB565   		//���� layer0 ����ɫ��ʽ
//#define	ColorMode_0   LTDC_PIXEL_FORMAT_ARGB1555  
//#define	ColorMode_0    LTDC_PIXEL_FORMAT_ARGB4444  
//#define	ColorMode_0   LTDC_PIXEL_FORMAT_RGB888
//#define	ColorMode_0   LTDC_PIXEL_FORMAT_ARGB8888   

#if  LCD_NUM_LAYERS == 2	// ���������˫�㣬���ڴ˴����� layer1 ����ɫ��ʽ			

//	#define	ColorMode_1    LTDC_PIXEL_FORMAT_ARGB1555  // ARGB1555��֧��һλ͸��ɫ��������͸���Ͳ�͸������״̬
	#define	ColorMode_1    LTDC_PIXEL_FORMAT_ARGB4444  // ARGB4444֧��4λ͸��ɫ����16��͸��״̬
		
#endif


// ��ʾ�������
// ʹ��ʾ����LCD_DisplayDirection(Direction_H) ��������Ļ������ʾ
// ʹ��ʾ����LCD_DisplayDirection(Direction_V) ��������Ļ��ֱ��ʾ

#define	Direction_H	0		//LCD������ʾ
#define	Direction_V	1		//LCD������ʾ

// ���ñ�����ʾʱ����λ��0���ǲ��ո�
// ֻ�� LCD_DisplayNumber() ��ʾ���� �� LCD_DisplayDecimals()��ʾС�� �����������õ�
// ʹ��ʾ���� LCD_ShowNumMode(Fill_Zero) ���ö���λ���0������ 123 ������ʾΪ 000123
//
#define  Fill_Zero  0		//���0
#define  Fill_Space 1		//���ո�

// ������ؽṹ����
typedef struct _pFont
{    
	const uint8_t 		*pTable;  		//	��ģ�����ַ
	uint16_t 			Width; 		 	//	�����ַ�����ģ���
	uint16_t 			Height; 			//	�����ַ�����ģ����
	uint16_t 			Sizes;	 		//	�����ַ�����ģ���ݸ���
	uint16_t				Table_Rows;		// �ò���ֻ�к�����ģ�õ�����ʾ��ά������д�С
} pFONT;

/*------------------------------------ �������� ---------------------------------------------*/

extern	pFONT	CH_Font12 ;		//	1212����
extern	pFONT	CH_Font16 ;    //	1616����
extern	pFONT	CH_Font20 ;    //	2020����
extern	pFONT	CH_Font24 ;    //	2424����
extern	pFONT	CH_Font32 ;    //	3232����

/*------------------------------------ ASCII���� ---------------------------------------------*/

extern pFONT Font32;	// 3216 ����
extern pFONT Font24;	// 2412 ����
extern pFONT Font20; // 2010 ����
extern pFONT Font16; // 1608 ����
extern pFONT Font12; // 1206 ����
/*---------------------------------------- ������ɫ ------------------------------------------------------

 1. ����Ϊ�˷����û�ʹ�ã��������32λ��ɫ��Ȼ����ͨ�������Զ�ת���ɶ�Ӧ��ɫ��ʽ����Ҫ�ĵ���ɫ
 2. 32λ����ɫ�У��Ӹ�λ����λ�ֱ��Ӧ A��R��G��B  4����ɫͨ��������A��ʾ͸��ͨ��
 3. ��������255��͸��ɫ��ff��ʾ��͸����0��ʾ��ȫ͸��
 4. ����ʹ��ARGB1555��ARGB8888��֧��͸��ɫ����ɫ��ʽ����Ȼ͸��ɫ�������ã�����ARGB1555��֧��һλ
	 ͸��ɫ��������͸���Ͳ�͸������״̬��ARGB4444��16��͸���ȣ�ARGB8888֧��255��͸����
 5. �û������ڵ����õ�ɫ���ȡ24λRGB��ɫ��Ȼ���ٲ���͸��ͨ���õ�32λ����ɫ���ٽ���32λ��ɫ����
	 LCD_SetColor()��LCD_SetBackColor()�Ϳ�����ʾ����Ӧ����ɫ�� 
 6. ʹ��ʾ��������ɫ��RGBֵΪ0x0000FF���������Ҫ͸��ɫ�����Ӧ��32λ��ɫֵΪ 0xff0000FF
 7. ���¶������ɫ������Ϊ��͸�����û��ɸ����������ж����Ӧ����ɫ								*/

#define 	LCD_WHITE       0xffFFFFFF		// ����ɫ
#define 	LCD_BLACK       0xff000000		// ����ɫ
                           
#define 	LCD_BLUE        0xff0000FF		//	����ɫ
#define 	LCD_GREEN       0xff00FF00    //	����ɫ
#define 	LCD_RED         0xffFF0000    //	����ɫ
#define 	LCD_CYAN        0xff00FFFF    //	����ɫ
#define 	LCD_MAGENTA     0xffFF00FF    //	�Ϻ�ɫ
#define 	LCD_YELLOW      0xffFFFF00    //	��ɫ
#define 	LCD_GREY        0xff2C2C2C    //	��ɫ
													
#define 	LIGHT_BLUE      0xff8080FF    //	����ɫ
#define 	LIGHT_GREEN     0xff80FF80    //	����ɫ
#define 	LIGHT_RED       0xffFF8080    //	����ɫ
#define 	LIGHT_CYAN      0xff80FFFF    //	������ɫ
#define 	LIGHT_MAGENTA   0xffFF80FF    //	���Ϻ�ɫ
#define 	LIGHT_YELLOW    0xffFFFF80    //	����ɫ
#define 	LIGHT_GREY      0xffA3A3A3    //	����ɫ
													
#define 	DARK_BLUE       0xff000080    //	����ɫ
#define 	DARK_GREEN      0xff008000    //	����ɫ
#define 	DARK_RED        0xff800000    //	����ɫ
#define 	DARK_CYAN       0xff008080    //	������ɫ
#define 	DARK_MAGENTA    0xff800080    //	���Ϻ�ɫ
#define 	DARK_YELLOW     0xff808000    //	����ɫ
#define 	DARK_GREY       0xff404040    //	����ɫ

/*---------------------------------------------------------- �������� -------------------------------------------------------*/
	
void LTDC_Init(void);		// LTDC�Լ����ʼ�� 
void LCD_Clear(void);			// ��������
void LCD_ClearRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height);	// �ֲ���������

void  LCD_SetLayer(uint8_t Layerx); 					//	���ò�
void  LCD_SetColor(uint32_t Color); 				   	//	���û�����ɫ
void  LCD_SetBackColor(uint32_t Color);  				//	���ñ�����ɫ
void  LCD_DisplayDirection(uint8_t direction);  		//	������ʾ����

//>>>>>	��ʾASCII�ַ�
void    LCD_SetFont(pFONT *fonts);												//	����ASCII����
void 	LCD_DisplayChar(uint16_t x, uint16_t y,uint8_t c);				//	��ʾ����ASCII�ַ�
void 	LCD_DisplayString( uint16_t x, uint16_t y, char *p);	 		//	��ʾASCII�ַ���

//>>>>>	��ʾ�����ַ�������ASCII��
void 	LCD_SetTextFont(pFONT *fonts);										// �����ı����壬�������ĺ�ASCII����
void 	LCD_DisplayChinese(uint16_t x, uint16_t y, char *pText);		// ��ʾ��������
void 	LCD_DisplayText(uint16_t x, uint16_t y, char *pText) ;		// ��ʾ�ַ������������ĺ�ASCII�ַ�

//>>>>>	��ʾ������С��
void  LCD_ShowNumMode(uint8_t mode);														// ������ʾģʽ������λ���ո������0
void  LCD_DisplayNumber( uint16_t x, uint16_t y, int32_t number,uint8_t len) ;				// ��ʾ����
void  LCD_DisplayDecimals( uint16_t x, uint16_t y, double number,uint8_t len,uint8_t decs);	// ��ʾС��

//>>>>>	����ͼƬ

void 	LCD_DrawImage(uint16_t x,uint16_t y,uint16_t width,uint16_t height,const uint8_t *pImage)  ;

//>>>>>	2Dͼ�λ��ƺ���

void  LCD_DrawPoint(uint16_t x,uint16_t y,uint32_t color);   									//����
uint32_t 	LCD_ReadPoint(uint16_t x,uint16_t y);												//����
void  LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);							//����
void  LCD_DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height);					//������
void  LCD_DrawCircle(uint16_t x, uint16_t y, uint16_t r);										//��Բ
void  LCD_DrawEllipse(int x, int y, int r1, int r2);											//����Բ

//>>>>>	������亯��

void  LCD_FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height);				//������
void  LCD_FillCircle(uint16_t x, uint16_t y, uint16_t r);									//���Բ

/*-------------------------------------------------------- LCD��ز��� -------------------------------------------------------*/

#define HBP  43	// ������Ļ���ֲ��������
#define VBP  12
#define HSW  1
#define VSW  1
#define HFP  8
#define VFP  8

#define LCD_Width     	480				//	LCD�����س���
#define LCD_Height    	272				//	LCD�����ؿ��
#define LCD_MemoryAdd   0x24000000 		//	�Դ����ʼ��ַ  

#if ( ColorMode_0 == LTDC_PIXEL_FORMAT_RGB565 || ColorMode_0 == LTDC_PIXEL_FORMAT_ARGB1555 || ColorMode_0 ==LTDC_PIXEL_FORMAT_ARGB4444 )
	#define BytesPerPixel_0		2		//16λɫģʽÿ������ռ2�ֽ�
#elif ColorMode_0 == LTDC_PIXEL_FORMAT_RGB888
	#define BytesPerPixel_0		3		//24λɫģʽÿ������ռ3�ֽ�
#else
	#define BytesPerPixel_0		4		//32λɫģʽÿ������ռ4�ֽ�
#endif	

#if LCD_NUM_LAYERS == 2
	#if ( ColorMode_1 == LTDC_PIXEL_FORMAT_RGB565 || ColorMode_1 == LTDC_PIXEL_FORMAT_ARGB1555 || LTDC_PIXEL_FORMAT_ARGB4444)
		#define BytesPerPixel_1		2		//16λɫģʽÿ������ռ2�ֽ�
	#endif	
	#define LCD_MemoryAdd_OFFSET   LCD_Width * LCD_Height * BytesPerPixel_0 	 //�ڶ�����Դ��ƫ�Ƶ�ַ 
#endif

/*-------------------------------------------------------- LCD�������� -------------------------------------------------------*/

#define  	LCD_Backlight_PIN		GPIO_PIN_15								
#define		LCD_Backlight_PORT		GPIOD									
 	

#define		LCD_Backlight_OFF		GPIO_Pin_Set(GPIOD,PIN15,0);  	// �رձ���
#define 	LCD_Backlight_ON		GPIO_Pin_Set(GPIOD,PIN15,1);  	// ��������






#endif // ! LTDC_H
