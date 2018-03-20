/********************************************************************************
 *                            Module: radio_engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang   2011-09-05 15:00     1.0             build this file
 ********************************************************************************/
#ifndef __APP_FMENGINE_H__
#define __APP_FMENGINE_H__

#include "psp_includes.h"
#include "case_include.h"

#include "fmengine.h"
#include "setting_common.h"
#include "audio_device.h"

/*!定义FM   引擎的任务栈和主线程优先级*/
#define    AP_FMENGINE_STK_POS   AP_BACK_HIGH_STK_POS
#define    AP_FMENGINE_PRIO         AP_BACK_LOW_PRIO

//调试阶段，暂不调用底层驱动，上层实现出声音
//#define  test

#ifdef test
typedef volatile unsigned int* REG32;
#endif

/* 默认搜台门限定义*/
#define    THRESHOLD_DEFAULT       5

/*FMAIN 的类型标识，需和Anolog 驱动传参匹配*/
#define    FM_AIN_TYPE       1
/*FMAIN 增益设置*/
#define    FM_AIN_GAIN       2

/* extern 变量声明*/
extern Engine_Status_t g_fmengine_status;
extern FMEngine_cfg_t g_fmengine_cfg;
extern Module_Status_e g_module_status;
//保存中间件句柄
extern void *g_mmm_pp_handle;

extern mmm_pp_ainout_setting_t g_ainout_param;

//fm通道选择
extern uint8 g_fm_chanel;
//for dae adjust freq
extern dae_config_t *g_dae_cfg_shm;


extern bool play(void)__FAR__;

extern bool stop(void)__FAR__;

extern app_result_e fmengine_play(private_msg_t* cur_msg)
__FAR__;

extern app_result_e fmengine_stop(private_msg_t* cur_msg)
__FAR__;

/* extern 函数声明*/
extern app_result_e fmengine_control_block(void)
__FAR__;
extern app_result_e fm_module_init(private_msg_t* cur_msg)
__FAR__;
extern app_result_e fm_module_reinstall(private_msg_t* cur_msg)
__FAR__;
extern app_result_e fm_module_standby(void)
__FAR__;

extern app_result_e fmengine_set_freq_sync(private_msg_t* cur_msg)
__FAR__;
extern app_result_e fmengine_mute_sync(private_msg_t* cur_msg)
__FAR__;
extern app_result_e fmengine_set_band_sync(private_msg_t* cur_msg)
__FAR__;
extern app_result_e fmengine_set_throd(private_msg_t* cur_msg)
__FAR__;
extern app_result_e fmengine_set_softseek(private_msg_t* cur_msg)
__FAR__;
extern app_result_e fmengine_set_hardseek(private_msg_t* cur_msg)
__FAR__;
extern app_result_e fmengine_seek_break(private_msg_t* cur_msg)
__FAR__;

extern app_result_e fmengine_get_seekflag(private_msg_t* cur_msg)
__FAR__;
extern app_result_e fmengine_get_status(private_msg_t* cur_msg)
__FAR__;
extern app_result_e fmengine_get_band(private_msg_t* cur_msg)
__FAR__;
extern app_result_e fmengine_get_freq(private_msg_t* cur_msg)
__FAR__;
extern app_result_e fmengine_get_intensity(private_msg_t* cur_msg)
__FAR__;
extern app_result_e fmengine_get_antenna(private_msg_t* cur_msg)
__FAR__;
extern app_result_e fmengine_get_stereo(private_msg_t* cur_msg)
__FAR__;
extern app_result_e fmengine_get_playflag(private_msg_t* cur_msg)
__FAR__;
extern app_result_e fmengine_get_volume(private_msg_t* cur_msg)
__FAR__;
extern app_result_e fmengine_get_modual_status(private_msg_t* cur_msg)
__FAR__;

extern void fm_eg_reply_msg(private_msg_t *private_msg) 
__FAR__;

extern void volume_deal(uint8 vol_in) 
__FAR__;

#endif //__APP_FMENGINE_H__
