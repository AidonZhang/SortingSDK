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
 * \���ò����ٶȻص�����
 * \param[in]    speed_value�����ٶ�ֵ  para1
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
 ��Ҷ�Ӳ˵�����ʱ���ݵ�ǰ���õ�speed����play speed
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool
 * \ingroup      music_setmenu_soundset.c
 * \note
 * \li  ����Ҷ�Ӳ˵�֧��ʵʱ�����ص�����˿��ܸı�ʵ�ʵ�EQ�����ظ��˵�ʱ���øú���
 �ָ������赱ǰplay speed
 */
/*******************************************************************************/
static void restore_music_speed(void)
{
    set_scene_speed_callback(g_music_config.playspeed);
}

/******************************************************************************/
/*!
 * \par  Description:
 ���ò����ٶ�ΪNORMAL��ʱ�ص�����
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
 ���ò����ٶ�Ϊslower��ʱ�ص�����
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
 ���ò����ٶ�Ϊfaster��ʱ�ص�����
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
    speed����ΪnormalģʽҶ�Ӳ˵�ִ�к���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT ������һ���˵�
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_spnormal_sure(void *param)
{
    g_music_config.playspeed = VALUE_SPEED_NORMAL;
    
    set_scene_speed_callback(VALUE_SPEED_NORMAL);

    //���ص���һ���˵�
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
    speed����ΪslowerģʽҶ�Ӳ˵�ִ�к���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT ������һ���˵�
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_spslower_sure(void *param)
{
    g_music_config.playspeed = VALUE_SPEED_SLOWER;
    
    set_scene_speed_callback(VALUE_SPEED_SLOWER);

    //���ص���һ���˵�
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
    speed����ΪfasterģʽҶ�Ӳ˵�ִ�к���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT ������һ���˵�
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_spfaster_sure(void *param)
{
    g_music_config.playspeed = VALUE_SPEED_FASTER;
    
    set_scene_speed_callback(VALUE_SPEED_FASTER);

    //���ص���һ���˵�
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_speed_sure(void *param)
 * \���ò����ٶ�
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the resul t
 * retval RESULT_IGNORE ����
 * retval
 * ingroup music_setmenu_soundset.c
 * note
 */
/**************************************************************************** ***/
app_result_e set_scene_speed_sure(void *param)
{
    uint16 value = g_music_config.playspeed;
    //ָ�򷵻ز�����ָ��
    menu_title_action_t* retptr = (menu_title_action_t*) param;

    //������һ���˵�strid��λ��ǰactive��
    retptr->style_id   =  MENULIST_TITLE;
    retptr->title_id   =  S_SPEED_UNIT;


    //ȷ���ַ���id
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

    //���÷��ص�ǰ���˵��Ļص�����
    g_eq_restore = 1;
    gui_menulist_set_back2parent(restore_music_speed, &g_eq_restore);
      
    //������һ���Ӳ˵�
    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_vollimit_sure(void *param)
 * \��������
 * \param[in]    void  para1
 * par am[out] none
 * return app_result_e the result
 * retval RESULT_IGNORE ����
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

        return (app_result_e) RESULT_REDRAW; //������һ��
    }
    else
    {
        return result;
    }
}

