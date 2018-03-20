#include "app_music.h"
#include "app_music_playing.h"

//在.xdata段，该文件.xdata段置于bank段
uint8 bm_buf[SECTOR_SIZE] _BANK_DATA_ATTR_;

#ifdef SUPPORT_ApeCue_SWITCH
const key_map_t cue_key_map_list[] =
{
    /*! UP 按键转换为 SELECT_PREV 事件 */
    {{ KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_PLAYING_PREVSONG },
    /*! DOWN 按键转换为 SELECT_NEXT 事件 */
    {{ KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_PLAYING_NEXTANDAB },
    /*! 短按KEY_PLAY 按键转换为 SELECT_SURE 事件 */
    {{ KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_PAUSE },
    /*! 短按KEY_MENU 按键转换为 ENTER_OPTION 事件 */
    {{ KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_OPTION },
    /*! 短按KEY_RETURN 按键转换为 RETURN 事件 */
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_RETURN },
    /*! 结束标志 */
    {{ KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};
extern void menulist_select_next(menulist_control_t *p_ml_control);
extern void menulist_select_prev(menulist_control_t *p_ml_control);
#endif
/******************************************************************************/
/*!
 * \par  Description:
 *	  浏览项控件
 * \param[in]  mode 浏览模式 选择书签项 msg_call_back 对应的进程私有消息
 * 			   和系统消息处理函数
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
     //设置文件路径
    fsel_set_location(&g_file_path_info.file_path.dirlocation, cur_type);
    if(FALSE == cue_open(&cue_handle, &g_cue_file.total))
    {
        //打开文件失败
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_FILE_OPEN_ERROR);
        return;
    }
    
    for (i = 0; i < cue_show_data.item_valid; i++)
    {
        //解析cue文件
        ape_getinfo_byno(cue_handle, 1 + i + mbk_list_pt->top, &cue_track_p);
        //music_print(cue_track_p.tit2_buffer,0,0);
        
        cue_show_data.items[i].data.str = cue_track_p.tit2_buffer;
        cue_show_data.items[i].language = ANSIDATAAUTO;
        cue_show_data.items[i].length   = ID3_ITEM_SIZE;
        cue_track_p.tit2_buffer = cue_track_p.tit2_buffer + ID3_ITEM_SIZE;
    }
    cue_close(cue_handle);
    
    //当前激活项
    cue_show_data.active = mbk_list_pt->active;
    //之前激活项
    cue_show_data.old = mbk_list_pt->old;
    //所有项总数，用于滑动杆和ratio
    cue_show_data.list_total = mbk_list_pt->total;
    //当前激活项在所有项的位置，用于滑动杆和ratio
    cue_show_data.list_no = mbk_list_pt->list_no;

    ui_show_listbox(&mbk_infor, &cue_show_data, mbk_list_pt->draw_mode);
#endif
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e ui_show_cue(uint8 mode, msg_cb_func msg_call_back)
 * \显示书签项
 * \param[in]    mode : 绘制模式
 * \param[in]    msg_call_back: 消息处理回调函数
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
    input_gui_msg_t input_msg; //输入型消息，即gui消息
    
    //gui 消息对应gui 事件
    msg_apps_type_e gui_event;
    private_msg_t private_msg; //私有消息

    //music_print("in cue",0,0);
    libc_memset(&ml_control_deal, 0, sizeof(ml_control_deal));
    ml_control_deal.total = g_cue_file.total;
    ml_control_deal.bottom = 7 - 1;
    ml_control_deal.draw_mode = LIST_DRAW_ALL;
    ml_control_deal.one_page_count = 7;
    need_read_cue = TRUE;

    if ((g_status_deal_flag & up_ApeNoCue) != 0)
    {
        //没有文件提示
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

        //获取gui消息
        ret = get_gui_msg(&input_msg);
        if (ret == TRUE)//有gui消息
        {
            ////进行按键映射
            if (com_key_mapping(&input_msg, &gui_event, cue_key_map_list) == TRUE)
            {
                result = RESULT_NULL;
                //GUI按键消息处理
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
                    //选择标签
                    result = handle_cue((uint8) ml_control_deal.list_no);
                    g_cue_file.num = (uint8) ml_control_deal.list_no;
                    
                    break;
                    //返回按键
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
            //到此，gui消息队列已经处理完毕
            //处理ap私有消息和系统消息
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
 * \param[in]  active 当前激活项 param1
 * \param[in]  mode 添加 or 删除 param2
 * \param[in]  cur_index 当前项的序号 param3
 * \param[out]
 * \return     uint8
 * \retval     TRUE 成功
 * \retval     FALSE 失败
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
    
    //cue文件中title从1开始   
    file_no = active + 1;

    //设置文件路径
    fsel_set_location(&g_file_path_info.file_path.dirlocation, cur_type);
    //获取指定歌曲信息
    if(TRUE == cue_open(&cue_handle, &g_cue_file.total))
    {
        //标题buffer清零
        libc_memset(g_title_buf, 0, sizeof(g_title_buf));
        //歌手buffer和长度清零
        libc_memset(g_artist_buf, 0, sizeof(g_artist_buf));
        //专辑buffer清零
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
                                cue_p.end_time.msecond - 1000;//减去最后1s
                                
        //快退切歌到上一首的末尾             
        if (fast_mode == FBPlaySta)
        {
            //断点时间
            bk_cue.bp_time_offset = g_cue_file.end_time - 5000; 
        }
        else
        {
            //断点时间
            bk_cue.bp_time_offset = g_cue_file.start_time;
        }
        
        //关闭cue文件
        cue_close(cue_handle);
    }
    else
    {
        //打开文件失败
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_FILE_OPEN_ERROR);
        return RESULT_REDRAW;
    }
    
    g_file.total = g_cue_file.total;
    g_file.num = file_no;//cue文件曲目号从1开始

    //最后一首特殊处理
    if (file_no == g_cue_file.total)
    {
        music_get_fileinfo(&music_fileinfo);
        g_cue_file.end_time = (uint32)(music_fileinfo.total_time - 3000);//减掉最后的3s
    }
    
    //歌曲总时间重新计算
    g_total_time = g_cue_file.end_time - g_cue_file.start_time;
    
    //标志置位   
    g_status_deal_flag |= up_ApeInCue;
    set_scene_cuemode(1);//同步cue列表播歌消息到后台

    //发送定点播放消息
    music_play_cuemusic(&bk_cue);
    //刷新切换歌曲参数
    SetSwitchInterface();
#endif
    return RESULT_PLAYING_SCENE;
}

