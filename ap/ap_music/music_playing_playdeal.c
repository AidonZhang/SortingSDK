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
 * \��֯ʱ�����ݽṹ
 * \param[in]    time_sec ʱ����ֵ  para1
 * \param[in]    time_buf ���ʱ��ṹ��buffer
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
 * \���Ƶ�ǰ����ʱ��
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

    //music��ʱ����Ϣ
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
        //��ʾ��ǰʱ�����ʱ��
        //timer box���ݽṹ
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
        //ˢ������
        //���������ݽṹ
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
 * ��ʾһ�����
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
    //���Ƹ����ʾ������Ĳ�������������ʾ��һ����ɫ
    region.x = LYRIC_X;
    region.y = LYRIC_Y;
    region.width = LYRIC_W;
    region.height = LYRIC_H;
    ui_set_backgd_color(0x528a);//(0x50,0x50,0x50)��ɫ
    ui_clear_screen(&region);

    lrc_textbox.style_id = SHOW_LYRIC_TEXTBOX;
    lrc_textbox.type = UI_AP;

    //��ʾ����
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
 * ��ʾ��ǰ��ʣ���ҪӦ��������ͣ�ĵ��øú���
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

            //����ַ�������
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
 * \���ų���ѭ������
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
    //��ȡgui��Ϣ
    input_gui_msg_t gui_msg;
    style_infor_t style;
    uint8 prev_status;

    bool result;

    app_result_e ret_vals = RESULT_IGNORE;

    style.type = UI_AP;

    while (1)
    {
        if (get_screensave_state() == FALSE)//��������ģʽ�Ż���ʾ
        {
            if ((g_status_deal_flag & up_MengineStatus) != 0)
            {
                g_status_deal_flag &= clr_MengineStatus;

                ret_vals = playing_check_status(path_locat, &prev_status);//�������ȡ��ǰ״̬��Ϣ
                if ((ret_vals != RESULT_IGNORE) && (ret_vals != RESULT_NULL))
                {
                    break;
                }

                if ((g_status_deal_flag & up_getfileInfoID3) != 0)
                {
                    //�Ƿ�֧��id3 ��û��cue�����б�
                    if (g_music_config.id3_support == TRUE )
                    {
                        //��ȡid3��Ϣ
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
                //С��11sʱ������ʾ�����ǲ��ϱ䶯��
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
                //���Ƴ������� ����g_display_flag�ж���ı�־λ��ˢ��ͼ��
                play_scene_paint(&g_display_bitmap);
            }

            if ((g_status_deal_flag & up_ScrollDeal) != 0)
            {
                g_status_deal_flag &= clr_ScrollDeal;
                id3_scroll_deal();
            }
            //���
            if (((g_display_bitmap & up_lyricflag) != 0) && (g_lyric_flag != 0))
            {
                show_lyric();
                //���־
                g_display_bitmap &= clr_lyricflag;
            }
           
            //��ʾʱ��(Ϊ������bank ��ʱ��ͽ�������ʾ���ڴ˴�)
            if ((g_display_bitmap & up_musictimeinfo) != 0)
            {
                //��ͼ
                _show_time_progress(FALSE, &style, 0);
                //���־
                g_display_bitmap &= clr_musictimeinfo;
            }
            //��ʾ������
            if ((g_display_bitmap & up_musicprogress) != 0)
            {
                //��ͼ
                _show_time_progress(FALSE, &style, 1);
                //���־
                g_display_bitmap &= clr_musicprogress;
            }
        }
        //��ȡ�ʹ���GUI��Ϣ��˽����Ϣ
        result = get_gui_msg(&gui_msg);

        if ((result == TRUE))
        {
            ret_vals = _scene_play_guimsg(&gui_msg);
        }
        else
        {
            //һ��ѭ������һ�����͵���Ϣ
            ret_vals = _scene_play_sysmsg();
        }
        if (ret_vals == RESULT_REDRAW)
        {
            //ˢ�½����ʶ
            SetFullInterface()
            g_status_deal_flag |= (up_Paintflag | up_MengineStatus);
        }
        if ((ret_vals != RESULT_IGNORE) && (ret_vals != RESULT_NULL) && (ret_vals != RESULT_REDRAW))
        {
            break;
        }
        //����10ms �����񽻻�
        sys_os_time_dly(2);
    }//end of while

    return ret_vals;
}

