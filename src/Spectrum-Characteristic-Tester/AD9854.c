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

//MCLK��Ƶ��Ϊ8000KHz����������ʱms������
void DELAY_AD9854_MS (unsigned int ms)
{
	unsigned int iq0,iq1 ;
	for ( iq0 = ms ; iq0 > 0 ;iq0 --)
	for ( iq1 = LOOPCNT  ; iq1 > 0 ;iq1 --) ;
}

//******************************************************************************
//��������SpisendByte(Uchar dat)
//���룺���͵�����
//�������
//����������SPI����һ���ֽ�
//******************************************************************************
Uchar sendByte_AD9854(Uchar dat)
{
//----------------------������ģ��SPIʱ��ʽ-----------------------------------
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
	if(AD9854_SDO_IN)temp++;  //��ȡMISO״̬
		AD9854_SCLK_DOWN;
	}
	return temp;
}


//******************************************************************************
//��������writeSPIBurstReg_AD9854(Uchar addr, Uchar *buffer, Uchar count)
//���룺��ַ��д�뻺������д�����
//�������
//����������SPI����д���üĴ���
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
//��������void readSPIBurstReg_AD9854(Uchar addr, Uchar *buffer, Uchar count)
//���룺��ַ���������ݺ��ݴ�Ļ��������������ø���
//�������
//����������SPI���������üĴ���
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

//��������:��������
//�������:��
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

//��������:��λ
//�������:��
//******************************************************************
void resetAD9854(void)
{
	unsigned i=100 ;        //
	AD9854_IO_RESET_UP ;
	while(i--) ;
	AD9854_IO_RESET_DOWN ;
}
//******************************************************************

//��������:AD9854��ʼ�� д���ƼĴ���
//�������:��
//Ӳ��˵����
//******************************************************************
void initAD9854(void)
{
	HARDWARE_AD9854 ;       //ͨ���궨������
	AD9854_IO_serial;		//����ģʽ
	AD9854_OSC_ON;			//��ʱ��
	AD9854_CS_DOWN ;        //Ƭѡ	
	AD9854_RESET_UP;	
	DELAY_AD9854_MS(10);
	AD9854_RESET_DOWN;		//����λ	
	resetAD9854();
	AD9854_OSC_OFF;			//�ر�ʱ�ӣ���������
	writeSPIBurstReg_AD9854(AD9854_Addr_CTR_REG,		//д���ƼĴ���
						AD9854_Reg_CTR_REG, AD9854_Length_CTR_REG);			
	AD9854_OSC_ON;			//��ʱ�ӣ���������
	DELAY_AD9854_MS(10);
	AD9854_UPDATE_OUT;
	AD9854_UPDATE_DOWN;		//���ø���ʱ��Ϊ���
}

//******************************************************************
//�������ܣ�д Frequency Tuning Word #1
//�������: 
//д���ַ�ֽ���:1 Byte
//д�������ֽ���:6 Bytes 
//******************************************************************
void writeAD9854_Freq1(void)
{ 	
	writeSPIBurstReg_AD9854(AD9854_Addr_FRE1,		//дƵ�ʿ��ƼĴ���1
						AD9854_Reg_FRE2, AD9854_Length_FRE1);
	 configAD9854();

}

//******************************************************************
//�������ܣ���������ɨƵ
//�������:
//Serial Register Address :2��3��4��6
//д���ַ�ֽ���:1 Byte
//д�������ֽ���:6 Bytes 
//******************************************************************
void writeAD9854_FreqSW(void)
{ 	
	writeSPIBurstReg_AD9854(AD9854_Addr_FRE1,		//дƵ�ʿ��ƼĴ���1
						AD9854_Reg_FRE1, AD9854_Length_FRE1);
		  resetAD9854();
	writeSPIBurstReg_AD9854(AD9854_Addr_FRE2,		//дƵ�ʿ��ƼĴ���2
						AD9854_Reg_FRE2, AD9854_Length_FRE2);
		  resetAD9854();
	writeSPIBurstReg_AD9854(AD9854_Addr_DELTA,		//дƵ�������Ĵ���
						AD9854_Reg_DELTA, AD9854_Length_DELTA);
		  resetAD9854();
	writeSPIBurstReg_AD9854(AD9854_Addr_RAMP_CLK,	//дɨƵʱ�ӼĴ���
						AD9854_Reg_RAMP_CLK, AD9854_Length_RAMP_CLK);
	  	configAD9854();
}


//******************************************************************************
//ϵͳ��ʼ��
//******************************************************************************
/*
void Initsys()
{
   unsigned int iq0;
   _DINT();
   BCSCTL1 &=~XT2OFF;
   do
   {
      IFG1 &= ~OFIFG;		    	// �������ʧЧ��־
  for (iq0 = 0xFF; iq0 > 0; iq0--);	// ��ʱ���ȴ�XT2����
   }
   while ((IFG1 & OFIFG) != 0);		// �ж�XT2�Ƿ�����		
   BCSCTL2 |=SELM1+SELS;			//MCLKΪ8MHz��SMCLKΪ8MHz ;                
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
