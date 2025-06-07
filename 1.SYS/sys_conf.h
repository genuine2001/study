/*
********************************************************************************
*    文件名称 : sys_conf.h
*    说    明 : 控制文件
*
*    Ver: 1.0    Date: 2025-05-28     Author: xzt
*
*    Copyright (C), All Rights Reserved.
*    Note    :  1tab = 4 spaces!
********************************************************************************
*/
#ifndef __SYS_CONF_H__
#define __SYS_CONF_H__

#define USE_FIFO                            1
#if USE_FIFO
    #define FIFO_USE_MULTI                  0
#endif

#define USE_TIMER                           1  
#if USE_TIMER
    #define TIMER_MAX_COUNT                10    /* timer max count           */
    #define TIMER_USE_DATA                  1    /* timer use data            */
#endif

#define USE_BSP_BTN                         1  
#if USE_BSP_BTN
    #define BTN_FIFO_SIZE                  16    /* btn fifo size             */ 
    #define BTN_FILTER_TIME                 4    /* btn filter time           */
    #define BTN_DOUBLE_CLICK_TIME          30    /* btn double click interval */
    #define BTN_LONG_PRESS_TIME           200    /* btn long pressed interval */
    #define BTN_LONG_REPEAT_TIME           60    /* btn long repeat interval  */
    #define BTN_USE_EVENT                   0    /* btn use event callback    */  
    #if BTN_USE_EVENT
        #define BTN_EVENT_NUM               4    /* btn event count           */
    #endif
#endif

#define USE_PERF_COUNTER                    0 
#define USE_PRINTF                          0 

#endif /* __SYS_CONF_H__ */
