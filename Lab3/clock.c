/**
 * Author: Sonya Schuppan
 *
 * File: clokc.c
 *
 * Description: defines function to configure clock
 *
 * Tools: TI v16.9.3.LTS compiler, XTS JTAG debug probe
 *
 */
#include "msp.h"
#include "clock.h"

void clock_config(void){
    CS->KEY = 0x695A; //unlock CS module for register access (CS = Clock Source)
    CS->CTL0 = 0; //*reset tuning parameters*
    CS->CTL0 |= 0x0001008D; //(0x00010092)(CS_CTL0_DCORSEL_1) set up DCO clock(eventually change tuning bits(the last bits) to make it as close to 4MHz as possible (that is the frequency with the closest errors in the clock setup table for UART) - right now it is 0)(2-4 MHz is chosen because it has more "resolution" than 4-8 since there is less of a range)
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3; //Select ACLK = REFOCLK, SMCLK = MCLK = DCO
    CS->KEY = 0; //lock CS module for register access

    //output SMCLK freq to pin to tune freq using oscilliscope
    //P7.0 primary mode is PM_SMCLK (SMCLK output)
    P7->SEL0 |= BIT0;
    P7->SEL1 &= ~BIT0;
    P7->DIR |= BIT0; //output
}
