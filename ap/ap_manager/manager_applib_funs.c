/*******************************************************************************
 *                              us212A
 *                            Module: manager
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-28 14:42     1.0             build this file
 *******************************************************************************/
/*!
 * \file     manager_applib_funs.c
 * \brief    applib ���̹���ģ�飨bank���֣�ʵ��
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               ʵ��һ�׻��� ���̹��� �ӿڡ�
 * \par      EXTERNALIZED FUNCTIONS:
 *               ���� libc �� psp �Ľӿ�ʵ�֡�
 * \version 1.0
 * \date  2011-9-28
 *******************************************************************************/

#include "applib.h"
extern bool open_config_file(void);
/******************************************************************************/
/*!
 * \par  Description:
 *    Ӧ���� applib ģ���ϵĳ�ʼ��������Ӧ�õ� main ������ڴ�����
 * \param[in]    app_id��Ӧ��ID
 * \param[in]    type��Ӧ�����ͣ��μ�app_type_e ����
 * \param[out]   none
 * \return       bool
 * \retval           �����Ƿ��ʼ���ɹ���true��ʾ�ɹ���false��ʾʧ��
 * \note
 *******************************************************************************/
bool applib_init(uint8 app_id, app_type_e type)
{
    app_info_t *cur_app_info;

    if (app_id >= APP_ID_MAX)
    {
        return FALSE;
    }

    //��ȡ app_info �ṹ��ָ��
    cur_app_info = &g_app_info_vector[type];

    //��������ֹ�������
    sys_os_sched_lock();

    //��ʼ��Ӧ����Ϣ
    libc_memset(cur_app_info, 0x0, sizeof(app_info_t));
    cur_app_info->used = 1;//��־�ѱ�����ʹ��
    cur_app_info->app_id = app_id;
    cur_app_info->app_type = (uint8) type;
    if (type == APP_TYPE_PMNG)
    {
        cur_app_info->mq_id = (uint8) MQ_ID_MNG;
    }

    //�����������������
    sys_os_sched_unlock();

    return TRUE;
}
