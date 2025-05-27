/*
********************************************************************************
*    文件名称 : lv_sys.c
*    库 依 赖 : bsp_btn.h
*    说    明 : LVGL的全局管理模块
*
*    Ver: 1.0    Date: 2025-05-07     Author: xzt
*
*    Copyright (C), All Rights Reserved.
*    Note    :  1tab = 4 spaces!
********************************************************************************
*/
/******************************* INCLUDES *************************************/
#include "lv_sys.h"
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

#define LV_THEME_DEFAULT_COLOR      lv_color_make(color[0], color[1], color[2])                                    
#define LV_THEME_FOCUS_COLOR        lv_color_make(color[3], color[4], color[5])  
/******************************* DEFINES **************************************/
/***************************** DECLARATIONS ***********************************/
static uint8_t keypad_mode = LV_KEYPAD_MODE_NAIV;          /* keypad mode     */

static lv_style_t lv_sys_default_style;                    /* default style   */
static lv_style_t lv_sys_foucus_style;                     /* focus style     */

static lv_language_t lv_language = LV_CN;                  /* system language */
static uint8_t color[6] = {0x00, 0x00, 0x00, 0x24, 0xBA, 0xFF};/* theme color */

static void lv_sys_default_style_init(void);
static void lv_sys_foucus_style_init(void);
/***************************** DECLARATIONS ***********************************/

/*******************************************************************************
* @brief Initialize the LVGL.
*******************************************************************************/
void lv_sys_init(void)
{
    lv_init();
    lv_port_disp_init(); 
    lv_port_indev_init();
    lv_sys_default_style_init();
    lv_sys_foucus_style_init();
    lv_app_all_init();
}

/*******************************************************************************
* @brief Initialize the LVGL default style.
*******************************************************************************/
static void lv_sys_default_style_init(void)
{
    lv_style_init(&lv_sys_default_style);
    lv_style_set_pad_all(&lv_sys_default_style, 0);
    lv_style_set_border_width(&lv_sys_default_style, 0);
    lv_style_set_shadow_width(&lv_sys_default_style, 0);
    lv_style_set_radius(&lv_sys_default_style, 0);
    lv_style_set_bg_color(&lv_sys_default_style, LV_THEME_DEFAULT_COLOR);
    lv_style_set_text_color(&lv_sys_default_style, lv_color_white());
    lv_style_set_text_font(&lv_sys_default_style, &lv_font_montserrat_18);
}

/*******************************************************************************
* @brief Initialize the LVGL focus style.
*******************************************************************************/
static void lv_sys_foucus_style_init(void)
{
    lv_style_init(&lv_sys_foucus_style);
    lv_style_set_outline_width(&lv_sys_foucus_style, 0);
    lv_style_set_bg_color(&lv_sys_foucus_style, LV_THEME_FOCUS_COLOR);
    lv_style_set_border_color(&lv_sys_foucus_style, LV_THEME_FOCUS_COLOR);
}

/*******************************************************************************
* @brief Set the default style for the object.
* @param  obj  pointer to the object.
*******************************************************************************/
inline void lv_sys_set_default_style(lv_obj_t *obj)
{
    lv_obj_add_style(obj, &lv_sys_default_style, 0);
}

/*******************************************************************************
* @brief Set the focus style for the object.
* @param  obj  pointer to the object.
*******************************************************************************/
inline void lv_sys_set_focus_style(lv_obj_t *obj)
{
    lv_obj_add_style(obj, &lv_sys_foucus_style, 4);
}

/*******************************************************************************
* @brief Set the default theme color.
* @param  r  red value.
* @param  g  green value.
* @param  b  blue value.
*******************************************************************************/
void lv_sys_set_default_theme_color(uint8_t r, uint8_t g, uint8_t b)
{
    color[0] = r;
    color[1] = g;
    color[2] = b;
    lv_style_set_bg_color(&lv_sys_default_style, LV_THEME_DEFAULT_COLOR);
}

/*******************************************************************************
* @brief Set the focus theme color.
* @param  r  red value.
* @param  g  green value.
* @param  b  blue value.
*******************************************************************************/
void lv_sys_set_focus_theme_color(uint8_t r, uint8_t g, uint8_t b)
{
    color[3] = r;
    color[4] = g;
    color[5] = b;
    lv_style_set_bg_color(&lv_sys_foucus_style, LV_THEME_FOCUS_COLOR);
    lv_style_set_border_color(&lv_sys_foucus_style, LV_THEME_FOCUS_COLOR);
}

/*******************************************************************************
* @brief Get the default theme color.
* @return the default theme color.
*******************************************************************************/
inline lv_color_t lv_sys_get_default_theme_color(void)
{
    return LV_THEME_DEFAULT_COLOR;
}

/*******************************************************************************
* @brief Get the focus theme color.
* @return the focus theme color.
*******************************************************************************/
inline lv_color_t lv_sys_get_focus_theme_color(void)
{
    return LV_THEME_FOCUS_COLOR;
}

/*******************************************************************************
* @brief Get the system language.
* @return the system language.
*******************************************************************************/
inline lv_language_t lv_sys_get_language(void)
{
    return lv_language;
}

/*******************************************************************************
* @brief Set the system language.
*******************************************************************************/
inline void lv_sys_set_language(lv_language_t lang)
{
    lv_language = lang;
}

/*******************************************************************************
* @brief Get the current indev value.
* @return the current indev value.
*******************************************************************************/
inline uint32_t lv_sys_get_indev_value(void)
{
    return lv_indev_get_key(lv_sys_indev);
}

/*******************************************************************************
* @brief Set the keypad mode.
* @param  mode  the keypad mode.It can be the following values:
*         - LV_KEYPAD_MODE_NAIV: naive mode.
*         - LV_KEYPAD_MODE_EDIT: edit mode.
*******************************************************************************/
inline void lv_sys_set_keypad_mode(lv_keypad_t mode)
{
    keypad_mode = mode;
}

// static uint8_t lv_sys_keypad_val[][3] = {
//     {LV_KEY_ENTER, LV_KEY_NEXT, LV_KEY_PREV},
//     {LV_KEY_ENTER, LV_KEY_UP,   LV_KEY_DOWN},
// }

// void keypad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
// {
//     static uint8_t last_key = 0;
//     int8_t act_key = bsp_btn_get_value();

//     if(NONE_KEY != act_key)
//     {
//         data->state = LV_INDEV_STATE_PR;
//         switch(act_key)
//         {
//             case KEY1_PRESSED: act_key = lv_sys_keypad_val[keypad_mode][0]; break;
//             case KEY2_PRESSED: act_key = lv_sys_keypad_val[keypad_mode][1]; break;
//             case KEY3_PRESSED: act_key = lv_sys_keypad_val[keypad_mode][2]; break;
//             case KEY4_PRESSED: act_key = lv_sys_keypad_val[keypad_mode][3]; break;
//         }
//         last_key = act_key;
//     }
//     else
//     {
//         data->state = LV_INDEV_STATE_REL;
//     }

//     data->key = last_key;
// }
/********************************** END ***************************************/
