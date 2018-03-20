/*******************************************************************************
 *                              us212a
 *                             Module: record
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : record_paint.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      leiming       2011-10-27          v1.0
 *******************************************************************************/
/*include file of this application, ��Ӧ�õ�ͷ�ļ�*/
#include "ap_record.h"
#include <card.h>

//static const uint8 rec_sample_rate[10] =
//{ 8, 16, 32, 32, 16, 16, 24, 32, 48, 0xff };

extern void alarm_fm_record_proc(void);
/******************************************************************************/
/*!
 * \par  Description:
 * \record��ADC
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \ingroup      record_scene_recording_sub.c
 * \note
 */
/*******************************************************************************/
static void _record_open_adc(void)
{
    uint8 save_ai_source;

    save_ai_source = g_record_vars.ai_source;

    //set ain param
    g_mr_param.mr_ain_setting.input_source = save_ai_source; //set record source
    if ((g_record_vars.ai_source == AI_SOURCE_FM) || (g_record_vars.ai_source == AI_SOURCE_LINEIN))
    {
        g_mr_param.mr_ain_setting.input_gain = g_record_vars.gain_input_linein; //����¼��input����
        g_mr_param.mr_ain_setting.adc_gain = g_record_vars.gain_adc_linein; //����¼��adc����
    }
    else
    {
        g_mr_param.mr_ain_setting.input_gain = g_record_vars.gain_input_mic; //����¼��input����
        g_mr_param.mr_ain_setting.adc_gain = g_record_vars.gain_adc_mic; //����¼��adc����
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \record�رպ�̨����
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \ingroup      record_scene_recording_sub.c
 * \note
 */
/*******************************************************************************/
static void _record_close_engine(void)
{
    engine_type_e engine_type;
    engine_state_e engine_state;
    msg_apps_t msg;

    /*��ѯ�Ƿ��к�̨engine����*/
    engine_state = get_engine_state();
    if (engine_state != ENGINE_STATE_NULL)
    {
        engine_type = get_engine_type();
        if (ENGINE_MUSIC == engine_type)
        {
            //ɱ�����ֺ�̨����
            msg.type = MSG_KILL_APP_SYNC;
            msg.content.data[0] = APP_ID_MENGINE;
            //����ͬ����Ϣ
            send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
        }
        else if (ENGINE_RADIO == engine_type)
        {
            //����fm¼�����ر�fm
            if (g_entry_mode != PARAM_FROM_RADIO)
            {
                msg.type = MSG_KILL_APP_SYNC;
                msg.content.data[0] = APP_ID_FMENGINE;
                //����ͬ����Ϣ
                send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
            }
        }
        else
        {
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \static uint16 _scene_recording_init(void)
 * \record������ʼ������
 * \param[in]    void
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����
 * \retval       ......
 * \ingroup      record_scene_recording_sub.c
 * \note
 */
/*******************************************************************************/
bool _scene_recording_init(void)
{
    uint32 time_ms;

    uint8 pa_mode;
    uint8 aout_mode;
    bool keytone_state;
    pa_mode = g_app_info_state.pa_mode;
    aout_mode = g_app_info_state.aout_mode;
    
    _record_close_engine();

    if(get_card_update() == FALSE)
    {
        change_card_update(TRUE);
#ifndef PC
        g_card_para |= 0x80;
#endif         
    }

    //��Ƶ����   
    
    /*������ʼ��,��ʼ���ļ�ѡ��������ȡUI����ʾ�����ݵ�*/
    gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_IMMEDIATE, S_WAITING);
    
    /*װ�ش����������ļ�ϵͳ*/
    if (!_record_fs_init(g_record_vars.path.file_path.dirlocation.disk))
    {
        goto err_ret;
    }

    /*set record path , check disk info, check filenum, create filename*/
    if (!_record_file_setup())
    {
        goto err_ret;
    }

    /*variables init*/
    rec_free_space = g_fs_param.free_space; //����¼����ʼǰ��free space
    rec_free_time = g_scene_param.free_time; //����¼����ʼǰ��free time
    g_scene_param.record_result = RESULT_NULL;
    rewrite_times = 0;

//  sys_bank_flush(AP_BANK_FRONT_ENHANCED_1);
     sys_bank_flush(AP_BANK_FRONT_ENHANCED_2);

/*����¼��codec�����*/
#ifndef PC
    if (-1 == sys_load_mmm("mmm_mr.al", 0))
    {
        return FALSE;
    }
#endif

    keytone_state = get_keytone_enable();

    if(keytone_state == TRUE )
    {
        g_sound_state_bak = TRUE;
        com_set_sound_keytone(0);
    }  

    enable_pa(pa_mode, 0, aout_mode);

    time_ms = sys_get_ab_timer();
    
    _record_open_adc();

    _recording_start();

    if (g_record_from_alarm != 0)
    {
        g_alarm_timer = set_app_timer(APP_TIMER_ATTRB_UI, 1000, alarm_fm_record_proc);
    }

    //��ǰΪ��¼��
    if (DISK_H == g_record_vars.path.file_path.dirlocation.disk)
    {
        change_card_state(CARD_STATE_CARD_PLAY);
    }

    return TRUE;

    err_ret: 
    disable_adc();

    if (g_entry_mode != PARAM_FROM_RADIO)
    {
        disable_ain((uint32) g_record_vars.ai_source);
    }

    return FALSE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \static bool _scene_recording_exit(void)
 * \ ���ó����˳�����
 * \param[in]    void
 * \param[in]    none
 * \param[out]   bool
 * \return       the result
 * \retval       none
 * \retval       none
 * \ingroup      record_scene_recording_sub.c
 * \note
 */
/*******************************************************************************/
bool _scene_recording_exit(void)
{
    disable_pa(g_app_info_state.aout_mode)  ;

    if(g_sound_state_bak == TRUE )
    {
        g_sound_state_bak = FALSE;
        com_set_sound_keytone(1);
    }
        
    
    //�رն�ʱ��timer
    if (g_scene_param.timer_id != -1)
    {
        if (!kill_app_timer(g_scene_param.timer_id))
        {
            return FALSE;
        }
    }


    if (REC_STOP_STATUS != g_scene_param.record_status)
    {
        //stop recording
        _recording_stop();
        _recording_close();
        /*��ʾ�ļ��ѱ���*/
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_IMMEDIATE, SAVED2);
    }

    //�ر��м��
    sys_free_mmm(0);

    if (g_record_from_alarm != 0)
    {
        kill_app_timer(g_alarm_timer);
    }
    return TRUE;
}


void clear_RTCACCESS(void) //2016-06-29
{
    act_writel(0, RTC_ACCESS);
    act_writel(0xA596, RTC_REGUPDATA);
    while (act_readl(RTC_REGUPDATA) != 0x5A69)
    {
    	;//do nothing
    }
}
