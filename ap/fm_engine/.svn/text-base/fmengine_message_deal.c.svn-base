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
 * \开始播放时打开声音输出回调函数
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
 * \FM 模组初始化消息处理，负责调用驱动执行初始化操作
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval       RESULT_APP_QUIT 失败退出
 * \note   FM 模组初始化配置，并配置PA , FM 增益等
 */
/*******************************************************************************/
app_result_e fm_module_init(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint8 band_info, level;
    int result;
    app_result_e ret;

    band_info = (data_ptr->msg.content.data[0]); //初始化配置频段信息
    level = g_fmengine_cfg.fm_threshold; //初始化设置搜台门限
    
    result = fm_open(band_info, level, 0);

    //初始化成功
    if (result != -1) 
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        ret = RESULT_IGNORE;
        /* 硬件操作*/

        com_set_sound_out(TRUE, SOUND_OUT_START, _fmengine_start_sound);

        //模组初始化成功，处于work 状态
        g_module_status = MODULE_WORK;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
        ret = RESULT_NULL; //模组初始化失败，退出引擎AP
        g_module_status = MODULE_NO_INIT;
    }

    //回复同步消息(发送信号量)
    reply_sync_msg(cur_msg);

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fm_module_reinstall(private_msg_t* cur_msg)
 * \ Restore FM Driver global data，so no need to initial fm driver
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval       RESULT_APP_QUIT 失败退出
 * \note
 *    调用该接口恢复FM驱动所使用的全局数组WriteBuffer内容，因此可避免重新初始化FM模组
 */
/*******************************************************************************/
app_result_e fm_module_reinstall(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint8 band_info, level;
    int result;
    app_result_e ret;
    uint16 freq;

    band_info = (data_ptr->msg.content.data[0]); //初始化配置频段信息
    freq = data_ptr->msg.content.data[2];
    freq <<= 8;
    freq |= (data_ptr->msg.content.data[1]);
    level = g_fmengine_cfg.fm_threshold; //初始化设置搜台门限

#ifndef PC
    result = fm_open(band_info, level, freq);
#else
    result = 1;
#endif

    //初始化成功
    if (result != -1)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        ret = RESULT_IGNORE;

        //模组初始化成功，处于work 状态
        g_module_status = MODULE_WORK;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
        ret = RESULT_NULL; //模组初始化失败，退出引擎AP
        g_module_status = MODULE_NO_INIT;
    }

    //回复同步消息(发送信号量)
    reply_sync_msg(cur_msg);

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fm_module_standby(void)
 * \FM 模组进Standby  消息处理，负责调用驱动执行
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval       RESULT_APP_QUIT 失败退出
 * \note
 */
/*******************************************************************************/
app_result_e fm_module_standby(void)
{
    int result;

    /* 有些模组可能存在漏电，在未初始化时就需要让其进入standby */
    if (g_module_status != MODULE_STANDBY)
    {
#ifndef PC
        result = fm_close();
#else
        result = 1;
#endif
        //进入Standby 成功
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
            //模组进入Standby 状态
            g_module_status = MODULE_STANDBY;
        }
    }
    return RESULT_IGNORE;
}

//SpeakOut: TRUE打开AA输出,FLASE关闭AA输出
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

//FM模拟通道打开/中间件PLAY
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
    //回复同步消息(发送信号量)
    //  reply_sync_msg(cur_msg);
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_set_freq_sync(private_msg_t* cur_msg)
 * \同步设置单个频点，开始FM 播放
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval       RESULT_APP_QUIT 失败退出
 * \note:   频率值的设置，对于FM ui 及引擎应用而言是统一的
 * \ 具体针对各种模组的转换在驱动中完成
 **/
/*******************************************************************************/
app_result_e fmengine_set_freq_sync(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint32 freq;
    int result;
    //当前电台的频率。单位为1k (最高位恒为1)，取最低2 字节  所以加上0x10000, 频点最高为为1.
    freq = 0x10000 + *((uint16*) &(data_ptr->msg.content.data[0]));
    //libc_print("SFQ:",freq,2);
#ifndef PC
    result = fm_set_freq(freq);
#else
    result = 1;
#endif
    //fm_get_freq(&freq);
    //libc_print("GFQ:",freq,2);
    /* 设置频点成功*/
    if (result != -1)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    fm_Ain_open();
    //回复同步消息(发送信号量)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_mute_sync(private_msg_t* cur_msg)
 * \设置静音或者解除静音
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval       RESULT_APP_QUIT 失败退出
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
    /* 解除静音*/
    if (mode == releaseMUTE)
    {

        /* 按音量表转成软件音量 */
        volume_deal(vol_bak);
        // com_set_sound_out(TRUE, SOUND_OUT_PAUSE, NULL);
        //com_set_phy_volume(vol_bak);

    }
    /* 静音*/
    else if (mode == SetMUTE)
    {
        volume_deal(0);// shut down by volume shenlei  2016-01-18
        // com_set_sound_out(TRUE, SOUND_OUT_RESUME, NULL);
        //com_set_phy_volume(0);
    }
    else
    {
        ; /* 一般不会跑这里 */
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
    //回复同步消息(发送信号量)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_set_band_sync(private_msg_t* cur_msg)
 * \设置电台波段
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval
 * \note:
 **/
/*******************************************************************************/
app_result_e fmengine_set_band_sync(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;
    radio_band_e band_info;

    //需设置的电台波段，见radio_band_e
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
    //回复同步消息(发送信号量)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_set_throd(private_msg_t* cur_msg)
 * \设置搜台门限
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
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
    //回复同步消息(发送信号量)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_set_softseek(private_msg_t* cur_msg)
 * \进行软件搜台
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval       RESULT_APP_QUIT 失败退出
 * \note:  单个频点设置，并判断是否真台，真台回复成功
 **/
/*******************************************************************************/
app_result_e fmengine_set_softseek(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint32 now_freq;//uint16 now_freq;
    int result;
    uint8 dir;

    //软件搜台接口函数参数
    now_freq = *(uint16*) &(data_ptr->msg.content.data[0]) + 0x10000; //软件搜台当前判断频点
    dir = data_ptr->msg.content.data[2]; //搜台方向

#ifndef PC

    //for search too long
    change_engine_state(ENGINE_STATE_PLAYING); //2015-12-24

    result = fm_soft_seek(now_freq, dir);
#else
    result = 0;
#endif
    g_fmengine_status.FM_CurrentFreq = now_freq; //更新引擎层当前频点值

    if (result != -1)
    {
        //有效电台
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
    }
    else
    {
        //无效电台
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //回复同步消息(发送信号量)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_set_hardseek(private_msg_t* cur_msg)
 * \启动硬件搜台
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval       硬件搜台启动成功，回复成功
 * \note:   搜台方向说明
 * bit0   ~~~  向上或向下搜台。0:   UP;    1: DOWN
 * bit1   ~~~  是否边界回绕。     0:   回绕;    1:  不回绕
 **/
/*******************************************************************************/
app_result_e fmengine_set_hardseek(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint32 now_freq;//uint16 now_freq;
    uint8 dir;
    int result;

    //硬件搜台接口函数参数
    now_freq = *(uint16*) &(data_ptr->msg.content.data[0]) + 0x10000; //硬件搜台起始频点
    dir = data_ptr->msg.content.data[2]; //搜台方向

    //启动硬件搜台
    // com_set_sound_out(TRUE, SOUND_OUT_PAUSE, NULL);
#ifndef PC
    result = fm_hard_seek(now_freq, dir);
#else
    result = 1;
#endif

    if (result != -1)
    {
        //启动成功
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        g_fmengine_status.STC_flag = HARDSEEK_DOING;
    }
    else
    {
        //启动失败
        data_ptr->reply->type = MSG_REPLY_FAILED;
        g_fmengine_status.STC_flag = HARDSEEK_NOT_START;
    }
    //回复同步消息(发送信号量)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_seek_break(private_msg_t* cur_msg)
 * \用于硬件搜台过程中，手动停止自动搜台过程
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval       成功停止seek  过程，回复成功；否则回复失败
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

    //成功停止硬件搜台
    if (result != -1)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
#ifndef PC
        fm_get_freq(&g_fmengine_status.FM_CurrentFreq); //更新当前频率
#endif
        g_fmengine_status.STC_flag = HARDSEEK_COMPLETE;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //回复同步消息(发送信号量)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;

}

