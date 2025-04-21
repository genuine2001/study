/*
********************************************************************************
*
*    文件名称 : bsp_btn.c
*    库 依 赖 : sys.h
*    说    明 : 扫描独立按键，具有按键FIFO，可以实现如下功能：
*               - 按键单击
*               - 按键双击
*               - 按键长按
*
*    Copyright (C), All Rights Reserved.
*    Note    :  1tab = 4 spaces!
*
********************************************************************************
*/
/******************************* INCLUDES *************************************/
#include "bsp_btn.h"
#include "sys.h"
/******************************* INCLUDES *************************************/

/******************************* INCLUDES *************************************/

/******************************* DEFINES **************************************/
#define BTN_PRINTF(x)                printf(x)
#define BTN_SET_DOUBLE_FLAG(x, y)    x |  (1 << x)
#define BTN_CLEAN_DOUBLE_FLAG(x, y)  x & ~(1 << y)
#define BTN_GET_DOUBLE_FLAG(x, y)    x &  (1 << y)
/******************************* DEFINES **************************************/

/***************************** DECLARATIONS ***********************************/
typedef struct 
{
    uint8_t                   num;
    sys_fifo_t               fifo; /* button fifo         */
    bsp_btn_t   *btn[BSP_BTN_NUM]; /* button object array */
} bsp_btn_manager_t;

static uint16_t double_click_flag = 0;
static uint8_t fifo_buffer[BSP_BTN_FIFO_SIZE] = {0};
static bsp_btn_manager_t bsp_btn_manager = {0};

/***************************** DECLARATIONS ***********************************/

void bsp_btn_init(void)
{
    sys_fifo_init(&bsp_btn_manager.fifo, fifo_buffer, BSP_BTN_FIFO_SIZE, 1);
}

void bsp_add_btn(bsp_btn_t *btn, uint8_t (*pf_is_btn_down)(void))
{
#if BTN_DEBUG
    /*	step1,	check the parameters */
    if (NULL == btn || NULL == pf_is_btn_down)
    {
        BTN_PRINTF("bsp_add_btn: invalid parameters");
        return;
    }
#endif /* BTN_DEBUG */

    /*	step2,	initialize the button object */
    btn->status            = BTN_IDLE;
    btn->pf_is_btn_down    = pf_is_btn_down;
    btn->filter_time       = 0;
    btn->double_click_time = 0;
    btn->hold_time         = 0;

    for (int i = 0; i < BSP_BTN_EVENT_NUM; i++)
    {
        btn->btn_callback[i] = NULL;
    }

    /*	step3,	add the button object to the manager */
    bsp_btn_manager.btn[bsp_btn_manager.num] = btn;
    bsp_btn_manager.num++;

#if BTN_DEBUG
    BTN_PRINTF("bsp_add_btn: success");
#endif /* BTN_DEBUG */
}

static void bsp_detect_btn(uint8_t num)
{
    /*	step1,	get the button object       */
    bsp_btn_t *btn = bsp_btn_manager.btn[num];

    /*	step2,	check the button is pressed */
    if (btn->pf_is_btn_down())
    {
        /*	step2.1, dithering elimination	*/
        if(BSP_BTN_FILTER_TIME > btn->filter_time)
        {
            btn->filter_time++;
            return;
        }
        /*	step2.2, button is pressed	*/
        else if(btn->status == BTN_IDLE)
        {
            btn->status = BTN_PRESSED;
        }
        /*	step2.3, button is hold	*/
        else if(btn->status == BTN_PRESSED)
        {
            if(BSP_BTN_HOLD_TIME > btn->hold_time)
            {
                btn->hold_time++;
                return;
            }
            else
            {
                btn->status = BTN_LONG_PRESS;
            }
        }
    }

    /*	step3,	button is released	*/
    else
    {
        /*	step3.1, button is not idle	*/
        if(BTN_IDLE != btn->status)
        {
            if(0 != btn->filter_time)
            {
                btn->filter_time--;
                return;
            }
            if(BTN_PRESSED == btn->status)
            {
                sys_fifo_push(&bsp_btn_manager.fifo, (num << 4) | BTN_CLICKED);
            }
            else if(BTN_LONG_PRESS == btn->status)
            {
                sys_fifo_push(&bsp_btn_manager.fifo, (num << 4) | BTN_LONG_PRESS);
            }
            btn->status = BTN_IDLE;
            double_click_flag = BTN_SET_DOUBLE_FLAG(num);
            btn->double_click_time++;
        }

        /*	step3.2, reset the hold time	*/
        btn->hold_time = 0;
        if(BTN_GET_DOUBLE_FLAG(double_click_flag, num)) 
        {
            btn->double_click_time++;
            if(btn->double_click_time > BSP_BTN_DOUBLE_CLICK_TIME)
            {
                double_click_flag = BTN_CLEAN_DOUBLE_FLAG(double_click_flag, num);
                btn->double_click_time = 0;
            }
        }
    }
}

void bsp_btn_scan(void)
{
    /*	step1,	scan all buttons	*/
    for (int i = 0; i < bsp_btn_manager.num; i++)
    {
        bsp_detect_btn(i);
    }
}

void bsp_btn_add_event(bsp_btn_t *btn, bsp_status_t status,
                       void (*btn_callback)(void))
{
#if BTN_DEBUG
    /*	step1,	check the parameters */
    if (NULL == btn || NULL == btn_callback)
    {
        BTN_PRINTF("bsp_btn_add_event: invalid parameters");
        return;
    }
    BTN_PRINTF("bsp_btn_add_event: start");
#endif /* BTN_DEBUG */

    /*	step2,	add the event to the button object */
    btn->btn_callback[status] = btn_callback;

#if BTN_DEBUG
    BTN_PRINTF("bsp_btn_add_event: success");
#endif /* BTN_DEBUG */
}

void bsp_btn_handle(void)
{
    int i = -1;
    /*	step1,	get the event from the fifo */
    sys_fifo_pop(&bsp_btn_manager.fifo, &i);

    /*	step2,	check the btn fifo is empty */
    if (-1 == i)
    {
        return;
    }
    uint8_t btn_num = i >> 4;      /* get the button number */
    uint8_t event_type = i & 0x0F; /* get the event type    */

    /*	step3,	get the button object */
    bsp_btn_t *btn = bsp_btn_manager.btn[btn_num];

    /*	step4,	call the callback function */
    btn->btn_callback[event_type]();
}

int bsp_btn_get_value(void)
{
    int key_value = -1;
    sys_fifo_pop(&bsp_btn_manager.fifo, &key_value);
    return key_value;
}