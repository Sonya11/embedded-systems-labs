/**
 * Author: Sonya Schuppan
 *
 * File: main.c
 *
 * Description: callse functions to configure clock, systick timer, and UART; sends AA
 * and "Why not Zoidberg?" to serial terminal
 *
 * Tools: TI v16.9.3.LTS compiler, XTS JTAG debug probe
 *
 */

#include "msp.h"
#include "uart.h"
#include "clock.h"
#include "Lab3.h"
#include <stdlib.h>

#define STR_SIZE 17 //(in bytes)
#define AA_SIZE 1 //(in bytes)

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    uint8_t data = 0xAA; //one byte
    uint8_t str[] = "Why not Zoidberg?"; //17 char * 8 bits (actually 7) per ASCII char to find data type size needed -- make it a pointer/array
	clock_config();
	systick_config();
	UART_config();
	__enable_irq();

	UART_send_n(&data, AA_SIZE);
	ftoa(320.02);
	itoa(124);

	while(1){ //send AA and "Why not Zoidberg?" infinitely
	    //UART_send_n(str, STR_SIZE);
	    UART_send_n(&data, AA_SIZE);
	}
}
