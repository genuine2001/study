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

/******************************* INCLUDES *************************************/

/******************************* DEFINES **************************************/
#define USE_SIMULATOR          1    /* Enable simulator.                      */
#if USE_SIMULATOR
    #include "lv_drivers/win32drv/win32drv.h"
    #define app_indev lv_win32_keypad_device_object
#else
    #define app_indev  indev_keypad
    extern lv_indev_t *indev_keypad;
#endif

#define LV_SYS_DEBUG           0    /* Enable system debugging.               */

#define LV_APP_MAX_NUM         10   /* Maximum number of managed app.         */
#define LV_MAIN_APP            0    /* Index of the main application.         */
#define LV_RETURN_MAIN         0    /* Enable to switch to the main interface */

#define LV_SYS_WIDTH           240  /* Width of the system display            */
#define LV_SYS_HEIGHT          240  /* Height of the system display           */

/* Redefine the lv_del_obj function to avoid wild Pointers */
#define lv_del_obj(obj)         lv_obj_del(obj); \
                                obj = NULL
#define lv_obj_del_async(obj)   lv_obj_del_async(obj); \
                                obj = NULL
#define lv_timer_del(timer)     lv_timer_del(timer); \
                                timer = NULL
#define lv_group_del(group)     lv_group_del(group); \
                                group = NULL

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
    /*	Declare your app   */

} lv_app_t;

typedef void (*app_init_t)(void);          /* app init function type          */
typedef void (*app_group_t)(lv_group_t *); /* app group init function type    */

/***************************** DECLARATIONS ***********************************/

void lv_sys_init(void);

extern inline void lv_set_default_style(lv_obj_t *obj);
extern inline void lv_set_focus_style(lv_obj_t *obj);

void lv_app_init(lv_app_t app_name, app_init_t create_cb, app_group_t group_cb);
void lv_app_load(lv_app_t app_name);
void lv_app_switch(lv_app_t app_name, lv_app_state_t last_app_state);
lv_obj_t *lv_app_add_page(void);
void lv_app_return(void);
extern inline lv_ovj_t * lv_app_get_screen(lv_app_t app_name);

lv_obj_t *lv_sys_mask_create(lv_obj_t *obj);

#endif /* __LV_SYS_H__ */
