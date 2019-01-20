/*=======================================================
Author				  :				ctlvie
Email Address		  :				ctlvie@gmail.com
Filename			  :				AD9854.c
Date				  :			    2018-11-22
Description			  :				AD9854 DDS驱动程序

Modification History:
Date		By			Version		Description
----------------------------------------------------------
181122		ctlvie		1.0
181216      ctlvie      2.0         Final Version			
========================================================*/

#include<msp430f5529.h>
#include"AD9854.h"

#include<msp430f5529.h>
#include"AD9854.h"
ulong  Freq_mult_ulong  = 1005268;
double Freq_mult_doulle = 1005267.773966629;
uchar FreqWord[6];               //6个字节频率控制字
//=================================================================
// 函数名称 ：void delay (uint us)
// 函数功能 ：us级延时,大概2~4 us
// 入口参数 ：us  延时时间的长短
// 出口参数 ：无
//=================================================================
//====================================================================================
//函数名称:void initAD9854(void)
//函数功能:AD9854初始化
//入口参数:无
//出口参数:无
//====================================================================================
void initAD9854(void)
{
    DIROUT_AD9854_DataBus;
    DIROUT_AD9854_AddrBus;
    DIROUT_RD;
    DIROUT_WR;
    DIROUT_RST;
    DIROUT_UDCLK;
    WR1_AD9854;//将读、写控制端口设为无效
    RD1_AD9854;
    UCLK0_AD9854;
    RST1_AD9854;                 //复位AD9854
    RST0_AD9854;

    WriteByte_AD9854(0x1d,0x10);      //关闭比较器
    WriteByte_AD9854(0x1e,CLK_Set);      //设置系统时钟倍频
    WriteByte_AD9854(0x1f,0x00);     //设置系统为模式0，由外部更新
    WriteByte_AD9854(0x20,0x60);     //设置为可调节幅度，取消插值补偿

    UCLK1_AD9854;                     //更新AD9854输出
    UCLK0_AD9854;
}
//====================================================================================
//函数名称:void convertFreq(long Freq)
//函数功能:正弦信号频率数据转换
//入口参数:Freq   需要转换的频率，取值从0~SYSCLK/2
//出口参数:无   但是影响全局变量FreqWord[6]的值
//说明：   该算法位多字节相乘算法，有公式FTW = (Desired Output Frequency × 2N)/SYSCLK
//         得到该算法，其中N=48，Desired Output Frequency 为所需要的频率，即Freq，SYSCLK
//         为可编程的系统时钟，FTW为48Bit的频率控制字，即FreqWord[6]
//====================================================================================
void convertFreq(long Freq)
{
    ulong FreqBuf;
    ulong Temp=Freq_mult_ulong;
    uchar Array_Freq[4];                 //将输入频率因子分为四个字节
    Array_Freq[0]=(uchar)Freq;
    Array_Freq[1]=(uchar)(Freq>>8);
    Array_Freq[2]=(uchar)(Freq>>16);
    Array_Freq[3]=(uchar)(Freq>>24);

    FreqBuf=Temp*Array_Freq[0];
    FreqWord[0]=FreqBuf;
    FreqBuf>>=8;

    FreqBuf+=(Temp*Array_Freq[1]);
    FreqWord[1]=FreqBuf;
    FreqBuf>>=8;

    FreqBuf+=(Temp*Array_Freq[2]);
    FreqWord[2]=FreqBuf;
    FreqBuf>>=8;

    FreqBuf+=(Temp*Array_Freq[3]);
    FreqWord[3]=FreqBuf;
    FreqBuf>>=8;

    FreqWord[4]=FreqBuf;
    FreqWord[5]=FreqBuf>>8;
}
//====================================================================================
//函数名称:void setSinOutput(ulong Freq,uint Shape)
//函数功能:AD9854正弦波产生程序
//入口参数:Freq   频率设置，取值范围为0~(1/2)*SYSCLK
//         Shape  幅度设置. 为12 Bit,取值范围为(0~4095) ,取值越大,幅度越大
//出口参数:无
//====================================================================================
void setSinOutput(ulong Freq,uint Shape)
{
    uchar count;
    uchar Adress;

    Adress = 0x04;                        //选择频率控制字地址的初值

    convertFreq(Freq);                //频率转换

    for(count=6;count>0;)             //写入6字节的频率控制字
    {
        WriteByte_AD9854(Adress++,FreqWord[--count]);
    }

    WriteByte_AD9854(0x21,Shape>>8);    //设置I通道幅度
    WriteByte_AD9854(0x22,(uchar)(Shape&0xff));

    WriteByte_AD9854(0x23,Shape>>8);    //设置Q通道幅度
    WriteByte_AD9854(0x24,(uchar)(Shape&0xff));

    UCLK1_AD9854;                    //更新AD9854输出
        UCLK0_AD9854;
}
//====================================================================================
//函数名称:void Freq_doublt_convert(double Freq)
//函数功能:正弦信号频率数据转换
//入口参数:Freq   需要转换的频率，取值从0~SYSCLK/2
//出口参数:无   但是影响全局变量FreqWord[6]的值
//说明：   有公式FTW = (Desired Output Frequency × 2N)/SYSCLK得到该函数，
//         其中N=48，Desired Output Frequency 为所需要的频率，即Freq，SYSCLK
//         为可编程的系统时钟，FTW为48Bit的频率控制字，即FreqWord[6]
//注意：   该函数与上面函数的区别为该函数的入口参数为double，可使信号的频率更精确
//         谷雨建议在100HZ以下用本函数，在高于100HZ的情况下用函数void convertFreq(long Freq)
//====================================================================================
void convertFreq_double(double Freq)
{
   ulong Low32;
   uint  High16;
    double Temp=Freq_mult_doulle;                   //23ca99为2的48次方除以120M
   Freq*=(double)(Temp);
//  1 0000 0000 0000 0000 0000 0000 0000 0000 = 4294967295
   High16 = (int)(Freq/4294967295);                  //2^32 = 4294967295
   Freq -= (double)High16*4294967295;
   Low32 = (ulong)Freq;

    FreqWord[0]=Low32;
    FreqWord[1]=Low32>>8;
    FreqWord[2]=Low32>>16;
    FreqWord[3]=Low32>>24;
    FreqWord[4]=High16;
    FreqWord[5]=High16>>8;
}
//====================================================================================
//函数名称:void setSinOutput_double(double Freq,uint Shape)
//函数功能:AD9854正弦波产生程序
//入口参数:Freq   频率设置，取值范围为0~1/2*SYSCLK
//         Shape  幅度设置. 为12 Bit,取值范围为(0~4095)
//出口参数:无
//====================================================================================
void setSinOutput_double(double Freq,uint Shape)
{
    uchar count=0;
    uchar Adress;

    Adress=0x04;                             //选择频率控制字1地址的初值

    convertFreq_double(Freq);                 //频率转换

    for(count=6;count>0;)                       //写入6字节的频率控制字
    {
        WriteByte_AD9854(Adress++,FreqWord[--count]);
    }

    WriteByte_AD9854(0x21,Shape>>8);    //设置I通道幅度
    WriteByte_AD9854(0x22,(uchar)(Shape&0xff));

    WriteByte_AD9854(0x23,Shape>>8);    //设置Q通道幅度
    WriteByte_AD9854(0x24,(uchar)(Shape&0xff));

    UCLK1_AD9854;                    //更新AD9854输出
        UCLK0_AD9854;
}

//====================================================================================
//函数名称:void WriteByte_AD9854(uchar addr,uchar dat)
//函数功能:AD9854并行口写入数据
//入口参数:addr     6位地址
//         dat      写入的数据
//出口参数:无
//====================================================================================
void WriteByte_AD9854(uchar addr,uchar dat)
{
    AD9854_AddrBus  = addr;
    AD9854_DataBus = dat;
    WR0_AD9854;
    WR1_AD9854;
}
