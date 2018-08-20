/**
 * Author: Sonya Schuppan
 *
 * File: lab4.h
 *
 * Description:
 *
 * Tools: TI v16.9.3.LTS compiler, XTS JTAG debug probe
 *
 */

#include "adc_circbuf.h"

#ifndef LAB4_H_
#define LAB4_H_

void ftoa(float number);
void itoa(volatile uint32_t integer);
void buffer_check();
void uart_check();
void pin_config();
void LED_config();
void button_config();
void PORT1_IRQHandler(void);

#endif /* LAB4_H_ */
