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
 * \����ר������album art
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

	//��������
    wregion1.x = ALBUMART_X;
    wregion1.y = ALBUMART_Y;
    wregion1.w = ALBUMART_W;
    wregion1.h = ALBUMART_H;
    file_offset = album_art_info.offset;
	
	//���沥��״̬
	play_status = g_music_status.play_status;
	
    //ֻ�в���״̬����Ҫ��ͣ
    if (play_status == PlaySta)
    {
		music_pause();//��ͣ
		com_set_sound_out(TRUE, SOUND_OUT_STOP, NULL);//ֹͣ��Ƶ������ر�PA
    }
	
    fsel_browser_set_file(&(g_file_path_info.file_path.plist_location), g_file_path_info.file_source);  
	//�����½�ͼƬ
    music_paint_albumart(FALSE, file_offset, &wregion1, &wregion2);

	//ֻ����һ��״̬Ϊ���Ų���Ҫ�ָ�
	if (play_status == PlaySta)
    {
		com_set_sound_out(TRUE, SOUND_OUT_START, NULL);//��ʼ��Ƶ�������PA
		music_resume();//����
	}
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void play_scene_albumart(void)
 * \����ר������album art
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

    //���ڴ�С����
    wregion1.x = ALBUMART_X;
    wregion1.y = ALBUMART_Y;
    wregion1.w = ALBUMART_W;
    wregion1.h = ALBUMART_H;
    
    if (album_picinfo->apic_type == 0)
    {
        //��λ���ļ�
        fsel_browser_set_file(&(g_file_path_info.file_path.plist_location), g_file_path_info.file_source);
        file_offset = album_picinfo->offset;
    }
    else
    {
        //��λ���ļ�
        vfs_file_dir_offset(g_music_mount_id, NULL, &(album_picinfo->cluster_no), 1);
    }
    music_paint_albumart(TRUE, file_offset, &wregion1, &wregion2);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void play_scroll_init(void)
 * \������ʼ������
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
    //��ʼ��
    g_scroll_vars.init = TRUE;
    g_scroll_vars.update = TRUE;
    g_scroll_vars.need_scroll = 0;

    //��ʼ����ʱ�����ñ�־λ
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

    //����������Ҫ��������Ҫ�򿪹�����ʱ��
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
        {//������Ҫ������رն�ʱ��
            kill_app_timer(scroll_frush_id);
            scroll_frush_id = -1;
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void play_scene_paint(uint32 *display_flag)
 * \���Ʋ��Ž���
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
    //Ĭ�ϲ�ˢ�±���
    uint8 draw_bg = FALSE;

    style_infor_t style;
    picbox_private_t picbox_param;

    style.type = UI_AP;
    picbox_param.pic_id = -1;
    picbox_param.frame_id = 0;

    //�����audible �򲻻�ѭ��ģʽ��shuffle��eq��
    if (g_audible_now == TRUE)
    {
        //��ѭ��ģʽ��־
        *display_flag &= clr_musicloopmode;
        //���eq��ʶ
        *display_flag &= clr_musiceqmode;
        //���shuffle��ʶ
        *display_flag &= clr_musicshuffle;
    }
    
    //����
    if ((*display_flag & up_musicBackGDPic) != 0)
    {
        region_t region;
   
        region.x = 0;
        region.y = 0;
        region.width = 320;
        region.height = 28;
        ui_set_backgd_color(Color_BACKCOLOR_def);//��ɫ
        ui_clear_screen(&region);
        
        region.x = 0;
        region.y = 28;
        region.width = 320;
        region.height = 151;
        ui_set_backgd_color(0x39e7);//��ɫ
        ui_clear_screen(&region);
        
        region.x = 0;
        region.y = 179;
        region.width = 320;
        region.height = 61;
        ui_set_backgd_color(Color_BACKCOLOR_def);//��ɫ
        ui_clear_screen(&region);
        
        //���־
        *display_flag &= clr_musicBackGDPic;
        draw_bg = TRUE;
    }
    //������
    if ((*display_flag & up_musicheadbar) != 0)
    {
        //��ͼ
        if (g_music_config.browser_sel == TRUE)
        {
            gui_set_headbar_mode(HEADBAR_MODE_NORMAL, HEADBAR_ICON_BROWSER);
        }
        else
        {
            gui_set_headbar_mode(HEADBAR_MODE_NORMAL, HEADBAR_ICON_MUSIC);
        }
        //���־
        *display_flag &= clr_musicheadbar;
    }
    //��Ŀ��Ϣ
    if ((*display_flag & up_musictracks) != 0)
    {
        //number box
        numbox_private_t music_track_num;

        style.style_id = TRACK_NUMBER_BOX;

        music_track_num.value = g_file.num;

        music_track_num.total = g_file.total;

        if (draw_bg == TRUE)
        {
            //����numbox��ʾ ����͸����ʾ
            ui_show_numbox(&style, &music_track_num, NUMBOX_DRAW_ALL);
        }
        else
        {
            //����numbox��ʾ ����͸����ʾ
            ui_show_numbox(&style, &music_track_num, NUMBOX_DRAW_NUMBER);
        }
        //���־
        *display_flag &= clr_musictracks;
    }
    //����id3��Ϣ
    if ((*display_flag & up_musicinformation) != 0)
    {
        play_scroll_init();
        //���־
        *display_flag &= clr_musicinformation;
    }
    //ʱ����Ϣ(Ϊ����bank�����л��������ѭ��)
    if ((*display_flag & up_musictimeinfo) != 0)
    {
        //��ͼ
        _show_time_progress(TRUE, &style, 0);
        //���־
        *display_flag &= clr_musictimeinfo;
    }
    //eqģʽ
    if ((*display_flag & up_musiceqmode) != 0)
    {
        //��ͼ
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
        //���־
        *display_flag &= clr_musiceqmode;
    }
    //ѭ��ģʽ
    if ((*display_flag & up_musicloopmode) != 0)
    {
        //��ͼ
        music_get_playmode(&play_mode);
        play_mode &= 0x0f;

        style.style_id = LOOP_MODE_PICBOX;

        picbox_param.frame_id = play_mode;
        
        g_setting_comval.music_comval.music_repeat_mode = play_mode;
        
        ui_show_picbox(&style, &picbox_param);
        //���־
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

    //������(Ϊ����bank�����л��������ѭ��)
    if ((*display_flag & up_musicprogress) != 0)
    {
        //��ͼ
        _show_time_progress(TRUE, &style, 1);
        //���־
        *display_flag &= clr_musicprogress;
    }
    //ר��ͼƬ
    if ((*display_flag & up_musicalbumart) != 0)
    {
        //��ʾ����ͼ��
        style.style_id = ALBUM_ART_PICBOX;
        ui_show_picbox(&style, NULL);
        //��ʾ����ͼ��
        style.style_id = ID3_MUSIC_TITLE_PICBOX; 
        ui_show_picbox(&style, NULL);
        //��ʾ����ͼ��
        style.style_id = ID3_MUSIC_ARTIST_PICBOX;
        ui_show_picbox(&style, NULL);
        //��ʾ����ͼ��
        style.style_id = ID3_MUSIC_INFO_PICBOX;
        ui_show_picbox(&style, NULL);
        //��ʾר��ͼ��
        style.style_id = ID3_MUSIC_ALBUM_PICBOX;
        ui_show_picbox(&style, NULL);
        //��ʾ����ͼ��
        style.style_id = PLAY_STATUS_PICBOX;
        picbox_param.frame_id = 1;
        ui_show_picbox(&style, &picbox_param);

        //���������л��Ž��н���ͼƬ��play����
        if ((g_status_deal_flag & up_Switchflag) != 0)
        {
            g_status_deal_flag &= clr_Switchflag;

            if ((g_status_deal_flag & up_AlbumART) != 0)
            {
                //���벢����ר��ͼƬ
                play_paint_albumart(&album_art_info);
            }
            if ((g_status_deal_flag & up_NeedOpenPA) == 0)
            {
                music_play();
                //��ȡ״̬��Ϣ
                music_get_status(&g_music_status);
            }
        }
        
        if ((g_status_deal_flag & up_AlbumART) != 0)
        {
            //��ʾר��ͼƬ
            show_albumart_pic();
        }
        //���־
        *display_flag &= clr_musicalbumart;
    }

    //��ʾAudible�½�ͼƬ
    if ((*display_flag & up_audibleimage) != 0)
    {
        _show_audible_image();
        
        *display_flag &= clr_audibleimage;
    }
    
    //����״̬
    if ((*display_flag & up_musicplaystatus) != 0)
    {
        style.style_id = PLAY_STATUS_PICBOX;
        if (g_music_status.play_status == PlayFast)
        {
            //�����
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
            //��������
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

        //���־
        *display_flag &= clr_musicplaystatus;
    }
    //����Ƶ
    if ((g_status_deal_flag & up_NeedOpenPA) != 0)
    {
#if 0   
        g_status_deal_flag &= clr_NeedOpenPA;
        com_set_sound_out(TRUE, SOUND_OUT_START, NULL);

        if (g_enter_mode == PARAM_BACKTO_PLAYING)
        {
            //�ָ����ֲ���
            music_send_msg(NULL, NULL, MSG_MENGINE_RESUME_SYNC);

            //��ȡ״̬��Ϣ
            music_get_status(&g_music_status);

            //����ˢ�±�־λ
            g_display_bitmap |= up_musicplaystatus;
            g_status_deal_flag |= up_Paintflag;
        }
        else
        {
            //�ر���Ƶ���
            com_set_sound_out(FALSE, SOUND_OUT_PAUSE, NULL);
        }
#else
        g_status_deal_flag &= clr_NeedOpenPA;
        music_standby_play();
        //��ȡ״̬��Ϣ
        music_get_status(&g_music_status);

        //����ˢ�±�־λ
        g_display_bitmap |= up_musicplaystatus;
        g_status_deal_flag |= up_Paintflag;
        
        //���˶��յ���play������Ϣ
        sys_mq_flush(MQ_ID_GUI);     
#endif
    }

}

