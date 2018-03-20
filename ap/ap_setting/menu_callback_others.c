/*
 *******************************************************************************
 *                              USDK
 *                             Module: SETTING AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : menu_callback_others.C
 * History:
 *      <johnsen>    <2011-9-25 20:43>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, ��Ӧ�õ�ͷ�ļ� */
#include "ap_setting.h"
#include "menu_callback_others.h"
#include "sys_info.h"

const uint8 legal_file[] = "legal.txt";
const uint8 music_type_file[] = "m_type.txt";

//һ���˵���-��ʱ�ػ�
//��ʱ�ػ�(�ر�)|-��
//              |-�ػ��趨
//��ʱ�ػ�(����)|-��
//              |-�ػ��趨
////////////////////
//һ���˵���-����������ѡ��
//����������ѡ��|-MSC
//              |-MTP
//              |-ʲô��MSC
//              |-ʲô��MTP
////////////////////
//һ���˵���-��Ϣ
//��Ϣ|-��������Ϣ
//    |-���̿ռ���Ϣ
//    |-֧���ļ�|-����
//              |-ͼƬ
//              |-��Ƶ
////////////////////
//һ���˵���-������Ϣ
//������Ϣ-ֱ����ʾ������Ϣ


/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص���������ȡ�ö�ʱ�ػ��Ĳ˵���ڣ����ڿ����͹رն�ʱ�ػ�
 ���治һ�£�
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ���ز˵����
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e get_shut_down_entry(void * title_action)
{
    uint16 next_entry;
    string_desc_t string_desc;
    string_desc_t string_name;
    uint8 temp_string[5] = { 0 };

    menu_title_action_t* menu_title_shut_down;
    menu_title_shut_down = (menu_title_action_t*) title_action;
    if (g_setting_vars.g_comval.sleep_timer == VALUE_SHUT_DOWN_TIMER_OFF)
    {
        next_entry = MENU_ENTRY_ID_SHUT_DOWN_OFF;
        menu_title_shut_down->str_id = S_OFF;
        menu_title_shut_down->ret_str = NULL;
        menu_title_shut_down->source_id = -1;
        menu_title_shut_down->style_id = MENULIST;
    }
    else
    {

        string_desc.data.str = string_buffer;
        string_desc.length = TIPS_BUFFER_SIZE;
        string_desc.argv = S_REMAIN_TIME;
        string_desc.language = (int8) g_setting_vars.g_comval.language_id;

//        libc_itoa((uint32)(g_setting_vars.g_comval.sleep_timer - g_sys_counter.sleep_counter / 120),
//                &temp_string[1], 2); //��ʱ��ת���ַ�
				if ((g_sys_counter.sleep_counter / 60)>99 || (g_sys_counter.sleep_counter / 60)<0 )
				{
					libc_itoa(100,&temp_string[1], 2);
					libc_print("sleep_counter_error:",g_sys_counter.sleep_counter/60 ,2);
				}
				else
				{
					libc_itoa((uint32)(g_sys_counter.sleep_counter / 60),
					&temp_string[1], 2); //��ʱ��ת���ַ�
				}  
        temp_string[0] = '[';
        temp_string[3] = ']';
        temp_string[4] = 0;
        string_name.data.str = temp_string;
        string_name.length = 5;
        string_name.language = (int8) g_setting_vars.g_comval.language_id;

        com_string_format(&string_desc, &string_name);

        next_entry = MENU_ENTRY_ID_SHUT_DOWN_ON; //�������
        menu_title_shut_down->str_id = S_SHUT_DOWN_SETTING;
        menu_title_shut_down->ret_str = string_buffer;
        menu_title_shut_down->source_id = -1;
        menu_title_shut_down->style_id = MENULIST_TITLE;
    }
    menu_title_shut_down->menu_entry = next_entry; //�������
    return (app_result_e) RESULT_MENU_SON; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڹرն�ʱ�ػ�
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e shut_down_timer_off_callback(void *menu_title_void)
{
    menu_title_action_t* menu_title_shut_down_timer;
    g_setting_vars.g_comval.sleep_timer = VALUE_SHUT_DOWN_TIMER_OFF;

    menu_title_shut_down_timer = (menu_title_action_t*) menu_title_void;
    menu_title_shut_down_timer->str_id = S_OFF;
    g_sys_counter.sleep_counter = 0;
    return (app_result_e) RESULT_MENU_PARENT; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڵ����ػ�ʱ�����Сֵ
 *      �ص�˵�������ڵ��ڲ���ֵ����ϴ󳬹������ص㣬Ϊ�����γɱ�����ϵ����������
 *      99/33=3 ����һ���̶ȱ�ʾ3����ֵ����ֵ�仯3����һ���̶�
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
adjust_result_e adjust_min_time(parambox_one_t *params, uint8 active, bool inc)
{
    if (params->value == 0)
    {
        params->value = 1;
    }
    return ADJUST_RESULT_VALUE;
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڵ�����ʱ�ػ�ʱ��
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e shut_down_time_adjust_callback(void *menu_title_void)
{
    app_result_e return_result_temp;

    param_com_data_t s_param_com;
    parambox_private_t s_param_private;
    parambox_one_t s_param_item;
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;

    menu_title_action_t* menu_title_shut_down_timer;
    uint8 sleep_timer = g_setting_vars.g_comval.sleep_timer;

    if (sleep_timer == 0)
    {
        sleep_timer = 1;
    }
    s_param_item.min = VALUE_SHUT_DOWN_TIME_MIN - 1;
    s_param_item.max = VALUE_SHUT_DOWN_TIME_MAX;
    s_param_item.step = VALUE_SHUT_DOWN_TIME_STEP;
    s_param_item.value = (int16) sleep_timer; //value�Է���Ϊ��λ
    s_param_item.cycle = FALSE;
    s_param_item.adjust_func = adjust_min_time;
    s_param_item.value_str = NULL;
    s_param_item.callback = NULL;
    s_param_item.unit_id = S_UINT_MIN;

    s_param_private.back_id = -1;
    s_param_private.title_id = -1;
    s_param_private.icon_id = -1;
    s_param_private.active = 0;
    s_param_private.param_cnt = 1;
    s_param_private.items = &s_param_item;
    s_param_private.sign_flag = 0;
    s_param_private.scale = 2;

    s_param_com.private_data = &s_param_private;

    //return_result_temp = gui_set_parameter(SLIDER_NUM, &s_param_com);
    return_result_temp = gui_set_parameter(PROGRESSBAR_NUM, &s_param_com);

    if(return_result_temp== RESULT_CONFIRM)
    {
        g_setting_vars.g_comval.sleep_timer = (uint8) s_param_com.private_data->items->value; //value�Է���Ϊ��λ
        g_sys_counter.sleep_counter = s_param_com.private_data->items->value *60;
        string_desc.data.id = S_SET_SHUT_DOWN_TIMER_END;
        string_desc.language = UNICODEID;

        dialog_com.dialog_type = DIALOG_INFOR_WAIT;
        dialog_com.button_type = BUTTON_NO_BUTTON;
        dialog_com.icon_id = V_U16_INVALID;
        dialog_com.string_desc = &string_desc;
        gui_dialog(DIALOG_MSG, &dialog_com);
        menu_title_shut_down_timer = (menu_title_action_t*) menu_title_void;
        menu_title_shut_down_timer->str_id = S_SHUT_DOWN_SETTING;
        menu_title_shut_down_timer->source_id = -1;
        menu_title_shut_down_timer->style_id = MENULIST_TITLE;

        return (app_result_e) RESULT_MENU_CUR; //���½��뵱ǰ�������������⴦��
    }
    else
    {
        return return_result_temp;
    }

}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص���������ȡ��PC������ѡ���string ID
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ����PC������ѡ��string id
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e get_pc_connect_information_id_callback(void * title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title_pc_connect_information;
    menu_title_pc_connect_information = (menu_title_action_t*) title_action;
    if (g_setting_vars.g_comval.online_device == VALUE_STORAGE_MSC)
    {
        active_id = S_STORAGE_MSC;
    }
    else
    {
        active_id = S_STORAGE_MTP;
    }
    menu_title_pc_connect_information->menu_entry = -1; //�������
    menu_title_pc_connect_information->str_id = active_id;
    menu_title_pc_connect_information->ret_str = NULL;
    menu_title_pc_connect_information->source_id = -1;
    return (app_result_e) RESULT_MENU_SON; //������һ��
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص�������������PC������ѡ��ΪMSC
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_storage_msc_callback(void *menu_title_void)
{

    menu_title_action_t* menu_title_storage_msc;

    g_setting_vars.g_comval.online_device = VALUE_STORAGE_MSC;

    menu_title_storage_msc = (menu_title_action_t*) menu_title_void;
    menu_title_storage_msc->str_id = S_PC_CONNECT_OPTION;
    return (app_result_e) RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص�������������PC������ѡ��ΪMTP
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e set_storage_mtp_callback(void *menu_title_void)
{
    menu_title_action_t* menu_title_storage_mtp;

    g_setting_vars.g_comval.online_device = VALUE_STORAGE_MTP;

    menu_title_storage_mtp = (menu_title_action_t*) menu_title_void;
    menu_title_storage_mtp->str_id = S_PC_CONNECT_OPTION;
    return (app_result_e) RESULT_MENU_PARENT;

}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���ʲô��MSC
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e explain_msc_callback(void *menu_title_void)
{
    /*string_desc_t string_desc;
    dialog_com_data_t dialog_com;

    string_desc.data.id = S_NOTE_MSC;
    string_desc.language = UNICODEID;

    dialog_com.dialog_type = DIALOG_BUTTON_ANYKEY;  //Ҳ�ɿ���DIALOG_INFOR_WAIT;
    dialog_com.button_type = BUTTON_NO_BUTTON;
    dialog_com.icon_id = V_U16_INVALID;
    dialog_com.string_desc = &string_desc;
    gui_dialog(DIALOG_MSG, &dialog_com);*/

    return RESULT_REDRAW;
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص��������ڽ���ʲô��MTP
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e explain_mtp_callback(void *menu_title_void)
{
    /*string_desc_t string_desc;
    string_desc_t string_name;
    uint8 *temp_string = "TOOLS";
    dialog_com_data_t dialog_com;

    string_desc.data.str = string_buffer;
    string_desc.length = TIPS_BUFFER_SIZE;
    string_desc.argv = S_NOTE_MTP;
    string_desc.language = (int8) g_setting_vars.g_comval.language_id;

    string_name.data.str = temp_string;
    string_name.length = 5;
    string_name.language = (int8) g_setting_vars.g_comval.language_id;

    com_string_format(&string_desc, &string_name); //����tools�����ϵ�S_NOTE_MTP��


    dialog_com.dialog_type = DIALOG_BUTTON_ANYKEY;//Ҳ�ɿ���DIALOG_INFOR_WAIT;
    dialog_com.button_type = BUTTON_NO_BUTTON;
    dialog_com.icon_id = V_U16_INVALID;
    dialog_com.string_desc = &string_desc;
    gui_dialog(DIALOG_MSG, &dialog_com);
    */
    return RESULT_REDRAW;
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص���������ȡ����Ϣ�Ĳ˵���ڣ���������SN�룩
 *      �ص�˵��
 * \param[in]    ��none
 * \param[out]   : (menu_title_action_t*) title_action
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e get_information_entry(void *title_action)
{
    menu_title_action_t* menu_title_information;
    menu_title_information = (menu_title_action_t*) title_action;

    //libc_print("get_information_entry",0,0);
    
    sys_get_sys_info(SN_buffer,  SYS_GET_SN);
    if (!SN_buffer[0])  //��SN��
    {
        menu_title_information->menu_entry = MENU_ENTRY_ID_INFOR_WITHOUT_SN;
    }
    else //��SN��
    {
        menu_title_information->menu_entry = MENU_ENTRY_ID_INFOR_WITH_SN;
    }
    menu_title_information->str_id = -1;
    menu_title_information->ret_str = NULL;
    menu_title_information->source_id = -1;
    return RESULT_MENU_SON;
}


/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص�����������ʾ��������Ϣ
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e show_player_information_callback(void *menu_title_void)
{

    string_desc_t string_desc;
    string_desc_t string_name[3];
    uint8 software_version[FW_INFO_VERSION_LEN+2];
    uint8 *player_model = "US216A";
    uint8 *p_software_version;
    uint8 *support_website = "www.actions-semi.com";
    dialog_com_data_t dialog_com;

    software_version[0] = 'V';
//    software_version[1] = 'e';
//    software_version[2] = 'r';
    software_version[FW_INFO_VERSION_LEN+1] = 0;
    sys_get_fw_info ((uint8 *)&software_version[1], FW_INFO_VERSION_ADDR, FW_INFO_VERSION_LEN); //��ȡ�汾��
    p_software_version = software_version;

    string_desc.data.str = string_buffer;
    string_desc.length = TIPS_BUFFER_SIZE;
    string_desc.argv = S_NOTE_PLAYER;
    string_desc.language = (int8) g_setting_vars.g_comval.language_id;

    string_name[0].data.str = player_model;
    string_name[0].length = 6;
    string_name[0].language = (int8) g_setting_vars.g_comval.language_id;

    string_name[1].data.str = p_software_version;
    string_name[1].length = 11;
    string_name[1].language = (int8) g_setting_vars.g_comval.language_id;

    string_name[2].data.str = support_website;
    string_name[2].length = 20;
    string_name[2].language = (int8) g_setting_vars.g_comval.language_id;
    com_string_format(&string_desc, string_name); //��player ��Ϣ���ϵ�S_NOTE_PLAYER��

    dialog_com.dialog_type = DIALOG_INFOR_WAIT;
    dialog_com.button_type = BUTTON_NO_BUTTON;
    dialog_com.icon_id = V_U16_INVALID;
    dialog_com.string_desc = &string_desc;
    gui_dialog(DIALOG_MSG, &dialog_com);

    return RESULT_REDRAW;
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص�����������ʾ���̿ռ���Ϣ����Ҫ���ִ������޿�
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e show_disk_information_callback(void *menu_title_void)
{
    uint32 usabel_space_sec;
    uint32 total_space_sec;
    uint8 usable_space[6] =
    { 0 };
    uint8 total_space[6] =
    { 0 };
    uint8 total_musics[6] =
    { 0 };
    string_desc_t string_desc;
    string_desc_t string_name[3];
    dialog_com_data_t dialog_com;
    uint8 count = 0;
    uint16 g_ap_file_num[MAX_APP_NUM];

    if (g_vfs_mount_id == -1)
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);
        return RESULT_REDRAW;
    }

    gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_IMMEDIATE, S_WAITING);


    sys_vm_read(&g_ap_file_num, VM_FILE_NUM, sizeof(g_ap_file_num));

    vfs_get_space((void*) g_vfs_mount_id, (void*) &usabel_space_sec, GET_SPARE_SPACE);
    vfs_get_space((void*) g_vfs_mount_id, (void*) &total_space_sec, GET_TOTAL_SPACE);

    usabel_space_sec = usabel_space_sec / 2048; //sector change to Mbytes
    total_space_sec = total_space_sec / 2048; //sector change to Mbytes

    count = libc_itoa((uint32) usabel_space_sec, usable_space, 0); //�����ÿռ�ת���ַ���
   
    
    count = libc_itoa((uint32) total_space_sec, total_space, 0); //���ܿռ�ת���ַ���
    
    
    count = libc_itoa((uint32) g_ap_file_num[APP_ID_MUSIC], total_musics, 0); //����������ת���ַ���
   
    
    string_desc.data.str = string_buffer;
    string_desc.length = TIPS_BUFFER_SIZE;
    //���ÿռ䣺[%s] 
    //�ܿռ䣺[%s] 
    //������[%s]
    //ͼƬ��[%s]
    //��Ƶ��[%s]
    string_desc.argv = S_DISK_INFO_DETAIL;

    string_desc.language = (int8) g_setting_vars.g_comval.language_id;

    string_name[0].data.str = usable_space; //���ÿռ�
    string_name[0].length = 6;
    string_name[0].language = (int8) g_setting_vars.g_comval.language_id;

    string_name[1].data.str = total_space; //�ܿռ�
    string_name[1].length = 6;
    string_name[1].language = (int8) g_setting_vars.g_comval.language_id;

    string_name[2].data.str = total_musics; //�����������4000��
    string_name[2].length = 5;
    string_name[2].language = (int8) g_setting_vars.g_comval.language_id;

    com_string_format(&string_desc, string_name); //��������Ϣ���ϵ�S_DISK_INFO_DETAIL��

    dialog_com.dialog_type = DIALOG_INFOR_WAIT;
    dialog_com.button_type = BUTTON_NO_BUTTON;
    dialog_com.icon_id = V_U16_INVALID;
    dialog_com.string_desc = &string_desc;
    gui_dialog(DIALOG_MSG, &dialog_com);
    return RESULT_REDRAW;
}

/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص�����������ʾ������֧��������Ϣ
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e show_music_type_callback(void *menu_title_void)
{
    uint8 music_type[12];
    app_result_e result;

    libc_memcpy(music_type, music_type_file, 12);
   // result = gui_text_read(TEXTREAD_BG, music_type);
    result = gui_text_read(music_type);

    return result; //��������·��� RESULT_REDRAW���ػ浱ǰ���˵�
}



/*
 *****************************************************************************
 * \par  Description:
 *      ��ͨ˵��:���ص�����������ʾ������Ϣ
 *      �ص�˵��
 * \param[in]    ����
 * \param[out]   : ��
 * \retval       ��������һ��
 * \note: ע������˵��
 *
 ******************************************************************************
 */
app_result_e show_legal_information_callback(void *menu_title_void)
{
    uint8 legal_name[12];
    app_result_e result;

    libc_memcpy(legal_name, legal_file, 12);
    //result = gui_text_read(legal_name);
    result = gui_text_read(legal_name);

    return result; //��������·��� RESULT_REDRAW���ػ浱ǰ���˵�
}
 
app_result_e show_SN_code_callback(void *menu_title_void)
{

    string_desc_t string_desc;
    uint8 SN_buffer2[SN_SIZE];
   
    dialog_com_data_t dialog_com;

    libc_memcpy(SN_buffer2,&SN_buffer[1],67);
    string_desc.data.str = SN_buffer2;
    string_desc.length = SN_SIZE;
    string_desc.argv = S_NOTE_PLAYER;
    string_desc.language = (int8) g_setting_vars.g_comval.language_id;

  
    dialog_com.dialog_type = DIALOG_INFOR_WAIT;
    dialog_com.button_type = BUTTON_NO_BUTTON;
    dialog_com.icon_id = V_U16_INVALID;
    dialog_com.string_desc = &string_desc;
    gui_dialog(DIALOG_MSG, &dialog_com);

    return RESULT_REDRAW;
}


