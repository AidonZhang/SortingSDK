/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang   2011-09-26          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"

bool radio_detect_antenna(void)
{    
    uint32 value;
    act_writel(act_readl(MFP_CTL2) & (~(1 << 31)), MFP_CTL2);
    //EVB ʹ��GPIO_B1 ���ж������
    act_writel((act_readl(GPIOBOUTEN) & (0xfffffffD)), GPIOBOUTEN);
    act_writel((act_readl(GPIOBINEN) | (0x00000002)), GPIOBINEN);
    //B1 pu en
    act_writel(act_readl(GPIOBPUEN) | (0x00000002), GPIOBPUEN);
    sys_mdelay(10);
    value = act_readl(GPIOBDAT) & (0x00000002);

    // enable MEJTAGEN function
    act_writel(act_readl(MFP_CTL2) | (1 << 31), MFP_CTL2);
    if (value == 0)
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
 * \bool radio_engine_init(void)
 * \�����ʼ��
 * \param[in]    void  para1
 * \param[out]   none
 * \return        success/fail
 * \retval   True:   �����Ѿ���װ��ģ���Ѿ���ʼ��
 * \retval   False:  ��װ����ʧ�ܻ��ʼ��ģ��ʧ��
 * \note
 */
/*******************************************************************************/
bool radio_engine_init(void)
{
    engine_type_e engine_type;
    msg_apps_t msg;
    bool result = TRUE;
    bool need_init = FALSE;
    Module_Status_e status;
    uint8 pa_mode;
    uint8 aout_mode;
   

    //��ȡ��ǰ��̨��������
    engine_type = get_engine_type();
    msg.type = MSG_KILL_APP_SYNC; //MSG_KILL_APP;   //ע��ԭ�з�radio ��̨
    if (engine_type == ENGINE_MUSIC)
    {
        msg.content.data[0] = APP_ID_MENGINE;
        //����ͬ����Ϣ
        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }

    //��ǰδ��װ���棬�谲װ
    if (engine_type != ENGINE_RADIO)
    {
        //ͨ��common  �ӿ��ж϶����Ƿ�����, ��δ������, ��װFM  ����
        //�����ٴβ��϶���ʱ��PA
        //���Ѿ���װ����, ��ɽ��벥�ų���, ������
        pa_mode = g_app_info_state.pa_mode;
        aout_mode = g_app_info_state.aout_mode;
        if(pa_mode == 1)
        {
            enable_pa(pa_mode, 0, aout_mode);
            sys_os_time_dly(10);
        }
        
  //      if (get_antenna_state() == FALSE)
        if( radio_detect_antenna() == FALSE)
        {
            //��ʾ��������
            show_dialog(INSERT_EARPHONE, DIALOG_INFOR_WAIT, 0);
            result = FALSE;
            return result;
        }

        result = radio_open_engine();
        if (result == TRUE)
        {
            //���ʼ��ģ��
            need_init = TRUE;
            g_first_open_engine = TRUE;
        }
    }
    //��ǰ�Ѱ�װ���棬���ж�ģ���Ƿ��Ѿ���ʼ��
    else
    {
        g_first_open_engine = FALSE;
        if (g_playwin_mode == ENTER_PLAY_FROM_MAIN)
        {
        	if( radio_detect_antenna() == FALSE)
        	{
        		//��ʾ��������
        		show_dialog(INSERT_EARPHONE, DIALOG_INFOR_WAIT, 0);
        		result = FALSE;
        		return result;
        	}
        }
        if (need_reinstall == TRUE)
        {
            //��FM  ¼�����أ�����¼��codec ������fm �������룬��װ����
#ifndef PC
            //ж��fm ����
            sys_drv_uninstall(DRV_GROUP_FM);
            //��װfm  ����
            sys_drv_install(DRV_GROUP_FM, 0, "drv_fm.drv");
#endif
            need_reinstall = FALSE;
            //need_init = TRUE;
            //��¼������, �����³�ʼ��, ע���ȹ�֮ǰ������
            //com_set_sound_out(FALSE, SOUND_OUT_STOP, NULL);
            radio_modual_reinstall();
            need_init = FALSE;
        }
        else
        {
            radio_get_modual_status(&status);
            if (status != MODULE_WORK)
            {
                need_init = TRUE;
            }
        }
    }

    if (need_init == TRUE)
    {
        //��ʼ��ģ��
        result = radio_modual_init();

        if (!result)
        {
            //ģ���ʼ��ʧ�ܣ���ʾӲ������
            show_dialog(FMERROR, DIALOG_INFOR_WAIT, 0);
            radio_close_engine();
        }

        else
        {            
            //��ʼ������mute��ˢ�½���󣬿�ʼ����
            radio_set_mute(SetMUTE);
            first_wait = TRUE;
        }
    }
    else
    {
        first_wait = FALSE;
    }

    return result;
}

/****************************************************************************************/
/*!
 * \par  Description:
 * \void clear_preset_list(radio_band_e band)
 * \���ָ�����εĵ�̨�б�
 * \param[in]   radio_band_e band
 * \param[out]   none
 * \return       none
 * \retval
 * \retval
 * \note
 1.  �����ǰ����Ϊ�û�ģʽ�������ָ�����ε��б��ֱ�ӷ��أ�
 ��Ϊ�û�ģʽ
 2.  ������յ�ǰ�����б�
 ��յ�̨�б�󣬵�ǰ��̨Ƶ��Ϊָ�����ε�Ĭ��Ƶ��
 */
/******************************************************************************************/
void clear_preset_list(radio_band_e band)
{
    radio_band_e menu_band = band;
    bool user_mode = FALSE;

    if (g_radio_config.band_mode == Band_MAX)
    {
        //��ǰ����Ϊ�û�ģʽ
        user_mode = TRUE;
    }

    /* �˵�ָʾΪ��ͨ����*/
    if (menu_band == Band_China_US)
    {
        libc_memset(g_radio_config.fmstation_us, 0, MAX_STATION_COUNT * 2);
        if (!user_mode)
        {
            //���û�ģʽ�����µ�ǰ����״̬��Ĭ��Ƶ��
            g_radio_config.FMStatus.station = 0;
            g_radio_config.FMStatus.station_count = 0;
            g_radio_config.FMStatus.freq = MIN_US;
        }
    }
    /* �˵�ָʾΪ�ձ�����*/
    else if (menu_band == Band_Japan)
    {
        libc_memset(g_radio_config.fmstation_jp, 0, MAX_STATION_COUNT * 2);
        if (!user_mode)
        {
            g_radio_config.FMStatus.station = 0;
            g_radio_config.FMStatus.station_count = 0;
            g_radio_config.FMStatus.freq = MIN_JP;
        }
    }
    /* �˵�ָʾΪŷ�޲���*/
    else if (menu_band == Band_Europe)
    {
        libc_memset(g_radio_config.fmstation_eu, 0, MAX_STATION_COUNT * 2);
        if (!user_mode)
        {
            g_radio_config.FMStatus.station = 0;
            g_radio_config.FMStatus.station_count = 0;
            g_radio_config.FMStatus.freq = MIN_EU;
        }
    }
    else
    {
        //�˵�����ֻ��Ϊ������һ��
        //;
    }
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void set_start_freq(FM_SeekDir_e dir)
 * \ȫ�Զ���̨��ʼƵ������
 * \param[in]    dir
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void set_start_freq(FM_SeekDir_e dir)
{
    //��ʼƵ������
    if (g_menu_band == Band_China_US)
    {
        if (dir == DIR_UP)
        {
            g_radio_config.FMStatus.freq = MIN_US; //87.5MHz
        }
        else
        {
            g_radio_config.FMStatus.freq = MAX_US; //108MHz
        }
    }
    else if (g_menu_band == Band_Japan)
    {
        if (dir == DIR_UP)
        {
            g_radio_config.FMStatus.freq = MIN_JP; //76MHz
        }
        else
        {
            g_radio_config.FMStatus.freq = MAX_JP; //90MHz
        }
    }
    else
    {
        if (dir == DIR_UP)
        {
            g_radio_config.FMStatus.freq = MIN_EU; //87.5MHz
        }
        else
        {
            g_radio_config.FMStatus.freq = MAX_EU; //108MHz
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_AutoSearch_init(FM_SeekDir_e dir)
 * \ȫ�Զ���̨����������ʼ��
 * \param[in]    dir: ��̨����
 * \param[out]   none
 * \return      success/fail
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
bool radio_AutoSearch_init(FM_SeekDir_e dir)
{
    bool bret;

    bret = radio_engine_init();
    if (!bret)
    {
        //��ʼ��ʧ�ܷ���
        return FALSE;
    }

    //���Ӷ�����Ϊ����������
    show_dialog(USE_ANT, DIALOG_INFOR_WAIT, 0);


    //���Ӵ���-- ��δ���Ӷ���, �������Զ���̨
    if(radio_detect_antenna() == FALSE)
    {
        //��ʾ��������
        show_dialog(INSERT_EARPHONE, DIALOG_INFOR_WAIT, 0);
        return FALSE;
    }

    //����
    ui_set_backgd_color(Color_BACKCOLOR_def);
    ui_clear_screen(NULL);
    //��յ�ǰ�˵����εĵ�̨�б�
    clear_preset_list(g_menu_band);

    //������̨����������ʼƵ��
    set_start_freq(dir);
    //��ջ����̨�б�
    libc_memset(&Auto_tab[0], 0, MAX_STATION_COUNT * 2);

    if (timer_auto_seek != -1)
    {
        kill_app_timer(timer_auto_seek);
    }
    //���ö�ʱ�������ݲ�����
    timer_auto_seek = set_app_timer(APP_TIMER_ATTRB_UI, 300, radio_auto_proc);
    stop_app_timer(timer_auto_seek);

    //headbar���Զ���̨��ˢheadbar
    gui_set_headbar_mode(HEADBAR_MODE_NULL, HEADBAR_ICON_RADIO);
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_AutoSearch_exit(void)
 * \ȫ�Զ���̨�����˳�
 * \param[in]    none
 * \param[out]   none
 * \return      success/fail
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
bool radio_AutoSearch_exit(void)
{
    uint16 play_freq;
    //ɾ����ʱ��
    if (timer_auto_seek != -1)
    {
        kill_app_timer(timer_auto_seek);
    }

    if (g_radio_config.FMStatus.station_count != 0)
    {
        //���������ĵ�̨��ѡ���һ������
        g_radio_config.FMStatus.station = 1;
        g_radio_config.FMStatus.freq = Auto_tab[0];
    }
    else
    {
        //һ����̨Ҳδ��������ѡ��Ĭ��Ƶ�㲥��
        g_radio_config.FMStatus.station = 0;
        if (g_menu_band == Band_Japan)
        {
            play_freq = MIN_JP;
        }
        else
        {
            play_freq = MIN_US;
        }
        g_radio_config.FMStatus.freq = play_freq;
    }

    //��������Ϣ������Ӧ����
    if (g_menu_band == Band_China_US)
    {
        libc_memcpy(&g_radio_config.fmstation_us[0], &Auto_tab[0], MAX_STATION_COUNT * 2);
    }
    else if (g_menu_band == Band_Japan)
    {
        libc_memcpy(&g_radio_config.fmstation_jp[0], &Auto_tab[0], MAX_STATION_COUNT * 2);
    }
    else
    {
        libc_memcpy(&g_radio_config.fmstation_eu[0], &Auto_tab[0], MAX_STATION_COUNT * 2);
    }

    //����Ƶ�㲥��
    radio_set_freq(g_radio_config.FMStatus.freq);
    
    if (radio_detect_antenna() == TRUE)
    {
        //�����Ӷ���, �Ŵ�PA
        radio_set_mute(releaseMUTE);
    }

    //�Զ���̨���л�����Ӧ���Σ��������û�ģʽ
    g_radio_config.band_mode = g_menu_band;
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void auto_display_init(void)
 * \ȫ�Զ���̨���������״�ˢ��
 * \param[in]    none
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void auto_display_init(void)
{
    /*pic_box �ӿڱ���*/
    style_infor_t auto_sty;
    picbox_private_t picbox_param;
    progressbar_private_t progressbar_param;
    textbox_private_t textbox_param;

    ui_icon_count = 0;
    auto_sty.type = UI_AP;
#if 0
    /*��ʾ����ͼƬ*/
    auto_sty.style_id = PICBOX_SEARCH_BG;
    ui_show_picbox(&auto_sty, (void*) NULL);
#endif
    /* ��ʾ��һ��ת��ͼ��*/
    auto_sty.style_id = PICBOX_SEARCH;
    picbox_param.pic_id = -1;
    picbox_param.frame_id = 0;
    //picbox_param.direction = DIRECTION_NORMAL;
    ui_show_picbox(&auto_sty, &picbox_param);

    /* ��ʾ��ʼ������*/
    auto_sty.style_id = PBAR_SEARCH;
    progressbar_param.total = 118;
    progressbar_param.value = 0;
    ui_show_progressbar(&auto_sty, &progressbar_param, PROGRESS_DRAW_ALL);

    /* ��ʾ����������ʾ*/
    auto_sty.style_id = TEXTBOX_SEARCH;
    textbox_param.lang_id = UNICODEID;
    textbox_param.str_id = SEARCH1;
    textbox_param.str_value = NULL;
    ui_show_textbox(&auto_sty, &textbox_param, TEXTBOX_DRAW_NORMAL);

}

