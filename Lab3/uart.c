/**
 * Author: Sonya Schuppan
 *
 * File: uart.c
 *
 * Description: defines functions to configure UART module, configure the SysTick timer
 * send bits and bytes of data to the serial terminal, and handle UART interrupts
 *
 * Tools: TI v16.9.3.LTS compiler, XTS JTAG debug probe
 *
 */

#include "msp.h"
#include "uart.h"

void UART_config(void){
    //configuring P1.2 and P1.3 to primary mode (UCA0RXD and UCA0TXD)
    P1->SEL0 |= BIT2 | BIT3;
    P1->SEL1 &= ~(BIT2 | BIT3);
    //(P1->DIR config doesn't matter for this (Datasheet pg 139))

    //configuring UART :)
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST; //enable eUSCI reset (necessary for configuration)
    EUSCI_A0->CTLW0 = 0x0081; //parity disable, LSB first, 8 bit char, 1 stop bit, UART mode, Asynch, BRCLK = SMCLK :), etc (frame config)
    EUSCI_A0->CTLW1 |= EUSCI_A_CTLW1_GLIT_3; //set deglitch time
    EUSCI_A0->BRW = 26; //clock prescaler for baud rate
    EUSCI_A0->MCTLW = 0xB601; // timer settings based on 9600 baud and 4MHz BRCLK (from table)
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST; //reset disable (initialize eUSCI)

    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;//enable receive interrupt
    EUSCI_A0->IFG = 0x0; //reset all flags to 0
    NVIC_EnableIRQ(16); //in tech ref and in msp432p401r.h (ctrl-F IRQn_Type)
}

void UART_send_n(uint8_t * data, uint32_t length){ //data is uint8_t so that pointer increments by 8 bits (1 byte) each time i increments
    uint8_t i;
    for(i = 0; i < length; i++){
        UART_send_byte(*(data+i));
    }
}

void UART_send_byte(uint16_t data){
    while(UCA0_BUSY); //while the UART transmit or receive is busy
    //while(!(EUSCI_A0->IFG & )); //while trans complete interrupt flag is not set
    EUSCI_A0->TXBUF = data;
}

void systick_config(void){
    SYSTICK_STRVR = 0x00FFFFFF; //set to max values since this timer decrements from max to 0 :)
    SYSTICK_STCVR = 0x00; //count value
    SYSTICK_STCSR |= 0x0001; //enable systick
}

void EUSCIA0_IRQHandler(void){ //works when I step through but when I run it gets stuck inside the handler
   if(EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG){ //if RX buffer receives a complete character
       EUSCI_A0->IFG &= ~EUSCI_A_IFG_RXIFG; //clear flag - dont need to do because it automatically does this when it reads the recieive (and same with transmit too)
       EUSCI_A0->TXBUF = EUSCI_A0->RXBUF; //echo
   }
}
