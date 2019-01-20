/*=======================================================
Author				  :				ctlvie
Email Address		  :				ctlvie@gmail.com
Filename			  :				GlobalTimer.c
Date				  :			    2018-12-13
Description			  :				定时器TimerB配置

Modification History:
Date		By			Version		Description
----------------------------------------------------------
181213  	ctlvie		1.0			功能实现
181216      ctlvie      2.0         Final Version
========================================================*/


#ifndef GLOBALTIMER_H_
#define GLOBALTIMER_H_

#define TIMER_PARAM 10500

void initGlobalTimer(void);
void startTimer(void);
void stopTimer(void);

#endif /* GLOBALTIMER_H_ */
