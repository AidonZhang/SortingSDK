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
/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "ap_setting.h"

//һ���˵�-��������
//��������
//        |-��������
//        |-������

//������ȫ�ֱ���
param_com_data_t s_param_com;
parambox_private_t s_param_private;
parambox_one_t s_param_item[5];


/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص���������������������
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       �����ء�������һ������Ϣ
 * \note: ע������˵��
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
        return (app_result_e) RESULT_REDRAW; //������һ��
    }
    else
    {
        return result;
    }

}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص���������ȡ�ð���������״��
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ���˵�
 * \note: ע������˵��
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
    return (app_result_e) RESULT_MENU_SON; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص���������ʹ�ܰ�����
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e enable_key_tone_setting_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.key_tone = VALUE_KEY_TONE_ON;

    com_set_sound_keytone(VALUE_KEY_TONE_ON);//ʹ�ܰ�����

    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڹرհ�����
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e disable_key_tone_setting_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.key_tone = VALUE_KEY_TONE_OFF;

    com_set_sound_keytone(VALUE_KEY_TONE_OFF);//��ֹ������

    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

