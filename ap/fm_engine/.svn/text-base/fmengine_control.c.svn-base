/*******************************************************************************
 *                              US211A
 *                            Module: radio engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>        <time>           <version >            <desc>
 *     mikeyang     2011-09-06 15:00     1.0             build this file
 *******************************************************************************/
#include "app_fmengine.h"

const uint32 VolumeTbl[32] =
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
    0x00
};

void fm_eg_reply_msg(private_msg_t *private_msg)
{
    libc_sem_post(private_msg->sem);
}


/******************************************************************************/
/*!
 * \par  Description:
 * \uint32 volume_deal(uint8 vol_in)
 * \音量转换
 * \param[in]    uint8 vol_in
 * \param[out]   none
 * \return       uint32 vol_out
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void volume_deal(uint8 vol_in)
{
    uint32 Volume, volum_pa, volum_da;
    uint8 tab_index;

    tab_index = (31 - vol_in);
    Volume = VolumeTbl[tab_index];

 //   libc_print("fm volume",Volume,2);
    volum_pa = (Volume & 0x3f);
    volum_da = ((Volume >> 8) & 0xff);
    set_pa_volume(volum_pa,volum_da);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_message_deal(private_msg_t* cur_msg)
 * \引擎消息处理,处理引擎接收到的私有消息和系统消息
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略
 * \retval       RESULT_APP_QUIT 退出app
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_message_deal(private_msg_t* cur_msg)
{
    app_result_e ret_value = RESULT_NULL;

    uint32 temp = 0;
    uint32 display_vol = 0;
    uint32 set_vol = 0;

    temp = temp;
    display_vol = display_vol;
    set_vol = set_vol;

    act_writel(act_readl(MFP_CTL0)&(~(1 << 17)),MFP_CTL0);

    switch (cur_msg->msg.type)
    {
        /* 模组初始化及卸载相关*/

        /* 同步初始化FM  模组*/
        case MSG_FMENGINE_INIT:
        ret_value = fm_module_init(cur_msg);
        break;

        case MSG_FMENGINE_RESINTALL:
        ret_value = fm_module_reinstall(cur_msg);
        break;

        /* FM 模组进入Standby */
        case MSG_FMENGINE_STANDBY:
        ret_value = fm_module_standby();
        break;

        /* 要求引擎退出，返回后卸载驱动*/
        case MSG_FMENGINE_CLOSE:
        case MSG_APP_QUIT: //退出app
        /*其他进程也可向FM 引擎发MSG_FMENGINE_CLOSE  消息要求FM 引擎退出*/
        if (g_module_status != MODULE_STANDBY)
        {
            fm_module_standby();
        }
        ret_value = RESULT_APP_QUIT;
        break;

        /* 参数设置相关*/

        /* 同步设置频点播放*/
        case MSG_FMENGINE_SETFREQ_SYNC:
        ret_value = fmengine_set_freq_sync(cur_msg);
        break;

        /* 静音或解除静音*/
        case MSG_FMENGINE_SETMUTE:
        ret_value = fmengine_mute_sync(cur_msg);
        break;

        /* 设置电台波段*/
        case MSG_FMENGINE_SETBAND:
        ret_value = fmengine_set_band_sync(cur_msg);
        break;

        /* 设置搜台门限*/
        case MSG_FMENGINE_SETTHRED:
        ret_value = fmengine_set_throd(cur_msg);
        break;

        /* 启动软件搜台*/
        case MSG_FMENGINE_SET_SOFTSEEK:
        ret_value = fmengine_set_softseek(cur_msg);
        break;

        /* 启动硬件搜台*/
        case MSG_FMENGINE_SET_HARDSEEK:
        ret_value = fmengine_set_hardseek(cur_msg);
        break;

        /* 退出硬件seek 过程*/
        case MSG_FMENGINE_SET_SEEKBREAK:
        ret_value = fmengine_seek_break(cur_msg);
        break;

        /* 设置音量*/
        case MSG_FMENGINE_SET_VOLUME:

        display_vol = (uint32)(*(uint8*) (cur_msg->msg.content.addr));

        volume_deal((uint8)display_vol);

        cur_msg->reply->type = MSG_REPLY_SUCCESS;
        //回复同步消息(发送信号量)
        reply_sync_msg(cur_msg);
        ret_value = RESULT_NULL;
        break;

        /* 获取信息相关*/

        /* 获取硬件自动搜台是否完成标识*/
        case MSG_FMENGINE_AUTO_SEEK_OVER:
        ret_value = fmengine_get_seekflag(cur_msg);
        break;

        /* 获取当前状态信息*/
        case MSG_FMENGINE_GET_STATUS:
        ret_value = fmengine_get_status(cur_msg);
        break;

        /* 获取当前波段信息*/
        case MSG_FMENGINE_GET_BAND:
        ret_value = fmengine_get_band(cur_msg);
        break;

        /* 获取当前频率信息*/
        case MSG_FMENGINE_GET_FREQ:
        ret_value = fmengine_get_freq(cur_msg);
        break;

        /* 获取当前电台信号强度信息*/
        case MSG_FMENGINE_GET_INTENSITY:
        ret_value = fmengine_get_intensity(cur_msg);
        break;

        /* 获取天线状态( 耳机是否插入) */
        case MSG_FMENGINE_GET_ANTENNA:
        ret_value = fmengine_get_antenna(cur_msg);
        break;

        /* 获取立体声状态信息*/
        case MSG_FMENGINE_GET_STEREO:
        ret_value = fmengine_get_stereo(cur_msg);
        break;

        /* 获取当前引擎状态*/
        case MSG_FMENGINE_GET_PLAYSTATUS:
        ret_value = fmengine_get_playflag(cur_msg);
        break;

        /* 获取当前音量*/
        case MSG_FMENGINE_GET_VOLUME:
        ret_value = fmengine_get_volume(cur_msg);
        break;

        /* 获取当前模组状态*/
        case MSG_FMENGINE_GET_MODUAL_STATUS:
        ret_value = fmengine_get_modual_status(cur_msg);
        break;

        default:
        break;
    }

    return ret_value;
}

/******************************************************************************/
/*!
 * \Description: FM 引擎应用为被动模块，也是其他应用进程
 * \ 和FM 驱动通讯的接口模块
 * \app_result_e fmengine_control_block(void)
 * \引擎功能处理函数
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_control_block(void)
{
    app_result_e result = RESULT_NULL;
    private_msg_t fmengine_msg;

    while (1)
    {
        //libc_print("loop_receive_message",0,0); //2015-12-15
        //获取并处理消息
        if (get_app_msg_for_engine(&fmengine_msg) == TRUE)
        {
            result = fmengine_message_deal(&fmengine_msg);
            if (result == RESULT_APP_QUIT)
            {
                break;
            }
        }
        //挂起10ms 多任务交互
        sys_os_time_dly(1);
    }
    return result;
}
