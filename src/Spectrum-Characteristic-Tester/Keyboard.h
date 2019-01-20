/*=======================================================
Author              :               ctlvie
Email Address       :               ctlvie@gmail.com
Filename            :               Keyboad.h
Date                :               2018-11-14
Description         :               键盘扫描头文件

Modification History:
Date        By          Version     Description
----------------------------------------------------------
181114      ctlvie      1.0         基本实现键盘扫描与数字输入
181216      ctlvie      2.0         Final Version
========================================================*/

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#ifndef uchar
#define uchar unsigned char
#endif
#ifndef ulong
#define ulong unsigned long
#endif
#ifndef uint
#define uint unsigned int
#endif

#define IO_KEYBOARD_SEL     P6SEL
#define IO_KEYBOARD_REN     P6REN
#define IO_KEYBOARD_DIR     P6DIR
#define IO_KEYBOARD_OUT     P6OUT
#define IO_KEYBOARD_IN      P6IN

#define CPU_F_KEYBORD ((double)8000000)
#define DELAY_KEYBOARD_MS(x) __delay_cycles((long)(CPU_F_KEYBORD*(double)x/1000.0))
#define PARA_DEBOUNCE      30           //用于调整消抖的最短时间参数
#define PARA_WAITFORBUTTON 20         //用于调整两次按键之间有效间隔的参数
uchar keyScan(void);
uchar getKeyValue(void);
uchar getKeyCdnt(void);

#endif /* KEYBOARD_H_ */
