
 /*******************************************************************************
 * \file     upgrade_main.c
 * \brief    �Զ�����ap��ģ��
 * \author   hujunhua
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date    2016-01-13
 *******************************************************************************/
#include "ap_upgrade.h"

#define APP_TIMER_COUNT 1


//�ٷֱȼ���
uint8 g_precent_count;

//ˢ�¶�ʱ��ID
int8 g_flush_timer_id;

//cpu�ͷż�ʱ��
uint8 cpu_release_count;

//�������ݽṹ��
PCFWDecrypt_struct g_decrypt;


app_timer_t conf_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];

const uint16 style_upgrade_point[5] =
{ UPGRADE_POINT_1, UPGRADE_POINT_2, UPGRADE_POINT_3, UPGRADE_POINT_4, UPGRADE_POINT_5 };

/*main��Ҫ���������ֵ����ϵͳ��Ա����*/
#ifndef PC
OS_STK *ptos = (OS_STK *) AP_FRONT_LOW_STK_POS;
#endif
INT8U prio = AP_FRONT_LOW_PRIO;



/******************************************************************************/
/*!
 * \par  Description:
 *    upgrade applib�˳�
 * \param[in]    void
 * \param[out]   void
 * \return       bool
 * \retval       1    success
 * \retval       0    fail
 * \ingroup      upgrade
 * \note
 *******************************************************************************/
bool _upgrade_app_deinit(void)
{
    kill_app_timer(g_flush_timer_id);

    //ж���ļ�ϵͳ
    //_upgrade_fs_deinit(DISK_C);

    sys_timer_exit();

    ui_res_close(UI_COM);
    ui_res_close(UI_AP);

    //ui_set_display_buf(JPEG_RAM_INDEX);

    applib_quit();

    return TRUE;
}

 /********************************************************************************
 * Description : �����ݴ������ͼƬ��������
 *
 * Arguments  :     ramaddr
 *
 *
 * Returns     :
 *            ��
 * Notes       :
 *
 ********************************************************************************/
static void upgrade_read_connect_pic_to_ram(void)
{
    res_size_t tmp_size;
    uint8 i;
    for (i = 0; i < (UPGRADE_CONNECT_PIC_FRAME / 2); i++)
    {
        ui_read_pic_to_buffer(P_UPGRADE_POINT_1A + 2 * i, \
        (uint8 *) (UPGRADE_CONNECT_PIC_BUFFER + i * UPGRADE_CONNECT_PIC_LENGTH), &tmp_size);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    upgrade��ʾ��ʼ��
 * \param[in]    void
 * \param[out]   void
 * \return       none
 * \retval       none
 * \ingroup      upgrade
 * \note
 *******************************************************************************/

void _upgrade_display_init(void)
{
    /*pic_box �ӿڱ���*/
    style_infor_t upgrade_sty;
    textbox_private_t textbox_param;
    picbox_private_t picbox_data;
    uint8 i;

    upgrade_sty.type = UI_AP;

    //draw bg
    ui_set_backgd_color(Color_BACKCOLOR_def);
    ui_clear_screen(NULL); 

    // ��ʾ���������ַ���
    textbox_param.lang_id = UNICODEID;
    textbox_param.str_id = -1;
    textbox_param.str_value = NULL;
    upgrade_sty.style_id = UPGRADE_STRING;
    ui_show_textbox(&upgrade_sty, &textbox_param, TEXTBOX_DRAW_NORMAL);

    // ��ʾ��ʾ�ַ���
    upgrade_sty.style_id = UPGRADE_STRING2;
    ui_show_textbox(&upgrade_sty, &textbox_param, TEXTBOX_DRAW_NORMAL);

    //������Ҫ��̬ˢ�µ�ͼƬ���ڴ�
    upgrade_read_connect_pic_to_ram();

    for (i = 0; i < (UPGRADE_CONNECT_PIC_FRAME / 2); i++)
    {
        upgrade_sty.style_id = style_upgrade_point[i];
        picbox_data.pic_id = -1;
        picbox_data.frame_id = 0;
        //��ʾ����
        ui_show_picbox(&upgrade_sty, &picbox_data);
    }
    
    //��ʱ��300ms
    g_flush_timer_id = set_app_timer(APP_TIMER_ATTRB_UI, 300, _upgrade_proc_timer);
}

/******************************************************************************/
/*!
 * \par  Description:
 *  applib��ʼ��
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      upgrade
 * \note
 *******************************************************************************/
bool _upgrade_app_init(void)
{
    //��ʼ��applib�⣬ǰ̨AP
    applib_init(APP_ID_UPGRADE, APP_TYPE_GUI);

    //��ʼ����ʱ��
    init_app_timers(conf_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    //��ʼ�� applib ��Ϣģ��
    applib_message_init(NULL);

    /*��ʼ��ϵͳ��ʱ��*/
    sys_timer_init();

    //��ʼ��ap��Դ
    ui_res_open("upgrade.sty", UI_AP);

    //��ʼ��common��Դ
    ui_res_open("common.sty", UI_COM);

    //װ���ļ�ϵͳ
    //_upgrade_fs_init(DISK_H);

    _upgrade_display_init();

   // _close_engine();

    return TRUE;
}



/******************************************************************************/
/*!
 * \par  Description:
 *    upgradeӦ�õ���ں���
 * \param[in]    0-����������ֵ���ػ�����16bit����ػ�ʱ��ǰ̨Ӧ��ID����16bit��ʾ��̨�����״̬
 * \param[out]   none
 * \return       the result
 * \retval
 * \retval
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
int main(int argc, const char *argv[])
{
    upgrade_status_e result;
    uint32 irq_save = 0;
    
    //��ʼ��applib
    _upgrade_app_init();

    irq_save = sys_local_irq_save();
    adjust_freq_set_level(AP_FRONT_LOW_PRIO, FREQ_LEVEL11, FREQ_NULL);//max frequency

    result = _upgrade_firmware();

    _upgrade_app_deinit();

    return result;
}

