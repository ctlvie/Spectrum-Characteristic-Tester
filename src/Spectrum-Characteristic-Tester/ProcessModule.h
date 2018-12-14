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

#define X_START     10
#define Y_START     2
#define X_DEST      110
#define Y_DEST      62
#define X_LENGTH    100 //X_DEST - X_START
#define Y_LENGTH    60  //Y_DEST - Y_START
#define Y_MIDDLE    32  //Y_START + (Y_LENGTH / 2)


#define MODE_AMP_LN    1
#define MODE_AMP_DB    2
#define MODE_PHASE     3
void ScanFreq(void);
void PointFreq(void);
void Calculate_Amp(void);
void Calculate_Phase(void);
void Calculate_PointFreq(void);
void Calculate_CutOffFreq(void);
int convertCord_Y(int inputY);
void drawAmpCordinate_Linear(void);
void drawPhaseCordinate(void);
void showAmpCurve_Linear(void);
void showAmpCurve_dB(void);
void showPhaseCurve(void);
void showInfo_Unit(int mode, int isZoom);
void showInfo_CutOffFreq(void);
void showCurve(int mode);
float getCorrectValue(float input);
void ScanOutput(void);
void PointOutput(void);
float DataFitting_Amp(float inputAmp);
float DataFitting_Phase(float inputPhase);
void CustomScan(void);

#endif /* PROCESSMODULE_H_ */
