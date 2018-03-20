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
 * \file     *.c
 * \brief    这里填写文件的概述
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               默认创建文件使用长名
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/
#include "plist.h"
#include "rdata_ex.h"

//系统默认创建文件使用长名
static const uint8 BOOKMARK_NAME_SHORT[12]= "MUSICBMKBMK";

#ifdef USE_83NAME_CREAT
static const uint8 BOOKMARK_NAME[12] = "MUSICBMKBMK";
#else
static const uint8 BOOKMARK_NAME[] = "MUSICBMK.BMK";
#endif

uint32 g_bookmark_fp _BANK_DATA_ATTR_;

/******************************************************************************/
/*
 * \par  Description: mbyte to wchar
 *
 * \param[in]    dest，src

 * \param[out]

 * \return  UNICODE 个数，不包含标识符和结束符

 * \note    ASC转成UNICODE，自动加入标识符和结束符
 *******************************************************************************/
static uint16 mbyte_to_wchar(uint8 *dest, uint8 *src)
{
    uint16 num = 0;

#ifdef USE_83NAME_CREAT

    while(*src!=0x00)
    {
        *dest=*src;
        dest++;
        src++;
        num++;
    }

    dest=0x00;

#else

    *dest = 0xff;
    *(dest + 1) = 0xfe;
    dest += 2;

    while (*src != 0x00)
    {
        *dest = *src;
        *(dest + 1) = 0x00;
        dest += 2;
        src++;
        num++;
    }

    *dest = 0x00;
    *(dest + 1) = 0x00;

#endif

    return num;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \void bool check_bookmark_exit_sub(void)
 * \检测书签文件是否存在
 * \param[in]   none
 * \param[out]  none
 * \return      none
 * \retval
 * \ingroup     music_bookmark
 * \note
 * \    对于要写入VM的书签文件，如果判断不存在，立即创建。对于写入
        卡盘的书签文件，如果文件不存在，则返回FALSE,由接下来的程序创建之 
 *******************************************************************************/
static bool check_bookmark_exit_sub(void)
{
    music_bm_head *bm_head_ptr;
    bool result = TRUE;

    //定位到根目录
    vfs_cd(ap_vfs_mount, CD_ROOT, 0);
    mbyte_to_wchar(temp_plist_buf, (uint8 *) BOOKMARK_NAME);        
    //打开文件
    g_bookmark_fp = vfs_file_open(ap_vfs_mount, temp_plist_buf, R_NORMAL_SEEK);
    if (g_bookmark_fp == 0)
    {
        result = FALSE;
    }
    else
    {
        vfs_file_read(ap_vfs_mount, temp_plist_buf, SECTOR_SIZE, g_bookmark_fp);
        vfs_file_close(ap_vfs_mount, g_bookmark_fp);
        bm_head_ptr = (music_bm_head *) temp_plist_buf;
        if (bm_head_ptr->magic != MAGIC_BOOKMARK)
        {
            result = FALSE;
        }
    }
    
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *  bool check_bookmark_exist(void)
 *	  检测书签文件手机否存在，如不存在，则会创建文件
 * \param[in]   handle *fp 如果打开成功，返回打开的句柄
 * \param[out] none
 * \return     bool
 * \retval     TRUE 文件存在 or 创建成功
 * \ingroup    music_bookmark
 * \note
 *******************************************************************************/
bool check_bookmark_exist(void)
{
    uint32 bm_fp;
    uint32 file_len;
    uint16 i;
    uint16 bound_val;
    music_bm_head *bm_head_ptr;
    bool retval = TRUE;

    if (FALSE == check_bookmark_exit_sub())
    {
        //打开文件失败
        vfs_file_dir_remove(ap_vfs_mount, temp_plist_buf, FS_FILE);//删除长名文件
        vfs_file_dir_remove(ap_vfs_mount, BOOKMARK_NAME_SHORT, FS_FILE);//删除短名文件
        
        bound_val = (BM_HEADER_SECTOR_NUM + BM_DATA_SECTOR_NUM);
        file_len = (bound_val << 9);
        //创建文件
        bm_fp = vfs_file_create(ap_vfs_mount, temp_plist_buf, file_len);
        if (0 == bm_fp)
        {
            //error to create file
            retval = FALSE;
        }
        else
        {
#ifdef HIDE_FILE_OPT        
            vfs_file_attralter(ap_vfs_mount, ATTR_HIDDEN, bm_fp, 0);
#endif            
            libc_memset(temp_plist_buf, 0xff, SECTOR_SIZE);
            bm_head_ptr = (music_bm_head *) temp_plist_buf;
            bm_head_ptr->magic = MAGIC_BOOKMARK;
            vfs_file_write(ap_vfs_mount, temp_plist_buf, SECTOR_SIZE, bm_fp);
            bm_head_ptr->magic = 0xffff;
            for (i = 1; i < (BM_HEADER_SECTOR_NUM + BM_DATA_SECTOR_NUM); i++)
            {
                vfs_file_write(ap_vfs_mount, temp_plist_buf, SECTOR_SIZE, bm_fp);
            }
            vfs_file_close(ap_vfs_mount, bm_fp);
        }
    }

    return retval;
}


