/*******************************************************************************
 *                              US211A
 *                            Module: music engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>        <time>           <version >            <desc>
 *     fiona.yang     2011-11-03 15:00     1.0             build this file
 *******************************************************************************/
#include "app_mengine.h"
#include "mengine_rdata.h"
#include "Dac_interface.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 mengine_message_done(void)
 * \������Ϣ����,����������յ���˽����Ϣ��ϵͳ��Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����
 * \retval       Exit_App �˳�app
 * \ingroup      mengine_mesasge_done.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_message_done_bank(private_msg_t* cur_msg)
{
    g_mengine_result = RESULT_NULL;

    switch (cur_msg->msg.type)
    {
        /*���ò������*/
        case MSG_MENGINE_SET_EQ_SYNC://����eq +eq curve data
        mengine_set_eq(cur_msg);
        break;
        
        case MSG_MENGINE_SET_VOLUME_SYNC://��������
        mengine_set_volume(cur_msg);
        break;
        
        case MSG_MENGINE_GET_BKINFO_SYNC://��ȡ��ǰ���ŵĶϵ���Ϣ
        mengine_get_bkinfo(&g_cur_msg);
        break;

        case MSG_MENGINE_SET_PLAYMODE_SYNC://����ѭ��ģʽ+shuffle
        mengine_set_playmode(cur_msg);
        break;

        /*�����������*/
        case MSG_MENGINE_SWITCH_SYNC://�л��ļ���־
        mengine_switch(cur_msg);
        break;
        
        case MSG_MENGINE_PLAY_SYNC://��ʼ��������
		if(DAC_EXTERNAL == g_dac_type)        
		{
		    dac_active();
		}
        mengine_play(cur_msg);
        break;

        case MSG_MENGINE_STOP_SYNC://ֹͣ��������
        if(DAC_EXTERNAL == g_dac_type)        
		{
		    dac_standby();
		}
        mengine_stop(cur_msg);
        break;

        case MSG_MENGINE_PAUSE_SYNC://��ͣ��������
        if(DAC_EXTERNAL == g_dac_type)        
		{
		    dac_standby();
		}
        mengine_pause(cur_msg);
        break;

        case MSG_MENGINE_RESUME_SYNC://�ָ���������(����ͣ������)
		if(DAC_EXTERNAL == g_dac_type)        
		{
		    dac_active();
		}
        mengine_resume(cur_msg);
        break;

        /*��������*/
        case MSG_MENGINE_FFWD_SYNC:
        mengine_fast_forward(cur_msg);//���
        break;

        case MSG_MENGINE_FBWD_SYNC:
        mengine_fast_backward(cur_msg);//����
        break;

        case MSG_MENGINE_CANCEL_FFB_SYNC:
        mengine_cancel_ffb(cur_msg);//ȡ�������
        break;

        case MSG_MENGINE_SET_FFBSTEP_SYNC:
        mengine_fastplay_step(cur_msg);//���ÿ���˲���
        break;

        /*ѡ���ļ�����*/
        case MSG_MENGINE_SET_FILEPATH_SYNC://���ò����ļ���·��location
        mengine_set_filepath(cur_msg);
        break;

        case MSG_MENGINE_PLAY_NEXT_SYNC://������һ��
        mengine_play_next(cur_msg);
        break;

        case MSG_MENGINE_PLAY_PREV_SYNC://������һ��
        mengine_play_prev(cur_msg);
        break;

        case MSG_MENGINE_PLAY_BOOKMARK_SYNC://������ǩ
        if (g_is_audible == TRUE)
        {
            mengine_audible_bookmark(cur_msg);
        }
        //else
        //{
        //    mengine_play_bookmark(cur_msg);
        //}
        break;
        case MSG_MENGINE_PLAY_CUE_SYNC://����APE cue����
        mengine_play_cuemusic(cur_msg);
        break;

        /*��������*/
        case MSG_MENGINE_DELFILE_MUSUI_SYNC://music���ų���ɾ���ļ�
        mengine_musui_delete_file(cur_msg);
        break;

        case MSG_MENGINE_DELFILE_OTER_SYNC://�����ط�ɾ���ļ�
        mengine_other_delete_file(cur_msg);
        break;

        case MSG_MENGINE_CHANGE_FILEPATH_SYNC://�ı��ļ�����·��
        mengine_change_filepath(cur_msg);
        break;

        case MSG_MENGINE_CLEAR_ERR_SYNC://��������
        mengine_clear_error(cur_msg);
        break;

        case MSG_MENGINE_DEL_FAVORITE_SYNC://ɾ���ղؼ�
        mengine_del_favorite_deal(cur_msg);
        break;

        case MSG_MENGINE_ADD_FAVORITE_SYNC://����ղؼ�
        mengine_add_favorite_deal(cur_msg);
        break;
        
        case MSG_MENGINE_SET_SECTION_MODE_SYNC://����audible�½�ģʽ
        mengine_set_section_mode(cur_msg);
        break;

        case MSG_MENGINE_CAHPTER_NEXT_SYNC://�½��л�����һ��
        mengine_chapter_next(cur_msg);
        break;

        case MSG_MENGINE_CHAPTER_PREV_SYNC://�½��л�����һ��
        mengine_chapter_prev(cur_msg);
        break;

        //case MSG_MENGINE_AUDIBLE_GETIMAGE_SYNC://��ȡ��ǰ�ļ���ͼƬ��Ϣ
        //mengine_audible_getimage(cur_msg);
        //break;

        case MSG_MENGINE_GET_SECTION_INFO_SYNC:
        mengine_audible_get_section_info(cur_msg);
        break;
        
        case MSG_MENGINE_SET_PLAYSPEED_SYNC://audible���ٲ���
        mengine_audible_playspeed(cur_msg);
        break;

        case MSG_MENGINE_STANDBY_PLAY_SYNC:
        mengine_standby_play(cur_msg);
        break;
        
        case MSG_APP_QUIT://�˳�app
        g_mengine_result = RESULT_APP_QUIT;
        break;
        
        default:
        break;
    }

    return g_mengine_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_switch(void* msg_ptr)
 * \���벥�ų������ò����ļ�
 * \param[in]    void  para1
 * \param[out]   none
 * \return       none
 * \ingroup      mengine_mesasge_done.c
 * \note
 */
/*******************************************************************************/

app_result_e mengine_switch(void* msg_ptr)
{
    uint8 ret = TRUE;
    
    g_play_mode = PLAY_NORMAL;
    //���������״̬
    mengine_info.eg_status.err_status = EG_ERR_NONE;
    //�ļ��л���־��1
    mengine_info.eg_playinfo.cur_file_switch |= (uint8)0x01;
    //ǰ̨�����ڲ��Ž��������
    mengine_info.eg_playinfo.ui_type = 0;

    //�ں�̨�򿪵�����·������и����б�
    //ѡ���������ʱ��Ҫset_fileһ��
    ret = _set_file();
    if (ret == 0)
    {
        g_set_file_flag = FALSE;
    }
    
    if (g_is_audible == TRUE)
    {
        _check_pos_time();
        g_image_num = 0xffff;
    }
    else
    {
        //��ͨ������ǰʱ������
        mengine_info.eg_playinfo.cur_time = 0;
    }
    sys_vm_write(&mengine_info.eg_config, VM_AP_MENGINE, sizeof(mengine_config_t));//����̨дһ�ζϵ���Ϣ
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_play(void* msg_ptr)
 * \��ʼ����
 * \param[in]    msg_ptr��Ϣָ��  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//ͬ����־��1B
 *                 sem_id_t sem_id; 		  //�ź���ID��1B
 *                 msg_apps_t msg;		    //��Ϣ���ݣ�6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//��Ϣ���ͣ�2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//��Ϣ���ݻ����ַ������Ϣ�Լ�Լ����4B
 *               } msg_apps_t;//˽����Ϣ���е���Ϣ�ṹ��

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE ����
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_play(void* msg_ptr)
{
    bool ret_vals;
    if (g_is_audible_resume == TRUE)
    {
        g_play_mode = PLAY_RESUME;
        g_is_audible_resume = FALSE;
    }
    
    if (g_play_mode != PLAY_NO_PLAY)
    { 
        ret_vals = _play(g_play_mode);//����
    }
    else
    {
        mengine_info.eg_status.play_status = PauseSta;
        g_eg_status_p->err_status = EG_ERR_NONE;
        //����setfile��־��������Ҫ����set
        g_set_file_flag = FALSE;
    }
    g_play_mode = PLAY_NORMAL;
    //mengine_info.eg_playinfo.cur_file_switch |= 0x01;
    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_stop(void* msg_ptr)
 * \ֹͣ����
 * \param[in]    msg_ptr��Ϣָ��  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//ͬ����־��1B
 *                 sem_id_t sem_id; 		  //�ź���ID��1B
 *                 msg_apps_t msg;		    //��Ϣ���ݣ�6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//��Ϣ���ͣ�2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//��Ϣ���ݻ����ַ������Ϣ�Լ�Լ����4B
 *               } msg_apps_t;//˽����Ϣ���е���Ϣ�ṹ��

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE ����
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_stop(void* msg_ptr)
{
    bool ret_vals;

    ret_vals = _stop(STOP_NORMAL);//ֹͣ����

    change_engine_state(ENGINE_STATE_PAUSE);

    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_pause(void* msg_ptr)
 * \��ͣ����
 * \param[in]    msg_ptr��Ϣָ��  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//ͬ����־��1B
 *                 sem_id_t sem_id; 		  //�ź���ID��1B
 *                 msg_apps_t msg;		    //��Ϣ���ݣ�6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//��Ϣ���ͣ�2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//��Ϣ���ݻ����ַ������Ϣ�Լ�Լ����4B
 *               } msg_apps_t;//˽����Ϣ���е���Ϣ�ṹ��

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE ����
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_pause(void* msg_ptr)
{
    bool ret_vals;

    ret_vals = _stop(STOP_PAUSE);//��ͣ����

    change_engine_state(ENGINE_STATE_PAUSE);

    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_resume(void* msg_ptr)
 * \�ָ�����(����ͣ�ָ�)
 * \param[in]    msg_ptr��Ϣָ��  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//ͬ����־��1B
 *                 sem_id_t sem_id; 		  //�ź���ID��1B
 *                 msg_apps_t msg;		    //��Ϣ���ݣ�6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//��Ϣ���ͣ�2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//��Ϣ���ݻ����ַ������Ϣ�Լ�Լ����4B
 *               } msg_apps_t;//˽����Ϣ���е���Ϣ�ṹ��

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE ����
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_resume(void* msg_ptr)
{
    bool ret_vals;

    //���¿�ʼ����, �����ļ�
    //ret_vals = _set_file();
    //if (ret_vals == TRUE)
    //{
    ret_vals = _play(PLAY_RESUME);//����
    //}
    //todo ��ʱ�������޸�
    change_engine_state(ENGINE_STATE_PLAYING);

    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_play_switch(void *msg_ptr, uint8 mode)
 * \�л���������
 * \param[in]    msg_ptr��Ϣָ��  para1
 * \param[in]    mode �л�ģʽ  TRUE ��һ�� FALSE ��һ��
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE ����
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
static app_result_e mengine_play_switch(void *msg_ptr, switch_mode_e switch_mode)
{
    bool bret;
    play_mode_e play_mode;//�����Ƿ����ڲ���
    switch_result_e switch_ret;//�л��Ƿ�ɹ�
 
    if (g_eg_status_p->play_status != PauseSta)//������ͣ����״̬��Ҫ����
    {
        play_mode = PLAY_NORMAL;//���ڲ��ű�־
    }
    else
    {
        play_mode = PLAY_NO_PLAY;//δ�ڲ���
        //����ϵ�
        libc_memset(&(g_eg_cfg_p->bk_infor), 0, sizeof(mmm_mp_bp_info_t));
    }

    switch_ret = mengine_file_switch(STOP_NORMAL, switch_mode, play_mode);

    if (switch_ret == SWITCH_NO_ERR)
    {
        bret = TRUE;
    }
    else
    {
        bret = FALSE;
    }

    mengine_reply_msg(msg_ptr, bret);

    return RESULT_IGNORE;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_play_next(void* msg_ptr)
 * \�л�����һ��
 * \param[in]    msg_ptr��Ϣָ��  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//ͬ����־��1B
 *                 sem_id_t sem_id; 		  //�ź���ID��1B
 *                 msg_apps_t msg;		    //��Ϣ���ݣ�6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//��Ϣ���ͣ�2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//��Ϣ���ݻ����ַ������Ϣ�Լ�Լ����4B
 *               } msg_apps_t;//˽����Ϣ���е���Ϣ�ṹ��

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE ����
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_play_next(void* msg_ptr)
{
    return mengine_play_switch(msg_ptr, FORCE_SWITCH_NEXT);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_play_prev(void* msg_ptr)
 * \�л�����һ��
 * \param[in]    msg_ptr��Ϣָ��  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//ͬ����־��1B
 *                 sem_id_t sem_id; 		  //�ź���ID��1B
 *                 msg_apps_t msg;		    //��Ϣ���ݣ�6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//��Ϣ���ͣ�2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//��Ϣ���ݻ����ַ������Ϣ�Լ�Լ����4B
 *               } msg_apps_t;//˽����Ϣ���е���Ϣ�ṹ��

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE ����
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_play_prev(void* msg_ptr)
{
    play_status_e play_status = g_eg_status_p->play_status;

    if (g_is_audible == FALSE)
    {
        //����5s��ͷ��ʼ���裬����ǰ�и�
        if (g_eg_playinfo_p->cur_time > 5000)
        {
            g_eg_playinfo_p->cur_time = 0;
            
            //����ϵ�
            libc_memset(&(g_eg_cfg_p->bk_infor), 0, sizeof(mmm_mp_bp_info_t));
            
            _stop(STOP_NORMAL);//ֹͣ����
            
            if (play_status == PlaySta)
            {
                _set_file();
                _play(PLAY_NORMAL);//���� 
            }
            mengine_reply_msg(msg_ptr, TRUE);
            return RESULT_IGNORE;
        }
    }
    return mengine_play_switch(msg_ptr, FORCE_SWITCH_PREV);
}

