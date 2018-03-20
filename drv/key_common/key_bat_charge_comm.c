
/*******************************************************************************
 *                              US282A
 *                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       Wekan   2015-3-26am         1.0             review this file
 *******************************************************************************/

#include "psp_includes.h"
#include "key_interface.h"

extern uint8 g_key_bat_charge_mode;
extern uint16 g_charge_tri_time_cnt;
extern uint8 g_charge_bat_check;
extern uint8 g_need_check_battery;
extern uint8 g_battery_check_cnt;

#define _CHARGE_PRINTD_INFO_INT(a,b)     //PRINTD_INFO_INT(a,b)
#define _CHARGE_PRINTD_DBG(a)                 // PRINTD_INFO(a)

void key_sampling_battery_voltage(void);

void key_bat_charge_init(chg_current_e CurrentMA, chg_voltage_e VoltageMV);

const chg_FullVol_e  set_chg_full_vol_tab[8] = //充电结果电压
{
    Chg_FullVol_4160mV,
    Chg_FullVol_4160mV,
    Chg_FullVol_4180mV,
    Chg_FullVol_4180mV,
    Chg_FullVol_4180mV,
    Chg_FullVol_4320mV,
    Chg_FullVol_4320mV,
    Chg_FullVol_4340mV
};

battery_status_e g_bat_charge_status = BAT_NORMAL;

uint8 g_bat_adc_val[BAT_ADC_DATA_NUM] =
{ 0 };//记录电池电压的ADC值
uint8 g_bat_adc_val_index = 0;
uint8 cur_battery_val = 0;
bool g_bat_val_full = FALSE;

//用大电池和稳压源为准结果1-30
//低电电压
#define BATLOW_VEL      BAT_ADC_3_30_V
//充满电电压
#define BATFUL_VEL      BAT_ADC_4_20_V

//电池电量等级对应 ADC 参考值
#define BATTERY_GRADE_MAX_COMM	5

const uint8  battery_grade_vel[BATTERY_GRADE_MAX_COMM+1] =
{
    0,              //低电    返回 0 -- 低电关机
    BATLOW_VEL,     //空格电  返回 1 -- 低电提示，1分钟提示1次
    BAT_ADC_3_50_V,
    BAT_ADC_3_60_V,
    BAT_ADC_3_65_V,
    BAT_ADC_3_85_V,
};

//电压降低后需要限制音量，以限制输出功率，否则会被拉死
/*const uint8  battery_voltage_volume_limit[] =
{
    BAT_ADC_3_75_V, //返回 0db
    BAT_ADC_3_70_V, //返回 -1db
    BAT_ADC_3_65_V, //返回 -2db
    BAT_ADC_3_60_V, //返回 -3db
    BAT_ADC_3_50_V, //返回 -4db
    BAT_ADC_3_45_V, //返回 -5db
    BAT_ADC_3_40_V, //返回 -6db
};*/


uint8 g_charge_MA = 0;
uint8 g_charge_MV = 0;



/******************************************************************************/
/*!
 * \par  Description:
 *    获取电池电量，转换为电量等级。
 * \param[in]    batadc 电池电量ADC采样值，已由按键驱动处理过
 * \param[out]   none
 * \return       uint8 返回电池电量等级；如果大于等于充电满电压，返回 BATTERY_GRADE_MAX + 1
 * \ingroup      misc_func
 * \note
 *******************************************************************************/
static uint8 key_get_battery_grade(uint8 batadc)
{
    int8 i;

    if (batadc >= BATFUL_VEL)
    {
        return 6;
    }

    for (i = 5; i >= 0; i--)
    {

        if (batadc >= battery_grade_vel[i])
        {
            return i;
        }

    }

    return 0;
}

/******************************************************************************
 * \par  Description:   关闭充电
 * \param[in]     null
 * \param[out]   none
 * \return           null
 * \note
 *   <author>    <time>           <version >             <desc>
 *    Wekan   2015-3-26am         1.0             review this
 *******************************************************************************/
void key_inner_battery_charge_stop(void* null0, void* null1, void* null2)
{

    bat_charge_close();
    g_bat_charge_status = BAT_FULL;

}

/******************************************************************************/
/*!
 * \par  Description:
 *    获取当前电池电压需要降低音量多少db。
 * \param[in]    batadc 电池电量ADC采样值，已由按键驱动处理过
 * \param[out]   none
 * \return       int8 返回音量降低db
 * \ingroup      misc_func
 * \note
 *******************************************************************************/
/*static int8  key_get_volume_limit(uint8 batadc)
 {
 int8 i;

 if (g_bat_charge_status != BAT_NORMAL)
 {
 return 0;
 }

 for (i = 0; i < sizeof(battery_voltage_volume_limit); i++)
 {
 if (batadc >= battery_voltage_volume_limit[i])
 {
 break;
 }
 }

 return -i;
 }
 */
/******************************************************************************
 * \par  Description:   检测电源，检测电池，和开关充电
 * \                           电源插入会自动打开充电
 * \param[in]     null
 * \param[out]   none
 * \return           null
 * \note
 *   <author>    <time>           <version >             <desc>
 *    Wekan   2015-3-26am         1.0             review this file
 *******************************************************************************/
void key_battery_charge_deal(void)
{

    uint8 bat_adc;
    static uint8 samping_div = 0;
    samping_div++;
    if (samping_div < 5)
    {
        return;
    }
    samping_div = 0;

    bat_adc = (act_readb(BATADC_DATA)) & 0x7f;

    g_bat_adc_val[g_bat_adc_val_index] = bat_adc;
    g_bat_adc_val_index++;
    if (g_bat_adc_val_index < BAT_ADC_DATA_NUM)
    {
        return;
    }

    _CHARGE_PRINTD_INFO_INT("BAT adc:",bat_adc);
            
    g_bat_adc_val_index = 0;

    if ((act_readl(CHG_DET) & (3 << CHG_DET_UVLO)) != 0)//电源接入
    {
        if ((g_bat_charge_status == BAT_FULL) || (bat_charge_full() != 0))//充满
        {
            _CHARGE_PRINTD_INFO_INT("BAT FULL",act_readl(CHG_DET));
            g_bat_charge_status = BAT_FULL;
            bat_charge_close();

            return;
        }

        if (get_bat_charge_open() != 0)//正在充电
        {
/*            if (g_bat_val_full == TRUE)
            {
                g_charge_tri_time_cnt++;

                
                if (g_charge_tri_time_cnt >= (60 * 15)) //15min
                {
                    _CHARGE_PRINTD_INFO_INT("++ charge full ++",bat_adc);
                    key_inner_battery_charge_stop(0, 0, 0);
                    g_charge_tri_time_cnt = 0;
                }
            }
            else
            {
                g_charge_tri_time_cnt = 0;
            }*/

            if(g_need_check_battery == 1)
            {
                g_charge_bat_check++;
                if (g_charge_bat_check >= 60)
                {
                    _CHARGE_PRINTD_INFO_INT("++ bat check ++",bat_adc);
 //                   g_charge_bat_check = 0;
                    bat_charge_close();
                }
            }
            return;
        }
        else
        {
            if(g_need_check_battery == 1)
            {       

                if (bat_adc == 0)
                {
                    g_need_check_battery = 0;
                    g_bat_charge_status = BAT_NO_EXIST;//没电池
                    _CHARGE_PRINTD_INFO_INT("BAT NO EXIST2",bat_adc);        
                    return;
                }
                
                if (bat_adc < BAT_ADC_3_00_V)
                {
                    if(g_bat_charge_status == BAT_CHECKING)
                    {
                        g_charge_bat_check = 0;
                        _CHARGE_PRINTD_INFO_INT("BAT NO EXIST0",bat_adc);
                    }
                    else
                    {
                        g_need_check_battery = 0;
                        g_bat_charge_status = BAT_NO_EXIST;//没电池
                        _CHARGE_PRINTD_INFO_INT("BAT NO EXIST1",bat_adc);        
                        return;
                    }
                }
                else
                {  
                    g_battery_check_cnt++;
                    g_charge_bat_check = 0;
                     _CHARGE_PRINTD_INFO_INT("BAT  EXIST0",g_battery_check_cnt);
                     if(g_battery_check_cnt >= 2)
                     {   
                         g_need_check_battery = 0;
                     }
                }
            }

            if(g_bat_charge_status != BAT_NO_EXIST ||cur_battery_val >= BAT_ADC_3_00_V)
            {
                bat_charge_open();            
                g_bat_charge_status = BAT_CHARGING;
                _CHARGE_PRINTD_INFO_INT("++charge open++",bat_adc);
            }
        }

    }
    else
    {
        g_bat_charge_status = BAT_NORMAL;
//        g_no_battery_cnt = 0;
        bat_charge_close();
    }

}

/******************************************************************************
 * \par  Description:   打开充电
 * \param[in]     CurrentMA 设置充电电流
 * \param[in]     VoltageMV 设置充电电压
 * \param[out]   none
 * \return           null
 * \note
 *   <author>    <time>           <version >             <desc>
 *    Wekan   2015-3-26am         1.0             review this file
 *******************************************************************************/
void key_inner_battery_charge_open(chg_current_e CurrentMA, chg_voltage_e VoltageMV, void* null2)
{
    key_bat_charge_init(CurrentMA, VoltageMV);

}

/******************************************************************************/
/*!
 * \par  Description:
 *	  Set charge
 * \param[in]      setting CHARGE_START:start charging CHARGE_STOP:stop charging
 * \param[in]      current  choose from charge_current_e
 * \param[in]      param3   choose from battery_full_t
 * \param[out]   charge state
 * \return       the result
 * \retval           0 sucess
 * \retval           1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
void key_inner_chargeset(charge_control_e setting, chg_current_e current, chg_voltage_e voltage)
{
    if (setting == CHARGE_START)
    {
        key_inner_battery_charge_open(current, voltage, 0);

    }
    else
    {
        key_inner_battery_charge_stop(0, 0, 0);
    }

}

/******************************************************************************
 * \par  Description:   获取电池充电状态
 * \param[in]     null
 * \param[out]   ad_val  本次读取的电池采样adc值
 * \param[out]   vol_limit  电压降低后需要限制音量，以限制输出功率，否则会被拉死，单位为 db
 * \return           null
 * \note
 *   <author>    <time>           <version >             <desc>
 *    Wekan   2015-3-26am         1.0             review this
 *******************************************************************************/
battery_status_e key_inner_battery_get_status(uint8 *ad_val, void *null1, void* null2)
{
    uint32 bat_val = 0;
    uint8 i;
    //	bool bat_val_full = FALSE;

    for (i = 0; i < BAT_ADC_DATA_NUM; i++)
    {
        bat_val += g_bat_adc_val[i];
    }

    cur_battery_val = (uint8) ((bat_val / BAT_ADC_DATA_NUM) & 0x7f);//本次ADC的值的计算结果

    //	_CHARGE_PRINTD_INFO_INT("cur battery",cur_battery_val);

    *ad_val = key_get_battery_grade(cur_battery_val);

    //	_CHARGE_PRINTD_INFO_INT("grade",*ad_val);
    //    *vol_limit = key_get_volume_limit(cur_battery_val);

    if (*ad_val > BATTERY_GRADE_MAX_COMM)
    {
        g_bat_val_full = TRUE;
    }
    else
    {
        g_bat_val_full = FALSE;
    }

    return g_bat_charge_status;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  Get charge state
 * \param[in]      none reserve 0
 * \param[in]      none reserve 0
 * \param[in]      none reserve 0
 * \param[out]   charge state
 * \return       the result
 * \retval           0 sucess
 * \retval           1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/

battery_status_e key_inner_chargeget(void *ad_val, void* null1, void* null2)
{

    return key_inner_battery_get_status(ad_val, 0, 0);

}

/******************************************************************************
 * \par  Description:  初始化充电
 * \param[in]    CurrentMA 设置电电流
 * \param[in]    VoltageMV 设置电电压
 * \param[out]   null
 * \return           null
 * \note
 *   <author>    <time>
 *    Wekan   2015-3-26am
 *******************************************************************************/
void key_bat_charge_init(chg_current_e CurrentMA, chg_voltage_e VoltageMV)
{
    uint32 tmp;

    if (CurrentMA == 0)
    {
        CurrentMA = g_charge_MA;
    }
    if (VoltageMV == 0)
    {
        VoltageMV = g_charge_MV;
    }
    g_charge_MA = CurrentMA;
    g_charge_MV = VoltageMV;
    if (CurrentMA >= Chg_current_500mA)
    {
        CurrentMA = Chg_current_500mA;
    }
    if (VoltageMV >= Chg_voltage_4410mV)
    {
        VoltageMV = Chg_voltage_4410mV;
    }
    bat_charge_close();//关充电
    ;//延时
    tmp = act_readl(CHG_CTL);
    tmp &= (~CHG_CTL_CHG_CURRENT_MASK) & (~CHG_CTL_STOPV_MASK) & (~CHG_CTL_ENFASTCHG_MASK);

    tmp |= (uint32) ((1 << CHG_CTL_ENTKLE) //涓流(使能涓流时为充电电流0.1)
            | (CurrentMA << CHG_CTL_CHG_CURRENT_SHIFT)//设置充电电流
            | (VoltageMV << CHG_CTL_ENFASTCHG_SHIFT) //设置恒压充电电压
            | (set_chg_full_vol_tab[(uint8) VoltageMV] << CHG_CTL_STOPV_SHIFT));//设置充满电压

    //            | (VoltageMV << CHG_CTL_STOPV_SHIFT) //设置充满电压
    //            | (set_chg_full_vol_tab[(uint8)VoltageMV] << CHG_CTL_ENFASTCHG_SHIFT));//设置恒压充电电压
    act_writel(tmp, CHG_CTL);
    //注：这里不使能充电
    if ((act_readl(CHG_DET) & (3 << CHG_DET_UVLO)) != 0)//电源接入
    {
        bat_charge_close();//关充电
        //        bat_check_open();
        g_bat_charge_status = BAT_CHECKING;
    }
    else
    {
        g_bat_charge_status = BAT_NORMAL;
    }

    g_charge_tri_time_cnt = 0;
    g_charge_bat_check = 0;
    g_need_check_battery = 1;
    g_battery_check_cnt = 0;    
    _CHARGE_PRINTD_INFO_INT("CHG_CTL:",act_readl(CHG_CTL));
    _CHARGE_PRINTD_INFO_INT("charge-MA-MV:", (CurrentMA << 16) + VoltageMV);
}

/******************************************************************************
 * \par  Description:  电池电压采样
 * \param[in]    void
 * \param[out]   null
 * \return           null
 * \note
 *   <author>    <time>
 *    Wekan   2015-3-26am
 *******************************************************************************/
void key_sampling_battery_voltage(void)
{
    /*   uint8 bat_adc;
     static uint8 samping_div = 0;
     samping_div++;
     if(samping_div < 10)
     {
     return;
     }
     samping_div = 0;

     bat_adc = (act_readb(BATADC_DATA)) & 0x7f;//0-6bit有效

     g_bat_adc_val[g_bat_adc_val_index] = bat_adc;
     g_bat_adc_val_index++;
     if(g_bat_adc_val_index >= BAT_ADC_DATA_NUM)
     {
     g_bat_adc_val_index = 0;
     }*/
}

/******************************************************************************
 * \par  Description:  电池电压采样初始化
 * \param[in]    void
 * \param[out]   null
 * \return           null
 * \note
 *   <author>    <time>
 *    Wekan   2015-3-26am
 *******************************************************************************/

void key_sampling_battery_voltage_init(void)
{
    uint8 bat_adc;
    uint8 i;

    //初始化电池电量采样计算模块

    bat_adc = (act_readb(BATADC_DATA)) & 0x7F;

    for (i = 0; i < BAT_ADC_DATA_NUM; i++)
    {
        g_bat_adc_val[i] = bat_adc;//记录电池电压的ADC值
    }
    g_bat_adc_val_index = 0;
    cur_battery_val = bat_adc;
}

