/*=======================================================
Author              :               ctlvie
Email Address       :               ctlvie@gmail.com
Filename            :               main.c
Date                :               2018-11-14
Description         :               

Modification History:
Date        By          Version     Description
----------------------------------------------------------
181114      ctlvie      1.0         Initial Version
========================================================*/

#include<msp430f5529.h>

#include"AD9854.h"
#include"Keyboard.h"

#define uint  unsigned int
#define uchar unsigned char
#define ulong unsigned long
//ulong  Freq_mult_ulong;
//double Freq_mult_doulle;
void DelayXms(unsigned int i);
//extern uchar FreqWord[6];

uchar virKey = 99;
  uchar arrayKey[40];
  int arrayNum = 0;


void main( void )
{
	
	WDTCTL = WDTPW + WDTHOLD; //关闭看门狗
	initLCD();
	while(1)
	{
	
       DisplayString(1,1,"speed");   		
                
  }
	
}


/*Keyboard测试用main函数
main(void) {
  WDTCTL = WDTPW + WDTHOLD;
  uchar tempKey;


  while(1) {
    arrayKey[arrayNum] = getKeyValue();
    arrayNum ++ ;
  }
}
*/



/* AD9854测试用main函数
int main( void )
{

   double f = 1000000.0;
   unsigned int i;
   // Stop watchdog timer to prevent time out reset
   WDTCTL = WDTPW + WDTHOLD;

   AD9854_Init();            //波形发生器初始化

   while(1){
     for(i=1;i<60;i++){

        AD9854_SetSine(f*i,4000); //产生xxMHz正弦信号
        DelayXms(8000);
     }
  }
  return 0;
}
void DelayXms(unsigned int i){
    unsigned int j;
    for( ; i>0; i--){
      for(j=0;j<200;j++);
    }

}
*/
