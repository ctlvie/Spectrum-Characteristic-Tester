/*=======================================================
Author				  :				ctlvie
Email Address		  :				ctlvie@gmail.com
Filename			  :				ADS1115.h
Date				  :			    2018-11-20
Description			  :				ADS1115 模数转换器驱动程序

Modification History:
Date		By			Version		Description
----------------------------------------------------------
181120		ctlvie		1.0			
========================================================*/

/*=======================================================
    接线方式:
    1. V    ->  VCC
    2. G    ->  GND
    3. SCL  ->  SCL  ->  P2.6
    4. SDA  ->  SDA  ->  P2.7
    5. ADDR ->  GND
    6. ALERT -> NC
    7. A0    -> 输入模拟信号
========================================================*/



#ifndef ADS1115_H_
#define ADS1115_H_

//=====================================================================
/**********ADDR宏定义***************/
#define ADDRESS_0  0x90   //ADDR PIN ->GND
#define ADDRESS_1  0x92   //ADDR PIN ->VDD
#define ADDRESS_2  0x94   //ADDR PIN ->SDA
#define ADDRESS_3  0x96   //ADDR PIN ->SCL


/************POINTER REGISTER*****************/
#define Pointer_0  0x00   //Convertion register
#define Pointer_1  0x01   //Config register
#define Pointer_2  0x02   //Lo_thresh register
#define Pointer_3  0x03   //Hi_thresh register

/************CONFIG REGISTER*****************/
#define OS_0   0x0000
#define OS_1   0x8000

#define MUX_0  0x0000  //AINp=AIN0, AINn=AIN1
#define MUX_1  0x1000  //AINp=AIN0, AINn=AIN3
#define MUX_2  0x2000  //AINp=AIN1, AINn=AIN3
#define MUX_3  0x3000  //AINp=AIN2, AINn=AIN3
#define MUX_4  0x4000  //AINp=AIN0, AINn=GND
#define MUX_5  0x5000  //AINp=AIN1, AINn=GND
#define MUX_6  0x6000  //AINp=AIN2, AINn=GND
#define MUX_7  0x7000  //AINp=AIN3, AINn=GND

#define PGA_0  0x0000   //FS=6.144V
#define PGA_1  0x0200   //FS=4.096V
#define PGA_2  0x0400  //FS=2.048V
#define PGA_3  0x0600  //FS=1.024V
#define PGA_4  0x0800  //FS=0.512V
#define PGA_5  0x0A00  //FS=0.256V
#define PGA_6  0x0C00  //FS=0.256V
#define PGA_7  0x0E00  //FS=0.256V

#define MODE_0 0x0000
#define MODE_1 0x0100

#define DR_0   0x0000   //Data Rate = 8
#define DR_1   0x0020   //Data Rate = 16
#define DR_2   0x0040   //Data Rate = 32
#define DR_3   0x0060   //Data Rate = 64
#define DR_4   0x0080   //Data Rate = 128
#define DR_5   0x00A0   //Data Rate = 250
#define DR_6   0x00C0   //Data Rate = 475
#define DR_7   0x00E0   //Data Rate = 860

#define COMP_MODE_0  0x0000
#define COMP_MODE_1  0x0010

#define COMP_POL_0   0x0000
#define COMP_POL_1   0x0008

#define COMP_LAT_0   0x0000
#define COMP_LAT_1   0x0040

#define COMP_QUE_0   0x0000
#define COMP_QUE_1   0x0001
#define COMP_QUE_2   0x0002
#define COMP_QUE_3   0x0003
//=====================================================================



#define  uchar unsigned char
#define  uint unsigned int

#define CPU_F_ADS1115 ((double)1045000)
#define DELAY_ADS1115_US(x) __delay_cycles((long)((CPU_F_ADS1115 * (double)x / 1000000.0)))
#define DELAY_ADS1115_MS(x) __delay_cycles((long)((CPU_F_ADS1115 * (double)x / 1000.0)))


#define IO_ADS1115_DIR      P2DIR
#define IO_ADS1115_OUT      P2OUT
#define IO_ADS1115_SEL      P2SEL
#define IO_ADS1115_IN       P2IN
#define IO_ADS1115_BIT_SCL  BIT6
#define IO_ADS1115_BIT_SDA  BIT7


#define SCL1       IO_ADS1115_OUT |=  IO_ADS1115_BIT_SCL   //P2^0高电平
#define SCL0       IO_ADS1115_OUT &=~ IO_ADS1115_BIT_SCL //P2^0低电平

#define SDA1       IO_ADS1115_OUT |=  IO_ADS1115_BIT_SDA   //P2^1高电平
#define SDA0       IO_ADS1115_OUT &=~ IO_ADS1115_BIT_SDA  //P2^1低电平

#define SDA_In      IO_ADS1115_DIR &=~ IO_ADS1115_BIT_SDA//SDA改成输入模式
#define SDA_Out     IO_ADS1115_DIR |=  IO_ADS1115_BIT_SDA  //SDA变回输出模式
#define SCL_Out     IO_ADS1115_DIR |=  IO_ADS1115_BIT_SCL  //SCL设置为输出模式


/***************引脚定义*****************/
#define SCL              0x40
#define SDA              0x80

#define SCL_SEL          IO_ADS1115_SEL &=~ IO_ADS1115_BIT_SCL
#define SCL_DDR_OUT      IO_ADS1115_DIR |=  IO_ADS1115_BIT_SCL
#define SCL_H            IO_ADS1115_OUT |=  IO_ADS1115_BIT_SCL
#define SCL_L            IO_ADS1115_OUT &=~ IO_ADS1115_BIT_SCL

#define SDA_SEL          IO_ADS1115_SEL &= ~IO_ADS1115_BIT_SDA
#define SDA_DDR_OUT      IO_ADS1115_DIR |= IO_ADS1115_BIT_SDA
#define SDA_DDR_IN       IO_ADS1115_DIR &=  ~IO_ADS1115_BIT_SDA
#define SDA_IN           IO_ADS1115_IN
#define SDA_H            IO_ADS1115_OUT |= IO_ADS1115_BIT_SDA
#define SDA_L            IO_ADS1115_OUT &= ~IO_ADS1115_BIT_SDA

#define TRUE   1
#define FALSE  0

/*************ADDR Initial********************/
#define ADDRESS    ADDRESS_0   //ADDR PIN ->GND
#define ADDRESS_W  ADDRESS |0x00  //写地址
#define ADDRESS_R  ADDRESS|0x01    //读地址

/*************Config Initial*********************/
#define OS         OS_1
#define MUX        MUX_4      //AINp=AIN0, AINn=GND
#define PGA        PGA_0      //FS=6.144V
#define DR         DR_7       //Data Rate = 860
#define COMP_QUE   COMP_QUE_3


void DELAY_ADS1115(void);
void DELAY_ADS1115_1MS(void);
void StartADS1115(void);
void StopADS1115(void);
void ACKtoADS1115(void);
void NACKtoADS1115(void);
unsigned char CheckADS1115(void);
void Write1Byte_ADS1115(unsigned char DataByte);
unsigned char WriteNByte_ADS1115(unsigned char *writebuffer,unsigned char n);
unsigned char Read1Byte_ADS1115(void);
void ReadNByte_ADS1115(unsigned int*readbuff,unsigned char n);
void WriteWord_ADS1115(void);
void ReadWord_ADS1115(void);
void configADS1115(void);
void convertBINtoDEC(long int x);

void initADC(int channel);
void setADCChannel(int channel);
float getADCValue(void);


#endif
