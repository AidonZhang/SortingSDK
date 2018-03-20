/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：CASE 充电和电量管理模块，包括电量等级表配置，充电和电量监测HANDLE，需要
 *       挂载在2Hz软定时器运行。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_func.h"
#include "key_interface.h"

#define _BAT_PRINTD_INFO_INT(a,b)   // PRINTD_INFO_INT(a,b)
#define _BAT_PRINTD_DBG(a)           // PRINTD_INFO(a)

//充电与电量发生改变时需要处理
//返回TRUE表示处理完毕，本次检测处理已OK；返回FALSE表示继续后续计时处理
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
                //检测到低电，广播低电消息
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
 *  处理跟电量有关功能，显显示充电标志，检测低压(<3.6V为低压)等
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup
 * \note
 *******************************************************************************/
void com_battery_charge_deal(void)
{
    uint16 msg;
    uint8 tmp_battery_value; //当前电池电量
    battery_status_e tmp_charge_state; //当前充电状态，分无充电，正在充电，充电已满，无电池
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

    //需要过滤电池供电时电池电量不稳定的情况，只允许电池电量往下跌，不允许往上涨
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
            //检测到低电，广播低电消息
            msg = MSG_LOW_POWER;
            sys_mq_send(MQ_ID_SYS, &msg);
        }
    }
    else if (g_app_info_state.charge_state == BAT_FULL)
    {
        g_sys_counter.charge_full_cnt++;
        if (g_sys_counter.charge_full_cnt == 8)//充满约4秒，检测电压是否满
        {
            if (real_charge_value < (BATTERY_GRADE_MAX ))
            {
                g_sys_counter.charge_full_cnt = 0;
                key_chargeset(0, 0, 0);//打开充电，用上一次参数
            }
        }
        if (g_sys_counter.charge_full_cnt >= 120)
        {
            g_sys_counter.charge_full_cnt = 0;

            //充电已满
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
