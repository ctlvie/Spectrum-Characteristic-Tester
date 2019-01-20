/*=======================================================
Author				  :				ctlvie
Email Address		  :				ctlvie@gmail.com
Filename			  :				Board.c
Date				  :			    2018-11-24
Description			  :				MSP430F5529LP ??????????

Modification History:
Date		By			Version		Description
----------------------------------------------------------
181124  	ctlvie		1.0			MSP430F5529LP ??????????
181216      ctlvie      2.0         Final Version
========================================================*/
#ifndef BOARD_H_
#define BOARD_H_

#define LED1_ON     P1OUT |= BIT0;
#define LED1_OFF    P1OUT |= BIT0;
#define LED2_ON     P4OUT |= BIT7;
#define LED2_OFF    P4OUT |= BIT7;

void initLED(void);
void initBoard(void);
#endif /* BOARD_H_ */
