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
    1. VSS ->       GND
    2. VDD ->       +5V
    3. VO  ->       NC
    4. RS (CS)   -> CS      ->  P2.0
    5. R/W (SID) -> SID     ->  P2.4
    6. E (SCLK)  -> SCLK    ->  P2.5
    7~14.DB0~DB7    并行输入
    15. PSB->       串行接GND 并行接+5V
    16. NC ->       NC
    17. RST ->      +5V
    18. VEE ->      NC
    19. BLA ->      +5V
    20. BLK ->      GND
========================================================*/

#ifndef LCD12864_H_
#define LCD12864_H_

#define IO_LCD_DIR      P2DIR
#define IO_LCD_OUT      P2OUT
#define IO_LCD_IN       P2IN
#define IO_LCD_BIT_CS   BIT0
#define IO_LCD_BIT_SID  BIT4
#define IO_LCD_BIT_SCLK BIT5

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
void LCD_clearCommand(void);
void LCD_clearScreen(void);
void LCD_clearBuff(void);
void LCD_disString(unsigned int x,unsigned int y,unsigned char* s);
void LCD_disChar(unsigned int x,unsigned int y,uchar s);
void LCD_disGBStr(unsigned char *LCD_ChineseBuff);
void LCD_disPic(uchar *xc_PicArea);
void LCD_disGraph(void);
void LCD_CursorON(unsigned int x,unsigned int y);
void LCD_CursorOFF(void);
void LCD_drawPoints(int x_pos, int y_pos,uint color);
void LCD_drawLine_X(unsigned char X0,unsigned char X1,unsigned char Y,unsigned char Color);
void LCD_drawLine_Y(unsigned char X,unsigned char Y0,unsigned char Y1,unsigned char Color);
void LCD_drawLine(unsigned char X0,unsigned char Y0,unsigned char X1,unsigned char Y1,unsigned char Color);
void initLCD();
void LCD_BacktoStrMode(void);
#endif /* LCD12864_H_ */
