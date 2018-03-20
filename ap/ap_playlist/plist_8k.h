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
 * \brief    ���ݽṹ�ͳ�������,��������8k���������ʱ����
 * \author   liminxian
 * \version 1.0
 * \date  2011/09/15
 *******************************************************************************/

#ifndef _plist_8k_h

#define _plist_8k_h

#include "plist.h"


/*
 *! ram��buffer�ռ����
 */


#define SORT_BUFFER_SIZE_8k            0x7400//0xD400 //53k=54274>9*6000
#define INDEX_BUFFER_SIZE_8k           0x4000//0x3000//12K
#define FILE_INFO_BUF_SIZE_8k          0x2000//8k
#define TRACK_BUFFER_SIZE_8k           0x2000//0X1800//6k track����ռ�


/*
*! creat�ļ���С
*/
#define MUSIC_LIB_SIZE_8k  	           ((PLIST_MUSIC_MAX + PLIST_MUSIC_MAX/4)* SECTOR_SIZE + SORT_BUFFER_SIZE_8k)
#define M3U_LIB_SIZE_8k                (M3U_LIST_SIZE + M3U_DAT_SIZE + SORT_BUFFER_SIZE_8k)


/*
 *!  LIB�ļ��п���ʹ�õĿռ�[64K]
 */
#define STORE_MUSIC_ADDR_8k              ((PLIST_MUSIC_MAX + PLIST_MUSIC_MAX/4) * SECTOR_SIZE)//0x271000
#define TITLE_SORT_INDEX_VM_ADDR_8k      (STORE_MUSIC_ADDR + 0*INDEX_BUFFER_SIZE_8k) 
#define ALBUM_SORT_INDEX_VM_ADDR_8k      (STORE_MUSIC_ADDR + 1*INDEX_BUFFER_SIZE_8k) 
#define ARTIST_SORT_INDEX_VM_ADDR_8k     (STORE_MUSIC_ADDR + 2*INDEX_BUFFER_SIZE_8k) 
#define TIDY_BUF_A_VM_ADDR_8k            (STORE_MUSIC_ADDR + 3*INDEX_BUFFER_SIZE_8k) 
#define TIDY_BUF_B_VM_ADDR_8k            (STORE_MUSIC_ADDR + 4*INDEX_BUFFER_SIZE_8k) 



/*
 *! ɨ���ļ�ʱ��SRAM �Ĵ洢��ַ�ռ�ֲ�  SRAM�ռ�[0x20a00-0x3ffff]125k
 *! TRACK_DATA_ADDR����COUNT_BUF_B_ADDRǰ4K�ռ�,COUNT_BUF_B_ADDR������
 *! artistʱ��ʹ�ã�����������album��ʹ��
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

