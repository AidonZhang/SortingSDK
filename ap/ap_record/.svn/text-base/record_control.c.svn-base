/*
 *******************************************************************************
 *                              us212a
 *                             Module: record
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : record_control.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *              leiming    2011-10-27         v1.0                 create this file
 *******************************************************************************
 */
/*include file of this application, 本应用的头文件*/
#include "ap_record.h"


/*卡连续写提速的时候需打开以下头文件*/
#include  <card.h>
#include  <sysdef.h>

//record bitrate:8000为ACT比特率，
//32000, 64000, 128000, 192000为mp3比特率
//512000, 768000, 1024000, 1536000为wav pcm比特率
static const int rec_bit_rate[11] =
{ 8000, 32000, 64000, 128000, 192000, 256000, 512000, 768000, 1024000, 1536000, -1 };

static const int rec_sample_rate[11] =
{ 8000, 8000, 16000, 32000, 48000, 16000, 16000, 24000, 32000, 48000, -1 };

static const int rec_channel[11] =
{ SINGLE_CH, SINGLE_CH, SINGLE_CH, SINGLE_CH, SINGLE_CH, SINGLE_CH, DUAL_CH, DUAL_CH, DUAL_CH, DUAL_CH, -1 };

extern void _scene_record_proc_timer(void);

/******************************************************************************/
/*!
 * \par  Description:
 * \static app_result_e _recording_set_audio_param(void)
 * \设置wav格式录音的audio参数
 * \param[in]    void
 * \param[out]   none
 * \return       int result
 * \retval       none
 * \ingroup      record_control.c
 * \note
 */
/*******************************************************************************/
static app_result_e _recording_set_audio_param(void)
{
    int mr_ret;

    //设置audio param参数
    if (g_record_vars.file_type == REC_FORMAT_WAV)
    {
        g_mr_param.mr_audio_param.encode_mode = g_record_vars.encode_type; //设置ADPCM还是PCM

    }
    else if(g_record_vars.file_type == REC_FORMAT_MP3)
    {
        g_mr_param.mr_audio_param.encode_mode = MP3;

    }else if(g_record_vars.file_type == REC_FORMAT_PCM_WAV)
    {
    g_mr_param.mr_audio_param.encode_mode = PCM;
    }else if(g_record_vars.file_type == REC_FORMAT_ADPCM_WAV)
    {
    g_mr_param.mr_audio_param.encode_mode = ADPCM;
    }
   
    //vad mode 0:正常录音;1:声控录音;2:自动分曲
    g_mr_param.mr_audio_param.vad_mode = 0; //正常录音
    g_mr_param.mr_audio_param.vad_delay = 0; //avr delay set 0

    if ((g_record_vars.ai_source != AI_SOURCE_LINEIN) && (g_record_vars.ai_source != AI_SOURCE_FM))//mic
    {
        if (g_record_vars.avr_mode == AVR_ON)
        {
            g_mr_param.mr_audio_param.vad_mode = 1; //模式:声控录音
            g_mr_param.mr_audio_param.vad_delay = g_record_vars.track_delay * 1000; //set avr delay
            g_mr_param.mr_audio_param.vad_threshold = g_record_vars.track_level_avr; //set avr level
        }
    }
    else
    {
        //linein & fm
        if (g_record_vars.track_mode == TRACK_ON)
        {
            g_mr_param.mr_audio_param.vad_mode = 2; //录音模式:自动分曲
            g_mr_param.mr_audio_param.vad_delay = g_record_vars.track_delay * 1000; //avr delay set 0
            g_mr_param.mr_audio_param.vad_threshold = g_record_vars.track_level_breaksong;
        }
    }
    //sample rate must greater than 32000

    //set record bitrate
    g_mr_param.mr_audio_param.bitrate = rec_bit_rate[g_record_vars.bitrate];

    //set record sample rate
    g_mr_param.mr_audio_param.sample_rate = rec_sample_rate[g_record_vars.bitrate];
    //set record channel num
    g_mr_param.mr_audio_param.channel = rec_channel[g_record_vars.bitrate];

    g_mr_param.mr_ain_setting.listening_volume = g_record_vars.listening_volume;
    
    g_mr_param.mr_ain_setting.listening_flag = g_record_vars.listening_flag;
    
    //设置录音采样率/通道数/比特率
    mr_ret = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_AUDIO_PARAM, (unsigned int) &g_mr_param.mr_audio_param);
    if (mr_ret == -1)
    {
        return RESULT_SCENE_EXIT; //退出场景
    }

    return RESULT_NULL;
}


/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e _recording_start(void)
 * \开始录音
 * \param[in]    void
 * \param[out]   none
 * \return       int result
 * \retval       none
 * \ingroup      record_control.c
 * \note
 */
/*******************************************************************************/
app_result_e _recording_start(void)
{
    int mr_ret;
    app_result_e app_ret;
    mmm_mr_fs_para_t fs_para; //codec fs param
	storage_io_t file_io;


    if (DISK_H == g_record_vars.path.file_path.dirlocation.disk)
    {        
        card_update();                    
        g_card_para |= 0x08;    
    }

    //add for thread create
    adjust_freq_set_level(AP_BACK_HIGH_PRIO, FREQ_LEVEL8, FREQ_LEVEL6);
    
    //open recording file
    mr_ret = (int) mmm_mr_cmd(&g_mr_param.mr_handle, MMM_MR_OPEN, 0);
    if (mr_ret == -1)
    {
        adjust_freq_set_level(AP_BACK_HIGH_PRIO, FREQ_NULL, FREQ_NULL);    
        return RESULT_SCENE_EXIT; //退出场景
    }
    
    //set (mp3/wav) audio param
    app_ret = _recording_set_audio_param();

    if (app_ret == RESULT_SCENE_EXIT)
    {
        //关闭录音中间件
        mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_CLOSE, 0);

        adjust_freq_set_level(AP_BACK_HIGH_PRIO, FREQ_NULL, FREQ_NULL);
        //退出场景
        return RESULT_SCENE_EXIT;
    }
    

    //打开声音输入
    mr_ret = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_AIN_OPEN, (unsigned int) &g_mr_param.mr_ain_setting);
    if (mr_ret == -1)
    {
        //关闭录音中间件
        mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_CLOSE, 0);

        adjust_freq_set_level(AP_BACK_HIGH_PRIO, FREQ_NULL, FREQ_NULL);

        return RESULT_SCENE_EXIT; //退出场景
    }

    
    //设置录音文件 /*! 0: vfs, 1: sd file system */
    fs_para.fs_type = 0;

    fs_para.file_mount_id = (void*) g_fs_param.vfs_handle;

    fs_para.file_handle = (void*) g_fs_param.fhandle;

    //设置录音文件
    mr_ret = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_SET_FILE, (unsigned int) &fs_para);

	/*设置录音写回调函数*/
	file_io.write = mmm_mr_write;
	mmm_mr_cmd(g_mr_param.mr_handle,MMM_MR_SET_FILE_IO,(uint32)&file_io);
	//libc_print("IO OK",0,0);

    if (mr_ret == -1)
    {
        //关闭声音输入
        mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_AIN_CLOSE, 0);
        //关闭录音中间件
        mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_CLOSE, 0);

        adjust_freq_set_level(AP_BACK_HIGH_PRIO, FREQ_NULL, FREQ_NULL);

        return RESULT_SCENE_EXIT; //退出场景
    }

    //设定降噪等级
    mr_ret = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_SET_DENOISE, g_record_vars.noise_cancel_level);

    //start record
    mr_ret = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_START, 0);
    
    if (mr_ret == -1)
    {
        //clear file,close ain,close record
        _recording_close();
        //退出场景
        return RESULT_SCENE_EXIT;
    }

        
    g_scene_param.record_status = REC_PLAY_STATUS;

    //get cur time infor for display
    mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_GET_STATUS, (unsigned int) &g_mr_param.mr_status);
    //scene timer init
    g_scene_param.timer_id = set_app_timer(APP_TIMER_ATTRB_UI, REC_SCENE_TIMER_INTERVAL,
            (timer_proc) _scene_record_proc_timer);
    if (g_scene_param.timer_id == -1)
    {
        return RESULT_SCENE_EXIT;
    }
    
    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e _recording_pause(void)
 * \录音pause处理
 * \param[in]    void
 * \param[out]   none
 * \return       int
 * \retval       error
 * \ingroup      record_control.c
 * \note
 */
/*******************************************************************************/
app_result_e _recording_pause(void)
{
    int mr_ret;

    //stop timer
    stop_app_timer(g_scene_param.timer_id);

    //取当前录音时间
    mr_ret = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_GET_STATUS, (unsigned int) &g_mr_param.mr_status); //取出现在时间

    //发送pause命令
    mr_ret = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_PAUSE, 0);
    if (mr_ret == -1)
    {
        //return error
        return RESULT_SCENE_EXIT; //退出场景
    }

    g_scene_param.record_status = REC_PAUSE_STATUS;

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e _recording_resume(void)
 * \录音resume处理
 * \param[in]    void
 * \param[out]   none
 * \return       void
 * \retval       none
 * \ingroup      ap_record_control.c
 * \note
 */
/*******************************************************************************/
app_result_e _recording_resume(void)
{
    int mr_ret;

    //发送record resume
    mr_ret = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_RESUME, 0);
    if (mr_ret == -1)
    {
        return RESULT_SCENE_EXIT; //退出场景
    }

    //restart record timer
    if (!restart_app_timer(g_scene_param.timer_id))
    {
        return RESULT_ERROR;
    }

    g_scene_param.record_status = REC_PLAY_STATUS;
    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e _recording_close(void)
 * \关闭录音
 * \param[in]    void
 * \param[out]   none
 * \return       app_result_e
 * \retval       none
 * \ingroup      record_control.c
 * \note
 */
/*******************************************************************************/
app_result_e _recording_close(void)
{
    bool ret;
    //uint32 save_freq;

    //关闭录音文件
    mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_CLEAR_FILE, 0);

    vfs_file_close(g_fs_param.vfs_handle, g_fs_param.fhandle);

    //save record file num
    g_record_vars.rec_num = g_rec_num;

    //关闭声音输入
    mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_AIN_CLOSE, 0);

    //关闭录音中间件
    mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_CLOSE, 0);

    adjust_freq_set_level(AP_BACK_HIGH_PRIO, FREQ_NULL, FREQ_NULL);

    //判断是否wav录音，wav录音需重写文件头
    if (REC_FORMAT_WAV == g_record_vars.file_type
    ||REC_FORMAT_PCM_WAV == g_record_vars.file_type
    ||REC_FORMAT_ADPCM_WAV == g_record_vars.file_type)
    {
        ret = _record_write_back_filehead();
        if (ret == FALSE)
        {

            return RESULT_MAIN_APP;

        }
    }

    //将vm中的掉电回写标志清掉
    sys_vm_read(&g_writehead, VM_AP_RECORD_WAV, sizeof(record_writehead_t));
 	g_writehead.rewrite_need = FALSE; //2016-06-29
    sys_vm_write(&g_writehead, VM_AP_RECORD_WAV, sizeof(g_writehead));


    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e _recording_stop(void)
 * \关闭录音
 * \param[in]    void
 * \param[out]   none
 * \return       app_result_e
 * \retval       none
 * \ingroup      record_control.c
 * \note
 */
/*******************************************************************************/
app_result_e _recording_stop(void)
{
    int mr_ret;
    /*卡连续写提速参考*/
    if (g_record_vars.bitrate >= BIT_768KBPS)
    {
    #ifndef PC
        if (DISK_H == g_record_vars.path.file_path.dirlocation.disk)
        {
 
            card_update();
            g_card_para &= 0xf7;

        }
 /*    if(DISK_C == g_record_vars.path.file_path.dirlocation.disk)
        {
            //if(base_get_base_type() == 0x02)
            if((int)base_get_base_type() == 0x02)
            {
                //base_update();
                base_update(0,0,0);
                g_card_para &= 0xf7;
            }
        }*/
    #endif
    }
    /* kill timer */
    if (g_scene_param.timer_id != -1)
    {
        kill_app_timer(g_scene_param.timer_id);
        g_scene_param.timer_id = -1;
    }

    g_scene_param.record_status = REC_STOP_STATUS;

    //录音状态下，先停止录音
    mr_ret = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_STOP, 0);
    if (mr_ret < 0)
    {
        return RESULT_SCENE_EXIT;
    }

    //恢复按键音
/*    if (g_entry_mode != PARAM_FROM_RADIO)
    {
        com_set_sound_record(FALSE, 0);
    }
    else
    {
        com_set_sound_record(FALSE, 1);
    }*/

    //读取录音时长
    /*获取中间件状态*/
    mr_ret = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_GET_STATUS, (unsigned int) &g_mr_param.mr_status);

    if (mr_ret < 0)
    {
        return RESULT_ERROR;
    }

    return RESULT_NULL;
}

