/*******************************************************************************
 *                              US212A
 *                            Module: music engine
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      fiona.yang  2011-09-07        1.0              create this file
 *******************************************************************************/

#include "app_mengine.h"
#include "mengine_rdata.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \void _file_switch_info(void)
 * \文件信息获取
 * \param[in]    play_mode_e para1
 * \param[in]    last_mode:

 * \param[out]   none
 * \return       the result
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_play_deal.c
 * \note
 */
/*******************************************************************************/
bool _file_switch_info(void)
{
    bool ret = TRUE;
    
    //获取文件信息，如播放总时间
    ret = _set_file();
    if (ret == 0)
    {
        g_set_file_flag = FALSE;
    }
    //通过SET_FILE会检测命令是否正确，但对FILE_SWITCH_INFO,不关心错误号
    //需要清除错误号
    //if(g_eg_status_p->err_status != EG_ERR_NONE)
    //{
    g_eg_status_p->err_status = EG_ERR_NONE;
    //}
    
    if (mengine_enter_mode == ENTER_START)
    {
        //关闭文件
        mmm_mp_cmd(mp_handle, MMM_MP_CLEAR_FILE, (unsigned int) NULL);
    }
    
    if (g_is_audible == TRUE)
    {
        _check_pos_time();
    }

    return ret;
}


/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_set_section_mode(void* msg_ptr)
 * \设置audible的章节模式/标题模式
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   msg_ptr->msg.content.addr //传送eq信息的指针
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_set_section_mode(void* msg_ptr)
{
    //设置内容指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    uint8 last_mode = mengine_info.eg_config.section_mod;

    //将消息指针指向的数据copy到引擎变量
    libc_memcpy(&mengine_info.eg_config.section_mod, data_ptr->msg.content.addr, sizeof(audible_switch_mode_e));
    //待修改
    mmm_mp_cmd(mp_handle, MMM_MP_SET_CHAPTER, (unsigned int) mengine_info.eg_config.section_mod);

    //从文件模式切换到章节模式
    if ((last_mode == SWITCH_TITLE) && (mengine_info.eg_config.section_mod == SWITCH_SECTION))
    {
        _check_audible_chapter();
        if (mengine_info.eg_status.play_status == PlaySta)
        {
            _set_chapter_timer();
        }
        mengine_info.eg_playinfo.cur_file_switch |= 0x02;
    }

    //从章节模式切换到文件模式
    if ((mengine_info.eg_config.section_mod == SWITCH_TITLE) && (last_mode == SWITCH_SECTION))
    {
        //mengine_info.eg_config.location.plist_location.file_total = audbile_tracks.locat_total;
        //mengine_info.eg_config.location.plist_location.file_num = audbile_tracks.loact_num;
        _clr_chapter_timer();
    }

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_fastplay_step(void *msg_ptr)
 * \快进退步长控制
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略
 * \retval       Exit_App 退出app
 * \ingroup      mengine_mesasge_done.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_fastplay_step(void *msg_ptr)
{
    //消息指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;
    //速度参数 毫秒为单位
    uint32 *speed_ptr = (uint32 *) (data_ptr->msg.content.addr);

    //是正常播放 or 快进
    if ((g_eg_status_p->play_status == PlaySta) || (g_eg_status_p->play_status == PlayFast))
    {
        mmm_mp_cmd(mp_handle, MMM_MP_FFB_STEP, (unsigned int) (*speed_ptr));//快进
    }

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_get_bkinfo(void* msg_ptr)
 * \获取当前音乐断点信息
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   msg_ptr->msg.content.addr //传送循环方式的指针
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_get_bkinfo(void* msg_ptr)
{
    //消息指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    mengine_status_t *eg_status = &mengine_info.eg_status;
    mengine_config_t *eg_cfg = &mengine_info.eg_config;
    //正在播放
    if (eg_status->play_status == PlaySta)
    {
        //获取断点
        mmm_mp_cmd(mp_handle, MMM_MP_GET_BREAK_POINT, (unsigned int) &(eg_cfg->bk_infor));
    }
    //copy当前播放时间和比特率到消息指针指向的数据区
    libc_memcpy(data_ptr->reply->content, &(eg_cfg->bk_infor), sizeof(mmm_mp_bp_info_t));

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_play_cuemusic(void* msg_ptr)
 * \设置cue列表播放
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_play_cuemusic(void* msg_ptr)
{
    bool ret_vals;

    //消息指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    //mengine_status_t *eg_status = &mengine_info.eg_status;
    mengine_config_t *eg_cfg = &mengine_info.eg_config;

    //void * data_ptr = msg_ptr->msg.content.addr;//存放书签断点的数据指针

    //当前是播放状态,则停止播放

    ret_vals = _stop(STOP_PAUSE);//暂停播放,由内部保证是播放状态才停止
    if (ret_vals == FALSE)
    {
        goto msg_end;
    }

    //保存断点,必须在_stop之后,因为_stop中会获取断点，放在_stop之前断点会被覆盖
    libc_memcpy(&eg_cfg->bk_infor, data_ptr->msg.content.addr, sizeof(mmm_mp_bp_info_t));

    //重新开始播放, 设置文件
    //ret_vals = _set_file();
    //if (ret_vals == FALSE)
    //{
    //    goto msg_end;
    //}

    ret_vals = _play(PLAY_RESUME);//播放

    msg_end:
    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_set_playmode(void* msg_ptr)
 * \设置循环模式
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   msg_ptr->msg.content.addr //传送循环方式的指针
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_set_playmode(void* msg_ptr)
{

    //设置循环模式的指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;
    uint8 cur_mode;
    uint8 old_mode;
    uint8 buf_mode;
    mengine_status_t *eg_status = &mengine_info.eg_status;

    cur_mode = *(uint8*) (data_ptr->msg.content.addr);//获取当前循环模式
    old_mode = mengine_info.eg_config.repeat_mode;
    
    mengine_info.eg_config.shuffle_flag = (uint8)(cur_mode & 0x30);//保存shuffle模式
    mengine_info.eg_config.repeat_mode = (cur_mode & 0x0f);//保存循环模式
    mengine_info.eg_config.cue_flag = (uint8)(cur_mode & 0xc0);//cue列表模式
    //music_print("<CUE> ",mengine_info.eg_config.cue_flag,2);
    
    if ((eg_status->play_status == StopSta) || (eg_status->play_status == PauseSta))
    {
        buf_mode = (uint8)(cur_mode & 0x3f);
        fsel_set_mode(buf_mode);//设置循环模式
    }
    else
    {
        //只有当前模式更改为浏览模式而之前模式不为浏览模式时才进行设置
        if (((cur_mode & 0x0f) == FSEL_MODE_INTRO) && (old_mode != FSEL_MODE_INTRO))
        {
            mmm_mp_status_t mmm_status;
            mmm_mp_cmd(mp_handle, MMM_MP_GET_STATUS, (unsigned int) &mmm_status);
            g_intro_start = (uint32) mmm_status.cur_time;
        }
    }

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_clear_error(void* msg_ptr)
 * \清除错误号
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_clear_error(void* msg_ptr)
{
    mengine_status_t *eg_status = &mengine_info.eg_status;

    eg_status->err_status = EG_ERR_NONE;//错误号清除

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void change_locat_deal(void)
 * \文件路径修改的处理
 * \param[in]    para1
 * \param[in]    none:
 * \param[out]   none
 * \return       the result
 * \ingroup      mengine_control.c
 * \note
 */
/*******************************************************************************/
bool change_locat_deal(void)
{
    bool ret_val = TRUE;

    //配置指针
    mengine_config_t *eg_cfg = &mengine_info.eg_config;

    if (g_path_type != eg_cfg->fsel_type)
    {
        ret_val = FALSE;
    }
    else
    {
        ret_val = fsel_set_location(&(eg_cfg->location.plist_location), eg_cfg->fsel_type);
    }

    if (ret_val == FALSE)
    {
        fsel_exit();
        ret_val = mengine_file_init();//重新初始化
    }
    
    g_change_path_flag = 0;

    return ret_val;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_musui_delete_file(void* msg_ptr)
 * \删除当前播放的文件
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_musui_delete_file(void* msg_ptr)
{
    bool is_playing;
    bool ret_vals;
    //bool switch_ret;

    plist_location_t* cur_locat_ptr = (plist_location_t*) &(g_eg_cfg_p->location.plist_location);

    //当前播放状态
    if (g_eg_status_p->play_status == PlaySta)
    {
        is_playing = TRUE;
    }
    else
    {
        is_playing = FALSE;
    }
    ret_vals = _stop(STOP_NORMAL);//停止播放
    if (ret_vals == FALSE)
    {
        goto msg_end;
    }

    cur_locat_ptr->file_num--;

    cur_locat_ptr->file_total--;

    //设置删除当前文件标识
    g_del_curfile = 1;

    if (g_change_path_flag == 1)
    {
        ret_vals = change_locat_deal();
    }

    //清除断点
    libc_memset(&(g_eg_cfg_p->bk_infor), 0, sizeof(mmm_mp_bp_info_t));

    msg_end:
    //返回成功
    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_other_delete_file(void* msg_ptr)
 * \删除浏览的文件
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_other_delete_file(void* msg_ptr)
{
    bool ret_vals;

    //消息指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    //要删除的文件信息指针
    file_path_info_t* del_file_path = (file_path_info_t *) (data_ptr->msg.content.addr);

    //当前正在播放的文件指针
    file_location_t* cur_locat_ptr = (file_location_t*) &(g_eg_cfg_p->location.dirlocation);

    uint32 del_clustno = del_file_path->file_path.dirlocation.cluster_no;//删除文件的目录项所在的簇号

    uint32 del_direntry = del_file_path->file_path.dirlocation.dir_entry;//删除文件的目录项在所在簇号内的偏移

    //判断是否是删除当前文件，如果是则停止播放
    if ((del_clustno == cur_locat_ptr->cluster_no) && (del_direntry == cur_locat_ptr->dir_entry))
    {
        ret_vals = _stop(STOP_NORMAL);//停止播放

        cur_locat_ptr->file_num--;

        cur_locat_ptr->file_total--;

        //设置删除当前文件标识
        g_del_curfile = 1;

        if (g_change_path_flag == 1)
        {
            ret_vals = change_locat_deal();
        }

        //清除断点
        libc_memset(&(g_eg_cfg_p->bk_infor), 0, sizeof(mmm_mp_bp_info_t));
    }
    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_del_favorite_deal(void* msg_ptr)
 * \删除当前播放的收藏夹中的文件
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_del_favorite_deal(void* msg_ptr)
{
    //删除文件序号的指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;
    //删除文件序号
    uint16 del_num;

    //当前正在播放的文件指针
    flist_location_t* cur_locat_ptr = (flist_location_t*) &(mengine_info.eg_config.location.flist_location);

    del_num = *(uint16*) (data_ptr->msg.content.addr);//获取当前删除文件序号

    if (cur_locat_ptr->file_num >= del_num)
    {
        cur_locat_ptr->file_num--;
    }
    cur_locat_ptr->file_total--;
    //删除文件
    g_del_curfile = 1;

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_add_favorite_deal(void* msg_ptr)
 * \添加当前播放的收藏夹中的文件
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_add_favorite_deal(void* msg_ptr)
{
    //当前正在播放的文件指针
    flist_location_t* cur_locat_ptr = (flist_location_t*) &(mengine_info.eg_config.location.flist_location);

    cur_locat_ptr->file_total++;
    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int _error_timeout_handle(void* param)
 * \后台music播放时出错文件自动切换下一曲函数
 * \param[in]    param  para1
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略
 * \retval
 * \ingroup      mengine_control.c
 * \note
 * \li
 */
/*******************************************************************************/
void _error_timeout_handle(void)
{
    //mengine_config_t *cfg_ptr = &mengine_info.eg_config;
    //mengine_status_t *status = &mengine_info.eg_status;
    uint8 loop_mode;
    uint16 loopcnt;
    bool ret_val;

    //codec和enhanced复用关系，需先关闭解码中间件，再更改循环模式
    _stop(STOP_NORMAL);

    //临时设置循环模式为所有循环
    loop_mode = g_eg_cfg_p->shuffle_flag;//循环模式shuffle
    loop_mode |= FSEL_MODE_LOOPALL;
    fsel_set_mode(loop_mode);

    loopcnt = fsel_get_total(FALSE);//获取当前模式的文件总数
    //循环查找,直到找到可播放文件
    while (loopcnt > 0)
    {
        loopcnt--;
        ret_val = mengine_file_switch(STOP_NORMAL, END_SWITCH, PLAY_NORMAL);
        if (ret_val == SWITCH_NO_ERR)
        {
            break;
        }
        else if (ret_val == SWITCH_ERR_PLAY)
        {
            //因此时歌曲未播放，如果播放出错，需调用CLEAR_FILE命令关闭文件句柄
            mmm_mp_cmd(mp_handle, MMM_MP_CLEAR_FILE, (unsigned int) NULL);
        }
        else
        {
            ;//nothing
        }

        sys_os_time_dly(g_delay_time);
    }

    //if (loopcnt == 0)
    //{
        //未找到有效可以播放的歌曲
    //    g_eg_cfg_p->location.dirlocation.file_total = 0;
    //}

    //恢复默认的循环模式
    loop_mode |= (uint8) g_eg_cfg_p->repeat_mode;
    sys_os_sched_lock();
    fsel_set_mode(loop_mode);
    sys_os_sched_unlock();
}

/******************************************************************************/
/*!
 * \par  Description:
 * \eg_err_e _convert_error_no(mmm_mp_err_t err_no)
 * \转换解码库的错误号
 * \param[in]    err_no错误号  para1
 * \param[in]    mmm_mp_err_t:
 *							 typedef enum
 *							 {
 *							 MMM_MP_ERR_OPEN_FILE            = 0x10001,
 *							 MMM_MP_ERR_FILE_NOT_SUPPORT     = 0x10002,
 *							 MMM_MP_ERR_FILE_DAMAGED         = 0x10003,
 *							 MMM_MP_ERR_DECODER_ERROR        = 0x10004,
 *							 MMM_MP_ERR_NO_LICENSE           = 0x10005,
 *							 MMM_MP_ERR_SECURE_CLOCK         = 0x10006,
 *							 MMM_MP_ERR_LICENSE_INFO         = 0x10007,
 *							 MMM_MP_ERR_CMD_ILLEGAL          = 0x10008,
 *							 MMM_MP_ERR_CMD_NOTEXIST         = 0x10009,
 *							 MMM_MP_ERR_SEEK_BEHIND          = 0x1000A,
 *							 MMM_MP_ERR_PARSER_LOAD          = 0x1000B,
 *							 MMM_MP_ERR_DECODER_LOAD         = 0x1000C,
 *							 MMM_MP_ERR_FIFO_CREATE          = 0x1000D,
 *							 MMM_MP_ERR_TIME_OVERFLOW        = 0x1000E,
 *							 MMM_MP_ERR_IO_MISMATCH          = 0x1000F,
 *							 MMM_MP_ERR_FF_END               = 0x10010,
 *							 MMM_MP_ERR_FB_START             = 0x10011,
 *							 MMM_MP_ERR_PARSER_ERROR         = 0x10012,
 *							 MMM_MP_ERR_DEVICE_ERROR         = 0x10013,
 *							 MMM_MP_ERR_UNKNOWN              = 0x100FF
 *							 } mmm_mp_err_t;
 * \param[out]   none
 * \return       eg_err_e the result
 * \retval       typedef enum
 *							 {
 *							 EG_ERR_NONE = 0,
 *							 EG_ERR_OPEN_FILE,//打开文件失败
 *							 EG_ERR_NOT_SUPPORT,//文件格式不支持
 *							 EG_ERR_DECODER_ERROR,//解码出错
 *							 EG_ERR_NO_LICENSE,//无license，播放次数已满或是播放时间已经到期
 *							 EG_ERR_SECURE_CLOCK,//DRM时钟错误
 *							 EG_ERR_DRM_INFO//DRM信息错误
 *							 }eg_err_e;
 * \retval
 * \ingroup      mengine_mesasge_done.c
 * \note
 */
/*******************************************************************************/
eg_err_e _convert_error_no(mmm_mp_err_t err_no)
{
    eg_err_e err_status = EG_ERR_NONE; //当前错误状态

    switch (err_no)
    {
        case MMM_MP_ERR_OPEN_FILE:
        err_status = EG_ERR_OPEN_FILE;//打开文件失败
        break;

        case MMM_MP_ERR_FILE_NOT_SUPPORT://文件格式不支持
        case MMM_MP_ERR_FILE_DAMAGED://文件损毁
        err_status = EG_ERR_NOT_SUPPORT;
        break;

        case MMM_MP_ERR_DECODER_ERROR://解码出错
        err_status = EG_ERR_DECODER_ERROR;
        break;

        case MMM_MP_ERR_NO_LICENSE://无license
        err_status = EG_ERR_NO_LICENSE;
        break;

        case MMM_MP_ERR_SECURE_CLOCK://DRM时钟错误
        err_status = EG_ERR_SECURE_CLOCK;
        break;

        case MMM_MP_ERR_LICENSE_INFO://播放次数已满或是播放时间已经到期
        err_status = EG_ERR_DRM_INFO;
        break;

        default:
        err_status = EG_ERR_DECODER_ERROR;//解码出错
        break;
    }

    return err_status;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \void mengine_save_errno_no(void)
 * \保存引擎错误号
 * \param[in]    void  para1
 * \param[out]   none
 * \return       none
 * \ingroup      mengine_mesasge_done.c
 * \note
 */
/*******************************************************************************/
void mengine_save_errno_no(void)
{
    eg_err_e err_status;
    mmm_mp_status_t mmm_status;

    //读取错误号
    mmm_mp_cmd(mp_handle, MMM_MP_GET_ERRORNO, (unsigned int) &mmm_status.err_no);
    //转换错误号
    err_status = _convert_error_no(mmm_status.err_no);
    if (err_status != EG_ERR_NONE)
    {
        //保存错误状态
        mengine_info.eg_status.err_status = err_status;
    }
}

