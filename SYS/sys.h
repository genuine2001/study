/*
********************************************************************************
*
*    文件名称 : sys.h
*    库 依 赖 : stdint.h, stdio.h, stdlib.h
*    说    明 : 头文件声明
*    版    本 : V1.0
*    修改记录 :
*        版本号    日期           作者     说明
*        V1.0     2025-04-05    genuine   第一次创建
*
*    Copyright (C), All Rights Reserved.
*    Note    :  1tab = 4 spaces!
*
********************************************************************************
*/
#ifndef __SYS_H__
#define __SYS_H__

/******************************* INCLUDES *************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
/******************************* INCLUDES *************************************/

/******************************* DEFINES **************************************/

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

void sys_fifo_init(sys_fifo_t *fifo, void *buffer,
                   uint16_t buffer_size, uint16_t element_size);
void sys_fifo_push(sys_fifo_t *fifo, const void *data);
void sys_fifo_push_array(sys_fifo_t *fifo, const void *data, int16_t count);
void sys_fifo_pop(sys_fifo_t *fifo, void *data);
void sys_fifo_pop_array(sys_fifo_t *fifo, void *data, int16_t count);
/***************************** DECLARATIONS ***********************************/
#endif /* SYS_H */