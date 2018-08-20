/*
 *
Author: Sonya Schuppan

File: port.c

Description: Configures 2 LEDs, a latency test output pin, and right and
left buttons. Enables interrupts for the two buttons.
Defines the Port 1 Interrupt Service Routine. If the left button is pushed,
the Red LED (P1.0) will toggle and the RGB LED will change colors in one 
direction. If the right button is pushed, the RGB LED will change colors in 
the opposite direction. The pin P1.7 will also be set high inside the ISR for
latency testing.

Tools: TI v16.9.3.LTS compiler, XTS JTAG debug probe
*
*/

#include "port.h"
#include "msp.h"

void GPIO_configure(void)
{
    //Red LED Configuration (P1.0)
    P1->SEL0 &= ~0x01;
    P1->SEL1 &= ~0x01;
    P1->DIR |= 0x01;
    P1->OUT |= 0x01;
    //RGB LED Configuration (P2.0,P2.1,P2.2)
    P2->SEL0 &= ~0x07;
    P2->SEL1 &= ~0x07;
    P2->DIR |= 0x07;
    P2->OUT &= ~0x07;

    //Left button configure (P1.1)
    P1->SEL0 &= ~0x02; //General I/O mode for button
    P1->SEL1 &= ~0x02;
    P1->DIR &= ~0x02; //set as input
    P1->REN |= 0x02; //enable pullup/pulldown resistor
    P1->OUT |= 0x02; //enable specifically the pullup resistor
    P1->IFG &= ~0x02; //remove pending interrupt flag(always clear flag before enabling it or else an interrupt might occur right when it is enabled :) )
    P1->IES |= 0x02; //high to low trigger type
    P1->IE |= 0x02; //interrupt enabled for corresponding port

    //Right button configure (P1.4)
    P1->SEL0 &= ~0x10;
    P1->SEL1 &= ~0x10;
    P1->DIR &= ~0x10;
    P1->REN |= 0x10;
    P1->OUT |= 0x10;
    P1->IFG &= ~0x10;
    P1->IES |= 0x10;
    P1->IE |= 0x10;

    //Configure Latency Test Output Pin (P1.7)
    P1->SEL0 &= ~(1 << 7);
    P1->SEL1 &= ~(1 << 7);
    P1->DIR |= (1 << 7);
    P1->OUT &= ~(1 << 7);

    //NVIC_EnableIRQ(PORT1_IRQn); //function inside header file(does same thing as line below) //IRQn_Type->PORT1_IRQn = bit 35 
    NVIC->ISER[1] |= 0x08; //enable Port 1 Interrupt in the NVIC (Interrupt IRQn #35)
}
void PORT1_IRQHandler (void) //Interrupt Service Routine
{
    P1->OUT ^= (1 << 7); //toggle when either of the button interrupts are flagged
    if(P1->IFG & 0x02){
        P1->IFG &= ~0x02; //clear flag :) 
        //uint32_t i = 0;
        //for (i = 0; i < 300000; i++); //for loop delay
        P1->OUT ^= 0x01;
        //color change of RGB LED:
        if((P2->OUT & 0x07) == 0x00){ //if 1st 3 LSBs are all 0, then change back to all 1s
            P2->OUT |= 0x07;
        }
        else{
            P2->OUT -= 1;
        }
    }
    if(P1->IFG & 0x10){
        P1->IFG &= ~0x10; //clear flag :)
        if((P2->OUT & 0x07) == 0x07){ //if 1st 3 LSBs are all 1, then change back to all 0s
            P2->OUT &= ~0x07;
        }
        else{
            P2->OUT += 1; //run through colors
        }
    }
}
