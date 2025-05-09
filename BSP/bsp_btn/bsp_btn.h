/*
********************************************************************************
*
*    文件名称 : bsp_btn.h
*    库 依 赖 : stdint.h
*    说    明 : 头文件
*    版    本 : V1.0
*
*    Copyright (C), All Rights Reserved.
*    Note    :  1tab = 4 spaces!
*
********************************************************************************
*/
#ifndef __BSP_BTN_H__
#define __BSP_BTN_H__
/******************************* INCLUDES *************************************/
#include <stdint.h>
/******************************* INCLUDES *************************************/

/******************************* DEFINES **************************************/
#define BTN_DEBUG             0                  /* DEBUG define              */

#define BTN_EVENT_NUM         4                  /* btn event count           */
#define BTN_FIFO_SIZE         16                 /* btn fifo size             */ 

/* Note: the time unit is 5ms */
#define BTN_FILTER_TIME       4                  /* btn filter time           */
#define BTN_DOUBLE_CLICK_TIME 50                 /* btn double click interval */
#define BTN_LONG_PRESS_TIME   250                /* btn long pressed interval */
#define BTN_LONG_REPEAT_TIME  25                 /* btn long repeat interval  */


/******************************* DEFINES **************************************/

/***************************** DECLARATIONS ***********************************/
typedef void (*btn_callback_t)(void); /* btn callback function type */

typedef struct btn_t
{
    /***************** feature of target  *****************/
    uint8_t                                      status : 3;
    uint8_t                                   pin_level : 1;
    uint8_t                                 filter_time : 3;
    uint8_t                                   click_cnt : 1;
    uint8_t                                              id;
    uint16_t                                          ticks;
    /***************** IO need of target  *****************/
    /* the interface from core layer                      */
    uint8_t                         (*pf_is_btn_down)(void);
    /* the interface from user                            */
    btn_callback_t              btn_callback[BTN_EVENT_NUM];
    /******************** Next btn  ***********************/
    struct btn_t*                                      next;
} bsp_btn_t;

/**************** The APIs of bsp_btn module ****************/
/* First API: if you want to use other apis of library,
   you should call this api firstly.                        */
void bsp_btn_init(void);                       /* btns init */

/* Second API: this api need to be called regularly.        */
void bsp_btn_scan(void);   /* scan the btn status           */

/* Third API: this api need to be called in the main loop.  */
void bsp_btn_handle(void); /* handle the btn event          */

/* Fourth API: this api can be called to get the btn value. */
int bsp_btn_get_value(void); /* get the btn value           */
/***************************** DECLARATIONS ***********************************/
#endif // __BSP_BTN_H__
