/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2012-5-11 12:34     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _setting_comval.c
 * \brief    common 系统配置功能
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               杂项，包括获取默认 setting_comval 值，初始化 sys_comval 指针，等等
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2012-5-11
*******************************************************************************/

#include "common_ui.h"
#include "common_func.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    从config.bin中读取默认设置
 * \param[in]    setting_comval 公共配置信息结构体指针
 * \param[out]   none 
 * \return       none
 * \ingroup      misc_func
 * \note 
*******************************************************************************/
void com_get_default_setting(setting_comval_t *setting_comval)
{
    //背光亮度等级
    setting_comval->g_comval.lightness = (uint8)com_get_config_default(SETTING_AP_ID_LIGHTNESS);
    //主题 0：蓝色 1：绿色 2：灰色 3：粉红色 4：红色
    setting_comval->g_comval.theme = (uint8) com_get_config_default(SETTING_AP_ID_THEME);
    //屏幕保护 0: 无 1: 数字时钟 2：相册图画 3：关闭屏幕 4：演示模式
    setting_comval->g_comval.screen_saver_mode = (uint8) com_get_config_default(SETTING_AP_ID_SCREEN_SAVER_MODE);
    //背光时间 以0.5秒为单位，0表示常亮
    setting_comval->g_comval.light_timer = (uint8) com_get_config_default(SETTING_AP_ID_LIGHT_TIMER);
    //屏幕保护时间 以0.5秒为单位，0表示无屏幕保护
    setting_comval->g_comval.screen_saver_timer = (uint8) com_get_config_default(SETTING_AP_ID_SCREEN_SAVER_TIMER);
    //睡眠时间,已分钟为单位；0表示不定时关机
    setting_comval->g_comval.sleep_timer = (uint8) com_get_config_default(SETTING_AP_ID_SLEEP_TIMER);
    //省电关机时间 以0.5秒为单位，0表示无省电关机
    setting_comval->g_comval.poweroff_timer = (uint8) com_get_config_default(SETTING_AP_ID_POWEROFF_TIMER);
    //定时返回正在播放界面 以0.5秒为单位，0表示无须返回
    setting_comval->g_comval.playing_timer = (uint8) com_get_config_default(SETTING_AP_ID_PLAYING_TIMER);
    //时间格式 0：12小时制 1：24小时制
    setting_comval->g_comval.time_format = (uint8) com_get_config_default(SETTING_AP_ID_TIME_FORMAT);
    //日期格式 0：DD_MM_YYYY 1：MM_DD_YYYY 2：YYYY_MM_DD
    setting_comval->g_comval.date_format = (uint8) com_get_config_default(SETTING_AP_ID_DATE_FORMAT);
    //语言类型
    setting_comval->g_comval.language_id = (uint8) com_get_config_default(SETTING_AP_ID_LANGUAGE_ID);
    //联机模式，0：MSC 1：MTP
    setting_comval->g_comval.online_device = (uint8) com_get_config_default(SETTING_AP_ID_ONLINE_DEVICE);
    //autorun支持选择 0 ：autorun，1： 支持autorun
    setting_comval->g_comval.autorun_set = (uint8) com_get_config_default(SETTING_AP_ID_AUTORUN_SET);
    //支持卡选择  0:不支持  1:支持
//    setting_comval->g_comval.support_card = (uint8) com_get_config_default(SETTING_AP_ID_SUPPORT_CARD);
    setting_comval->g_comval.volume_limit = (uint8) com_get_config_default(SETTING_AP_ID_VOLUME_LIMIT);
    setting_comval->g_comval.volume_current = (uint8) com_get_config_default(SETTING_AP_ID_VOLUME_CURRENT);
    setting_comval->g_comval.volume_gain = (uint8) com_get_config_default(SETTING_SOUND_MAIN_VOLUME_GAIN);    
//    setting_comval->g_comval.slide_time = (uint8) com_get_config_default(SETTING_AP_ID_SLIDE_TIME);
//    setting_comval->g_comval.slide_repeat_mode = (uint8) com_get_config_default(SETTING_AP_ID_SLIDE_REPEAT_MODE);
//    setting_comval->g_comval.slide_shuffle = (uint8) com_get_config_default(SETTING_AP_ID_SLIDE_SHUFFLE);
    setting_comval->g_comval.dark_lightoff_timer = (uint8) com_get_config_default(SETTING_AP_ID_DARK_LIGHTOFF_TIMER);
    setting_comval->g_comval.screen_lightoff_timer=(uint8)com_get_config_default(SETTING_AP_ID_SCREEN_LIGHTOFF_TIMER);
    
    //获取按键音配置项
    setting_comval->g_comval.key_tone = (uint8) com_get_config_default(SETTING_AP_ID_KEYTONE_ENABLE);

    //get music setting default
    setting_comval->music_comval.music_from = (uint8) com_get_config_default(SETTING_AP_ID_MUSIC_FROM);
    setting_comval->music_comval.music_repeat_mode = (uint8) com_get_config_default(SETTING_AP_ID_MUSIC_REPEAT_MODE);
    setting_comval->music_comval.music_shuffle = (uint8) com_get_config_default(SETTING_AP_ID_MUSIC_SHUFFLE);
    setting_comval->music_comval.audible_seek_image = (uint8) com_get_config_default(SETTING_AP_ID_AUDIBLE_SEEK_IMAGE);
//    setting_comval->music_comval.fullsound = (uint8) com_get_config_default(SETTING_AP_ID_FULLSOUND);
//    setting_comval->music_comval.variable_playback = (int8) com_get_config_default(SETTING_AP_ID_VARIABLE_PLAYBACK); 
//    setting_comval->music_comval.eq_setting = (uint8) com_get_config_default(SETTING_AP_ID_EQ_SETTING);
//    libc_memset(setting_comval->music_comval.eq_parameter, 0, 12);

    //CASE 层自定义 Graphics EQ 类型
    setting_comval->g_comval.eq_enable = (uint8) com_get_config_default(DAE_GRAPHICS_EQ_ENABLE);
    setting_comval->g_comval.eq_type   = (uint8) com_get_config_default(DAE_GRAPHICS_EQ_TYPE);

    setting_comval->g_comval.fade_in_time = (int8) com_get_config_default(SETTING_AP_ID_FADE_IN_TIME); 
    setting_comval->g_comval.fade_out_time = (int8) com_get_config_default(SETTING_AP_ID_FADE_OUT_TIME);    

    //DAE配置项
    //com_load_dae_config(&setting_comval->g_comval);    
}

void com_get_default_dae( dae_config_t * p_dae_cfg)
{
//    dae_config_t *p_dae_cfg;
    uint8 i;
    mdrc_config_t tmp_limiter_config;
       
//    p_dae_cfg = &(setting_comval->dae_cfg);

    p_dae_cfg->bypass = (uint8) com_get_config_default(DAE_BYPASS_ENABLE);
    p_dae_cfg->peq_enable = (uint8) com_get_config_default(DAE_PEQ_ENABLE);
    
    for (i = 0; i < MAX_PEQ_SEG; i++)
    {
        peq_config_t tmp_peq_config;
        com_get_config_struct((uint16) (DAE_PEQ_BANK0_SETTING + i), (uint8 *) &tmp_peq_config, sizeof(peq_config_t));
        libc_memcpy(&(p_dae_cfg->peq_bands[i]), &(tmp_peq_config.eq_data), sizeof(peq_band_t));
    }

    p_dae_cfg->mdrc_enable = (uint8) com_get_config_default(DAE_MDRC_ENABLE);
    p_dae_cfg->limiter_enable = (uint8) com_get_config_default(DAE_LIMITER_ENABLE);

    com_get_config_struct(DAE_LIMITER_SETTING, (uint8 *) &tmp_limiter_config, sizeof(tmp_limiter_config));
    
    p_dae_cfg->limiter_threshold = tmp_limiter_config.mdrc_data.threshold;
    p_dae_cfg->limiter_attack_time  = tmp_limiter_config.mdrc_data.attack_time;
    p_dae_cfg->limiter_release_time = tmp_limiter_config.mdrc_data.release_time;
}
    
/*! \endcond */
