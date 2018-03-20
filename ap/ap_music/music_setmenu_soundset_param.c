/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-21          1.0              create this file
 *******************************************************************************/

#include "app_music.h"


/******************************************************************************/
/*!
 * \par  Description:
 * \bool set_scene_speed_callback(uint16 speed_value)
 * \设置播放速度回调函数
 * \param[in]    speed_value播放速度值  para1
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 fail
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/************************** *****************************************************/
bool set_scene_speed_callback(uint16 speed_value)
{
    bool bret;
    bret = music_send_msg(&speed_value, NULL, MSG_MENGINE_SET_PLAYSPEED_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 从叶子菜单返回时根据当前设置的speed重设play speed
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool
 * \ingroup      music_setmenu_soundset.c
 * \note
 * \li  由于叶子菜单支持实时参数回调，因此可能改变实际的EQ，返回父菜单时调用该函数
 恢复或重设当前play speed
 */
/*******************************************************************************/
static void restore_music_speed(void)
{
    set_scene_speed_callback(g_music_config.playspeed);
}

/******************************************************************************/
/*!
 * \par  Description:
 设置播放速度为NORMAL及时回调函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_spnormal_callback(void)
{
    set_scene_speed_callback(VALUE_SPEED_NORMAL);
}

/******************************************************************************/
/*!
 * \par  Description:
 设置播放速度为slower及时回调函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_spslower_callback(void)
{
    set_scene_speed_callback(VALUE_SPEED_SLOWER);
}

/******************************************************************************/
/*!
 * \par  Description:
 设置播放速度为faster及时回调函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_spfaster_callback(void)
{
    set_scene_speed_callback(VALUE_SPEED_FASTER);
}

/******************************************************************************/
/*!
 * \par  Description:
    speed设置为normal模式叶子菜单执行函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_spnormal_sure(void *param)
{
    g_music_config.playspeed = VALUE_SPEED_NORMAL;
    
    set_scene_speed_callback(VALUE_SPEED_NORMAL);

    //返回到上一级菜单
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
    speed设置为slower模式叶子菜单执行函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_spslower_sure(void *param)
{
    g_music_config.playspeed = VALUE_SPEED_SLOWER;
    
    set_scene_speed_callback(VALUE_SPEED_SLOWER);

    //返回到上一级菜单
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
    speed设置为faster模式叶子菜单执行函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_spfaster_sure(void *param)
{
    g_music_config.playspeed = VALUE_SPEED_FASTER;
    
    set_scene_speed_callback(VALUE_SPEED_FASTER);

    //返回到上一级菜单
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_speed_sure(void *param)
 * \设置播放速度
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the resul t
 * retval RESULT_IGNORE 忽略
 * retval
 * ingroup music_setmenu_soundset.c
 * note
 */
/**************************************************************************** ***/
app_result_e set_scene_speed_sure(void *param)
{
    uint16 value = g_music_config.playspeed;
    //指向返回参数的指针
    menu_title_action_t* retptr = (menu_title_action_t*) param;

    //根据下一级菜单strid定位当前active项
    retptr->style_id   =  MENULIST_TITLE;
    retptr->title_id   =  S_SPEED_UNIT;


    //确认字符串id
    switch (value)
    {
        //speed normal
        case VALUE_SPEED_NORMAL:
        retptr->str_id = S_NORMAL;
        break;
        //speed slower
        case VALUE_SPEED_SLOWER:
        retptr->str_id = S_SLOWER;
        break;
        //speed faster
        case VALUE_SPEED_FASTER:
        retptr->str_id = S_FASTER;
        break;
        
        default:
        retptr->str_id = -1;
        break;
    }

    //设置返回当前根菜单的回调函数
    g_eq_restore = 1;
    gui_menulist_set_back2parent(restore_music_speed, &g_eq_restore);
      
    //进入下一级子菜单
    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_vollimit_sure(void *param)
 * \设置音量
 * \param[in]    void  para1
 * par am[out] none
 * return app_result_e the result
 * retval RESULT_IGNORE 忽略
 * retval
 * ingroup music_setmenu_soundset.c
 * note
 */
/********************************************************************** *********/
app_result_e set_scene_vollimit_sure(void *param)
{
    app_result_e result;
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;
    
    result = gui_volumebar(&g_setting_comval.g_comval.volume_current, &g_setting_comval.g_comval.volume_limit,
            SET_VOLUME_LIMIT);
    if (result == RESULT_CONFIRM)
    {
        string_desc.data.id = S_SET_VOLUME_END;
        string_desc.language = UNICODEID;

        dialog_com.dialog_type = DIALOG_INFOR_WAIT;
        dialog_com.button_type = BUTTON_NO_BUTTON;
        dialog_com.icon_id = V_U16_INVALID;
        dialog_com.string_desc = &string_desc;
        gui_dialog(DIALOG_MSG, &dialog_com);

        return (app_result_e) RESULT_REDRAW; //返回上一级
    }
    else
    {
        return result;
    }
}

