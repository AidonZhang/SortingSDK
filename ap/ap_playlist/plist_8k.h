/*******************************************************************************
 *                              US212A
 *                            Module: PLAYLSIT
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     en_lrc.h
 * \brief    数据结构和常量定义,仅服务于8k歌曲排序的时候用
 * \author   liminxian
 * \version 1.0
 * \date  2011/09/15
 *******************************************************************************/

#ifndef _plist_8k_h

#define _plist_8k_h

#include "plist.h"


/*
 *! ram中buffer空间分配
 */


#define SORT_BUFFER_SIZE_8k            0x7400//0xD400 //53k=54274>9*6000
#define INDEX_BUFFER_SIZE_8k           0x4000//0x3000//12K
#define FILE_INFO_BUF_SIZE_8k          0x2000//8k
#define TRACK_BUFFER_SIZE_8k           0x2000//0X1800//6k track排序空间


/*
*! creat文件大小
*/
#define MUSIC_LIB_SIZE_8k  	           ((PLIST_MUSIC_MAX + PLIST_MUSIC_MAX/4)* SECTOR_SIZE + SORT_BUFFER_SIZE_8k)
#define M3U_LIB_SIZE_8k                (M3U_LIST_SIZE + M3U_DAT_SIZE + SORT_BUFFER_SIZE_8k)


/*
 *!  LIB文件中可以使用的空间[64K]
 */
#define STORE_MUSIC_ADDR_8k              ((PLIST_MUSIC_MAX + PLIST_MUSIC_MAX/4) * SECTOR_SIZE)//0x271000
#define TITLE_SORT_INDEX_VM_ADDR_8k      (STORE_MUSIC_ADDR + 0*INDEX_BUFFER_SIZE_8k) 
#define ALBUM_SORT_INDEX_VM_ADDR_8k      (STORE_MUSIC_ADDR + 1*INDEX_BUFFER_SIZE_8k) 
#define ARTIST_SORT_INDEX_VM_ADDR_8k     (STORE_MUSIC_ADDR + 2*INDEX_BUFFER_SIZE_8k) 
#define TIDY_BUF_A_VM_ADDR_8k            (STORE_MUSIC_ADDR + 3*INDEX_BUFFER_SIZE_8k) 
#define TIDY_BUF_B_VM_ADDR_8k            (STORE_MUSIC_ADDR + 4*INDEX_BUFFER_SIZE_8k) 



/*
 *! 扫描文件时，SRAM 的存储地址空间分布  SRAM空间[0x20a00-0x3ffff]125k
 *! TRACK_DATA_ADDR复用COUNT_BUF_B_ADDR前4K空间,COUNT_BUF_B_ADDR在排序
 *! artist时才使用，可以先在排album中使用
 */


#define FILE_INFO_ADDR_8k              (PLIST_DATA_BASE_ADDR + 0x0000)//8K
#define SORT_DATA_ADDR_8k              (SYS_SRAM_TOP_ADDRESS - SORT_BUFFER_SIZE_8k)

#define FILE_INDEX_BUF_ADDR_8k        (PLIST_DATA_BASE_ADDR + 0*INDEX_BUFFER_SIZE_8k)
#define SORT_INDEX_BUF_ADDR_8k        (PLIST_DATA_BASE_ADDR + 1*INDEX_BUFFER_SIZE_8k)
#define TIDY_BUF_A_ADDR_8k            (PLIST_DATA_BASE_ADDR + 2*INDEX_BUFFER_SIZE_8k)
#define COUNT_BUF_A_ADDR_8k           (PLIST_DATA_BASE_ADDR + 3*INDEX_BUFFER_SIZE_8k)
#define TIDY_BUF_B_ADDR_8k            (PLIST_DATA_BASE_ADDR + 4*INDEX_BUFFER_SIZE_8k)
#define COUNT_BUF_B_ADDR_8k           (PLIST_DATA_BASE_ADDR + 5*INDEX_BUFFER_SIZE_8k)
#define TRACK_DATA_ADDR_8k            (COUNT_BUF_B_ADDR + TRACK_BUFFER_SIZE_8k)




#endif

