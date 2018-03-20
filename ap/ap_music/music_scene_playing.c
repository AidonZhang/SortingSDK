/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-14          1.0              create this file
 *******************************************************************************/

#include "app_music.h"

#include "app_music_playing.h"

#include "app_music_setmenu.h"
/******************************************************************************/
/*!
 * \par  Description:
 * \static bool check_audible(file_location_t* locatptr, music_play_set_e play_set)
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
static bool check_audible(file_location_t* locatptr, music_play_set_e play_set)
{
    id3_type_e cur_type ;
    
    g_audible_now = FALSE;
    cur_type = play_scene_checktype(locatptr->filename);
    
    //在未打开music引擎之前检测当前文件类型是否为audible并加载sys文件
    //否则因为盘符问题处理比较麻烦
    if ((cur_type == ID3_TYPE_AAX) || (cur_type == ID3_TYPE_AA))
    {
        g_audible_now = TRUE;
        if (play_set != PLAY_KEEP)
        {
            music_load_sysfile();
        }
    }

    return g_audible_now;
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
static bool check_apecue(file_location_t* locatptr, music_play_set_e play_set,uint8 load_flag)
{
    bool ret = FALSE;
    id3_type_e cur_type ;
    //music_print("APE",(g_status_deal_flag & up_ApeIsNow),2);
    cur_type = play_scene_checktype(locatptr->filename);
    if (cur_type == ID3_TYPE_APE)
    {
        //第一次进入才读取CUE
        if ((g_status_deal_flag & up_ApeIsNow) == 0)
        {
            if (load_flag == TRUE)
            {
                //APE音乐读取cue文件
                music_load_cuefile();
            }
            //是APE文件播放但是还没有进入CUE列表播放
            g_status_deal_flag |= up_ApeIsNow;
            ret = TRUE;
        }
    }
    else
    {
        //非APE文件
        g_status_deal_flag &= (clr_ApeIsNow & clr_ApeInCue);
    }
    
    return ret;
}

#endif
/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_open_engine(mengine_enter_mode_e start_mode)
 * \装载引擎,如果引擎存在，则不再安装
 * \param[in]    bool  para1
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_open_engine(mengine_enter_mode_e start_mode)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t temp_reply;
    engine_type_e engine_type;

    engine_type = get_engine_type();

    if (engine_type == ENGINE_NULL)
    {
        //无消息内容
        msg.content.data[0] = APP_ID_MENGINE;
        msg.content.data[1] = (uint8)(start_mode | g_file_path_info.file_path.plist_location.disk);
        //消息类型(即消息名称)
        msg.type = MSG_CREAT_APP_SYNC;
        //发送同步消息
        bret = send_sync_msg(APP_ID_MANAGER, &msg, &temp_reply, 0);
    }
    else if (engine_type == ENGINE_MUSIC)
    {
        //引擎已存在，则不再安装
        bret = TRUE;
    }
    else
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_volume(uint8 volume)
 * \设置音量
 * \param[in]    volume 音量等级
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_set_volume(uint8 volume)
{
    bool bret;
    bret = music_send_msg(&volume, NULL, MSG_MENGINE_SET_VOLUME_SYNC);
    //bret = com_set_sound_volume(volume);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _scene_play_seteq(uint8 eq_type)
 * \设置eq
 * \param[in]    eq_type  para1
 * \param[out]   none
 * \return       void the result
 * \retval       void
 * \retval       none
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/
void _scene_play_seteq(uint8 eq_type)
{

    //eq 设置
    switch (eq_type)
    {
        //eq off
        case VALUE_EQ_OFF:
        set_scene_eqnormal_callback();
        break;
        //eq rock
        case VALUE_EQ_ROCK:
        set_scene_eqrock_callback();
        break;
        //ea funk
        case VALUE_EQ_FUNK:
        set_scene_eqfunk_callback();
        break;
        //eq hip hop
        case VALUE_EQ_HIP_HOP:
        set_scene_eqhiphop_callback();
        break;
        //eq jazz
        case VALUE_EQ_JAZZ:
        set_scene_eqjazz_callback();
        break;
        //eq classical
        case VALUE_EQ_CLASSICAL:
        set_scene_eqclassic_callback();
        break;
        //eq teckno
        case VALUE_EQ_TECHNO:
        set_scene_eqtechno_callback();
        break;

        default:
        set_scene_eqnormal_callback();
        break;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _scene_play_init(file_path_info_t* path_locat music_play_set_e play_set)
 * \播放场景初始化
 * \param[in]    paly_set进入播放场景后操作 para1
 *               typedef struct
 *							 {
 *							 	fsel_type_e file_sorce;
 *							 	uint16 reserved;
 *							 	musfile_location_u file_path;
 *							 }file_path_info_t
 * \param[in]    path_locat文件路径信息指针 para2
 *							 typedef enum
 *							 {
 *							 	PLAY_START //设置文件开始播放
 *							 	PLAY_RESUME //从暂停恢复播放
 *							 	PLAY_KEEP //保持播放/暂停状态
 *							 }music_play_set_e;
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略
 * \retval       ......
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/
bool _scene_play_init(file_path_info_t* path_locat, music_play_set_e play_set)
{
    bool ret_val = TRUE;

    engine_type_e engine_type;

    msg_apps_t msg;
    
    fsel_type_e comp_type = path_locat->file_source;
    //指向location的指针
    file_location_t* locatptr = (file_location_t*) &(path_locat->file_path.dirlocation);
    
    if (FALSE == check_audible(locatptr, play_set))
    {
#ifdef SUPPORT_ApeCue_SWITCH
        check_apecue(locatptr, play_set, TRUE);
#endif
    }
    //返回到radio播放场景
    msg.type = MSG_KILL_APP_SYNC; //MSG_KILL_APP; //注销radio后台

    engine_type = get_engine_type();

    if (engine_type == ENGINE_RADIO)
    {
        msg.content.data[0] = APP_ID_FMENGINE;
        //发送同步消息
        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }

    g_status_deal_flag &= clr_NeedOpenPA;

    if (engine_type != ENGINE_MUSIC)
    {
        //打开引擎
        if ((play_set == PLAY_START) || (play_set == PLAY_AUDIBLE_RESUME))
        {
            music_open_engine(ENTER_START);
        }
        else
        {
            g_status_deal_flag |= up_NeedOpenPA;
            if (g_enter_mode == PARAM_BACKTO_PLAYING)
            {
                music_open_engine(ENTER_RESUME);
            }
            else
            {
                music_open_engine(ENTER_RESUME_PAUSE);
            }
        }
        //set eq
        _scene_play_seteq(g_setting_comval.g_comval.eq_type);
        //set playspeed
        set_scene_speed_callback(g_music_config.playspeed);
        //set volume
        music_set_volume(g_setting_comval.g_comval.volume_current);
    }

    //之所以要限制为PLAY_KEEP,是因为前面两种都是重新开始播放，需要根据
    //setting中的循环模式进行设置。而对于PLAY_KEEP,如果正常播放，则不该
    //改变循环模式;如果是standby起来，也应该根据引擎的参数自己设置，而不
    //应该由应用设置
    if (play_set != PLAY_KEEP)
    {
        //为满足在未启动引擎之前 在setting应用设置循环模式和eq的情况
        if (g_audible_now == FALSE)
        {
            if ((g_status_deal_flag & up_ShuttleTmporary) == 0)
            {
                music_set_playmode(g_setting_comval.music_comval.music_repeat_mode
                        | g_setting_comval.music_comval.music_shuffle);
            }
            else
            {
                //临时shuffle
                music_set_playmode(FSEL_MODE_NORMAL | 0x10);
            }
        }
        else
        {
            //强制全部循环
            music_set_playmode(FSEL_MODE_LOOPALL);
        }
    }

    if (play_set == PLAY_START)
    {
        path_locat->file_source = g_browser_filepath.file_source;

        //设置文件路径
        ret_val = music_set_filepath(path_locat);

        //只关心music_set_filepath是否成功
        //解析完专辑图片后再播放 20151210修改
        if(ret_val == TRUE)
        {
            music_switch();
        }
        else
        {
            music_close_engine();
            //提示出错
            //gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_FILE_OPEN_ERROR);
        }
    }
    else if (play_set == PLAY_AUDIBLE_RESUME)
    {
        //设置文件路径
        ret_val = music_set_filepath(path_locat);

        //只关心music_set_filepath是否成功
        if(ret_val == TRUE)
        {
            //选择歌曲播放
            music_play_bookmark(&g_audible_bkinfo);
        }

        //浏览类型
        comp_type = g_browser_filepath.file_source;

        //断点播放时设置状态为播放状态
        g_music_status.play_status = PlaySta;
    }
    else
    {
        //保持原有状态
        //当前播放的总数和曲目号
        music_get_filepath(path_locat);//获取文件路径

        //查看此时盘符是否存在
        if(locatptr->disk == DISK_H)
        {
            if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
            {
                //检测不到卡盘
                music_deal_sd_out();
                return FALSE;
            }
        }

        //保持原有播放状态时,需要初始化文件选择器为原有状态
        if (path_locat->file_path.dirlocation.file_total != 0)
        {
            comp_type = g_browser_filepath.file_source;
            
            //对于standby起来的歌曲，之前并不知道具体文件类型
            //因此要再次确认
            if (FALSE == check_audible(locatptr, play_set))
            {
        #ifdef SUPPORT_ApeCue_SWITCH
                check_apecue(locatptr, play_set, FALSE);
        #endif
            }
        
        }
        else
        {
            //提示出错
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_FILE_NO_FILE);
            music_close_engine();
            ret_val = FALSE;
        }
    }

    if(ret_val == TRUE)
    {
        if ((path_locat->file_source != comp_type) || (locatptr->disk != g_browser_filepath.file_path.dirlocation.disk))
        {
            fsel_exit();
            ret_val = music_file_init(path_locat);

            //重新设置m3u location，
            //解决从m3u选择文件播放后返回到搜索曲目，
            //然后选择正常播放后无法删除的问题,
            //主要是退出后enhance中m3u index被清除，这里需要重设一下
            if((path_locat->file_source == FSEL_TYPE_M3ULIST)&&((play_set == PLAY_KEEP)))
            {
                fsel_browser_set_location(&(path_locat->file_path.plist_location),FSEL_TYPE_M3ULIST);
            }

        }

        //同步location信息
        libc_memcpy(&g_browser_filepath, path_locat, sizeof(file_path_info_t));

        //用于显示
        //g_file.total = locatptr->file_total;
        //g_file.num = locatptr->file_num;

        //设置显示刷新timer 置位当前时间、歌词等显示标识1s
        time_frush_id = set_app_timer(APP_TIMER_ATTRB_UI, 250, play_flag_update);

        //播放卡上音乐
        if (locatptr->disk == DISK_H)
        {
            change_card_state(CARD_STATE_CARD_PLAY);
        }

        
        if (g_audible_now == TRUE)
        {
            //需要播放audible，存在断点
            g_music_config.audible_bk_flag = TRUE;
            g_display_bitmap &= clr_audibleimage;
        }

        //设置为播放状态
        change_app_state(APP_STATE_PLAYING);
#ifdef PRINT_BANK_INFO
        print_label = 0x12345678;
#endif

    }

    return ret_val;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _scene_play_exit(void)
 * \退出播放场景
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/
bool _scene_play_exit(void)
{
    //滚屏定时器取消
    kill_app_timer(scroll_frush_id);
    scroll_frush_id = -1;

    kill_app_timer(time_frush_id);
    time_frush_id = -1;

    //设置为非播放状态
    change_app_state(APP_STATE_NO_PLAY);

#ifdef PRINT_BANK_INFO
    print_label = 0;
#endif
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint6 music_scene_playing(app_param_e enter_mode file_path_info_t* path_locat music_play_set_e paly_set)
 * \播放场景处理
 * \param[in]    enter_mode进入模式  para1
 *               typedef enum
 *							 {
 *							 	ENTER_FROM_DESKTOP
 *							 	ENTER_FROM_BROWSER
 *							 	ENTER_FROM_PLAYNOW
 *							 	ENTER_FROM_UI_MENU
 *							 	MODEMAX
 *							 }app_param_e;
 * \param[in]    paly_set进入播放场景后操作 para2
 *               typedef struct
 *							 {
 *							 	fsel_type_e file_sorce;
 *							 	uint16 reserved;
 *							 	musfile_location_u file_path;
 *							 }file_path_info_t
 * \param[in]    path_locat文件路径信息指针 para3
 *							 typedef enum
 *							 {
 *							 	PLAY_START //设置文件开始播放
 *							 	PLAY_RESUME //从暂停恢复播放
 *							 	PLAY_KEEP //保持播放/暂停状态
 *							 }music_play_set_e;
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略
 * \retval       ......
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/

app_result_e music_scene_playing(app_param_e enter_mode, file_path_info_t* path_locat, music_play_set_e paly_set)
{
    bool result;
    app_result_e ret_vals;

    //场景初始化 初始化文件选择器，获取UI需显示的数据等
    //music_print("scene_play",0,0);
    result = _scene_play_init(path_locat, paly_set);
    if (result == FALSE)
    {
        //回到列表菜单
        return RESULT_LISTMENU_MENU;
    }
    //刷新界面标识
    SetFullInterface()
    //场景循环
    g_error_num = 0;
    g_speed_counter = 0;
    g_status_deal_flag |=(up_Paintflag | up_MengineStatus);
    g_status_deal_flag &= clr_LrcMulScreen;

 
#ifdef SUPPORT_ApeCue_SWITCH
    if ((g_status_deal_flag & up_ApeInCue) == 0)
    {
        g_total_time = 0;
    }
#else
    g_total_time = 0;
#endif

    ret_vals = playing_loop_deal(path_locat);
    //调用场景退出函数
    _scene_play_exit();
    //music_print("scene_exit",0,0);
    return ret_vals;
}
