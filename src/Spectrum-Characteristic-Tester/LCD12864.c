/*=======================================================
Author				  :				ctlvie
Email Address		  :				ctlvie@gmail.com
Filename			  :				LCD12864.c
Date				  :			    2018-11-19
Description			  :				12864液晶屏驱动

Modification History:
Date		By			Version		Description
----------------------------------------------------------
181119		ctlvie		1.0			字符、汉字显示和清屏函数
========================================================*/
#include<msp430f5529.h>
#include"LCD12864.h"
 
uchar CorpInf[]=
{
	"频谱特性测试"
	"Tester"
	""
	"123.58Mhz"
};

void DelayUs2x(unsigned char t)
{   
 while(--t);
}
 
void DELAY_LCD_MS(unsigned char t)
{
     
 while(t--)
 {
     //大致延时1mS
     DelayUs2x(245);
	 DelayUs2x(245);
 }
}
 
void SendByte(unsigned char zdata)
{
	unsigned int i;
	
	for(i=0; i<8; i++)
	{
		if((zdata << i) & 0x80)
		{
			SID1;
		}
		else
		{
			SID0;
		}
		SCLK0;
		SCLK1;
	}
}
 
void WriteCommand(unsigned char cmdcode)
{
	CS1;
	SendByte(0xf8);   //1 1 1 1 RS RW 0   写操作RW=0 11111000写数据 11111010写指令
	SendByte(cmdcode & 0xf0);
	SendByte((cmdcode << 4) & 0xf0);
	DELAY_LCD_MS(1);
	CS0;
}
 
void WriteData(unsigned char Dispdata)
{
	CS1;
	SendByte(0xfa);
	SendByte(Dispdata & 0xf0);
	SendByte((Dispdata << 4) & 0xf0);
	DELAY_LCD_MS(1);
	CS0;
}
 
void initLCD()
{
    IO_LCD_DIR = 0xFF;
    IO_LCD_OUT = 0x00;
	DELAY_LCD_MS(200);
	WriteCommand(0x30);  //功能设定：基本指令集
	DELAY_LCD_MS(20);
	WriteCommand(0x0c);  //显示状态：整体显示，游标关
	DELAY_LCD_MS(20);
	WriteCommand(0x01);  //清空显示
	DELAY_LCD_MS(200);
}
 
 void LCS_clearAll(void)
 {
     WriteCommand(0x01);
     DELAY_LCD_MS(100);
 }
  
void LCD_disString(unsigned int x,unsigned int y,unsigned char* s)
{
	 switch(y)
     {
	  case 1: WriteCommand(0x80+x);break;
	  case 2: WriteCommand(0x90+x);break;
	  case 3: WriteCommand(0x88+x);break;
	  case 4: WriteCommand(0x98+x);break;
      default:break;
	 }
    while(*s>0)
   { 
      WriteData(*s);
      s++;
      DelayUs2x(50);
   }
}

 void LCD_disGBStr(uchar *CorpInf)
{
	uchar uc_GBCnt;

	WriteCommand(0x30); 	//DL=1:8-BIT interface
	WriteCommand(0x30); 	//RE=0:basic instruction
	WriteCommand(0x0C); 	//Display OFF,Cursor OFF,Cursor position blink OFF

	WriteCommand(0x80);
	for (uc_GBCnt=0;uc_GBCnt<16;uc_GBCnt++)
	{
		WriteData(CorpInf[2 * uc_GBCnt]);
		WriteData(CorpInf[2 * uc_GBCnt + 1]);
	};

	WriteCommand(0x90);
	for (uc_GBCnt=0;uc_GBCnt<16;uc_GBCnt++)
	{
		WriteData(CorpInf[2 * uc_GBCnt + 32]);
		WriteData(CorpInf[2 * uc_GBCnt + 33]);
	};

	DELAY_LCD_MS(20);
}
