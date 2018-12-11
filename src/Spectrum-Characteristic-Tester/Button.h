/*=======================================================
Author			    :				ctlvie
Email Address		:				ctlvie@gmail.com
Filename	        :				Button.h
Date				:		        2018-11-22
Description			:				LaunchPad片上按键控制

Modification History:
Date		By			Version		Description
----------------------------------------------------------
181122		ctlvie		1.0			功能实现
========================================================*/

/*=======================================================
    接线方式:
    *  S2正常使用LaunchPad片上的按键
    *  将P1.2引线至S1引脚后面(P1.2控制S1引脚)
    *  需要在主函数中设置全局变量 BUTTON_S1 和 BUTTON_S2
    *  每次读取键值之后需要清零 BUTTON_S1 或 BUTTON_S2
========================================================*/
#ifndef BUTTON_H_
#define BUTTON_H_

#define BUTTON_THRESHOLD  60

#define IO_BUTTON_S1_IFG P1IFG
#define IO_BUTTON_S1_IN  P1IN
#define IO_BUTTON_S1_DIR P1DIR
#define IO_BUTTON_S1_IES P1IES
#define IO_BUTTON_S1_OUT P1OUT
#define IO_BUTTON_S1_IE  P1IE
#define IO_BUTTON_S1_REN P1REN

#define IO_BUTTON_S2_IFG P1IFG
#define IO_BUTTON_S2_IN  P1IN
#define IO_BUTTON_S2_DIR P1DIR
#define IO_BUTTON_S2_IES P1IES
#define IO_BUTTON_S2_OUT P1OUT
#define IO_BUTTON_S2_IE  P1IE
#define IO_BUTTON_S2_REN P1REN

#define IO_BUTTON_S3_IFG P1IFG
#define IO_BUTTON_S3_IN  P1IN
#define IO_BUTTON_S3_DIR P1DIR
#define IO_BUTTON_S3_IES P1IES
#define IO_BUTTON_S3_OUT P1OUT
#define IO_BUTTON_S3_IE  P1IE
#define IO_BUTTON_S3_REN P1REN

#define IO_BUTTON_S4_IFG P1IFG
#define IO_BUTTON_S4_IN  P1IN
#define IO_BUTTON_S4_DIR P1DIR
#define IO_BUTTON_S4_IES P1IES
#define IO_BUTTON_S4_OUT P1OUT
#define IO_BUTTON_S4_IE  P1IE
#define IO_BUTTON_S4_REN P1REN

#define IO_BUTTON_BIT_S1 BIT2
#define IO_BUTTON_BIT_S2 BIT3
#define IO_BUTTON_BIT_S3 BIT4
#define IO_BUTTON_BIT_S4 BIT5

void initButtons(void);
void initButtonsTimer(void);

#endif /* BUTTON_H_ */
