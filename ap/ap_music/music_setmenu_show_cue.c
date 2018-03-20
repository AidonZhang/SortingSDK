#include "app_music.h"
#include "app_music_playing.h"

//��.xdata�Σ����ļ�.xdata������bank��
uint8 bm_buf[SECTOR_SIZE] _BANK_DATA_ATTR_;

#ifdef SUPPORT_ApeCue_SWITCH
const key_map_t cue_key_map_list[] =
{
    /*! UP ����ת��Ϊ SELECT_PREV �¼� */
    {{ KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_PLAYING_PREVSONG },
    /*! DOWN ����ת��Ϊ SELECT_NEXT �¼� */
    {{ KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_PLAYING_NEXTANDAB },
    /*! �̰�KEY_PLAY ����ת��Ϊ SELECT_SURE �¼� */
    {{ KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_PAUSE },
    /*! �̰�KEY_MENU ����ת��Ϊ ENTER_OPTION �¼� */
    {{ KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_OPTION },
    /*! �̰�KEY_RETURN ����ת��Ϊ RETURN �¼� */
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_RETURN },
    /*! ������־ */
    {{ KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};
extern void menulist_select_next(menulist_control_t *p_ml_control);
extern void menulist_select_prev(menulist_control_t *p_ml_control);
#endif
/******************************************************************************/
/*!
 * \par  Description:
 *	  �����ؼ�
 * \param[in]  mode ���ģʽ ѡ����ǩ�� msg_call_back ��Ӧ�Ľ���˽����Ϣ
 * 			   ��ϵͳ��Ϣ������
 * \param[out]
 * \return
 * \retval
 * \ingroup     music_ui
 * \note
 *******************************************************************************/
void ui_show_cue_title(menulist_control_t* mbk_list_pt)
{
#ifdef SUPPORT_ApeCue_SWITCH
    uint8 i;
    style_infor_t mbk_infor;
    listbox_private_t cue_show_data;
    cue_track_t cue_track_p;
    handle cue_handle;
    fsel_type_e cur_type = g_file_path_info.file_source;

    cue_track_p.tit2_buffer = bm_buf;
    cue_track_p.tpe1_buffer = NULL;
    cue_track_p.talb_buffer = NULL;
    cue_track_p.genre_buffer = NULL;
    cue_track_p.tit2_length = ID3_ITEM_SIZE;
    cue_track_p.tpe1_length = 0;
    cue_track_p.talb_length = 0;
    cue_track_p.genre_length = 0;

    mbk_infor.style_id = FILELIST;
    mbk_infor.type     = UI_COM;
    
    libc_memset(bm_buf, 0x00, SECTOR_SIZE);

    cue_show_data.item_valid = mbk_list_pt->bottom - mbk_list_pt->top + 1;
     //�����ļ�·��
    fsel_set_location(&g_file_path_info.file_path.dirlocation, cur_type);
    if(FALSE == cue_open(&cue_handle, &g_cue_file.total))
    {
        //���ļ�ʧ��
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_FILE_OPEN_ERROR);
        return;
    }
    
    for (i = 0; i < cue_show_data.item_valid; i++)
    {
        //����cue�ļ�
        ape_getinfo_byno(cue_handle, 1 + i + mbk_list_pt->top, &cue_track_p);
        //music_print(cue_track_p.tit2_buffer,0,0);
        
        cue_show_data.items[i].data.str = cue_track_p.tit2_buffer;
        cue_show_data.items[i].language = ANSIDATAAUTO;
        cue_show_data.items[i].length   = ID3_ITEM_SIZE;
        cue_track_p.tit2_buffer = cue_track_p.tit2_buffer + ID3_ITEM_SIZE;
    }
    cue_close(cue_handle);
    
    //��ǰ������
    cue_show_data.active = mbk_list_pt->active;
    //֮ǰ������
    cue_show_data.old = mbk_list_pt->old;
    //���������������ڻ����˺�ratio
    cue_show_data.list_total = mbk_list_pt->total;
    //��ǰ���������������λ�ã����ڻ����˺�ratio
    cue_show_data.list_no = mbk_list_pt->list_no;

    ui_show_listbox(&mbk_infor, &cue_show_data, mbk_list_pt->draw_mode);
#endif
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e ui_show_cue(uint8 mode, msg_cb_func msg_call_back)
 * \��ʾ��ǩ��
 * \param[in]    mode : ����ģʽ
 * \param[in]    msg_call_back: ��Ϣ����ص�����
 * \param[in]
 * \param[out]
 * \return       uint16
 * \note
 *******************************************************************************/
app_result_e ui_show_cue(msg_cb_func msg_call_back)
{
    app_result_e result = RESULT_NULL;

#ifdef SUPPORT_ApeCue_SWITCH
    bool ret;
    bool need_read_cue;
    menulist_control_t ml_control_deal;
    input_gui_msg_t input_msg; //��������Ϣ����gui��Ϣ
    
    //gui ��Ϣ��Ӧgui �¼�
    msg_apps_type_e gui_event;
    private_msg_t private_msg; //˽����Ϣ

    //music_print("in cue",0,0);
    libc_memset(&ml_control_deal, 0, sizeof(ml_control_deal));
    ml_control_deal.total = g_cue_file.total;
    ml_control_deal.bottom = 7 - 1;
    ml_control_deal.draw_mode = LIST_DRAW_ALL;
    ml_control_deal.one_page_count = 7;
    need_read_cue = TRUE;

    if ((g_status_deal_flag & up_ApeNoCue) != 0)
    {
        //û���ļ���ʾ
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_FILE_NO_FILE);
        result = RESULT_REDRAW;
        return result;
    }
    
    while (1)
    {
        if (ml_control_deal.draw_mode != LIST_DRAW_NULL)
        {
            ui_show_cue_title(&ml_control_deal);
            ml_control_deal.draw_mode = LIST_DRAW_NULL;
        }

        //��ȡgui��Ϣ
        ret = get_gui_msg(&input_msg);
        if (ret == TRUE)//��gui��Ϣ
        {
            ////���а���ӳ��
            if (com_key_mapping(&input_msg, &gui_event, cue_key_map_list) == TRUE)
            {
                result = RESULT_NULL;
                //GUI������Ϣ����
                switch (gui_event)
                {
                    case EVENT_PLAYING_NEXTANDAB:
                    menulist_select_next(&ml_control_deal);
                    break;

                    case EVENT_PLAYING_PREVSONG:
                    menulist_select_prev(&ml_control_deal);
                    break;

                    case EVENT_PLAYING_OPTION:
                    result = RESULT_REDRAW;
                    break;

                    case EVENT_PLAYING_PAUSE:
                    //ѡ���ǩ
                    result = handle_cue((uint8) ml_control_deal.list_no);
                    g_cue_file.num = (uint8) ml_control_deal.list_no;
                    
                    break;
                    //���ذ���
                    case EVENT_PLAYING_RETURN:
                    result = RESULT_REDRAW;
                    break;
                    default:
                    result = com_message_box(gui_event);
                    break;
                }
                if ((result != RESULT_NULL) && (result != RESULT_IGNORE))
                {
                    break;
                }
            }
        }
        else
        {
            //���ˣ�gui��Ϣ�����Ѿ��������
            //����ap˽����Ϣ��ϵͳ��Ϣ
            ret = get_app_msg(&private_msg);
            if (ret == TRUE)
            {
                result = msg_call_back(&private_msg);
                if ((result == RESULT_REDRAW) || (result == RESULT_NULL))
                {
                    need_read_cue = TRUE;
                    ml_control_deal.draw_mode = LIST_DRAW_ALL;
                }
                else
                {
                    //goto menu_exit;
                    break;
                }
            }
        }
        sys_os_time_dly(2);

    }//end of while(1)
#endif
    return result;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e handle_cue(uint8 active, uint8 mode, uint16 cur_index)
 * \
 * \param[in]  active ��ǰ������ param1
 * \param[in]  mode ��� or ɾ�� param2
 * \param[in]  cur_index ��ǰ������ param3
 * \param[out]
 * \return     uint8
 * \retval     TRUE �ɹ�
 * \retval     FALSE ʧ��
 * \ingroup    ape cue
 * \note
 *******************************************************************************/
app_result_e handle_cue(uint8 active)
{
#ifdef SUPPORT_ApeCue_SWITCH

    uint8 file_no;
    handle cue_handle;
    mmm_mp_file_info_t music_fileinfo;    
    mmm_mp_bp_info_t bk_cue;
    cue_track_t cue_p;

    uint8 fast_mode = g_music_status.fast_status;
    fsel_type_e cur_type = g_file_path_info.file_source;
    
    //cue�ļ���title��1��ʼ   
    file_no = active + 1;

    //�����ļ�·��
    fsel_set_location(&g_file_path_info.file_path.dirlocation, cur_type);
    //��ȡָ��������Ϣ
    if(TRUE == cue_open(&cue_handle, &g_cue_file.total))
    {
        //����buffer����
        libc_memset(g_title_buf, 0, sizeof(g_title_buf));
        //����buffer�ͳ�������
        libc_memset(g_artist_buf, 0, sizeof(g_artist_buf));
        //ר��buffer����
        libc_memset(g_album_buf, 0, sizeof(g_album_buf));
        
        cue_p.tit2_buffer = g_title_buf;
        cue_p.tit2_length = Id3BufSIZE;
        cue_p.tpe1_buffer = g_artist_buf;
        cue_p.tpe1_length = Id3BufSIZE;
        cue_p.talb_buffer = g_album_buf;
        cue_p.talb_length = Id3BufSIZE;
        
        ape_getinfo_byno(cue_handle,file_no,&cue_p);
        g_cue_file.start_time = cue_p.start_time.minute * 60 * 1000 +
                                cue_p.start_time.second * 1000 +
                                cue_p.start_time.msecond;
        g_cue_file.end_time =   cue_p.end_time.minute * 60 * 1000 + 
                                cue_p.end_time.second * 1000 +
                                cue_p.end_time.msecond - 1000;//��ȥ���1s
                                
        //�����и赽��һ�׵�ĩβ             
        if (fast_mode == FBPlaySta)
        {
            //�ϵ�ʱ��
            bk_cue.bp_time_offset = g_cue_file.end_time - 5000; 
        }
        else
        {
            //�ϵ�ʱ��
            bk_cue.bp_time_offset = g_cue_file.start_time;
        }
        
        //�ر�cue�ļ�
        cue_close(cue_handle);
    }
    else
    {
        //���ļ�ʧ��
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_FILE_OPEN_ERROR);
        return RESULT_REDRAW;
    }
    
    g_file.total = g_cue_file.total;
    g_file.num = file_no;//cue�ļ���Ŀ�Ŵ�1��ʼ

    //���һ�����⴦��
    if (file_no == g_cue_file.total)
    {
        music_get_fileinfo(&music_fileinfo);
        g_cue_file.end_time = (uint32)(music_fileinfo.total_time - 3000);//��������3s
    }
    
    //������ʱ�����¼���
    g_total_time = g_cue_file.end_time - g_cue_file.start_time;
    
    //��־��λ   
    g_status_deal_flag |= up_ApeInCue;
    set_scene_cuemode(1);//ͬ��cue�б�����Ϣ����̨

    //���Ͷ��㲥����Ϣ
    music_play_cuemusic(&bk_cue);
    //ˢ���л���������
    SetSwitchInterface();
#endif
    return RESULT_PLAYING_SCENE;
}

