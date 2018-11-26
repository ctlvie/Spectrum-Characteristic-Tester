/*=======================================================
Author				  :				ctlvie
Email Address		  :				ctlvie@gmail.com
Filename			  :				Board.c
Date				  :			    2018-11-24
Description			  :				MSP430F5529LP ��ʼ���ͻ����������

Modification History:
Date		By			Version		Description
----------------------------------------------------------
181124  	ctlvie		1.0			MSP430F5529LP ��ʼ���ͻ����������
========================================================*/
#include<msp430f5529.h>
#include "Board.h"

void initLED(void)
{
    P1OUT |= BIT0;
    P4OUT |= BIT7;
    P1OUT &= ~BIT0;
    P4OUT &= ~BIT7;
}

void initBoard(void)
{
    initLED();
    P2REN |= BIT1;
}
