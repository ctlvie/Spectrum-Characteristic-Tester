/*=======================================================
Author				  :				ctlvie
Email Address		  :				ctlvie@gmail.com
Filename			  :				ProcessModule.h
Date				  :			    2018-11-26
Description			  :				核心处理运算函数

Modification History:
Date		By			Version		Description
----------------------------------------------------------
181126		ctlvie		1.0			核心处理运算函数
========================================================*/

#ifndef PROCESSMODULE_H_
#define PROCESSMODULE_H_
#define MODE_10HZ    1
#define MODE_1KHZ    2

#define CPU_F_PROCESS ((double)8000000)
#define DELAY_PROCESS_MS(x) __delay_cycles((long)(CPU_F_KEYBORD*(double)x/1000.0))
#define SCAN_SIZE 100
#define STEP_FREQ 10000

void ScanFreq(void);
void Calculate_Amp(void);
void Calculate_Phase(void);

#endif /* PROCESSMODULE_H_ */
