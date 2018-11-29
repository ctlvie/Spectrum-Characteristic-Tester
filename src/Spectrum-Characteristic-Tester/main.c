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
#include "Board.h"
#include"ProcessModule.h"
#include<math.h>

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
float ADCResult0 = 0;
float ADCResult1 = 0;
int t = 0;
volatile int Button_S1;
volatile int Button_S2;


void testADC(void)
{
  unsigned char TEST_ADCValue0[8];
  unsigned char TEST_ADCValue1[8];
  WDTCTL = WDTPW + WDTHOLD; //关闭看门狗
  initLCD();
  LCD_clearScreen();
  while(1)
  { 
    initADC(0);
    ADCResult0 = getADCValue();
    convertFloattoCharArray(TEST_ADCValue0,8,ADCResult0,5);
    LCD_disString(1,1,TEST_ADCValue0);
    initADC(1);
    ADCResult1 = getADCValue();
    convertFloattoCharArray(TEST_ADCValue1,8,ADCResult1,5);
    LCD_disString(1,3,TEST_ADCValue1);
  }
}

void testLCD(void)
{
  initLCD();
  initButtons();
  while(1)
	{
      LCD_drawLine(5,60,120,5,1);
      LCD_disGraph();
      if(Button_S2)
      {
        Button_S2 = 0;
        LCD_BacktoStrMode();
        while(1)
        { 
          LCD_disString(1,1,"HELLO WORLD!");
        }
      }
  }
}

void testDDS(void)
{
  initAD9854();
  setSinOutput(1500,4090);
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

int testInputResult = 0;
uchar arrayBuff[4] = {'0','0','0','0'};

int  inputNum(void)
{
    uint currInputBits;
    int inputNumResult;
    int cleatVir = 0;
startInput:
    for(cleatVir = 0; cleatVir < 4; cleatVir ++)
        arrayBuff[cleatVir] = '0';
    currInputBits = 0;
    inputNumResult = 0;
    LCD_clearScreen();
    LCD_disString(1,1,"Input:");
    LCD_disString(1,3,arrayBuff); 
    for(currInputBits = 0; currInputBits <= 3; currInputBits ++)
    {
        arrayBuff[currInputBits] = getKeyValue();
        LCD_disString(1,3,arrayBuff);
    }
    inputNumResult = convertCharArraytoInt(arrayBuff, 4);
    LCD_disString(1,4,"S1:Clear S2:OK");
    while(1)
    {
      if(Button_S1)
      {
        Button_S1 = 0;
        goto startInput;
      }
      if(Button_S2)
      {
        Button_S2 = 0;
        LCD_clearScreen();
        LCD_disString(1,2,"Confirmed:");
        LCD_disString(1,3,arrayBuff);
        return inputNumResult;
      }
    }
    
}

void testInput(void)
{
  uchar testInputBuff[4];
  initLCD();
  initButtons();
  testInputResult = 0;
  testInputResult = inputNum();
  LCD_clearScreen();
  convertInttoCharArray(testInputBuff,testInputResult,4);
  LCD_disString(1,1,arrayBuff);
}

unsigned long testCurrFreq;

float ScanResult_I[SCAN_SIZE];
float ScanResult_Q[SCAN_SIZE];
float AmpResult[SCAN_SIZE];
float AmpResult_dB[SCAN_SIZE];
float PhaseResult[SCAN_SIZE];
float x_Scale;
float y_Scale;


void testScanFreq(void)
{
  initAD9854();
  initADC(0);
  initLCD();
  initButtons();
  ScanFreq();
  Calculate_Amp();
  Calculate_Phase();
  showCurve(MODE_AMP_LN);
}

void testPointFreq(void)
{
  initAD9854();
  initADC(0);
  initLCD();
  initButtons();
  PointFreq();
}

float test = 0;
unsigned long test1 = 0;
void main(void)
{
 	WDTCTL = WDTPW + WDTHOLD; //关闭看门狗
  testPointFreq();
  //testDDS();
}

/*
void main(void)
{
	
 	WDTCTL = WDTPW + WDTHOLD; //关闭看门狗
  initLCD();
  initButtons();
  initBoard();
  initAD9854();
  initADC();
  LCD_disString(1,1,"1.Amplitude");
  LCD_disString(1,2,"2.Phase");
  LED1_ON;
  LED2_ON;
  while(1)
  {
    if(Button_S1)
    {
      Button_S1 = 0;
      LED1_OFF;
      LCD_clearScreen();
      LCD_disString(1,1,"Amplitude Test");
      LCD_disString(1,2,"1.Scan");
      LCD_disString(1,3,"2.Point");
      while(1)
      {
        if(Button_S1)
        {
          Button_S1 = 0;
          LCD_clearScreen();
          LCD_disString(1,1,"Scan Method");
          LCD_disString(0,3,"Set Step Freq");
          LCD_disString(0,4,"S1:10Hz S2:1kHz");
          while(1)
          {
            if(Button_S1)
            {
              Button_S1 = 0;
              LCD_clearScreen();
              LCD_disString(1,2,"10Hz");
            }
            if(Button_S2)
            {
              Button_S2 = 0;
              LCD_clearScreen();
              LCD_disString(1,2,"1kHz");
            }
          }
        }
        if(Button_S2)
        {
          Button_S2 = 0;
          LCD_clearScreen();
          LCD_disString(1,2,"Point Method");
        }
      }
    }
    if(Button_S2)
    {
      Button_S2 = 0;
      LED2_OFF;
      LCD_clearScreen();
      LCD_disString(1,1,"Phase Test");
      LCD_disString(1,2,"1.Scan");
      LCD_disString(1,3,"2.Point");
      while(1)
      {
        if(Button_S1)
        {
          Button_S1 = 0;
          LCD_clearScreen();
          LCD_disString(1,1,"Scan Method");
          LCD_disString(0,3,"Set Step Freq");
          LCD_disString(0,4,"S1:10Hz S2:1kHz");
          while(1)
          {
            if(Button_S1)
            {
              Button_S1 = 0;
              LCD_clearScreen();
              LCD_disString(1,2,"10Hz");
            }
            if(Button_S2)
            {
              Button_S2 = 0;
              LCD_clearScreen();
              LCD_disString(1,2,"1kHz");
            }
          }
        }
        if(Button_S2)
        {
          Button_S2 = 0;
          LCD_clearScreen();
          LCD_disString(1,2,"Point Method");
        }
      }
    }
  } 
}
*/

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
