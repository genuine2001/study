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
#define ARRIVE_TIME         sys_get_ticks() - timer->ticks >= timer->period
#define HAS_COUNTER         (timer->repeat_count != 0)
#define INFINITELY          (timer->repeat_count == -1)
/******************************* DEFINES **************************************/
/***************************** DECLARATIONS ***********************************/
static sys_timer_t *timer_list = NULL;
/***************************** DECLARATIONS ***********************************/

void sys_timer_create(timer_cb_t cb, uint32_t period, void *user_data)
{
    /*	step1,	malloc a timer struct */
    sys_timer_t *timer = (sys_timer_t *)sys_malloc(sizeof(sys_timer_t));
    if(NULL == timer)
    {
        return;
    }

    /*	step2,	initialize the timer struct */
    timer->period       = period;
    timer->repeat_count = -1;
    timer->ticks        = sys_get_ticks();
    timer->timer_cb     = cb;
    timer->user_data    = user_data;

    /*	step3,	add the timer to the timer list */
    timer->next = timer_list;
    timer_list  = timer;
}

void sys_timer_set_period(sys_timer_t *timer, uint32_t period)
{
    timer->period = period;
}

void sys_timer_set_repeat_count(sys_timer_t *timer, int repeat_count)
{
    timer->repeat_count = repeat_count;
}

void sys_timer_loop(void)
{
    sys_timer_t *timer = timer_list;
    while(NULL != timer)
    {
        if(ARRIVE_TIME && HAS_COUNTER)
        {   
            timer->ticks = sys_get_ticks();
            timer->repeat_count = INFINITELY ? -1 : timer->repeat_count - 1;
            timer->timer_cb(timer->user_data);            
            if(timer->repeat_count == 0)
            {
                sys_timer_delete(timer);
            }
        }
        timer = timer->next;
    }
}

void sys_timer_delete(sys_timer_t *timer)
{
    sys_timer_t *head = timer_list;
    /*	step1,	The node to be deleted is the head node */
    if(head == timer)
    {
        timer_list = timer_list->next;
        sys_free(timer);
        return;
    }

    /*	step2,	The node to be deleted is not the head node */
    while (head->next != NULL && head->next != timer) 
    {
        head = head->next;
        /*	step2.1, no found */
        if(head->next == NULL) return;
    }
    
    /*	step3,	The node to be deleted is found */
    head->next = timer->next;
    sys_free(timer);
}