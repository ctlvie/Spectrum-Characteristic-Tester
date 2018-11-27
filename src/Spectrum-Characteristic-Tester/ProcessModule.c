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

extern unsigned long testCurrFreq;

void ScanAmpFreq(unsigned int mode)
{
    unsigned long currFreq = 1000;
    unsigned long stepFreq = 0;
    unsigned int currShedule_int = 0;
    unsigned char currShedule_char[2] = {'0','0'};
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
        if((currFreq % 10000) == 0)
        {
            currShedule_int = currFreq / 10000;
            convertInttoCharArray(currShedule_char,currShedule_int,2);
            LCD_disString(3,3,currShedule_char);
            LCD_disString(4,3," %");
        }
        currFreq += stepFreq;
    }
    LCD_clearScreen();
    LCD_disString(1,2,"Scan Finish!");
}

