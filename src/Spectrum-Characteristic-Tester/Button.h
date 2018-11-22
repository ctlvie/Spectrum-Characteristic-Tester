/*=======================================================
Author			    :				ctlvie
Email Address		:				ctlvie@gmail.com
Filename	        :				Button.h
Date				:		        2018-11-22
Description			:				??????

Modification History:
Date		By			Version		Description
----------------------------------------------------------
181122		ctlvie		1.0			
========================================================*/
#ifndef BUTTON_H_
#define BUTTON_H_


#include <stdint.h>

#define BUTTON_S2       0x0002          //P1.1
#define BUTTON_S1       0x0200          //P2.1
#define BUTTON_ALL      0x0202          

volatile extern uint16_t buttonsPressed;
volatile extern uint8_t  buttonDebounce;

extern void Buttons_init(uint16_t buttonsMask);
extern void Buttons_interruptEnable(uint16_t buttonsMask);
extern void Buttons_interruptDisable(uint16_t buttonsMask);



#endif /* BUTTON_H_ */
