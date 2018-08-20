/**
 * Author: Sonya Schuppan
 *
 * File: lab4.c
 *
 * Description: defines functions for converting integers and floats to ASCII, defines functions to check UART and buffer functionalities,
 * defines button, UART, and LED configurations, defines the interrupt handler for port 1
 *
 * Tools: TI v16.9.3.LTS compiler, XTS JTAG debug probe
 *
 */

#include "msp.h"
#include "lab4.h"
#include "adc_circbuf.h"
#include "uart.h"

extern uint8_t buttonFlag;

void itoa(volatile uint32_t integer){ //volatile to make compatible with buffer structure elements
    UART_send_byte(((integer % 100)/10)+48); //converts to ascii value based on ascii integer offset
    UART_send_byte(((integer % 10)/1)+48);
}

void ftoa(float number){
    //seperates the numbers before and after the decimal point
    uint8_t integer = (uint8_t)number;
    uint8_t decimal = (number - integer)*1000;

    UART_send_byte(((integer % 1000)/100)+48); //converts to ascii value based on ascii integer offset
    UART_send_byte(((integer % 100)/10)+48);
    UART_send_byte(((integer % 10)/1)+48);
    UART_send_byte(0x2E); //decimal point
    UART_send_byte(((decimal % 1000)/100)+48);
    UART_send_byte(((decimal % 100)/10)+48);
    UART_send_byte(((decimal % 10)/1)+48);

}

void uart_check(){ //checks that UART is working correctly
    uint8_t data = 0xAA;
    UART_send_n(&data, 1);
    ftoa (9.9);
    UART_send_n(&data, 1);
    ftoa(320.02);
    UART_send_n(&data, 1);
    itoa(124);
    UART_send_n(&data, 1);
}

void buffer_check(){
    CircBuf * buf = initializeBuffer(3); //small enough to check that circular functionality works using code below

    //checking buffer functionalities :)
    uint8_t removed = 0;

    addItemToBuffer(buf, 2);
    addItemToBuffer(buf, 3);
    addItemToBuffer(buf, 4);

    removed = removeItemFromBuffer(buf); //doesn't actually remove data, just moves head over so that that value is allowed to get overwritten (which makes sense)
    removed = removeItemFromBuffer(buf);

    addItemToBuffer(buf, 5); //(???)the wrapping functionality doesn't work here with the & method -- so switched to modulo method and now it works (look at notebook)
    addItemToBuffer(buf, 6);

    clearBuffer(buf); //this actually removes the data and sets the head, tail, and num_items back to 0

    addItemToBuffer(buf, 2);
    addItemToBuffer(buf, 3);
    addItemToBuffer(buf, 4);

    deleteBuffer(buf);
}

void LED_config(){
    //RGB LED Configuration (P2.0,P2.1,P2.2)
    P2->SEL0 &= ~0x07;
    P2->SEL1 &= ~0x07;
    P2->DIR |= 0x07;
    P2->OUT &= ~0x07; //keep LED turned off to start
}

void button_config(){
   //Right button configure (P1.4)
   P1->SEL0 &= ~0x10; //general IO mode
   P1->SEL1 &= ~0x10;
   P1->DIR &= ~0x10; //input direction
   P1->REN |= 0x10; //enable pullup/pulldown resistor
   P1->OUT |= 0x10; //making resistor a pullup
   P1->IES |= 0x10; //high to low trigger type
   P1->IFG = 0x0; //clear ALL flags before interrupt enable so that no interrupts occur immediately when interrupt is enabled
   P1->IE |= 0x10; //enable interrupt

   NVIC->ISER[1] |= 0x08; //enable Port 1 Interrupt in the NVIC (Interrupt IRQn #35)
}

void pin_config(){
    P1->SEL0 &= ~(1 << 7);
    P1->SEL1 &= ~(1 << 7);
    P1->DIR |= (1 << 7); //out
    P1->OUT &= ~(1 << 7); //off
}

void PORT1_IRQHandler (void){
    //(since the button is the only interrupt in port 1 that we are using, we do not currently need if statements to check which flag is set)
    P1->IFG &= ~0x10; //clear flag
    //P2->OUT ^= 0x07; //toggle LED to check that interrupt is working correctly
    buttonFlag = 1; //set flag to dump buffer data in main
}

