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

#define BUTTON_THRESHOLD  60

#define IO_BUTTON_S1_IFG P2IFG
#define IO_BUTTON_S1_IN  P2IN
#define IO_BUTTON_S1_DIR P2DIR
#define IO_BUTTON_S1_IES P2IES
#define IO_BUTTON_S1_OUT P2OUT
#define IO_BUTTON_S1_IE  P2IE
#define IO_BUTTON_S1_REN P2REN

#define IO_BUTTON_S2_IFG P1IFG
#define IO_BUTTON_S2_IN  P1IN
#define IO_BUTTON_S2_DIR P1DIR
#define IO_BUTTON_S2_IES P1IES
#define IO_BUTTON_S2_OUT P1OUT
#define IO_BUTTON_S2_IE  P1IE
#define IO_BUTTON_S2_REN P1REN

#define IO_BUTTON_BIT_S1 BIT2
#define IO_BUTTON_BIT_S2 BIT1

#define CPU_F_BUTTON ((double)8000000)
#define DELAY_BUTTON_US(x) __delay_cycles((long)(CPU_F_BUTTON*(double)x/1000000.0))
#define DELAY_BUTTON_MS(x) __delay_cycles((long)(CPU_F_BUTTON*(double)x/1000.0))


void initButtons(void);
void initButtonsTimer(void);

#endif /* BUTTON_H_ */
