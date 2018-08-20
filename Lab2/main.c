/*
 *
Author: Sonya Schuppan

File: main.c

Description: calls functions to configure ports and timers, enables global interrupts,
and sets a pin high for latency testing.

Tools: TI v16.9.3.LTS compiler, XTS JTAG debug probe
*
*/

#include "msp.h"
#include <stdint.h>
#include "timer.h"
#include "port.h"

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; //disable watchdog

    GPIO_configure(); //configure LED, button, and pin ports
    systick_config(); //configure 
    //timer_a0_config(); //configure timer

    __enable_irq(); //enable global interrupts

    //Latency test: set P1.7 high and add interrupt flag
    /*(Note: setting the flag after the pin was set high will increase
     * the latency time by one cycle*/
    P1->OUT |= (1<<7);
    P1->IFG |= 0x10;

    while (1);
}
