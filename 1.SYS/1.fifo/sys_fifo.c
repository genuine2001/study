/*
********************************************************************************
*    文件名称 : sys_fifo.c
*    库 依 赖 : sys_fifo.h
*    说    明 : 基于FIFO的消息队列实现
*
*    Ver: 1.0    Date: 2025-01-13     Author: xzt
*    
*    Copyright (C), All Rights Reserved.
*    Note    :  1tab = 4 spaces!
********************************************************************************
*/
/******************************* INCLUDES *************************************/
#include "sys_fifo.h"
/******************************* INCLUDES *************************************/
/******************************* DEFINES **************************************/
#if USE_PERF_COUNTER == 0
#define MIN(a, b)               ((a) < (b) ? (a) : (b))   
#endif 

#define FIFO_HEAD(fifo)         ((fifo)->head & (fifo)->buffer_size - 1)
#define FIFO_TAIL(fifo)         ((fifo)->tail & (fifo)->buffer_size - 1)
/******************************* DEFINES **************************************/
/***************************** DECLARATIONS ***********************************/
/***************************** DECLARATIONS ***********************************/

/*******************************************************************************
* @brief init the fifo struct
*
* @param fifo: pointer of the fifo struct
* @param buffer: pointer of the buffer
* @param buffer_size: buffer size
* @param element_size: element size
*
* @note the buffer size should be an integer power of 2
*******************************************************************************/
void sys_fifo_init(sys_fifo_t *fifo, void *buffer,
                   uint16_t buffer_size, uint16_t element_size)
{
    fifo->buffer = buffer;
    fifo->buffer_size = buffer_size;
    fifo->element_size = element_size;
    fifo->head = 0;
    fifo->tail = 0;
}

/*******************************************************************************
* @brief push data to the fifo
*
* @param fifo: pointer of the fifo struct
* @param data: pointer of the data to be pushed
*
* @return None
*******************************************************************************/
void sys_fifo_push(sys_fifo_t *fifo, const void *data)
{
    /*	step1,	check if the fifo is full	       */
    if (FIFO_IS_FULL(fifo))
    {
        return;
    }
  
    /*	step2,	calculate the write position       */
    uint8_t *buffer = (uint8_t *)fifo->buffer;
    buffer += (FIFO_HEAD(fifo) * fifo->element_size);

    /*	step3,	write the data to the buffer      */
    memcpy(buffer, data, fifo->element_size);

    /*	step4,	update the head pointer		      */
    fifo->head++;
}

/*******************************************************************************
* @brief pop data from the fifo
*
* @param fifo: pointer of the fifo struct
* @param data: pointer of the data to be popped
*
* @return None
*******************************************************************************/
void sys_fifo_pop(sys_fifo_t *fifo, void *data)
{
    /*	step1,	check if the fifo is empty	       */
    if (FIFO_IS_EMPTY(fifo))
    {
        return;
    }

    /*	step2,	calculate the read position        */
    uint8_t *buffer = (uint8_t *)fifo->buffer;
    buffer += (FIFO_TAIL(fifo) * fifo->element_size);

    /*	step3,	read the data from the buffer      */
    memcpy(data, buffer, fifo->element_size);

    /*	step4,	update the tail pointer		       */
    fifo->tail++;
}

#if FIFO_USE_MULTI
/*******************************************************************************
* @brief push multiple data into the fifo
*
* @param fifo: pointer of the fifo struct
* @param data: pointer of the data to be pushed
* @param count: the number of data to be pushed
*
* @return None
*******************************************************************************/
void sys_fifo_push_array(sys_fifo_t *fifo, const void *data, int16_t count)
{
    /*	step1,	check if the fifo is full	          */
    if (FIFO_IS_FULL(fifo) || count == 0)
    {
        return;
    }

    /*	step2,	Calculate the segment lengths         */
    int16_t cnt1, cnt2;
    cnt1 = cnt2 = FIFO_GET_FREE(fifo);
    if(cnt1+FIFO_HEAD(fifo) > fifo->buffer_size)
    {
        cnt1 = fifo->buffer_size - FIFO_HEAD(fifo);
    }
    cnt2 -= cnt1;

    cnt1 = MIN(cnt1, count);
    count -= cnt1;

    cnt2 = MIN(cnt2, count);

    /*	step3,	write the first piece of data         */
    uint8_t *buffer = (uint8_t *)fifo->buffer;
    buffer += (FIFO_HEAD(fifo) * fifo->element_size);
    memcpy(buffer, data, cnt1 * fifo->element_size);
    fifo->head += cnt1;
    
    /*	step4,	check there is any data left          */
    if(count <= 0)
    {
        return;
    }

    /*	step5,	write the second piece of data        */
    buffer = (uint8_t *)fifo->buffer;
    data = (const uint8_t *)data + cnt1 * fifo->element_size;
    memcpy(buffer, data, cnt2 * fifo->element_size);
    fifo->head += cnt2;
}

/*******************************************************************************
* @brief pop multiple data from the fifo
*
* @param fifo: pointer of the fifo struct
* @param data: pointer of the data to be popped
* @param count: the number of data to be popped
*
* @return None
*******************************************************************************/
void sys_fifo_pop_array(sys_fifo_t *fifo, void *data, int16_t count)
{
    /*	step1,	check if the fifo is empty	       */
    if (FIFO_IS_EMPTY(fifo) || count == 0)
    {
        return;
    }

    /*	step2,	Calculate the segment lengths      */
    int16_t cnt1, cnt2;
    cnt1 = cnt2 = FIFO_DATA_COUNT(fifo);
    if(cnt1+FIFO_TAIL(fifo) > fifo->buffer_size)
    {
        cnt1 = fifo->buffer_size - FIFO_TAIL(fifo);
    }
    cnt2 -= cnt1;

    cnt1 = MIN(cnt1, count);
    count -= cnt1;

    cnt2 = MIN(cnt2, count);

    /*	step3,	read the first piece of data       */
    uint8_t *buffer = (uint8_t *)fifo->buffer;
    buffer += (FIFO_TAIL(fifo) * fifo->element_size);
    memcpy(data, buffer, cnt1 * fifo->element_size);
    fifo->tail += cnt1;
    
    /*	step4,	check there is any data left       */
    if(count <= 0)
    {
        return;
    }

    /*	step5,	read the second piece of data      */
    buffer = (uint8_t *)fifo->buffer;
    data = (uint8_t *)data + cnt1 * fifo->element_size;
    memcpy(data, buffer, cnt2 * fifo->element_size);
    fifo->tail += cnt2;
}
#endif
/*********************************** END **************************************/
