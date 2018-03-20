/*
 ********************************************************************************
 *                       ACTOS
 *                  key board message define
 *
 *                (c) Copyright 2002-2013, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File   : key.msa
 * By     : johnsen
 * Version: 1> v1.00     first version     2013-5-18 13:53
 ********************************************************************************
 ********************************************************************************
 */
#ifndef _KEY_INNER_H            //防止重定义
#define _KEY_INNER_H

//#include "psp_includes.h"
#include "key_interface.h"
#include "gui_msg.h"
#include "app_msg.h"
#include "TWI.h"

//#define lock_key

/*define key message of key driver*/

#define Msg_KeyShortUp        0x30    //短按键抬起
#define Msg_KeyLongUp         0x31    //长按键抬起
#define Msg_KeyHold           0x32        //避免和Msg_KeyLoop|AP_KEY_UP冲突
#define Msg_KeyUnHold         0x34

//#define lock_key

#define MAX_HOLDKEY_ADC     0x39
#define MIN_HOLDKEY_ADC     0x33

extern uint8 g_oldkey_val;
extern uint8 g_tmpkey_val;
extern uint8 g_tmp_count;
extern uint8 g_hold_count;

extern int g_time_irq;

extern unsigned char key_map[16];
extern unsigned char g_speakonoff;

//scan timer interrupt process
extern void key_scan_int_process(void);


//电池及其充电接口
extern void key_battery_charge_deal(void);
extern void key_inner_battery_charge_open(chg_current_e CurrentMA, chg_voltage_e VoltageMV, void *null2);
extern void key_inner_battery_charge_stop(void *null0, void *null1, void *null2);
extern battery_status_e key_inner_battery_get_status(uint8 *ad_val, int8 *vol_limit, void *null2);

extern uint8 *key_inner_getkeytabaddress(void *null0, void *null1, void *null2);
extern battery_status_e key_inner_chargeget(void *null0, void *null1, void *null2);
extern void key_inner_chargeset(charge_control_e setting, chg_current_e current, chg_voltage_e voltage);
extern uint8 key_inner_holdcheck(void *null0, void *null1, void *null2);
extern uint8 *key_inner_speakcheck(uint8 onoff, void *null1, void *null2);
void key_inner_earphone_check(uint8 onoff, void *null1, void *null2);

extern void key_bat_charge_init(chg_current_e CurrentMA, chg_voltage_e VoltageMV) __FAR__;
extern void key_sampling_battery_voltage_init(void) __FAR__;
extern void adjust_freq_do_init(void) __FAR__;

extern int adjust_freq_op_entry(void * , void * , int) __FAR__;

extern void key_inner_peripheral_detect_handle(uint32 detect_mode) __FAR__;

extern uint8 g_key_bat_charge_mode;


#endif/*_KEY_H*/
