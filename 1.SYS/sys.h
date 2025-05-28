/*
********************************************************************************
*    文件名称 : sys.h
*    库 依 赖 : stdint.h, stdio.h, stdlib.h, string.h, sys_conf.h
*    说    明 : 头文件声明
*
*    Ver: 1.0    Date: 2025-01-13     Author: xzt
*
*    Copyright (C), All Rights Reserved.
*    Note    :  1tab = 4 spaces!
********************************************************************************
*/
#ifndef __SYS_H__
#define __SYS_H__
/******************************* INCLUDES *************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sys_conf.h"
/******************************* INCLUDES *************************************/
/******************************* DEFINES **************************************/
#if USE_PERF_COUNTER
    #include "perf_counter.h"
#endif
/******************************* DEFINES **************************************/
/***************************** DECLARATIONS ***********************************/
typedef struct 
{
    void             *buffer;  /* pointer of buffer */
    uint16_t     buffer_size;  /* buffer size       */
    uint16_t    element_size;  /* element size      */
    uint32_t            head;  /* head of buffer    */
    uint32_t            tail;  /* tail of buffer    */
} sys_fifo_t;
/***************************** DECLARATIONS ***********************************/
/****************************** The APIs of FIFO ******************************/
/* First API: init the fifo                                    */
void sys_fifo_init(sys_fifo_t *fifo, void *buffer,
                   uint16_t buffer_size, uint16_t element_size);
                      
/* Second API: push data into the fifo                         */
void sys_fifo_push(sys_fifo_t *fifo, const void *data);

/* Third API: push multiple data into the fifo                 */
void sys_fifo_push_array(sys_fifo_t *fifo, const void *data, int16_t count);

/* Fourth API: pop data from the fifo                          */
void sys_fifo_pop(sys_fifo_t *fifo, void *data);

/* Fifth API: pop multiple data from the fifo                  */
void sys_fifo_pop_array(sys_fifo_t *fifo, void *data, int16_t count);
/****************************** The APIs of FIFO ******************************/
#endif /* __SYS_H__ */
