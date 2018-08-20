/*
 *
Author: Sonya Schuppan

File: timer.h

Description: Declares functions for timer configurations and an Interrupt
Service Routine for Timer_A0, and defines SysTick Timer macros.

Tools: TI v16.9.3.LTS compiler, XTS JTAG debug probe
*
*/

#ifndef TIMER_H_
#define TIMER_H_
/*
void timer_a0_config(); //configures timer
void ISR-FUNCTION-TIMER();
*/
void systick_config();

#define SYSTICK_STCSR *(uint32_t*) 0xE000E010
#define SYSTICK_STCVR *(uint32_t*) 0xE000E018
#define SYSTICK_STRVR *(uint32_t*) 0xE000E014
#define SYSTICK_STCR *(uint32_t*) 0xE000E01C

#endif /* TIMER_H_ */
