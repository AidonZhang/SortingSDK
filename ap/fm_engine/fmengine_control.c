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
 * \����ת��
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
 * \������Ϣ����,����������յ���˽����Ϣ��ϵͳ��Ϣ
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����
 * \retval       RESULT_APP_QUIT �˳�app
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
        /* ģ���ʼ����ж�����*/

        /* ͬ����ʼ��FM  ģ��*/
        case MSG_FMENGINE_INIT:
        ret_value = fm_module_init(cur_msg);
        break;

        case MSG_FMENGINE_RESINTALL:
        ret_value = fm_module_reinstall(cur_msg);
        break;

        /* FM ģ�����Standby */
        case MSG_FMENGINE_STANDBY:
        ret_value = fm_module_standby();
        break;

        /* Ҫ�������˳������غ�ж������*/
        case MSG_FMENGINE_CLOSE:
        case MSG_APP_QUIT: //�˳�app
        /*��������Ҳ����FM ���淢MSG_FMENGINE_CLOSE  ��ϢҪ��FM �����˳�*/
        if (g_module_status != MODULE_STANDBY)
        {
            fm_module_standby();
        }
        ret_value = RESULT_APP_QUIT;
        break;

        /* �����������*/

        /* ͬ������Ƶ�㲥��*/
        case MSG_FMENGINE_SETFREQ_SYNC:
        ret_value = fmengine_set_freq_sync(cur_msg);
        break;

        /* ������������*/
        case MSG_FMENGINE_SETMUTE:
        ret_value = fmengine_mute_sync(cur_msg);
        break;

        /* ���õ�̨����*/
        case MSG_FMENGINE_SETBAND:
        ret_value = fmengine_set_band_sync(cur_msg);
        break;

        /* ������̨����*/
        case MSG_FMENGINE_SETTHRED:
        ret_value = fmengine_set_throd(cur_msg);
        break;

        /* ���������̨*/
        case MSG_FMENGINE_SET_SOFTSEEK:
        ret_value = fmengine_set_softseek(cur_msg);
        break;

        /* ����Ӳ����̨*/
        case MSG_FMENGINE_SET_HARDSEEK:
        ret_value = fmengine_set_hardseek(cur_msg);
        break;

        /* �˳�Ӳ��seek ����*/
        case MSG_FMENGINE_SET_SEEKBREAK:
        ret_value = fmengine_seek_break(cur_msg);
        break;

        /* ��������*/
        case MSG_FMENGINE_SET_VOLUME:

        display_vol = (uint32)(*(uint8*) (cur_msg->msg.content.addr));

        volume_deal((uint8)display_vol);

        cur_msg->reply->type = MSG_REPLY_SUCCESS;
        //�ظ�ͬ����Ϣ(�����ź���)
        reply_sync_msg(cur_msg);
        ret_value = RESULT_NULL;
        break;

        /* ��ȡ��Ϣ���*/

        /* ��ȡӲ���Զ���̨�Ƿ���ɱ�ʶ*/
        case MSG_FMENGINE_AUTO_SEEK_OVER:
        ret_value = fmengine_get_seekflag(cur_msg);
        break;

        /* ��ȡ��ǰ״̬��Ϣ*/
        case MSG_FMENGINE_GET_STATUS:
        ret_value = fmengine_get_status(cur_msg);
        break;

        /* ��ȡ��ǰ������Ϣ*/
        case MSG_FMENGINE_GET_BAND:
        ret_value = fmengine_get_band(cur_msg);
        break;

        /* ��ȡ��ǰƵ����Ϣ*/
        case MSG_FMENGINE_GET_FREQ:
        ret_value = fmengine_get_freq(cur_msg);
        break;

        /* ��ȡ��ǰ��̨�ź�ǿ����Ϣ*/
        case MSG_FMENGINE_GET_INTENSITY:
        ret_value = fmengine_get_intensity(cur_msg);
        break;

        /* ��ȡ����״̬( �����Ƿ����) */
        case MSG_FMENGINE_GET_ANTENNA:
        ret_value = fmengine_get_antenna(cur_msg);
        break;

        /* ��ȡ������״̬��Ϣ*/
        case MSG_FMENGINE_GET_STEREO:
        ret_value = fmengine_get_stereo(cur_msg);
        break;

        /* ��ȡ��ǰ����״̬*/
        case MSG_FMENGINE_GET_PLAYSTATUS:
        ret_value = fmengine_get_playflag(cur_msg);
        break;

        /* ��ȡ��ǰ����*/
        case MSG_FMENGINE_GET_VOLUME:
        ret_value = fmengine_get_volume(cur_msg);
        break;

        /* ��ȡ��ǰģ��״̬*/
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
 * \Description: FM ����Ӧ��Ϊ����ģ�飬Ҳ������Ӧ�ý���
 * \ ��FM ����ͨѶ�Ľӿ�ģ��
 * \app_result_e fmengine_control_block(void)
 * \���湦�ܴ�����
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
        //��ȡ��������Ϣ
        if (get_app_msg_for_engine(&fmengine_msg) == TRUE)
        {
            result = fmengine_message_deal(&fmengine_msg);
            if (result == RESULT_APP_QUIT)
            {
                break;
            }
        }
        //����10ms �����񽻻�
        sys_os_time_dly(1);
    }
    return result;
}
