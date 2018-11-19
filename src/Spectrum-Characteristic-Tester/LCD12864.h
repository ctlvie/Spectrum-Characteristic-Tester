/*=======================================================
Author				  :				ctlvie
Email Address		  :				ctlvie@gmail.com
Filename			  :				LCD12864.h
Date				  :			    2018-11-19
Description			  :				12864液晶屏驱动

Modification History:
Date		By			Version		Description
----------------------------------------------------------
181119		ctlvie		1.0			
========================================================*/


/*=======================================================
接线方式:
1. VSS -> GND
2. VDD -> +5V
3. VO  -> +5V ? 空脚
4. CS  -> CS
5. SID -> SID
6. SCLK -> SCLK
7~14.DB0~DB7 并行输入
15. PSB-> 串行接GND 并行接+5V
16. NC -> 空脚
17. RST -> +5V
18. VEE -> 空脚
19. BLA -> +5V
20. BLK -> GND
========================================================*/

#ifndef LCD12864_H_
#define LCD12864_H_

#define IO_LCD_DIR      P6DIR
#define IO_LCD_OUT      P6OUT
#define IO_LCD_BIT_CS   BIT0
#define IO_LCD_BIT_SID  BIT1
#define IO_LCD_BIT_SCLK BIT2

#define CS1     IO_LCD_OUT |=   IO_LCD_BIT_CS			
#define CS0     IO_LCD_OUT &=   IO_LCD_BIT_CS
#define SID1    IO_LCD_OUT |=   IO_LCD_BIT_SID
#define SID0    IO_LCD_OUT &=~  IO_LCD_BIT_SID
#define SCLK1   IO_LCD_OUT |=   IO_LCD_BIT_SCLK
#define SCLK0   IO_LCD_OUT &=~  IO_LCD_BIT_SCLK

#define  uchar unsigned char
#define  uint unsigned int

void DelayUs2x(unsigned char t);
void DELAY_LCD_MS(unsigned char t);
void SendByte(unsigned char zdata);
void WriteCommand(unsigned char cmdcode);
void WriteData(unsigned char Dispdata);
void LCS_clearAll(void);
void LCD_disString(unsigned int x,unsigned int y,unsigned char* s);
void LCD_disGBStr(unsigned char *CorpInf);
void initLCD();


#endif /* LCD12864_H_ */
