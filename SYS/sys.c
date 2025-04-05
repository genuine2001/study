/*
*******************************************************************************
*
*    文件名称 : sys.c
*    说    明 : 实现环形缓冲区的功能
*    版    本 : V1.0
*    修改记录 :
*        版本号    日期           作者     说明
*        V1.0     2024-12-31    genuine   创建文件
*		 V2.0     2025-01-01    genuine   增加环形缓冲区操作函数
*		 V2.1     2025-01-25    genuine   修改环形缓冲区的初始化函数，直接使用指针
*										  指向缓冲区，而不是申请数组空间，提高效率
*
*    Copyright (C), genuine, All Rights Reserved.
*    Note    :  1tab = 4 spaces!
*
*******************************************************************************
*/

/******************************* 头文件 **************************************/
#include "sys.h"
/******************************* 宏定义 **************************************/
#define  DWT_CYCCNT  *(volatile unsigned int *)0xE0001004
#define  DWT_CR      *(volatile unsigned int *)0xE0001000
#define  DEM_CR      *(volatile unsigned int *)0xE000EDFC

#define  DEM_CR_TRCENA               (1 << 24)
#define  DWT_CR_CYCCNTENA            (1 <<  0)
/****************************** 类型声明 *************************************/
/* 环形缓冲区的类 */
CLASS(cycle_buffer)
{
    IMPLEMENTS(cycle_buffer_t);
    uint8_t    	 head;
    uint8_t       tail;
    uint8_t       size;
    buff_type  *buffer;
};

/****************************** 全局变量 *************************************/

/****************************** 函数声明 *************************************/

/*
*******************************************************************************
*    函 数 名: sys_dwt_init
*    功能说明: 初始化DWT计时器
*    形    参: 无
*    返 回 值: 无
*******************************************************************************
*/
void sys_dwt_init(void)
{
	DEM_CR         |= (unsigned int)DEM_CR_TRCENA;   
	DWT_CYCCNT      = (unsigned int)0u;
	DWT_CR         |= (unsigned int)DWT_CR_CYCCNTENA;
}

/*
*******************************************************************************
*    函 数 名: sys_delay_ms
*    功能说明: 这里的延时采用CPU的内部计数实现us级延时，为了让底层驱动在裸机或  
*              者RTOS环境下有更好的兼容性，且不影响系统的实时性。 
*    形    参: _ulDelayTime : 延时时间，单位us
*    返 回 值: 无
*******************************************************************************
*/
void sys_delay_us(uint32_t _ulDelayTime)
{
    uint32_t tCnt, tDelayCnt;
	uint32_t tStart;
		
	tStart = DWT_CYCCNT;                                    
	tCnt = 0;
	tDelayCnt = _ulDelayTime * (SystemCoreClock / 1000000);		      

	while(tCnt < tDelayCnt)
	{
		tCnt = DWT_CYCCNT - tStart; 
	}
}

/*
*******************************************************************************
*    函 数 名: sys_delay_ms
*    功能说明: 延时函数，单位ms
*    形    参: _ulDelayTime : 延时时间，单位ms
*    返 回 值: 无
*******************************************************************************
*/
void sys_delay_ms(uint32_t _ulDelayTime)
{
	sys_delay_us(1000*_ulDelayTime);
}

/*
*******************************************************************************
*    函 数 名: sys_cycle_buffer_init
*    功能说明:	初始化环形缓冲区
*    形    参:	_pBuffer : 缓冲区指针
*              _ulBufferSize : 缓冲区大小
*******************************************************************************
*/
static void sys_cycle_buffer_init(void* _pBuffer, buff_type* _ulBuffer)
{
	if( NULL == _pBuffer || NULL == _ulBuffer )
	{
		return;
	}
	cycle_buffer* cthis = (cycle_buffer*)_pBuffer;
	cthis->buffer = _ulBuffer;
	cthis->size   = 10;
	cthis->head   = 0;
	cthis->tail   = 0;
}

/*
*******************************************************************************
*    函 数 名: 	sys_cycle_buffer_head
*    功能说明: 	向环形缓冲区写入数据
*    形    参: 	_pBuffer : 缓冲区指针
*              data : 写入的数据
*******************************************************************************
*/
static void sys_cycle_buffer_head(void* _pBuffer, uint8_t data)
{
	if( NULL == _pBuffer )
	{
		return;
	}
	cycle_buffer* cthis = (cycle_buffer*)_pBuffer;
	/*	step1,	将数据通过写指针写入到环形缓冲区				   */
	cthis->buffer[cthis->head] = data;
	/*	step2,	写指针指向下一个空闲位置	                      */
	cthis->head++;
	/*	step3,	如果写指针超出缓冲区的大小，则循环到缓冲区的头部	*/
	if(cthis->head >= cthis->size)
	{
		cthis->head = 0;
	}
}

/*
*******************************************************************************
*    函 数 名:  sys_cycle_buffer_tail
*    功能说明:  从环形缓冲区读取数据
*    形    参: 	_pBuffer : 缓冲区指针
*    返 回 值: 读取的数据
*******************************************************************************
*/
static buff_type sys_cycle_buffer_tail(void* _pBuffer)
{
	if( NULL == _pBuffer )
	{
		return NULL;
	}
	buff_type data = NULL;
	cycle_buffer* cthis = (cycle_buffer*)_pBuffer;
	/*	step1,	判断读指针是否等于写指针，等于则表示缓冲区为空 */
	if(cthis->tail == cthis->head)
	{
		return data;
	}
	/*	step2,	将数据通过读指针读取到环形缓冲区				   */
	data = cthis->buffer[cthis->tail];
	/*	step3,	读指针指向下一个空闲位置	                      */
	cthis->tail++;
	/*	step4,	如果读指针超出缓冲区的大小，则循环到缓冲区的头部	*/
	if(cthis->tail >= cthis->size)
	{
		cthis->tail = 0;
	}
	return data;
}

/*
*******************************************************************************
*    函 数 名: 	sys_cycle_buffer_offset
*    功能说明: 	对环形缓冲区的写指针进行偏移
*    形    参: 	_pBuffer : 缓冲区指针
*              offset : 偏移量
*    Note   :  注意！！！只有使用DMA接收数据时，才需要调用该函数，否则会导致数据错乱。
*******************************************************************************
*/
static void sys_cycle_buffer_offset(void* _pBuffer, uint16_t offset)
{
	if( NULL == _pBuffer )
	{
		return;
	}
	cycle_buffer* cthis = (cycle_buffer*)_pBuffer;
	/*	step1,	将写指针进行偏移				  	   	             */
	cthis->head += offset;
	/*	step2,	如果写指针超出缓冲区的大小，则循环到缓冲区的头部再偏移	*/
	if(cthis->head >= cthis->size)
	{
		cthis->head -= cthis->size;
	}
}

/*
*******************************************************************************
*    函 数 名:  sys_cycle_buffer_get_head
*    功能说明:  获取环形缓冲区的写指针
*    形    参: 	_pBuffer : 缓冲区指针
*    返 回 值:  写指针位置
*******************************************************************************
*/
static uint16_t sys_cycle_buffer_get_head(void* _pBuffer)
{
	if( NULL == _pBuffer )
	{
		return NULL;
	}
	cycle_buffer* cthis = (cycle_buffer*)_pBuffer;
	return cthis->head;
}

static uint8_t sys_cycle_buffer_is_empty(void* _pBuffer)
{
    if( NULL == _pBuffer )
    {
        return NULL;
    }
    cycle_buffer* cthis = (cycle_buffer*)_pBuffer;
    if(cthis->tail == cthis->head)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
*******************************************************************************
*    功能说明: 将环形缓冲区的操作函数赋值给结构体的函数指针
*******************************************************************************
*/
CTOR(cycle_buffer)
	FUNCTION_SETTINF(cycle_buffer_t.pfinit, sys_cycle_buffer_init);
	FUNCTION_SETTINF(cycle_buffer_t.pfwrite, sys_cycle_buffer_head);
	FUNCTION_SETTINF(cycle_buffer_t.pfread, sys_cycle_buffer_tail);
	FUNCTION_SETTINF(cycle_buffer_t.pfhead_offset, sys_cycle_buffer_offset);
	FUNCTION_SETTINF(cycle_buffer_t.pfget_head, sys_cycle_buffer_get_head);
    FUNCTION_SETTINF(cycle_buffer_t.pfisempty, sys_cycle_buffer_is_empty);
END_CTOR
