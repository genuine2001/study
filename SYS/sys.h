#ifndef SYS_H
#define SYS_H
/*
*******************************************************************************
*
*    文件名称 : sys.h
*    说    明 : 作用于整个系统的头文件
*    版    本 : V2.0
*    修改记录 :
*        版本号    日期           作者     说明
*        V1.0     2024-12-31    genuine   创建文件
*        V1.1     2024-12-31    genuine   增加了一个宏定义用于创建对象时申请
*                                         内存并返回对象指针
*        V2.0     2025-01-01    genuine   增加并修改宏定义用于构建面向对象C代码，
*                                         添加了环形缓冲区的接口和类声明
*    Copyright (C), genuine, All Rights Reserved.
*    Note    :  1tab = 4 spaces!
*
*******************************************************************************
*/

/******************************* 头文件 **************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/* 添加自己的芯片头文件*/

/******************************* 宏定义 **************************************/
/*	构建宏定义使得能轻易写出面向对象的C代码	*/
#define CLASS(type) \
typedef struct type type;\
struct type
	
#define INTERFACE(type)             \
typedef struct type type;           \
struct type
	
#define IMPLEMENTS(type) struct type type
	
#define FUNCTION_SETTINF(f1, f2) cthis->f1 = f2;

#define CTOR(type) \
type *type##_create(void) \
{ \
    struct type *cthis;\
    cthis = (struct type *)malloc(sizeof(struct type));\
    if(NULL == cthis) return NULL;
	
#define END_CTOR return cthis;}
	
#define buff_type uint8_t

/****************************** 类型声明 *************************************/
/* 环形缓冲区的接口 */
INTERFACE(cycle_buffer_t)
{
    void      (*pfinit)(void *cb, buff_type* buffer);
    void      (*pfwrite)(void *cb, buff_type data);
    void      (*pfhead_offset)(void *cb, uint16_t offset);
    uint8_t   (*pfisempty)(void *cb);
    buff_type (*pfread)(void *cb);
    uint16_t  (*pfget_head)(void *cb);
}; 

/****************************** 函数声明 *************************************/
void sys_dwt_init(void);
void sys_delay_us(uint32_t us);
void sys_delay_ms(uint32_t ms);

#endif /* SYS_H */
