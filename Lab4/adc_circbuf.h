/**
 * Author: Sonya Schuppan
 *
 * File: lab4.c
 *
 * Description: defines buffer struct and declares buffer functions 
 *
 * Tools: TI v16.9.3.LTS compiler, XTS JTAG debug probe
 *
 */

#ifndef ADC_CIRCBUF_H_
#define ADC_CIRCBUF_H_
#include "msp.h"

typedef struct CircBuf_t{
    volatile uint8_t head; // first item
    volatile uint8_t tail; // last item
    volatile uint8_t num_items; // Number of items in buffer
    volatile uint8_t length; // Max number of items the buffer can hold
    volatile uint16_t * data; // Pointer to beginning of buffer in heap
} CircBuf;

CircBuf * initializeBuffer(uint8_t length);
void clearBuffer(CircBuf * buf);
void deleteBuffer(CircBuf * buf);
int8_t bufferFull(CircBuf * buf);
int8_t BufferEmpty(CircBuf * buf);
void addItemToBuffer(CircBuf * buf, uint16_t item);
uint16_t removeItemFromBuffer(CircBuf * buf);

#endif /* ADC_CIRCBUF_H_ */
