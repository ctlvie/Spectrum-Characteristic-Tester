/*=======================================================
Author				  :				ctlvie
Email Address		  :				ctlvie@gmail.com
Filename			  :				AD9854.h
Date				  :			    2018-11-22
Description			  :				AD9854 DDS��������

Modification History:
Date		By			Version		Description
----------------------------------------------------------
181122		ctlvie		1.0
181216      ctlvie      2.0         Final Version			
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
    ���߷�ʽ:
    1. DataBus  ->  P3.0 ~ P3.7
    2. AddrBus  ->  P4.0 ~ P4.7
    3. RD       ->  P8.1
    4. WR       ->  P8.2
    5. UCLK     ->  P7.0
    6. RST      ->  P7.4
========================================================*/


#define IO_AD9854_DataBus_OUT       P3OUT
#define IO_AD9854_AddrBus_OUT       P4OUT
#define IO_AD9854_CtlBus1_OUT       P8OUT
#define IO_AD9854_CtlBus2_OUT       P7OUT
#define IO_AD9854_DataBus_DIR       P3DIR 
#define IO_AD9854_AddrBus_DIR       P4DIR
#define IO_AD9854_CtlBus1_DIR       P8DIR
#define IO_AD9854_CtlBus2_DIR       P7DIR

#define IO_AD9854_BIT_RD            BIT1
#define IO_AD9854_BIT_WR            BIT2
#define IO_AD9854_BIT_UCLK          BIT0
#define IO_AD9854_BIT_RST           BIT4



#define AD9854_DataBus              IO_AD9854_DataBus_OUT             //P3BIT0 ~ BIT7 Ϊ��������, ��Ӧ D0 ~ D7
#define AD9854_AddrBus              IO_AD9854_AddrBus_OUT             //P4BIT0 ~ BIT5 Ϊ��ַ����, ��Ӧ A0 ~ A5
#define DIROUT_AD9854_DataBus       IO_AD9854_DataBus_DIR = BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7 ;      //������IO����Ϊ���
#define DIROUT_AD9854_AddrBus       IO_AD9854_AddrBus_DIR = BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5;     //��ַ��IO����Ϊ���
#define DIROUT_RD                   IO_AD9854_CtlBus1_DIR |= IO_AD9854_BIT_RD;    //P8.1 -> RD
#define DIROUT_WR                   IO_AD9854_CtlBus1_DIR |= IO_AD9854_BIT_WR;    //P8.2 -> WR
#define DIROUT_UDCLK                IO_AD9854_CtlBus2_DIR |= IO_AD9854_BIT_UCLK;    //P7.0 -> UCLK
#define DIROUT_RST                  IO_AD9854_CtlBus2_DIR |= IO_AD9854_BIT_RST;    //P7.4 -> RST
#define RD0_AD9854                  IO_AD9854_CtlBus1_OUT &= ~IO_AD9854_BIT_RD;   //RDʹ��(����Ч)
#define RD1_AD9854                  IO_AD9854_CtlBus1_OUT |= IO_AD9854_BIT_RD;
#define WR0_AD9854                  IO_AD9854_CtlBus1_OUT &= ~IO_AD9854_BIT_WR;   //WRʹ��(����Ч)
#define WR1_AD9854                  IO_AD9854_CtlBus1_OUT |= IO_AD9854_BIT_WR;
#define UCLK0_AD9854                IO_AD9854_CtlBus2_OUT &= ~IO_AD9854_BIT_UCLK;   //����ʱ��
#define UCLK1_AD9854                IO_AD9854_CtlBus2_OUT |= IO_AD9854_BIT_UCLK;
#define RST0_AD9854                 IO_AD9854_CtlBus2_OUT &= ~IO_AD9854_BIT_RST;   //��λ�ź�
#define RST1_AD9854                 IO_AD9854_CtlBus2_OUT |= IO_AD9854_BIT_RST;
//**************************���²���Ϊ��������********************************

void WriteByte_AD9854(uchar addr,uchar dat);
void initAD9854(void);
void convertFreq(long Freq);
void setSinOutput(ulong Freq,uint Shape);
void convertFreq_double(double Freq);
void setSinOutput_double(double Freq,uint Shape);


//extern uchar FreqWord[6];              //6���ֽ�Ƶ�ʿ�����
//**********************����Ϊϵͳʱ���Լ�����ر�������**************************

/*
      �˴������Լ�����Ҫ����ϵͳʱ���Լ�������ص����ӣ�һ������ֻ�迪��һ��
      CLK_SetΪʱ�ӱ�Ƶ���ã�������4~20����Ƶ��������ܳ���300MHZ
      Freq_mult_ulong��Freq_mult_doulle��Ϊ2��48�η�����ϵͳʱ�ӣ�һ��Ϊ�����Σ�һ��Ϊ˫������
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
