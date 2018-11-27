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

extern unsigned long testCurrFreq;


extern float ScanResult_I_1kHz[99];
extern float ScanResult_Q_1kHz[99];


void ScanAmpFreq(unsigned int mode)
{
    unsigned long currFreq = 1000;
    unsigned long stepFreq = 0;
    unsigned int currPercent_int = 0;
    unsigned char currPercent_char[2] = {'0','0'};
    unsigned int currSchedule = 0;
    LCD_clearScreen();
    LCD_disString(1,1,"Scaning...");
    if(mode == MODE_10HZ)
        stepFreq = 10;
    else
        stepFreq = 1000;
    while(currFreq != 1000000)
    {
        setSinOutput(currFreq,4096);
      //  DELAY_PROCESS_MS(5);
        testCurrFreq = currFreq;
        if(mode == MODE_1KHZ)
        {
            initADC(0);
            ScanResult_I_1kHz[currSchedule] = getADCValue();
            initADC(1);
            ScanResult_Q_1kHz[currSchedule] = getADCValue();
        }

        if((currFreq % 10000) == 0)
        {
            currPercent_int = currFreq / 10000;
            convertInttoCharArray(currPercent_char,currPercent_int,2);
            LCD_disString(3,3,currPercent_char);
            LCD_disString(4,3," %");
        }
        currFreq += stepFreq;
        currSchedule++;
    }
    LCD_clearScreen();
    LCD_disString(1,2,"Scan Finish!");
}

