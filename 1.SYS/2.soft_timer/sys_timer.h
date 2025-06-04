/*
********************************************************************************
*    文件名称 : sys_timer.h
*    库 依 赖 : sys.h
*    说    明 : 头文件
*    版    本 : V1.0
*
*    Ver: 1.0    Date: 2025-06-4     Author: xzt
* 
*    Copyright (C), All Rights Reserved.
*    Note    :  1tab = 4 spaces!
********************************************************************************
*/
#ifndef __SYS_TIMER_H__
#define __SYS_TIMER_H__
/******************************* INCLUDES *************************************/
#include "sys.h"
/******************************* INCLUDES *************************************/
/******************************* DEFINES **************************************/
/******************************* DEFINES **************************************/
/***************************** DECLARATIONS ***********************************/
struct _sys_timer;
typedef void (*timer_cb_t)(struct _sys_timer *timer);
typedef struct _sys_timer
{
    uint32_t           ticks       ;
    uint32_t           period      ;
    uint32_t           repeat_count;
    void              *user_data   ;
    timer_cb_t         timer_cb    ;
    struct _sys_timer *next        ;
} sys_timer_t;
/***************************** DECLARATIONS ***********************************/

/*********************************** END **************************************/
#endif /* __SYS_TIMER_H__ */