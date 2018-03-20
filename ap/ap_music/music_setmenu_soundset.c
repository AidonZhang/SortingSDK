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
 从叶子菜单返回时根据当前设置的EQ音量重设EQ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool
 * \ingroup      music_setmenu_soundset.c
 * \note
 * \li  由于叶子菜单支持实时参数回调，因此可能改变实际的EQ，返回父菜单时调用该函数
 恢复或重设当前EQ
 */
/*******************************************************************************/
static void restore_music_eq(void)
{
    _scene_play_seteq(g_setting_comval.g_comval.eq_type);
}


/******************************************************************************/
/*!
 * \par  Description:
 EQ设置根菜单确定执行函数，根据当前的EQ值进行激活项匹配
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eq_sure(void* param)
{
    //指向返回参数的指针
    menu_title_action_t* retptr = (menu_title_action_t*) param;

    //确认字符串id
    switch (g_setting_comval.g_comval.eq_type)
    {
        //eq off
        case VALUE_EQ_OFF:
        retptr->str_id = S_OFF;
        break;
        //eq rock
        case VALUE_EQ_ROCK:
        retptr->str_id = S_ROCK;
        break;
        //ea funk
        case VALUE_EQ_FUNK:
        retptr->str_id = S_FUNK;
        break;
        //eq hip hop
        case VALUE_EQ_HIP_HOP:
        retptr->str_id = S_HIPHOP;
        break;
        //eq jazz
        case VALUE_EQ_JAZZ:
        retptr->str_id = S_JAZZ;
        break;
        //eq classical
        case VALUE_EQ_CLASSICAL:
        retptr->str_id = S_CLASSIC;
        break;
        //eq teckno
        case VALUE_EQ_TECHNO:
        retptr->str_id = S_TECHNO;
        break;

        default:
        retptr->str_id = -1;
        break;
    }

    //设置返回当前根菜单的回调函数
    g_eq_restore = 1;
    gui_menulist_set_back2parent(restore_music_eq, &g_eq_restore);
    //进入下一级菜单
    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq设置为normal模式叶子菜单执行函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqnormal_sure(void *param)
{
    //设置eq模式
    eq_param_t eq_info;

    eq_info.eq_mode = EQ_NORMAL;
    eq_info.eq_type = VALUE_EQ_OFF;
    
    music_set_eq(&eq_info);
    //eq关闭
    g_setting_comval.g_comval.eq_type = VALUE_EQ_OFF;
    //返回到上一级菜单
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq设置为rock模式叶子菜单执行函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       热键返回值
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqrock_sure(void *param)
{
    //设置eq模式
    eq_param_t eq_info;
    
    eq_info.eq_mode = EQ_EQ_USR_MODE;
    eq_info.eq_type = VALUE_EQ_ROCK;
    
    music_set_eq(&eq_info);
    //eq rock
    g_setting_comval.g_comval.eq_type = VALUE_EQ_ROCK;


    //返回到上一级菜单
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq设置为funk模式叶子菜单执行函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       热键返回值
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqfunk_sure(void *param)
{
    //设置eq模式
    eq_param_t eq_info;
    
    eq_info.eq_mode = EQ_EQ_USR_MODE;
    eq_info.eq_type = VALUE_EQ_FUNK;

    music_set_eq(&eq_info);
    //eq funk
    g_setting_comval.g_comval.eq_type = VALUE_EQ_FUNK;


    //返回到上一级菜单
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq设置为hiphop模式叶子菜单执行函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       热键返回值
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqhiphop_sure(void *param)
{
    //设置eq模式
    eq_param_t eq_info;
    
    eq_info.eq_mode = EQ_EQ_USR_MODE;
    eq_info.eq_type = VALUE_EQ_HIP_HOP;

    music_set_eq(&eq_info);
    //eq soft
    g_setting_comval.g_comval.eq_type = VALUE_EQ_HIP_HOP;

    //返回到上一级菜单
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq设置为jazz模式叶子菜单执行函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       热键返回值
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqjazz_sure(void *param)
{
    //设置eq模式
    eq_param_t eq_info;
    
    eq_info.eq_mode = EQ_EQ_USR_MODE;
    eq_info.eq_type = VALUE_EQ_JAZZ;
    
    music_set_eq(&eq_info);
    //eq jazz
    g_setting_comval.g_comval.eq_type = VALUE_EQ_JAZZ;

    //返回到上一级菜单
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq设置为classical模式叶子菜单执行函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       热键返回值
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqclassical_sure(void *param)
{
    //设置eq模式
    eq_param_t eq_info;
    
    eq_info.eq_mode = EQ_EQ_USR_MODE;
    eq_info.eq_type = VALUE_EQ_CLASSICAL;
    
    music_set_eq(&eq_info);
    //eq classical
    g_setting_comval.g_comval.eq_type = VALUE_EQ_CLASSICAL;

    //返回到上一级菜单
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq设置为techno模式叶子菜单执行函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       热键返回值
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqtechno_sure(void *param)
{
    //设置eq模式
    eq_param_t eq_info;

    eq_info.eq_mode = EQ_EQ_USR_MODE;
    eq_info.eq_type = VALUE_EQ_TECHNO;
    
    music_set_eq(&eq_info);
    //eq dbb
    g_setting_comval.g_comval.eq_type = VALUE_EQ_TECHNO;

    //返回到上一级菜单
    return RESULT_MENU_PARENT;
}



/******************************************************************************/
/*!
 * \par  Description:
 设置实际的EQ到音量回调函数
 * \param[in]    eq_mode  EQ类型，参见eq_param_t
 * \param[in]    eq_type  EQ索引，参见eq_param_e
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
static app_result_e set_scene_set_real_eq(uint8 eq_mode, uint8 eq_type)
{
    //设置eq模式
    eq_param_t eq_info;

    eq_info.eq_mode = eq_mode;
    eq_info.eq_type = eq_type;
    
    music_set_eq(&eq_info);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 设置EQ为NORMAL及时回调函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_eqnormal_callback(void)
{
    set_scene_set_real_eq(EQ_NORMAL, EQ_PARAM_NONE);
}

/******************************************************************************/
/*!
 * \par  Description:
 设置EQ为rock及时回调函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_eqrock_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_ROCK);
}

/******************************************************************************/
/*!
 * \par  Description:
 设置EQ为funk及时回调函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_eqfunk_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_FUNK);
}

/******************************************************************************/
/*!
 * \par  Description:
 设置EQ为hiphop及时回调函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_eqhiphop_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_HIPHOP);
}

/******************************************************************************/
/*!
 * \par  Description:
 设置EQ为jazz及时回调函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_eqjazz_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_JAZZ);
}

/******************************************************************************/
/*!
 * \par  Description:
 设置EQ为classical及时回调函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_eqclassic_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_CALSSIC);
}

/******************************************************************************/
/*!
 * \par  Description:
 设置EQ为techno及时回调函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_eqtechno_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_TECHNO);
}

