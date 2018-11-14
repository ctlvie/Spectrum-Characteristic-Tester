/*
 * AD9854.h
 *
 *  Created on: 2018年11月15日
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
//**************************修改硬件时要修改的部分********************************

#define AD9854_DataBus P3OUT
#define AD9854_AdrBus  P2OUT
#define DIROUT_AD9854_DataBus  P3DIR = 0XFF //数据线IO口设为输出
#define DIROUT_AD9854_AdrBus   P2DIR = 0XFF//地址线IO口设为输出
#define DIROUT_RD    P4DIR |= BIT4;
#define DIROUT_WR    P4DIR |= BIT5;
#define DIROUT_UDCLK  P4DIR |= BIT6;
#define DIROUT_RST  P4DIR |= BIT7;
#define CLR_9854RD   P4OUT &= ~BIT4;   //AD9854读使能，低有效
#define SET_9854RD   P4OUT |= BIT4;
#define CLR_9854WR   P4OUT &= ~BIT5;   //AD9854写使能，低有效
#define SET_9854WR   P4OUT |= BIT5;
#define CLR_9854UDCLK   P4OUT &= ~BIT6;   //更新时钟
#define SET_9854UDCLK   P4OUT |= BIT6;
#define CLR_9854RST   P4OUT &= ~BIT7;   //复位信号
#define SET_9854RST   P4OUT |= BIT7;
//**************************以下部分为函数定义********************************

void AD9854_WR_Byte(uchar addr,uchar dat);
void AD9854_Init(void);
void Freq_convert(long Freq);
void AD9854_SetSine(ulong Freq,uint Shape);
void Freq_double_convert(double Freq);
void AD9854_SetSine_double(double Freq,uint Shape);




#endif /* AD9854_H_ */
