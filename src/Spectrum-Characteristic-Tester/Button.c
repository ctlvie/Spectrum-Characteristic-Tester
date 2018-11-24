


#include <msp430f5529.h>
#include "Button.h"
volatile int key_pre_cnt = 0;
volatile int interrupt_key = 0;
extern volatile int Button_S1; //should be defined in the "main.c" as a global varible
extern volatile int Button_S2; //should be defined in the "main.c" as a global varible

extern int t ;

void initButtonsTimer(void)
{
    TA0CCTL0 = CCIE;
    TA0CCR0 = 10000;
    TA0CTL = TASSEL_2 + MC_1 + TACLR;
}

void initButtons(void)
{
    _DINT();
    IO_BUTTON_S2_REN |= IO_BUTTON_BIT_S2;
    IO_BUTTON_S2_OUT |= IO_BUTTON_BIT_S2;
    IO_BUTTON_S2_DIR &= IO_BUTTON_BIT_S2;
    IO_BUTTON_S2_IES |= IO_BUTTON_BIT_S2;
    IO_BUTTON_S2_IFG &= ~IO_BUTTON_BIT_S2;
    IO_BUTTON_S2_IE |=  IO_BUTTON_BIT_S2;

    IO_BUTTON_S1_REN |= IO_BUTTON_BIT_S1;
    IO_BUTTON_S1_OUT |= IO_BUTTON_BIT_S1;
    IO_BUTTON_S1_DIR &= IO_BUTTON_BIT_S1;
    IO_BUTTON_S1_IES |= IO_BUTTON_BIT_S1;
    IO_BUTTON_S1_IFG &= ~IO_BUTTON_BIT_S1;
    IO_BUTTON_S1_IE |=  IO_BUTTON_BIT_S1;
    
    Button_S1 = 0;
    Button_S2 = 0;

    initButtonsTimer();
    _EINT();
}



#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    t ++;
    if(IO_BUTTON_S1_IFG&IO_BUTTON_BIT_S1)
    {
        if (key_pre_cnt < BUTTON_THRESHOLD) Button_S1=Button_S1;

        if (key_pre_cnt > BUTTON_THRESHOLD)
            {
            Button_S1 = 1;
            key_pre_cnt=0;
            } 
    }
    IO_BUTTON_S1_IFG &=~ IO_BUTTON_BIT_S1;
    IO_BUTTON_S1_IFG = 0;
    IO_BUTTON_S1_IES |= IO_BUTTON_BIT_S1;
    IO_BUTTON_S1_IE |= IO_BUTTON_BIT_S1;

}



#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    t ++;
    if(IO_BUTTON_S2_IFG&IO_BUTTON_BIT_S2)
    {

         if (key_pre_cnt < BUTTON_THRESHOLD) Button_S2 = Button_S2;

         if (key_pre_cnt > BUTTON_THRESHOLD)
             {
             Button_S2 = 1;
             key_pre_cnt=0;
             }
     
    }

    IO_BUTTON_S2_IFG &=~ IO_BUTTON_BIT_S2;
    IO_BUTTON_S2_IFG = 0;
    IO_BUTTON_S2_IES |= IO_BUTTON_BIT_S2;
    IO_BUTTON_S2_IE |= IO_BUTTON_BIT_S2;
}



#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void)
{
    key_pre_cnt ++;
}
