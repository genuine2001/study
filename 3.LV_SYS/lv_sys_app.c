/*
********************************************************************************
*    文件名称 : lv_sys_app.c
*    库 依 赖 : lv_sys.h
*    说    明 : 提供LVGL的app管理函数，目前的del函数暂时只能删除当前页面的对象
*    版    本 : V1.0
*
*    Ver: 1.0    Date: 2025-05-07     Author: xzt
*
*    Copyright (C), All Rights Reserved.
*    Note    :  1tab = 4 spaces!
********************************************************************************
*/
/******************************* INCLUDES *************************************/
#include "lv_sys_app.h"
/******************************* INCLUDES *************************************/
/******************************* DEFINES **************************************/
#if USE_SIMULATOR
    #include "lv_drivers/win32drv/win32drv.h"
    #define lv_sys_indev lv_win32_keypad_device_object
#else
    #include "lv_port_disp_template.h"
    #include "lv_port_indev_template.h" 
    extern lv_indev_t *indev_keypad;
    #define lv_sys_indev indev_keypad
#endif

/* Redefine the lv_del_obj function to avoid wild Pointers */
#define lv_timer_del(timer)         lv_timer_del(timer);             \
                                    timer = NULL
#define lv_group_del(group)         lv_group_del(group);             \
                                    group = NULL

#define LV_PAGE_SET_GROUP(PAGE)     if(NULL != PAGE->group)          \
                                    lv_indev_set_group(lv_sys_indev, \
                                                       PAGE->group)
                                    
#define LV_APP_DEL_PAGE(PAGE)       if(NULL != PAGE->group)          \
                                    lv_group_del(PAGE->group);       \
                                    if(NULL != PAGE->timer)          \
                                    lv_timer_del(PAGE->timer);       \
                                    lv_obj_del(PAGE->screen);        \
                                    lv_mem_free(PAGE->name);         \
                                    lv_mem_free(PAGE) 
/******************************* DEFINES **************************************/
/***************************** DECLARATIONS ***********************************/
typedef struct lv_app_page
{
    char                  *name  ;
    lv_obj_t              *screen;
    lv_group_t            *group ;
    lv_timer_t            *timer ;
    struct lv_app_page    *next  ;
} lv_app_page_t;

typedef struct lv_app_manage
{
    uint8_t             app   : 6;
    uint8_t             state : 2;
    lv_obj_t           *screen   ;
    lv_app_page_t      *page_list;
    app_init_cb         create_cb;
} lv_app_manage_t;

static lv_app_manage_t *lv_app_current = NULL;         /* current app         */
static lv_app_manage_t lv_app_buffer[APP_MAX_NUM];     /* app buffer          */
/***************************** DECLARATIONS ***********************************/

/*******************************************************************************
* @brief Initialize the LVGL app manager.
*
* @param app_name: the name of the app.
                   You need to define the app name in enum lv_app_t.
* @param create_cb: the callback function to create the app.
*******************************************************************************/
void lv_app_init(lv_app_t app_name, app_init_cb create_cb)
{
    /*	step1,	create the app screen     */
    lv_obj_t *app_screen = lv_obj_create(NULL);

    /*	step2,	init the app manage struct    */
    lv_app_buffer[app_name].screen              = app_screen;
    lv_app_buffer[app_name].app                 = app_name;
    lv_app_buffer[app_name].page_list           = NULL;
    lv_app_buffer[app_name].state               = LV_APP_STATE_DELETE;
    lv_app_buffer[app_name].create_cb           = create_cb;
}

/*******************************************************************************
* @brief Load the app at first.
*
* @param app_name: The name of the loaded app.
*******************************************************************************/
void lv_app_load(lv_app_t app_name)
{
    lv_app_current = &lv_app_buffer[app_name];
    lv_app_current->create_cb();
    lv_scr_load(lv_app_current->screen);
    lv_app_current->state = LV_APP_STATE_ACTIVE;
}

/*******************************************************************************
* @brief Switch the app.
*
* @param app_name: The name of the switched app.
* @param last_app_state: The state of the last app.
*        LV_APP_STATE_ACTIVE: The last app will be deleted.
*        LV_APP_STATE_DELETE: The last app will be deleted.
*        LV_APP_STATE_HIDE:   The last app will be hidden.
*******************************************************************************/
void lv_app_switch(lv_app_t app_name, lv_app_state_t last_app_state)
{
    /*	step1,	process the current app  */
    if(LV_APP_STATE_HIDE == last_app_state)
    {
        lv_obj_add_flag(lv_app_current->screen, LV_OBJ_FLAG_HIDDEN);
        lv_app_current->state = LV_APP_STATE_HIDE;
    }
    else
    {
        lv_app_page_t *page = lv_app_current->page_list;
        while(page != NULL)
        {
            lv_app_current->page_list = lv_app_current->page_list->next;
            LV_APP_DEL_PAGE(page);
            page = lv_app_current->page_list;
        }
        lv_app_current->state = LV_APP_STATE_DELETE;
    }

    /*	step2,	process the switch app */
    lv_app_current = &lv_app_buffer[app_name];
    if(LV_APP_STATE_HIDE == lv_app_current->state)
    {
        lv_obj_clear_flag(lv_app_current->screen, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_app_current->create_cb();
    }
    lv_scr_load(lv_app_current->screen);
    lv_app_current->state = LV_APP_STATE_ACTIVE;
}

/*******************************************************************************
* @brief Add a page to the current app.
*
* @param page_name: The name of the page.
*        null:  current page.
*        other: the name of the page.
*
* @return the screen of the page.
*******************************************************************************/
lv_obj_t *lv_app_add_page(const char *page_name)
{
    /*	step1,	create the page */
    lv_app_page_t *page = (lv_app_page_t *)lv_mem_alloc(sizeof(lv_app_page_t));

    page->name = lv_mem_alloc(strlen(page_name) + 1);
    strcpy(page->name, page_name);

    page->screen = lv_obj_create(lv_app_current->screen);
    lv_obj_set_size(page->screen, LV_SYS_WIDTH, LV_SYS_HEIGHT);
    lv_sys_set_default_style(page->screen);

    page->group = NULL;

    page->timer = NULL;

    /*	step2,	add the page to the page list */
    lv_app_page_t *head = lv_app_current->page_list;
    page->next = head;
    lv_app_current->page_list = page;

    return page->screen;
}

/*******************************************************************************
* @brief Add a group to the page.
*
* @param page_name: The name of the page.
*        null:  current page.
*        other: the name of the page.
*
* @return the group of the page.
*******************************************************************************/
lv_group_t *lv_page_add_group(const char *page_name)
{
    lv_app_page_t *page = lv_app_current->page_list;

     /*	step1,	add the group to now page */
    if(NULL == page_name && NULL != page) 
    {
        page->group = lv_group_create();
        LV_PAGE_SET_GROUP(page);
        return page->group;
    }

    /*	step2,	find the page */
    while(page != NULL)
    {
        if(0 == strcmp(page->name, page_name))
        {
            break;
        }
        page = page->next;
    }

    /*	step3,	create the group */
    page->group = lv_group_create();
    LV_PAGE_SET_GROUP(page);
    return page->group;
}

/*******************************************************************************
* @brief Add a timer to the page.
*
* @param page_name: The name of the page.
*        null:  current page.
*        other: the name of the page.
*
* @return the timer of the page.
*******************************************************************************/
lv_timer_t *lv_page_add_timer(const char *page_name)
{
    lv_app_page_t *page = lv_app_current->page_list;

    /*	step1,	add the timer to now page */
    if(NULL == page_name && NULL != page) 
    {
        page->timer = lv_timer_create(NULL, 1000, NULL);
        return page->timer;
    }

    /*	step2,	find the page */
    while(page != NULL)
    {
        if(0 == strcmp(page->name, page_name))
        {
            break;
        }
        page = page->next;
    }

    /*	step3,	create the timer */
    if(NULL != page)
    {
        page->timer = lv_timer_create(NULL, 1000, NULL);
    }
    return page->timer;
}

/*******************************************************************************
* @brief Get a page of the current app.
*
* @param page_name: The name of the page.
*        null:  current page.
*        other: the name of the page.
*
* @return the screen of the page.
*******************************************************************************/
lv_obj_t *lv_app_get_page(const char *page_name)
{
    lv_app_page_t *page = lv_app_current->page_list;

    /*	step1,	get now page */
    if(NULL == page_name) 
    {
        return page->screen;
    }

    /*	step2,	find the page */
    while(page != NULL)
    {
        if(0 == strcmp(page->name, page_name))
        {
            return page->screen;
        }
        page = page->next;
    }

    /*	step3,	not found */
    return NULL;
}

/*******************************************************************************
* @brief Get a group of the page.
*
* @param page_name: The name of the page.
*        null:  current page.
*        other: the name of the page.

* @return the group of the page.
*******************************************************************************/
lv_group_t *lv_page_get_group(const char *page_name)
{
    lv_app_page_t *page = lv_app_current->page_list;

    /*	step1,	get now page group */
    if(NULL == page_name && NULL != page) 
    {
        return page->group;
    }

    /*	step2,	find the page */
    while(page != NULL)
    {
        if(0 == strcmp(page->name, page_name))
        {
            return page->group;
        }
        page = page->next;
    }

    /*	step3,	not found */
    return NULL;
}

/*******************************************************************************
* @brief Get a timer of the page.
*
* @param page_name: The name of the page.
*        null:  current page.
*        other: the name of the page.

* @return the timer of the page.
*******************************************************************************/
lv_timer_t *lv_page_get_timer(const char *page_name)
{
    lv_app_page_t *page = lv_app_current->page_list;

    /*	step1,	get now page timer */
    if(NULL == page_name && NULL != page) 
    {
        return page->timer;
    }

    /*	step2,	find the page */
    while(page != NULL)
    {
        if(0 == strcmp(page->name, page_name))
        {
            return page->timer;
        }
        page = page->next;
    }

    /*	step3,	not found */
    return NULL;
}

/*******************************************************************************
* @brief Delete a page of the current app.
*
* @param page_name: The name of the page.
*        null:  current page.
*        other: the name of the page.
*******************************************************************************/
void lv_app_del_page(const char *page_name)
{
    /*	step1,	find the current page list */
    lv_app_page_t *current_page = lv_app_current->page_list;

    /*	step2,	del now page  */
    if(NULL == page_name && NULL != current_page && NULL != current_page->next) 
    {
        /*	step2.1, get the last page */
        lv_app_page_t *last_page = current_page->next;

        /*	step2.2, delete the current page */
        LV_APP_DEL_PAGE(current_page);

        /*	step2.3, set the last page as the current page */
        lv_app_current->page_list = last_page;
        LV_PAGE_SET_GROUP(lv_app_current->page_list);
        return;
    }
}

/*******************************************************************************
* @brief Delete a timer of the page.
*
* @param page_name: The name of the page.
*        null:  current page.
*        other: the name of the page.
*******************************************************************************/
void lv_page_del_timer(const char *page_name)
{
    lv_app_page_t *page = lv_app_current->page_list;
    
    /*	step1,	del now page timer */
    if(NULL == page_name && NULL != page && NULL != page->timer) 
    {
        lv_timer_t *timer = page->timer;
        page->timer = NULL;
        lv_timer_del(timer);
        return;
    }
}

/*******************************************************************************
* @brief Delete a group of the page.
*
* @param page_name: The name of the page.
*        null:  current page.
*        other: the name of the page.
*******************************************************************************/
void lv_page_del_group(const char *page_name)
{
    lv_app_page_t *page = lv_app_current->page_list;
    
    /*	step1,	del now page group */
    if(NULL == page_name && NULL != page && NULL != page->group) 
    {
        lv_group_del(page->group);
        return;
    }
}

/*******************************************************************************
* @brief The callback function for delaying the deletion of the page
*
* @param timer: The timer date.
*******************************************************************************/
static void lv_app_del_page_cb(lv_timer_t *timer)
{
    const char *page_name = (const char *)lv_timer_get_user_data(timer);
    lv_app_del_page(page_name);
    lv_timer_del(timer);
}

/*******************************************************************************
* @brief Delete a page of the current app after a delay.
*
* @param page_name: The name of the page.
*        null:  current page.
*        other: the name of the page.
* @param delay_ms: The delay time in milliseconds.
*******************************************************************************/
void lv_app_del_page_delayed(const char *page_name, uint32_t delay_ms)
{
    lv_timer_t *timer = lv_timer_create(lv_app_del_page_cb, delay_ms, page_name);
}

/*******************************************************************************
* @brief Switch the group of the page.
*
* @param page_name: The name of the page.
*        null:  current page.
*        other: the name of the page.
* @param group: The new group.
*******************************************************************************/
void lv_page_switch_group(const char *page_name, lv_group_t *group)
{
    lv_app_page_t *page = lv_app_current->page_list;
    
    /*	step1,	switch now page group */
    if(NULL == page_name && NULL != page && NULL != page->group) 
    {
        page->group = group;
        LV_PAGE_SET_GROUP(page);
        return;
    }

    /*	step2,	find the page */
    while(page != NULL)
    {
        if(0 == strcmp(page->name, page_name))
        {
            break;
        }
        page = page->next;
    }

    /*	step3,	switch the page group */
    if(NULL != page) 
    {
        page->group = group;
        LV_PAGE_SET_GROUP(page);
    }
}

/*******************************************************************************
* @brief Return to the previous interface
*
* @param event: The event data.
*******************************************************************************/
void lv_up_cb(lv_event_t *event)
{
    lv_app_del_page(NULL);
}

/*******************************************************************************
* @brief Return to the main interface
*
* @param event: The event data.
*******************************************************************************/
void lv_home_cb(lv_event_t *event)
{
    lv_app_switch(LV_HOME_APP, LV_APP_STATE_DELETE);
}

/*******************************************************************************
* @brief Initialize all apps.
*******************************************************************************/
void lv_app_all_init(void)
{
    
}
/********************************** END ***************************************/
