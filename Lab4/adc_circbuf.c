/**
 * Author: Sonya Schuppan
 *
 * File: adc_circbuf.c
 *
 * Description: defines functions for the buffer: initialize, delete, clear, if full, if empty, add item, and remove item
 *
 * Tools: TI v16.9.3.LTS compiler, XTS JTAG debug probe
 *
 */

#include "adc_circbuf.h"
#include <stdlib.h>
#include "msp.h"
#include "lab4.h"

CircBuf * initializeBuffer(uint8_t length){
    CircBuf * buf = (CircBuf *)malloc(sizeof(CircBuf));
    buf->data = (uint16_t *)malloc(length);
    buf->length = length;
    buf->head = 0;
    buf->num_items = 0;
    buf->tail = 0;
    LED_config(); //configure LED flag for full buffer
    button_config(); //configure button dumping data through UART to screen
    return buf;
}

void deleteBuffer(CircBuf * buf){
    free((void *)buf->data);
    free(buf);
    P2->OUT &= ~0x07; //turn LED off
}

void clearBuffer(CircBuf * buf){
    uint8_t i = 0;
    for (i = 0; i < buf->length; i++) buf->data[i] = 0;
    buf->head = 0;
    buf->num_items = 0;
    buf->tail = 0;
    P2->OUT &= ~0x07; //turn LED off
}

int8_t bufferFull(CircBuf * buf){
    return (buf->num_items >= buf->length);
}

int8_t bufferEmpty(CircBuf * buf){
    return (buf->num_items < 1);
}

void addItemToBuffer(CircBuf * buf, uint16_t item){ //add "if full" check in main (if full print that queue is full to terminal)
    //(if buffer is full, it will start overwriting oldest data)
    buf->data[buf->tail] = item;
    buf->tail = (++buf->tail) % buf->length; //(buf->tail++ & buf->length;)
    if(buf->num_items < buf->length) buf->num_items++;
    else buf->head = (++buf->head) % buf->length;
    if(bufferFull(buf)){
        P2->OUT |= 0x05;  //turn LED on
    }
}

uint16_t removeItemFromBuffer(CircBuf * buf){ //add "if empty" checks in main
    if(!bufferEmpty(buf)){ //if buffer is not empty
        uint16_t item = buf->data[buf->head];
        buf->head++;
        buf->head %= buf->length; //((buf->head++) & buf->length;)
        buf->num_items--;

        P2->OUT &= ~0x07; //turn LED off
        return item;
    }
    else{ //if empty
        return 255; //for debugging purposes (b/c NULL is just 0 so that doesnt work)
    }
}
