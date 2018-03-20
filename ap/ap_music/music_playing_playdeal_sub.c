/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     yanghong   2015-12-9         1.0              create this file
 *******************************************************************************/


#include "app_music.h"
#include "app_music_playing.h"
#include "app_music_show_lyric.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_clear_error(void)
 * \清除错误号
 * \param[in]    void para1
 * \param[in]
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_playing_paint.c
 * \note
 */
/*******************************************************************************/
bool music_clear_error(void)
{
    bool bret = TRUE;
    msg_apps_t msg;

    //消息类型(即消息名称)
    msg.type = MSG_MENGINE_CLEAR_ERR_SYNC;
    //发送同步消息
    bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e play_eror_deal(play_status_e status_prev)
 * \出错提示并切换歌曲
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       app_result_e
 * \retval       none
 * \ingroup      music_playing_paint.c
 * \note
 */
/*******************************************************************************/
app_result_e play_eror_deal(play_status_e status_prev)
{
    bool ret_val;
    file_location_t* locatptr;//指向location的指针
    uint16 str_id;
    app_result_e err_ret = RESULT_NULL;

    locatptr = (file_location_t*) &(g_file_path_info.file_path.flist_location);

    //清除错误号
    music_clear_error();

    g_error_num++;

    if (g_file_path_info.file_path.dirlocation.disk == DISK_H)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            //检测不到卡盘
            return music_deal_sd_out();
        }
    }

    if (g_error_num <= (locatptr->file_total))
    {
        switch (g_music_status.err_status)
        {
            case EG_ERR_DRM_INFO:
            str_id = S_LICENSE_FAIL;
            break;

            case EG_ERR_OPEN_FILE:
            str_id = S_FILE_OPEN_ERROR;
            break;

            case EG_ERR_NOT_SUPPORT:
            str_id = S_FILE_NO_SUPPORT;
            break;

            default:
            str_id = S_FORMAT_ERROR;
            break;
        }
        
        //提示出错
        err_ret = gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, str_id);        
        //ui_set_backgd_color(Color_BACKCOLOR_def);//清为全黑屏
        //ui_clear_screen(NULL);  //2015-10-30
        if (g_error_num < (locatptr->file_total))
        {
            if ((g_status_deal_flag & up_SwitchPrev) != 0)
            {
                //切换到上一曲
                ret_val = music_play_prev();
            }
            else
            {
                //切换到下一曲
                ret_val = music_play_next();
            }

            music_get_status(&g_music_status);//获取状态信息
            SetFullInterface()
        }
        else
        {
            //如果错误歌曲数等于歌曲总数
            //处理
            switch (g_enter_mode)
            {
                case PARAM_FROM_BROWSER:
                err_ret = RESULT_RETURN_BROWSER;
                break;
                
                case PARAM_FROM_PLAYLIST:
                case PARAM_FROM_MAINMENU:
                err_ret = RESULT_LISTMENU_LIST;
                break;
                
                default:
                err_ret = RESULT_LISTMENU_LIST;
                break;  
            }
        }
    }

    if (((locatptr->file_total) == 0) || ((locatptr->file_total) == g_error_num))
    {
        //是否后台存在music
        engine_type_e engine_type = get_engine_type();

        if ((locatptr->file_total) == 0)
        {
            //提示出错
            err_ret = gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_FILE_NO_FILE);
            err_ret = RESULT_MAIN_APP;
        }
        if (engine_type == ENGINE_MUSIC)
        {
            //引擎关闭
            music_close_engine();
        }
    }

    if (err_ret == RESULT_REDRAW)
    {
        err_ret = RESULT_NULL;
    }
    return err_ret;
}

#ifdef SUPPORT_ApeCue_SWITCH
/******************************************************************************/
/*!
 * \par  Description:
 * \static void check_apecue(file_location_t* locatptr, music_play_set_e play_set,uint8 load_flag)
 * \检查APE是否有cue文件
 * \param[in]    locatptr  文件路径
 * \param[in]    play_set  当前播放状态
 * \param[in]    load_flag 是否要加载cue
 * \param[out]   none
 * \return       void
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/
static bool check_apecue(file_location_t* locatptr, music_play_set_e play_set, uint8 load_flag)
{
    bool ret = FALSE;
    id3_type_e cur_type;//当前音乐的类型
    
    cur_type = play_scene_checktype(locatptr->filename);
    //后台自动切歌需要处理APE类型 
    if (cur_type == ID3_TYPE_APE)
    {
        if ((g_status_deal_flag & up_ApeIsNow) == 0)
        {
            if (load_flag == TRUE)
            {
                //APE音乐读取cue文件
                music_load_cuefile();
            }
            g_status_deal_flag |= up_ApeIsNow;
            ret = TRUE;
        }
    }
    else
    {
        g_status_deal_flag &= (clr_ApeIsNow & clr_ApeInCue);
    }

    return ret;
}

#endif
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e playing_check_status(file_path_info_t* path_locat)
 * \从引擎获取当前状态信息 判断是否出错 是否切换歌曲
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       app_result_e
 * \retval       none
 * \ingroup      music_playing_playdeal.c
 * \note
 */
/*******************************************************************************/
app_result_e playing_check_status(file_path_info_t* path_locat, uint8 *prev_status)
{
    bool ret;
    mengine_playinfo_t playinfo;
    app_result_e sta_ret = RESULT_NULL;

    if (g_error_num == 0)
    {
        //只在未出错之前获取一下当前引擎的播放状态
        *prev_status = g_music_status.play_status;
    }

    //获取状态信息
    music_get_status(&g_music_status);

    if (g_music_status.err_status != EG_ERR_NONE)
    {
        sta_ret = play_eror_deal(*prev_status);
    }
    else
    {
        g_error_num = 0;
    }

    if (*prev_status != g_music_status.play_status)
    {
        g_display_bitmap |= up_musicplaystatus;//播放状态
        g_status_deal_flag |= up_Paintflag;
    }

    //获取播放信息
    ret = music_get_playinfo(&playinfo);
    if (ret == FALSE)
    {
        return RESULT_LISTMENU_LIST;
    }
#ifdef SUPPORT_ApeCue_SWITCH
    if ((g_status_deal_flag & up_ApeInCue) != 0)
    {    
        if(playinfo.cur_time > g_cue_file.start_time)
        {
            g_cur_time = playinfo.cur_time - g_cue_file.start_time;
        }
        else
        {
            g_cur_time = 0;
        }
    }
    else
    {
        g_cur_time = playinfo.cur_time;
    }
#else
    g_cur_time = playinfo.cur_time;
#endif
    
    if (g_prev_time != g_cur_time)
    {
        g_prev_time = g_cur_time;
        g_display_bitmap |= up_musictimeinfo;//播放时间
        g_display_bitmap |= up_musicprogress;//进度条
    }
    
    //for audible 图片信息
    if (g_audible_now == TRUE)
    {
        //对于audible文件，是一定有专辑图片的
        //对于AAX文件可能有多张，通过get_playinfo只能表示当前的AAX文件
        if ((playinfo.cur_imag_flag != 0) && (g_music_config.album_art_support == TRUE))
        {
            //专辑图片在文件中的偏移
            album_art_info.offset = playinfo.cur_imag_pos;
            album_art_info.apic_type = 0;
            g_display_bitmap |= up_audibleimage;
            g_status_deal_flag |= up_Paintflag;
        }
        
        if (g_music_config.section_mode == SWITCH_SECTION)
        {
            if ((playinfo.cur_file_switch & 0x02) != 0)
            {
                //audible章节切换
                music_get_section_info(&g_file);
                
                g_display_bitmap |= up_musictracks;
                g_status_deal_flag |= up_Paintflag;//绘制
            }
        }
    }

#ifdef SUPPORT_ApeCue_SWITCH
    //apecue是否切歌
    if ((g_status_deal_flag & up_ApeInCue) != 0)
    {
        //cue播放模式处理
        sta_ret = set_playmode_apecue();
    }
    else if (((playinfo.cur_file_switch & 0x01) != 0)||((g_status_deal_flag & up_getfileInfoPATH) != 0))
#endif
    if (((playinfo.cur_file_switch & 0x01) != 0)||((g_status_deal_flag & up_getfileInfoPATH) != 0))
    {
        music_get_filepath(path_locat);

        if ((playinfo.cur_file_switch & 0x01) != 0)
        {
            //前台是正在播放界面非屏保
            if (playinfo.ui_type == 0)
            {
                //清除文件切换标志和有专辑图片标志
                g_status_deal_flag |= up_Switchflag;
                //切换文件后设置状态为播放状态
                //music_print("<SW_music>",0,0);
            }
        }
        #ifdef SUPPORT_ApeCue_SWITCH
            file_location_t* locatptr;//指向location的指针
            locatptr = (file_location_t*) &(path_locat->file_path.dirlocation);
            check_apecue(locatptr, PLAY_KEEP, TRUE);
        #endif
        if ((g_audible_now == TRUE) && (g_music_config.section_mode == SWITCH_SECTION))
        {
            music_get_section_info(&g_file);
        }
        else
        {
            g_file.total = path_locat->file_path.dirlocation.file_total;
            g_file.num = path_locat->file_path.dirlocation.file_num;
        }

        //切换歌曲
        g_status_deal_flag |= up_getfileInfoID3;
        g_status_deal_flag &= clr_getfileInfoPATH;
        g_total_time = 0;
        
        //切歌需要更新刷新标志位
        SetSwitchInterface()
    }

    return sta_ret;
}


