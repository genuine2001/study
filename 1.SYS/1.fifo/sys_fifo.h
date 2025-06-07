/*
********************************************************************************
*    文件名称 : sys_fifo.h
*    库 依 赖 : sys.h
*    说    明 : 头文件声明
*
*    Ver: 1.0    Date: 2025-01-13     Author: xzt
*
*    Copyright (C), All Rights Reserved.
*    Note    :  1tab = 4 spaces!
********************************************************************************
*/
#ifndef __SYS_FIFO_H__
#define __SYS_FIFO_H__
/******************************* INCLUDES *************************************/
#include "sys.h"
/******************************* INCLUDES *************************************/
/******************************* DEFINES **************************************/
#define FIFO_DATA_COUNT(fifo)   ((fifo)->head - (fifo)->tail)
#define FIFO_IS_EMPTY(fifo)     ((fifo)->head == (fifo)->tail)
#define FIFO_IS_FULL(fifo)      (FIFO_DATA_COUNT(fifo) == (fifo)->buffer_size)
#define FIFO_GET_FREE(fifo)     ((fifo)->buffer_size - FIFO_DATA_COUNT(fifo))
/******************************* DEFINES **************************************/
/***************************** DECLARATIONS ***********************************/
typedef struct 
{
    void                  *buffer;  /* pointer of buffer */
    uint16_t          buffer_size;  /* buffer size       */
    uint16_t         element_size;  /* element size      */
    volatile uint32_t        head;  /* head of buffer    */
    volatile uint32_t        tail;  /* tail of buffer    */
} sys_fifo_t;
/***************************** DECLARATIONS ***********************************/
/****************************** The APIs of FIFO ******************************/
/* First API: init the fifo                                    */
void sys_fifo_init(sys_fifo_t *fifo, void *buffer,
                   uint16_t buffer_size, uint16_t element_size);
                      
/* Second API: push data into the fifo                         */
void sys_fifo_push(sys_fifo_t *fifo, const void *data);

/* Third API: pop data from the fifo                          */
void sys_fifo_pop(sys_fifo_t *fifo, void *data);

#if FIFO_USE_MULTI
/* Fourth API: push multiple data into the fifo               */
void sys_fifo_push_array(sys_fifo_t *fifo, const void *data, int16_t count);

/* Fifth API: pop multiple data from the fifo                 */
void sys_fifo_pop_array(sys_fifo_t *fifo, void *data, int16_t count);
#endif 
/****************************** The APIs of FIFO ******************************/
#endif /* __SYS_FIFO_H__ */
