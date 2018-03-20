/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-15          1.0              create this file
 *******************************************************************************/
#include "app_music.h"
#include "app_music_albumlist.h"
#include "app_music_playing.h"

#include "mmm_id.h"

wregion_t wregion1 _BANK_DATA_ATTR_;
wregion_t wregion2 _BANK_DATA_ATTR_;

extern int32 music_paint_albumart(uint8 show_or_store, 
    uint32 file_offset, wregion_t *region1, wregion_t *region2);
extern bool show_albumart_pic(void);

/******************************************************************************/
/*!
 * \par  Description:
 * \void _show_audible_image(void)
 * \绘制专辑封面album art
 * \param[in]    void  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_paint.c
 * \note
 */
/*******************************************************************************/
static void _show_audible_image(void)
{
    wregion_t wregion1 , wregion2;
    uint32 file_offset = 0;
	play_status_e play_status;

	//窗口设置
    wregion1.x = ALBUMART_X;
    wregion1.y = ALBUMART_Y;
    wregion1.w = ALBUMART_W;
    wregion1.h = ALBUMART_H;
    file_offset = album_art_info.offset;
	
	//保存播放状态
	play_status = g_music_status.play_status;
	
    //只有播放状态才需要暂停
    if (play_status == PlaySta)
    {
		music_pause();//暂停
		com_set_sound_out(TRUE, SOUND_OUT_STOP, NULL);//停止音频输出，关闭PA
    }
	
    fsel_browser_set_file(&(g_file_path_info.file_path.plist_location), g_file_path_info.file_source);  
	//解码章节图片
    music_paint_albumart(FALSE, file_offset, &wregion1, &wregion2);

	//只有上一次状态为播放才需要恢复
	if (play_status == PlaySta)
    {
		com_set_sound_out(TRUE, SOUND_OUT_START, NULL);//开始音频输出，打开PA
		music_resume();//播放
	}
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void play_scene_albumart(void)
 * \绘制专辑封面album art
 * \param[in]    void  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_paint.c
 * \note
 */
/*******************************************************************************/
void play_paint_albumart(album_picinfo_t *album_picinfo)
{
    //uint8 full_mode;

    //uint8 *file_name = g_file_path_info.file_path.dirlocation.filename;

    uint32 file_offset = 0;

    //窗口大小设置
    wregion1.x = ALBUMART_X;
    wregion1.y = ALBUMART_Y;
    wregion1.w = ALBUMART_W;
    wregion1.h = ALBUMART_H;
    
    if (album_picinfo->apic_type == 0)
    {
        //定位到文件
        fsel_browser_set_file(&(g_file_path_info.file_path.plist_location), g_file_path_info.file_source);
        file_offset = album_picinfo->offset;
    }
    else
    {
        //定位到文件
        vfs_file_dir_offset(g_music_mount_id, NULL, &(album_picinfo->cluster_no), 1);
    }
    music_paint_albumart(TRUE, file_offset, &wregion1, &wregion2);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void play_scroll_init(void)
 * \滚屏初始化函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
void play_scroll_init(void)
{
    //初始化
    g_scroll_vars.init = TRUE;
    g_scroll_vars.update = TRUE;
    g_scroll_vars.need_scroll = 0;

    //初始化的时候不设置标志位
    g_status_deal_flag &= clr_ScrollDeal;

    //ID3_ARTIST_TEXTBOX
    g_scroll_vars.index = 1;
    if(TRUE == play_scroll_deal())
    {
        g_scroll_vars.need_scroll |= 0x02;
    }

    //ID3_ALBUM_TEXTBOX
    g_scroll_vars.index = 2;
    if(TRUE == play_scroll_deal())
    {
        g_scroll_vars.need_scroll |= 0x04;
    }
    
    //ID3_INFO_TEXTBOX
    g_scroll_vars.index = 3;
    if(TRUE == play_scroll_deal())
    {
        g_scroll_vars.need_scroll |= 0x08;
    }
    //ID3_TITLE_TEXTBOX
    g_scroll_vars.index = 0;
    if (play_scroll_deal() == TRUE)
    {
        g_scroll_vars.need_scroll |= 0x01;
        g_status_deal_flag |= up_ScrollStatus;
        g_scroll_vars.init = FALSE;
        g_scroll_vars.update = TRUE;
    }
    else
    {
        g_status_deal_flag &= clr_ScrollStatus;
    }

    //三个都不需要滚屏不需要打开滚屏定时器
    if (g_scroll_vars.need_scroll != 0x00)
    {
        if (scroll_frush_id == -1)
        {
            scroll_frush_id = set_app_timer(APP_TIMER_ATTRB_UI, 1000, play_scroll_update);
        }
        else
        {
            modify_app_timer(scroll_frush_id, 1000);
        }
    }
    else 
    {
        if (scroll_frush_id != -1)
        {//都不需要滚屏则关闭定时器
            kill_app_timer(scroll_frush_id);
            scroll_frush_id = -1;
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void play_scene_paint(uint32 *display_flag)
 * \绘制播放界面
 * \param[in]    para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_paint.c
 * \note
 */
/********************************************** *********************************/
void play_scene_paint(uint32 *display_flag)
{
    uint8 play_mode;
    //默认不刷新背景
    uint8 draw_bg = FALSE;

    style_infor_t style;
    picbox_private_t picbox_param;

    style.type = UI_AP;
    picbox_param.pic_id = -1;
    picbox_param.frame_id = 0;

    //如果是audible 则不画循环模式、shuffle、eq等
    if (g_audible_now == TRUE)
    {
        //清循环模式标志
        *display_flag &= clr_musicloopmode;
        //清除eq标识
        *display_flag &= clr_musiceqmode;
        //清除shuffle标识
        *display_flag &= clr_musicshuffle;
    }
    
    //背景
    if ((*display_flag & up_musicBackGDPic) != 0)
    {
        region_t region;
   
        region.x = 0;
        region.y = 0;
        region.width = 320;
        region.height = 28;
        ui_set_backgd_color(Color_BACKCOLOR_def);//黑色
        ui_clear_screen(&region);
        
        region.x = 0;
        region.y = 28;
        region.width = 320;
        region.height = 151;
        ui_set_backgd_color(0x39e7);//灰色
        ui_clear_screen(&region);
        
        region.x = 0;
        region.y = 179;
        region.width = 320;
        region.height = 61;
        ui_set_backgd_color(Color_BACKCOLOR_def);//黑色
        ui_clear_screen(&region);
        
        //清标志
        *display_flag &= clr_musicBackGDPic;
        draw_bg = TRUE;
    }
    //标题栏
    if ((*display_flag & up_musicheadbar) != 0)
    {
        //绘图
        if (g_music_config.browser_sel == TRUE)
        {
            gui_set_headbar_mode(HEADBAR_MODE_NORMAL, HEADBAR_ICON_BROWSER);
        }
        else
        {
            gui_set_headbar_mode(HEADBAR_MODE_NORMAL, HEADBAR_ICON_MUSIC);
        }
        //清标志
        *display_flag &= clr_musicheadbar;
    }
    //曲目信息
    if ((*display_flag & up_musictracks) != 0)
    {
        //number box
        numbox_private_t music_track_num;

        style.style_id = TRACK_NUMBER_BOX;

        music_track_num.value = g_file.num;

        music_track_num.total = g_file.total;

        if (draw_bg == TRUE)
        {
            //调用numbox显示 居中透明显示
            ui_show_numbox(&style, &music_track_num, NUMBOX_DRAW_ALL);
        }
        else
        {
            //调用numbox显示 居中透明显示
            ui_show_numbox(&style, &music_track_num, NUMBOX_DRAW_NUMBER);
        }
        //清标志
        *display_flag &= clr_musictracks;
    }
    //歌曲id3信息
    if ((*display_flag & up_musicinformation) != 0)
    {
        play_scroll_init();
        //清标志
        *display_flag &= clr_musicinformation;
    }
    //时间信息(为减少bank交叉切换放在外层循环)
    if ((*display_flag & up_musictimeinfo) != 0)
    {
        //绘图
        _show_time_progress(TRUE, &style, 0);
        //清标志
        *display_flag &= clr_musictimeinfo;
    }
    //eq模式
    if ((*display_flag & up_musiceqmode) != 0)
    {
        //绘图
        style.style_id = EQ_MODE_PICBOX;
        if (g_setting_comval.g_comval.eq_type!= VALUE_EQ_OFF)
        {
            //eq
            picbox_param.frame_id = 1;
        }
        else
        {
            picbox_param.frame_id = 0;
        }
        ui_show_picbox(&style, &picbox_param);
        //清标志
        *display_flag &= clr_musiceqmode;
    }
    //循环模式
    if ((*display_flag & up_musicloopmode) != 0)
    {
        //绘图
        music_get_playmode(&play_mode);
        play_mode &= 0x0f;

        style.style_id = LOOP_MODE_PICBOX;

        picbox_param.frame_id = play_mode;
        
        g_setting_comval.music_comval.music_repeat_mode = play_mode;
        
        ui_show_picbox(&style, &picbox_param);
        //清标志
        *display_flag &= clr_musicloopmode;
    }
    if ((*display_flag & up_musicshuffle) != 0)
    {
        music_get_playmode(&play_mode);

        style.style_id = SHUFFLE_MODE_PICBOX;

        picbox_param.frame_id = (play_mode >> 0x04);

        ui_show_picbox(&style, &picbox_param);
        
        g_setting_comval.music_comval.music_shuffle = (uint8)(play_mode&0xf0);
        
        *display_flag &= clr_musicshuffle;
    }

    //进度条(为减少bank交叉切换放在外层循环)
    if ((*display_flag & up_musicprogress) != 0)
    {
        //绘图
        _show_time_progress(TRUE, &style, 1);
        //清标志
        *display_flag &= clr_musicprogress;
    }
    //专辑图片
    if ((*display_flag & up_musicalbumart) != 0)
    {
        //显示光盘图标
        style.style_id = ALBUM_ART_PICBOX;
        ui_show_picbox(&style, NULL);
        //显示标题图标
        style.style_id = ID3_MUSIC_TITLE_PICBOX; 
        ui_show_picbox(&style, NULL);
        //显示作者图标
        style.style_id = ID3_MUSIC_ARTIST_PICBOX;
        ui_show_picbox(&style, NULL);
        //显示属性图标
        style.style_id = ID3_MUSIC_INFO_PICBOX;
        ui_show_picbox(&style, NULL);
        //显示专辑图标
        style.style_id = ID3_MUSIC_ALBUM_PICBOX;
        ui_show_picbox(&style, NULL);
        //显示播放图标
        style.style_id = PLAY_STATUS_PICBOX;
        picbox_param.frame_id = 1;
        ui_show_picbox(&style, &picbox_param);

        //歌曲文有切换才进行解码图片和play处理
        if ((g_status_deal_flag & up_Switchflag) != 0)
        {
            g_status_deal_flag &= clr_Switchflag;

            if ((g_status_deal_flag & up_AlbumART) != 0)
            {
                //解码并保存专辑图片
                play_paint_albumart(&album_art_info);
            }
            if ((g_status_deal_flag & up_NeedOpenPA) == 0)
            {
                music_play();
                //获取状态信息
                music_get_status(&g_music_status);
            }
        }
        
        if ((g_status_deal_flag & up_AlbumART) != 0)
        {
            //显示专辑图片
            show_albumart_pic();
        }
        //清标志
        *display_flag &= clr_musicalbumart;
    }

    //显示Audible章节图片
    if ((*display_flag & up_audibleimage) != 0)
    {
        _show_audible_image();
        
        *display_flag &= clr_audibleimage;
    }
    
    //播放状态
    if ((*display_flag & up_musicplaystatus) != 0)
    {
        style.style_id = PLAY_STATUS_PICBOX;
        if (g_music_status.play_status == PlayFast)
        {
            //快进退
            if (g_music_status.fast_status == FFPlaySta)
            {
                picbox_param.frame_id = 2;
            }
            else
            {
                picbox_param.frame_id = 3;
            }
        }
        else
        {
            //正常播放
            if (g_music_status.play_status == PlaySta)
            {
                picbox_param.frame_id = 0;
            }
            else
            {
                picbox_param.frame_id = 1;
            }
        }
        ui_show_picbox(&style, &picbox_param);

        //清标志
        *display_flag &= clr_musicplaystatus;
    }
    //打开音频
    if ((g_status_deal_flag & up_NeedOpenPA) != 0)
    {
#if 0   
        g_status_deal_flag &= clr_NeedOpenPA;
        com_set_sound_out(TRUE, SOUND_OUT_START, NULL);

        if (g_enter_mode == PARAM_BACKTO_PLAYING)
        {
            //恢复音乐播放
            music_send_msg(NULL, NULL, MSG_MENGINE_RESUME_SYNC);

            //获取状态信息
            music_get_status(&g_music_status);

            //设置刷新标志位
            g_display_bitmap |= up_musicplaystatus;
            g_status_deal_flag |= up_Paintflag;
        }
        else
        {
            //关闭音频输出
            com_set_sound_out(FALSE, SOUND_OUT_PAUSE, NULL);
        }
#else
        g_status_deal_flag &= clr_NeedOpenPA;
        music_standby_play();
        //获取状态信息
        music_get_status(&g_music_status);

        //设置刷新标志位
        g_display_bitmap |= up_musicplaystatus;
        g_status_deal_flag |= up_Paintflag;
        
        //过滤多收到的play按键消息
        sys_mq_flush(MQ_ID_GUI);     
#endif
    }

}

