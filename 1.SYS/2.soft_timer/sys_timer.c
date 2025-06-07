/*
********************************************************************************
*    文件名称 : sys_timer.c
*    库 依 赖 : sys_timer.h
*    说    明 : 软件定时器的实现
*    版    本 : V1.0
*
*    Ver: 1.0    Date: 2025-06-4     Author: xzt
* 
*    Copyright (C), All Rights Reserved.
*    Note    :  1tab = 4 spaces!
********************************************************************************
*/
/******************************* INCLUDES *************************************/
#include "sys_timer.h"
/******************************* INCLUDES *************************************/
/******************************* DEFINES **************************************/
#define INFINITELY          -1
#define TIMER_IDLE          0
#define TIMER_RUN           1
#define ARRIVE_TIME         sys_get_ticks() - timer->ticks >= timer->period
#define HAS_COUNTER         (timer->repeat_count != 0)
#define COUNT_INFINITELY    (timer->repeat_count == -1)
/******************************* DEFINES **************************************/
/***************************** DECLARATIONS ***********************************/
static sys_timer_t *operate_list = NULL;
static sys_timer_t *execute_list = NULL;
static sys_timer_t timer_pool[TIMER_MAX_COUNT];
static uint32_t    timer_pool_index = 0;
/***************************** DECLARATIONS ***********************************/
void sys_timer_init(void)
{
    memset(timer_pool, 0, sizeof(timer_pool));
    timer_pool_index = ~((1U << TIMER_MAX_COUNT) - 1);
}


sys_timer_t* sys_timer_create(uint32_t period, timer_cb_t cb, void *arg)
{
    /*	step1,	if no available timer, return */
    if(timer_pool_index == 0xFFFFFFFF)
    {
        return NULL;
    }

    /*	step2,	find the first available timer */
    uint8_t index = __builtin_clz((~timer_pool_index));
    timer_pool_index |= (1U << index);
    sys_timer_t *timer = &timer_pool[index];

    /*	step3,	initialize the timer */
    timer->id           = index;
    timer->state        = TIMER_RUN;
    timer->period       = period;
    timer->ticks        = sys_get_ticks();
    timer->repeat_count = INFINITELY;
    timer->timer_cb     = cb;
#if TIMER_USE_DATA
    timer->user_data    = arg;
#else 
    (void *)arg;
#endif

    /*	step4,	add the timer to operate list */
    timer->next = operate_list;
    timer->prev = NULL;
    operate_list = timer;

    return timer;
}

void sys_timer_delete(sys_timer_t *timer)
{
    if(timer == NULL)
    {
        return;
    }

    /*	step1,	remove the timer from operate list */
    if(NULL == timer->prev)
    {
        operate_list = timer->next;
        timer_pool_index &= ~(1U << timer->id);
        memset(timer, 0, sizeof(sys_timer_t));
        return;
    }
    timer->prev->next = timer->next;
    if(NULL != timer->next)
    {
        timer->next->prev = timer->prev;
    }
    timer_pool_index &= ~(1U << timer->id);
    memset(timer, 0, sizeof(sys_timer_t));
}

