#ifndef __BSP_UART_H__
#define __BSP_UART_H__
/*
*******************************************************************************
*
*    文件名称 : 
*    说    明 : 
*    版    本 : V1.0
*    修改记录 :
*        版本号    日期           作者     说明
*        V1.0     2025-01-27    genuine   创建文件
*
*    Copyright (C), genuine, All Rights Reserved.
*    Note    :  1tab = 4 spaces!
*
*******************************************************************************
*/

/******************************* 头文件 **************************************/
#include "sys.h"
/******************************* 宏定义 **************************************/
#define USE_HAL 1
#define BUFFER_SIZE 100
/****************************** 类型声明 *************************************/
/* 定义串口的各种标志位 */
CLASS(UART)
{
    uint8_t          tx_flag : 1;
    uint8_t      tx_irq_flag : 1;
    uint8_t      tx_dma_flag : 1;
    uint8_t          rx_flag : 1;
    uint8_t      rx_irq_flag : 1;
    uint8_t rx_dma_half_flag : 1;
    uint8_t rx_dma_full_flag : 1;
    uint8_t rx_dma_idel_flag : 1;
}


#endif /* __BSP_UART_H__ */