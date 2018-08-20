/*
 *
Author: Sonya Schuppan
File: timer.c
Description:
Tools: TI v16.9.3.LTS compiler, XTS JTAG debug probe
*
*/

#include "timer.h"
#include "msp.h" //Remember to include this early! (what allows you to hover over the macors to see if they are linked)

void systick_config(){
    SYSTICK_STRVR = 0x00FFFFFF; //set to max values since this timer decrements from max to 0 :)
    SYSTICK_STCVR = 0x00; //count value
    SYSTICK_STCSR |= 0x0001; //enable systick
}

void timer_a0_config()
{
    /*Ctrl-F in msp432p401r.h for "TIMER_A_CTL[IFG] Bits" for find the masks used below
                (it lists all timer A bits and you can differentiate the registers based on the bit values (ie the
                CTL and EX0 Registers???) */
        //Ctrl-F in msp432p401r.h for TIMER_A0 and go through all "Finds" to remember how it is all linked together
        TIMER_A0->CTL |= TIMER_A_CTL_MC__UP; /*set Mode Control to Up mode(timer counts up to the value of compare
                                         register TAxCCR0, and when it reaches that value, it starts back at 0)
                                            (up mode is MC bits 01 and TIMER_A_CTL_MC__UP represents 0x0010)*/
        TIMER_A0->CTL |= TIMER_A_CTL_SSEL__SMCLK; //select SMCLK as clock source -- why SMCKL?
        TIMER_A0->EX0 |= TIMER_A_EX0_IDEX__1;//input divider set to /1
        //TIMER_A0->CCTL |= TIMER_A_CCTLN_CCIE; //Capture/Compare Interrupt enable
        //TIMER_A0->CCTL &= ~TIMER_A_CCTLN_CCIFG;//clear any pending timer (Capture/Compare)interrupt flags (DO THIS BEFORE YOU ENABLE INTERRUPT)
        //????Capture/Compare timer interrupt enabled at the peripheral????
        NVIC->ISER[0] |= 0x08; //NVIC Timer A0 enable -- bit 8 of ISER0? (***find exact bit number in near top of mspjfkdsfhsjfhs.h***)   alternative way: NVIC_EnableIRQ(TA0_0_IRQn);?
        //Global interrupts enable -- (in main)
        //set to compare mode

        //??? Do we need to set rising or falling edge for the capture? (Ref Manual PG 609)
}



