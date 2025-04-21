/*
********************************************************************************
*
*    文件名称 : bsp_btn.h
*    库 依 赖 : stdint.h
*    说    明 : 头文件
*    版    本 : V1.0
*    修改记录 :
*        版本号    日期           作者     说明
*        V1.0     2025-03-23    genuine   第一次创建
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
#define BTN_DEBUG                 0              /* DEBUG define              */

/* Note: The maximum number of keys is 16 */
#define BSP_BTN_NUM               1              /* btn count                 */
#define BSP_BTN_EVENT_NUM         5              /* btn event count           */
#define BSP_BTN_FIFO_SIZE         16             /* btn fifo size             */ 

/* Note: the time unit is ms */
#define BSP_BTN_FILTER_TIME       5              /* btn filter time           */
#define BSP_BTN_DOUBLE_CLICK_TIME 1500           /* btn double click interval */
#define BSP_BTN_HOLD_TIME         1000           /* btn hold time             */

/******************************* DEFINES **************************************/

/***************************** DECLARATIONS ***********************************/
typedef enum
{
    BTN_IDLE         = 0,  /* btn is idle            */
    BTN_PRESSED      = 1,  /* btn is pressed         */
    BTN_CLICKED      = 2,  /* btn is clicked         */
    BTN_DOUBLE_CLICK = 3,  /* btn is double click    */
    BTN_LONG_PRESS   = 4,  /* btn is long press      */
} bsp_status_t;

typedef void (*btn_callback_t)(void); /* btn callback function type */

typedef struct
{
    bsp_status_t                                     status;
    uint16_t                                    filter_time;
    uint16_t                              double_click_time;
    uint16_t                                      hold_time;
    /***************** IO need of target  *****************/
    /* the interface from core layer                      */
    uint8_t                         (*pf_is_btn_down)(void);
    /* the interface from user                            */
    btn_callback_t          btn_callback[BSP_BTN_EVENT_NUM];
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
