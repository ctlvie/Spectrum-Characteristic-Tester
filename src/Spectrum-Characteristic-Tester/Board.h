/*
 * Board.h
 *
 *  Created on: 2018��11��26��
 *      Author: Admin
 */

#ifndef BOARD_H_
#define BOARD_H_

#define LED1_ON     P1OUT |= BIT0;
#define LED1_OFF    P1OUT |= BIT0;
#define LED2_ON     P4OUT |= BIT7;
#define LED2_OFF    P4OUT |= BIT7;

void initLED(void);
void initBoard(void);
#endif /* BOARD_H_ */
