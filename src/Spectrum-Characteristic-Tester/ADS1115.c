/*=======================================================
Author				  :				ctlvie
Email Address		  :				ctlvie@gmail.com
Filename			  :				ADS1115.c
Date				  :			    2018-11-20
Description			  :				ADS1115 模数转换器驱动程序

Modification History:
Date		By			Version		Description
----------------------------------------------------------
181120		ctlvie		1.0			ADS1115单通道转换功能实现
========================================================*/
#include<msp430f5529.h>
#include"ADS1115.h"

unsigned int Config;
unsigned char Writebuff[4],Readbuff[3];
unsigned int Result[2];
int D_ADS; //转换的数字量
float VIN_DAS; //输入的电压值
unsigned char  t_DAS;
int  Format[5]={0};  //转换的数字量转换成十进制


/*******************************************
函数名称：delay
功    能：延时约15us的时间
参    数：无
返回值  ：无
********************************************/
void DELAY_ADS1115(void)
{
    unsigned char i;

    for(i = 0;i < 15;i++)
      _NOP();
}

/******************延时1ms的时间*********************/
void DELAY_ADS1115_1MS(void)
{
    unsigned char i;
    for(i = 150;i > 0;i--)    _NOP();
}


/*******************************************
函数名称：Start
功    能：完成IIC的起始条件操作
参    数：无
返回值  ：无
********************************************/
void StartADS1115(void)
{
        SCL_H;
        SDA_H;
        DELAY_ADS1115();
    SDA_L;
        DELAY_ADS1115();
    SCL_L;
        DELAY_ADS1115();
}

/*******************************************
函数名称：Stop
功    能：完成IIC的终止条件操作
参    数：无
返回值  ：无
********************************************/
void StopADS1115(void)
{
        SDA_L;
        DELAY_ADS1115();
        SCL_H;
        DELAY_ADS1115();
    SDA_H;
        DELAY_ADS1115();
}


/*******************************************
函数名称：ACK
功    能：完成IIC的主机应答操作
参    数：无
返回值  ：无
********************************************/
void ACKtoADS1115(void)
{
        SDA_L;
        _NOP(); _NOP();
        SCL_H;
        DELAY_ADS1115();
        SCL_L;
    _NOP(); _NOP();
    SDA_H;
        DELAY_ADS1115();
}

/*******************************************
函数名称：NACK
功    能：完成IIC的主机无应答操作
参    数：无
返回值  ：无
********************************************/
void NACKtoADS1115(void)
{
    SDA_H;
    _NOP(); _NOP();
    SCL_H;
    DELAY_ADS1115();
    SCL_L;
    _NOP(); _NOP();
    SDA_L;
    DELAY_ADS1115();
}

/**********检查应答信号函数******************/
/*如果返回值为1则证明有应答信号，反之没有*/
/*******************************************
函数名称：Check
功    能：检查从机的应答操作
参    数：无
返回值  ：从机是否有应答：1--有，0--无
********************************************/
unsigned char CheckADS1115(void)
{
    unsigned char slaveack;

    SDA_H;
    _NOP(); _NOP();
    SCL_H;
    _NOP(); _NOP();
    SDA_DDR_IN;
    _NOP(); _NOP();
    slaveack = SDA_IN&SDA;   //读入SDA数值
    SCL_L;
    DELAY_ADS1115();
    SDA_DDR_OUT;
    if(slaveack)    return FALSE;
    else            return TRUE;
}

/***************Write a Byte****************/
void Write1Byte_ADS1115(unsigned char DataByte)
{
    int i;

    for(i=0;i<8;i++)
    {
        if(DataByte&0x80)  //if((DataByte<<i)&0x80)
          SDA_H;
        else
          SDA_L;
        DELAY_ADS1115();
    SCL_H;
        DELAY_ADS1115();
        SCL_L;
        DELAY_ADS1115();
        DataByte <<= 1;
    }
        SDA_H;
        _NOP();
}

/***************Write N Byte****************/
unsigned char WriteNByte_ADS1115(unsigned char *writebuffer,unsigned char n)
{
    int i;

    for(i=0;i<n;i++)
    {
        Write1Byte_ADS1115(*writebuffer);
        if(CheckADS1115())
        {
            writebuffer ++;
        }
        else
        {
            StopADS1115();
            return FALSE;
        }
    }
    StopADS1115();
    return TRUE;
}

/***************Read a Byte****************/
unsigned char Read1Byte_ADS1115(void)
{
    unsigned char data=0,FLAG, i;

    for(i=0;i<8;i++)
    {

    SDA_H;
    DELAY_ADS1115();
        SCL_H;
    SDA_DDR_IN; //端口为输入
    DELAY_ADS1115();
        FLAG=SDA_IN& SDA;
        data <<= 1;
    if( FLAG)
       data |= 0x01;
    SDA_DDR_OUT;
        SCL_L;
        DELAY_ADS1115();
    }
    return data;
}

/***************Read N Byte****************/
void ReadNByte_ADS1115(unsigned int*readbuff,unsigned char n)
{
    unsigned char i;

    for(i=0;i<n;i++)
    {
        readbuff[i]=Read1Byte_ADS1115();
        if(i==n-1)
            NACKtoADS1115();  //不连续读字节
        else
            ACKtoADS1115(); //连续读字节
    }
    StopADS1115();

}


/*****************初始化******************/
void initADS1115(void)
{
    SCL_SEL;
    SDA_SEL;
    SCL_DDR_OUT;
    SDA_DDR_OUT;
    SDA_H;
    Config = OS+MUX+PGA+DR+COMP_QUE;
    Writebuff[0]=ADDRESS_W;
    Writebuff[1]=Pointer_1;
    Writebuff[2]=Config/256;
    Writebuff[3]=Config%256;

    Readbuff[0]=ADDRESS_W;
    Readbuff[1]=Pointer_0;

    Readbuff[2]=ADDRESS_R;
}

/***************Write a Word***********************/
void WriteWord_ADS1115(void)
{
     int t;
     StartADS1115();   //写入4个字节
     do
     {
          t=WriteNByte_ADS1115(Writebuff,4);
     }while(t==0);
}

/***************Read Word***********************/
void ReadWord_ADS1115(void)
{
    int t;
    StartADS1115();  //写入2个字节
    do
    {
      t=WriteNByte_ADS1115(Readbuff,2);
    }while(t==0);

    StartADS1115();   //写入2个字节
    do
    {
      t=WriteNByte_ADS1115(&Readbuff[2],1);
    }while(t==0);

    ReadNByte_ADS1115(Result,2);  //读出2个字节

}

/***************ADS1115********************/
float getADCValue(void)
{
    float result = 0;
    initADS1115();
    WriteWord_ADS1115();
    DELAY_ADS1115_1MS();
    ReadWord_ADS1115();

    D_ADS=Result[0]*256+Result[1];  //转换的数字量
    t_DAS=0;
    convertBINtoDEC(D_ADS);    //转换的数字量转换成十进制
    VIN_DAS=6.144*D_ADS/32768;  //输入的电压值
    result = VIN_DAS;
    return result;
}

//用递归方法将正整数x转换为10进制格式
void convertBINtoDEC(long int x)
{
    if(x)
    {
          Format[t_DAS]= x%10;
          t_DAS  ++;
          convertBINtoDEC(x/10);
    }
}




