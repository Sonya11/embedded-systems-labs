/**
 * Author: Sonya Schuppan
 *
 * File: uart.h
 *
 * Description: declares functions to configure UART module, configure the SysTick timer
 * send bits and bytes of data to the serial terminal, and handle UART interrupts; defines
 * macros for UART and SysTick
 *
 * Tools: TI v16.9.3.LTS compiler, XTS JTAG debug probe
 *
 */
#include "msp.h"

#ifndef UART_H_
#define UART_H_

#define UCA0_BUSY (EUSCI_A0->STATW & 0x01)
#define TX_INTERRUPT BIT1
#define RX_INTERRUPT BIT0

#define SYSTICK_STCSR *(uint32_t*) 0xE000E010
#define SYSTICK_STCVR *(uint32_t*) 0xE000E018
#define SYSTICK_STRVR *(uint32_t*) 0xE000E014
#define SYSTICK_STCR *(uint32_t*) 0xE000E01C

void systick_config(void);
void UART_config(void);
void UART_send_n(uint8_t * data, uint32_t length);
void UART_send_byte(uint16_t data);
void EUSCIA0_IRQHandler(void); //registering UART interrupt handler

#endif /* UART_H_ */
