/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������CASE ���͵�������ģ�飬���������ȼ������ã����͵������HANDLE����Ҫ
 *       ������2Hz��ʱ�����С�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_func.h"
#include "key_interface.h"

#define _BAT_PRINTD_INFO_INT(a,b)   // PRINTD_INFO_INT(a,b)
#define _BAT_PRINTD_DBG(a)           // PRINTD_INFO(a)

//�������������ı�ʱ��Ҫ����
//����TRUE��ʾ������ϣ����μ�⴦����OK������FALSE��ʾ����������ʱ����
void com_battery_charge_change_deal(uint8 cur_charge_state, uint8 cur_bat_val)
{
    uint16 msg;

    if (cur_charge_state == BAT_NORMAL)
    {
        if (cur_bat_val <= 0)
        {
            if ((g_app_info_state.charge_state != BAT_NORMAL)
                    || (g_app_info_state.bat_value > cur_bat_val))
            {
                g_sys_counter.bat_low_cnt = 0;

       /*          _BAT_PRINTD_INFO_INT("bat lower0:",g_sys_counter.bat_low_cnt);
                //��⵽�͵磬�㲥�͵���Ϣ
                msg = MSG_LOW_POWER;
                sys_mq_send(MQ_ID_SYS, &msg);
*/
            }
        }
    }
    else if (cur_charge_state == BAT_FULL)
    {
        if (g_app_info_state.charge_state != BAT_FULL)
        {
            g_sys_counter.charge_full_cnt = 0;
        }
    }
    else
    {
        ; //nothing for QAC
    }

    g_app_info_state.charge_state = cur_charge_state;
    g_app_info_state.bat_value = cur_bat_val;
}

/******************************************************************************/
/*!
 * \par  Description:
 *  ����������йع��ܣ�����ʾ����־������ѹ(<3.6VΪ��ѹ)��
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup
 * \note
 *******************************************************************************/
void com_battery_charge_deal(void)
{
    uint16 msg;
    uint8 tmp_battery_value; //��ǰ��ص���
    battery_status_e tmp_charge_state; //��ǰ���״̬�����޳�磬���ڳ�磬����������޵��
    uint8 real_charge_value;
    int8 volume_limit;

    tmp_charge_state = key_chargeget(&real_charge_value);
//    _BAT_PRINTD_INFO_INT("charge state:",tmp_charge_state);
//    _BAT_PRINTD_INFO_INT("charge value:",real_charge_value);
    if (real_charge_value < BATTERY_GRADE_MAX)
    {
        tmp_battery_value = real_charge_value;
    }
    else
    {
        tmp_battery_value = BATTERY_GRADE_MAX;
    }

    if (tmp_charge_state == BAT_CHECKING)
//    if ((tmp_charge_state == BAT_CHECKING) || (tmp_charge_state == BAT_NO_EXIST))
    {
        tmp_charge_state = BAT_NORMAL;
        tmp_battery_value = BATTERY_GRADE_MAX;
    }

    //��Ҫ���˵�ع���ʱ��ص������ȶ��������ֻ�����ص������µ���������������
    if ((g_app_info_state.charge_state == BAT_NORMAL) && (tmp_charge_state == BAT_NORMAL) && (tmp_battery_value
            > g_app_info_state.bat_value))
    {
        tmp_battery_value = g_app_info_state.bat_value;

    }

    if ((g_app_info_state.charge_state != tmp_charge_state) || (g_app_info_state.bat_value != tmp_battery_value))
    {
        com_battery_charge_change_deal(tmp_charge_state, tmp_battery_value);
        return;
    }

    if ((g_app_info_state.charge_state == BAT_NORMAL) && (g_app_info_state.bat_value == 0))
    {
        g_sys_counter.bat_low_cnt++;
        if (g_sys_counter.bat_low_cnt >= 120)
        {
            g_sys_counter.bat_low_cnt = 0;

    //        _BAT_PRINTD_INFO_INT("bat lower1:",g_app_info_state.bat_value );
            //��⵽�͵磬�㲥�͵���Ϣ
            msg = MSG_LOW_POWER;
            sys_mq_send(MQ_ID_SYS, &msg);
        }
    }
    else if (g_app_info_state.charge_state == BAT_FULL)
    {
        g_sys_counter.charge_full_cnt++;
        if (g_sys_counter.charge_full_cnt == 8)//����Լ4�룬����ѹ�Ƿ���
        {
            if (real_charge_value < (BATTERY_GRADE_MAX ))
            {
                g_sys_counter.charge_full_cnt = 0;
                key_chargeset(0, 0, 0);//�򿪳�磬����һ�β���
            }
        }
        if (g_sys_counter.charge_full_cnt >= 120)
        {
            g_sys_counter.charge_full_cnt = 0;

            //�������
            msg = MSG_FULL_CHARGE;
            sys_mq_send(MQ_ID_SYS, &msg);
        }
    }
    else
    {
        g_sys_counter.bat_low_cnt= 0;
        ; //nothing for QAC
    }
}
