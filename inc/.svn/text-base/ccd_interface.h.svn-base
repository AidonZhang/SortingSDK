/*
 ********************************************************************************
 *                       ACTOS
 *                  key board message define
 *
 *                (c) Copyright 2002-2013, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File   : 
 * By     : lmx
 * Version: 1> v1.00     first version     2014-9-4 11:17
 ********************************************************************************
 ********************************************************************************
 */

#ifndef _CCD_INTERFACE_H            //防止重定义
#define _CCD_INTERFACE_H

#include "psp_includes.h"
#include "case_independent.h"
#include "common_speaker.h"

typedef enum
{
    CCD_CMD_I2S_PA_POWERON,
    CCD_CMD_I2S_PA_POWERDOWN,
    CCD_CMD_I2S_PA_SET_VOLUME,
    CCD_CMD_I2S_PA_GET_VOLUME,
    CCD_CMD_MAX,
} ccd_cmd_e;

extern void *ccd_op_entry(void *param1, void *param2, void *param3, ccd_cmd_e op_cmd)__FAR__;

//I2S PA 上电并恢复音量
#define ccd_i2s_pa_poweron() \
ccd_op_entry((void *)0,(void *)0, (void *)0, CCD_CMD_I2S_PA_POWERON)

//I2S PA 进入省电模式
#define ccd_i2s_pa_powerdown() \
ccd_op_entry((void *)0,(void *)0, (void *)0, CCD_CMD_I2S_PA_POWERDOWN)

//设置I2S PA音量
//uint8_gain:音量负增益（衰减）(0db ~ -12db)
//uint8_val:要设置的音量值 (0 ~ 31)
#define ccd_i2s_pa_set_volume(uint8_gain, uint8_vol) \
ccd_op_entry((void *)(uint32)(uint8_gain),(void *)(uint32)(uint8_vol),(void *)0, CCD_CMD_I2S_PA_SET_VOLUME)

//获取I2S PA音量 (0 ~ 31)
//uint8_gain:音量负增益（衰减）(0db ~ -12db)
#define ccd_i2s_pa_get_volume(uint8_gain) \
(uint8) (uint32) ccd_op_entry((void *)(uint32)(uint8_gain),(void *)0, (void *)0, CCD_CMD_I2S_PA_GET_VOLUME)

#endif //#ifndef _CCD_INTERFACE_H

