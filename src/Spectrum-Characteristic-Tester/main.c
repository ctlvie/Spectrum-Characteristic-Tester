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
181216      ctlvie      2.0         Final Version
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
#include"GlobalTimer.h"
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
volatile int Button_S3;
volatile int Button_S4;

volatile float cutOffFreq1;
volatile float cutOffFreq2;


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
      testButton1 ++;
      LCD_disString(testButton1,1,"A");
    }
    if(Button_S2)
    {
      Button_S2 = 0;
      testButton2 ++;
      LCD_disString(testButton2,2,"B");
    }
    if(Button_S3)
    {
      Button_S3 = 0;
      testButton2 ++;
      LCD_disString(testButton2,3,"C");
    }

  }

}

int testInputResult = 0;
uchar arrayBuff[4] = {'0','0','0','0'};
int  inputNum(unsigned int x, unsigned int y, unsigned char *s)
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
    LCD_disString(x,y,s);
    LCD_disChar(1,3,arrayBuff[0]);
    LCD_disChar(2,3,arrayBuff[1]);
    LCD_disChar(3,3,arrayBuff[2]);
    LCD_disChar(4,3,arrayBuff[3]);
    LCD_disString(5,3,"kHz");
    for(currInputBits = 0; currInputBits <= 3; currInputBits ++)
    {
        LCD_CursorON(currInputBits + 1, 3);
        arrayBuff[currInputBits] = getKeyValue();
        LCD_disChar(1,3,arrayBuff[0]);
        LCD_disChar(2,3,arrayBuff[1]);
        LCD_disChar(3,3,arrayBuff[2]);
        LCD_disChar(4,3,arrayBuff[3]);
    }
    LCD_CursorOFF();
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
  testInputResult = inputNum(1,1,"Input:");
  LCD_clearScreen();
  convertInttoCharArray(testInputBuff,testInputResult,4);
  LCD_disString(1,1,arrayBuff);
}

unsigned long testCurrFreq;

float SweepResult_I[SCAN_SIZE];
float SweepResult_Q[SCAN_SIZE];
float SweepAmpResult[SCAN_SIZE];
float SweepAmpResult_dB[SCAN_SIZE];
float SweepPhaseResult[SCAN_SIZE];
float DotResult_I;
float DotResult_Q;
float DotAmpResult;
float DotPhaseResult;
volatile float x_Scale;
volatile float y_Scale;


void testSweepFreq(void)
{
  initAD9854();
  initADC(0);
  initLCD();
  initButtons();
  SweepFreq();
  Calculate_Amp();
  Calculate_Phase();
  showPhaseCurve(100);
}

void testSweepForever(void)
{
  initAD9854();
  initADC(0);
  initLCD();
  initButtons();
  unsigned char TEST_ADCValue0[8];
  unsigned char TEST_ADCValue1[8];
  unsigned long currFreq = 1000;
  unsigned long stepFreq = 0;
  unsigned char currPercent_char[2] = {'0','0'};
  unsigned int currSchedule = 0;
  while(1)
  {
    currFreq = 1000;
    stepFreq = STEP_FREQ;
    currSchedule = 0;
    while(currFreq < 1000000)
    {
        setSinOutput(currFreq,4090);
        DELAY_PROCESS_MS(1);
        testCurrFreq = currFreq;
        initADC(0);
        SweepResult_I[currSchedule] = getCorrectValue(getADCValue());
        ADCResult0 = getADCValue();
        convertFloattoCharArray(TEST_ADCValue0,8,ADCResult0,5);
        LCD_disString(1,1,TEST_ADCValue0);
        initADC(1);
        SweepResult_Q[currSchedule] = getCorrectValue(getADCValue());
        ADCResult1 = getADCValue();
        convertFloattoCharArray(TEST_ADCValue1,8,ADCResult1,5);
        LCD_disString(1,2,TEST_ADCValue1);
        convertInttoCharArray(currPercent_char,currSchedule,2);
        LCD_disString(3,3,currPercent_char);
        LCD_disString(4,3," %");
        currFreq += stepFreq;
        currSchedule++;
        if(Button_S3)
        {
          break ; 
        }
    }
    currFreq = 1000;
    if(Button_S3)
    {
      Button_S3 = 0;
      break;
    }
  }
}

void testDotFreq(void)
{
  initAD9854();
  initADC(0);
  initLCD();
  initButtons();
  DotFreq();
  Calculate_DotFreq();
}

void testSpecificFreq(unsigned long specificFreq)
{
  initAD9854();
  initADC(0);
  initLCD();
  unsigned char TEST_ADCValue0[8];
  unsigned char TEST_ADCValue1[8];
  while(1)
  {
      setSinOutput(specificFreq,4090);
      initADC(0);
      ADCResult0 = getADCValue();
      convertFloattoCharArray(TEST_ADCValue0,8,ADCResult0,5);
      LCD_disString(1,1,TEST_ADCValue0);
      initADC(1);
      ADCResult1 = getADCValue();
      convertFloattoCharArray(TEST_ADCValue1,8,ADCResult1,5);
      LCD_disString(1,2,TEST_ADCValue1);
  }

}

void testCutOffFreq(void)
{
  initAD9854();
  initADC(0);
  initLCD();
  initButtons();
  SweepFreq();
  Calculate_Amp();
  Calculate_Phase();
  Calculate_CutOffFreq();
  unsigned char Fc_1[10], Fc_2[10];
  convertFloattoCharArray(Fc_1,(long)10,cutOffFreq1,(long)7);
  convertFloattoCharArray(Fc_2,(long)10,cutOffFreq2,(long)3);
  LCD_clearScreen();
  LCD_disString(1,2,Fc_1);
  LCD_disString(1,3,Fc_2);
}


volatile int test = 0;
unsigned long test1 = 0;
unsigned int globalTime_min = 0;
unsigned int globalTime_sec = 0;
unsigned int globalTime_ms = 0;
unsigned char TimerBuff[9] = {'0','0',':','0','0','.','0','0','\0'};


unsigned long testFitting_Freq = 0;
void testFitting(void)
{
  unsigned long disCurrFreq;
  unsigned char disCurrFreq_char[7] = {'0','0','0','0','0','0','0'};
  initLCD();
  initLCD();
  initButtons();
  initBoard();
  initAD9854();
  while(1)
  {
  if(Button_S1)
    {
      Button_S1 = 0;
      testFitting_Freq += 5000;
      setSinOutput(testFitting_Freq, 4090);
      disCurrFreq = (float)testFitting_Freq;
      convertFloattoCharArray(disCurrFreq_char,7,disCurrFreq,0);
      DELAY_PROCESS_MS(10);
      initADC(0);
      DotResult_I = getCorrectValue(getADCValue());
      initADC(1);
      DotResult_Q = getCorrectValue(getADCValue());
      Calculate_DotFreq();
      LCD_disString(2,1,disCurrFreq_char);
      LCD_disString(5,1,"Hz");
    }
  }
}

void testTimer(void)
{
    initLCD();
    initGlobalTimer();
    startTimer();
    while(1)
    {
      LCD_disString(0,3,TimerBuff);
    }
}



unsigned int isExittoMenu = 0;
void main(void)
{
	
 	WDTCTL = WDTPW + WDTHOLD; //关闭看门狗
  initLCD();
  initButtons();
  initBoard();
  initAD9854();
  initGlobalTimer();

start: LCD_BacktoStrMode();
  LCD_clearScreen();
  int isSelected = 0;
  unsigned int ScanSize = 100;
  //LCD_BacktoStrMode();
  LCD_disString(0,2,"1.Generation");
  LCD_disString(0,3,"2.Measurement"); 
  while(1)
  {
    if(Button_S1)
    {
      Button_S1 = 0;
      LCD_clearScreen();
      while(1)
      {
        LCD_disString(1,2,"1.Sweep Output");
        LCD_disString(1,3,"2.Dot Output");
        if(Button_S1)
        {
          Button_S1 = 0;
          SweepOutput();
        }
        if(Button_S2)
        {
          Button_S2 = 0;
          DotOutput();
        }
         if(Button_S4)
        {
          Button_S4 = 0;
          DELAY_PROCESS_MS(10);
          goto start;
        } 
      }
    }
    if(Button_S2)
    {
      Button_S2 = 0;
      LCD_clearScreen();
      LCD_disString(1,1,"Spectrum: ");
      LCD_disString(1,2,"1.Sweep");
      LCD_disString(1,3,"2.Dot");
      LCD_disString(1,4,"3.Costom Sweep");
      while(1)
      {
        if(Button_S1 == 1 || Button_S3 == 1)
        {
          LCD_clearScreen();
          if(Button_S1)
          {
            Button_S1 = 0;
            ScanSize = 100;
            x_Scale = 50;
            SweepFreq();
          }
          else
          {
            Button_S3 = 0;
            x_Scale = 0;
            ScanSize = CustomSweep();
          }
          Calculate_Amp();
          Calculate_Phase();
          while(1)
          {
            LCD_BacktoStrMode();
            LCD_clearScreen();
            LCD_disString(1,2,"1.Amp Curve");
            LCD_disString(1,3,"2.Phase Curve");
            do{
              if(Button_S1)
              {
                Button_S1 = 0;
                LCD_clearScreen();
                LCD_disString(1,2,"1.dB");
                LCD_disString(1,3,"2.Linear");
                do{
                  if(Button_S1)
                  {
                    isSelected = MODE_AMP_DB;
                    Button_S1 = 0;
                  }
                  else if(Button_S2)
                  {
                    isSelected = MODE_AMP_LN;
                    Button_S2 = 0;
                  }
                  else
                    isSelected = 0;
                }while(!isSelected);
              }
              else if (Button_S2)
              {
                isSelected = MODE_PHASE;
                Button_S2 = 0;
              }
              else if(Button_S4)
              {
                Button_S4 = 0;
                goto start;
              }
              else
                isSelected = 0;
            }while(!isSelected);
            showCurve(isSelected,ScanSize);
          }
        }
        if(Button_S2)
        {
          Button_S2 = 0;
          LCD_clearScreen();
          LCD_disString(1,2,"Dot Method");
          DELAY_PROCESS_MS(100);
          DotFreq();
          Calculate_DotFreq();
          while(1)
          {
            if(Button_S4)
            {
              Button_S4 = 0;
              goto start;
            }
          }
        }
      }
    }
    if(Button_S3)
    {
      Button_S3 = 0;
      testSweepForever();
      goto start;
    }
  } 
}

