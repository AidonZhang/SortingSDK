/*******************************************************************************
 *                              US212A
 *                            Module: udiskap
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       yliang     2011-11-07      1.0             build this file
 *******************************************************************************/
/*!
 * \file     udisk_enter2exit.c
 * \brief    udisk init & exit deal
 * \author   yliang
 * \version  1.0
 * \date  2011/10/07
 *******************************************************************************/
#include  "ap_udisk.h"

/******************************************************************************/
/*!
 * \par  Description:
 *udisk ap init
 * \param[in]
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      main_udisk.c
 * \note
 *******************************************************************************/
void udisk_init_all(void)
{
    picbox_private_t picbox_data;
    textbox_private_t textbox_param;
    style_infor_t style_infor;
    //  setting_comval_t u_g_setting_vars;
    //bool  result=0;
    uint8 i = 0;

    /*显示连接界面*/
    style_infor.type = UI_AP;
    //style_infor.style_id = UDISK_LINK_BG;
    //ui_show_picbox(&style_infor, NULL);

    style_infor.style_id = UDISK_MP3;
    ui_show_picbox(&style_infor, NULL);

    style_infor.style_id = UDISK_USB_PIC;
    ui_show_picbox(&style_infor, NULL);

    style_infor.style_id = UDISK_LINK_TYPE;
    picbox_data.pic_id = -1;
    picbox_data.frame_id = 0;
    ui_show_picbox(&style_infor, &picbox_data);

    for (i = 0; i < 5; i++)
    {
        style_infor.style_id = style_usb_point[i];
        picbox_data.pic_id = -1;
        picbox_data.frame_id = 0;
        //显示界面
        ui_show_picbox(&style_infor, &picbox_data);
    }
    if (timer_usb_connect_id == -1)
    {
        timer_usb_connect_id = set_app_timer(APP_TIMER_ATTRB_UI, 1000, timer_usb_display_connect);
        time_count = 0;
    }
    style_infor.style_id = UDISK_STATE_STR;
    textbox_param.lang_id = UNICODEID;
    textbox_param.str_id = S_UDISK_LINKING;
    textbox_param.str_value = NULL;
    ui_show_textbox(&style_infor, &textbox_param, TEXTBOX_DRAW_NORMAL);

    style_infor.style_id = UDISK_USB_STR;
    textbox_param.str_id = S_UDISK_USB;
    ui_show_textbox(&style_infor, &textbox_param, TEXTBOX_DRAW_NORMAL);

    ui_show_battery();
    //保存需要动态刷新的图片到内存
    read_connect_pic_to_ram(); //2015-11-10
    read_battery_pic_to_ram();

}

//0:空闲  10h:上传  20h:下传  30h:弹出要求退出  40h:拔线要求退出
//60h:分区处理状态  70h:插入到USB充电器  80h:进入Suspend状态  90h:退出Suspend状态

app_result_e udisk_exit_all(int argc)
{
    app_result_e ret = 0;
    msg_apps_t msg = {0};

    /*读取VM信息*/
    sys_vm_read(&update_flag, UDISK_VM, sizeof(uint32));
    msg.type = MSG_CREAT_APP; //创建播放列表app
    //按键退出
    if (g_udisk_status.line_sta == 0x00)
    {

        if (returnkey_press == 1)
        {
            goto exit_0;
        }

    }
    else if (g_udisk_status.line_sta == 0x70)
    {
        goto exit_0;
    }
    else
    {
        //拔线,弹出
        goto exit_0;
    }

    exit_0:
    //创建playlist

#ifdef  _CARD_BASE_
    if ((update_flag & 0x00000001) != 0)
    {
        msg.content.data[1] = (uint8) PARAM_FROM_UDISK;
        msg.content.data[1] |= (uint8)PLIST_DISK_C;//playlist ap param
        

#else
    if (((update_flag & 0x00000001) != 0) || ((update_flag & 0x00010000) != 0))
    {
        msg.content.data[1] = (uint8) PARAM_FROM_UDISK;
        if ((update_flag & 0x00000001) != 0)
        {
            msg.content.data[1] |= (uint8) PLIST_DISK_C;//playlist ap param
            
        }
        else
        {
            msg.content.data[1] |= (uint8) PLIST_DISK_H;//playlist ap param
            
        }
#endif
        msg.content.data[0] = (uint8) APP_ID_PLAYLIST;//playlist ap
        change_card_update(FALSE);
        ret = RESULT_CREATE_LIST_APP;
        //card_rw_flag = 1;
    }
    //返回上一次的AP
    else
    {
        ret = RESULT_LASTPLAY;
        if ((argc & 0x00ff) == APP_ID_RADIO)
        {
            msg.content.data[0] = APP_ID_RADIO;
            //直接回radio播放.  传参为PARAM_BACKTO_PLAYING时不会重设频率
            if ((((uint32) argc >> 16) & 0x00ff) == ENGINE_STATE_PLAYING)
            {
                msg.content.data[1] = PARAM_FROM_CONFIG; //PARAM_BACKTO_PLAYING;
            }
            else
            {
                msg.content.data[1] = PARAM_FROM_CONFIG; //PARAM_BACKTO_PLAYED;
            }
        }
        else if ((argc & 0x00ff) == APP_ID_MUSIC)
        {
            msg.content.data[0] = APP_ID_MUSIC;
            if ((((uint32) argc >> 16) & 0x00ff) == ENGINE_STATE_PLAYING)
            {
                msg.content.data[1] = PARAM_BACKTO_PLAYING;
            }
            else
            {
                msg.content.data[1] = PARAM_BACKTO_PLAYED;
            }
        }
        else
        {
            msg.content.data[0] = APP_ID_MAINMENU;
            msg.content.data[1] = PARAM_FROM_UDISK;
        }
    }
    send_async_msg(APP_ID_MANAGER, &msg); //发送异步消息,创建播放列表app的进程

    return ret;
}

