#include<msp430f5529.h>
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


void Delay(void);
void Delay_1ms(void);
void Start(void);
void Stop(void);
void ACK(void);
void NACK(void);
unsigned char Check(void);
void Write_1_Byte(unsigned char DataByte);
unsigned char Write_N_Byte(unsigned char *writebuffer,unsigned char n);
unsigned char Read_1_Byte(void);
void Read_N_Byte(unsigned int *readbuff,unsigned char n);
void InitADS1115(void);
void WriteWord(void);
void ReadWord(void);
float ADS1115(void);
void ShowInt(long int x);

/************************************
 *   ADDR---> GND
 *   VDD ---> 3.3V
 *   SCL ---> P2.6
 *   SDA ---> P2.7
 *   AINp=AIN0, AINn=GND
 ************************************/


/***************引脚定义*****************/
#define SCL              0x40
#define SDA              0x80

#define SCL_SEL          P2SEL &= ~SCL
#define SCL_DDR_OUT      P2DIR |= SCL
#define SCL_OUT      P2OUT
#define SCL_H        SCL_OUT |= SCL
#define SCL_L        SCL_OUT &= ~SCL

#define SDA_SEL          P2SEL &= ~SDA
#define SDA_DDR_OUT      P2DIR |= SDA
#define SDA_DDR_IN       P2DIR &=  ~SDA
#define SDA_OUT      P2OUT
#define SDA_IN           P2IN
#define SDA_H        SDA_OUT |= SDA
#define SDA_L        SDA_OUT &= ~SDA

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
void Delay(void)
{
    unsigned char i;

    for(i = 0;i < 15;i++)
      _NOP();
}

/******************延时1ms的时间*********************/
void Delay_1ms(void)
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
void Start(void)
{
        SCL_H;
        SDA_H;
        Delay();
    SDA_L;
        Delay();
    SCL_L;
        Delay();
}

/*******************************************
函数名称：Stop
功    能：完成IIC的终止条件操作
参    数：无
返回值  ：无
********************************************/
void Stop(void)
{
        SDA_L;
        Delay();
        SCL_H;
        Delay();
    SDA_H;
        Delay();
}


/*******************************************
函数名称：ACK
功    能：完成IIC的主机应答操作
参    数：无
返回值  ：无
********************************************/
void ACK(void)
{
        SDA_L;
        _NOP(); _NOP();
        SCL_H;
        Delay();
        SCL_L;
    _NOP(); _NOP();
    SDA_H;
        Delay();
}

/*******************************************
函数名称：NACK
功    能：完成IIC的主机无应答操作
参    数：无
返回值  ：无
********************************************/
void NACK(void)
{
    SDA_H;
    _NOP(); _NOP();
    SCL_H;
    Delay();
    SCL_L;
    _NOP(); _NOP();
    SDA_L;
    Delay();
}

/**********检查应答信号函数******************/
/*如果返回值为1则证明有应答信号，反之没有*/
/*******************************************
函数名称：Check
功    能：检查从机的应答操作
参    数：无
返回值  ：从机是否有应答：1--有，0--无
********************************************/
unsigned char Check(void)
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
    Delay();
    SDA_DDR_OUT;
    if(slaveack)    return FALSE;
    else            return TRUE;
}

/***************Write a Byte****************/
void Write_1_Byte(unsigned char DataByte)
{
    int i;

    for(i=0;i<8;i++)
    {
        if(DataByte&0x80)  //if((DataByte<<i)&0x80)
          SDA_H;
        else
          SDA_L;
        Delay();
    SCL_H;
        Delay();
        SCL_L;
        Delay();
        DataByte <<= 1;
    }
        SDA_H;
        _NOP();
}

/***************Write N Byte****************/
unsigned char Write_N_Byte(unsigned char *writebuffer,unsigned char n)
{
    int i;

    for(i=0;i<n;i++)
    {
        Write_1_Byte(*writebuffer);
        if(Check())
        {
            writebuffer ++;
        }
        else
        {
            Stop();
            return FALSE;
        }
    }
    Stop();
    return TRUE;
}

/***************Read a Byte****************/
unsigned char Read_1_Byte(void)
{
    unsigned char data=0,FLAG, i;

    for(i=0;i<8;i++)
    {

    SDA_H;
    Delay();
        SCL_H;
    SDA_DDR_IN; //端口为输入
    Delay();
        FLAG=SDA_IN& SDA;
        data <<= 1;
    if( FLAG)
       data |= 0x01;
    SDA_DDR_OUT;
        SCL_L;
        Delay();
    }
    return data;
}

/***************Read N Byte****************/
void Read_N_Byte(unsigned int*readbuff,unsigned char n)
{
    unsigned char i;

    for(i=0;i<n;i++)
    {
        readbuff[i]=Read_1_Byte();
        if(i==n-1)
            NACK();  //不连续读字节
        else
            ACK(); //连续读字节
    }
    Stop();

}


/*****************初始化******************/
void InitADS1115(void)
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
void WriteWord(void)
{
     int t;
     Start();   //写入4个字节
     do
     {
          t=Write_N_Byte(Writebuff,4);
     }while(t==0);
}

/***************Read Word***********************/
void ReadWord(void)
{
    int t;
    Start();  //写入2个字节
    do
    {
      t=Write_N_Byte(Readbuff,2);
    }while(t==0);

    Start();   //写入2个字节
    do
    {
      t=Write_N_Byte(&Readbuff[2],1);
    }while(t==0);

    Read_N_Byte(Result,2);  //读出2个字节

}

/***************ADS1115********************/
float ADS1115(void)
{
    float result = 0;
    InitADS1115();
    WriteWord();
    Delay_1ms();
    ReadWord();

    D_ADS=Result[0]*256+Result[1];  //转换的数字量
    t_DAS=0;
    ShowInt(D_ADS);    //转换的数字量转换成十进制
    VIN_DAS=6.144*D_ADS/32768;  //输入的电压值

    return result;
}

//用递归方法将正整数x转换为10进制格式
void ShowInt(long int x)
{
    if(x)
    {
          Format[t_DAS]= x%10;
          t_DAS  ++;
          ShowInt(x/10);
    }
}





