/*******************************************************************************
 *                              US212A
 *                            Module: music ape cue
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     yanghong   2015-11-11          1.0              create this file
 *******************************************************************************/
#include "app_music.h"
#include "app_music_playing.h"

#ifndef  _CARD_BASE_
int ext_vm_read(uint8 *pbuf, unsigned int vm_offset, unsigned int len)
{
    sd_sec_param_t  temp_sd_sec_param_t;
    temp_sd_sec_param_t.file_offset = vm_offset;
    temp_sd_sec_param_t.sec_num = 1;
    temp_sd_sec_param_t.sram_addr = (uint32)pbuf;

    if(base_ext_vram_read(&temp_sd_sec_param_t, len) == -1)
    {
        return -1;
    }
    return 0;
}

int ext_vm_write(uint8 *pbuf, unsigned int vm_offset)
{
    sd_sec_param_t  temp_sd_sec_param_t;
    temp_sd_sec_param_t.file_offset = vm_offset;
    temp_sd_sec_param_t.sec_num = 1;//len;
    temp_sd_sec_param_t.sram_addr = (uint32)pbuf;
    if(base_ext_vram_write(&temp_sd_sec_param_t) == -1)
    {
        return -1;
    }
    return 0;
}

#endif
#ifdef SUPPORT_ApeCue_SWITCH
/******************************************************************************/
/*!
 * \par  Description:
 * \uint8 cue_shuffle_num(uint8 total, uint8 num)
 * \cue随机播放 产生随机种子
 * \param[in]    total cue歌曲总数
 * \param[out]   none
 * \return       void
 * \retval       
 * \retval       
 * \ingroup      ape cue
 * \note
 */
/*******************************************************************************/
uint8 cue_shuffle_num(uint8 total, uint8 num)
{
    uint8 i, a = 0;
    uint32 data;

    g_cue_shuffle_num |= (0x00000001 << num);
    //music_print("S=>:", g_cue_shuffle_num, 2);

    data = g_cue_shuffle_num;
    //获取g_cue_shuffle_num中1的个数
    for (i=0;data;++i)
    {
        data &= (data-0x00000001);
    }

    if(i >= total)
    {   
        g_cue_shuffle_num = 0;
        //0xff表示已经随机遍历完列表
        a = 0xff;
        return a;
    }
    
    while(1)
    {
        a = sys_random()%total;
        if ((g_cue_shuffle_num & (0x00000001 << a)) == 0)
        {
            return a;
        }
        
        //挂起10ms 多任务交互
        sys_os_time_dly(2);
    }

}
#endif
/******************************************************************************/
/*!
 * \par  Description:
 * \void music_load_cuefile(void)
 * \解析ape的cue文件,读出cue歌曲总数和歌曲title写入VM用于cue列表显示
 * \param[in]    play_set 播放场景模式
 * \param[out]   none
 * \return       void
 * \retval       
 * \retval       
 * \ingroup      ape cue
 * \note
 */
/*******************************************************************************/
void music_load_cuefile(void)
{
#ifdef SUPPORT_ApeCue_SWITCH

    handle cuefile_handle;
    fsel_type_e cur_type = g_file_path_info.file_source;

    g_status_deal_flag &= clr_ApeNoCue;
    g_cue_shuffle_num = 0;
    
    //设置文件路径
    fsel_set_location(&g_file_path_info.file_path.dirlocation, cur_type);

    //处理cue
    if(FALSE == cue_open(&cuefile_handle, (uint8*)&g_cue_file.total))
    {
        //提示没有cue文件标志
        g_status_deal_flag |= up_ApeNoCue;
        return;
    }
    
    cue_close(cuefile_handle);
#endif
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_playmode_apecue(void)
 * \单独处理播放cue列表歌曲的循环模式,随机模式,快进退切歌
 * \param[in]    void
 * \param[out]   bool
 * \return       true
 * \retval       
 * \retval       
 * \ingroup      ape cue
 * \note
 */
/*******************************************************************************/
app_result_e set_playmode_apecue(void)
{
    app_result_e result = RESULT_NULL;

#ifdef SUPPORT_ApeCue_SWITCH
    uint8 data, exit_flag = 0;
    uint8 playmode, shuffle_mode, fast_mode;
    bool need_play = FALSE;
    
    //当前音乐播放模式
    music_comval_t   *music_comval = &g_setting_comval.music_comval; 
    //当前音乐播放状态
    mengine_status_t *music_status = &g_music_status;
    //当前播放文件信息
    mengine_playinfo_t playinfo;
    
    //获取播放信息
    music_get_playinfo(&playinfo);

    //循环模式
    playmode = music_comval->music_repeat_mode;
    //随机模式
    shuffle_mode = music_comval->music_shuffle;
    //快进模式
    fast_mode = music_status->fast_status;
    
    switch(playmode)
    {
        //cue单曲循环
        case FSEL_MODE_LOOPONE:
        if (g_cur_time >= g_total_time)
        {
            //music_print("#CUE_LOOPONE",0,0);
            need_play = TRUE;
        }
        break;

        //cue循环所有
        case FSEL_MODE_LOOPALL:
        if (g_cur_time >= g_total_time)
        {
            //music_print("#CUE_LOOPALL",0,0);
            need_play = TRUE;
            g_cue_file.num ++;
            if (g_cue_file.num == g_cue_file.total)
            {
                g_cue_file.num = 0;
            }
        }
        break;

        //cue浏览播放
        case FSEL_MODE_INTRO:
        if (g_cur_time >= (g_cue_intro_start + 11*1000))//11s
        {
            //music_print("<MODE_INTRO>",0,0);
            g_cue_intro_start = 0;
            
            if (g_cue_file.num < (g_cue_file.total - 1))
            {
                //music_print("#CUE_INTRO",0,0);
                g_cue_file.num ++;
                need_play = TRUE;
            }
            else
            {
                //随机播歌完毕暂停播歌
                music_stop();
                g_cur_time = 0;
                result = RESULT_NULL;
                goto exit;
            }
        }
        break;

        //cue顺序播下一首
        default:
        if (g_cur_time >= g_total_time)
        {
            g_cue_file.num ++;
            if (g_cue_file.num < g_cue_file.total)
            {
                //music_print("#CUE_NORMAL",0,0);
                need_play = TRUE;
            }
            else//退出CUE列表播歌
            {
                exit_flag = 1;
            }
        }
        break;
    }

    //快退切上一曲处理
    if ((playinfo.cur_time <= g_cue_file.start_time) && (fast_mode == FBPlaySta))
    {
        g_cue_file.num --;
        if (g_cue_file.num < 0xff)
        {
            //music_print("#CUE_FBPlaySta",0,0);
            need_play = TRUE;
        }
        else//退出CUE列表播歌
        {
            music_cancel_ffb();
            exit_flag = 2;
        }
    }
    if (need_play == TRUE)
    {
        //快进时取消快进
        if (fast_mode == FFPlaySta)
        {
            music_cancel_ffb();
        }
        
        data = g_cue_file.num;

        //随机模式
        if (shuffle_mode == 0x10)
        {
            if (playmode == FSEL_MODE_LOOPALL || playmode == FSEL_MODE_NORMAL)
            {
                //随机模式开
                //music_print("#CUE_SHUFFLE",0,0);
                data = cue_shuffle_num(g_cue_file.total, g_cue_file.num);
                if(data == 0xff)
                {
                    exit_flag = 1;
                    goto exit;
                    //随机遍历完列表处理
                }
            }
        }
        handle_cue(data);
    }
exit:

    if(exit_flag != 0)
    {
        g_status_deal_flag &= (clr_ApeIsNow & clr_ApeInCue);
        set_scene_cuemode(0);//同步非cue列表播歌消息到后台
        if(exit_flag == 1)
        {
            //发送同步消息播放下一个文件
            music_send_msg(NULL, NULL, MSG_MENGINE_PLAY_NEXT_SYNC);
            //设置切换文件刷新标志位
            SetSwitchInterface();
        }
        if(exit_flag == 2)
        {
            g_display_bitmap |= ( up_musicplaystatus
                             |    up_musictimeinfo 
                             |    up_musicprogress  );//绘制状态
            g_status_deal_flag |= up_Paintflag;
            //发送同步消息播放上一个文件
            //music_send_msg(NULL, NULL, MSG_MENGINE_PLAY_PREV_SYNC);
            //设置切换文件刷新标志位
            //SetSwitchInterface()
        }
    }
#endif 
    return result;
}


