/*
 *
Author: Sonya Schuppan

File: port.h

Description: Declares port configure and Interrupt Service Routine
functions

Tools: TI v16.9.3.LTS compiler, XTS JTAG debug probe
*
*/

#ifndef PORT_H_
#define PORT_H_

void GPIO_configure(void);
void PORT1_IRQHandler(void);

#endif /* PORT_H_ */
