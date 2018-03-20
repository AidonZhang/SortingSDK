/********************************************************************************
 *                            Module: radio_engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * 描述:			Dac给case层的接口
********************************************************************************/



#ifndef _DAC_INTERFACE_H         
#define _DAC_INTERFACE_H

#include "psp_includes.h"


/* dac 驱动命令枚举类型*/
typedef enum
{
		DAC_INIT = 0,
		DAC_SET_VOLUME,
		DAC_MUTE,
		DAC_UNMUTE,
		DAC_STANDBY,
        DAC_ACTIVE,
        DAC_MODE,
		
}dac_cmd_e;

/* dac 驱动向上层提供的统一入口定义*/
typedef void* (*dac_op_func)(void*, void*, void*);


typedef struct
{
    dac_op_func  DAC_INIT;
    dac_op_func  DAC_SET_VOLUME;
    dac_op_func  DAC_MUTE;
     dac_op_func  DAC_UNMUTE;
    dac_op_func  DAC_STANDBY;
    dac_op_func  DAC_ACTIVE;
     dac_op_func  DAC_MODE;
}dac_driver_operations;

extern void* dac_op_entry(void *param1, void *param2, void *param3, dac_cmd_e cmd) __FAR__;

#define dac_open()  (int)dac_op_entry((void*)(uint32)(0), (void*)(uint32)(0), (void*)(uint32)(0), DAC_INIT) 
#define dac_set_volume(a) (int)dac_op_entry((void*)(uint32)(a), (void*)(uint32)(0), (void*)(uint32)(0), DAC_SET_VOLUME) 
#define dac_mute() (int)dac_op_entry((void*)(uint32)(0), (void*)(uint32)(0), (void*)(uint32)(0), DAC_MUTE)
#define dac_unmute() (int)dac_op_entry((void*)(uint32)(0), (void*)(uint32)(0), (void*)(uint32)(0), DAC_UNMUTE)

#define dac_standby() (int)dac_op_entry((void*)(uint32)(0), (void*)(uint32)(0), (void*)(uint32)(0), DAC_STANDBY)
#define dac_active() (int)dac_op_entry((void*)(uint32)(0), (void*)(uint32)(0), (void*)(uint32)(0), DAC_ACTIVE)
#define dac_mode(r) (int)dac_op_entry((void*)(uint32)(r), (void*)(uint32)(0), (void*)(uint32)(0), DAC_ACTIVE)
//#define MAX( a, b) ( libc_print("test", a, b) ) 测试用

#endif 
