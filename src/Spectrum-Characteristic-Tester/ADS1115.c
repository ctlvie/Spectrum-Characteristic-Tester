/*=======================================================
Author				  :				ctlvie
Email Address		  :				ctlvie@gmail.com
Filename			  :				ADS1115.c
Date				  :			    2018-11-20
Description			  :				ADS1115 ģ��ת������������

Modification History:
Date		By			Version		Description
----------------------------------------------------------
181120		ctlvie		1.0			ADS1115��ͨ��ת������ʵ��
========================================================*/
#include<msp430f5529.h>
#include"ADS1115.h"

unsigned int Config;
unsigned char Writebuff[4],Readbuff[3];
unsigned int Result[2];
int D_ADS; //ת����������
float VIN_DAS; //����ĵ�ѹֵ
unsigned char  t_DAS;
int  Format[5]={0};  //ת����������ת����ʮ����


/*******************************************
�������ƣ�delay
��    �ܣ���ʱԼ15us��ʱ��
��    ������
����ֵ  ����
********************************************/
void DELAY_ADS1115(void)
{
    unsigned char i;

    for(i = 0;i < 15;i++)
      _NOP();
}

/******************��ʱ1ms��ʱ��*********************/
void DELAY_ADS1115_1MS(void)
{
    unsigned char i;
    for(i = 150;i > 0;i--)    _NOP();
}


/*******************************************
�������ƣ�Start
��    �ܣ����IIC����ʼ��������
��    ������
����ֵ  ����
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
�������ƣ�Stop
��    �ܣ����IIC����ֹ��������
��    ������
����ֵ  ����
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
�������ƣ�ACK
��    �ܣ����IIC������Ӧ�����
��    ������
����ֵ  ����
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
�������ƣ�NACK
��    �ܣ����IIC��������Ӧ�����
��    ������
����ֵ  ����
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

/**********���Ӧ���źź���******************/
/*�������ֵΪ1��֤����Ӧ���źţ���֮û��*/
/*******************************************
�������ƣ�Check
��    �ܣ����ӻ���Ӧ�����
��    ������
����ֵ  ���ӻ��Ƿ���Ӧ��1--�У�0--��
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
    slaveack = SDA_IN&SDA;   //����SDA��ֵ
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
    SDA_DDR_IN; //�˿�Ϊ����
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
            NACKtoADS1115();  //���������ֽ�
        else
            ACKtoADS1115(); //�������ֽ�
    }
    StopADS1115();

}


/*****************��ʼ��******************/
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
     StartADS1115();   //д��4���ֽ�
     do
     {
          t=WriteNByte_ADS1115(Writebuff,4);
     }while(t==0);
}

/***************Read Word***********************/
void ReadWord_ADS1115(void)
{
    int t;
    StartADS1115();  //д��2���ֽ�
    do
    {
      t=WriteNByte_ADS1115(Readbuff,2);
    }while(t==0);

    StartADS1115();   //д��2���ֽ�
    do
    {
      t=WriteNByte_ADS1115(&Readbuff[2],1);
    }while(t==0);

    ReadNByte_ADS1115(Result,2);  //����2���ֽ�

}

/***************ADS1115********************/
float getADCValue(void)
{
    float result = 0;
    initADS1115();
    WriteWord_ADS1115();
    DELAY_ADS1115_1MS();
    ReadWord_ADS1115();

    D_ADS=Result[0]*256+Result[1];  //ת����������
    t_DAS=0;
    convertBINtoDEC(D_ADS);    //ת����������ת����ʮ����
    VIN_DAS=6.144*D_ADS/32768;  //����ĵ�ѹֵ
    result = VIN_DAS;
    return result;
}

//�õݹ鷽����������xת��Ϊ10���Ƹ�ʽ
void convertBINtoDEC(long int x)
{
    if(x)
    {
          Format[t_DAS]= x%10;
          t_DAS  ++;
          convertBINtoDEC(x/10);
    }
}




