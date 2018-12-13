/*=======================================================
Author				  :				ctlvie
Email Address		  :				ctlvie@gmail.com
Filename			  :				GlobalTimer.c
Date				  :			    2018-12-13
Description			  :				???????

Modification History:
Date		By			Version		Description
----------------------------------------------------------
181213  	ctlvie		1.0			
========================================================*/
#include<msp430f5529.h>
#include"GlobalTimer.h"
unsigned long globalTime = 0;
extern unsigned int globalTime_min;
extern unsigned int globalTime_sec;
extern unsigned int globalTime_ms;
extern unsigned char TimerBuff[8];

volatile int isTimerOn = 0;
void initGlobalTimer(void)
{
    TBCCTL0 = CCIE;
    TBCCR0 = TIMER_PARAM;
    TBCTL = TASSEL_2 + MC_1 + TACLR;
    _EINT();
}

void startTimer(void)
{
    globalTime = 0;
    isTimerOn = 1;
}

void stopTimer(void)
{
    globalTime = 0;
    isTimerOn = 0;
    globalTime_min = 0;
    globalTime_sec = 0;
    globalTime_ms = 0;
}


#pragma vector=TIMERB0_VECTOR
__interrupt void TIMERB0_ISR (void)
{
    globalTime ++;
    if(isTimerOn)
    {
        if(globalTime < 100)
        {
            globalTime_ms = globalTime;
            TimerBuff[7] = '0' + (globalTime_ms % 10);
            TimerBuff[6] = '0' + (globalTime_ms / 10);
        }
        else
        {
            globalTime = 0;
            globalTime_sec ++;
            TimerBuff[4] = '0' + (globalTime_sec % 10);
            TimerBuff[3] = '0' + (globalTime_sec / 10);
        }
        if(globalTime_sec >= 60)
        {
            globalTime_sec = 0;
            globalTime_min ++ ;
            TimerBuff[1] = '0' + (globalTime_min % 10);
            TimerBuff[0] = '0' + (globalTime_min / 10);
        }
    }
}
