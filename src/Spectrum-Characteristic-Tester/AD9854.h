/*=======================================================
Author				  :				ctlvie
Email Address		  :				ctlvie@gmail.com
Filename			  :				AD9854.h
Date				  :			    2018-11-22
Description			  :				AD9854 DDS驱动程序

Modification History:
Date		By			Version		Description
----------------------------------------------------------
181122		ctlvie		1.0			
========================================================*/

#ifndef AD9854_H_
#define AD9854_H_
#ifndef uchar
#define uchar unsigned char
#endif
#ifndef ulong
#define ulong unsigned long
#endif
#ifndef uint
#define uint unsigned int
#endif

/*=======================================================
    接线方式:
    1. DataBus  ->  P3.0 ~ P3.7
    2. AddrBus  ->  P4.0 ~ P4.7
    3. RD       ->  P1.2
    4. WR       ->  P1.3
    5. UCLK     ->  P1.4
    6. RST      ->  P1.5
========================================================*/


#define IO_AD9854_DataBus_OUT       P3OUT
#define IO_AD9854_AddrBus_OUT       P4OUT
#define IO_AD9854_CtlBus_OUT        P1OUT
#define IO_AD9854_DataBus_DIR       P3DIR 
#define IO_AD9854_AddrBus_DIR       P4DIR
#define IO_AD9854_CtlBus_DIR        P1DIR

#define IO_AD9854_BIT_RD            BIT4
#define IO_AD9854_BIT_WR            BIT5
#define IO_AD9854_BIT_UCLK          BIT6
#define IO_AD9854_BIT_RST           BIT7



#define AD9854_DataBus              IO_AD9854_DataBus_OUT             //P3BIT0 ~ IO_AD9854_BIT_RST 为数据总线, 对应 D0 ~ D7
#define AD9854_AddrBus              IO_AD9854_AddrBus_OUT             //P2BIT0 ~ IO_AD9854_BIT_RST 为地址总线, 对应 A0 ~ A5
#define DIROUT_AD9854_DataBus       IO_AD9854_DataBus_DIR = BIT0 + BIT1 + BIT2 + BIT3 + IO_AD9854_BIT_RD + IO_AD9854_BIT_WR + IO_AD9854_BIT_UCLK + IO_AD9854_BIT_RST ;      //数据线IO口设为输出
#define DIROUT_AD9854_AddrBus       IO_AD9854_AddrBus_DIR = BIT0 + BIT1 + BIT2 + BIT3 + IO_AD9854_BIT_RD + IO_AD9854_BIT_WR ;     //地址线IO口设为输出
#define DIROUT_RD                   IO_AD9854_CtlBus_DIR |= IO_AD9854_BIT_RD;    //P4.4 -> RD
#define DIROUT_WR                   IO_AD9854_CtlBus_DIR |= IO_AD9854_BIT_WR;    //P4.5 -> WR
#define DIROUT_UDCLK                IO_AD9854_CtlBus_DIR |= IO_AD9854_BIT_UCLK;    //P4.6 -> UCLK
#define DIROUT_RST                  IO_AD9854_CtlBus_DIR |= IO_AD9854_BIT_RST;    //P4.7 -> RST
#define RD0_AD9854                  IO_AD9854_CtlBus_OUT &= ~IO_AD9854_BIT_RD;   //RD使能(低有效)
#define RD1_AD9854                  IO_AD9854_CtlBus_OUT |= IO_AD9854_BIT_RD;
#define WR0_AD9854                  IO_AD9854_CtlBus_OUT &= ~IO_AD9854_BIT_WR;   //WR使能(低有效)
#define WR1_AD9854                  IO_AD9854_CtlBus_OUT |= IO_AD9854_BIT_WR;
#define UCLK0_AD9854                IO_AD9854_CtlBus_OUT &= ~IO_AD9854_BIT_UCLK;   //更新时钟
#define UCLK1_AD9854                IO_AD9854_CtlBus_OUT |= IO_AD9854_BIT_UCLK;
#define RST0_AD9854                 IO_AD9854_CtlBus_OUT &= ~IO_AD9854_BIT_RST;   //复位信号
#define RST1_AD9854                 IO_AD9854_CtlBus_OUT |= IO_AD9854_BIT_RST;
//**************************以下部分为函数定义********************************

void WriteByte_AD9854(uchar addr,uchar dat);
void initAD9854(void);
void convertFreq(long Freq);
void setSinOutput(ulong Freq,uint Shape);
void convertFreq_double(double Freq);
void AD9854_SetSine_double(double Freq,uint Shape);


//extern uchar FreqWord[6];              //6个字节频率控制字
//**********************以下为系统时钟以及其相关变量设置**************************

/*
      此处根据自己的需要设置系统时钟以及与其相关的因子，一次需且只需开启一个
      CLK_Set为时钟倍频设置，可设置4~20倍倍频，但最大不能超过300MHZ
      Freq_mult_ulong和Freq_mult_doulle均为2的48次方除以系统时钟，一个为长整形，一个为双精度型
*/

/*
#define      CLK_Set            4
const ulong  Freq_mult_ulong  = 3518437;
const double Freq_mult_doulle = 3518437.2088832;
*/

/*
#define      CLK_Set            5
const ulong  Freq_mult_ulong  = 2814750;
const double Freq_mult_doulle = 2814749.76710656;
*/

/*
#define      CLK_Set            6
const ulong  Freq_mult_ulong  = 2345625;
const double Freq_mult_doulle = 2345624.80592213;
*/

/*
#define      CLK_Set            7
const ulong  Freq_mult_ulong  = 2010536;
const double Freq_mult_doulle = 2010535.54793326;
*/

/*
#define      CLK_Set            8
const ulong  Freq_mult_ulong  = 1759219;
const double Freq_mult_doulle = 1759218.6044416;
*/

/*
#define      CLK_Set            9
const ulong  Freq_mult_ulong  = 1563750;
const double Freq_mult_doulle = 1563749.87061476;
*/

/*
#define      CLK_Set            10
const ulong  Freq_mult_ulong  = 1407375;
const double Freq_mult_doulle = 1407374.88355328;
*/

/*
#define      CLK_Set            11
const ulong  Freq_mult_ulong  = 1279432;
const double Freq_mult_doulle = 1279431.712321164;
*/

/*
#define      CLK_Set            12
const ulong  Freq_mult_ulong  = 1172812;
const double Freq_mult_doulle = 1172812.402961067;
*/

/*
#define      CLK_Set            13
const ulong  Freq_mult_ulong  = 1082596;
const double Freq_mult_doulle = 1082596.064271754;
*/


#define      CLK_Set            14
//extern ulong  Freq_mult_ulong  = 1005268;
//extern double Freq_mult_doulle = 1005267.773966629;


/*
#define      CLK_Set            15
const ulong  Freq_mult_ulong  = 938250;
const double Freq_mult_doulle = 938249.9223688533;
*/


#endif /* AD9854_H_ */
