/**
 * Author: Sonya Schuppan
 *
 * File: Lab3.c
 *
 * Description: defines functions for type conversions
 *
 * Tools: TI v16.9.3.LTS compiler, XTS JTAG debug probe
 *
 */

#include <stdlib.h>
#include "Lab3.h"
#include "msp.h"

void itoa(uint16_t integer){
    UART_send_byte(((integer % 1000)/100)+48); //decimal ascii val for hundreds place (because integer division always rounds down so number/100 will be 1 if number = 145, and 48 is the decimal offset for ascii numbers (ascii 0 is decimal 48)
    UART_send_byte(((integer % 100)/10)+48); //decimal ascii val for tens place (because number % 100 will be between 0 and 99 so it doesnt care about hundreds place
    UART_send_byte(((integer % 10)/1)+48); //decimal ascii val for ones place (number % 10 will only care about ones place)
}

void ftoa(float number){
    uint16_t integer = (uint16_t)number;
    uint16_t decimal = (number - integer)*1000;

    UART_send_byte(((integer % 1000)/100)+48); //decimal ascii val for hundreds place (because integer division always rounds down so number/100 will be 1 if number = 145, and 48 is the decimal offset for ascii numbers (ascii 0 is decimal 48)
    UART_send_byte(((integer % 100)/10)+48); //decimal ascii val for tens place (because number % 100 will be between 0 and 99 so it doesnt care about hundreds place
    UART_send_byte(((integer % 10)/1)+48); //decimal ascii val for ones place (number % 10 will only care about ones place)
    UART_send_byte(0x2E); //decimal point
    UART_send_byte(((decimal % 1000)/100)+48);
    UART_send_byte(((decimal % 100)/10)+48);
    UART_send_byte(((decimal % 10)/1)+48);

}

