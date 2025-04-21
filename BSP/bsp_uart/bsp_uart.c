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
#include "bsp_uart.h"
/****************************** 函数声明 *************************************/

/*
*******************************************************************************
*    功能说明: 关闭半主机模式使得能不使用微库来重定向printf
*******************************************************************************
*/
#pragma import(__use_no_semihosting)             

// 使用HAL库报错时增添此函数
#if USE_HAL
int _ttywrch(int ch)    
{
    ch=ch;
	return ch;
}
#endif

struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       

void _sys_exit(int x) 
{ 
	x = x; 
} 

extern UART_HandleTypeDef huart1;
int fputc(int ch, FILE *f)
{ 	
	/* 增填自己的发送函数，例如串口发送 */
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET)
	return ch;
}

/*
*******************************************************************************
*    函 数 名:  HAL_UARTEx_RxEventCallback
*    功能说明:  UART接收中断回调函数
*    形    参:  huart : 句柄
*               Size  : 接收字节数
*    说    明 : 主要是为了在DMA接收数据时，对环形缓冲区的写指针进行偏移
*******************************************************************************
*/
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (&huart1 == huart)
	{
	    uint16_t buffer_head;
		int16_t head_offset;

		/*	step1,	获取当前接收数组的写指针位置                                 */
		buffer_head = buff1->pfget_head(buff1);

		/*	step2,	判断是否是DMA半满中断          								 */                      
		if ((BUFFER_SIZE >> 1) == Size)
		{
			head_offset = (BUFFER_SIZE >> 1) - buffer_head;
			/*	step2.1,	执行DMA半满中断操作         					     */ 
			
		}

		/*	step3,	判断是否是DMA全满中断										 */
		else if (BUFFER_SIZE == Size)
		{
			head_offset = BUFFER_SIZE  - buffer_head;
			/*	step3.1,	执行DMA全满中断操作         					     */ 
			
		}

		/*	step4,	判断是否是空闲中断											 */
		else
		{
			head_offset = Size - (buffer_head % BUFFER_SIZE);
			/*	step4.1,	执行空闲中断操作         					         */ 
		}

		/*	step5,	偏移写指针位置                                               */
		if(head_offset < 0)
		{
				head_offset += BUFFER_SIZE;
		}
		head_offset = head_offset;
		buff1->pfhead_offset(buff1, head_offset);
		
		/*	step6,	接收完毕标志位置1                                            */
		rx_flag = 1;
	}
}
