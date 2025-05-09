/*
********************************************************************************
*
*    文件名称 : lv_sys.h
*    库 依 赖 : lvgl.h
*    说    明 : 头文件声明
*    版    本 : V1.0
*
*    Copyright (C), All Rights Reserved.
*    Note    :  1tab = 4 spaces!
*
********************************************************************************
*/
#ifndef __LV_SYS_H__
#define __LV_SYS_H__

/******************************* INCLUDES *************************************/
#include "lvgl.h"
#include "lv_sys_font.h"
#include "lv_sys_img.h"
/******************************* INCLUDES *************************************/

/******************************* DEFINES **************************************/
#define USE_SIMULATOR           1    /* Enable simulator.                     */

#define LV_TURE                 1    /* True value.                           */
#define LV_FALSE                0    /* False value.                          */
 
#define LV_SYS_WIDTH            240  /* Width of the system display           */
#define LV_SYS_HEIGHT           240  /* Height of the system display          */

/* Redefine the lv_del_obj function to avoid wild Pointers */
#define lv_timer_del(timer)     lv_timer_del(timer); \
                                timer = NULL
#define lv_group_del(group)     lv_group_del(group); \
                                group = NULL

/* Define the data types used for user data */
#define LV_UINT(x)              (void *)(uintptr_t)(x) 
#define LV_INI(x)               (void *)(intptr_t)(x) 
/******************************* DEFINES **************************************/

/***************************** DECLARATIONS ***********************************/
typedef enum
{
    LV_APP_STATE_DELETE = 0, /* Application is being deleted */
    LV_APP_STATE_ACTIVE = 1, /* Application is being active  */
    LV_APP_STATE_HIDE   = 2, /* Application is being hidden  */
} lv_app_state_t;

typedef enum
{
    LV_CN = 0, /* Simplified Chinese  */
    LV_TC = 1, /* Traditional Chinese */
    LV_EN = 2, /* English             */
    LV_JA = 3, /* Japanese            */
    LV_KO = 4, /* Korean              */
    LV_RU = 5, /* Russian             */
} lv_language_t;

typedef enum
{
    /*	Declare your app   */

    /*	        END        */
    APP_MAX_NUM
} lv_app_t;

typedef struct 
{
    lv_obj_t *title_label;
    lv_obj_t *content_label;
    lv_obj_t *sure_btn;
    lv_obj_t *sure_btn_label;
    lv_obj_t *cancel_btn;
    lv_obj_t *cancel_btn_label;
} lv_popup_t;                              /* Custom pop-up widget            */

typedef void (*app_init_t)(void);          /* app init function type          */
/***************************** DECLARATIONS ***********************************/

/**************************** APIs of LV_SYS **********************************/
void lv_sys_init(void);

void lv_app_init(lv_app_t app_name, app_init_t create_cb);
void lv_app_load(lv_app_t app_name);
void lv_app_switch(lv_app_t app_name, lv_app_state_t last_app_state);
lv_obj_t   *lv_app_add_page(const char *page_name);
lv_group_t *lv_page_add_group(const char *page_name);
lv_timer_t *lv_page_add_timer(const char *page_name, lv_timer_cb_t cb,
                              uint32_t period, void *user_data);
lv_obj_t   *lv_app_get_page(const char *page_name);
lv_group_t *lv_page_get_group(const char *page_name);
lv_timer_t *lv_page_get_timer(const char *page_name);
void lv_page_switch_back(void);

lv_obj_t *lv_sys_mask_create(lv_obj_t *obj);
void lv_sys_popup_create(lv_popup_t *popup, uint16_t w, uint16_t h, bool group);

extern inline void lv_set_default_style(lv_obj_t *obj);
extern inline void lv_set_focus_style(lv_obj_t *obj);

extern inline lv_language_t lv_get_language(void);
extern inline void lv_set_language(lv_language_t lang);
/********************************** END ***************************************/
#endif /* __LV_SYS_H__ */
