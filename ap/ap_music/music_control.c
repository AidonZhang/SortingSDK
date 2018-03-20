/*******************************************************************************
 *                                  US216A
 *                             Module: music ui
 *                   Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>            <time>            <version>              <desc>
 *      fiona.yang        2011-09-22            1.0              creat this file
 *******************************************************************************
 *
 * \file     music_control.c
 * \brief    music��פ����������Ƶ��ʹ�õĴ������ڸ��ļ�
 * \author   fiona.yang
 * \par      GENERAL DESCRIPTION:
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *
 * \version 1.0
 * \date    2011-09-22
 *******************************************************************************/
#include "app_music.h"
#include "app_music_playing.h"

#if 0
/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_send_sync_msg(msg_apps_t *msg, msg_reply_t *reply, uint32 timeout)
 * \music ui�����淢����Ϣ����
 * \param[in]    msg ��Ϣ����ָ��
 * \param[in]    reply  ��Ϣ��ִָ��
 * \param[out]   timeout ��ʱ����
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 * \li  ����applib��send_sync_msg�Ͷ�ʱ������ͬ��UI bank,Ϊ��Сbank�л������ú���
 �򻯺��ʵ�ַ���music��פ����,Ҳ���Բ�ʹ�øô���
 */
/*******************************************************************************/
bool music_send_sync_msg(msg_apps_t *msg, msg_reply_t *reply, uint32 timeout)
{
    private_msg_t cur_send_pmsg;
    msg_reply_t temp_reply;
    os_event_t *sem;//ͬ����Ϣͬ���ź���ָ��
    bool retvalue = TRUE;

    //��ȡ��̨�������ͣ����mengine�����ڣ�����ʧ��
    engine_type_e engine_type = get_engine_type();

    if(engine_type != ENGINE_MUSIC)
    {
        return FALSE;
    }

    //����ʱ�ȴ��ź�����ͬʱֻ����1���̷߳���ͬ����Ϣ
    libc_sem_wait(thread_mutex, 0);

    //���˽����Ϣ�ṹ��
    libc_memcpy(&(cur_send_pmsg.msg), msg, sizeof(msg_apps_t));
    libc_sem_init(&sem, 0);
    cur_send_pmsg.sem = sem;
    if(reply == NULL)
    {
        cur_send_pmsg.reply = &temp_reply;
    }
    else
    {
        cur_send_pmsg.reply = reply;
    }

    //������Ϣ��������
    if(sys_mq_send(MQ_ID_EGN, (void *)&cur_send_pmsg) == -1)
    {
        libc_sem_destroy(&sem);
        libc_sem_post(thread_mutex);
        return FALSE;
    }

    //�ȴ�ͬ����Ϣ��Ӧ
    if(libc_sem_wait(sem, timeout) < 0)
    {
        retvalue = FALSE;
    }

    libc_sem_destroy(&sem);
    libc_sem_post(thread_mutex);
    return retvalue;

}
#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_param(void *param, uint32 msg_type)
 * \��ȡ������ز�������
 * \param[in]    input_param �������ָ��
 * \param[in]    msg_type  ��Ϣ����
 * \param[out]   output_param �������ָ��
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_send_msg(void *input_param, void *output_param, uint32 msg_type)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //������Ϣ���ݴ�ű���
    msg_reply_t reply;

    //�����������ĵ�ַ
    msg.content.addr = input_param;

    //��Ż�ȡ�����ĵ�ַ
    reply.content = output_param;

    //��Ϣ����(����Ϣ����)
    msg.type = msg_type;

#if 0
    //����ͬ����Ϣ
    bret = music_send_sync_msg(&msg, &reply, 0);
#else
    bret = send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0);
#endif

    if (reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_playinfo(mengine_playinfo_t* play_info)
 * \��ȡ��ǰ���ŵ�ʱ���Ӧ��ר��ͼƬ
 * \param[in]    play_info  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_playing_getid3.c
 * \note
 */
/*******************************************************************************/
/*bool music_get_audible_image(mengine_playinfo_t* play_info)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //������Ϣ���ݴ�ű���
    msg_reply_t reply;
    //��ȡ��ǰaudible��ͼƬ��Ϣ
    reply.content = play_info;
    //��Ϣ����(����Ϣ����)
    msg.type = MSG_MENGINE_AUDIBLE_GETIMAGE_SYNC;
    //����ͬ����Ϣ
    bret = send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0);
    return bret;
}*/

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_filepath(file_path_info_t *filepath)
 * \��ȡ��ǰ�����ļ���·����Ϣ
 * \param[in]    filepath����ļ�·��buffer��ָ��  para1
 * \param[in]    file_path_info_t  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_filepath(file_path_info_t *filepath)
{
    return music_send_msg(NULL, filepath, MSG_MENGINE_GET_FILEPATH_SYNC);
}
/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_fileinfo(mmm_mp_file_info_t* fileinfo)
 * \��ȡ��ǰ���ŵ��ļ���ʽ����ʱ�䡢�����ʵ���Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_fileinfo(mmm_mp_file_info_t* fileinfo)
{
    return music_send_msg(NULL, fileinfo, MSG_MENGINE_GET_FILEINFO_SYNC);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_status(mengine_status_t* play_status)
 * \��ȡ��ǰ����״̬
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_status(mengine_status_t* play_status)
{
    return music_send_msg(NULL, play_status, MSG_MENGINE_GET_STATUS_SYNC);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_playinfo(mengine_playinfo_t* play_info)
 * \��ȡ��ǰ���ŵ�ʱ��ͱ�������Ϣ
 * \param[in]    play_info  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_playinfo(mengine_playinfo_t* play_info)
{
    return music_send_msg(NULL, play_info, MSG_MENGINE_GET_PLAYINFO_SYNC);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_bkinfo(mmm_mp_bp_info_t* curbk_info)
 * \��ȡ��ǰ���ŵĶϵ���Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_bkinfo(mmm_mp_bp_info_t* curbk_info)
{
    return music_send_msg(NULL, curbk_info, MSG_MENGINE_GET_BKINFO_SYNC);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_playmode(uint8* playmode)
 * \��ȡѭ��ģʽ&shuffleģʽ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_playmode(uint8* playmode)
{
    return music_send_msg(NULL, playmode, MSG_MENGINE_GET_PLAYMODE_SYNC);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_switch(void)
 * \�л��ļ�
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_switch(void)
{
    return music_send_msg(NULL, NULL, MSG_MENGINE_SWITCH_SYNC);

}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_play(void)
 * \��ʼ��������
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_play(void)
{
    return music_send_msg(NULL, NULL, MSG_MENGINE_PLAY_SYNC);

}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_filepath(file_path_info_t *filepath)
 * \���ò����ļ���·����Ϣ
 * \param[in]    filepath����ļ�·��buffer��ָ��  para1
 * \param[in]    file_path_info_t  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_set_filepath(file_path_info_t *filepath)
{
    if (filepath->file_path.dirlocation.disk == DISK_H)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            music_deal_sd_out();
            return FALSE;
        }
    }
    return music_send_msg(filepath, NULL, MSG_MENGINE_SET_FILEPATH_SYNC);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_play_next(void)
 * \������һ��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_play_next(void)
{
    bool bret = TRUE;
    uint32 msg_type;
#ifdef SUPPORT_ApeCue_SWITCH
    uint8 exit_flag = 0;
    music_comval_t *music_comval = &g_setting_comval.music_comval;

    if ((g_status_deal_flag & up_ApeInCue) != 0)
    {
        //���ģʽ��֧���������
        if (music_comval->music_shuffle == 0x10 && music_comval->music_repeat_mode != FSEL_MODE_INTRO)
        {
            g_cue_file.num = cue_shuffle_num(g_cue_file.total, g_cue_file.num);
            if (g_cue_file.num == 0xff)
            {
                //����������б��־
                exit_flag = 1;
            }
        }
        else
        {
            g_cue_file.num ++;
            //num��0����total��1����
            if ((g_cue_file.num + 1)> g_cue_file.total)
            {
                exit_flag = 1;
            }
        }

        if (exit_flag != 0)
        {
            g_status_deal_flag &= clr_ApeInCue;
            set_scene_cuemode(0);
        }
        else
        {
            handle_cue(g_cue_file.num);
        }

    }
#endif

    if ((g_status_deal_flag & up_ApeInCue) == 0)//��cue�б���
    {
        if ((g_audible_now == TRUE)//audible�б���
            && (g_music_config.section_mode == SWITCH_SECTION)
            && (g_music_status.err_status == EG_ERR_NONE))
        {
            //��Ϣ����(����Ϣ����)������һ��
            msg_type = MSG_MENGINE_CAHPTER_NEXT_SYNC;
            //ֻ��Ҫ�����ļ����
            g_display_bitmap |= up_musictracks;
        }
        else
        {
            g_status_deal_flag &= clr_ApeIsNow;
            msg_type = MSG_MENGINE_PLAY_NEXT_SYNC;
            //�����л��ļ�ˢ�±�־
            SetSwitchInterface()
        }

        //����ͬ����Ϣ
        bret = music_send_msg(NULL, NULL, msg_type);    
    }
    
    return bret;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_play_prev(void)
 * \������һ��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_play_prev(void)
{
    bool bret = TRUE;
    uint32 msg_type;

#ifdef SUPPORT_ApeCue_SWITCH

    uint8 exit_flag = 0;
    //��ǰ���ֲ���ģʽ
    music_comval_t *music_comval = &g_setting_comval.music_comval;

    //cue������һ�׵�������
    if ((g_status_deal_flag & up_ApeInCue) != 0)
    {
        //���ģʽ��֧���������
        if (music_comval->music_shuffle == 0x10
                && music_comval->music_repeat_mode != FSEL_MODE_INTRO)
        {
            g_cue_file.num = cue_shuffle_num(g_cue_file.total, g_cue_file.num);
            if(g_cue_file.num == 0xff)
            {
                //����������б���
                exit_flag = 2;
            }
        }
        else
        {
            g_cue_file.num --;
            //num<0Ϊ0xff���
            if ((g_cue_file.num> g_cue_file.total))
            {
                exit_flag = 2;
            }
        }

        if(exit_flag != 0)
        {
            g_status_deal_flag &= clr_ApeInCue;
            set_scene_cuemode(0);//ͬ����cue�б�����Ϣ����̨
        }
        else
        {
            handle_cue(g_cue_file.num);
        }
    }
#endif

    if ((g_status_deal_flag & up_ApeInCue) == 0)
    {
        if ((g_audible_now == TRUE)
        && (g_music_config.section_mode == SWITCH_SECTION)
        && (g_music_status.err_status == EG_ERR_NONE))
        {
            //��Ϣ����(����Ϣ����)������һ��
            msg_type = MSG_MENGINE_CHAPTER_PREV_SYNC;
            //ֻ��Ҫ�����ļ����
            g_display_bitmap |= up_musictracks;
        }
        else
        {
            g_status_deal_flag &= clr_ApeIsNow;
            msg_type = MSG_MENGINE_PLAY_PREV_SYNC;
            //�����л��ļ�ˢ�±�־λ
            SetSwitchInterface()
        }
        
        //����ͬ����Ϣ
        bret = music_send_msg(NULL, NULL, msg_type);
    }

    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void play_flag_update(void)
 * \1�붨ʱ���ò��ű�ʶ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
void play_flag_update(void)
{
    g_timer_counter++;
    g_status_deal_flag |= up_MengineStatus;
    if (g_timer_counter == 4)
    {
        g_timer_counter = 0;
        g_display_bitmap |= up_musictimeinfo;//����ʱ��
        g_display_bitmap |= up_musicprogress;//������

        if (((g_music_status.play_status != StopSta) && (g_music_status.play_status != PauseSta))
                && (g_lyric_flag != 0))
        {
            g_display_bitmap |= up_lyricflag;//���
        }
        //g_status_deal_flag |= up_Paintflag;//��Ҫ���»��ƽ���

        if (g_music_status.play_status == PlayFast)
        {
            g_speed_counter++;
        }
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 * \bool play_scroll_deal(void)
 * \��������
 * \param[in]    void  para1
 * \param[in]
 * \param[out]   none
 * \return       bool the result
 * \retval       0 ʧ����Ҫ��������/��������
 * \retval       none
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool play_scroll_deal(void)
{
    bool result;
    scroll_param_t scroll_param;
    style_infor_t style_infor;

    //��ȡ��������
    style_infor.type = UI_AP;
    /*
     ID3_TITLE_TEXTBOX  = 0x2001;
     ID3_ARTIST_TEXTBOX = 0x2002;
     ID3_ALBUM_TEXTBOX  = 0x2003;
     ID3_INFO_TEXTBOX   = 0x2004;
     */
    style_infor.style_id = (uint16) ID3_TITLE_TEXTBOX + g_scroll_vars.index;
    //�����������
    scroll_param.init = g_scroll_vars.init;//�Ƿ�Ϊ��ʼ��
    scroll_param.update = g_scroll_vars.update;//�Ƿ���Ҫ����
    scroll_param.active = g_scroll_vars.index;//������
    scroll_param.id3_text = &style_infor;

    result = (bool) (int32) ui_scroll_string_ext(&g_scroll_content[g_scroll_vars.index], &scroll_param);

    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void play_scroll_update(void)
 * \������ʱ��������
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
void play_scroll_update(void)
{
    g_status_deal_flag |= up_ScrollDeal;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void id3_scroll_deal(void)
 * \ID3����״̬������
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
void id3_scroll_deal(void)
{
    //��ǰTEXTBOX��Ҫ����
    if ((g_status_deal_flag & up_ScrollStatus) != 0)
    {
        if (g_scroll_vars.update == TRUE)
        {
            g_scroll_vars.update = FALSE;
            modify_app_timer(scroll_frush_id, 80);
        }

        if (play_scroll_deal() == FALSE)
        {
            //��ǰģʽ��������������2s��ʱ����ʱ�л�����
            modify_app_timer(scroll_frush_id, 2000);
            g_status_deal_flag &= clr_ScrollStatus;
        }
        else
        {
            g_scroll_vars.init = FALSE;
        }
    }
    else
    {//������һ����Ҫ������TEXTBOX
        skip_index: g_scroll_vars.index++;
        if (g_scroll_vars.index == 4)
        {
            g_scroll_vars.index = 0;
        }

        if (g_scroll_vars.need_scroll != 0)
        {
            if (((g_scroll_vars.need_scroll >> g_scroll_vars.index) & 0x01) == 0)
            {
                goto skip_index;
            }
        }
        //libc_print("index:",g_scroll_vars.index,2);
        g_scroll_vars.update = TRUE;
        g_scroll_vars.init = TRUE;
        g_status_deal_flag |= up_ScrollStatus;

    }
}
/******************************************************************************/
/*!
 * \par  Description:
 * \int _scene_play_sysmsg(void)
 * \����ϵͳ��Ϣ
 * \param[in]    gui_msg  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
app_result_e _scene_play_sysmsg(void)
{
    //get message
    private_msg_t music_msg;
    app_result_e msg_result = RESULT_IGNORE;
    if (get_app_msg(&music_msg) == TRUE)
    {
        msg_result = music_message_deal(&music_msg);
    }
    return msg_result;
}
