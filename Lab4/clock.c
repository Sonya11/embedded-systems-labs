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
    //UART clock configs
    CS->KEY = 0x695A; //unlock CS module for register access (CS = Clock Source)
    CS->CTL0 = 0; //*reset tuning parameters*
    CS->CTL0 |= 0x0001008D; //set up DCO clock to make as close to 4MHz as possible 
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3; //Select ACLK = REFOCLK, SMCLK = MCLK = DCO
    CS->CTL1 |= 0x00200000; //dividing HSMCLK to divide ADC clock more
    CS->KEY = 0; //lock CS module for register access
    //output SMCLK freq to pin to tune freq using oscilliscope
    //P7.0 primary mode is PM_SMCLK (SMCLK output)
    P7->SEL0 |= BIT0;
    P7->SEL1 &= ~BIT0;
    P7->DIR |= BIT0; //output

    //P4.2 primary mode is ACLK output
    P4->SEL0 |= BIT2;
    P4->SEL1 &= ~BIT2;
    P4->DIR |= BIT2; //output
}
