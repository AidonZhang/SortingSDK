/********************************************************************************
 *                            Module: music_engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-05 15:00     1.0             build this file
 ********************************************************************************/
#ifndef __APP_MENGINE_H__
#define __APP_MENGINE_H__

#include "psp_includes.h"
#include "case_include.h"
#include "enhanced.h"

#include "music_common.h"
#include "setting_common.h"

/* 应用自己需要的软定时器个数 */
#define  APP_TIMER_COUNT		3

/*! 定义音乐引擎的任务栈和主线程优先级 */
#define  AP_MENGINE_STK_POS  AP_BACK_LOW_STK_POS
#define  AP_MENGINE_PRIO     AP_BACK_LOW_PRIO

/*! 歌曲切换进行断点保存开关 */
//#define CUR_FILE_SWITCH

/*! Audible是否需要显示章节图片开关 */
#define AUDIBLE_CHAPTER_IMAGE

/*浏览播放的时间*/
#define  INTRO_INTERVAL 11000

/*! 当前时间无变化等待时间 */
#define WAIT_TIMEOUT   12 //12*250ms=3000ms
#define TIME_LIMIT     2000

/*
  每种EQ类型的增益点个数点已经确定
  如果增加增益点个数,每增加1个点,DSP频率需要提高10M左右
  对应eq_param数据,打开此宏表示有增加增益点数
  打开此宏会增加功耗,需谨慎
*/
//#define EQ_PARAM_POINT_ADD

typedef struct
{
    uint16 magic;       //VM 变量魔数
    uint8 ap_id;        //关机的应用
    uint8 engine_state; //引擎状态
    uint8 keyID[16];    //按键定义数组
} g_config_var_t;


//音乐播放模式
typedef enum
{
    //正常播放
    PLAY_NORMAL,
    //续播
    PLAY_RESUME,
    //继续快进
    PLAY_FFON,
    //快退播放
    PLAY_FBON,
    //不需要播放
    PLAY_NO_PLAY
} play_mode_e;

//音乐停止模式
typedef enum
{
    //正常停止
    STOP_NORMAL,
    //暂停
    STOP_PAUSE
} stop_mode_e;

typedef enum
{
    NORMAL_SWITCH_NEXT = 0x0,
    NORMAL_SWITCH_PREV = 0x1,
    FORCE_SWITCH_NEXT  = 0x2,
    FORCE_SWITCH_PREV  = 0x3,
    END_SWITCH         = 0x4
} switch_mode_e;

typedef enum
{
    SWITCH_NO_ERR,
    SWITCH_ERR_STOP,
    SWITCH_ERR_SWITCH,
    SWITCH_ERR_PLAY
} switch_result_e;


//for audible
typedef struct
{
    // 小机序列号
    unsigned char DeviceId_[20];
    //激活文件,sys文件内容
    unsigned char ActivationRecord_[560];
    //pos文件内容
    unsigned char playbackPosFilebuffer[16];
    //总的章节数目
    unsigned int m_cChapters_;
    // 章节时间
    unsigned int SongChapters_[200];

    //总的图片数目
    unsigned int m_cImages_;
    //图片时间
    unsigned int imageTime_[200];
    //图片位置
    unsigned int imagePos_[200];
    // 图片大小
    //unsigned int imageSize_[200];

    //章节文本位置
    //unsigned int textPos_[200];

} audible_para_t;

/*!
 * \brief
 *  引擎信息结构体变量定义
 */ 
typedef struct
{
    /******************************************************************
     *          引擎状态信息
     *******************************************************************/
    mengine_status_t eg_status;

    /******************************************************************
     *          播放文件的时间和bit率信息
     *******************************************************************/
    mengine_playinfo_t eg_playinfo;

    /******************************************************************
     *          中间件播放文件信息
     *******************************************************************/
    mmm_mp_file_info_t eg_file_info;

    /******************************************************************

     *          设置信息(保存配置)
     *******************************************************************/
    mengine_config_t eg_config;

} mengine_info_t;


//functions

//for audible
extern void _deal_audible_posfile(uint8 mode)
__FAR__;

extern void _check_time_switch(uint32 time_sel)
__FAR__;

extern void _check_time_switch(uint32 time_sel)
__FAR__;

extern void _deal_audible_sysfile(void)
__FAR__;

extern void _clr_chapter_timer(void)
__FAR__;

extern void _clr_imag_timer(void)
__FAR__;

extern void _set_chapter_timer(void)
__FAR__;

extern void _set_imag_timer(void)
__FAR__;

extern void _check_audible_chapter(void)
__FAR__;

extern void _check_audible_image(void)
__FAR__;

extern bool mengine_audible_getinfo(void)
__FAR__;

extern void mengine_save_audible_breakinfo(void) 
__FAR__;

extern void _check_pos_time(void)
__FAR__;

extern void _chapter_switch_force(bool next_prev)
__FAR__;

//extern app_result_e mengine_audible_getimage(void* msg_ptr)
//__FAR__;

extern app_result_e mengine_control_block(void)
__FAR__;

extern bool change_locat_deal(void)
__FAR__;

extern app_result_e mengine_message_done(private_msg_t* cur_msg)
__FAR__;

extern bool mengine_file_init(void)
__FAR__;

//extern void _end_switch(play_mode_e last_mode)
//__FAR__;

extern bool _set_file(void)
__FAR__;

extern bool _play(play_mode_e play_mode)

__FAR__;

extern bool _stop(stop_mode_e stop_mode)
__FAR__;

//extern bool _next(bool force_mod)
//__FAR__;

//extern bool _prev(bool force_mod)
//__FAR__;

extern eg_err_e _convert_error_no(mmm_mp_err_t err_no)
__FAR__;

extern void _error_timeout_handle(void)
__FAR__;

//extern uint32 _volume_deal(uint8 vol_in, bool set_mode)
//__FAR__;

extern void mengine_save_errno_no(void)
__FAR__;
//for event
//extern app_result_e mengine_get_config(void* msg_ptr) __FAR__;

extern app_result_e mengine_get_playmode(void* msg_ptr)
__FAR__;

extern app_result_e mengine_get_filepath(void* msg_ptr)
__FAR__;

extern app_result_e mengine_get_fileinfo(void* msg_ptr)
__FAR__;

extern app_result_e mengine_get_status(void* msg_ptr)
__FAR__;

extern app_result_e mengine_get_playinfo(void* msg_ptr)
__FAR__;

extern app_result_e mengine_get_bkinfo(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_eq(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_volume(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_playmode(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_ab_count(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_ab_gap(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_ab_gap(void* msg_ptr)
__FAR__;

extern app_result_e mengine_start_ab(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_apoint(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_bpoint(void* msg_ptr)
__FAR__;

extern app_result_e mengine_clear_ab(void* msg_ptr)
__FAR__;

extern app_result_e mengine_switch(void* msg_ptr)
__FAR__;

extern app_result_e mengine_play(void* msg_ptr)
__FAR__;

extern app_result_e mengine_stop(void* msg_ptr)
__FAR__;

extern app_result_e mengine_pause(void* msg_ptr)
__FAR__;

extern app_result_e mengine_resume(void* msg_ptr)
__FAR__;

extern app_result_e mengine_fast_forward(void* msg_ptr)
__FAR__;

extern app_result_e mengine_fast_backward(void* msg_ptr)
__FAR__;

extern app_result_e mengine_fastplay_step(void *msg_ptr)
__FAR__;

extern app_result_e mengine_cancel_ffb(void* msg_ptr)
__FAR__;

extern app_result_e mengine_play_next(void* msg_ptr)
__FAR__;

extern app_result_e mengine_play_prev(void* msg_ptr)
__FAR__;

extern app_result_e mengine_play_cuemusic(void* msg_ptr)
__FAR__;

extern app_result_e mengine_musui_delete_file(void* msg_ptr)
__FAR__;

extern app_result_e mengine_other_delete_file(void* msg_ptr)
__FAR__;

extern app_result_e mengine_filepath_deal(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_filepath(void* msg_ptr)
__FAR__;

extern app_result_e mengine_change_filepath(void* msg_ptr)
__FAR__;

extern app_result_e mengine_del_favorite_deal(void* msg_ptr)
__FAR__;

extern app_result_e mengine_add_favorite_deal(void* msg_ptr)
__FAR__;

extern app_result_e mengine_clear_error(void* msg_ptr)
__FAR__;

extern app_result_e mengine_chapter_next(void* msg_ptr)
__FAR__;

extern app_result_e mengine_chapter_prev(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_section_mode(void* msg_ptr)
__FAR__;

extern app_result_e mengine_audible_bookmark(void* msg_ptr)
__FAR__;

extern app_result_e mengine_message_done_bank(private_msg_t* cur_msg)
__FAR__;

extern void mengine_set_real_eq(uint8 mode)
__FAR__;

extern app_result_e mengine_audible_playspeed(void* msg_ptr)
__FAR__;

//extern app_result_e mengine_get_engine_info(void* msg_ptr)
//__FAR__;

extern void mengine_reply_msg(void* msg_ptr, bool ret_vals)
__FAR__;

extern switch_result_e mengine_file_switch(stop_mode_e stop_mode, switch_mode_e force_mode, play_mode_e play_mode)
__FAR__;

extern app_result_e mengine_standby_play(void* msg_ptr)
__FAR__;

extern app_result_e mengine_audible_get_section_info(void* msg_ptr);

extern void _adjust_big_little_endian(uint8 *data_address, uint8 data_length);

extern bool _mengine_start_sound(uint8 cur_volume);

extern bool _mengine_stop_sound(void);

extern bool _mengine_check_sd_in(void);

extern int _get_file_info(uint8 normal_file);

extern bool _file_switch_info(void);

extern void mengine_set_real_playspeed(void);

//file io
extern int32 mmm_mp_read(void *buf, int32 size, int32 count, storage_io_t *io) __FAR__;
extern int32 mmm_mp_seek(storage_io_t *io, int32 offset, int32 whence)__FAR__;
extern int32 mmm_mp_tell(storage_io_t *io)__FAR__;
extern int32 mmm_mp_read_sd(void *buf, int32 size, int32 count, storage_io_t *io)__FAR__;
extern int32 mmm_mp_seek_sd(storage_io_t *io, int32 offset, int32 whence)__FAR__;
extern int32 mmm_mp_tell_sd(storage_io_t *io)__FAR__;

#endif //__APP_MENGINE_H__
