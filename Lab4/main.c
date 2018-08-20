/**
 * Author: Sonya Schuppan
 *
 * File: main.c
 *
 * Description: configures pins, clock, UART, and ADC
 * Initializes buffer, dumps data from buffer and sets up interrupts for button and ADC conversions
 *
 * Tools: TI v16.9.3.LTS compiler, XTS JTAG debug probe
 *
 */

#include "msp.h"
#include "adc_circbuf.h"
#include "lab4.h"
#include "uart.h"
#include "clock.h"
#include "adc.h"
#include "core_cm4.h"

#define SCB_SCR_ENABLE_SLEEPONEXIT 0x00000002


CircBuf * buffer = 0; //declare buffer as global variable so it can be accessed by interrupts
uint8_t buttonFlag = 0;

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	buffer = initializeBuffer(60); // initialize buffer with size of 60 samples (aka 60 sets of 8 bits since parameter of function is uint8_t) -- do we need to change buffer to hold 16 bit data so that it can store temp values with 14 bit resolution?

	pin_config();
	clock_config();
	UART_config();
	ADC_temp_config();
	ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC; //enable conversion, start conversion (Figure 20-8) (reset start conversion back to low in interrupt)

	__enable_irq(); //enable global interrupts
	SCB->SCR &= ~SCB_SCR_ENABLE_SLEEPONEXIT; //wake up ADC on exit from ISR (allows processor to go into sleep mode when the __sleep() function is called) -- allows for a low-power mode

	buffer_check();
	uart_check();


	uint8_t space = 0xA0;
	uint8_t sample[] = "Sample ";
	uint8_t colon[] = ": ";
	uint8_t comma[] = ", ";
	while(1){
	    if(buttonFlag & 1){ //procedure when button is pressed
	        buttonFlag = 0; //reset flag to 0
	        //P2->OUT ^= 0x07; //toggle LED to check that interrupt is working correctly
	        uint8_t i = 0;
	        uint8_t head_check = buffer->head;
	        for(i = 0; i < buffer->num_items; i++){
	            UART_send_n(&sample, 7);
	            itoa(i+1);
	            UART_send_n(&colon,2);
	            itoa(*(buffer->data + head_check));
               if(++head_check > buffer->length-1){
                   head_check = 0;
               }
               UART_send_n(&comma, 2);
	        }
	        clearBuffer(buffer);
	    }
	    //ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC; //enable conversion, start conversion (Figure 20-8) (reset start conversion back to low in interrupt)
	    __sleep(); // blocks until conversion is finished -- (puts processor into a low power mode until interrupt happens -- when interrupt completes whte processor will wake upt its main thread aka the code at the point where __sleep() is called and execute the lines after that)
	    __no_operation(); //used for breakpoint

	    //)add temp data to buffer every 1 second for up to 1 minute in handler)
	}
}
