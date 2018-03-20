/*******************************************************************************
 *                              us211A
 *                            Module: case
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-10-24 16:20     1.0             build this file 
*******************************************************************************/
/*!
 * \file     vm_def.h
 * \brief    包含各个应用所占VM空间宏定义
 * \author   zhangxs
 * \version 1.0
 * \date  2011-10-24
*******************************************************************************/
//#include "case_include.h"

#ifndef _VM_DEF_H
#define _VM_DEF_H

#include "vm_fwsp_def.h"


#ifndef  _CARD_BASE_

#define VM_AP_SETTING         0x00000000
#define VM_AP_MAINMENU        0x00010000
#define VM_AP_CONFIG          0x00020000//4 
#define VM_AP_UDISK           0x00030000
#define VM_AP_MUSIC           0x00040000
#define VM_AP_RECORD          0x00050000
#define VM_AP_PICTURE         0x00060000  
#define VM_AP_BROWSER         0x00070000
#define VM_AP_EBOOK           0x00080000
#define VM_AP_RADIO           0x00090000
#define VM_AP_MENGINE         0x000A0000
#define VM_AP_ALARM           0x000B0000
#define VM_AP_RECORD_WAV	  0x000C0000//断电恢复的数据记录地址
#define VM_FILE_NUM           0X000D0000
#define VM_AP_FMENGINE        0x000E0000  
#define VM_AP_DAE             0x000F0000  

//#define VM_DEW_PARAM_BASE            0x00180000//13 ~ 14 音效增强和减弱参数区
//#define VM_DEW_PARAM_BASE_REV    0x00190000//13 ~ 14 音效增强和减弱参数区
//#define VM_SPK_COMP_DAT_BASE        0x001A0000//0x20 ~ 0x27 speaker compensation 滤波器参数表

/* AP VRAM 数据区标志 = 0x55AA + ID (0x00 - 0x1f) */
#define  VRAM_MAGIC(vram_addr)  (0x55AA + (short)((vram_addr) >> 16))

//vram逻辑项相对于vram块首的字节偏移
/* 文件浏览路径记录（每次创建播放列表时清除），8KB，0x00000000 - 0x00002000 */
#define  VM_EXT_DIR_HISTORY       0x00000000

/* 菜单路径记录（每次开机后清除），8KB，0x00002000 - 0x00004000 */
#define  VM_EXT_MENU_HISTORY      0x00002000

/* ENHANCED 的歌词显示时的时间戳和内容(显示歌词时占用) ,16KB,0x00004000 - 0x0008000*/
#define  VM_EXT_LYRIC_INFO        0x00004000

/*APE CUE列表歌词标题 ,1KB,0x00008000 - * */
//#define VM_EXT_APE_CUE            0x00008000  

//for album art info 23K, 分配32K，以留作扩展，0x00050000 - 0x00058000
#define  VM_ALBUMART_INFO         0x001B0000


#else      // card base

#define VM_AP_SETTING         0x00000000
#define VM_AP_MAINMENU        0x00000400
#define VM_AP_CONFIG          0x00000800
#define VM_AP_UDISK           0x00000c00
#define VM_AP_MUSIC           0x00001000
#define VM_AP_RECORD          0x00001400
#define VM_AP_PICTURE         0x00001800 
#define VM_AP_BROWSER         0x00001c00
#define VM_AP_EBOOK           0x00002000
#define VM_AP_RADIO           0x00002400
#define VM_AP_MENGINE         0x00002800
#define VM_AP_ALARM           0x00002c00
#define VM_AP_RECORD_WAV	  0x00003000//断电恢复的数据记录地址
#define VM_FILE_NUM           0X00003400
#define VM_AP_FMENGINE        0x00003800  
#define VM_AP_DAE             0x00003c00  

#define VM_DEW_PARAM_BASE        0x00004000//13 ~ 14 音效增强和减弱参数区
#define VM_DEW_PARAM_BASE_REV    0x00004800//13 ~ 14 音效增强和减弱参数区
#define VM_SPK_COMP_DAT_BASE     0x00005000//0x20 ~ 0x27 speaker compensation 滤波器参数表

#define MAGIC_BOOKMARK        0x55AA
#define MAGIC_AUDBK           0x55AA

/* AP VRAM 数据区标志 = 0x55AA + ID (0x00 - 0x1f) */
#define  VRAM_MAGIC(vram_addr)  (0x55AA + (short)((vram_addr) >> 16))

//vram逻辑项相对于vram块首的字节偏移
/* 文件浏览路径记录（每次创建播放列表时清除），8KB，0x00006000 - 0x00008000 */
#define  VM_EXT_DIR_HISTORY       0x00006000

/* 菜单路径记录（每次开机后清除），8KB，0x00008000 - 0x0000a000 */
#define  VM_EXT_MENU_HISTORY      0x00008000

/* ENHANCED 的歌词显示时的时间戳和内容(显示歌词时占用) ,16KB,0x0000a000 - 0x000e000*/
#define  VM_EXT_LYRIC_INFO        0x0000A000

/*APE CUE列表歌词标题 ,1KB,0x0000E000 - * */
//#define VM_EXT_APE_CUE            0x0000E000  

//for album art info 23K, 分配32K，以留作扩展，0x00050000 - 0x00058000
#define  VM_ALBUMART_INFO         0x00010000      

//保存audible sys file 1K
#define VM_AUD_SYS                0x00010400

//保存audible断点信息 0.5K
#define VM_AP_MUSIC_AUDBK         0x00010800


#endif       // #ifndef  _CARD_BASE_


#endif
