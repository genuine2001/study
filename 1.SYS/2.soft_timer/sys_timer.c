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
#define ARRIVE_TIME         sys_get_ticks() - task->ticks >= task->period
#define HAS_COUNTER         (task->repeat_count != 0)
#define INFINITELY          (task->repeat_count == -1)
/******************************* DEFINES **************************************/
/***************************** DECLARATIONS ***********************************/
static sys_task_t *task_list = NULL;
static sys_fifo_t task_fifo;
static sys_task_t task_buffer[10];
static sys_task_t* sys_task_inst(task_cb_t cb, uint32_t period, void *user_data);
/***************************** DECLARATIONS ***********************************/

void sys_task_init(void)
{
    sys_fifo_init(&task_fifo, task_buffer, sizeof(sys_task_t), 10);
}

void sys_task_create(task_cb_t cb, uint32_t period, void *user_data)
{
    /*	step1,	inst a task struct */
    sys_task_t *task = sys_task_inst(cb, period, user_data);

    /*	step2,	initialize the task struct */
    task->priority      =  0;
    task->repeat_count  =  -1;
}

void sys_task_later(task_cb_t cb, uint32_t delay, void *user_data)
{
    /*	step1,	inst a task struct */
    sys_task_t *task = sys_task_inst(cb, delay, user_data);

    /*	step2,	initialize the task struct */
    task->priority      =  0;
    task->repeat_count  =  1;
}

void sys_task_create_isr(task_cb_t cb, uint32_t period, void *user_data)
{
    /*	step1,	inst a task struct */
    sys_task_t *task = sys_task_inst(cb, period, user_data);

    /*	step2,	initialize the task struct */
    task->priority      =  1;
    task->repeat_count  =  -1;
}

void sys_task_later_isr(task_cb_t cb, uint32_t delay, void *user_data)
{
    /*	step1,	inst a task struct */
    sys_task_t *task = sys_task_inst(cb, delay, user_data);

    /*	step2,	initialize the task struct */
    task->priority      =  1;
    task->repeat_count  =  1;
}

void sys_task_set_priority(sys_task_t *task, uint8_t priority)
{
    task->priority = priority;
}

void sys_task_set_period(sys_task_t *task, uint32_t period)
{
    task->period = period;
}

void sys_task_set_repeat_count(sys_task_t *task, int repeat_count)
{
    task->repeat_count = repeat_count;
}

void sys_task_loop(void)
{
    sys_task_t *task = task_list;
    while(NULL != task)
    {
        if(ARRIVE_TIME && HAS_COUNTER)
        {   
            task->ticks = sys_get_ticks();
            task->repeat_count = INFINITELY ? -1 : task->repeat_count - 1;
            if(task->priority == 0)
            {
                sys_fifo_push(&task_fifo, task);
            }      
            else
            {
                task->task_cb(task->user_data);    
            }  
            if(task->repeat_count == 0)
            {
                sys_task_delete(task);
            }
        }
        task = task->next;
    }
}

void sys_task_mainloop(void)
{
    sys_task_t *task = NULL;
    sys_fifo_pop(&task_fifo, task);
    if(NULL != task)
    {
        task->task_cb(task->user_data);
    }
    else
    { /* Idle tasks can be executed */

    }
}

static sys_task_t* sys_task_inst(task_cb_t cb, uint32_t period, void *user_data)
{
    /*	step1,	malloc a task struct */
    sys_task_t *task = (sys_task_t *)sys_malloc(sizeof(sys_task_t));
    if(NULL == task)
    {
        return;
    }

    /*	step2,	initialize the task struct */
    task->period       = period;
    task->ticks        = sys_get_ticks();
    task->task_cb      = cb;
    task->user_data    = user_data;

    /*	step3,	add the task to the task list */
    task->next = task_list;
    task_list  = task;

    return task;
}

static void sys_task_delete(sys_task_t *task)
{
    sys_task_t *head = task_list;
    /*	step1,	The node to be deleted is the head node */
    if(head == task)
    {
        task_list = task_list->next;
        sys_free(task);
        return;
    }

    /*	step2,	The node to be deleted is not the head node */
    while (head->next != NULL && head->next != task) 
    {
        head = head->next;
        /*	step2.1, no found */
        if(head->next == NULL) return;
    }
    
    /*	step3,	The node to be deleted is found */
    head->next = task->next;
    sys_free(task);
}