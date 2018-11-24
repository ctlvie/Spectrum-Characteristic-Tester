/*=======================================================
Author              :               ctlvie
Email Address       :               ctlvie@gmail.com
Filename            :               main.c
Date                :               2018-11-14
Description         :               

Modification History:
Date        By          Version     Description
----------------------------------------------------------
181114      ctlvie      1.0         Initial Version
========================================================*/

#include<msp430f5529.h>
#include<stdio.h>
#include"FormatConvert.h"
#include"AD9854.h"
#include"Keyboard.h"
#include"LCD12864.h"
#include"ADS1115.h"
#include "Button.h"
#define uint  unsigned int
#define uchar unsigned char
#define ulong unsigned long
//ulong  Freq_mult_ulong;
//double Freq_mult_doulle;
void DelayXms(unsigned int i);
//extern uchar FreqWord[6];

uchar virKey = 99;
  uchar arrayKey[40];
  int arrayNum = 0;

extern uchar LCD_ChineseBuff[];
extern uchar LCD_GraphBuff[];
float ADCResult = 0;
int t = 0;
volatile int Button_S1;
volatile int Button_S2;


void testADC(void)
{
  unsigned char TEST_ADCValue[8];
  WDTCTL = WDTPW + WDTHOLD; //关闭看门狗
  initLCD();
  LCD_clearCommand();
  initADC(0);
  while(1)
  { 
    ADCResult = getADCValue();
    convertFloattoCharArray(TEST_ADCValue,8,ADCResult,5);
    LCD_disString(1,1,TEST_ADCValue);
    DELAY_LCD_MS(100);
  }
}

void testLCD(void)
{
  initLCD();
  while(1)
	{
      //LCD_drawLine(5,60,120,5,1);
      //LCD_disGraph();
      LCD_disString(1,1,"HELLO WORLD!");
  }
}

void testDDS(void)
{
  initAD9854();
  setSinOutput(200,4090);
}


void testKeyboard(void)
{
  while(1) {
    arrayKey[arrayNum] = getKeyValue();
    arrayNum ++ ;
  }
}

int testButton1 = 0;
int testButton2 = 0;
void testButton(void)
{
  initLCD();
  initButtons();
  P1DIR |= BIT0;
  while(1)
  {
    if(Button_S1)
    {
      Button_S1 = 0;
      P1OUT ^= BIT0;
      testButton1 ++;
      LCD_disString(testButton1,1,"A");
    }
    if(Button_S2)
    {
      Button_S2 = 0;
      P1OUT ^= BIT0;
      testButton2 ++;
      LCD_disString(testButton2,2,"B");
    }

  }

}

void main( void )
{
	
 	WDTCTL = WDTPW + WDTHOLD; //关闭看门狗
  testButton();
}
/*
int testButton1 = 0;
int testButton2 = 0;

#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void)
{
  stateOfS1 = 1;
  P1IFG &= ~BIT1;
  testButton1 ++;
}

#pragma vector = PORT2_VECTOR
__interrupt void Port_2(void)
{
  stateOfS2 = 1;
  P2IFG &= ~BIT1;
  testButton2 ++;
}
*/



/* AD9854测试用main函数
int main( void )
{

   double f = 1000000.0;
   unsigned int i;
   // Stop watchdog timer to prevent time out reset
   WDTCTL = WDTPW + WDTHOLD;

   AD9854_Init();            //波形发生器初始化

   while(1){
     for(i=1;i<60;i++){

        AD9854_SetSine(f*i,4000); //产生xxMHz正弦信号
        DelayXms(8000);
     }
  }
  return 0;
}
void DelayXms(unsigned int i){
    unsigned int j;
    for( ; i>0; i--){
      for(j=0;j<200;j++);
    }

}
*/
