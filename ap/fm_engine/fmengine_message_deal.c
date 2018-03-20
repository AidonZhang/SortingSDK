/*******************************************************************************
 *                              US211A
 *                            Module: radio engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>        <time>           <version >            <desc>
 *     mikeyang     2011-11-03 15:00     1.0             build this file
 *******************************************************************************/
#include "app_fmengine.h"

void fm_analog_deal(bool SpeakOut);


/******************************************************************************/
/*!
 * \par  Description:
 * \bool _fmengine_start_sound(uint8 cur_volume)
 * \��ʼ����ʱ����������ص�����
 * \param[in]    cur_volume
 * \param[out]   none
 * \return       bool
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_play_deal.c
 * \note
 */
/*******************************************************************************/
static bool _fmengine_start_sound(uint8 cur_volume)
{
    //param1 audio codec, param2 sample rate
    //mengine_info.eg_config.volume = cur_volume;
    //com_set_phy_volume(cur_volume);
    volume_deal(cur_volume);
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fm_module_init(private_msg_t* cur_msg)
 * \FM ģ���ʼ����Ϣ���������������ִ�г�ʼ������
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval       RESULT_APP_QUIT ʧ���˳�
 * \note   FM ģ���ʼ�����ã�������PA , FM �����
 */
/*******************************************************************************/
app_result_e fm_module_init(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint8 band_info, level;
    int result;
    app_result_e ret;

    band_info = (data_ptr->msg.content.data[0]); //��ʼ������Ƶ����Ϣ
    level = g_fmengine_cfg.fm_threshold; //��ʼ��������̨����
    
    result = fm_open(band_info, level, 0);

    //��ʼ���ɹ�
    if (result != -1) 
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        ret = RESULT_IGNORE;
        /* Ӳ������*/

        com_set_sound_out(TRUE, SOUND_OUT_START, _fmengine_start_sound);

        //ģ���ʼ���ɹ�������work ״̬
        g_module_status = MODULE_WORK;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
        ret = RESULT_NULL; //ģ���ʼ��ʧ�ܣ��˳�����AP
        g_module_status = MODULE_NO_INIT;
    }

    //�ظ�ͬ����Ϣ(�����ź���)
    reply_sync_msg(cur_msg);

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fm_module_reinstall(private_msg_t* cur_msg)
 * \ Restore FM Driver global data��so no need to initial fm driver
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval       RESULT_APP_QUIT ʧ���˳�
 * \note
 *    ���øýӿڻָ�FM������ʹ�õ�ȫ������WriteBuffer���ݣ���˿ɱ������³�ʼ��FMģ��
 */
/*******************************************************************************/
app_result_e fm_module_reinstall(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint8 band_info, level;
    int result;
    app_result_e ret;
    uint16 freq;

    band_info = (data_ptr->msg.content.data[0]); //��ʼ������Ƶ����Ϣ
    freq = data_ptr->msg.content.data[2];
    freq <<= 8;
    freq |= (data_ptr->msg.content.data[1]);
    level = g_fmengine_cfg.fm_threshold; //��ʼ��������̨����

#ifndef PC
    result = fm_open(band_info, level, freq);
#else
    result = 1;
#endif

    //��ʼ���ɹ�
    if (result != -1)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        ret = RESULT_IGNORE;

        //ģ���ʼ���ɹ�������work ״̬
        g_module_status = MODULE_WORK;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
        ret = RESULT_NULL; //ģ���ʼ��ʧ�ܣ��˳�����AP
        g_module_status = MODULE_NO_INIT;
    }

    //�ظ�ͬ����Ϣ(�����ź���)
    reply_sync_msg(cur_msg);

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fm_module_standby(void)
 * \FM ģ���Standby  ��Ϣ���������������ִ��
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval       RESULT_APP_QUIT ʧ���˳�
 * \note
 */
/*******************************************************************************/
app_result_e fm_module_standby(void)
{
    int result;

    /* ��Щģ����ܴ���©�磬��δ��ʼ��ʱ����Ҫ�������standby */
    if (g_module_status != MODULE_STANDBY)
    {
#ifndef PC
        result = fm_close();
#else
        result = 1;
#endif
        //����Standby �ɹ�
        if (result != -1)
        {
#ifndef test
            if (g_module_status == MODULE_WORK)
            {
                if (g_fm_chanel == 1)
                {
 //                  libc_print("FM CLOSE",0,0);
                    stop();
                }
                else
                {
                    fm_analog_deal(FALSE);
                }
                
                com_set_sound_out(FALSE, SOUND_OUT_STOP, NULL);
            }
#endif
            //ģ�����Standby ״̬
            g_module_status = MODULE_STANDBY;
        }
    }
    return RESULT_IGNORE;
}

//SpeakOut: TRUE��AA���,FLASE�ر�AA���
void fm_analog_deal(bool SpeakOut)
{
    if (SpeakOut == TRUE)
    {
//      enable_ain(AI_SOURCE_FM, 7);

        set_adc_rate(48);
        
        set_dac_rate(48,0);
        
        enable_adc(ADFOS_ASRC,(uint32)g_ainout_param.adc_gain);
        
        enable_dac(DAFIS_CPU, 0);
        
        enable_ain(AI_SOURCE_FM,(uint32)g_ainout_param.ain_gain);
    
        
        sys_mdelay(100);
        enable_ain_out(AI_SOURCE_FM, 2);

//        libc_print("FM A play",0,0);
    }
    else
    { 
        disable_adc();
        disable_dac(0);    
        disable_ain_out(AI_SOURCE_FM);
        disable_ain(AI_SOURCE_FM);
  //      libc_print("FM A stop",0,0);
    }


}

//FMģ��ͨ����/�м��PLAY
app_result_e fm_Ain_open()//(private_msg_t* cur_msg)
{
    app_result_e ret;
    if (g_module_status == MODULE_WORK)
    {
        //  cur_msg->reply->type = MSG_REPLY_SUCCESS;
        ret = RESULT_IGNORE;

        if (g_fm_chanel == 1)
        {
  //          libc_print("FM PLAY",0,0);
            play(); //play
        }
        else
        {
            fm_analog_deal(TRUE); //analog channel
        }

        // com_set_sound_out(TRUE,SOUND_OUT_START,NULL);
    }
    else
    {
        //cur_msg->reply->type = MSG_REPLY_FAILED;
        ret = RESULT_NULL;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    //  reply_sync_msg(cur_msg);
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_set_freq_sync(private_msg_t* cur_msg)
 * \ͬ�����õ���Ƶ�㣬��ʼFM ����
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval       RESULT_APP_QUIT ʧ���˳�
 * \note:   Ƶ��ֵ�����ã�����FM ui ������Ӧ�ö�����ͳһ��
 * \ ������Ը���ģ���ת�������������
 **/
/*******************************************************************************/
app_result_e fmengine_set_freq_sync(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint32 freq;
    int result;
    //��ǰ��̨��Ƶ�ʡ���λΪ1k (���λ��Ϊ1)��ȡ���2 �ֽ�  ���Լ���0x10000, Ƶ�����ΪΪ1.
    freq = 0x10000 + *((uint16*) &(data_ptr->msg.content.data[0]));
    //libc_print("SFQ:",freq,2);
#ifndef PC
    result = fm_set_freq(freq);
#else
    result = 1;
#endif
    //fm_get_freq(&freq);
    //libc_print("GFQ:",freq,2);
    /* ����Ƶ��ɹ�*/
    if (result != -1)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    fm_Ain_open();
    //�ظ�ͬ����Ϣ(�����ź���)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_mute_sync(private_msg_t* cur_msg)
 * \���þ������߽������
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval       RESULT_APP_QUIT ʧ���˳�
 * \note:
 **/
/*******************************************************************************/
app_result_e fmengine_mute_sync(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint8 vol_bak;
    FM_MUTE_e mode;
    int result = 0;

    mode = (FM_MUTE_e) data_ptr->msg.content.data[0];
    vol_bak = data_ptr->msg.content.data[1];

#ifndef PC
    result = fm_mute(mode);
#else
    result = 1;
#endif

#ifndef test
    /* �������*/
    if (mode == releaseMUTE)
    {

        /* ��������ת��������� */
        volume_deal(vol_bak);
        // com_set_sound_out(TRUE, SOUND_OUT_PAUSE, NULL);
        //com_set_phy_volume(vol_bak);

    }
    /* ����*/
    else if (mode == SetMUTE)
    {
        volume_deal(0);// shut down by volume shenlei  2016-01-18
        // com_set_sound_out(TRUE, SOUND_OUT_RESUME, NULL);
        //com_set_phy_volume(0);
    }
    else
    {
        ; /* һ�㲻�������� */
    }
#endif

    if (result != -1)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        if (mode == releaseMUTE)
        {
            g_fmengine_status.FM_VolStatus = STATUS_PLAY;

            change_engine_state(ENGINE_STATE_PLAYING);
        }
        else if (mode == SetMUTE)
        {
            g_fmengine_status.FM_VolStatus = STATUS_PAUSE;

            change_engine_state(ENGINE_STATE_PAUSE);
        }
        else
        {
            //;
        }

    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_set_band_sync(private_msg_t* cur_msg)
 * \���õ�̨����
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval
 * \note:
 **/
/*******************************************************************************/
app_result_e fmengine_set_band_sync(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;
    radio_band_e band_info;

    //�����õĵ�̨���Σ���radio_band_e
    band_info = (radio_band_e) data_ptr->msg.content.data[0];
#ifndef PC
    result = fm_set_band(band_info);
#else
    result = 1;
#endif

    if (result != -1)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_set_throd(private_msg_t* cur_msg)
 * \������̨����
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval
 * \note:
 **/
/*******************************************************************************/
app_result_e fmengine_set_throd(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint8 level;
    int result;

    level = data_ptr->msg.content.data[0];
#ifndef PC
    result = fm_set_throd(level);
#else
    result = 1;
#endif
    if (result != -1)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        g_fmengine_cfg.fm_threshold = level;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_set_softseek(private_msg_t* cur_msg)
 * \���������̨
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval       RESULT_APP_QUIT ʧ���˳�
 * \note:  ����Ƶ�����ã����ж��Ƿ���̨����̨�ظ��ɹ�
 **/
/*******************************************************************************/
app_result_e fmengine_set_softseek(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint32 now_freq;//uint16 now_freq;
    int result;
    uint8 dir;

    //�����̨�ӿں�������
    now_freq = *(uint16*) &(data_ptr->msg.content.data[0]) + 0x10000; //�����̨��ǰ�ж�Ƶ��
    dir = data_ptr->msg.content.data[2]; //��̨����

#ifndef PC

    //for search too long
    change_engine_state(ENGINE_STATE_PLAYING); //2015-12-24

    result = fm_soft_seek(now_freq, dir);
#else
    result = 0;
#endif
    g_fmengine_status.FM_CurrentFreq = now_freq; //��������㵱ǰƵ��ֵ

    if (result != -1)
    {
        //��Ч��̨
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
    }
    else
    {
        //��Ч��̨
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_set_hardseek(private_msg_t* cur_msg)
 * \����Ӳ����̨
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval       Ӳ����̨�����ɹ����ظ��ɹ�
 * \note:   ��̨����˵��
 * bit0   ~~~  ���ϻ�������̨��0:   UP;    1: DOWN
 * bit1   ~~~  �Ƿ�߽���ơ�     0:   ����;    1:  ������
 **/
/*******************************************************************************/
app_result_e fmengine_set_hardseek(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint32 now_freq;//uint16 now_freq;
    uint8 dir;
    int result;

    //Ӳ����̨�ӿں�������
    now_freq = *(uint16*) &(data_ptr->msg.content.data[0]) + 0x10000; //Ӳ����̨��ʼƵ��
    dir = data_ptr->msg.content.data[2]; //��̨����

    //����Ӳ����̨
    // com_set_sound_out(TRUE, SOUND_OUT_PAUSE, NULL);
#ifndef PC
    result = fm_hard_seek(now_freq, dir);
#else
    result = 1;
#endif

    if (result != -1)
    {
        //�����ɹ�
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        g_fmengine_status.STC_flag = HARDSEEK_DOING;
    }
    else
    {
        //����ʧ��
        data_ptr->reply->type = MSG_REPLY_FAILED;
        g_fmengine_status.STC_flag = HARDSEEK_NOT_START;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_seek_break(private_msg_t* cur_msg)
 * \����Ӳ����̨�����У��ֶ�ֹͣ�Զ���̨����
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval       �ɹ�ֹͣseek  ���̣��ظ��ɹ�������ظ�ʧ��
 * \note:
 **/
/*******************************************************************************/
app_result_e fmengine_seek_break(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;

#ifndef PC
    result = fm_set_breakseek();
#else
    result = 1;
#endif

    //�ɹ�ֹͣӲ����̨
    if (result != -1)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
#ifndef PC
        fm_get_freq(&g_fmengine_status.FM_CurrentFreq); //���µ�ǰƵ��
#endif
        g_fmengine_status.STC_flag = HARDSEEK_COMPLETE;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //�ظ�ͬ����Ϣ(�����ź���)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;

}

