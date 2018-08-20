/*
 * circbuf.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ryan
 */

#ifndef CIRCBUF_H_
#define CIRCBUF_H_
#include "msp.h"
#include "stdlib.h"

typedef struct Circbuf_t{           // Type define our new circular buffer
    uint8_t * base;                 // Pointer to base of the buffer in the heap
    volatile uint8_t * head;        // Pointer to first entered item
    volatile uint8_t * tail;        // Pointer to last entered item
    uint8_t num_items;              // Number of items currently in buffer
    uint8_t length;                 // Max number of items the buffer can hold
} Circbuf;

typedef enum {
    CB_NO_ERROR = 0,
    CB_FULL = 1,
    CB_NOT_FULL = 0,
    CB_EMPTY = 1,
    CB_NOT_EMPTY = 1,
    CB_NULL = 1,
} CB_STATUS;

/**********************************
 *   CB_initialize_buffer()
 *
 * Initialize a circular buffer of byte
 * size length. Allocate memory from
 * the heap to create our buffer.
 *********************************/
Circbuf * CB_initialize_buffer(uint8_t length);

/**********************************
 *      CB_is_buffer_empty()
 *
 * Check to see if our circular buffer
 * is empty. We return a nonzero if the
 * buffer is empty and zero
 * if the buffer is not empty.
 *********************************/
int8_t CB_is_buffer_empty(Circbuf * buf);

/**********************************
 *      CB_is_buffer_full()
 *
 * Check to see if our circular buffer
 * is full. We return a nonzero
 * if the buffer is full and
 * zero if the buffer is not empty
 *********************************/
int8_t CB_is_buffer_full(Circbuf * buf);

/***********************************
 *       CB_delete_buffer()
 *
 * Free our circular buffer buf from
 *            the heap
 **********************************/

CB_STATUS CB_delete_buffer(Circbuf * buf);

/***********************************
 *        CB_clear_buffer()
 *
 * This function clears all items in
 * our buffer to zero and resets the
 * head and tail at the base of buf
 **********************************/
CB_STATUS CB_clear_buffer(Circbuf * buf);

/***********************************
 *   CB_remove_item_from_buffer()
 *
 * This function removes the first
 * item in the buffer and puts the
 * value into (uint8_t * item)
 **********************************/
int8_t CB_remove_item_from_buffer(Circbuf *buf, uint8_t * item);

/***********************************
 *     CB_add_item_to_buffer()
 *
 * This function adds an item,
 * (uint8_t * item), into the buffer
 * at the location of the head.
 **********************************/
int8_t CB_add_item_to_buffer(Circbuf * buf, uint8_t item);

#endif /* CIRCBUF_H_ */
