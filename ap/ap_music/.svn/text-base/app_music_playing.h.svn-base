/********************************************************************************
 *                            Module: music_ui
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-13 15:00     1.0             build this file
 ********************************************************************************/
#ifndef __APP_MUSIC_PLAYING_H__
#define __APP_MUSIC_PLAYING_H__

#include "psp_includes.h"
#include "case_include.h"
#include "music_common.h"

//play界面显示元素标识
#define up_audibleimage      0x00001000   //audible章节图片
#define up_musicshuffle      0x00000800   //shuffle
#define up_musicprogress     0x00000400   //进度条
#define up_musicloopmode     0x00000200   //循环模式
#define up_musiceqmode       0x00000100   //eq模式
#define up_musictimeinfo     0x00000080   //播放时间
#define up_musicinformation  0x00000040   //id3信息
#define up_musicplaystatus   0x00000020   //播放状态
#define up_lyricflag         0x00000010   //歌词
#define up_musicalbumart     0x00000008   //专辑图片
#define up_musictracks       0x00000004   //曲目信息(总数与序号)
#define up_musicheadbar      0x00000002   //标题栏
#define up_musicBackGDPic    0x00000001   //背景
//--------------------------清除刷新标志
#define clr_audibleimage     0xffffefff   //清除audible章节图片
#define clr_musicshuffle     0xfffff7ff   //清除shuffle
#define clr_musicprogress    0xfffffbff   //清除进度条
#define clr_musicloopmode    0xfffffdff   //清除循环模式
#define clr_musiceqmode      0xfffffeff   //清除eq信息
#define clr_musictimeinfo    0xffffff7f   //清除播放时间
#define clr_musicinformation 0xffffffbf   //清除id3信息
#define clr_musicplaystatus  0xffffffdf   //清除播放状态、AB复读
#define clr_lyricflag        0xffffffef   //清除歌词信息
#define clr_musicalbumart    0xfffffff7   //清除专辑信息
#define clr_musictracks      0xfffffffb   //清除曲目信息
#define clr_musicheadbar     0xfffffffd   //清除标题栏
#define clr_musicBackGDPic   0xfffffffe   //清除背景
/**********************设置刷新全界面参数********************/
#define SetFullInterface()             \
{                                      \
    g_display_bitmap=g_display_bitmap|   \
                    up_musicshuffle |   \
                    up_musicprogress |   \
                    up_musicloopmode |   \
                    up_musiceqmode |     \
                    up_musictimeinfo |   \
                    up_musicinformation| \
                    up_musicplaystatus | \
                    up_musicalbumart |   \
                    up_musictracks |     \
                    up_musicheadbar |    \
                    up_musicBackGDPic;   \
}

/**********************设置刷新切换歌曲界面参数*****************/
#define SetSwitchInterface()             \
{                                        \
    g_display_bitmap=g_display_bitmap|   \
                    up_musicshuffle |   \
                    up_musicprogress |   \
                    up_musictimeinfo |   \
                    up_musicinformation| \
                    up_musicplaystatus | \
                    up_musicalbumart |   \
                    up_musictracks;    \
}

//----------------------设置 播歌状态标志
#define up_ApeNoCue          0x00008000   //没有cue文件的APE歌曲(APE支持CUE使用)
#define up_ApeInCue          0x00004000   //APE歌曲进入cue列表播放(APE支持CUE使用)
#define up_ApeIsNow          0x00002000   //正在播放的歌曲为APE(APE支持CUE使用)
#define up_ScrollDeal        0x00001000   //滚屏状态机调度标志
#define up_NeedOpenPA        0x00000800   //需要打开PA标志
#define up_LrcMulScreen      0x00000400   //歌词多屏显示
#define up_MengineStatus     0x00000200   //引擎状态标志
#define up_SwitchPrev        0x00000100   //歌曲向前切标志
#define up_ScrollStatus      0x00000080   //滚屏定时器状态标志
#define up_ABswitch          0x00000040   //AB复读替换标志
#define up_ShuttleTmporary   0x00000020   //临时随机标志
#define up_Switchflag        0x00000010   //需要切歌标志
#define up_Paintflag         0x00000008   //需要显示标志
#define up_AlbumART          0x00000004   //专辑图片标志
#define up_getfileInfoPATH   0x00000002   //获取文件路径PATH标志
#define up_getfileInfoID3    0x00000001   //获取文件路径ID3标志
//----------------------清除 播放状态标志
#define clr_ApeNoCue         0xffff7fff   //没有cue文件的APE歌曲(APE支持CUE使用)
#define clr_ApeInCue         0xffffbfff   //APE歌曲进入cue列表播放(APE支持CUE使用)
#define clr_ApeIsNow         0xffffdfff   //正在播放的歌曲为APE(APE支持CUE使用)
#define clr_ScrollDeal       0xffffefff   //滚屏状态机调度标志
#define clr_NeedOpenPA       0xfffff7ff   //需要打开PA标志
#define clr_LrcMulScreen     0xfffffbff   //歌词多屏显示
#define clr_MengineStatus    0xfffffdff   //引擎状态标志
#define clr_SwitchPrev       0xfffffeff   //歌曲向前切标志
#define clr_ScrollStatus     0xffffff7f   //滚屏定时器状态标志
#define clr_ABswitch         0xffffffbf   //AB复读替换标志
#define clr_ShuttleTmporary  0xffffffdf   //临时随机标志
#define clr_Switchflag       0xffffffef   //需要切歌标志
#define clr_Paintflag        0xfffffff7   //需要显示标志
#define clr_AlbumART         0xfffffffb   //专辑图片标志
#define clr_getfileInfoPATH  0xfffffffd   //获取文件路径PATH标志
#define clr_getfileInfoID3   0xfffffffe   //获取文件路径ID3标志
//-----------------------------------end

typedef enum
{
    Music_NON,
    Music_MP1,
    Music_MP2,
    Music_MP3,
    Music_WMA,
    Music_WMV,
    Music_AIF,
    Music_APE,
    Music_ASF,
    Music_FLA,
    Music_OGG,
    Music_MPC,
    Music_M4A,
    Music_AAC,
    Music_AA
} music_type_e;

//typedef enum
//{
//    ID3_DEAL_INIT,
//    ID3_DEAL_SCROLL,
//    ID3_DEAL_SWITCH
//} id3_deal_status_e;

//for functions

extern void play_scene_getid3(file_path_info_t* path_info);

extern id3_type_e play_scene_checktype(uint8 * extname);

extern void play_scene_paint(uint32 *display_flag);

extern void play_scene_deal_id3(void);

extern app_result_e _scene_play_guimsg(input_gui_msg_t *gui_msg);

extern app_result_e play_key_prev(mengine_status_t* cur_status);

extern app_result_e play_key_next(mengine_status_t* cur_status);

extern app_result_e play_key_play(mengine_status_t* cur_status);

extern app_result_e play_key_return(void);

extern bool play_scroll_deal(void) __FAR__;

extern void id3_scroll_deal(void) __FAR__;

extern void play_scroll_update(void) __FAR__;

//extern void play_scroll_restart(void);

//extern void _show_now_time(uint8 para, style_infor_t * style);

//extern void _show_progress(uint8 para, style_infor_t * style);

extern void _show_time_progress(uint8 para, style_infor_t * style, uint8 type);

extern app_result_e playing_check_status(file_path_info_t* path_locat, uint8 *prev_status);

extern app_result_e playing_loop_deal(file_path_info_t* path_locat);

extern void music_playlist_update_history(file_path_info_t* path_locat, uint8 dest_type) __FAR__;

#ifdef SUPPORT_ApeCue_SWITCH
extern uint8 cue_shuffle_num(uint8 total, uint8 num) __FAR__;
#endif
extern void play_paint_albumart(album_picinfo_t *album_picinfo);
extern bool set_scene_speed_callback(uint16 speed_value);

#endif //__APP_MUSIC_PLAYING_H__
