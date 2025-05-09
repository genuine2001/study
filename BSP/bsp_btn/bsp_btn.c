/*
********************************************************************************
*
*    文件名称 : bsp_btn.c
*    库 依 赖 : sys.h
*    说    明 : 扫描独立按键，具有按键FIFO，可以实现如下功能：
*               - 按键单击
*               - 按键双击
*               - 按键长按
*               - 按键持续按下
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
#define PRESSED  1
#define RELEASED 0

#define BTN_CLICKED_VALUE          (uint8_t)(btn->id << 2 | BTN_CLICKED)
#define BTN_DOUBLE_CLICK_VALUE     (uint8_t)(btn->id << 2 | BTN_DOUBLE_CLICK)
#define BTN_LONG_PRESS_VALUE       (uint8_t)(btn->id << 2 | BTN_LONG_PRESS)
#define BTN_LONG_PRESS_HOLD_VALUE  (uint8_t)(btn->id << 2 | BTN_LONG_PRESS_HOLD)
/******************************* DEFINES **************************************/

/***************************** DECLARATIONS ***********************************/
typedef enum
{
    BTN_CLICKED          = 0,  /* btn is clicked         */
    BTN_DOUBLE_CLICK     = 1,  /* btn is double click    */
    BTN_LONG_PRESS       = 2,  /* btn is long press      */
    BTN_LONG_PRESS_HOLD  = 3,  /* btn is long press hold */
    BTN_IDLE             = 4,  /* btn is idle            */
    BTN_PRESSED          = 5,  /* btn is pressed         */
    BTN_RELEASED         = 6,  /* btn is released        */
} bsp_status_t;

static bsp_btn_t* btn_list = NULL; 
static sys_fifo_t btn_fifo;
static uint8_t btn_fifo_buffer[BTN_FIFO_SIZE];

/***************************** DECLARATIONS ***********************************/

void bsp_btn_init(void)
{
    sys_fifo_init(&btn_fifo, btn_fifo_buffer, BTN_FIFO_SIZE, 1);
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
    btn->pin_level         = 0;
    btn->filter_time       = 0;
    btn->ticks             = 0;
    btn->click_cnt         = 0;
    btn->id                = btn_list ? btn_list->id + 1 : 0;
    btn->next              = NULL;
    
    for (int i = 0; i < BTN_EVENT_NUM; i++)
    {
        btn->btn_callback[i] = NULL;
    }

    /*	step3,	add the button object to the btn list */
    btn->next = btn_list;
    btn_list  = btn;
}

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
        if(PRESSED == pin_level)
        {
            btn->status = BTN_PRESSED;
        }
        break;

    case BTN_PRESSED:
        if(PRESSED == pin_level && btn->ticks >= BTN_LONG_PRESS_TIME)
        {
            btn->status = BTN_LONG_PRESS;
        }
        else if(RELEASED == pin_level)
        {
            btn->status = BTN_RELEASED;
        } 
        break;
    
    case BTN_RELEASED:
        btn->ticks  = 0;
        btn->status = btn->click_cnt ? BTN_DOUBLE_CLICK : BTN_CLICKED;
        break;
    case BTN_CLICKED:
        if(PRESSED == pin_level && btn->ticks <= BTN_DOUBLE_CLICK_TIME)
        {
            btn->click_cnt++;
            btn->status = BTN_PRESSED;
        }
        else if(RELEASED == pin_level && btn->ticks > BTN_DOUBLE_CLICK_TIME)
        {
            key_value = BTN_CLICKED_VALUE;
            sys_fifo_push(&btn_fifo, &key_value);
            btn->status = BTN_IDLE;
        }
        break;
    case BTN_DOUBLE_CLICK:
        btn->click_cnt = 0;
        key_value = BTN_DOUBLE_CLICK_VALUE;
        sys_fifo_push(&btn_fifo, &key_value);
        btn->status = BTN_IDLE;
        break;
    case BTN_LONG_PRESS: 
        btn->ticks = 0;
        key_value = BTN_LONG_PRESS_VALUE;
        sys_fifo_push(&btn_fifo, &key_value);
        btn->status = pin_level ? BTN_LONG_PRESS_HOLD : BTN_IDLE;
        break;
    case BTN_LONG_PRESS_HOLD: 
        if( PRESSED == pin_level && !(btn->ticks%BTN_LONG_REPEAT_TIME) )
        {
            key_value = BTN_LONG_PRESS_HOLD_VALUE;
            sys_fifo_push(&btn_fifo, &key_value);
        }
        else if(RELEASED == pin_level)
        {
            btn->status = BTN_IDLE;
        }
        break;
    default: 
        btn->status = BTN_IDLE;
        break;
    }
}

static void bsp_btn_scan(void)
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
}

void bsp_btn_handle(void)
{
    int i = -1;
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

int bsp_btn_get_value(void)
{
    int key_value = -1;
    sys_fifo_pop(&btn_fifo, &key_value);
    return key_value;
}