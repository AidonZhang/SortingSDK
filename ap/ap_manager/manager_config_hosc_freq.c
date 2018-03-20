/*******************************************************************************
 *                              us212A
 *                            Module: manager
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-10-8 11:21     1.0             build this file
 *******************************************************************************/
/*!
 * \file     manager_config.c
 * \brief    应用配置脚本操作接口实现
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了一套简单的应用配置接口，包括读取配置项等。
 * \par      EXTERNALIZED FUNCTIONS:
 *               直接调用sd 文件系统接口进行定位和读取。
 * \version 1.0
 * \date  2011-10-8
 *******************************************************************************/

#include  "manager.h"

//HOSC 电容配置
void manager_config_hosc_freq(void)
{

#ifndef _CARD_BASE_
    int32 ret_val;
    uint32 trim_cap_value = 0;    
    uint32 adjust_value;   

    ret_val = base_trim_cap_read(&trim_cap_value, RW_TRIM_CAP_SNOR);

    if(ret_val == TRIM_CAP_READ_NO_ERROR)
    {
        ;//读取到正确的频偏数值              
    }
    else if(ret_val == TRIM_CAP_READ_ADJUST_VALUE)
    {
        //读取到频偏校准值，需要加上偏移量
        com_get_config_struct(SETTING_HARDWARE_FREQ_COMPENSATION_OFFSET, (void*) &adjust_value, 4);

        if(adjust_value > 0x1FFFE)
        {
            //参数无效
            return;
        }

        //频偏偏移值范围为-65535-65535,为避免出现负数，將所有数值
        //先加上65535,所以真实的数值要减去65535
        trim_cap_value = trim_cap_value + adjust_value - 65535;
        
        if(trim_cap_value > 0x10000)
        {
            //参数无效
            return;
        }
        
    }
    else
    {
        com_get_config_struct(SETTING_HARDWARE_FREQ_COMPENSATION_DEFAULT_VAL, (void*) &trim_cap_value, 4);
    }

    //读取到正确的频偏数值
    libc_print("set hosc value:", trim_cap_value, 2);

    sys_set_hosc_param(trim_cap_value);    
#endif

}

