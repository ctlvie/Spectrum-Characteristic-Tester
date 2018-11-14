/*
 * AD9854.h
 *
 *  Created on: 2018��11��15��
 *      Author: Admin
 */

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
//**************************�޸�Ӳ��ʱҪ�޸ĵĲ���********************************

#define AD9854_DataBus P3OUT
#define AD9854_AdrBus  P2OUT
#define DIROUT_AD9854_DataBus  P3DIR = 0XFF //������IO����Ϊ���
#define DIROUT_AD9854_AdrBus   P2DIR = 0XFF//��ַ��IO����Ϊ���
#define DIROUT_RD    P4DIR |= BIT4;
#define DIROUT_WR    P4DIR |= BIT5;
#define DIROUT_UDCLK  P4DIR |= BIT6;
#define DIROUT_RST  P4DIR |= BIT7;
#define CLR_9854RD   P4OUT &= ~BIT4;   //AD9854��ʹ�ܣ�����Ч
#define SET_9854RD   P4OUT |= BIT4;
#define CLR_9854WR   P4OUT &= ~BIT5;   //AD9854дʹ�ܣ�����Ч
#define SET_9854WR   P4OUT |= BIT5;
#define CLR_9854UDCLK   P4OUT &= ~BIT6;   //����ʱ��
#define SET_9854UDCLK   P4OUT |= BIT6;
#define CLR_9854RST   P4OUT &= ~BIT7;   //��λ�ź�
#define SET_9854RST   P4OUT |= BIT7;
//**************************���²���Ϊ��������********************************

void AD9854_WR_Byte(uchar addr,uchar dat);
void AD9854_Init(void);
void Freq_convert(long Freq);
void AD9854_SetSine(ulong Freq,uint Shape);
void Freq_double_convert(double Freq);
void AD9854_SetSine_double(double Freq,uint Shape);




#endif /* AD9854_H_ */
