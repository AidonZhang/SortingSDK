/*******************************************************************************
 *                              US212A
 *                            Module: music engine
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      fiona.yang  2012-05-30        1.0              create this file
 *******************************************************************************/

#include "app_mengine.h"
#include "mengine_rdata.h"        


const uint32 VolumeTbl[32 * 2] =
{
    //normal volume
    0xbf28,
    0xbb28,
    0xbd27,
    0xbe26,
    0xbd26,
    0xbe25,
    0xbe24,
    0xbe23,
    0xbe22,
    0xbe21,
    0xbe20,
    0xbe1f,
    0xbe1e,
    0xbd1d,
    0xbe1b,
    0xbe1a,
    0xba1a,
    0xba19,
    0xba18,
    0xb917,
    0xbd15,
    0xbc14,
    0xbb13,
    0xbc11,
    0xb910,
    0xb70e,
    0xb40c,
    0xae06,
    0x9c0c,
    0x8c0a,
    0x7605,
    0x00,
    
    //eq volume
    0xbe1f,
    0xbe1f,
    0xbe1f,
    0xbe1f,
    0xbe1f,
    0xbe1f,
    0xbe1f,
    0xbe1f,
    0xbe1f,
    0xbe1f,
    0xbe1f,
    0xbe1f,
    0xbe1e,
    0xbd1d,
    0xbe1b,
    0xbe1a,
    0xba1a,
    0xba19,
    0xba18,
    0xb917,
    0xbd15,
    0xbc14,
    0xbb13,
    0xbc11,
    0xb910,
    0xb70e,
    0xb40c,
    0xae06,
    0x9c0c,
    0x8c0a,
    0x7605,
    0x00,
};

const uint16 eq_cutoffs[MAX_GEQ_SEG] = {80,200,480,980,4000,8000,16000};
const uint16 eq_Q_values[MAX_GEQ_SEG] = {10,6,4,6,10,5,8};

//前7个参数对应7个频点，分别是 80Hz、200Hz、500Hz、1kHz、4kHz、8kHz、16kHz；
//参数为0,表示没有使用到,如果要改变为非0数据,需要打开EQ_PARAM_POINT_ADD
const int8 eq_param[EQ_PARAM_MAX][MAX_GEQ_SEG] =
{
    { 0, 0, 0,  0,  0,  0,  0 }, //none
    { 5, 3, 0, -1,  0,  4,  5 }, //rock
    { 3, 0, 0,  0,  0, -1, -2 }, //funk--pop
    { 0, 0, 1,  2,  0,  1,  1 }, //hiphop--soft
    { 3, 0, 0, -1,  0,  2,  4 }, //Jazz
    { 0, 0, 0, -1, -1, -2, -4 }, //Classic
    { 5, 2, 0,  0,  0,  0,  6 }, //techno--dbb
};


uint32 get_phy_volume_value(uint8 volume, uint8 eq_mode)
{
    uint8 eq_volume = FALSE;    
    uint8 tab_index;

    if(volume > 31)
    {
        return 0;
    }

   //判断EQ类型
    if (eq_mode == EQ_EQ_USR_MODE)
    {
        eq_volume = TRUE;
    }
    //取消上层设置PA VOLUMEL-R寄存器，全部采用软音量模式，有alsp中映射表配置
    if (eq_volume == FALSE)
    {
        //tab_index = (31 - volume);
		tab_index = (31 - volume)*2 + 0;
    }
    else
    {
       // tab_index = (31 - volume) + 32;
		tab_index = (31 - volume)*2 + 0 + 64;
    }

    //return VolumeTbl[tab_index];
    return (uint32)tab_index;
    
}
/******************************************************************************/
/*!
 * \par  Description:
 * \void mengine_set_real_volume(uint8 volume, uint8 eq_type)
 * \音量转换
 * \param[in]    volume  para1 输入音量值
 * \param[in]    eq_type  param2 eq类型
 * \param[out]   none
 * \return       uint32 the result
 * \retval
 * \retval
 * \ingroup      mengine_event_volume.c
 * \note
 */
/*******************************************************************************/
void mengine_set_real_volume(uint8 volume, uint8 eq_type)
{
    uint32 volume_val;

    volume_val = get_phy_volume_value(volume,eq_type);

    if(volume_val != 0)
    {
        mmm_mp_cmd(mp_handle, MMM_MP_SET_VOLUME, volume_val);
    }
    return;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_set_volume(void* msg_ptr)
 * \设置音量
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
 * \ingroup      mengine_event_volume.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_set_volume(void* msg_ptr)
{
    //设置音量内容的指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    //将消息指针指向的数据copy到引擎变量
    g_eg_cfg_p->volume = *(uint8*) (data_ptr->msg.content.addr);

    mengine_set_real_volume(g_eg_cfg_p->volume, (uint8) g_eg_cfg_p->eq_info.eq_mode);

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void mengine_set_real_eq(uint8 mode)
 * \向解码中间件发送设置EQ命令
 * \param[in]    mode TRUE 正常切歌设置EQ FALSE 通过菜单设置EQ
 * \param[out]   none
 * \return       none
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
void mengine_set_real_eq(uint8 mode)
{
    uint8 i;
    uint8 eq_type;
    uint8 eq_mode;
    uint32 volume_val;    
    uint8 cur_volume;
    dae_config_t *p_dae_cfg = &g_dae_cfg;
    
    eq_mode = mengine_info.eg_config.eq_info.eq_mode;
    eq_type = mengine_info.eg_config.eq_info.eq_type;
    g_setting_comval.g_comval.eq_type = eq_type;

    cur_volume = get_current_volume();
        
    if(g_setting_comval.g_comval.eq_enable == 0)
    {
        goto _set_volume_;
    }

    if(p_dae_cfg->mdrc_enable == TRUE)
    {
        goto _set_volume_;
    }
    
    p_dae_cfg->peq_enable = TRUE;
    if(eq_type == 0)
    {
        p_dae_cfg->limiter_enable = FALSE;
    #ifdef EQ_PARAM_POINT_ADD
        //提频恢复
        adjust_freq_add_value(0, 0);
    #endif
    }
    else
    {
        p_dae_cfg->limiter_enable = TRUE;

        //如果要改变eq_param中的0值为非0值,需要打开
    //#ifdef EQ_PARAM_POINT_ADD

    //开EQ时由于DSP的频率低会出现杂音，所以在开EQ时需要提高DSP的频率
        if (p_dae_cfg->run_freq < FREQ_LEVEL11)
        {
            //提频

            adjust_freq_add_value(0, 15);
        }
   // #endif
    }
        
    for (i = 0; i < MAX_GEQ_SEG; i++)
    {
        p_dae_cfg->peq_bands[i].cutoff = eq_cutoffs[i];
        p_dae_cfg->peq_bands[i].q = eq_Q_values[i];
        p_dae_cfg->peq_bands[i].gain = 10 * eq_param[eq_type][i];
        p_dae_cfg->peq_bands[i].type = 1;
    }

    for(; i < MAX_EQ_SEG; i++)
    {
        p_dae_cfg->peq_bands[i].cutoff = 0;
        p_dae_cfg->peq_bands[i].q = 0;
        p_dae_cfg->peq_bands[i].gain = 0;
        p_dae_cfg->peq_bands[i].type = 0;      
    }

    volume_val = get_phy_volume_value(cur_volume,eq_mode);
    p_dae_cfg->volume_value = volume_val;
        
    //更新DAE配置参数到共享内存
    libc_memcpy(g_dae_cfg_shm, p_dae_cfg, sizeof(dae_config_t));
    g_dae_cfg_shm = (dae_config_t *) sys_shm_mount(SHARE_MEM_ID_DAECFG);
    if (g_dae_cfg_shm == NULL)
    {
        PRINT_ERR("dae_cfg shm not exist!!");
        while (1)
        {
            ; //nothing for QAC
        }
    }

    //设置当前eq到解码器
    mmm_mp_cmd(mp_handle, MMM_MP_SET_EQ, (uint32)NULL);
	

_set_volume_:

    if(mode == TRUE)
    {
        mengine_set_real_volume(cur_volume, eq_type);
    }
    //开EQ时设置音量需要传入不同参数
	if (0 != g_eg_cfg_p->eq_info.eq_mode)
		{
			
			mengine_set_real_volume(get_current_volume(), EQ_EQ_USR_MODE);
	
		}
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_set_eq(void* msg_ptr)
 * \设置eq
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
app_result_e mengine_set_eq(void* msg_ptr)
{
    //设置eq数据内容的指针
    private_msg_t * data_ptr = (private_msg_t*) msg_ptr;
    
 //   eq_param_t *eq_info_p = data_ptr->msg.content.addr;

    //将消息指针指向的数据copy到引擎变量
    libc_memcpy(&(g_eg_cfg_p->eq_info), (data_ptr->msg.content.addr), sizeof(eq_param_t));

    //设置当前的EQ
    mengine_set_real_eq(0);
	
	
	
    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_IGNORE;
}


/******************************************************************************/
/*!
 * \par  Description:
 * \断点续播处理
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_standby_play(void* msg_ptr)
{
    bool ret_vals = TRUE;
    
    if (mengine_enter_mode == ENTER_RESUME)
    {
        //开启音频输入    
        com_set_sound_out(TRUE, SOUND_OUT_START, NULL);
        
        //恢复音乐播放,播放
        ret_vals = _play(PLAY_RESUME);
    }
    else if(mengine_enter_mode == ENTER_RESUME_PAUSE)
    {
        //开启音频输入
        com_set_sound_out(TRUE, SOUND_OUT_START, NULL);
        
        //关闭音频输出
        com_set_sound_out(FALSE, SOUND_OUT_PAUSE, NULL);
    }
    else
    {
        ;//nothing for QAC
    }

    //返回成功
    mengine_reply_msg(msg_ptr, ret_vals); 

    return RESULT_IGNORE;

}


