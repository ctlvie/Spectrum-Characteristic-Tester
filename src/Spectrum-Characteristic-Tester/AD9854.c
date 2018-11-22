#include<msp430f5529.h>
#include"AD9854.h"

//******************************************************************************

unsigned char AD9854_Reg_PHA1[2]	={0x00,0x00};
unsigned char AD9854_Reg_PHA2[2]	={0x00,0x00};
unsigned char AD9854_Reg_FRE1[6]	={0x00,0xF0,0x00,0x00,0x00,0x00};
unsigned char AD9854_Reg_FRE2[6]	={0x80,0x00,0x00,0x00,0x00,0x00};
unsigned char AD9854_Reg_DELTA[6]	={0x00,0x00,0x00,0x10,0x00,0x00};
unsigned char AD9854_Reg_UPDATA[4]	={0x00,0x00,0x00,0x00};
unsigned char AD9854_Reg_RAMP_CLK[3]={0x00,0x00,0x01};
unsigned char AD9854_Reg_CTR_REG[4]	={	Bin(00010000),       //comp pd dowm  ctr[28]=0 //qac pd down ctr[26]=0
                                        Bin(01000110),       //no pll pass ctr[21]=0  //multi times 6 ctr[19-16]=00110  
                                        Bin(00100100),       //ext update ctr[8]=0    //Ramped FSK 
                                        Bin(01000000) };     //pass inv CR[6]=1//osk EN ctr[5]=0 //msb first ctr[1]=0 //no sdo active ctr[0]=0
unsigned char AD9854_Reg_I_MUL[2]	={0x00,0x00};
unsigned char AD9854_Reg_Q_MUL[2]	={0x00,0x00};
unsigned char AD9854_Reg_SHAPED[1]	={0x00};
unsigned char AD9854_Reg_Q_DAC[2]	={0x00,0x00};

////////////////////////////////////////////////////////////////////
//                                                                //
//                             BASIC FUNCTION                     //
//                                                                //
////////////////////////////////////////////////////////////////////

//MCLK的频率为8000KHz的条件下延时ms个毫秒
void DELAY_AD9854_MS (unsigned int ms)
{
	unsigned int iq0,iq1 ;
	for ( iq0 = ms ; iq0 > 0 ;iq0 --)
	for ( iq1 = LOOPCNT  ; iq1 > 0 ;iq1 --) ;
}

//******************************************************************************
//函数名：SpisendByte(Uchar dat)
//输入：发送的数据
//输出：无
//功能描述：SPI发送一个字节
//******************************************************************************
Uchar sendByte_AD9854(Uchar dat)
{
//----------------------以下是模拟SPI时序方式-----------------------------------
	Uchar i,temp;
	temp = 0;	
	AD9854_SCLK_DOWN;
	for(i=0; i<8; i++)
	{
		if(dat & 0x80)
		{
	     AD9854_SDIO_UP;
		}
		else 
		{
		 AD9854_SDIO_DOWN;
		}
		dat <<= 1;
	        AD9854_SCLK_UP; 
		temp <<= 1;
	if(AD9854_SDO_IN)temp++;  //读取MISO状态
		AD9854_SCLK_DOWN;
	}
	return temp;
}


//******************************************************************************
//函数名：writeSPIBurstReg_AD9854(Uchar addr, Uchar *buffer, Uchar count)
//输入：地址，写入缓冲区，写入个数
//输出：无
//功能描述：SPI连续写配置寄存器
//******************************************************************************
void writeSPIBurstReg_AD9854(Uchar addr, Uchar *buffer, Uchar count)
{
    Uchar i,temp;
    temp = (addr & 0x0F); 				// Send Write address
	AD9854_CS_DOWN;
	while (AD9854_SDO_IN);
	sendByte_AD9854(temp);
    for (i = 0; i < count; i++)
    {
        sendByte_AD9854(buffer[i]);
    }
    AD9854_CS_UP;
}

//******************************************************************************
//函数名：void readSPIBurstReg_AD9854(Uchar addr, Uchar *buffer, Uchar count)
//输入：地址，读出数据后暂存的缓冲区，读出配置个数
//输出：无
//功能描述：SPI连续读配置寄存器
//******************************************************************************
void readSPIBurstReg_AD9854(Uchar addr, Uchar *buffer, Uchar count)
{
  unsigned int i,temp;
  temp = ((addr & 0x0F) | 0x80);  		// Send Read address
  AD9854_CS_DOWN;
  while (AD9854_SDO_IN);
  sendByte_AD9854(temp); 
  for (i = 0; i < count; i++) 
	{
      buffer[i] = sendByte_AD9854(0);
    }
  AD9854_CS_UP;
}

//函数功能:更新设置
//输入参数:无
//******************************************************************
void configAD9854(void)
{
  	unsigned i=100 ;        //
	AD9854_UPDATE_OUT;
	AD9854_UPDATE_UP ;      //
	while(i--) ;
	AD9854_UPDATE_DOWN ;
}
//******************************************************************

//函数功能:复位
//输入参数:无
//******************************************************************
void resetAD9854(void)
{
	unsigned i=100 ;        //
	AD9854_IO_RESET_UP ;
	while(i--) ;
	AD9854_IO_RESET_DOWN ;
}
//******************************************************************

//函数功能:AD9854初始化 写控制寄存器
//输入参数:无
//硬件说明：
//******************************************************************
void initAD9854(void)
{
	HARDWARE_AD9854 ;       //通过宏定义设置
	AD9854_IO_serial;		//串行模式
	AD9854_OSC_ON;			//打开时钟
	AD9854_CS_DOWN ;        //片选	
	AD9854_RESET_UP;	
	DELAY_AD9854_MS(10);
	AD9854_RESET_DOWN;		//主复位	
	resetAD9854();
	AD9854_OSC_OFF;			//关闭时钟，更新配置
	writeSPIBurstReg_AD9854(AD9854_Addr_CTR_REG,		//写控制寄存器
						AD9854_Reg_CTR_REG, AD9854_Length_CTR_REG);			
	AD9854_OSC_ON;			//打开时钟，更新配置
	DELAY_AD9854_MS(10);
	AD9854_UPDATE_OUT;
	AD9854_UPDATE_DOWN;		//配置更新时钟为输出
}

//******************************************************************
//函数功能：写 Frequency Tuning Word #1
//输入参数: 
//写入地址字节数:1 Byte
//写入数据字节数:6 Bytes 
//******************************************************************
void writeAD9854_Freq1(void)
{ 	
	writeSPIBurstReg_AD9854(AD9854_Addr_FRE1,		//写频率控制寄存器1
						AD9854_Reg_FRE2, AD9854_Length_FRE1);
	 configAD9854();

}

//******************************************************************
//函数功能：设置三角扫频
//输入参数:
//Serial Register Address :2，3，4，6
//写入地址字节数:1 Byte
//写入数据字节数:6 Bytes 
//******************************************************************
void writeAD9854_FreqSW(void)
{ 	
	writeSPIBurstReg_AD9854(AD9854_Addr_FRE1,		//写频率控制寄存器1
						AD9854_Reg_FRE1, AD9854_Length_FRE1);
		  resetAD9854();
	writeSPIBurstReg_AD9854(AD9854_Addr_FRE2,		//写频率控制寄存器2
						AD9854_Reg_FRE2, AD9854_Length_FRE2);
		  resetAD9854();
	writeSPIBurstReg_AD9854(AD9854_Addr_DELTA,		//写频率增量寄存器
						AD9854_Reg_DELTA, AD9854_Length_DELTA);
		  resetAD9854();
	writeSPIBurstReg_AD9854(AD9854_Addr_RAMP_CLK,	//写扫频时钟寄存器
						AD9854_Reg_RAMP_CLK, AD9854_Length_RAMP_CLK);
	  	configAD9854();
}


//******************************************************************************
//系统初始化
//******************************************************************************
/*
void Initsys()
{
   unsigned int iq0;
   _DINT();
   BCSCTL1 &=~XT2OFF;
   do
   {
      IFG1 &= ~OFIFG;		    	// 清除振荡器失效标志
  for (iq0 = 0xFF; iq0 > 0; iq0--);	// 延时，等待XT2起振
   }
   while ((IFG1 & OFIFG) != 0);		// 判断XT2是否起振		
   BCSCTL2 |=SELM1+SELS;			//MCLK为8MHz，SMCLK为8MHz ;                
}

void main( void )
{
     WDTCTL = WDTPW + WDTHOLD ; 
     Initsys();   
     initAD9854();
 	writeAD9854_FreqSW();
	//writeAD9854_Freq1();
     while(1) ;

}

*/
