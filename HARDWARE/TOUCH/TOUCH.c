/*
 * @Descripttion: 
 * @Version: V1.0
 * @Author: YURI
 * @Email: 1700695611@qq.com
 * @Website: www.heredifferent.com.cn
 * @Date: 2021-08-21 09:31:17
 * @LastEditors: YURI
 * @LastEditTime: 2021-08-21 10:14:13
 */

#include "TOUCH.h"  

u8 GT9XX_IIC_RADDR= 0xBB;			// IIC��ʼ����ַ
u8 GT9XX_IIC_WADDR= 0xBA;
/*****************************************************************************************
*	�� �� ��: Touch_IIC_GPIO_Config
*	��ڲ���: ��
*	�� �� ֵ: ��
*	��������: ��ʼ��IIC��GPIO��,�������
*	˵    ��: ����IICͨ���ٶȲ��ߣ������IO���ٶ�����Ϊ2M����
******************************************************************************************/

void Touch_IIC_GPIO_Config (void)
{

	Touch_IIC_SCL_CLK_ENABLE;	//��ʼ��IO��ʱ��
	Touch_IIC_SDA_CLK_ENABLE;
	Touch_INT_CLK_ENABLE;	
	Touch_RST_CLK_ENABLE;	
	
    GPIO_Set(Touch_IIC_SCL_PORT,Touch_IIC_SCL_PIN,GPIO_MODE_OUT,GPIO_OTYPE_OD,GPIO_SPEED_LOW,GPIO_PUPD_NONE);
    GPIO_Set(Touch_IIC_SDA_PORT,Touch_IIC_SDA_PIN,GPIO_MODE_OUT,GPIO_OTYPE_OD,GPIO_SPEED_LOW,GPIO_PUPD_NONE);
    GPIO_Set(Touch_INT_PORT,Touch_INT_PIN,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_LOW,GPIO_PUPD_PU);
    GPIO_Set(Touch_RST_PORT,Touch_RST_PIN,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_LOW,GPIO_PUPD_PU);
     
	GPIO_Pin_Set(Touch_IIC_SCL_PORT, Touch_IIC_SCL_PIN, 1);		// SCL����ߵ�ƽ
	GPIO_Pin_Set(Touch_IIC_SDA_PORT, Touch_IIC_SDA_PIN, 1);    // SDA����ߵ�ƽ
	GPIO_Pin_Set(Touch_INT_PORT, 	  Touch_INT_PIN,     0);  // INT����͵�ƽ
	GPIO_Pin_Set(Touch_RST_PORT,     Touch_RST_PIN,     1);    // RST����ߵ�ƽ

}

/*****************************************************************************************
*	�� �� ��: Touch_IIC_Delay
*	��ڲ���: a - ��ʱʱ��
*	�� �� ֵ: ��
*	��������: ����ʱ����
*	˵    ��: Ϊ����ֲ�ļ�����Ҷ���ʱ����Ҫ�󲻸ߣ����Բ���Ҫʹ�ö�ʱ������ʱ
******************************************************************************************/

void Touch_IIC_Delay(uint32_t a)
{
	volatile uint16_t i;
	while (a --)				
	{
		for (i = 0; i < 8; i++);
	}
}



/*****************************************************************************************
*	�� �� ��: Touch_IIC_Start
*	��ڲ���: ��
*	�� �� ֵ: ��
*	��������: IIC��ʼ�ź�
*	˵    ��: ��SCL���ڸߵ�ƽ�ڼ䣬SDA�ɸߵ�������Ϊ��ʼ�ź�
******************************************************************************************/

void Touch_IIC_Start(void)
{
	Touch_IIC_SDA(1);		
	Touch_IIC_SCL(1);
	Touch_IIC_Delay(IIC_DelayVaule);
	
	Touch_IIC_SDA(0);
	Touch_IIC_Delay(IIC_DelayVaule);
	Touch_IIC_SCL(0);
	Touch_IIC_Delay(IIC_DelayVaule);
}

/*****************************************************************************************
*	�� �� ��: Touch_IIC_Stop
*	��ڲ���: ��
*	�� �� ֵ: ��
*	��������: IICֹͣ�ź�
*	˵    ��: ��SCL���ڸߵ�ƽ�ڼ䣬SDA�ɵ͵�������Ϊ��ʼ�ź�
******************************************************************************************/

void Touch_IIC_Stop(void)
{
	Touch_IIC_SCL(0);
	Touch_IIC_Delay(IIC_DelayVaule);
	Touch_IIC_SDA(0);
	Touch_IIC_Delay(IIC_DelayVaule);
	
	Touch_IIC_SCL(1);
	Touch_IIC_Delay(IIC_DelayVaule);
	Touch_IIC_SDA(1);
	Touch_IIC_Delay(IIC_DelayVaule);
}

/*****************************************************************************************
*	�� �� ��: Touch_IIC_ACK
*	��ڲ���: ��
*	�� �� ֵ: ��
*	��������: IICӦ���ź�
*	˵    ��: ��SCLΪ�ߵ�ƽ�ڼ䣬SDA�������Ϊ�͵�ƽ������Ӧ���ź�
******************************************************************************************/

void Touch_IIC_ACK(void)
{
	Touch_IIC_SCL(0);
	Touch_IIC_Delay(IIC_DelayVaule);
	Touch_IIC_SDA(0);
	Touch_IIC_Delay(IIC_DelayVaule);	
	Touch_IIC_SCL(1);
	Touch_IIC_Delay(IIC_DelayVaule);
	
	Touch_IIC_SCL(0);		// SCL�����ʱ��SDAӦ�������ߣ��ͷ�����
	Touch_IIC_SDA(1);		
	
	Touch_IIC_Delay(IIC_DelayVaule);

}

/*****************************************************************************************
*	�� �� ��: Touch_IIC_NoACK
*	��ڲ���: ��
*	�� �� ֵ: ��
*	��������: IIC��Ӧ���ź�
*	˵    ��: ��SCLΪ�ߵ�ƽ�ڼ䣬��SDA����Ϊ�ߵ�ƽ��������Ӧ���ź�
******************************************************************************************/

void Touch_IIC_NoACK(void)
{
	Touch_IIC_SCL(0);	
	Touch_IIC_Delay(IIC_DelayVaule);
	Touch_IIC_SDA(1);
	Touch_IIC_Delay(IIC_DelayVaule);
	Touch_IIC_SCL(1);
	Touch_IIC_Delay(IIC_DelayVaule);
	
	Touch_IIC_SCL(0);
	Touch_IIC_Delay(IIC_DelayVaule);
}

/*****************************************************************************************
*	�� �� ��: Touch_IIC_WaitACK
*	��ڲ���: ��
*	�� �� ֵ: ��
*	��������: �ȴ������豸����Ӧ���ź�
*	˵    ��: ��SCLΪ�ߵ�ƽ�ڼ䣬����⵽SDA����Ϊ�͵�ƽ��������豸��Ӧ����
******************************************************************************************/

uint8_t Touch_IIC_WaitACK(void)
{
	Touch_IIC_SDA(1);
	Touch_IIC_Delay(IIC_DelayVaule);
	Touch_IIC_SCL(1);
	Touch_IIC_Delay(IIC_DelayVaule);	
	
	if( GPIO_Pin_Get(Touch_IIC_SDA_PORT,Touch_IIC_SDA_PIN) != 0) //�ж��豸�Ƿ���������Ӧ		
	{
		Touch_IIC_SCL(0);	
		Touch_IIC_Delay( IIC_DelayVaule );		
		return ACK_ERR;	//��Ӧ��
	}
	else
	{
		Touch_IIC_SCL(0);	
		Touch_IIC_Delay( IIC_DelayVaule );		
		return ACK_OK;	//Ӧ������
	}
}

/*****************************************************************************************
*	�� �� ��:	Touch_IIC_WriteByte
*	��ڲ���:	IIC_Data - Ҫд���8λ����
*	�� �� ֵ:	ACK_OK  - �豸��Ӧ����
*          	ACK_ERR - �豸��Ӧ����
*	��������:	дһ�ֽ�����
*	˵    ��:��λ��ǰ
******************************************************************************************/

uint8_t Touch_IIC_WriteByte(uint8_t IIC_Data)
{
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		Touch_IIC_SDA(IIC_Data & 0x80);
		Touch_IIC_Delay( IIC_DelayVaule );
		Touch_IIC_SCL(1);
		Touch_IIC_Delay( IIC_DelayVaule );
		Touch_IIC_SCL(0);		
		if(i == 7)
		{
			Touch_IIC_SDA(1);			
		}
		IIC_Data <<= 1;
	}
	return Touch_IIC_WaitACK(); //�ȴ��豸��Ӧ
}

/*****************************************************************************************
*	�� �� ��:	Touch_IIC_ReadByte
*	��ڲ���:	ACK_Mode - ��Ӧģʽ������1�򷢳�Ӧ���źţ�����0������Ӧ���ź�
*	�� �� ֵ:	ACK_OK  - �豸��Ӧ����
*          	ACK_ERR - �豸��Ӧ����
*	��������:��һ�ֽ�����
*	˵    ��:1.��λ��ǰ
*				2.Ӧ�������������һ�ֽ�����ʱ���ͷ�Ӧ���ź�
******************************************************************************************/

uint8_t Touch_IIC_ReadByte(uint8_t ACK_Mode)
{
	uint8_t IIC_Data = 0;
	uint8_t i = 0;
	
	for (i = 0; i < 8; i++)
	{
		IIC_Data <<= 1;
		Touch_IIC_SCL(1);
		Touch_IIC_Delay( IIC_DelayVaule );
		IIC_Data |= (GPIO_Pin_Get(Touch_IIC_SDA_PORT,Touch_IIC_SDA_PIN) & 0x01);	
		Touch_IIC_SCL(0);
		Touch_IIC_Delay( IIC_DelayVaule );
	}
	
	if ( ACK_Mode == 1 )				//	Ӧ���ź�
		Touch_IIC_ACK();
	else
		Touch_IIC_NoACK();		 	// ��Ӧ���ź�
	
	return IIC_Data; 
}


volatile TouchStructure touchInfo; 			//	������Ϣ�ṹ�壬�ں��� Touch_Scan() �ﱻ���ã��洢��������


/*************************************************************************************************************************************
*	�� �� ��:	GT9XX_Reset
*	��ڲ���:	��
*	�� �� ֵ:	��
*	��������:	��λGT911
*	˵    ��:	��λGT911������оƬ��IIC��ַ����Ϊ0xBA/0xBB
************************************************************************************************************************************/

void GT9XX_Reset(void)
{
	Touch_INT_Out();	//	��INT��������Ϊ���
	
	// ��ʼ������״̬
	GPIO_Pin_Set(Touch_INT_PORT,Touch_INT_PIN,0);  // INT����͵�ƽ
	GPIO_Pin_Set(Touch_RST_PORT,Touch_RST_PIN,1);    // RST�����	��ƽ
	Touch_IIC_Delay(10000);
	
	// ��ʼִ�и�λ
	//	INT���ű��ֵ͵�ƽ���䣬��������ַ����Ϊ0XBA/0XBB
	GPIO_Pin_Set(Touch_RST_PORT,Touch_RST_PIN,0); // ���͸�λ���ţ���ʱоƬִ�и�λ
	Touch_IIC_Delay(150000);			// ��ʱ
	GPIO_Pin_Set(Touch_RST_PORT,Touch_RST_PIN,1);			// ���߸�λ���ţ���λ����
	Touch_IIC_Delay(350000);			// ��ʱ
	Touch_INT_In();						// INT����תΪ��������
	Touch_IIC_Delay(20000);				// ��ʱ
									
}

/*************************************************************************************************************************************
*	�� �� ��:	GT9XX_WriteHandle
*	��ڲ���:	addr - Ҫ�����ļĴ���
*	�� �� ֵ:	SUCCESS - �����ɹ�
*					ERROR	  - ����ʧ��
*	��������:	GT9XX д����
*	˵    ��:	��ָ���ļĴ���ִ��д����
************************************************************************************************************************************/

uint8_t GT9XX_WriteHandle (uint16_t addr)
{
	uint8_t status;				// ״̬��־λ

	Touch_IIC_Start();	// ����IICͨ��
	if( Touch_IIC_WriteByte(GT9XX_IIC_WADDR) == ACK_OK ) //д����ָ��
	{
		if( Touch_IIC_WriteByte((uint8_t)(addr >> 8)) == ACK_OK ) //д��16λ��ַ
		{
			if( Touch_IIC_WriteByte((uint8_t)(addr)) != ACK_OK )
			{
				status = ERROR;	// ����ʧ��
			}			
		}
	}
	status = SUCCESS;	// �����ɹ�
	return status;	
}

/*************************************************************************************************************************************
*	�� �� ��:	GT9XX_WriteData
*	��ڲ���:	addr - Ҫд��ļĴ���
*					value - Ҫд�������
*	�� �� ֵ:	SUCCESS - �����ɹ�
*					ERROR	  - ����ʧ��
*	��������:	GT9XX дһ�ֽ�����
*	˵    ��:	��ָ���ļĴ���д��һ�ֽ�����
************************************************************************************************************************************/

uint8_t GT9XX_WriteData (uint16_t addr,uint8_t value)
{
	uint8_t status;
	
	Touch_IIC_Start(); //����IICͨѶ

	if( GT9XX_WriteHandle(addr) == SUCCESS)	//д��Ҫ�����ļĴ���
	{
		if (Touch_IIC_WriteByte(value) != ACK_OK) //д����
		{
			status = ERROR;						
		}
	}	
	Touch_IIC_Stop(); // ֹͣͨѶ
	
	status = SUCCESS;	// д��ɹ�
	return status;
}

/*************************************************************************************************************************************
*	�� �� ��:	GT9XX_WriteReg
*	��ڲ���:	addr - Ҫд��ļĴ��������׵�ַ
*					cnt  - ���ݳ���
*					value - Ҫд���������
*	�� �� ֵ:	SUCCESS - �����ɹ�
*					ERROR	  - ����ʧ��
*	��������:	GT9XX д�Ĵ���
*	˵    ��:	��оƬ�ļĴ�����д��ָ�����ȵ�����
************************************************************************************************************************************/

uint8_t GT9XX_WriteReg (uint16_t addr, uint8_t cnt, uint8_t *value)
{
	uint8_t status;
	uint8_t i;

	Touch_IIC_Start();

	if( GT9XX_WriteHandle(addr) == SUCCESS) 	// д��Ҫ�����ļĴ���
	{
		for(i = 0 ; i < cnt; i++)			 	// ����
		{
			Touch_IIC_WriteByte(value[i]);	// д������
		}					
		Touch_IIC_Stop();		// ֹͣIICͨ��
		status = SUCCESS;		// д��ɹ�
	}
	else
	{
		Touch_IIC_Stop();		// ֹͣIICͨ��
		status = ERROR;		// д��ʧ��
	}
	return status;	
}

/*************************************************************************************************************************************
*	�� �� ��:	GT9XX_ReadReg
*	��ڲ���:	addr - Ҫ��ȡ�ļĴ��������׵�ַ
*					cnt  - ���ݳ���
*					value - Ҫ��ȡ��������
*	�� �� ֵ:	SUCCESS - �����ɹ�
*					ERROR	  - ����ʧ��
*	��������:	GT9XX ���Ĵ���
*	˵    ��:	��оƬ�ļĴ�������ȡָ�����ȵ�����
************************************************************************************************************************************/

uint8_t GT9XX_ReadReg (uint16_t addr, uint8_t cnt, uint8_t *value)
{
	uint8_t status;
	uint8_t i;

	status = ERROR;
	Touch_IIC_Start();		// ����IICͨ��

	if( GT9XX_WriteHandle(addr) == SUCCESS) //д��Ҫ�����ļĴ���
	{
		Touch_IIC_Start(); //��������IICͨѶ

		if (Touch_IIC_WriteByte(GT9XX_IIC_RADDR) == ACK_OK)	// ���Ͷ�����
		{	
			for(i = 0 ; i < cnt; i++)	// ����
			{
				if (i == (cnt - 1))
				{
					value[i] = Touch_IIC_ReadByte(0);	// �������һ������ʱ���� ��Ӧ���ź�
				}
				else
				{
					value[i] = Touch_IIC_ReadByte(1);	// ����Ӧ���ź�
				}
			}					
			Touch_IIC_Stop();	// ֹͣIICͨ��
			status = SUCCESS;
		}
	}
	Touch_IIC_Stop();	// ֹͣIICͨ��
	return (status);	
}

/*************************************************************************************************************************************
*	�� �� ��: Touch_Init
*	��ڲ���: ��
*	�� �� ֵ: SUCCESS  - ��ʼ���ɹ�
*            ERROR 	 - ����δ��⵽������	
*	��������: ����IC��ʼ��������ȡ��Ӧ��Ϣ���͵�����
*	˵    ��: ��ʼ���������
************************************************************************************************************************************/

uint8_t Touch_Init(void)
{
	uint8_t GT9XX_Info[11];	// ������IC��Ϣ
	uint8_t cfgVersion = 0;	// �������ð汾
	
	Touch_IIC_GPIO_Config(); 	// ��ʼ��IIC����
	GT9XX_Reset();					// ��λIC
			
	GT9XX_ReadReg (GT9XX_ID_ADDR,11,GT9XX_Info);		// ��������IC��Ϣ
	GT9XX_ReadReg (GT9XX_CFG_ADDR,1,&cfgVersion);	// ���������ð汾
	if( GT9XX_Info[0] == '9' )	//�жϵ�һ���ַ��Ƿ�Ϊ 9
	{
		printf("Touch ID: GT%.4s \r\n",GT9XX_Info);	//��ӡ����оƬ��ID
		printf("�̼��汾�� 0X%.4x\r\n",(GT9XX_Info[5]<<8) + GT9XX_Info[4]);	// оƬ�̼��汾
		printf("�����ֱ��ʣ�%d * %d\r\n",(GT9XX_Info[7]<<8) + GT9XX_Info[6],(GT9XX_Info[9]<<8) +GT9XX_Info[8]);	// ��ǰ�����ֱ���		
		printf("�����������ð汾�� 0X%.2x \r\n",cfgVersion);	// �������ð汾	
		
		return SUCCESS;
	}
	else
	{
        GT9XX_IIC_RADDR=GT9XX_IIC_RADDR_TEMP;
        GT9XX_IIC_WADDR=GT9XX_IIC_WADDR_TEMP;
        GT9XX_ReadReg (GT9XX_ID_ADDR,11,GT9XX_Info);		// ��������IC��Ϣ
        GT9XX_ReadReg (GT9XX_CFG_ADDR,1,&cfgVersion);
		printf("Touch Error\r\n");	//����δ��⵽������
        printf("Touch ID: GT%.4s \r\n",GT9XX_Info);	//��ӡ����оƬ��ID
		printf("�̼��汾�� 0X%.4x\r\n",(GT9XX_Info[5]<<8) + GT9XX_Info[4]);	// оƬ�̼��汾
		printf("�����ֱ��ʣ�%d * %d\r\n",(GT9XX_Info[7]<<8) + GT9XX_Info[6],(GT9XX_Info[9]<<8) +GT9XX_Info[8]);	// ��ǰ�����ֱ���		
		printf("�����������ð汾�� 0X%.2x \r\n",cfgVersion);	// �������ð汾	
		
		return ERROR;
	}

}

/*************************************************************************************************************************************
*	�� �� ��: Touch_Scan
*	��ڲ���: ��
*	�� �� ֵ: ��
*	��������: ����ɨ��
*	˵    ��: �ڳ����������Եĵ��øú��������Լ�ⴥ��������������Ϣ�洢�� touchInfo �ṹ��
************************************************************************************************************************************/

void Touch_Scan(void)
{
 	uint8_t  touchData[2 + 8 * TOUCH_MAX ]; 		// ���ڴ洢��������
	uint8_t  i = 0;	
	
	GT9XX_ReadReg (GT9XX_READ_ADDR,2 + 8 * TOUCH_MAX ,touchData);	// ������
	GT9XX_WriteData (GT9XX_READ_ADDR,0);									//	�������оƬ�ļĴ�����־λ
	touchInfo.num = touchData[0] & 0x0f;									// ȡ��ǰ�Ĵ�������
	
	if ( (touchInfo.num >= 1) && (touchInfo.num <=5) ) 	//	���������� 1-5 ֮��ʱ
	{
		for(i=0;i<touchInfo.num;i++)		// ȡ��Ӧ�Ĵ�������
		{
			touchInfo.y[i] = (touchData[5+8*i]<<8) | touchData[4+8*i];	// ��ȡY����
			touchInfo.x[i] = (touchData[3+8*i]<<8) | touchData[2+8*i];	//	��ȡX����			
		}
		touchInfo.flag = 1;	// ������־λ��1�������д�����������
	}
	else                       
	{
		touchInfo.flag = 0;	// ������־λ��0���޴�������
	}
}
