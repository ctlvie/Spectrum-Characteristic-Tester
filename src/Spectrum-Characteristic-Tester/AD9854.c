/*=======================================================
Author				  :				ctlvie
Email Address		  :				ctlvie@gmail.com
Filename			  :				AD9854.c
Date				  :			    2018-11-22
Description			  :				AD9854 DDS��������

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
uchar FreqWord[6];               //6���ֽ�Ƶ�ʿ�����
//=================================================================
// �������� ��void delay (uint us)
// �������� ��us����ʱ,���2~4 us
// ��ڲ��� ��us  ��ʱʱ��ĳ���
// ���ڲ��� ����
//=================================================================
//====================================================================================
//��������:void initAD9854(void)
//��������:AD9854��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
void initAD9854(void)
{
    DIROUT_AD9854_DataBus;
    DIROUT_AD9854_AddrBus;
    DIROUT_RD;
    DIROUT_WR;
    DIROUT_RST;
    DIROUT_UDCLK;
    WR1_AD9854;//������д���ƶ˿���Ϊ��Ч
    RD1_AD9854;
    UCLK0_AD9854;
    RST1_AD9854;                 //��λAD9854
    RST0_AD9854;

    WriteByte_AD9854(0x1d,0x10);      //�رձȽ���
    WriteByte_AD9854(0x1e,CLK_Set);      //����ϵͳʱ�ӱ�Ƶ
    WriteByte_AD9854(0x1f,0x00);     //����ϵͳΪģʽ0�����ⲿ����
    WriteByte_AD9854(0x20,0x60);     //����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����

    UCLK1_AD9854;                     //����AD9854���
    UCLK0_AD9854;
}
//====================================================================================
//��������:void convertFreq(long Freq)
//��������:�����ź�Ƶ������ת��
//��ڲ���:Freq   ��Ҫת����Ƶ�ʣ�ȡֵ��0~SYSCLK/2
//���ڲ���:��   ����Ӱ��ȫ�ֱ���FreqWord[6]��ֵ
//˵����   ���㷨λ���ֽ�����㷨���й�ʽFTW = (Desired Output Frequency �� 2N)/SYSCLK
//         �õ����㷨������N=48��Desired Output Frequency Ϊ����Ҫ��Ƶ�ʣ���Freq��SYSCLK
//         Ϊ�ɱ�̵�ϵͳʱ�ӣ�FTWΪ48Bit��Ƶ�ʿ����֣���FreqWord[6]
//====================================================================================
void convertFreq(long Freq)
{
    ulong FreqBuf;
    ulong Temp=Freq_mult_ulong;
    uchar Array_Freq[4];                 //������Ƶ�����ӷ�Ϊ�ĸ��ֽ�
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
//��������:void setSinOutput(ulong Freq,uint Shape)
//��������:AD9854���Ҳ���������
//��ڲ���:Freq   Ƶ�����ã�ȡֵ��ΧΪ0~(1/2)*SYSCLK
//         Shape  ��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095) ,ȡֵԽ��,����Խ��
//���ڲ���:��
//====================================================================================
void setSinOutput(ulong Freq,uint Shape)
{
    uchar count;
    uchar Adress;

    Adress = 0x04;                        //ѡ��Ƶ�ʿ����ֵ�ַ�ĳ�ֵ

    convertFreq(Freq);                //Ƶ��ת��

    for(count=6;count>0;)             //д��6�ֽڵ�Ƶ�ʿ�����
    {
        WriteByte_AD9854(Adress++,FreqWord[--count]);
    }

    WriteByte_AD9854(0x21,Shape>>8);    //����Iͨ������
    WriteByte_AD9854(0x22,(uchar)(Shape&0xff));

    WriteByte_AD9854(0x23,Shape>>8);    //����Qͨ������
    WriteByte_AD9854(0x24,(uchar)(Shape&0xff));

    UCLK1_AD9854;                    //����AD9854���
        UCLK0_AD9854;
}
//====================================================================================
//��������:void Freq_doublt_convert(double Freq)
//��������:�����ź�Ƶ������ת��
//��ڲ���:Freq   ��Ҫת����Ƶ�ʣ�ȡֵ��0~SYSCLK/2
//���ڲ���:��   ����Ӱ��ȫ�ֱ���FreqWord[6]��ֵ
//˵����   �й�ʽFTW = (Desired Output Frequency �� 2N)/SYSCLK�õ��ú�����
//         ����N=48��Desired Output Frequency Ϊ����Ҫ��Ƶ�ʣ���Freq��SYSCLK
//         Ϊ�ɱ�̵�ϵͳʱ�ӣ�FTWΪ48Bit��Ƶ�ʿ����֣���FreqWord[6]
//ע�⣺   �ú��������溯��������Ϊ�ú�������ڲ���Ϊdouble����ʹ�źŵ�Ƶ�ʸ���ȷ
//         ���꽨����100HZ�����ñ��������ڸ���100HZ��������ú���void convertFreq(long Freq)
//====================================================================================
void convertFreq_double(double Freq)
{
   ulong Low32;
   uint  High16;
    double Temp=Freq_mult_doulle;                   //23ca99Ϊ2��48�η�����120M
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
//��������:void setSinOutput_double(double Freq,uint Shape)
//��������:AD9854���Ҳ���������
//��ڲ���:Freq   Ƶ�����ã�ȡֵ��ΧΪ0~1/2*SYSCLK
//         Shape  ��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095)
//���ڲ���:��
//====================================================================================
void setSinOutput_double(double Freq,uint Shape)
{
    uchar count=0;
    uchar Adress;

    Adress=0x04;                             //ѡ��Ƶ�ʿ�����1��ַ�ĳ�ֵ

    convertFreq_double(Freq);                 //Ƶ��ת��

    for(count=6;count>0;)                       //д��6�ֽڵ�Ƶ�ʿ�����
    {
        WriteByte_AD9854(Adress++,FreqWord[--count]);
    }

    WriteByte_AD9854(0x21,Shape>>8);    //����Iͨ������
    WriteByte_AD9854(0x22,(uchar)(Shape&0xff));

    WriteByte_AD9854(0x23,Shape>>8);    //����Qͨ������
    WriteByte_AD9854(0x24,(uchar)(Shape&0xff));

    UCLK1_AD9854;                    //����AD9854���
        UCLK0_AD9854;
}

//====================================================================================
//��������:void WriteByte_AD9854(uchar addr,uchar dat)
//��������:AD9854���п�д������
//��ڲ���:addr     6λ��ַ
//         dat      д�������
//���ڲ���:��
//====================================================================================
void WriteByte_AD9854(uchar addr,uchar dat)
{
    AD9854_AddrBus  = addr;
    AD9854_DataBus = dat;
    WR0_AD9854;
    WR1_AD9854;
}
