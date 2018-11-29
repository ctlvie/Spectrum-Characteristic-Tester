/*=======================================================
Author				  :				ctlvie
Email Address		  :				ctlvie@gmail.com
Filename			  :				ProcessModule.c
Date				  :			    2018-11-26
Description			  :				核心处理运算函数

Modification History:
Date		By			Version		Description
----------------------------------------------------------
181126		ctlvie		1.0			核心处理运算函数
========================================================*/
#include<msp430f5529.h>
#include"ProcessModule.h"
#include"FormatConvert.h"
#include"AD9854.h"
#include"Keyboard.h"
#include"LCD12864.h"
#include"ADS1115.h"
#include "Button.h"
#include "Board.h"
#include <stdlib.h>
#include <math.h>

extern int  inputNum(void);

extern unsigned long testCurrFreq;
extern volatile int Button_S1;
extern volatile int Button_S2;

extern float ScanResult_I[SCAN_SIZE];
extern float ScanResult_Q[SCAN_SIZE];
extern float ScanAmpResult[SCAN_SIZE];
extern float ScanAmpResult_dB[SCAN_SIZE];
extern float ScanPhaseResult[SCAN_SIZE];
extern float PointResult_I;
extern float PointResult_Q;
extern float PointAmpResult;
extern float PointPhaseResult;
extern float x_Scale;
extern float y_Scale;
extern float test;

float getCorrectValue(float input)
{
    float t;
    t = (input - 2) / 3;
    return t;
    //return input;
}

void ScanFreq(void)
{
    unsigned long currFreq = 1000;
    unsigned long stepFreq = 0;
    unsigned char currPercent_char[2] = {'0','0'};
    unsigned int currSchedule = 0;
    LCD_clearScreen();
    LCD_disString(1,1,"Scaning...");
    stepFreq = STEP_FREQ;
    while(currFreq < 1000000)
    {
        setSinOutput(currFreq,4090);
        DELAY_PROCESS_MS(1);
        testCurrFreq = currFreq;
        initADC(0);
        ScanResult_I[currSchedule] = getCorrectValue(getADCValue());
        initADC(1);
        ScanResult_Q[currSchedule] = getCorrectValue(getADCValue());

        convertInttoCharArray(currPercent_char,currSchedule,2);
        LCD_disString(3,3,currPercent_char);
        LCD_disString(4,3," %");
        /*
        if((currFreq % 10000) == 0)
        {
            currPercent_int = currFreq / 10000;
            convertInttoCharArray(currPercent_char,currPercent_int,2);
            LCD_disString(3,3,currPercent_char);
            LCD_disString(4,3," %");
        }*/
        currFreq += stepFreq;
        currSchedule++;
    }
    LCD_clearScreen();
    LCD_disString(1,2,"Scan Finish!");
}

extern unsigned long test1;
void PointFreq(void)
{
    int inputFreqValue = 0;
    int isOutOfRange = 1;
    do{
        inputFreqValue = inputNum();
        if(inputFreqValue > 1000)
        {
            LCD_clearScreen();
            LCD_disString(1,2,"Out of range!");
            DELAY_PROCESS_MS(300);
            isOutOfRange = 1;
        }
        else
            isOutOfRange = 0;
    }while(isOutOfRange);
    unsigned long tempFreq = (unsigned long)inputFreqValue * 1000 ;
    setSinOutput(tempFreq, 4090);
    DELAY_PROCESS_MS(10);
    initADC(0);
    PointResult_I = getADCValue();
    initADC(1);
    PointResult_Q = getADCValue();
}

void Calculate_Amp(void)
{
    float currI, currQ;
    float temp = 0;
    int i;
    LCD_clearScreen();
    LCD_disString(1,2,"Calculating...");
    for(i = 0; i < SCAN_SIZE; i++)
    {
        currI = ScanResult_I[i];
        currQ = ScanResult_Q[i];
        temp = (currI * currI) + (currQ * currQ);
        ScanAmpResult[i] = 2 * SqrtByNewton(temp);
    }
}

void Calculate_Phase(void)
{
    float currI, currQ;
    float temp = 0;
    int i;
    LCD_clearScreen();
    LCD_disString(1,2,"Calculating...");
    for(i = 0; i < SCAN_SIZE; i++)
    {
        currI = ScanResult_I[i];
        currQ = ScanResult_Q[i];
        temp = currQ / currI;
        ScanPhaseResult[i] = -1 * Arctan(temp);
    }
}

void Calculate_PointFreq(void)
{
    float currI, currQ;
    float temp = 0;
    unsigned char AmpValue[4];
    unsigned char PhaseValue[4];
    LCD_clearScreen();
    LCD_disString(1,2,"Calculating...");
    currI = PointResult_I;
    currQ = PointResult_Q;
    temp = (currI * currI) + (currQ * currQ);
    PointAmpResult = 2 * SqrtByNewton(temp);
    temp = 0;
    temp = currQ / currI;
    PointPhaseResult = -1 * Arctan(temp);
    convertFloattoCharArray(AmpValue,4,PointAmpResult,4);
    convertFloattoCharArray(PhaseValue,4,PointPhaseResult,4);
    LCD_clearScreen();
    LCD_disString(1,2,AmpValue);
    LCD_disString(3,2,"V");
    LCD_disString(1,3,PhaseValue);
    LCD_disString(3,3,"°");
}

int convertCord_Y(int inputY)
{
    return Y_DEST - inputY;
}

void drawAmpCordinate(void)
{
    LCD_clearBuff();
    LCD_drawLine_X(X_START,X_DEST,Y_DEST,1);
    LCD_drawLine_Y(X_START,Y_START,Y_DEST,1);

    //X axis
    LCD_drawPoints(X_DEST,Y_DEST,1);
    LCD_drawPoints(X_DEST - 1,Y_DEST + 1,1);
    LCD_drawPoints(X_DEST - 1,Y_DEST - 1,1);
    LCD_drawPoints(X_DEST - 2,Y_DEST + 2,1);
    LCD_drawPoints(X_DEST - 2,Y_DEST - 2,1);

    //Y axis
    LCD_drawPoints(X_START - 1,Y_START + 1,1);
    LCD_drawPoints(X_START + 1,Y_START + 1,1);
    LCD_drawPoints(X_START - 2,Y_START + 2,1);
    LCD_drawPoints(X_START + 2,Y_START + 2,1);

    //scales
    int i = 0;
    for(i = X_START; i <= X_DEST; )
    {
        LCD_drawPoints(i,Y_DEST - 1,1);
        i += 5;
    }
    for(i = Y_START; i <= Y_DEST;)
    {
        LCD_drawPoints(X_START + 1,i,1);
        i += 5;
    }
}

void drawPhaseCordinate(void)
{
    LCD_clearBuff();
    LCD_drawLine_X(X_START,X_DEST,Y_MIDDLE,1);
    LCD_drawLine_Y(X_START,Y_START,Y_DEST,1);

    test = Y_MIDDLE;
    //X axis
    LCD_drawPoints(X_DEST,Y_MIDDLE,1);
    LCD_drawPoints(X_DEST - 1,Y_MIDDLE + 1,1);
    LCD_drawPoints(X_DEST - 1,Y_MIDDLE - 1,1);
    LCD_drawPoints(X_DEST - 2,Y_MIDDLE + 2,1);
    LCD_drawPoints(X_DEST - 2,Y_MIDDLE - 2,1);

    //Y axis
    LCD_drawPoints(X_START - 1,Y_START + 1,1);
    LCD_drawPoints(X_START + 1,Y_START + 1,1);
    LCD_drawPoints(X_START - 2,Y_START + 2,1);
    LCD_drawPoints(X_START + 2,Y_START + 2,1);

    //scales
    int i = 0;
    for(i = X_START; i <= X_DEST; )
    {
        LCD_drawPoints(i,Y_MIDDLE - 1,1);
        i += 5;
    }
    for(i = Y_START; i <= Y_DEST;)
    {
        LCD_drawPoints(X_START + 1,i,1);
        i += 5;
    }
}

void drawAmpCurve_Linear(void)
{
    LCD_clearBuff();
    LCD_clearScreen();
    LCD_disString(1,2,"Drawing...");
    drawAmpCordinate();
    //find the maximum number of the results
    int i = 0;
    float currMax = ScanAmpResult[0];
    //int intTempResult = 0;
    float y_Scale_Single = 0;
    int x_GraphPos1 = 0;
    int y_GraphPos1 = 0;
    int x_GraphPos2;
    int y_GraphPos2;

    for(i = 0; i < SCAN_SIZE ; i++)
    {
        if(ScanAmpResult[i] > currMax)
            currMax = ScanAmpResult[i];
    }
    //intTempResult = (int)currMax;
    //currMax = (float)intTempResult + 1;
    y_Scale_Single = currMax / 50 ;
    y_Scale = y_Scale_Single * 5;
    x_Scale = 5;

    i = 0;

    for(i = 0 ; i < SCAN_SIZE ; i++)
    {
        x_GraphPos2 = i;
        y_GraphPos2 = convertCord_Y((int)(ScanAmpResult[i] / y_Scale_Single));
        LCD_drawLine(x_GraphPos1 + X_START, y_GraphPos1, x_GraphPos2 + X_START, y_GraphPos2, 1);
        x_GraphPos1 = x_GraphPos2;
        y_GraphPos1 = y_GraphPos2;
    }
    LCD_clearScreen();
    LCD_disGraph();

}



void drawAmpCurve_dB(void)
{
    LCD_clearBuff();
    LCD_disString(1,2,"Drawing...");
    drawAmpCordinate();
    int i;
    for(i = 0; i < SCAN_SIZE ; i++)
        ScanAmpResult_dB[i] = 20 * (float)log10((double)ScanAmpResult[i]);
    float currMax = ScanAmpResult_dB[0];
    int intTempResult = 0;
    float y_Scale_Single = 0;
    int x_GraphPos1 = 0;
    int y_GraphPos1 = 0;
    int x_GraphPos2;
    int y_GraphPos2;

    for(i = 0; i < SCAN_SIZE ; i++)
    {
        if(ScanAmpResult_dB[i] > currMax)
            currMax = ScanAmpResult_dB[i];
    }
    intTempResult = (int)currMax;
    currMax = (float)intTempResult + 1;
    y_Scale_Single = currMax / 60 ;
    y_Scale = y_Scale_Single * 5;
    x_Scale = 5;

    i = 0;

    for(i = 0 ; i < SCAN_SIZE ; i++)
    {
        x_GraphPos2 = i;
        y_GraphPos2 = convertCord_Y((int)(ScanAmpResult_dB[i] / y_Scale_Single));
        LCD_drawLine(x_GraphPos1 + X_START, y_GraphPos1, x_GraphPos2 + X_START, y_GraphPos2, 1);
        x_GraphPos1 = x_GraphPos2;
        y_GraphPos1 = y_GraphPos2;
    }
    LCD_clearScreen();
    LCD_disGraph();
    
}

void drawPhaseCurve()
{
    LCD_clearBuff();
    LCD_disString(1,2,"Drawing...");
    int i = 0;
    float y_Scale_Single = 0;
    x_Scale = 5;
    y_Scale_Single = 360 / Y_LENGTH;
    y_Scale = 5 * y_Scale_Single;
    int x_GraphPos1 = 0;
    int y_GraphPos1 = Y_MIDDLE;
    int x_GraphPos2;
    int y_GraphPos2;

    for(i = 0; i < SCAN_SIZE; i++)
    {
        x_GraphPos2 = i;
        y_GraphPos2 = convertCord_Y((int)((ScanPhaseResult[i] + 180) / y_Scale_Single));
        LCD_drawLine(x_GraphPos1 + X_START, y_GraphPos1, x_GraphPos2 + X_START, y_GraphPos2, 1);
        x_GraphPos1 = x_GraphPos2;
        y_GraphPos1 = y_GraphPos2;
    }
    drawPhaseCordinate();
    LCD_clearScreen();
    LCD_disGraph();
}

void showMoreInfo(int mode)
{
    LCD_BacktoStrMode();
    unsigned char x_Value[5];
    unsigned char y_Value[5];
    convertFloattoCharArray(x_Value,5,50.0,3);
    convertFloattoCharArray(y_Value,5,y_Scale,3);
    LCD_disString(0,1,"X:");
    LCD_disString(0,2,"Y:");
    LCD_disString(1,1,x_Value);
    LCD_disString(1,2,y_Value);
    if(mode == MODE_AMP_LN)
    {
        LCD_disString(4,1,"kHz/div");
        LCD_disString(4,2,"V/div");
    }
    else if(mode == MODE_AMP_DB)
    {
        LCD_disString(4,1,"kHz/div");
        LCD_disString(4,2,"dB/div");
    }
    else
    {
        LCD_disString(4,1,"kHz/div");
        LCD_disString(4,2,"°/divx");
    }
}

void showCurve(int mode)
{
    if(mode == MODE_AMP_LN)
        drawAmpCurve_Linear();
    else if(mode == MODE_AMP_DB)
        drawAmpCurve_dB();
    else 
        drawPhaseCurve();
disCurve:   LCD_clearScreen();
    LCD_disGraph();
    while(1)
    {
        if(Button_S2)
        {
            Button_S2 = 0;
            showMoreInfo(mode);
            while(1)
            {
                if(Button_S2)
                {
                    Button_S2 = 0;
                    goto disCurve;
                }
            }
        }
        if(Button_S1)
        {
            Button_S1 = 0;
            break ;
        }
    }
}

void ScanOutput(void)
{
    unsigned long currFreq = 1000;
    unsigned long stepFreq = 0;
    unsigned char displayCurrFreq_char[7] = {'0','0','0','0','0','0','0'};
    unsigned int displayCurrFreq = 0;
    LCD_clearScreen();
    LCD_disString(0,2,"Selcect Scan Step:");
    LCD_disString(0,3,"1. 10Hz");
    LCD_disString(0,4,"2. 1kHz");
    int isSelected = 0;
    do{
        if(Button_S1)
        {
            Button_S1 = 0;
            stepFreq = 10;
            isSelected = 1;
        }
        else if(Button_S2)
        {
            Button_S2 = 0;
            stepFreq = 1000;
            isSelected = 1;
        }
        else
            isSelected = 0;
    }while(!isSelected);
    isSelected = 0;
    LCD_clearScreen();
    LCD_disString(1,2,"Scaning ...");
    while(currFreq < 1000000)
    {
        setSinOutput(currFreq,4090);
        DELAY_PROCESS_MS(1);
        displayCurrFreq = (int)currFreq;
        convertInttoCharArray(displayCurrFreq_char,displayCurrFreq,7);
        LCD_disString(0,3,displayCurrFreq_char);
        LCD_disString(7,3,"Hz");
        currFreq += stepFreq;
    }
    LCD_clearScreen();
    LCD_disString(1,2,"Scan Finish!");
    DELAY_PROCESS_MS(10);
}

