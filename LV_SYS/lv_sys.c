/*
********************************************************************************
*
*    文件名称 : lv_sys.c
*    库 依 赖 : lv_sys.h, stdint.h
*    说    明 : LVGL的全局管理模块
*
*    Copyright (C), All Rights Reserved.
*    Note    :  1tab = 4 spaces!
*
********************************************************************************
*/

/******************************* INCLUDES *************************************/
#include "lv_sys.h"
#include <stdint.h>
/******************************* INCLUDES *************************************/

/******************************* DEFINES **************************************/
#if USE_SIMULATOR
    #include "lv_drivers/win32drv/win32drv.h"
    #define lv_sys_indev lv_win32_keypad_device_object
#else
    #include "lv_port_indev_template.h"
    #define lv_sys_indev  indev_keypad
    extern  lv_indev_t   *indev_keypad;
#endif


#define LV_APP_SET_GROUP(APP)       if(NULL != APP->page_list->group) \
                                    lv_indev_set_group(lv_sys_indev,  \
                                                 APP->page_list->group)

#define LV_APP_DEL_PAGE(PAGE)       lv_obj_del_async(PAGE->screen);   \
                                    lv_mem_free(PAGE->name);          \
                                    lv_mem_free(PAGE)
#define LV_PAGE_DEL_GROUP(PAGE)     if(NULL != PAGE->group)           \
                                    lv_group_del(PAGE->group)
#define LV_PAGE_DEL_TIMER(PAGE)     if(NULL != PAGE->timer)           \
                                    lv_timer_del(PAGE->timer)

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
    app_init_t          create_cb;
} lv_app_manage_t;

static lv_style_t lv_sys_default_style;                      /* default style */
static lv_style_t lv_sys_foucus_style;                       /* focus style   */

static lv_app_manage_t *lv_app_current = NULL;         /* current app         */
static lv_app_manage_t lv_app_buffer[APP_MAX_NUM];     /* app buffer          */

static lv_language_t lv_language = LV_CN;              /* system language     */
/***************************** DECLARATIONS ***********************************/

/******************************* SYS STYLE ************************************/
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
/******************************* SYS STYLE ************************************/

/***************************** SYS LANGUAGE ***********************************/
inline lv_language_t lv_get_language(void)
{
    return lv_language;
}

inline void lv_set_language(lv_language_t lang)
{
    lv_language = lang;
}
/***************************** SYS LANGUAGE ***********************************/

/******************************* APP MANAGE ***********************************/
void lv_app_init(lv_app_t app_name, app_init_t create_cb)
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

void lv_app_load(lv_app_t app_name)
{
    lv_app_current = &lv_app_buffer[app_name];
    lv_app_current->create_cb();
    LV_APP_SET_GROUP(lv_app_current);
    lv_scr_load(lv_app_current->screen);
    lv_app_current->state = LV_APP_STATE_ACTIVE;
}

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
        LV_PAGE_DEL_GROUP(lv_app_current->page_list);
        LV_PAGE_DEL_TIMER(lv_app_current->page_list);
        LV_APP_DEL_PAGE(lv_app_current->page_list);
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
    LV_APP_SET_GROUP(lv_app_current);
    lv_app_current->state = LV_APP_STATE_ACTIVE;
}

lv_obj_t *lv_app_add_page(const char *page_name)
{
    /*	step1,	create the page */
    lv_app_page_t *page = (lv_app_page_t *)lv_mem_alloc(sizeof(lv_app_page_t));

    page->name = lv_mem_alloc(strlen(page_name) + 1);
    strcpy(page->name, page_name);

    page->screen = lv_obj_create(lv_app_current->screen);
    lv_obj_set_size(page->screen, LV_SYS_WIDTH, LV_SYS_HEIGHT);
    lv_set_default_style(page->screen);

    page->group = NULL;

    page->timer = NULL;

    /*	step2,	add the page to the page list */
    lv_app_page_t *head = lv_app_current->page_list;
    page->next = head;
    lv_app_current->page_list = page;

    return page->screen;
}

lv_group_t *lv_page_add_group(const char *page_name)
{
    /*	step1,	find the page */
    lv_app_page_t *page = lv_app_current->page_list;
    while(page != NULL)
    {
        if(0 == strcmp(page->name, page_name))
        {
            break;
        }
        page = page->next;
    }

    /*	step2,	create the group */
    page->group = lv_group_create();
    LV_APP_SET_GROUP(lv_app_current);
    return page->group;
}

lv_timer_t *lv_page_add_timer(const char *page_name, lv_timer_cb_t cb,
                                     uint32_t period, void *user_data)
{
    /*	step1,	find the page */
    lv_app_page_t *page = lv_app_current->page_list;
    while(page != NULL)
    {
        if(0 == strcmp(page->name, page_name))
        {
            break;
        }
        page = page->next;
    }

    /*	step2,	create the timer */
    page->timer = lv_timer_create(cb, period, user_data);
    return page->timer;
}

lv_obj_t   *lv_app_get_page(const char *page_name)
{
    /*	step1,	find the page */
    lv_app_page_t *page = lv_app_current->page_list;
    while(page != NULL)
    {
        if(0 == strcmp(page->name, page_name))
        {
            return page->screen;
        }
        page = page->next;
    }

    /*	step2,	not found */
    return NULL;
}

lv_group_t *lv_page_get_group(const char *page_name)
{
    /*	step1,	find the page */
    lv_app_page_t *page = lv_app_current->page_list;
    while(page != NULL)
    {
        if(0 == strcmp(page->name, page_name))
        {
            return page->group;
        }
        page = page->next;
    }

    /*	step2,	not found */
    return NULL;
}

lv_timer_t *lv_page_get_timer(const char *page_name)
{
    /*	step1,	find the page */
    lv_app_page_t *page = lv_app_current->page_list;
    while(page != NULL)
    {
        if(0 == strcmp(page->name, page_name))
        {
            return page->timer;
        }
        page = page->next;
    }

    /*	step2,	not found */
    return NULL;
}

void lv_page_switch_back(void)
{
    /*	step1,	find the current page */
    lv_app_page_t *current_page = lv_app_current->page_list;

    /* step2,	return to the last page */
    if(NULL != current_page && NULL != current_page->next)
    {
        /*	step2.1, get the last page */
        lv_app_page_t *last_page = current_page->next;

        /*	step2.2, delete the current page */
        LV_PAGE_DEL_GROUP(current_page);
        LV_PAGE_DEL_TIMER(current_page);
        LV_APP_DEL_PAGE(current_page);

        /*	step2.3, set the last page as the current page */
        lv_app_current->page_list = last_page;
        LV_APP_SET_GROUP(lv_app_current);
    }
}
/******************************* APP MANAGE ***********************************/

/***************************** ​Custom Component​ *******************************/
lv_obj_t *lv_sys_mask_create(lv_obj_t *obj)
{
    lv_obj_update_layout(obj);
    uint16_t w = lv_obj_get_width(obj);
    uint16_t h = lv_obj_get_height(obj);
    lv_obj_t *mask = lv_obj_create(obj);
    lv_set_default_style(mask);
    lv_obj_set_size(mask, w, h);
    lv_obj_set_style_bg_opa(mask, LV_OPA_60, 0);
    return mask;
}

static void close_popup(lv_event_t *event)
{
    lv_return_to_last_page();
}
void lv_sys_popup_create(lv_popup_t *popup, uint16_t w, uint16_t h, bool group)
{
    uint16_t popup_title_height     = h >> 2;
    uint16_t popup_content_height   = h - popup_title_height;
    uint16_t popup_label_obj_height = (popup_content_height / 3) << 1;
    uint16_t popup_btn_height       = popup_title_height;
    uint16_t popup_btn_width        = w >> 2;
    uint16_t pupup_btn_width_gap    = popup_btn_height * 1.7;
    uint16_t popup_btn_height_gap   = popup_btn_width >> 2;

    lv_obj_t *popup_page = lv_app_add_page("popup");

    lv_obj_t *popup_obj = lv_obj_create(popup_page);
    lv_obj_set_size(popup_obj, w, h);
    lv_obj_center(popup_obj);
    lv_set_default_style(popup_obj);
    lv_obj_set_style_radius(popup_obj, 5, 0);
    lv_obj_set_style_border_width(popup_obj, 1, 0);
    lv_obj_set_style_border_color(popup_obj, lv_color_hex(0x24BAFF), 0);
    lv_obj_set_style_clip_corner(popup_obj, true, 0);
    lv_obj_set_scrollbar_mode(popup_obj, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(popup_obj, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *popup_title_obj = lv_obj_create(popup_obj);
    lv_obj_set_size(popup_title_obj, w, popup_title_height);
    lv_obj_align(popup_title_obj, LV_ALIGN_TOP_MID, 0, 0);
    lv_set_default_style(popup_title_obj);
    lv_obj_set_style_bg_color(popup_title_obj, lv_color_hex(0x24BAFF), 0);

    popup->title_label = lv_label_create(popup_title_obj);
    lv_obj_center(popup->title_label);

    lv_obj_t *popup_content_obj = lv_obj_create(popup_obj);
    lv_obj_set_size(popup_content_obj, w, popup_content_height);
    lv_obj_align(popup_content_obj, LV_ALIGN_TOP_LEFT, 0, popup_title_height);
    lv_set_default_style(popup_content_obj);
    lv_obj_set_style_bg_color(popup_content_obj, lv_color_hex(0x252933), 0);

    lv_obj_t *popup_label_obj = lv_obj_create(popup_content_obj);
    lv_obj_set_size(popup_label_obj, w, popup_label_obj_height);
    lv_obj_align(popup_label_obj, LV_ALIGN_TOP_MID, 0, 0);
    lv_set_default_style(popup_label_obj);
    lv_obj_set_style_text_color(popup_label_obj, lv_color_hex(0x252933), 0);

    popup->content_label = lv_label_create(popup_label_obj);
    lv_obj_center(popup->content_label);

    popup->sure_btn = lv_btn_create(popup_content_obj);
    lv_obj_set_size(popup->sure_btn, popup_btn_width, popup_btn_height);
    lv_obj_align(popup->sure_btn, LV_ALIGN_BOTTOM_MID, -pupup_btn_width_gap, -popup_btn_height_gap);
    lv_set_default_style(popup->sure_btn);
    lv_obj_set_style_bg_color(popup->sure_btn, lv_color_hex(0x323844), 0);
    lv_obj_set_style_bg_color(popup->sure_btn, lv_color_hex(0x24BAFF), 4);
    lv_obj_set_style_radius(popup->sure_btn, 5, 0);
    lv_obj_set_style_outline_width(popup->sure_btn, 0, 4);

    popup->sure_btn_label = lv_label_create(popup->sure_btn);
    lv_obj_center(popup->sure_btn_label);

    popup->cancel_btn = lv_btn_create(popup_content_obj);
    lv_obj_set_size(popup->cancel_btn, popup_btn_width, popup_btn_height);
    lv_obj_align(popup->cancel_btn, LV_ALIGN_BOTTOM_MID, pupup_btn_width_gap, -popup_btn_height_gap);
    lv_obj_set_style_radius(popup->cancel_btn, 5, 0);
    lv_obj_set_style_bg_color(popup->cancel_btn, lv_color_hex(0x323844), 0);
    lv_obj_set_style_bg_color(popup->cancel_btn, lv_color_hex(0x24BAFF), 4);
    lv_obj_set_style_outline_width(popup->cancel_btn, 0, 4);

    popup->cancel_btn_label = lv_label_create(popup->cancel_btn);
    lv_obj_center(popup->cancel_btn_label);

    if(has_group)
    {
        lv_group_t *popup_group = lv_page_add_group("popup");
        lv_group_add_obj(popup_group, popup->cancel_btn);
        lv_group_add_obj(popup_group, popup->sure_btn);
    }
    lv_obj_add_event_cb(popup->sure_btn, close_popup, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(popup->cancel_btn, close_popup, LV_EVENT_CLICKED, NULL);
}
/***************************** ​Custom Component​ *******************************/

/******************************* SYS INIT *************************************/
void lv_sys_init(void)
{
    lv_sys_default_style_init();
    lv_sys_foucus_style_init();
    memset(lv_app_buffer, 0, sizeof(lv_app_buffer));
}
/******************************* SYS INIT *************************************/
