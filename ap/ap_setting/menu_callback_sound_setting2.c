/*
 *******************************************************************************
 *                              USDK
 *                             Module: SETTING AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : menu_callback_sound_setting.C
 * History:
 *      <johnsen>    <2011-9-2 9:18>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, 本应用的头文件 */
#include "ap_setting.h"

//一级菜单-声音设置
//声音设置
//        |-音量限制
//        |-按键音

//参数框全局变量
param_com_data_t s_param_com;
parambox_private_t s_param_private;
parambox_one_t s_param_item[5];


/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于设置音量限制
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回“返回上一级”消息
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_volume_limit_callback(void *menu_title_void)
{
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;
    app_result_e result;

    result = gui_volumebar(&g_setting_vars.g_comval.volume_current, &g_setting_vars.g_comval.volume_limit,
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

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于取得按键音设置状况
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：进入下一级菜单
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e get_key_tone_setting_callback(void *title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title_key_tone_setting;
    menu_title_key_tone_setting = (menu_title_action_t*) title_action;
    if (g_setting_vars.g_comval.key_tone == VALUE_KEY_TONE_ON)
    {
        active_id = S_ON;
    }
    else
    {
        active_id = S_OFF;
    }
    menu_title_key_tone_setting->menu_entry = -1;
    menu_title_key_tone_setting->str_id = active_id;
    menu_title_key_tone_setting->ret_str = NULL;
    menu_title_key_tone_setting->source_id = -1;
    return (app_result_e) RESULT_MENU_SON; //进入下一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于使能按键音
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e enable_key_tone_setting_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.key_tone = VALUE_KEY_TONE_ON;

    com_set_sound_keytone(VALUE_KEY_TONE_ON);//使能按键音

    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于关闭按键音
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e disable_key_tone_setting_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.key_tone = VALUE_KEY_TONE_OFF;

    com_set_sound_keytone(VALUE_KEY_TONE_OFF);//禁止按键音

    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

