/*
********************************************************************************
*    文件名称 : lv_sys.h
*    库 依 赖 : lvgl.h
*    说    明 : 头文件声明
*    版    本 : V1.0
*
*    Ver: 1.0    Date: 2025-05-07     Author: xzt
*
*    Copyright (C), All Rights Reserved.
*    Note    :  1tab = 4 spaces!
********************************************************************************
*/
#ifndef __LV_SYS_H__
#define __LV_SYS_H__
/******************************* INCLUDES *************************************/
#include "lvgl.h"
#include "lv_sys_font.h"
#include "lv_sys_img.h"
#include "lv_sys_app.h"
/******************************* INCLUDES *************************************/
/******************************* DEFINES **************************************/
#define USE_SIMULATOR           1    /* Enable simulator.                     */
 
#define LV_SYS_WIDTH            240  /* Width of the system display           */
#define LV_SYS_HEIGHT           240  /* Height of the system display          */

/* Define the data types used for user data */
#define LV_UINT(x)              (void *)(uintptr_t)(x) 
/******************************* DEFINES **************************************/
/***************************** DECLARATIONS ***********************************/
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
    LV_KEYPAD_MODE_NAIV = 0, /* Navigation Mode */
    LV_KEYPAD_MODE_EDIT = 1, /* Edit Mode       */
} lv_keypad_t;

/***************************** DECLARATIONS ***********************************/
/**************************** APIs of LV_SYS **********************************/
void lv_sys_init(void);

void lv_sys_set_default_theme_color(uint8_t r, uint8_t g, uint8_t b);
void lv_sys_set_focus_theme_color(uint8_t r, uint8_t g, uint8_t b);

extern inline lv_color_t lv_sys_get_default_theme_color(void);
extern inline lv_color_t lv_sys_get_focus_theme_color(void);

extern inline void lv_sys_set_default_style(lv_obj_t *obj);
extern inline void lv_sys_set_focus_style(lv_obj_t *obj);

extern inline lv_language_t lv_sys_get_language(void);
extern inline void lv_sys_set_language(lv_language_t lang);

extern inline uint32_t lv_sys_get_indev_value(void);
extern inline void lv_sys_set_keypad_mode(lv_keypad_t mode);
/********************************** END ***************************************/
#endif /* __LV_SYS_H__ */
