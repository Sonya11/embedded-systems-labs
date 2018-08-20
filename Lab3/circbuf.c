/*
 * circbuf.c 
 *
 *  Created on: Sep 26, 2017
 *      Author: ryan
 */

#include "msp.h"
#include <stdlib.h>
#include "circbuf.h"


Circbuf * CB_initialize_buffer(uint8_t length)
{
   Circbuf * buf = (Circbuf *)malloc(sizeof(Circbuf));   // Allocate our buffer on the heap to relieve the stack
   buf->base = (uint8_t *) malloc(length);               // Allocate heap memory for our data
   buf->length = length;                                 // Initialize all variables
   buf->head = buf->base;                                // ...
   buf->tail = buf->base;                                // ...
   buf->num_items = 0;                                   // ...
   return buf;                                           // Return indicating no errors occurred
}                                                        // Do not check malloc here

CB_STATUS CB_clear_buffer(Circbuf * buf)
{
    if(!buf || !buf->base)      // If we do not have a buffer, return an error
        {return CB_NULL;}

    uint8_t i;
    buf->head = buf->base;                     // Set the head at the base
    buf->tail = buf->base;                     // Set the tail at the base

/* Increment our tail pointer, setting buffer values
 *    to zero until we hit the end of our buffer!   */
    for(i = 0; i < buf->length; i++)
    {
        *buf->tail = 0;
        buf->tail++;
    }

    buf->tail = buf->base;        // reset our base at the bottom of our buffer
    buf->num_items = 0;           // Initialize our number of items to zero

    return CB_NO_ERROR;
}

CB_STATUS CB_delete_buffer(Circbuf * buf)
{
    free((void *)buf->base);        // Free our allocated memory!
    free((void *)buf);
    return CB_NO_ERROR;             // Return no error
}

int8_t CB_is_buffer_full(Circbuf * buf)
{
    if(!buf || !buf->base)              // If we do not have a buffer, return an error
        {return CB_NULL;}

// If we have items equal to the length of our buffer, we're full
        return (buf->length & ~buf->num_items);
}


int8_t CB_is_buffer_empty(Circbuf * buf)
{
    if(!buf || !buf->base)  // If we do not have a circular buffer, return an error
        {return CB_NULL;}

// If our number of items is non-zero, then were not empty!//
        return (buf->num_items & 0xFF);
}


int8_t CB_add_item_to_buffer(Circbuf * buf, uint8_t item)
{
    if(!buf || !buf->base)      // If we do not have a buffer, return an error
        {return CB_NULL;}
    if(!CB_is_buffer_full(buf))  // Check that our buffer is not full before adding
        {return CB_FULL;}

    *buf->head = item;          // Add item to our buffer
    buf->num_items++;

    if(buf->head == buf->base + (buf->length - 1)) // If our head is at the end of the buffer, our head goes back to the base
        {buf->head = buf->base;}

    else buf->head++;                              // Increment our head if not at end of buffer

    return CB_NO_ERROR;
}

int8_t CB_remove_item_from_buffer(Circbuf *buf, uint8_t * item)
{
    if(!buf || !buf->base)      // If we do not have a buffer, return an error
        {return CB_NULL;}
    if(!CB_is_buffer_empty(buf))                             // Check if our buffer is empty
        {return CB_EMPTY;}

    *item = *buf->tail;                                      // Remove data from our buffer!
    buf->num_items--;

    if(buf->tail == buf->base + (buf->length -1))           // If at the end of our buffer, move back to the base
        {buf->tail = buf->base;}

    else buf->tail++;                                       // Otherwise we increment our tail!

   return CB_NO_ERROR;
}
