/*
********************************************************************************
*    文件名称 : bsp_btn.h
*    库 依 赖 : stdint.h
*    说    明 : 头文件
*    版    本 : V1.0
*
*    Ver: 1.0    Date: 2025-02-10     Author: xzt
* 
*    Copyright (C), All Rights Reserved.
*    Note    :  1tab = 4 spaces!
********************************************************************************
*/
#ifndef __BSP_BTN_H__
#define __BSP_BTN_H__
/******************************* INCLUDES *************************************/
#include <stdint.h>
#include "sys_conf.h"
/******************************* INCLUDES *************************************/
/***************************** DECLARATIONS ***********************************/
typedef enum
{
    BTN_CLICKED          = 0,  /* btn is clicked         */
    BTN_DOUBLE_CLICK     = 1,  /* btn is double click    */
    BTN_LONG_PRESS       = 2,  /* btn is long press      */
    BTN_LONG_PRESS_HOLD  = 3,  /* btn is long press hold */
    BTN_IDLE             = 4,  /* btn is idle            */
    BTN_PRESSED          = 5,  /* btn is pressed         */
    BTN_RELEASED         = 6,  /* btn is released        */
} btn_status_t;

typedef void (*btn_callback_t)(void); /* btn callback function type */

typedef struct btn_t
{
    /***************** feature of target  *****************/
    uint8_t                                      status : 3;
    uint8_t                                   pin_level : 1;
    uint8_t                                 filter_time : 3;
    uint8_t                                   click_cnt : 1;
    uint8_t                                          id : 4;
    uint8_t                                        flag : 4;
    uint16_t                                          ticks;
    /***************** IO need of target  *****************/
    /* the interface from core layer                      */
    uint8_t                         (*pf_is_btn_down)(void);
#if BTN_USE_EVENT
    /* the interface from user                            */
    btn_callback_t              btn_callback[BTN_EVENT_NUM];
#endif
    /******************** Next btn  ***********************/
    struct btn_t*                                      next;
} bsp_btn_t;
/***************************** DECLARATIONS ***********************************/
/************************** The APIs of bsp_btn module ************************/
/* First API: if you want to use other apis of library,
   you should call this api firstly.                        */
void bsp_btn_init(void);                       /* btns init */

/* Second API: this api need to be called regularly.        */
void bsp_btn_scan(void);   /* scan the btn status           */

#if BTN_USE_EVENT
/* Third API: this api need to be called in the main loop.  */
void bsp_btn_handle(void); /* handle the btn event          */
#endif

/* Fourth API: this api can add the btn object.             */
void bsp_btn_add(bsp_btn_t *btn, uint8_t (*pf_is_btn_down)(void));

#if BTN_USE_EVENT
/* Fifth API: this api can add the event for btn status.    */
void bsp_btn_add_event(bsp_btn_t *btn, uint8_t status, void(*btn_cb)(void));
#endif

/* Sixth API: this api can get the btn value.               */
int8_t bsp_btn_get_value(void); 

/* Seventh API: this api can add the status.                */
void bsp_btn_add_status(bsp_btn_t *btn, uint8_t status);

/* Eighth API: this api can remove the btn status.          */
void bsp_btn_remove_status(bsp_btn_t *btn, uint8_t status);
/*********************************** END **************************************/
#endif // __BSP_BTN_H__
