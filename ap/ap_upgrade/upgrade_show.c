#include "ap_upgrade.h"
//#include "audio_device.h"

static void display_connect_pic(uint8 value)
{
    uint8 i;
    uint8 frame;
    uint16 pix_cnt;
    region_t res_region;

    for (i = 0; i < 5; i++)
    {
        if (value == (2 * i + 2))
        {
            //libc_print("i:",i,2);
            break;
        }
    }

    if (i >= 5)
    {
        i = 4;
        //libc_print("case:5==i",0,0);
    }
    //frame = 2 * i;
    frame = i;

    /*��connect picture buffer�е�ͼƬˢ������*/
    res_region.x = UPGRADE_POS_CONNECT_X_BASE + i * UPGRADE_POS_CONNECT_INC_X;
    res_region.y = UPGRADE_POS_CONNECT_Y_BASE;
    res_region.width = UPGRADE_WIDTH_CONNECT;
    res_region.height = UPGRADE_HEIGHT_CONNECT;

    lcd_set_window(&res_region); //�贰��������
    pix_cnt = res_region.width * res_region.height;

    libc_memcpy(LCD_BUF1_ADDR, UPGRADE_CONNECT_PIC_BUFFER + frame * UPGRADE_CONNECT_PIC_LENGTH, pix_cnt * 2);
    lcd_dma_set_counter(pix_cnt, 0);
    lcd_dma_start_trans(LCD_RAM1_INDEX);

}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ���¹̼����������е���ʾ
 * \param[in]    value     ������ʾ��ֵ
 * \return       none
 * \note
 *******************************************************************************/
void _upgrade_refreash_progress(uint8 value)
{
    /*
     style_infor_t upgrade_sty;
     progressbar_private_t progressbar_param;
     textbox_private_t textbox_param;
     upgrade_sty.type = UI_AP;

     // ��ʾ�ٷֱ�
     upgrade_sty.style_id = UPGRADE_PERCENT;
     textbox_param.lang_id = ANSIDATAAUTO;
     textbox_param.str_value = (uint8 *) &upgrade_percent_str[value];
     ui_show_textbox(&upgrade_sty, &textbox_param, TEXTBOX_DRAW_NORMAL);
     // ��ʾ������
     upgrade_sty.style_id = UPGRADE_PROGRESS;
     progressbar_param.total = 100;
     progressbar_param.value = value * 10;
     libc_print("valu2",progressbar_param.value,2);
     ui_show_progressbar(&upgrade_sty, &progressbar_param, PROGRESS_DRAW_PROGRESS);*/

    /*****ע��:���ﲻ����ui_show_textbox(),ui_show_progressbar()��ui��ʾ��������Ϊ��Щ��������BANK������
     �ڶ�ʱ�����������Ƶ��������BANK�������в˵��Ļ��ƣ�����BUG������ֲ��ܰ���ϣ����ʽ���� -by hjh**********/

    //libc_print("refresh",value,2);
    display_connect_pic(value);
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  ��ʱ����ʾ���������ڸ��¹̼����������е���ʾ
 * \param[in]    none
 * \return       none
 * \note
 *******************************************************************************/
void _upgrade_proc_timer(void)
{
    uint8 old_precent_count;
    uint32 sector_total;

    old_precent_count = g_precent_count;
    sector_total = (uint32) ((_para.lfi_length + _para.brec_length) >> 9);
    //libc_print("se_to",sector_total,2);
    if (sector_total == 0)
    {
        return;
    }

    //libc_print("wsector",_para.g_write_sector,2); //2015-12-03
    //g_precent_count = (uint8)((9 * g_write_sector) / sector_total);
    g_precent_count = (uint8) ((10 * _para.g_write_sector) / sector_total);
    if (1 == g_precent_count)
    {
        //�����ļ���ʼ����ʱ������Ϊ20%
        g_precent_count = 2;
    }
    else if (9 == g_precent_count)
    {
        //�����ļ��쵽β���ˣ���ʱ������Ϊ100%
        g_precent_count = 10;
    }
    else
    {
        ; //nothing
    }

    //libc_print("OLDPER",old_precent_count,2); //2015-12-03
    //libc_print("PERCEN",g_precent_count,2); //2015-12-03

    if ((old_precent_count != g_precent_count) && (0 == (g_precent_count % 2)))
    {
        _upgrade_refreash_progress(g_precent_count);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *	 �̼����������л�ȡ��Ϣ�ӿ�
 * \param[in]    none
 * \return       app_result_e �ᱻ����
 * \note
 *******************************************************************************/

app_result_e upgrade_get_app_msg(void)
{
    private_msg_t private_msg;
    input_gui_msg_t gui_msg;
    app_result_e result = RESULT_NULL;

    if (get_gui_msg(&gui_msg) == TRUE)
    {
        if (INPUT_MSG_KEY == gui_msg.type)
        {
            if ((gui_msg.data.kmsg.val == KEY_LOCK) || (gui_msg.data.kmsg.val == KEY_UNLOCK))
            {
                gui_keylock(TRUE);
            }
        }
    }
    g_sys_counter.battery_counter = 100;//ÿ�ν��붼����͵�
    if (get_app_msg(&private_msg) == TRUE)
    {
        if (private_msg.msg.type == MSG_EARPHONE_IN)
        {
            /*���������ߣ�����*/
            change_antenna_state(TRUE);
        }
        else if (private_msg.msg.type == MSG_EARPHONE_OUT)
        {
            /*���������ߣ��γ�*/
            change_antenna_state(FALSE);
        }
        else
        {
        }
    }

    //����10ms�����������
    cpu_release_count++;
    if ((cpu_release_count % 5) == 0)
    {
        cpu_release_count = 0;
        //��ʱ�ͷ�cpu
        sys_os_time_dly(1);
    }

    return result;
}

