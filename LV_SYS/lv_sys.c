/*
********************************************************************************
*
*    文件名称 : lv_sys.c
*    库 依 赖 : lv_sys.h, sys.h
*    说    明 : 提供LVGL系统的APP管理跟一些自定义部件函数
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
    lv_obj_t             *screen;
    lv_group_t           *group;
    lv_app_t              app;
    uint8_t               page_count;
    lv_app_state_t        state;
    app_init_t            create_cb;
    app_group_t           group_cb;
} lv_app_manage_t;

static lv_style_t lv_sys_default_style;                      /* default style */
static lv_style_t lv_sys_foucus_style;                       /* focus style   */

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

inline void lv_set_default_style(lv_obj_t *obj)
{
    lv_obj_add_style(obj, &lv_sys_default_style, 0);
}

inline void lv_set_focus_style(lv_obj_t *obj)
{
    lv_obj_add_style(obj, &lv_sys_foucus_style, 4);
}

static inline void lv_app_manage_init(void)
{
    APP_COUNT = 0;
    lv_app_current.app = LV_APP_MAX_NUM;
    memset(lv_app_buffer, NULL, sizeof(lv_app_buffer));
}

void lv_app_init(lv_app_t app_name, app_init_t create_cb, app_group_t group_cb)
{
#if LV_SYS_DEBUG    
    /*	step1,	check the pramaters    */
    if( app_name >= LV_APP_MAX_NUM || NULL == create_cb || NULL == group_cb)
    {
        printf("lv_app_init: parameter error\n");
        return;
    }
#endif
     
    /*	step2,	create the app screen */
    lv_obj_t *app_screen = lv_obj_create(NULL);
    lv_obj_set_size(app_screen, LV_SYS_WIDTH, LV_SYS_HEIGHT);
    lv_set_default_style(app_screen);

    /*	step3,	create the app group */
    lv_group_t *app_group = lv_group_create();

    /*	step2,	init the app manage struct    */
    lv_app_buffer[app_name].screen     = app_screen;
    lv_app_buffer[app_name].app        = app_name;
    lv_app_buffer[app_name].page_count = 0;
    lv_app_buffer[app_name].group      = app_group;
    lv_app_buffer[app_name].state      = LV_APP_STATE_DELETE;
    lv_app_buffer[app_name].group_cb   = group_cb;
    lv_app_buffer[app_name].create_cb  = create_cb;

     /*	step3,	app count plus one    */
    APP_COUNT++;

#if LV_SYS_DEBUG
    printf("lv_app_init success:%d\n", app_name);
#endif
}

void lv_app_load(lv_app_t app_name)
{
    lv_app_current = lv_app_buffer[app_name];
    lv_app_current.create_cb();
    lv_app_current.group_cb(lv_app_current.group);
    lv_indev_set_group(app_indev, lv_app_current.group);
    lv_scr_load(lv_app_current.screen);
    lv_app_buffer[app_name].state = LV_APP_STATE_ACTIVE;
}

void lv_app_switch(lv_app_t app_name, lv_app_state_t last_app_state)
{
#if LV_SYS_DEBUG
    printf("lv_app_load start\n");
    /*	step1,	if now app count is zero or app_name is out of range */
    if(0 == APP_COUNT || app_name >= LV_APP_MAX_NUM)
    {
        return;
    }

    /*	step2,	check the app is runnig */
    if(lv_app_current.app == app_name)
    {
       return;
    }

    /*	step3,	find the new app in the mange buffer */
    lv_app_manage_t new_app = lv_app_buffer[app_name];

    /*	step4,	if the new app is not found, return */
    if(new_app.screen == NULL)
    {
        printf("lv_app_load : %d app is not found\n", app_name);
        return;
    }
    printf("lv_app_load : start to load the new app\n");
#endif

    /*	step5,	process the new app  */
    if(LV_APP_STATE_HIDE == lv_app_buffer[app_name].state)
    {
        lv_obj_clear_flag(lv_app_buffer[app_name].screen, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_app_buffer[app_name].create_cb();
        lv_app_buffer[app_name].group_cb(lv_app_buffer[app_name].group);
    }
    lv_indev_set_group(app_indev, lv_app_buffer[app_name].group);
    lv_scr_load(lv_app_buffer[app_name].screen);
    lv_app_buffer[app_name].state = LV_APP_STATE_ACTIVE;

#if LV_SYS_DEBUG
    printf("lv_app_load : start to process the last app\n");
#endif

    /*	step6,	process the last app */
    if(LV_APP_STATE_HIDE == last_app_state)
    {
        lv_obj_add_flag(lv_app_current.screen, LV_OBJ_FLAG_HIDDEN);
        lv_app_buffer[lv_app_current.app].app_state = LV_APP_STATE_HIDE;
    }
    else
    {
        lv_group_remove_all_objs(lv_app_current.group);
        lv_obj_clean(lv_app_current.screen);
        lv_app_buffer[lv_app_current.app].page_count = 0;
        lv_app_buffer[lv_app_current.app].state = LV_APP_STATE_DELETE;
    }

    /*	step7,	update the current app */
    lv_app_current = lv_app_buffer[app_name];

#if LV_SYS_DEBUG
    printf("lv_app_load success: app switch to %d\n", app_name);
#endif
}

lv_obj_t * lv_app_add_page(void)
{
#if LV_SYS_DEBUG
    /*	step1,	if now app is not created, return */
    if(NULL == lv_app_current.screen)
    {
        return NULL;
    }
    printf("lv_app_add_page start\n");
#endif
     /*	step2,	create the page */
     lv_obj_t *page = lv_obj_create(lv_app_current.screen);
     lv_obj_set_size(page, LV_SYS_WIDTH, LV_SYS_HEIGHT);
     lv_set_default_style(page);

     /*	step3,	add the page to the app */
     lv_app_current.page_count++;

     /*	step4,	update the app manage struct */
     lv_app_buffer[lv_app_current.app].page_count++;

#if LV_SYS_DEBUG
     printf("lv_app_add_page: success\n");
#endif

     return page;
}

void lv_app_return(void)
{
#if LV_SYS_DEBUG
    /*	step1,	if now no app is running, return */
    if(lv_app_current.screen == NULL)
    {
        return;
    }
    printf("lv_app_return to the previous interface start\n");
#endif

    /*	step2,	return to the previous interface */
    if(lv_app_current.page_count > 0)
    {
        lv_obj_t *page = lv_obj_get_child(lv_app_current.screen, -1);
        lv_obj_del(page);
        lv_app_current.page_count--;
        lv_app_buffer[lv_app_current.app].page_count--;
    }

#if LV_RETURN_MAIN

#if LV_SYS_DEBUG
    printf("lv_app_return to the main app start\n");
#endif
    /*	step3,	return to the main app      */
    /*	step3.1,	process the current app */
    lv_group_remove_all_objs(lv_app_current.group);
    lv_obj_clean(lv_app_current.screen);
    lv_app_buffer[lv_app_current.app].state = LV_APP_STATE_DELETE;

    /*	step3.2,	process the main app   */
    lv_app_buffer[LV_MAIN_APP].create_cb();
    lv_app_buffer[LV_MAIN_APP].group_cb(lv_app_buffer[LV_MAIN_APP].group);
    lv_scr_load(lv_app_buffer[LV_MAIN_APP].screen);
    lv_app_buffer[LV_MAIN_APP].state = LV_APP_STATE_ACTIVE;

     /*	step3.3,	update the current app */
    lv_app_current = lv_app_buffer[LV_MAIN_APP];
#endif
}

inline lv_ovj_t * lv_app_get_screen(lv_app_t app_name)
{
    return lv_app_buffer[app_name].screen;
}

lv_obj_t *lv_sys_mask_create(lv_obj_t *obj)
{
    lv_obj_update_layout(obj);
    uint16_t w = lv_obj_get_width(obj);
    uint16_t h = lv_obj_get_height(obj);
    lv_obj_t *mask = lv_obj_create(obj);
    lv_set_default_style(mask);
    lv_obj_set_size(mask, w, h);
}

void lv_sys_init(void)
{
    lv_sys_default_style_init();
    lv_sys_foucus_style_init();
    lv_app_manage_init();
}
