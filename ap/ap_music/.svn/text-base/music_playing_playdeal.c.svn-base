/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-11-14          1.0              create this file
 *******************************************************************************/

#include "app_music.h"

#include "app_music_playing.h"

#include "app_music_show_lyric.h"


/******************************************************************************/
/*!
 * \par  Description:
 * \void _mktime(uint32 time_sec,time_timebox_t *time_buf)
 * \组织时间数据结构
 * \param[in]    time_sec 时间数值  para1
 * \param[in]    time_buf 存放时间结构的buffer
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_playdeal.c
 * \note
 */
/*******************************************************************************/
void _mktime(uint32 time_sec, time_timebox_t *time_buf)
{
    time_buf->hour = (uint16)(time_sec / 3600);
    time_buf->minute = (uint8)((time_sec % 3600) / 60);
    time_buf->second = (uint8)(time_sec % 60);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _show_time_progress(uint8 para, style_infor_t * style,uint8 type)
 * \绘制当前播放时间
 * \param[in]    para  para1
 * \param[in]    style para2
 * \param[in]    type  par3 0--now time 1--progress
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_playdeal.c
 * \note
 */
/*******************************************************************************/
void _show_time_progress(uint8 para, style_infor_t * style, uint8 type)
{
    uint32 cur_time, total_time;

    //music总时间信息
    mmm_mp_file_info_t music_fileinfo;

    if (g_total_time == 0)
    {
        music_get_fileinfo(&music_fileinfo);
        g_total_time = (uint32) music_fileinfo.total_time;
    }

    //music_get_playinfo(&music_playinfo);

    cur_time = (uint32)(g_cur_time / 1000);
    total_time = (uint32)(g_total_time / 1000);

    if (type == 0)
    {
        //显示当前时间和总时间
        //timer box数据结构
        timebox_private_t music_timebox;
        _mktime(cur_time, &(music_timebox.times[0].union_time.time));

        _mktime(total_time, &(music_timebox.times[1].union_time.time));

        style->style_id = MUSIC_CURTIME_TIMERBOX;
        if (music_timebox.times[0].union_time.time.hour != 0)
        {
            music_timebox.times[0].mode = TIME_DISP_HHMMSS;
        }
        else
        {
            music_timebox.times[0].mode = TIME_DISP_MMSS;

        }
        if (music_timebox.times[1].union_time.time.hour != 0)
        {
            music_timebox.times[1].mode = TIME_DISP_HHMMSS;
        }
        else
        {
            music_timebox.times[1].mode = TIME_DISP_MMSS;
        }

        if (para == TRUE)
        {
            ui_show_timebox(style, &music_timebox, TIMEBOX_DRAW_ALL);
        }
        else
        {
            ui_show_timebox(style, &music_timebox, TIMEBOX_DRAW_TIME);
        }
    }
    else
    {
        //刷进度条
        //进度条数据结构
        progressbar_private_t music_progress;
        if (cur_time > total_time)
        {
            cur_time = total_time;
        }

        style->style_id = MUSIC_PLAY_PROGRESSBAR;

        music_progress.value = cur_time;

        music_progress.total = total_time;

        if (para == TRUE)
        {
            //draw_mode = PROGRESS_DRAW_ALL;
            ui_show_progressbar(style, &music_progress, PROGRESS_DRAW_ALL);
        }
        else
        {
            //draw_mode = PROGRESS_DRAW_PROGRESS;
            ui_show_progressbar(style, &music_progress, PROGRESS_DRAW_PROGRESS);
        }

    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * 显示一屏歌词
 * param[in] null
 * param[out] null
 * return
 * retval
 * ingroup music_playing_show_lyric.c
 * note
 *******************************************************************************/
void display_lrc_str(uint8 *cur_lrc_str)
{
    style_infor_t lrc_textbox;
    textbox_private_t string_infor;
    region_t region;
    //绘制歌词显示框上面的部分区域与歌词显示框一致颜色
    region.x = LYRIC_X;
    region.y = LYRIC_Y;
    region.width = LYRIC_W;
    region.height = LYRIC_H;
    ui_set_backgd_color(0x528a);//(0x50,0x50,0x50)灰色
    ui_clear_screen(&region);

    lrc_textbox.style_id = SHOW_LYRIC_TEXTBOX;
    lrc_textbox.type = UI_AP;

    //显示参数
    string_infor.str_id = -1;
    string_infor.str_value = cur_lrc_str;
//    string_infor.lang_id = (int8) g_setting_comval.g_comval.language_id;
    string_infor.lang_id = (int8) g_lyric_charcode;
    //text box
    ui_show_textbox(&lrc_textbox, &string_infor, TEXTBOX_DRAW_NORMAL);
}

/****************************** ************************************************/
/*!
 * \par  Description:
 * 显示当前歌词，需要应用主动不停的调用该函数
 * param[in] null
 * param[out] null
 * return
 * retval
 * ingroup music_playing_show_lyric.c
 * note
 *******************************************************************************/
void show_lyric(void)
{
    uint8 i;

    if (lyric_check_query_time(g_cur_time) != 0)
    {
        if (lyric_seek_for(lrc_buf, LRC_BUF_LEN, g_cur_time) == TRUE)
        {
            lyric_deal_string(g_setting_comval.g_comval.language_id);
                    
            lyric_param_init();
                
            libc_memset(display_buf, 0, LRC_BUF_LEN);

            //歌词字符串长度
            parse_lyric(&g_lyric_decode);
            display_lrc_str(g_lyric_decode.output_buffer);
        }
    }
    else
    {
        if ((g_status_deal_flag & up_LrcMulScreen) != 0)
        {
            for (i = 1; i < LYRIC_MAX_SCREEN; i++)
            {
                if (g_cur_time >= g_lyric_decode.output_time[i])
                {
                    display_lrc_str(&(g_lyric_decode.output_buffer[g_lyric_decode.output_start[i]]));
                    g_lyric_decode.output_time[i] = 0xffffffff;
                    if (i == (LYRIC_MAX_SCREEN - 1))
                    {
                        g_status_deal_flag &= clr_LrcMulScreen;
                    }
                }
            }
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e _playing_loop_deal( file_path_info_t* path_locat)
 * \播放场景循环处理
 * \param[in]    para  para1
 * \param[in]    style para2
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_playdeal.c
 * \note
 */
/*******************************************************************************/
app_result_e playing_loop_deal(file_path_info_t* path_locat)
{
    //获取gui消息
    input_gui_msg_t gui_msg;
    style_infor_t style;
    uint8 prev_status;

    bool result;

    app_result_e ret_vals = RESULT_IGNORE;

    style.type = UI_AP;

    while (1)
    {
        if (get_screensave_state() == FALSE)//不在屏保模式才会显示
        {
            if ((g_status_deal_flag & up_MengineStatus) != 0)
            {
                g_status_deal_flag &= clr_MengineStatus;

                ret_vals = playing_check_status(path_locat, &prev_status);//从引擎获取当前状态信息
                if ((ret_vals != RESULT_IGNORE) && (ret_vals != RESULT_NULL))
                {
                    break;
                }

                if ((g_status_deal_flag & up_getfileInfoID3) != 0)
                {
                    //是否支持id3 且没在cue播放列表
                    if (g_music_config.id3_support == TRUE )
                    {
                        //获取id3信息
                        play_scene_getid3(path_locat);
                    }
                    g_status_deal_flag &= clr_getfileInfoID3;
                    g_status_deal_flag |= up_Paintflag;
                }
            #ifdef SUPPORT_ApeCue_SWITCH
                if ((g_status_deal_flag & up_ApeInCue) != 0)
                {
                    play_scene_deal_id3();
                    g_status_deal_flag |= up_Paintflag;
                }
            #endif
            }

            if (g_music_status.play_status == PlayFast)
            {
                //小于11s时间快进显示数字是不断变动的
                if (g_speed_counter < 11)
                {
                    if (g_prev_time == g_cur_time)
                    {
                        g_status_deal_flag |= up_MengineStatus;
                    }
                }
            }
            if ((g_status_deal_flag & up_Paintflag) != 0)
            {
                g_status_deal_flag &= clr_Paintflag;
                //绘制场景界面 按照g_display_flag中定义的标志位来刷新图标
                play_scene_paint(&g_display_bitmap);
            }

            if ((g_status_deal_flag & up_ScrollDeal) != 0)
            {
                g_status_deal_flag &= clr_ScrollDeal;
                id3_scroll_deal();
            }
            //歌词
            if (((g_display_bitmap & up_lyricflag) != 0) && (g_lyric_flag != 0))
            {
                show_lyric();
                //清标志
                g_display_bitmap &= clr_lyricflag;
            }
           
            //显示时间(为了少切bank 将时间和进度条显示放在此处)
            if ((g_display_bitmap & up_musictimeinfo) != 0)
            {
                //绘图
                _show_time_progress(FALSE, &style, 0);
                //清标志
                g_display_bitmap &= clr_musictimeinfo;
            }
            //显示进度条
            if ((g_display_bitmap & up_musicprogress) != 0)
            {
                //绘图
                _show_time_progress(FALSE, &style, 1);
                //清标志
                g_display_bitmap &= clr_musicprogress;
            }
        }
        //获取和处理GUI消息、私有消息
        result = get_gui_msg(&gui_msg);

        if ((result == TRUE))
        {
            ret_vals = _scene_play_guimsg(&gui_msg);
        }
        else
        {
            //一次循环处理一种类型的消息
            ret_vals = _scene_play_sysmsg();
        }
        if (ret_vals == RESULT_REDRAW)
        {
            //刷新界面标识
            SetFullInterface()
            g_status_deal_flag |= (up_Paintflag | up_MengineStatus);
        }
        if ((ret_vals != RESULT_IGNORE) && (ret_vals != RESULT_NULL) && (ret_vals != RESULT_REDRAW))
        {
            break;
        }
        //挂起10ms 多任务交互
        sys_os_time_dly(2);
    }//end of while

    return ret_vals;
}

