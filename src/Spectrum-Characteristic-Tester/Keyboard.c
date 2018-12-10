/*=======================================================
Author				  :				ctlvie
Email Address		:				ctlvie@gmail.com
Filename			  :				Keyboad.c
Date				    :				2018-11-14
Description			:				����ɨ�����

Modification History:
Date		By			Version		Description
----------------------------------------------------------
181114		ctlvie		1.0			����ʵ�ּ���ɨ������������
========================================================*/

#include "Keyboard.h"
#include<msp430f5529.h>
int keylock = 0;
int key_buff;   //�洢��һ�ΰ��µļ�ֵ
int flag = 0;
uchar key;      //�洢ʵʱ�İ�����Ϣ, �޼�����ʱΪ 0
int NullCnt = 0;        //��¼û�м����µĲ�ѯ����
int ButtonDownCnt = 0;  //��¼�а������µĲ�ѯ����


//-------------------------------------------------
//Name:         keyScan(void)
//Description:  ����ɨ��(��ɨ�跨)
//Input:        ��
//Output:       ����λ�����꣬�޼�����ʱ���� 0 
//-------------------------------------------------
uchar keyScan(void)
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

    /*************************ɨ���2��***********************/
   IO_KEYBOARD_REN |=  0b00001101;
   IO_KEYBOARD_OUT = 0b00001101;
   temp = IO_KEYBOARD_IN& 0b11110000;
   if(temp != 0b11110000)    // ˵������λ�е͵�ƽ����
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
   /*************************ɨ���3��***********************/
   IO_KEYBOARD_REN |=  0b00001011;
   IO_KEYBOARD_OUT = 0b00001011;
      temp = IO_KEYBOARD_IN & 0b11110000;//
      if(temp != 0b11110000)    // ˵������λ�е͵�ƽ����
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
      /*************************ɨ���4��***********************/
      IO_KEYBOARD_REN |=  0b00000111;
      IO_KEYBOARD_OUT = 0b00000111;
         temp = IO_KEYBOARD_IN & 0b11110000;//
         if(temp != 0b11110000)    // ˵������λ�е͵�ƽ����
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
//Description:  �ȴ���ֱ���м����£�����ȡ�������� (��������¼һ��)
//Input:        ��
//Output:       ����λ�����꣬�޼�����ʱ���� 0 
//-------------------------------------------------          
uchar getKeyCdnt(void) {
  uchar KeyCdnt;
  NullCnt = 0;
  //��ѯ��һ�������ļ���Ϊ�� 
  do {
    KeyCdnt = keyScan();
    if(KeyCdnt == 0) {
      NullCnt ++;
    }
    else {
      NullCnt = 0;
    }
  }while(NullCnt < PARA_WAITFORBUTTON);
  //���������£�֮��ֻҪ�а���Ϊ����ֵ������Ϊ����������
  do {
    KeyCdnt = keyScan();
  }while(KeyCdnt == 0);
  ButtonDownCnt ++;
  return KeyCdnt;
}

//-------------------------------------------------
//Name:         getKeyValue(void)
//Description:  �����еİ�������ת��Ϊ��ֵ�����������ü�ֵ
//Input:        ��
//Output:       ��Ч��Χ�ڵļ�ֵ 
//------------------------------------------------- 
uchar getKeyValue(void) {
  uchar KeyValue;
  do {
    KeyValue = getKeyCdnt();
  }while ( (KeyValue == 14) ||(KeyValue == 34) ||(KeyValue == 44)
          ||(KeyValue == 41) ||(KeyValue == 42) ||(KeyValue == 43) ); //���β���Ҫ�ļ�ֵ
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
