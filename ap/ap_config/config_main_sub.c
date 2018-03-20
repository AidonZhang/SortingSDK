/*******************************************************************************
 *                              US212A
 *                            Module: Config
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     config_main_sub.c
 * \brief    ���ػ��Ĵ���
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "ap_config.h"

//configӦ�ñ���
extern g_config_var_t g_config_var;
//�Ƿ��һ���ϵ��־
extern bool first_boot;
//u�����ݸ��±�־
extern uint32 update_flag;

#ifdef _CARD_BASE_
static const uint8 nand_drv_name[] = "nor_ud.drv";
#else
static const uint8 nand_drv_name[] = "nand.drv";
#endif
static const uint8 card_drv_name[] = "card.drv";
static const uint8 uhost_drv_name[] = "uhost.drv";

/******************************************************************************/
/*!
 * \par  Description:
 *  װ��������ʺ��ļ�ϵͳ
 * \param[in]    disk_type ��������
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
int32 _config_fs_init(uint8 disk_type)
{
    uint8 drv_type;
    uint8 *drv_name;
    int32 file_sys_id;

    if (disk_type == DISK_C)
    {
        drv_type = DRV_GROUP_STG_BASE;
        drv_name = (uint8 *) nand_drv_name;
    }
    else if (disk_type == DISK_H)
    {
        drv_type = DRV_GROUP_STG_CARD;
        drv_name = (uint8 *) card_drv_name;
    }
    else if (disk_type == DISK_U)
    {
        drv_type = DRV_GROUP_STG_UHOST;
        drv_name = (uint8 *) uhost_drv_name;
    }
    else
    {
        return -1;
    }

    //װ���������
    if (sys_drv_install(drv_type, 0, drv_name) != 0)
    {
        return -2;
    }

    //װ���ļ�ϵͳ
    file_sys_id = sys_mount_fs(drv_type, disk_type, 0);

    if (file_sys_id == -1)
    {
         //��ʾ�����Ժ�
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_IMMEDIATE, S_COMMON_FORMAT_DEVICE);
         
        //δ�����ļ�ϵͳ����ʽ������
        sys_format_disk(DRV_GROUP_STG_BASE, DISK_C, FORMAT_FAT32);

        //װ���ļ�ϵͳ
        file_sys_id = sys_mount_fs(drv_type, disk_type, 0);

        if (file_sys_id == -1)
        {
            return -3;
        }
    }

    return file_sys_id;
}
/******************************************************************************/
/*!
 * \par  Description:
 *  ж��������ʺ��ļ�ϵͳ
 * \param[in]    disk_type ��������
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
bool _config_fs_deinit(uint8 disk_type, int32 file_sys_id)
{
    int32 ret_val;
    uint8 drv_type;

    if (file_sys_id != -1)
    {
        ret_val = sys_unmount_fs(file_sys_id);
        file_sys_id = -1;
    }
    else
    {
        return TRUE;
    }

    if (0 > ret_val)
    {
        return FALSE;
    }

    if (disk_type == DISK_C)
    {
        drv_type = DRV_GROUP_STG_BASE;
    }
    else if (disk_type == DISK_H)
    {
        drv_type = DRV_GROUP_STG_CARD;
    }
    else if (disk_type == DISK_U)
    {
        drv_type = DRV_GROUP_STG_UHOST;
    }
    else
    {
        return FALSE;
    }

    ret_val = sys_drv_uninstall(drv_type);

    if (0 > ret_val)
    {
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * ����Ӧ��
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
bool _config_format_disk(void)
{
    bool ret_val;
    int32 file_sys_id;
    uint8 volume_label[28];

    libc_memset(volume_label, 0, sizeof(volume_label));

    //װ������
    file_sys_id = _config_fs_init(DISK_C);
    if (file_sys_id < 0)
    {
        return FALSE;
    }

    if(0 == vfs_create_volume(file_sys_id, 0))//�жϾ���Ƿ����
    {
        //���������󣬾�겻����,д����������
        //���������ȡ�������,���֧��11��Ӣ���ַ�
        com_get_config_struct(INF_UDISK_LABEL, volume_label, 11); //CMD_Inquiry_information_FOB
    
        //����ת���ɳ�����ʽ
        com_ansi_to_unicode(volume_label);
    
        //ͳһ�ɳ������������
        ret_val = vfs_create_volume(file_sys_id, volume_label);
    }
    _config_fs_deinit(DISK_C, file_sys_id);

    return ret_val;
}

/******************************************************************************/
/*!
 * \par  Description:
 * ����Ӧ��
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
void _creat_application(uint8 param)
{
    bool mainmenu_flag = TRUE;
    msg_apps_t config_msg;
    app_result_e result = RESULT_NULL;

    ui_res_open("common.sty", UI_COM);
    
    if (param == PARAM_ENTER_ALARM)
    {
        result = com_alarm_msg_dispatch();
        if (result == RESULT_ALARMFM)
        {
            config_msg.content.data[0] = APP_ID_RADIO;
        }
        else if (result == RESULT_FMRECORD)
        {
            config_msg.content.data[0] = APP_ID_RECORD;
        }
        else if (result == RESULT_ALARM)
        {
            config_msg.content.data[0] = APP_ID_ALARM;
        }
        else
        {
            ;//do nothing
        }
        config_msg.type = MSG_CREAT_APP; //��process manager���ʹ�������������Ϣ
        config_msg.content.data[1] = (uint8) PARAM_FROM_ALARM;
        send_async_msg(APP_ID_MANAGER, &config_msg);
        return;
    }

    //�ж��Ƿ���Ҫ���²����б�
#ifdef  _CARD_BASE_	
    if ((update_flag & 0x00000001) != 0)
#else
    if (((update_flag & 0x00000001) != 0) || ((update_flag & 0x00010000) != 0))
#endif
    {
        config_msg.type = MSG_CREAT_APP;
        config_msg.content.data[0] = APP_ID_PLAYLIST;//playlist ap
        config_msg.content.data[1] = PARAM_FROM_CONFIG;
#ifdef  _CARD_BASE_	        
        if ((update_flag & 0x00000001) != 0)
        {
            
            config_msg.content.data[1] |= (uint8) PLIST_DISK_C;//playlist ap param
        }
#else
        if ((update_flag & 0x00000001) != 0)
        {
            
            config_msg.content.data[1] |= (uint8) PLIST_DISK_C;//playlist ap param
        }
        if ((update_flag & 0x00010000) != 0)
        {
          
            config_msg.content.data[1] |= PLIST_DISK_H;//playlist ap param           
        }
#endif
        
    }
    else
    {
        //��һ�ο��������������б�
        if (first_boot == TRUE)
        {
            config_msg.type = MSG_CREAT_APP;
            config_msg.content.data[0] = APP_ID_PLAYLIST;
            config_msg.content.data[1] = PARAM_FROM_CONFIG | PLIST_DISK_ALL;
        }
        else
        {
            //���ݹػ�ǰ��״̬����Ӧ�ã�Ŀǰ�����ǹػ�ǰ�к�̨���������ú�̨��Ӧ��ǰ̨Ӧ�ã�����޺�̨������������
            config_msg.type = MSG_CREAT_APP;
            config_msg.content.data[0] = g_config_var.ap_id;
            if (g_config_var.ap_id != APP_ID_MAINMENU)
            {
                if (g_config_var.ap_id == APP_ID_RADIO)
                {
                    //radio ����, ֱ�Ӵ��β���
                    config_msg.content.data[1] = PARAM_FROM_CONFIG;
                    mainmenu_flag = FALSE;
                }
                else if (g_config_var.ap_id == APP_ID_MUSIC && 
                         g_config_var.engine_state != ENGINE_STATE_NULL)
                {
                    dialog_com_data_t dialog_com;
                    app_result_e result;
                    string_desc_t string_desc;
                    
                    string_desc.data.id = S_LAST_PLAY;
                    string_desc.language = UNICODEID;
                    dialog_com.dialog_type = DIALOG_BUTTON_INCLUDE_WAIT;
                    dialog_com.button_type = BUTTON_YESNO;
                    dialog_com.icon_id = V_U16_INVALID;
                    dialog_com.string_desc = &string_desc;
                    dialog_com.active_default = 1;

                    //clear screen
                    ui_set_backgd_color(Color_BACKCOLOR_def);
                    ui_clear_screen(NULL); 
                    
                    //music��������ڵ���һ�β��ŶԻ���
                    result = gui_dialog(DIALOG_ASK, &dialog_com);
                    if(result == RESULT_DIALOG_YES)
                    {
                        if (g_config_var.engine_state == ENGINE_STATE_PLAYING)
                        {
                            config_msg.content.data[1] = PARAM_BACKTO_PLAYING;
                        }
                        else //if(g_config_var.engine_state == ENGINE_STATE_PAUSE)
                        {
                            config_msg.content.data[1] = PARAM_BACKTO_PLAYED;
                        }
                        mainmenu_flag = FALSE;
                    }
                    else
                    {
                        //ѡ����δ�������ֶϵ��ٴ� �������õ���"��һ�β���"
                        g_config_var.engine_state = ENGINE_STATE_NULL;
                        sys_vm_write(&g_config_var, VM_AP_CONFIG, sizeof(g_config_var_t));
                    }
                }
                else
                {
                    ;//for QAC
                }
                
                if (mainmenu_flag == TRUE)
                {
                    //���򴴽�mainmenu.ap
                    config_msg.content.data[0] = APP_ID_MAINMENU;
                    config_msg.content.data[1] = PARAM_FROM_CONFIG;
                }
            }
        }
    }
    
    ui_res_close(UI_COM);    
    send_async_msg(APP_ID_MANAGER, &config_msg);
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 *  ��ʾ���ػ�����
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
void draw_logo(uint8 param)
{
    animation_com_data_t temp_animation;
    style_infor_t temp_sty;

    temp_animation.direction = 0;
    temp_animation.interval = 10;

    ui_set_backgd_color(Color_BACKCOLOR_def);
    ui_clear_screen(NULL);  //2015-10-30

    if (param == SWITCH_ON)
    {
        temp_sty.style_id = STY_LOGO;

        ui_res_open("logo.sty", UI_AP);
    }
    else
    {
        temp_sty.style_id = STY_END;
        ui_res_open("end.sty", UI_AP);
    }
    temp_sty.type = UI_AP;
    gui_logo(&temp_sty, &temp_animation);

    ui_res_close(UI_AP);
}

/******************************************************************************/
/*!
 * \par  Description:
 *  �ػ�����
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
void switch_off(void)
{
    //�ػ�����
    draw_logo(SWITCH_OFF);
    com_app_sleep(2, NULL);
    //�ر���
    lcd_backlight_on_off(FALSE);
    lcd_standby_screen(FALSE);
    
    act_writel(act_readl(WAKE_PD), WAKE_PD);
        
    act_writel((act_readl(ONOFF_KEY) | 0x100) & (~(0x00000005 << ONOFF_KEY_ONOFF_PRESS_TIME_SHIFT)), 
    ONOFF_KEY); 
    //�ϵ�
    act_writel(act_readl(POWER_CTL) & (~(0x00000001 << POWER_CTL_EN_S1)), 
    POWER_CTL); 
    act_writel(act_readl(WD_CTL) | 0x08, WD_CTL);
    while (1)
    {
        //���Ӽ��USB����ֹ��ʱ����������ѭ����ϵͳû����������
        if ((act_readb(CHG_DET) & 0x80) != 0) 
        {
            lcd_backlight_on_off(TRUE);
            lcd_standby_screen(TRUE);
            break;    
        }
    }
}
