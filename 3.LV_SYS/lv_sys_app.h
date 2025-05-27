/*
********************************************************************************
*    文件名称 : lv_sys_app.h
*    库 依 赖 : 
*    说    明 : 头文件声明
*    版    本 : V1.0
*
*    Ver: 1.0    Date: 2025-05-07     Author: xzt
*
*    Copyright (C), All Rights Reserved.
*    Note    :  1tab = 4 spaces!
********************************************************************************
*/
#ifndef __LV_SYS_APP_H__
#define __LV_SYS_APP_H__
/******************************* INCLUDES *************************************/
#include "lv_sys.h"
/******************************* INCLUDES *************************************/
/******************************* DEFINES **************************************/
#define LV_HOME_APP                              /* the name of your main APP */
/******************************* DEFINES **************************************/
/***************************** DECLARATIONS ***********************************/
typedef enum
{
    LV_APP_STATE_DELETE = 0, /* Application is being deleted                  */
    LV_APP_STATE_ACTIVE = 1, /* Application is being active                   */
    LV_APP_STATE_HIDE   = 2, /* Application is being hidden                   */
} lv_app_state_t;

typedef enum
{
    /*	Declare your app   */

    /*	        END        */
    APP_MAX_NUM
} lv_app_t;

typedef void (*app_init_cb)(void);    /* app init function type               */
/***************************** DECLARATIONS ***********************************/
/***************************** APIs of LV_SYS_APP *****************************/
void lv_app_init(lv_app_t app_name, app_init_cb create_cb);
void lv_app_load(lv_app_t app_name);
void lv_app_switch(lv_app_t app_name, lv_app_state_t last_app_state);

lv_obj_t   *lv_app_add_page(const char *page_name);
lv_group_t *lv_page_add_group(const char *page_name);
lv_timer_t *lv_page_add_timer(const char *page_name);

lv_obj_t   *lv_app_get_page(const char *page_name);
lv_group_t *lv_page_get_group(const char *page_name);
lv_timer_t *lv_page_get_timer(const char *page_name);

void lv_app_del_page(const char *page_name);
void lv_page_del_timer(const char *page_name);
void lv_page_del_group(const char *page_name);
void lv_app_del_page_delayed(const char *page_name, uint32_t delay_ms);

void lv_page_switch_group(const char *page_name, lv_group_t *group);

void lv_up_cb(lv_event_t *event);
void lv_home_cb(lv_event_t *event);

void lv_app_all_init(void);
/************************************ END *************************************/
#endif /* __LV_SYS_APP_H__ */
