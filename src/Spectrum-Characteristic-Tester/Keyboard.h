/*=======================================================
Author              :               ctlvie
Email Address       :               ctlvie@gmail.com
Filename            :               Keyboad.h
Date                :               2018-11-14
Description         :               ¼üÅÌÉ¨ÃèÍ·ÎÄ¼þ

Modification History:
Date        By          Version     Description
----------------------------------------------------------
181114      ctlvie      1.0         Initial Version
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

#define IO_KEYBOARD_SEL     P4SEL
#define IO_KEYBOARD_REN     P4REN
#define IO_KEYBOARD_DIR     P4DIR
#define IO_KEYBOARD_OUT     P4OUT
#define IO_KEYBOARD_IN      P4IN

#define CPU_F ((double)8000000)
#define DELAY_KEYBOARD_MS(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

uchar keyscan(void);

#endif /* KEYBOARD_H_ */
