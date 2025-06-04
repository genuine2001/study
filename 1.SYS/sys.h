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
    #define sys_get_ticks()     get_system_ms()
#endif
#include "./1.printf/printf.h"
#if USE_FIFO
    #include "./1.fifo/sys_fifo.h"
#endif
#if USE_TIMER
    #include "./2.soft_timer/sys_timer.h"
#endif
#if USE_BSP_BTN
    #include "./1.btn/bsp_btn.h"
#endif

#define sys_malloc(x)         malloc(x)
#define sys_free(x)           free(x)
/******************************* DEFINES **************************************/
/***************************** DECLARATIONS ***********************************/
/***************************** DECLARATIONS ***********************************/
/****************************** The APIs of FIFO ******************************/
/* First API: init the sys  */
void sys_init(void);

/* Second API: send data to FIFO */
#if USE_PERF_COUNTER == 0
uint32_t sys_get_ticks(void);
#endif
/************************************ END *************************************/
#endif /* __SYS_H__ */
