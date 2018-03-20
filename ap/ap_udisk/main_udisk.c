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
 * \file     main_udisk.c
 * \brief    main for ap_udisk
 * \author   yliang
 * \version  1.0
 * \date  2011/10/07
 *******************************************************************************/
#include  "ap_udisk.h"

res_size_t res_connect_size[10]; //��Դ��Ϣ����
uint32 update_flag = 0;     //bit0Ϊ1��ʾFlash�̱�д����bit16Ϊ1��ʾ���̱�д��
uint8 returnkey_press = 0;  //�յ����ذ�����־
uint8 iscardout = 0;    //���γ���־
uint8 iscardin = 0;
//uint8 usbstatus = 0;        //USB��ǰ����״̬
ureader_status_t g_udisk_status;
uint32 line_status_bak;    //�ݴ�USB����״̬����������ǰ��״̬�Ƿ�ı�
battery_status_e charge_state;     //��ǰ���״̬���ӳ��ʹ��״̬ȡ��
battery_status_e charge_state_bak; //�����ݴ���״̬����������ǰ��״̬�Ƿ�ı�
uint8 charge_value = 0;
//��ʱ��id
int8 timer_usb_connect_id;  //USBæͼ����ʾ��ʱ��ID
int8 timer_usb_state_check_id;  //usb����״̬��ȡ��־
int8 timer_usb_battery_id;  //��س����ʾ��ʱ��ID
uint8 time_count;   //ʱ�������
setting_comval_t g_setting_vars;

/*main��Ҫ���������ֵ����ϵͳ��Ա����*/
OS_STK *ptos = (OS_STK *) AP_UDISK_STK_POS;
INT8U prio = AP_UDISK_PRIO;



//p_adfu_launcher
p_func_entry p_adfu_launcher;

/*!
 * \brief
 *  udisk_key_map_list��udiskӦ�ð���ӳ����
 */
const key_map_t udisk_key_map_list[] =
{
    {{ KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_UDISK_EXIT_LAST },
    {{ KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};
const uint16 style_usb_point[5] =
{ UDISK_POINT_5, UDISK_POINT_4, UDISK_POINT_3, UDISK_POINT_2, UDISK_POINT_1 };
/*ȫ�ֱ�������*/
//Ӧ���Լ���Ҫ����ʱ������
#define APP_TIMER_COUNT     2
/*!
 *  \brief
 *  app_timer_vector��Ӧ����ʱ������ָ�룬ָ��Ӧ�ÿռ����ʱ������
 */
app_timer_t usb_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];
void msgmanager(void);
app_result_e usb_main_control(int argc);

/******************************************************************************/
/*!
 * \par  Description:
 *    message mananger
 * \param[in]
 * \param[out]   none
 * \retval       none
 * \ingroup      main_udisk.c
 * \note
 *******************************************************************************/
void msgmanager(void)
{
    bool msg_result;
    input_gui_msg_t udisk_gui_msg;
    msg_apps_type_e gui_event;
    private_msg_t udisk_app_msg;

    if (returnkey_press == 1) //���յ�return��,��������Ϣ,�Է��ǿ���״̬���ܸú�����KeyUp����
    {
        return;
    }
    /*����gui��Ϣ*/
    msg_result = get_gui_msg(&udisk_gui_msg);
    if (msg_result == TRUE)
    {
        if (com_key_mapping(&udisk_gui_msg, &gui_event, udisk_key_map_list) == TRUE)
        {
            if(gui_event == EVENT_UDISK_EXIT_LAST)
            {

                if ( g_udisk_status.line_sta == 0)
                {
                    returnkey_press = 0x01;
                    return;
                }
            }

            /*else if((gui_event == EVENT_ENTER_KEYLOCK) || (gui_event == EVENT_ENTER_KEYLOCK_UN))
            {
                //ֱ�Ӱ���Чִ�д��븴�ƹ��������ⷢ��bank�л�
                if(get_keylock_state() == TRUE)
                {
                    change_keylock_state(FALSE);
                }
                else
                {
                    change_keylock_state(TRUE);
                }
            }*/
            else
            {
            }
        }
    }
    msg_result = get_app_msg(&udisk_app_msg);
    if (msg_result == TRUE)
    {
        if(udisk_app_msg.msg.type == MSG_SD_OUT)
        {
            iscardout = 0x01;
            iscardin = 0;
        }
        else if(udisk_app_msg.msg.type == MSG_SD_IN)
        {
            iscardout = 0x0;
            iscardin = 1;
        }
        else if(udisk_app_msg.msg.type == MSG_EARPHONE_IN)//���������ߣ�����
        {
            change_antenna_state(TRUE);
        }
        else if(udisk_app_msg.msg.type == MSG_EARPHONE_OUT)//���������ߣ��γ�
        {
            change_antenna_state(FALSE);
        }
        else
        {
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * ����vdd and ��Ƶ forusb
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      udisk_main.c
 * \note
 *******************************************************************************/
void rise_vdd_and_freq(void)
{
    adjust_freq_set_level(AP_FRONT_LOW_PRIO, FREQ_LEVEL11, FREQ_NULL);//max frequency
    //sys_enter_high_powered(3);
    sys_enter_high_powered(1);
}

/******************************************************************************/
/*!
 * \par  Description:
 * �ָ�vdd and freq
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      udisk_main.c
 * \note
 *******************************************************************************/
void back_vdd_and_freq(void)
{
    sys_exit_high_powered();
    adjust_freq_set_level(AP_FRONT_LOW_PRIO, FREQ_LEVEL7, FREQ_NULL);
}


/******************************************************************************/
/*!
 * \par  Description:
 *  ����Ƿ���usb���룬ȷ���Ƿ����U��Ӧ��
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      music
 * \note
 *******************************************************************************/
uint8 detect_usb_line_status(void)
{
    //uint32 temp_save;
    //private_msg_t pri_msg;
    unsigned char plug_in = 0;
    if (get_usb_port_state() == USB_PORT_HOST)
    {
        //��⵽uhost����
        if (sys_detect_disk(DRV_GROUP_STG_UHOST) == 0)
        {
            return FALSE;
        }

        key_peripheral_detect_handle(PER_DETECT_USB_FORCE);

        sys_os_time_dly(30);
    }

    //get usbcable plug status
    plug_in = (act_readb(DPDMCTRL) & (1 << DPDMCTRL_Plugin));

    if (plug_in != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}




/******************************************************************************/
/*!
 * \par  Description:
 *  �л���adfu�Ĵ���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      usb
 * \note
 *******************************************************************************/
void switch_to_adfu(void)
{

    p_adfu_launcher = *(uint32*) 0xbfc002a0;

    //select hosc
    adjust_freq_set_level(AP_FRONT_LOW_PRIO, FREQ_LEVEL7, FREQ_NULL);

    //rise vcc 3.3v and vdd 1.3v, improve usb signal.xiaomaky.2012-9-11 11:55.
    act_writel(((act_readl(VOUT_CTL) & 0xFFFFFF80) | 0x6A), VOUT_CTL);
    sys_mdelay(3);

    act_writel(0, INTC_CFG0);
    act_writel(0, INTC_CFG1);
    act_writel(0, INTC_CFG2); //assgin usb intrrupt to IP2

    //act_writel(0x58, WD_CTL);
    act_writel(0x4D, WD_CTL);

    p_adfu_launcher();

}

/******************************************************************************/
/*!
 * \par  Description:
 *    MainMenu Ӧ�õ���ں����ͳ���������ģ��
 * \param[in]    0, �޲���ģʽ��1�����ڲ���ģʽ��2����һ�β���ģʽ
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
int main(int argc, const char *argv[])
{
    app_result_e result;
    uint8 real_charge_state;

    //��ʼ��applib�⣬ǰ̨AP
    applib_init(APP_ID_UDISK, APP_TYPE_GUI);

    //��ʼ����ʱ��
    init_app_timers(usb_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    //��ʼ�� applib ��Ϣģ��
    applib_message_init(NULL);

    sys_timer_init();

    sys_vm_read(&g_setting_vars, VM_AP_SETTING, sizeof(setting_comval_t));

    com_set_sys_comval(&g_setting_vars.g_comval);

    ui_set_backgd_color(Color_BACKCOLOR_def);//��Ϊȫ���� //2015-10-30
    ui_clear_screen(NULL); //2015-10-30

    rise_vdd_and_freq();

    /*��ȡui��Դ�ļ�*/
    ui_res_open("udisk.sty", UI_AP); //2015-11-03
    ui_res_open("common.sty", UI_COM); //2015-11-03

    //card_exist_flag = 1;

    key_chargeset(CHARGE_START, 0, 0);
    charge_state = (battery_status_e) (uint32) key_chargeget(&real_charge_state);

    usb_print("enter_usb!", 0, 0);

    if (detect_usb_line_status() != FALSE)
    {
        config_disk_label();

        sys_drv_install(DRV_GROUP_UD, 0, "udisk.drv");

        //some config set
        set_usb_info();

        //disable detect when init
        DISABLE_USB_DETECT();
        //start init
        ud_module_start(NULL);
        //enable detect after init
        ENABLE_USB_DETECT();

        result = usb_main_control(argc);

        //disable detect when exit (need init)
        DISABLE_USB_DETECT();
        //stop and exit
        ud_module_stop();
        //enable detect after init
        ENABLE_USB_DETECT();

        sys_drv_uninstall(DRV_GROUP_UD);

    }
    else
    {   
        //Add by yanghong
        result = udisk_exit_all(argc);
    }

    //switch to adfu and upgrade
    if (result == RESULT_AUTO_UPGRADE)
    {
        switch_to_adfu();
    }

    if (result == RESULT_ENTER_STUB_DEBUG)
    {
        usb_print("open stub", 0, 0);

        //        result = com_sys_install_stub();
    }

#ifdef SUPPORT_LCD_RAM0
    ui_set_display_buf(LCD_RAM1_INDEX);
#endif

    ui_res_close(UI_AP);
    ui_res_close(UI_COM);

    sys_timer_exit();

    back_vdd_and_freq();

    applib_quit();

    return result;
}

void udisk_timer_exit(void)
{
    if (timer_usb_connect_id != -1)
    {
        kill_app_timer(timer_usb_connect_id);
    }
    if (timer_usb_battery_id != -1)
    {
        kill_app_timer(timer_usb_battery_id);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    MainMenu ��ȡӦ��VMȫ�ֱ���,��ʼ��ȫ�ֱ���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
app_result_e usb_main_control(int argc)
{
    ureader_status_t udisk_status;
    app_result_e result = RESULT_NULL;
    uint8 count_num = 0;
    uint16 wait_time_before_enum = 0;
    timer_usb_connect_id = -1;
    timer_usb_battery_id = -1;
    timer_usb_state_check_id = -1;
    time_count = 0;
    line_status_bak = -1;
    charge_state_bak = BAT_NORMAL;
    
    //����usb��ʼ������
    udisk_init_all(); //2015-11-10
    for (count_num = 0; count_num < 0x7f; count_num++)
    {
        ; //delay
    }

    //����usb�Զ�������������ud_module_start֮��
#ifdef SUPPORT_USER_DEF_USBCMD
    ud_set_usrcmd(0x00, ACT_SET_TIME, set_time_date);
    //ud_set_usrcmd(0x03, ACT_SET_TIME, set_time_date);
#endif

    //udisk ap��������
    while (1)
    {
        msgmanager(); //��Ϣ����

        if (iscardout == 1)
        {
            ud_set_cmd(SET_CARD_INOUT, 0);
            iscardout = 0;
        }

        if (iscardin == 1)
        {

            ud_set_cmd(SET_CARD_INOUT, 1);
            iscardin = 0;
        }

        //       ud_get_status(&g_udisk_status);

#ifndef _CARD_BASE_
        if ((wait_time_before_enum > 400) && (g_udisk_status.suspend_flag == 0) && (g_udisk_status.reset_count == 0))
        {
            break; //������ǳ����,����usb��ʱ��û��bus reset
        }
#endif

        ud_get_status(&udisk_status);
        if ((udisk_status.line_sta == 0x01) || (udisk_status.line_sta == 0x02))
        {
            break;
        }

        //  1:���ߣ�2:����
        /*     if((g_udisk_status.line_sta== 0x01)||(g_udisk_status.line_sta == 0x02))
         {
         break;
         }*/

        //switch to adfu
        if (g_udisk_status.switch_to_adfu == 0x01)
        {
            //�̼�����
            result = RESULT_AUTO_UPGRADE;
            goto exit_usb;
        }
        else if (g_udisk_status.switch_to_adfu == 0x02)
        {

            //����stubӦ�ó���
            result = RESULT_ENTER_STUB_DEBUG;
            goto exit_usb;
        }
        else
        {
            ;//do nothing
        }

        if (g_udisk_status.remove_card == 1)
        {
            ud_set_cmd(SET_CARD_INOUT, 0);
        }

        display();

        if (((returnkey_press == 1) && (g_udisk_status.line_sta == 0)) || (g_udisk_status.line_sta == 0x90))
        {
            break;
        }
        else
        {
            returnkey_press = 0;
        }
        //����10ms�����������
        sys_os_time_dly(1);
        wait_time_before_enum++;

    }

    result = udisk_exit_all(argc);

    exit_usb:

    udisk_timer_exit();
    return result;

}

