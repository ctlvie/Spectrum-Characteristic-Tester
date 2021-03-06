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
181216      ctlvie      2.0         Final Version
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
#include "GlobalTimer.h"
#include <stdlib.h>
#include <math.h>

extern int  inputNum(unsigned int x, unsigned int y, unsigned char *s);

extern unsigned long testCurrFreq;
extern volatile int Button_S1;
extern volatile int Button_S2;
extern volatile int Button_S3;
extern volatile int Button_S4;
extern volatile float cutOffFreq1;
extern volatile float cutOffFreq2;
extern float SweepResult_I[SCAN_SIZE];
extern float SweepResult_Q[SCAN_SIZE];
extern float SweepAmpResult[SCAN_SIZE];
extern float SweepAmpResult_dB[SCAN_SIZE];
extern float SweepPhaseResult[SCAN_SIZE];
extern float DotResult_I;
extern float DotResult_Q;
extern float DotAmpResult;
extern float DotPhaseResult;
extern volatile float x_Scale;
extern volatile float y_Scale;
extern volatile float test;
extern unsigned char TimerBuff[8];

float getCorrectValue(float input)
{
    float t;
    t = (input - 1.48)/2;
    return t;
    //return input;
}

void SweepFreq(void)
{
    unsigned long currFreq = 1000;
    unsigned long stepFreq = 0;
    unsigned char currPercent_char[2] = {'0','0'};
    unsigned int currSchedule = 0;
    LCD_clearScreen();
    LCD_disString(1,1,"Scaning...");
    stepFreq = STEP_FREQ;
    startTimer();
    while(currFreq <= 1000000)
    {
        setSinOutput(currFreq,4090);
        DELAY_PROCESS_MS(SWEEP_DELAY_PARAM);
        testCurrFreq = currFreq;
        initADC(0);
        SweepResult_I[currSchedule] = getCorrectValue(getADCValue());
        initADC(1);
        SweepResult_Q[currSchedule] = getCorrectValue(getADCValue());

        convertInttoCharArray(currPercent_char,currSchedule,2);
        LCD_disString(3,3,currPercent_char);
        LCD_disString(4,3," %");
        LCD_disString(2,4,TimerBuff);
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
    stopTimer();
}

extern unsigned long test1;
void DotFreq(void)
{
    int inputFreqValue = 0;
    int isOutOfRange = 1;
    do{
        inputFreqValue = inputNum(1,1,"Input:");
        if(inputFreqValue > 1000)
        {
            LCD_clearScreen();
            LCD_disString(1,2,"Out of range!");
            DELAY_PROCESS_MS(200);
            isOutOfRange = 1;
        }
        else
            isOutOfRange = 0;
    }while(isOutOfRange);
    unsigned long tempFreq = (unsigned long)inputFreqValue * 1000 ;
    setSinOutput(tempFreq, 4090);
    DELAY_PROCESS_MS(10);
    initADC(0);
    DotResult_I = getCorrectValue(getADCValue());
    initADC(1);
    DotResult_Q = getCorrectValue(getADCValue());
}

void DotOutput(void)
{
    int inputFreqValue = 0;
    int isOutOfRange = 1;
    do{
        inputFreqValue = inputNum(1,1,"Input:");
        if(inputFreqValue > 1000)
        {
            LCD_clearScreen();
            LCD_disString(1,2,"Out of range!");
            DELAY_PROCESS_MS(200);
            isOutOfRange = 1;
        }
        else
            isOutOfRange = 0;
    }while(isOutOfRange);
    unsigned long tempFreq = (unsigned long)inputFreqValue * 1000 ;
    setSinOutput(tempFreq, 4090);
    while(1)
    {
        LCD_disString(0,4,"Press S4 to Exit!");
        if(Button_S4)
        {
            break;
        }
    }
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
        currI = SweepResult_I[i];
        currQ = SweepResult_Q[i];
        temp = (currI * currI) + (currQ * currQ);
        SweepAmpResult[i] = 2 * SqrtByNewton(temp);
        SweepAmpResult[i] = DataFitting_Amp(SweepAmpResult[i]);
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
        currI = SweepResult_I[i];
        currQ = SweepResult_Q[i];
        temp = currQ / currI;
        SweepPhaseResult[i] = -1 * Arctan(temp);
        SweepPhaseResult[i] = DataFitting_Phase(SweepPhaseResult[i]);
    }
}

void Calculate_DotFreq(void)
{
    float currI, currQ;
    float temp = 0;
    unsigned char AmpValue[8];
    unsigned char PhaseValue[8];
    LCD_clearScreen();
    LCD_disString(1,2,"Calculating...");
    currI = DotResult_I;
    currQ = DotResult_Q;
    temp = (currI * currI) + (currQ * currQ);
    DotAmpResult = 2 * SqrtByNewton(temp);
    DotAmpResult = DataFitting_Amp(DotAmpResult);
    temp = 0;
    temp = currQ / currI;
    DotPhaseResult = -1 * Arctan(temp);
    DotPhaseResult = DataFitting_Phase(DotPhaseResult);
    convertFloattoCharArray(AmpValue,8,DotAmpResult,5);
    convertFloattoCharArray(PhaseValue,8,DotPhaseResult,5);
    LCD_clearScreen();
    LCD_disString(1,2,AmpValue);
    LCD_disString(1,3,PhaseValue);
    LCD_disString(5,3,"°");
}

int convertCord_Y(int inputY)
{
    return Y_DEST - inputY;
}

void drawAmpCordinate_Linear(void)
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

void drawAmpCordinate_dB(unsigned int posOfAxis_X)
{
    LCD_clearBuff();
    LCD_drawLine_X(X_START,X_DEST,posOfAxis_X,1);
    LCD_drawLine_Y(X_START,Y_START,Y_DEST,1);

    //X axis
    LCD_drawPoints(X_DEST,posOfAxis_X,1);
    LCD_drawPoints(X_DEST - 1,posOfAxis_X + 1,1);
    LCD_drawPoints(X_DEST - 1,posOfAxis_X - 1,1);
    LCD_drawPoints(X_DEST - 2,posOfAxis_X + 2,1);
    LCD_drawPoints(X_DEST - 2,posOfAxis_X - 2,1);

    //Y axis
    LCD_drawPoints(X_START - 1,Y_START + 1,1);
    LCD_drawPoints(X_START + 1,Y_START + 1,1);
    LCD_drawPoints(X_START - 2,Y_START + 2,1);
    LCD_drawPoints(X_START + 2,Y_START + 2,1);

    //scales
    int i = 0;
    for(i = X_START; i <= X_DEST; )
    {
        LCD_drawPoints(i,posOfAxis_X - 1,1);
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

void showAmpCurve_Linear(unsigned int ScanSize)
{
    AMPLN_reCalculateTotal:    LCD_clearBuff();
    LCD_clearScreen();
    LCD_disString(1,2,"Drawing...");
    drawAmpCordinate_Linear();
    //find the maximum number of the results
    int i = 0;
    float currMax = SweepAmpResult[0];
    //int intTempResult = 0;
    float y_Scale_Single = 0;
    int x_GraphPos1 = 0;
    int y_GraphPos1 = 0;
    int x_GraphPos2;
    int y_GraphPos2;
    int zoom = 10;
    int startIndex = 0;
    for(i = 0; i < ScanSize ; i++)
    {
        if(SweepAmpResult[i] > currMax)
            currMax = SweepAmpResult[i];
    }
    //intTempResult = (int)currMax;
    //currMax = (float)intTempResult + 1;
    y_Scale_Single = currMax / 50 ;
    y_Scale = y_Scale_Single * 5;

    i = 0;
    for(i = 0 ; i < ScanSize ; i++)
    {
        x_GraphPos2 = i;
        y_GraphPos2 = convertCord_Y((int)(SweepAmpResult[i] / y_Scale_Single));
        LCD_drawLine(x_GraphPos1 + X_START, y_GraphPos1, x_GraphPos2 + X_START, y_GraphPos2, 1);
        x_GraphPos1 = x_GraphPos2;
        y_GraphPos1 = y_GraphPos2;
    }

    AMPLN_displayTotal:    LCD_clearScreen();
    LCD_disGraph();    
    while(1)
    {
        if(Button_S1)       //S1: 显示坐标单位等信息
        {
            Button_S1 = 0;
            showInfo_Unit(MODE_AMP_LN,0);
            while(1)
            {
                if(Button_S1)
                {
                    Button_S1 = 0;
                    goto AMPLN_displayTotal;
                }
            }
        }
        if(Button_S2)       //S2: 显示截止频率
        {
            Button_S2 = 0;
            showInfo_CutOffFreq();
            while(1)
            {
                if(Button_S2)
                {
                    Button_S2 = 0;
                    goto AMPLN_displayTotal;
                }
            }
        }
        if(Button_S4)       //S4:结束曲线查看模式
        {
            Button_S4 = 0;
            break;
        }
        if(Button_S3)       //S3:进入放大模式
        {
            Button_S3 = 0;
            while(1)
            {
                LCD_clearBuff();
                drawAmpCordinate_Linear();
                x_GraphPos1 = 0;
                y_GraphPos1 = 0;
                for(i = startIndex; (i - startIndex) * zoom < X_LENGTH ; i++)
                {
                    x_GraphPos2 = (i - startIndex) * zoom;
                    y_GraphPos2 = convertCord_Y((int)(SweepAmpResult[i] / y_Scale_Single));
                    LCD_drawLine(x_GraphPos1 + X_START, y_GraphPos1, x_GraphPos2 + X_START, y_GraphPos2, 1);
                    x_GraphPos1 = x_GraphPos2;
                    y_GraphPos1 = y_GraphPos2;
                    LCD_drawPoints(x_GraphPos2 - 1 + X_START, y_GraphPos2, 1);
                    LCD_drawPoints(x_GraphPos2 + 1 + X_START, y_GraphPos2, 1);
                    LCD_drawPoints(x_GraphPos2 + X_START, y_GraphPos2 - 1, 1);
                    LCD_drawPoints(x_GraphPos2 + X_START, y_GraphPos2 + 1, 1);
                }
            
            displayZoom:    LCD_clearScreen();
                LCD_disGraph();
                while(1)
                {
                    if(Button_S1)
                    {
                        Button_S1 = 0;
                        showInfo_Unit(MODE_AMP_LN,1);
                        while(1)
                        {
                            if(Button_S1)
                            {
                                Button_S1 = 0;
                                goto displayZoom;
                            }
                        }
                      //  break;
                    }
                    if(Button_S2)
                    {
                        Button_S2 = 0;
                        startIndex = startIndex - 2;
                        break;
                    }
                    if(Button_S3)
                    {
                        Button_S3 = 0;
                        startIndex = startIndex + 2;
                        break;
                    }
                    if(Button_S4)
                    {
                        Button_S4 = 0;
                        goto AMPLN_reCalculateTotal;
                    }
                }
                if(startIndex < 0)
                    startIndex = 0;
                if(startIndex > ScanSize - 5)
                    startIndex = ScanSize - 5;
            }
        }
    }   
    
}

void showAmpCurve_dB(unsigned int ScanSize)
{
    AMPDB_reCalculateTotal: LCD_clearBuff();
    LCD_clearScreen();
    LCD_BacktoStrMode();
    LCD_disString(1,2,"Drawing...");
    int i;
    for(i = 0; i < ScanSize ; i++)
        SweepAmpResult_dB[i] = 20 * (float)log10((double)SweepAmpResult[i]);
    float currMax = -100000;
    float currMin = 100000;
    float y_Scale_Single = 0;
    int x_GraphPos1 = 0;
    int y_GraphPos1 = 0;
    int x_GraphPos2;
    int y_GraphPos2;
    unsigned int posOfAxis_X;
    int zoom = 10;
    int startIndex = 0;
    for(i = 0; i < ScanSize ; i++)
    {
        if(SweepAmpResult_dB[i] > currMax)
        {
            currMax = SweepAmpResult_dB[i];
        }
        if(SweepAmpResult_dB[i] < currMin)
        {
            currMin = SweepAmpResult_dB[i];
        }
    }
    if(currMax < 0 )
    {
        currMax = 5;
    }
    y_Scale_Single = (currMax - currMin) / 60 ;
    y_Scale = y_Scale_Single * 5;
    posOfAxis_X = (unsigned int)convertCord_Y((int)(( 0 - currMin) / y_Scale_Single));
    drawAmpCordinate_dB(posOfAxis_X);
    i = 0;
    for(i = 0 ; i < ScanSize ; i++)
    {
        x_GraphPos2 = i; 
        y_GraphPos2 = convertCord_Y((int)((SweepAmpResult_dB[i] - currMin) / y_Scale_Single));
        LCD_drawLine(x_GraphPos1 + X_START, y_GraphPos1, x_GraphPos2 + X_START, y_GraphPos2, 1);
        x_GraphPos1 = x_GraphPos2;
        y_GraphPos1 = y_GraphPos2;
    }
    AMPDB_displayTotal: LCD_clearScreen();
    LCD_disGraph();
    while(1)
    {
        if(Button_S1)       //S1: 显示坐标单位等信息
        {
            Button_S1 = 0;
            showInfo_Unit(MODE_AMP_DB,0);
            while(1)
            {
                if(Button_S1)
                {
                    Button_S1 = 0;
                    goto AMPDB_displayTotal;
                }
            }
        }
        if(Button_S2)       //S2: 显示截止频率
        {
            Button_S2 = 0;
            showInfo_CutOffFreq();
            while(1)
            {
                if(Button_S2)
                {
                    Button_S2 = 0;
                    goto AMPDB_displayTotal;
                }
            }
        }
        if(Button_S4)       //S4:结束曲线查看模式
        {
            Button_S4 = 0;
            break;
        }
        if(Button_S3)       //S3:进入放大模式
        {
            Button_S3 = 0;
            while(1)
            {
                LCD_clearBuff();
                drawAmpCordinate_dB(posOfAxis_X);
                x_GraphPos1 = 0;
                y_GraphPos1 = 0;
                for(i = startIndex; (i - startIndex) * zoom < X_LENGTH ; i++)
                {
                    x_GraphPos2 = (i - startIndex) * zoom;
                    y_GraphPos2 = convertCord_Y((int)((SweepAmpResult_dB[i] - currMin) / y_Scale_Single));
                    LCD_drawLine(x_GraphPos1 + X_START, y_GraphPos1, x_GraphPos2 + X_START, y_GraphPos2, 1);
                    x_GraphPos1 = x_GraphPos2;
                    y_GraphPos1 = y_GraphPos2;
                    LCD_drawPoints(x_GraphPos2 - 1 + X_START, y_GraphPos2, 1);
                    LCD_drawPoints(x_GraphPos2 + 1 + X_START, y_GraphPos2, 1);
                    LCD_drawPoints(x_GraphPos2 + X_START, y_GraphPos2 - 1, 1);
                    LCD_drawPoints(x_GraphPos2 + X_START, y_GraphPos2 + 1, 1);
                }
            
            AMPDB_displayZoom:    LCD_clearScreen();
                LCD_disGraph();
                while(1)
                {
                    if(Button_S1)
                    {
                        Button_S1 = 0;
                        showInfo_Unit(MODE_AMP_DB,1);
                        while(1)
                        {
                            if(Button_S1)
                            {
                                Button_S1 = 0;
                                goto AMPDB_displayZoom;
                            }
                        }
                    //    break;
                    }
                    if(Button_S2)
                    {
                        Button_S2 = 0;
                        startIndex = startIndex - 2;
                        break;
                    }
                    if(Button_S3)
                    {
                        Button_S3 = 0;
                        startIndex = startIndex + 2;
                        break;
                    }
                    if(Button_S4)
                    {
                        Button_S4 = 0;
                        goto AMPDB_reCalculateTotal;
                    }
                }
                if(startIndex < 0)
                    startIndex = 0;
                if(startIndex > ScanSize - 5)
                    startIndex = ScanSize - 5;
            }
        }
    }
}


void showPhaseCurve(unsigned int ScanSize)
{
    PHASE_reCalculateTotal: LCD_clearBuff();
    LCD_disString(1,2,"Drawing...");
    drawPhaseCordinate();
    int i = 0;
    float y_Scale_Single = 0;
    y_Scale_Single = 360 / Y_LENGTH;
    y_Scale = 5 * y_Scale_Single;
    int x_GraphPos1 = 0;
    int y_GraphPos1 = Y_MIDDLE;
    int x_GraphPos2;
    int y_GraphPos2;
    int zoom = 10;
    int startIndex = 0;
    for(i = 0; i < ScanSize; i++)
    {
        x_GraphPos2 = i;
        y_GraphPos2 = convertCord_Y((int)((SweepPhaseResult[i] + 180) / y_Scale_Single));
        LCD_drawLine(x_GraphPos1 + X_START, y_GraphPos1, x_GraphPos2 + X_START, y_GraphPos2, 1);
        x_GraphPos1 = x_GraphPos2;
        y_GraphPos1 = y_GraphPos2;
    }
    PHASE_displayTotal: LCD_clearScreen();
    LCD_disGraph();
    while(1)
    {
        if(Button_S1)       //S1: 显示坐标单位等信息
        {
            Button_S1 = 0;
            showInfo_Unit(MODE_PHASE,0);
            while(1)
            {
                if(Button_S1)
                {
                    Button_S1 = 0;
                    goto PHASE_displayTotal;
                }
            }
        }
        if(Button_S4)       //S4:结束曲线查看模式
        {
            Button_S4 = 0;
            break;
        }
        if(Button_S3)       //S3:进入放大模式
        {
            Button_S3 = 0;
            while(1)
            {
                LCD_clearBuff();
                drawPhaseCordinate();
                x_GraphPos1 = 0;
                y_GraphPos1 = 0;
                for(i = startIndex; (i - startIndex) * zoom < X_LENGTH ; i++)
                {
                    x_GraphPos2 = (i - startIndex) * zoom;
                    y_GraphPos2 = convertCord_Y((int)((SweepPhaseResult[i] + 180) / y_Scale_Single));
                    LCD_drawLine(x_GraphPos1 + X_START, y_GraphPos1, x_GraphPos2 + X_START, y_GraphPos2, 1);
                    x_GraphPos1 = x_GraphPos2;
                    y_GraphPos1 = y_GraphPos2;
                    LCD_drawPoints(x_GraphPos2 - 1 + X_START, y_GraphPos2, 1);
                    LCD_drawPoints(x_GraphPos2 + 1 + X_START, y_GraphPos2, 1);
                    LCD_drawPoints(x_GraphPos2 + X_START, y_GraphPos2 - 1, 1);
                    LCD_drawPoints(x_GraphPos2 + X_START, y_GraphPos2 + 1, 1);
                }
            
            PHASE_displayZoom:    LCD_clearScreen();
                LCD_disGraph();
                while(1)
                {
                    if(Button_S1)
                    {
                        Button_S1 = 0;
                        showInfo_Unit(MODE_PHASE,1);
                        while(1)
                        {
                            if(Button_S1)
                            {
                                Button_S1 = 0;
                                goto PHASE_displayZoom;
                            }
                        }
                     //   break;
                    }
                    if(Button_S2)
                    {
                        Button_S2 = 0;
                        startIndex = startIndex - 2;
                        break;
                    }
                    if(Button_S3)
                    {
                        Button_S3 = 0;
                        startIndex = startIndex + 2;
                        break;
                    }
                    if(Button_S4)
                    {
                        Button_S4 = 0;
                        goto PHASE_reCalculateTotal;
                    }
                }
                if(startIndex < 0)
                    startIndex = 0;
                if(startIndex > ScanSize - 5)
                    startIndex = ScanSize - 5;
            }
        }
    }
}

void showInfo_Unit(int mode, int isZoom)
{
    LCD_BacktoStrMode();
    unsigned char x_Value[5];
    unsigned char y_Value[5];
    if(isZoom)
        x_Scale = x_Scale / 10;
    convertFloattoCharArray(x_Value,5,x_Scale,3);
    convertFloattoCharArray(y_Value,5,y_Scale,3);
    LCD_disString(0,1,"X:");
    LCD_disString(0,2,"Y:");
    LCD_disString(1,1,x_Value);
    LCD_disString(1,2,y_Value);
    if(mode == MODE_AMP_LN)
    {
        LCD_disString(4,1,"kHz/div");
        LCD_disString(4,2," /div");
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

void showInfo_CutOffFreq(void)
{
    unsigned char Fc_1[8], Fc_2[8];
    Calculate_CutOffFreq();
    convertFloattoCharArray(Fc_1,(long)8,cutOffFreq1,(long)3);
    convertFloattoCharArray(Fc_2,(long)8,cutOffFreq2,(long)3);
    LCD_BacktoStrMode();
    LCD_clearScreen();
    LCD_disString(0,2,"Fc1:");
    LCD_disString(0,3,"Fc2:");
    LCD_disString(3,2,Fc_1);
    LCD_disString(3,3,Fc_2);
    LCD_disString(7,2,"Hz");
    LCD_disString(7,3,"Hz");
}
void showCurve(int mode, unsigned int ScanSize)
{
    switch(mode)
    {
        case MODE_AMP_LN: showAmpCurve_Linear(ScanSize); break;
        case MODE_AMP_DB: showAmpCurve_dB(ScanSize); break;
        case MODE_PHASE: showPhaseCurve(ScanSize); break;
        default: break;
    }
}



void SweepOutput(void)
{
    unsigned long currFreq = 1000;
    unsigned long stepFreq = 0;
    unsigned char displayCurrFreq_char[7] = {'0','0','0','0','0','0','0'};
    unsigned long displayCurrFreq = 0;
    unsigned int isExit = 0;
    LCD_clearScreen();
    LCD_disString(0,1,"Sweep Step:");
    LCD_disString(0,2,"1. 10Hz");
    LCD_disString(0,3,"2. 1kHz");
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
    while(currFreq <= 1000000)
    {
        LCD_disString(1,2,"Scaning ...");
        setSinOutput(currFreq,4090);
        DELAY_PROCESS_MS(1);
        displayCurrFreq = (float)currFreq;
        convertFloattoCharArray(displayCurrFreq_char,7,displayCurrFreq,0);
        LCD_disString(2,3,displayCurrFreq_char);
        LCD_disString(5,3,"Hz");
        currFreq += stepFreq;
        if(Button_S4)
        {
            //Button_S3         不需要,作为退出后直接返回程序起始点的判断标志
            Button_S4 = 0;
            isExit = 1;
            break;
        }
        if(Button_S2)
        {
            LCD_clearScreen();
            LCD_disString(2,1,"Stop...");
            LCD_disString(2,3,displayCurrFreq_char);
            LCD_disString(5,3,"Hz");
            Button_S2 = 0;
            while(Button_S2 != 1);
            LCD_clearScreen();
            Button_S2 = 0;
        }
    }
    LCD_clearScreen();
    if(isExit)
        LCD_disString(1,2,"Exit!");
    else
        LCD_disString(1,2,"Scan Finish!");
    DELAY_PROCESS_MS(100);
}

void Calculate_CutOffFreq(void)
{
    volatile int i;
    volatile int indexOfMax;
    volatile float cutOffValue = 0;
    volatile float currMax = SweepAmpResult[0];
    volatile float num1 = 0;
    volatile float num2 = 0;
    for(i = 0; i < SCAN_SIZE; i ++)
    {
        if(SweepAmpResult[i] > currMax)
        {
            currMax = SweepAmpResult[i];
            indexOfMax = i;
        }
    }
    cutOffValue = 0.707 * currMax;
   for(i = 0; i <= indexOfMax; i++)
   {
       num1 = SweepAmpResult[i];
       num2 = SweepAmpResult[i+1];
       if((num1 <= cutOffValue) && (num2 >= cutOffValue))
       {
           cutOffFreq1 = (float)i * 10000 + ((cutOffValue - num1) * 10000 / (num2 - num1));
           break;
       }
       cutOffFreq1 = 0.0;
   }
   num1 = 0;
   num2 = 0;
   for(i = indexOfMax; i < SCAN_SIZE - 1; i++)
   {
       num1 = SweepAmpResult[i];
       num2 = SweepAmpResult[i+1];
       if((num1 >= cutOffValue) && (num2 <= cutOffValue))
       {
           cutOffFreq2 = (float)i * 10000 + ((num1 - cutOffValue) * 10000 / (num1 - num2));
           break;
       }
       cutOffFreq2 = 0.0;
   }
}

float DataFitting_Amp(float inputAmp)
{
    return (1.2828 * inputAmp);
}

float DataFitting_Phase(float inputPhase)
{
    return (0.9 * inputPhase + 3.075);
}

unsigned int CustomSweep(void)
{
    unsigned long currFreq = 1000;
    unsigned long stepFreq = 0;
    unsigned int currSchedule = 0;
    unsigned int amount = 0;
    unsigned long startFreq = 0;
    unsigned long endFreq = 0;
startCustomSetting : LCD_clearScreen();
    startFreq = (unsigned long)inputNum(1,1,"Start:") * 1000;
    stepFreq = (unsigned long)inputNum(1,1,"Step:") * 1000;
    endFreq = (unsigned long)inputNum(1,1,"End:") * 1000;

    if(stepFreq == 0 || endFreq == 0 || stepFreq > endFreq || endFreq > 1200000)
    {
        LCD_clearScreen();
        LCD_disString(1,2,"Scope Error!");
        DELAY_PROCESS_MS(200);
        goto startCustomSetting;
    }
    amount = (endFreq - startFreq) / stepFreq ;
    if(amount > 100)
    {
        LCD_clearScreen();
        LCD_disString(1,2,"Cannot more than 100 points!");
        DELAY_PROCESS_MS(200);
        goto startCustomSetting;
    }
    x_Scale = (stepFreq / 1000) * 5 ;
    LCD_clearScreen();
    currFreq = startFreq;
    LCD_disString(1,1,"Scaning...");
    startTimer();
    while(currFreq <= endFreq)
    {
        setSinOutput(currFreq,4090);
        DELAY_PROCESS_MS(SWEEP_DELAY_PARAM);
        initADC(0);
        SweepResult_I[currSchedule] = getCorrectValue(getADCValue());
        initADC(1);
        SweepResult_Q[currSchedule] = getCorrectValue(getADCValue());
        LCD_disString(2,4,TimerBuff);
        currFreq += stepFreq;
        currSchedule++;
    }
    LCD_clearScreen();
    LCD_disString(1,2,"Scan Finish!");
    stopTimer();
    return amount;
}
