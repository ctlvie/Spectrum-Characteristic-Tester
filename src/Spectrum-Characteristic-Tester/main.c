#include<msp430f5529.h>
#include"AD9854.h"
#define uint  unsigned int
#define uchar unsigned char
#define ulong unsigned long
//ulong  Freq_mult_ulong;
//double Freq_mult_doulle;
void DelayXms(unsigned int i);
//extern uchar FreqWord[6];

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
