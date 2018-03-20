/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     Option Menu list
 * \brief    MainMenu的主菜单列表模块
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "main_menu.h"



const uint8 themes_ap2[5][13] =
{
    "mainmenu.sty", "mainmenu.sty",  "mainmenu.sty",  "mainmenu.sty",  "mainmenu.sty"
};


/******************************************************************************/
/*!
 * \par  Description:
 *	  Option菜单列表选择和模式切换
 * \param[in]    0, 无播放模式，1，正在播放模式，2，上一次播放模式
 * \param[out]   none
 * \return       the result
 * \retval       app_result_e, 菜单控件返回的结构消息
 * \ingroup      module name
 * \par          exmaple code
 * \code
 * \note
 *******************************************************************************/
app_result_e option_menulist(void)
{
    app_result_e result = 0;
    /*初始化菜单控件*/
    menu_com_data_t op_menu;
    op_menu.app_id = APP_ID_MAINMENU;
    /*定义不同模式下的入口菜单*/

    if (get_engine_state() == ENGINE_STATE_PLAYING)
    {
        op_menu.menu_entry = PLAYING_MENU;
    }
    else if (get_engine_state() == ENGINE_STATE_PAUSE)
    {
        op_menu.menu_entry = LASTPLAY_MENU;
    }
    else
    {
        op_menu.menu_entry = NOPLAY_MENU;
    }

    op_menu.path_id = 0;
    op_menu.menu_func = NULL;
    /*处理菜单私有消息*/
    result = gui_menulist(MENULIST_OPTION, &op_menu);
    return result;
}

/*!定义正在播放菜单执行函数*/
app_result_e menu_func_playing(void *param)
{
    msg_apps_t temp_msg;

    g_mainmenu_var.cfg_var.active_item = 0xff; //正在播放
    temp_msg.type = MSG_CREAT_APP;
    if (get_engine_type() == ENGINE_MUSIC)
    {
        temp_msg.content.data[0] = APP_ID_MUSIC;
    }
    else
    {
        temp_msg.content.data[0] = APP_ID_RADIO;
    }
    temp_msg.content.data[1] = PARAM_BACKTO_PLAYING;
    send_async_msg(APP_ID_MANAGER, &temp_msg);

    return RESULT_APP_QUIT;
}

/*!定义上一次播放菜单的执行函数*/
app_result_e menu_func_last_play(void *param)
{
    msg_apps_t temp_msg;

    g_mainmenu_var.cfg_var.active_item = 0xff; //正在播放
    temp_msg.type = MSG_CREAT_APP;
    if (get_engine_type() == ENGINE_MUSIC)
    {
        temp_msg.content.data[0] = APP_ID_MUSIC;
    }
    else
    {
        temp_msg.content.data[0] = APP_ID_RADIO;
    }
    temp_msg.content.data[1] = PARAM_BACKTO_PLAYED;
    send_async_msg(APP_ID_MANAGER, &temp_msg);

    return RESULT_APP_QUIT;
}



app_result_e menu_func_return(void)
{
    return RESULT_MENU_PARENT; //返回上一级
}


