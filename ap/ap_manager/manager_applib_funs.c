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
 * \brief    applib 进程管理模块（bank部分）实现
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现一套基本 进程管理 接口。
 * \par      EXTERNALIZED FUNCTIONS:
 *               调用 libc 和 psp 的接口实现。
 * \version 1.0
 * \date  2011-9-28
 *******************************************************************************/

#include "applib.h"
extern bool open_config_file(void);
/******************************************************************************/
/*!
 * \par  Description:
 *    应用在 applib 模块上的初始化处理，在应用的 main 函数入口处调用
 * \param[in]    app_id：应用ID
 * \param[in]    type：应用类型，参见app_type_e 定义
 * \param[out]   none
 * \return       bool
 * \retval           返回是否初始化成功，true表示成功，false表示失败
 * \note
 *******************************************************************************/
bool applib_init(uint8 app_id, app_type_e type)
{
    app_info_t *cur_app_info;

    if (app_id >= APP_ID_MAX)
    {
        return FALSE;
    }

    //获取 app_info 结构体指针
    cur_app_info = &g_app_info_vector[type];

    //上锁，禁止任务调度
    sys_os_sched_lock();

    //初始化应用信息
    libc_memset(cur_app_info, 0x0, sizeof(app_info_t));
    cur_app_info->used = 1;//标志已被分配使用
    cur_app_info->app_id = app_id;
    cur_app_info->app_type = (uint8) type;
    if (type == APP_TYPE_PMNG)
    {
        cur_app_info->mq_id = (uint8) MQ_ID_MNG;
    }

    //解锁，允许任务调度
    sys_os_sched_unlock();

    return TRUE;
}
