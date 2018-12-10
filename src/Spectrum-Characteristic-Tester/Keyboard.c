/*=======================================================
Author				  :				ctlvie
Email Address		:				ctlvie@gmail.com
Filename			  :				Keyboad.c
Date				    :				2018-11-14
Description			:				键盘扫描程序

Modification History:
Date		By			Version		Description
----------------------------------------------------------
181114		ctlvie		1.0			基本实现键盘扫描与数字输入
========================================================*/

#include "Keyboard.h"
#include<msp430f5529.h>
int keylock = 0;
int key_buff;   //存储上一次按下的键值
int flag = 0;
uchar key;      //存储实时的按键信息, 无键按下时为 0
int NullCnt = 0;        //记录没有键按下的查询次数
int ButtonDownCnt = 0;  //记录有按键按下的查询次数


//-------------------------------------------------
//Name:         keyScan(void)
//Description:  键盘扫描(行扫描法)
//Input:        无
//Output:       按键位置坐标，无键按下时返回 0 
//-------------------------------------------------
uchar keyScan(void)
{

    flag=0;
   uchar temp;
   uchar tbuff;
   IO_KEYBOARD_SEL = 0b00000000;                   //P1普通IO功能
   IO_KEYBOARD_DIR = 0b00001111; //P0~p3设置为输出模式，p4~p7为输入

   /*************************扫描第1行***********************/
      IO_KEYBOARD_REN |=  0b00001110;
      IO_KEYBOARD_OUT = 0b00001110;
      temp = IO_KEYBOARD_IN & 0b11110000;
      if(temp != 0b11110000)    // 说明高四位有低电平输入
      {
          tbuff = temp;
         DELAY_KEYBOARD_MS(PARA_DEBOUNCE);
          temp = IO_KEYBOARD_IN & 0b11110000;

        if(temp != 0b11110000 && temp == tbuff)
        {
            if(keylock == 0)
            {
              keylock = 1;
              temp = IO_KEYBOARD_IN & 0b11110000;
            switch(temp)
            {
              case  0b11100000: key=41; break;
              case  0b11010000: key=42; break;
              case  0b10110000: key=43; break;
              case  0b01110000: key=44; break;
              default : key = 00;break;
            }
            key_buff = key;
            return key;
            }

        }

      }

    /*************************扫描第2行***********************/
   IO_KEYBOARD_REN |=  0b00001101;
   IO_KEYBOARD_OUT = 0b00001101;
   temp = IO_KEYBOARD_IN& 0b11110000;
   if(temp != 0b11110000)    // 说明高四位有低电平输入
   {
       tbuff = temp;
      DELAY_KEYBOARD_MS(PARA_DEBOUNCE);
       temp = IO_KEYBOARD_IN& 0b11110000;

     if(temp != 0b11110000 && temp == tbuff)
     {
         if(keylock == 0)
         {
           keylock = 1;
           temp = IO_KEYBOARD_IN & 0b11110000;
         switch(temp)
         {
           case  0b11100000: key=31; break;
           case  0b11010000: key=32; break;
           case  0b10110000: key=33; break;
           case  0b01110000: key=34; break;
           default : key = 00;break;
         }
         key_buff = key;
         return key;
         }

     }

   }
   /*************************扫描第3行***********************/
   IO_KEYBOARD_REN |=  0b00001011;
   IO_KEYBOARD_OUT = 0b00001011;
      temp = IO_KEYBOARD_IN & 0b11110000;//
      if(temp != 0b11110000)    // 说明高四位有低电平输入
      {
          tbuff = temp;
          DELAY_KEYBOARD_MS(PARA_DEBOUNCE);
         temp = IO_KEYBOARD_IN& 0b11110000;
        if(temp != 0b11110000 && temp == tbuff)
        {
            if(keylock == 0)
            {
              keylock = 1;
              temp = IO_KEYBOARD_IN& 0b11110000;
            switch(temp)
            {
              case  0b11100000: key=21; break;
              case  0b11010000: key=22; break;
              case  0b10110000: key=23; break;
              case  0b01110000: key=24; break;
              default : key = 00;break;
            }
            key_buff = key;
            return key ;
            }
        }

      }
      /*************************扫描第4行***********************/
      IO_KEYBOARD_REN |=  0b00000111;
      IO_KEYBOARD_OUT = 0b00000111;
         temp = IO_KEYBOARD_IN & 0b11110000;//
         if(temp != 0b11110000)    // 说明高四位有低电平输入
         {
             tbuff = temp;
             DELAY_KEYBOARD_MS(PARA_DEBOUNCE);
            temp = IO_KEYBOARD_IN & 0b11110000;
           if(temp != 0b11110000 && temp == tbuff)
           {
              if(keylock == 0)
              {
                  keylock = 1;
                  temp = IO_KEYBOARD_IN & 0b11110000;
               switch(temp)
               {
                 case  0b11100000: key=11; break;
                 case  0b11010000: key=12; break;
                 case  0b10110000: key=13; break;
                 case  0b01110000: key=14; break;
                 default : key = 00;break;
               }
               key_buff = key;
               return key;
              }
           }

         }



    keylock = 0;
    return 0;


}

//-------------------------------------------------
//Name:         getKeyCdnt(void)
//Description:  等待，直至有键按下，并获取按键坐标 (长按仅记录一次)
//Input:        无
//Output:       按键位置坐标，无键按下时返回 0 
//-------------------------------------------------          
uchar getKeyCdnt(void) {
  uchar KeyCdnt;
  NullCnt = 0;
  //查询到一定次数的键盘为空 
  do {
    KeyCdnt = keyScan();
    if(KeyCdnt == 0) {
      NullCnt ++;
    }
    else {
      NullCnt = 0;
    }
  }while(NullCnt < PARA_WAITFORBUTTON);
  //允许按键按下，之后只要有按键为非零值即可认为是正常操作
  do {
    KeyCdnt = keyScan();
  }while(KeyCdnt == 0);
  ButtonDownCnt ++;
  return KeyCdnt;
}

//-------------------------------------------------
//Name:         getKeyValue(void)
//Description:  将已有的按键坐标转换为键值，并屏蔽无用键值
//Input:        无
//Output:       有效范围内的键值 
//------------------------------------------------- 
uchar getKeyValue(void) {
  uchar KeyValue;
  do {
    KeyValue = getKeyCdnt();
  }while ( (KeyValue == 14) ||(KeyValue == 34) ||(KeyValue == 44)
          ||(KeyValue == 41) ||(KeyValue == 42) ||(KeyValue == 43) ); //屏蔽不需要的键值
  if (KeyValue == 11) return '1';  
  else if (KeyValue == 21) return '2';
  else if (KeyValue == 31) return '3';
  else if (KeyValue == 12) return '4';
  else if (KeyValue == 22) return '5';
  else if (KeyValue == 32) return '6';
  else if (KeyValue == 13) return '7';
  else if (KeyValue == 23) return '8';
  else if (KeyValue == 33) return '9';
  else return '0';
}
