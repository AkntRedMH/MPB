#include "ads1220.h"
#include "uwallinit.h"

void ADS1220AssertCS(int fAssert)
{
		if (fAssert)
			CLR_CS1; 
		else
			SET_CS1; 
}
void ADS1220SendByte(unsigned char Byte)
{	
	HAL_SPI_Transmit(&hspi1,&Byte,1,10);   
}
unsigned char ADS1220ReceiveByte()
{
	unsigned char SData = 0xff ,Result = 0;

	HAL_SPI_TransmitReceive(&hspi1,&SData,&Result,1,10);

	return Result;
}
/*
******************************************************************************
 higher level functions
*/

void ADS1220ReadRegister(int StartAddress, int NumRegs, unsigned * pData)
{
   int i;
	/* assert CS to start transfer */
	ADS1220AssertCS(1);
 	/* send the command byte */
	ADS1220SendByte(ADS1220_CMD_RREG | (((StartAddress<<2) & 0x0c) |((NumRegs-1)&0x03)));
   	/* get the register content */
	for (i=0; i< NumRegs; i++)
	{
		*pData++ = ADS1220ReceiveByte();
	}
   	/* de-assert CS */
	ADS1220AssertCS(0);
	return;
}
void ADS1220WriteRegister(int StartAddress, int NumRegs, unsigned char * pData)
{
	int i;
   	/* assert CS to start transfer */
	ADS1220AssertCS(1);
   	/* send the command byte */
	ADS1220SendByte(ADS1220_CMD_WREG | (((StartAddress<<2) & 0x0c) |((NumRegs-1)&0x03)));
    /* send the data bytes */
	for (i=0; i< NumRegs; i++)
	{
		ADS1220SendByte(*pData++);
	}
   	/* de-assert CS */
	ADS1220AssertCS(0);
   	return;
}
void ADS1220SendResetCommand()
{
	/* assert CS to start transfer */
	ADS1220AssertCS(1);
   	/* send the command byte */
	ADS1220SendByte(ADS1220_CMD_RESET);
   	/* de-assert CS */
	ADS1220AssertCS(0);
   	return;
}
void ADS1220SendStartCommand()
{
	/* assert CS to start transfer */
	ADS1220AssertCS(1);
    /* send the command byte */
	ADS1220SendByte(ADS1220_CMD_SYNC);
   	/* de-assert CS */
	ADS1220AssertCS(0);
    return;
}
void ADS1220SendShutdownCommand()
{
	/* assert CS to start transfer */
	ADS1220AssertCS(1);
   	/* send the command byte */
	ADS1220SendByte(ADS1220_CMD_SHUTDOWN);
   	/* de-assert CS */
	ADS1220AssertCS(0);
    return;
}

/* ADS1220 Initial Configuration */
void ADS1220Init(void)
{
	uint8_t ch_cfg[4]={ADS1220_MUX_0_G|ADS1220_MUX_1_G|ADS1220_GAIN_1|ADS1220_PGA_BYPASS, ADS1220_CC|ADS1220_DR_45, ADS1220_VREF_EX_DED|ADS1220_PSW_SW, ADS1220_IDAC1_OFF|ADS1220_IDAC2_OFF};

	ADS1220SendResetCommand();//复位

	HAL_Delay(100);
	ADS1220WriteRegister(ADS1220_0_REGISTER,4,ch_cfg);//配置4个寄存器
	HAL_Delay(100);
	ADS1220SendStartCommand();
}


long ADS1220ReadData()
{
	long Data;
	/* assert CS to start transfer */

	ADS1220AssertCS(1);

	/* send the command byte */
//	ADS1220SendByte(ch,ADS1220_CMD_RDATA);
	/* get the conversion result */
#ifdef ADS1120
	Data = ADS1220ReceiveByte(ch);
	Data = (Data << 8) | ADS1220ReceiveByte(ch);
	/* sign extend data */
	if (Data & 0x8000)
		Data |= 0xffff0000;
#else
	Data = ADS1220ReceiveByte();
	Data = (Data << 8) | ADS1220ReceiveByte();
	Data = (Data << 8) | ADS1220ReceiveByte();
	/* sign extend data */
	if (Data & 0x800000)
		Data |= 0xff000000; 
#endif
	/* de-assert CS */

	ADS1220AssertCS(0);

	return Data;
}


//选择通道函数
void ADS1220_SelCh(uint8_t ch)
{
	uint8_t cfg[2]={ADS1220_MUX_0_G|ADS1220_GAIN_1|ADS1220_PGA_BYPASS};
	
	switch(ch)
	{
		case 0://选择通道0
			cfg[0] = ADS1220_MUX_0_G|ADS1220_GAIN_1|ADS1220_PGA_BYPASS;
			ADS1220WriteRegister(ADS1220_0_REGISTER,1,cfg);
			break;
		case 1://选择通道1
			cfg[0] = ADS1220_MUX_1_G|ADS1220_GAIN_1|ADS1220_PGA_BYPASS;
			ADS1220WriteRegister(ADS1220_0_REGISTER,1,cfg);
			break;
		case 2://选择通道2
			cfg[0] = ADS1220_MUX_2_G|ADS1220_GAIN_1|ADS1220_PGA_BYPASS;
			ADS1220WriteRegister(ADS1220_0_REGISTER,1,cfg);
			break;
		case 3://选择通道3
			cfg[0] = ADS1220_MUX_3_G|ADS1220_GAIN_1|ADS1220_PGA_BYPASS;
			ADS1220WriteRegister(ADS1220_0_REGISTER,1,cfg);
			break;
	}
}


__IO uint16_t ADS1220_Tick = 0;
uint32_t temp;
float Vin[4];
//循环读取4个通道
void ADS1220_GetData()
{
	static uint8_t ads_ch = 0;
		
  if(ADS1220_Tick<50)return;
  ADS1220_Tick=0;
	
	switch(ads_ch)
  {
    case 0://读取通道0 然后切换到通道1
        temp = ADS1220ReadData();
				Vin[0] = temp*5.0f/8388608;//转换为电压 单位V
				printf_(&huart1, "%f\r\n", Vin[0]);
				ads_ch = 1;
				ADS1220_SelCh(ads_ch);
        break;
    case 1://读取通道1 然后切换到通道2
        temp = ADS1220ReadData();
				Vin[1] = temp*5.0f/8388608;//转换为电压 单位V
				printf_(&huart1, "%f\r\n", Vin[1]);
        ads_ch = 2;
				ADS1220_SelCh(ads_ch);
        break;
		case 2://读取通道2 然后切换到通道3
        temp = ADS1220ReadData();
				Vin[2] = temp*2.048f/8388608;
        ads_ch = 3;
				ADS1220_SelCh(ads_ch);
        break;
		case 3://读取通道3 然后切换到通道0
        temp = ADS1220ReadData();
				Vin[3] = temp*2.048f/8388608;
        ads_ch = 0;
				ADS1220_SelCh(ads_ch);
        break;
    }
}
