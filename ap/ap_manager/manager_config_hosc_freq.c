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
 * \brief    Ӧ�����ýű������ӿ�ʵ��
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ʵ����һ�׼򵥵�Ӧ�����ýӿڣ�������ȡ������ȡ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               ֱ�ӵ���sd �ļ�ϵͳ�ӿڽ��ж�λ�Ͷ�ȡ��
 * \version 1.0
 * \date  2011-10-8
 *******************************************************************************/

#include  "manager.h"

//HOSC ��������
void manager_config_hosc_freq(void)
{

#ifndef _CARD_BASE_
    int32 ret_val;
    uint32 trim_cap_value = 0;    
    uint32 adjust_value;   

    ret_val = base_trim_cap_read(&trim_cap_value, RW_TRIM_CAP_SNOR);

    if(ret_val == TRIM_CAP_READ_NO_ERROR)
    {
        ;//��ȡ����ȷ��Ƶƫ��ֵ              
    }
    else if(ret_val == TRIM_CAP_READ_ADJUST_VALUE)
    {
        //��ȡ��ƵƫУ׼ֵ����Ҫ����ƫ����
        com_get_config_struct(SETTING_HARDWARE_FREQ_COMPENSATION_OFFSET, (void*) &adjust_value, 4);

        if(adjust_value > 0x1FFFE)
        {
            //������Ч
            return;
        }

        //Ƶƫƫ��ֵ��ΧΪ-65535-65535,Ϊ������ָ�������������ֵ
        //�ȼ���65535,������ʵ����ֵҪ��ȥ65535
        trim_cap_value = trim_cap_value + adjust_value - 65535;
        
        if(trim_cap_value > 0x10000)
        {
            //������Ч
            return;
        }
        
    }
    else
    {
        com_get_config_struct(SETTING_HARDWARE_FREQ_COMPENSATION_DEFAULT_VAL, (void*) &trim_cap_value, 4);
    }

    //��ȡ����ȷ��Ƶƫ��ֵ
    libc_print("set hosc value:", trim_cap_value, 2);

    sys_set_hosc_param(trim_cap_value);    
#endif

}

