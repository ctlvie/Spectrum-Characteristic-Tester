/*=======================================================
Author				:				ctlvie
Email Address		:				ctlvie@gmail.com
Filename			:				Keyboad.c
Date				:				2018-11-14
Description			:				����ɨ�����

Modification History:
Date		By			Version		Description
----------------------------------------------------------
181114		ctlvie		1.0			Initial Version
========================================================*/

#include "Keyboard.h"
#include<msp430f5529.h>
int keylock = 0;
int key_buff;   //�洢��һ�ΰ��µļ�ֵ
int flag = 0;
uchar key;      //�洢ʵʱ�İ�����Ϣ, �޼�����ʱΪ 0
 
uchar keyscan(void)
{

    flag=0;
   uchar temp;
   uchar tbuff;
   IO_KEYBOARD_SEL = 0b00000000;                   //P1��ͨIO����
   IO_KEYBOARD_DIR = 0b00001111; //P0~p3����Ϊ���ģʽ��p4~p7Ϊ����

   /*************************ɨ���1��***********************/
      IO_KEYBOARD_REN |=  0b00001110;
      IO_KEYBOARD_OUT = 0b00001110;
      temp = IO_KEYBOARD_IN & 0b11110000;
      if(temp != 0b11110000)    // ˵������λ�е͵�ƽ����
      {
          tbuff = temp;
         DELAY_KEYBOARD_MS(15);
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
            flag++;
            if(flag > 40) return 0 ;
            key_buff = key;
            return key;
            }

        }

      }

    /*************************ɨ���2��***********************/
   IO_KEYBOARD_REN |=  0b00001101;
   IO_KEYBOARD_OUT = 0b00001101;
   temp = IO_KEYBOARD_IN& 0b11110000;
   if(temp != 0b11110000)    // ˵������λ�е͵�ƽ����
   {
       tbuff = temp;
      DELAY_KEYBOARD_MS(15);
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
         flag++;
         if(flag > 60) return 0 ;
         key_buff = key;
         return key;
         }

     }

   }
   /*************************ɨ���3��***********************/
   IO_KEYBOARD_REN |=  0b00001011;
   IO_KEYBOARD_OUT = 0b00001011;
      temp = IO_KEYBOARD_IN & 0b11110000;//
      if(temp != 0b11110000)    // ˵������λ�е͵�ƽ����
      {
          tbuff = temp;
          DELAY_KEYBOARD_MS(15);
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
            flag++;
            if(flag > 60) return 0 ;
            key_buff = key;
            return key ;
            }
        }

      }
      /*************************ɨ���4��***********************/
      IO_KEYBOARD_REN |=  0b00000111;
      IO_KEYBOARD_OUT = 0b00000111;
         temp = IO_KEYBOARD_IN & 0b11110000;//
         if(temp != 0b11110000)    // ˵������λ�е͵�ƽ����
         {
             tbuff = temp;
             DELAY_KEYBOARD_MS(15);
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
               flag++;
               if(flag > 60) return 0 ;
               key_buff = key;
               return key;
              }
           }

         }



    keylock = 0;
    return 0;


}
