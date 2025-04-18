/*
********************************************************************************
*
*    文件名称 : lv_sys.c
*    库 依 赖 : lv_sys.h, sys.h
*    说    明 : 提供LVGL系统的通用接口
*    版    本 : V1.0
*    修改记录 :
*        版本号    日期           作者     说明
*        V1.0     2025-04-09    genuine   第一次创建
*
*    Copyright (C), All Rights Reserved.
*    Note    :  1tab = 4 spaces!
*
********************************************************************************
*/

/******************************* INCLUDES *************************************/
#include "lv_sys.h"
#include "sys.h"
/******************************* INCLUDES *************************************/

/******************************* DEFINES **************************************/

/******************************* DEFINES **************************************/

/***************************** DECLARATIONS ***********************************/

typedef struct lv_app_manage
{
    lv_obj_t             *app_screen;
    uint8_t               app_name;
    uint8_t               app_page_count;
    lv_app_state_t        app_state;
    lv_group_t           *app_group;
    app_callback_t        app_create_cb;
    app_callback_t        app_group_cb;
} lv_app_manage_t;

static uint8_t  APP_COUNT;                             /* app count           */
static lv_app_manage_t lv_app_current = {NULL};        /* current app         */
static lv_app_manage_t lv_app_buffer[LV_APP_MAX_NUM];  /* app buffer          */

/***************************** DECLARATIONS ***********************************/
static void lv_sys_default_style_init(void)
{
    lv_style_init(&lv_sys_default_style);
    lv_style_set_pad_all(&lv_sys_default_style, 0);
    lv_style_set_outline_width(&lv_sys_default_style, 0);
    lv_style_set_border_width(&lv_sys_default_style, 0);
    lv_style_set_shadow_width(&lv_sys_default_style, 0);
    lv_style_set_radius(&lv_sys_default_style, 0);
    lv_style_set_bg_color(&lv_sys_default_style, lv_color_black());
    lv_style_set_text_color(&lv_sys_default_style, lv_color_white());
    lv_style_set_text_font(&lv_sys_default_style, &lv_font_montserrat_28);
}

static void lv_sys_foucus_style_init(void)
{
    lv_style_init(&lv_sys_foucus_style);
    lv_style_set_pad_all(&lv_sys_foucus_style, 0);
    lv_style_set_outline_width(&lv_sys_foucus_style, 0);
    lv_style_set_border_width(&lv_sys_foucus_style, 0);
    lv_style_set_shadow_width(&lv_sys_foucus_style, 0);
    lv_style_set_radius(&lv_sys_foucus_style, 0);
    lv_style_set_bg_color(&lv_sys_foucus_style, lv_color_hex(0x24BAFF));
}

static inline void lv_app_manage_init(void)
{
    APP_COUNT = 0;
    lv_app_current.app_name = LV_APP_MAX_NUM;
    memset(lv_app_buffer, NULL, sizeof(lv_app_buffer));
}

lv_obj_t *lv_app_create(void)
{
    lv_obj_t *app_screen = lv_obj_create(NULL);
    lv_set_default_style(app_screen);
    return app_screen;
}

void lv_app_init(lv_obj_t *app_screen, uint8_t app_name, lv_group_t *app_group,
                 app_callback_t app_create_cb, app_callback_t app_group_cb)

{
#if LV_SYS_DEBUG    
    /*	step1,	check the pramaters    */
    if( app_name >= LV_APP_MAX_NUM || NULL == app_group || NULL == app_screen ||
        NULL == app_create_cb      || NULL == app_group_cb)
    {
        printf("lv_app_init: parameter error\n");
        return;
    }
#endif

    /*	step2,	init the app manage struct    */
    lv_app_buffer[app_name].app_screen     = app_screen;
    lv_app_buffer[app_name].app_name       = app_name;
    lv_app_buffer[app_name].app_page_count = 0;
    lv_app_buffer[app_name].app_group      = app_group;
    lv_app_buffer[app_name].app_state      = LV_APP_STATE_DELETE;
    lv_app_buffer[app_name].app_group_cb   = app_group_cb;
    lv_app_buffer[app_name].app_create_cb  = app_create_cb;

     /*	step3,	app count plus one    */
    APP_COUNT++;

#if LV_SYS_DEBUG
    printf("lv_app_init success:%d\n", app_name);
#endif
}

void lv_app_load(uint8_t app_name)
{
    lv_app_current = lv_app_buffer[app_name];
    lv_app_current.app_create_cb();
    lv_app_current.app_group_cb();
    lv_indev_set_group(app_indev, lv_app_current.app_group);
    lv_scr_load(lv_app_current.app_screen);
    lv_app_buffer[app_name].app_state = LV_APP_STATE_ACTIVE;
}

void lv_app_switch(uint8_t app_name, lv_app_state_t last_app_state)
{
#if LV_SYS_DEBUG
    printf("lv_app_load start\n");
    /*	step1,	if now app count is zero or app_name is out of range */
    if(0 == APP_COUNT || LV_APP_MAX_NUM <= app_name)
    {
        return;
    }

    /*	step2,	check the app is runnig */
    if(lv_app_current.app_name == app_name)
    {
       return;
    }

    /*	step3,	find the new app in the mange buffer */
    lv_app_manage_t new_app = lv_app_buffer[app_name];

    /*	step4,	if the new app is not found, return */
    if(new_app.app_screen == NULL)
    {
        return;
    }
    printf("lv_app_load : start to load the new app\n");
#endif

    /*	step5,	process the new app  */
    if(LV_APP_STATE_HIDE == lv_app_buffer[app_name].app_state)
    {
        lv_obj_clear_flag(lv_app_buffer[app_name].app_screen, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_app_buffer[app_name].app_create_cb();
        lv_app_buffer[app_name].app_group_cb();
    }
    lv_indev_set_group(app_indev, lv_app_buffer[app_name].app_group);
    lv_scr_load(lv_app_buffer[app_name].app_screen);
    lv_app_buffer[lv_app_buffer[app_name].app_name].app_state = LV_APP_STATE_ACTIVE;

#if LV_SYS_DEBUG
    printf("lv_app_load : start to process the last app\n");
#endif

    /*	step6,	process the last app */
    if(LV_APP_STATE_HIDE == last_app_state)
    {
        lv_obj_add_flag(lv_app_current.app_screen, LV_OBJ_FLAG_HIDDEN);
        lv_app_buffer[lv_app_current.app_name].app_state = LV_APP_STATE_HIDE;
    }
    else
    {
        lv_group_remove_all_objs(lv_app_current.app_group);
        lv_obj_clean(lv_app_current.app_screen);
        lv_app_buffer[lv_app_current.app_name].app_page_count = 0;
        lv_app_buffer[lv_app_current.app_name].app_state = LV_APP_STATE_DELETE;
    }

    /*	step7,	update the current app */
    lv_app_current = lv_app_buffer[app_name];

#if LV_SYS_DEBUG
    printf("lv_app_load success: app switch to %d\n", app_name);
#endif
}

lv_obj_t * lv_app_add_page(uint8_t app_name)
{
#if LV_SYS_DEBUG
    /*	step1,	if now app count is zero or app_name is out of range */
    if(0 == APP_COUNT || LV_APP_MAX_NUM <= app_name)
    {
#if LV_SYS_DEBUG
        printf("lv_app_add_page: none app is created\n");
#endif
        return NULL;
    }

    /*	step2,	find the app in the mange buffer */
    lv_app_manage_t app = lv_app_buffer[app_name];

    /*	step3,	if the app is not found, return */
    if(app.app_screen == NULL)
    {
#if LV_SYS_DEBUG
        printf("lv_app_add_page: the app is not found\n");
#endif
        return NULL;
    }
#endif
     /*	step3,	create the page */
     lv_obj_t *page = lv_obj_create(lv_app_buffer[app_name].app_screen);
     lv_obj_set_size(page, LV_SYS_WIDTH, LV_SYS_HEIGHT);
     lv_set_default_style(page);

     /*	step4,	add the page to the app */
     lv_app_buffer[app_name].app_page_count++;

#if LV_SYS_DEBUG
     printf("lv_app_add_page: success\n");
#endif

     return page;
}

void lv_app_return(void)
{
#if LV_SYS_DEBUG
    /*	step1,	if now no app is running, return */
    if(lv_app_current.app_screen == NULL)
    {
        printf("lv_app_return: no app is running\n");
        return;
    }
    printf("lv_app_return to the previous interface start\n");
#endif

    /*	step2,	return to the previous interface */
    if(lv_app_current.app_page_count > 0)
    {
        lv_obj_t *page = lv_obj_get_child(lv_app_current.app_screen, -1);
        lv_obj_del(page);
        lv_app_buffer[lv_app_current.app_name].app_page_count--;
    }

#if LV_RETURN_MAIN

#if LV_SYS_DEBUG
    printf("lv_app_return to the main app start\n");
#endif
    /*	step3,	return to the main app      */
    /*	step3.1,	process the current app */
    lv_group_remove_all_objs(lv_app_current.app_group);
    lv_obj_clean(lv_app_current.app_screen);
    lv_app_buffer[lv_app_current.app_name].app_state = LV_APP_STATE_DELETE;
    lv_app_buffer[lv_app_current.app_name].app_page_count = 0;

    /*	step3.2,	process the main app   */
    lv_app_buffer[LV_MAIN_APP].app_create_cb();
    lv_app_buffer[LV_MAIN_APP].app_group_cb();
    lv_scr_load(lv_app_buffer[LV_MAIN_APP].app_screen);
    lv_app_buffer[LV_MAIN_APP].app_state = LV_APP_STATE_ACTIVE;

     /*	step3.3,	update the current app */
    lv_app_current = lv_app_buffer[LV_MAIN_APP];
#endif
}

void lv_sys_init(void)
{
    lv_sys_default_style_init();
    lv_sys_foucus_style_init();
    lv_app_manage_init();
}
