/*
********************************************************************************
*    文件名称 : bsp_btn.c
*    库 依 赖 : sys.h
*    说    明 : 扫描独立按键，具有按键FIFO，可以实现如下功能：
*               - 按键单击
*               - 按键双击
*               - 按键长按
*               - 按键持续按下
*               - 能通过标志位设置是否获取除单击之外的按键状态
*
*    Ver: 1.0    Date: 2025-02-10     Author: xzt
*   
*    Copyright (C), All Rights Reserved.
*    Note    :  1tab = 4 spaces!
********************************************************************************
*/
/******************************* INCLUDES *************************************/
#include "bsp_btn.h"
#include "sys.h"
/******************************* INCLUDES *************************************/
/******************************* DEFINES **************************************/
#define IS_PRESSED                 pin_level == 1
#define IS_RELEASED                pin_level == 0
#define IS_SINGLE_CLICK            btn->ticks >= BTN_DOUBLE_CLICK_TIME
#define IS_DOUBLE_CLICK            btn->ticks <= BTN_DOUBLE_CLICK_TIME
#define IS_LONG_PRESSED            btn->ticks >= BTN_LONG_PRESS_TIME
#define IS_LONG_REPEAT             btn->ticks >= BTN_LONG_REPEAT_TIME

#define BTN_CLICKED_VALUE          (uint8_t)(btn->id << 2 | BTN_CLICKED)
#define BTN_DOUBLE_CLICK_VALUE     (uint8_t)(btn->id << 2 | BTN_DOUBLE_CLICK)
#define BTN_LONG_PRESS_VALUE       (uint8_t)(btn->id << 2 | BTN_LONG_PRESS)
#define BTN_LONG_PRESS_HOLD_VALUE  (uint8_t)(btn->id << 2 | BTN_LONG_PRESS_HOLD)

#define BTN_HAS_DOBLE_STATUS       (btn->flag & (1 << BTN_DOUBLE_CLICK))
#define BTN_HAS_LONG_STATUS        (btn->flag & (1 << BTN_LONG_PRESS))
#define BTN_HAS_LONG_HOLD_STATUS   (btn->flag & (1 << BTN_LONG_PRESS_HOLD))
/******************************* DEFINES **************************************/
/***************************** DECLARATIONS ***********************************/
static bsp_btn_t* btn_list = NULL; 
static sys_fifo_t btn_fifo;
static uint8_t btn_fifo_buffer[BTN_FIFO_SIZE];
/***************************** DECLARATIONS ***********************************/

/*******************************************************************************
* @brief init the btn fifo struct
*
* @note the buffer size should be an integer power of 2
*******************************************************************************/
void bsp_btn_init(void)
{
    sys_fifo_init(&btn_fifo, btn_fifo_buffer, BTN_FIFO_SIZE, 1);
}

/*******************************************************************************
* @brief handle the button status machine
*
* @param btn: pointer of the btn struct
*******************************************************************************/
static void bsp_btn_machine(bsp_btn_t *btn)
{
    /*	step1,	debounce the button */
    uint8_t key_value;
    uint8_t pin_level = btn->pf_is_btn_down();
    if(pin_level != btn->pin_level)
    {
        btn->filter_time++;
        if(btn->filter_time >= BTN_FILTER_TIME)
        {
            btn->pin_level = pin_level;
            btn->filter_time = 0;
        }
        return;
    }
    else 
    {
        btn->filter_time = 0;
    }

    /*	step2,	btn ticks plus one */
    btn->ticks++;

    /*	step2,	handle the button status */
    switch(btn->status){
    case BTN_IDLE:
        btn->ticks = 0;
        btn->click_cnt = 0;
        if(IS_PRESSED)
        {
            btn->status = BTN_PRESSED;
        }
        break;
    case BTN_PRESSED:
        if(IS_PRESSED && IS_LONG_PRESSED && BTN_HAS_LONG_STATUS)
        {
            key_value = BTN_LONG_PRESS_VALUE;
            sys_fifo_push(&btn_fifo, &key_value);
            btn->status = BTN_LONG_PRESS;
        }
        else if(IS_PRESSED && IS_LONG_REPEAT && BTN_HAS_LONG_HOLD_STATUS)
        {
            btn->ticks = 0;
            key_value = BTN_LONG_PRESS_HOLD_VALUE;
            sys_fifo_push(&btn_fifo, &key_value);
            btn->status = BTN_LONG_PRESS_HOLD;
        }
        else if(IS_RELEASED)
        {
            btn->status = BTN_RELEASED;
        } 
        break;
    case BTN_RELEASED:
        btn->ticks  = 0;
        btn->status = btn->click_cnt ? BTN_DOUBLE_CLICK : BTN_CLICKED;
        break;
    case BTN_CLICKED:
        if(IS_PRESSED && IS_DOUBLE_CLICK && BTN_HAS_DOBLE_STATUS)
        {
            btn->click_cnt++;
            btn->status = BTN_PRESSED;
        }
        else if(IS_RELEASED && IS_SINGLE_CLICK)
        {
            key_value = BTN_CLICKED_VALUE;
            sys_fifo_push(&btn_fifo, &key_value);
            btn->status = BTN_IDLE;
        }
        break;
    case BTN_DOUBLE_CLICK:
        key_value = BTN_DOUBLE_CLICK_VALUE;
        sys_fifo_push(&btn_fifo, &key_value);
        btn->status = BTN_IDLE;
        break;
    case BTN_LONG_PRESS: 
        btn->status = btn->pin_level ? BTN_LONG_PRESS : BTN_IDLE;
        break;
    case BTN_LONG_PRESS_HOLD: 
        if(IS_PRESSED && !(btn->ticks%BTN_LONG_REPEAT_TIME))
        {
            key_value = BTN_LONG_PRESS_HOLD_VALUE;
            sys_fifo_push(&btn_fifo, &key_value);
        }
        else if(IS_RELEASED)
        {
            btn->status = BTN_IDLE;
        }
        break;
    default: 
        btn->status = BTN_IDLE;
        break;
    }
}

/*******************************************************************************
* @brief scan all button and handle the button status machine
*******************************************************************************/
void bsp_btn_scan(void)
{
    /*	step1,	get the button object       */
    bsp_btn_t *btn = btn_list;

    /*	step2,	scan all button             */
    while(NULL != btn)
    {
        /*	step2.1,	handle the button status */
        bsp_btn_machine(btn);

        /*	step2.2,	scan next button         */
        btn = btn->next;
    }
}

#if BTN_USE_EVENT
/*******************************************************************************
* @brief run the button handle function
*******************************************************************************/
void bsp_btn_handle(void)
{
    int8_t i = -1;
    /*	step1,	get the event from the fifo */
    sys_fifo_pop(&btn_fifo, &i);

    /*	step2,	check the btn fifo is empty */
    if (-1 == i)
    {
        return;
    }

    uint8_t btn_num = i >> 2;      /* get the button number */
    uint8_t event_type = i & 0x03; /* get the event type    */

    /*	step3,	get the button object */
    bsp_btn_t *btn = btn_list;
    while(btn != NULL && btn->id != btn_num)
    {
        btn = btn->next;
    }

    /*	step4,	call the callback function */
    if(btn != NULL && btn->btn_callback[event_type] != NULL)
    {
        btn->btn_callback[event_type]();
    }
}
#endif

/*******************************************************************************
* @brief add the btn into the btn list
*
* @param pf_is_btn_down: the function pointer of the button is pressed or not
*                        1: pressed, 0: released
*******************************************************************************/
void bsp_btn_add(bsp_btn_t *btn, uint8_t (*pf_is_btn_down)(void))
{
    /*	step1,	initialize the button object */
    btn->status            = BTN_IDLE;
    btn->pf_is_btn_down    = pf_is_btn_down;
    btn->pin_level         = 0;
    btn->filter_time       = 0;
    btn->ticks             = 0;
    btn->click_cnt         = 0;
    btn->id                = btn_list ? btn_list->id + 1 : 0;
    btn->flag              = 0;
    btn->next              = NULL;
    
    for (int i = 0; i < BTN_EVENT_NUM; i++)
    {
        btn->btn_callback[i] = NULL;
    }

    /*	step2,	add the button object to the btn list */
    btn->next = btn_list;
    btn_list  = btn;
}

#if BTN_USE_EVENT
/*******************************************************************************
* @brief Add event functions to the button state
*
* @param btn: pointer of the btn struct
* @param status: The status of the event needs to be added
* @param btn_cb: event function pointer
*******************************************************************************/
void bsp_btn_add_event(bsp_btn_t *btn, uint8_t status, void(*btn_cb)(void))
{
    btn->btn_callback[status] = btn_cb;
}
#endif

/*******************************************************************************
* @brief Add statuses to the button object
*
* @param btn: pointer of the btn struct
* @param status: The states that need to be added
*
* @note By default, the button is only in the single-click state
* @note The status can be one or more of the following:
*       BTN_DOUBLE_CLICK
*       BTN_LONG_PRESS
*       BTN_LONG_PRESS_HOLD
*******************************************************************************/
void bsp_btn_add_status(bsp_btn_t *btn, uint8_t status)
{
    btn->flag |= (1 << status);
}

/*******************************************************************************
* @brief Remove statuses from the button object
*
* @param btn: pointer of the btn struct
* @param status: The states that need to be removed
*
* @note The click status cannot be removed
* @note The status can be one or more of the following:
*       BTN_DOUBLE_CLICK
*       BTN_LONG_PRESS
*       BTN_LONG_PRESS_HOLD
*******************************************************************************/
void bsp_btn_remove_status(bsp_btn_t *btn, uint8_t status)
{
    btn->flag &= ~(1 << status);
}

/*******************************************************************************
* @brief Return the key value of the button
*
* @return -1：no key value
*         others：key value
*******************************************************************************/
int8_t bsp_btn_get_value(void)
{
    int8_t key_value = -1;
    sys_fifo_pop(&btn_fifo, &key_value);
    return key_value;
}
/*********************************** END **************************************/
