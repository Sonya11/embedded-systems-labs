/*
 *
Author: Sonya Schuppan
File: timer.c
Description: systick and timerA0 config
Tools: TI v16.9.3.LTS compiler, XTS JTAG debug probe
*
*/

#include "timer.h"
#include "msp.h" 

void systick_config(){
    SYSTICK_STRVR = 0x00FFFFFF; //set to max values since this timer decrements from max to 0 :)
    SYSTICK_STCVR = 0x00; //count value
    SYSTICK_STCSR |= 0x0001; //enable systick
}

void timer_a0_config()
{
        TIMER_A0->CTL |= TIMER_A_CTL_MC__UP; /*set Mode Control to Up mode(timer counts up to the value of compare
                                         register TAxCCR0, and when it reaches that value, it starts back at 0)
                                            (up mode is MC bits 01 and TIMER_A_CTL_MC__UP represents 0x0010)*/
        TIMER_A0->CTL |= TIMER_A_CTL_SSEL__SMCLK; //select SMCLK as clock source 
        TIMER_A0->EX0 |= TIMER_A_EX0_IDEX__1;//input divider set to /1
        NVIC->ISER[0] |= 0x08; //NVIC Timer A0 enable 
        //Global interrupts enable -- (in main)
        //set to compare mode
}



