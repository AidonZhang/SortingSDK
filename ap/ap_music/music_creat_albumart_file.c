/*******************************************************************************
 *                              US212A
 *                            Module: PLAYLSIT
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       yanghong  2015-12-03 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     *.c
 * \brief    这里填写文件的概述
 * \author   yanghong
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               默认创建文件使用长名
 * \version 1.0
 * \date  2015/12/03
 *******************************************************************************/
#include "app_music.h"
#include "music_common.h"
#include "app_music_albumlist.h"


//系统默认创建文件使用长名
#ifdef USE_83NAME_CREAT
static const uint8 album_pic_name[12]="ALBUM   PIC";
#else
static const uint8 album_pic_name[] = "ALBUM.PIC";
#endif
static const uint8 album_pic_name_short[12] = "ALBUM   PIC";

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
/*
 * \par  Description: 检测专辑图片文件是否存在，不存在则创建
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/

static bool check_albumart_file_exist_sub(void)
{
    handle albumart_p;
    bool result = TRUE;
    //定位到根目录
    vfs_cd(g_music_mount_id, CD_ROOT, 0);
    //打开文件
    mbyte_to_wchar(bm_buf, (uint8 *) album_pic_name);
    albumart_p = vfs_file_open(g_music_mount_id, bm_buf, R_NORMAL_SEEK);
    if (0 == albumart_p)
    {
        result = FALSE;
    }
    else
    {
        vfs_file_close(g_music_mount_id, albumart_p);
    }

    return result;
}

/******************************************************************************/
/*
 * \par  Description: 在根目录下创建专辑图片文件
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
void check_albumart_file_exist(void)
{
    handle albumart_p;
    
    if (FALSE == check_albumart_file_exist_sub())
    {
        //文件不存在
        vfs_cd(g_music_mount_id, CD_ROOT, 0);
        libc_memcpy(bm_buf, album_pic_name_short, sizeof(album_pic_name_short));
        vfs_file_dir_remove(g_music_mount_id, bm_buf, FS_FILE);//清除短名
        mbyte_to_wchar(bm_buf, (uint8*) album_pic_name);
        vfs_file_dir_remove(g_music_mount_id, bm_buf, FS_FILE);//清除长名
        albumart_p = vfs_file_create(g_music_mount_id, bm_buf, ALBUM_PIC_SIZE);
        if (albumart_p != 0)
        {
#ifdef HIDE_FILE_OPT
            vfs_file_attralter(g_music_mount_id, ATTR_HIDDEN, albumart_p, 0);
#endif
            libc_memset(bm_buf, 0, SECTOR_SIZE);

            //文件识别符
            libc_memcpy(bm_buf, album_pic_name, 5);
            //加入标识符
            bm_buf[16] = 'N';
            bm_buf[17] = 'O';
            bm_buf[510] = 0x55;
            bm_buf[511] = 0xAA;

            vfs_file_write(g_music_mount_id, bm_buf, SECTOR_SIZE, albumart_p);
            libc_memset(bm_buf, 0, SECTOR_SIZE);

            vfs_file_close(g_music_mount_id, albumart_p);
            music_print(album_pic_name,0,0);
        }
    }

    return;
}
