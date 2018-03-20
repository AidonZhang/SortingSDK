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
 * \brief    ������д�ļ��ĸ���
 * \author   yanghong
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               Ĭ�ϴ����ļ�ʹ�ó���
 * \version 1.0
 * \date  2015/12/03
 *******************************************************************************/
#include "plist.h"
#include "rdata_ex.h"

//ϵͳĬ�ϴ����ļ�ʹ�ó���
#ifdef USE_83NAME_CREAT
static const uint8 album_pic_name[12]="ALBUM   PIC";
#else
static const uint8 album_pic_name[] = "ALBUM.PIC";
#endif
static const uint8 album_pic_name_short[12]= "ALBUM   PIC";

/******************************************************************************/
/*
 * \par  Description: mbyte to wchar
 *
 * \param[in]    dest��src

 * \param[out]

 * \return  UNICODE ��������������ʶ���ͽ�����

 * \note    ASCת��UNICODE���Զ������ʶ���ͽ�����
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
 * \par  Description: �ڸ�Ŀ¼�´���ר��ͼƬ�ļ�
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
void creat_album_pic(void)
{
    uint32 album_pic = 0;
    uint32 temp32 = 0;
    //if (0 == drv_sn)
    //{
        //nand���ʣ��������VM����,�˴�������
        //return;
    //}

    vfs_cd(ap_vfs_mount, CD_ROOT, 0);
    mbyte_to_wchar(temp_plist_buf, (uint8*) album_pic_name);
    album_pic = vfs_file_open(ap_vfs_mount, temp_plist_buf, R_NORMAL_SEEK);
    if (0 != album_pic)
    {
        vfs_file_get_size(ap_vfs_mount, &temp32, album_pic, 0);
        if (temp32 < ALBUM_PIC_SIZE)
        {
            //�ļ���С���ˣ���Ҫ���´���
            vfs_file_close(ap_vfs_mount, album_pic);
            album_pic = 0;
        }
    }
    
    if (0 == album_pic)
    {
        libc_memcpy(temp_plist_buf, album_pic_name_short, sizeof(album_pic_name_short));
        vfs_file_dir_remove(ap_vfs_mount, temp_plist_buf, FS_FILE);//�������
        mbyte_to_wchar(temp_plist_buf, (uint8*) album_pic_name);
        vfs_file_dir_remove(ap_vfs_mount, temp_plist_buf, FS_FILE);//�������
        
        album_pic = vfs_file_create(ap_vfs_mount, temp_plist_buf, ALBUM_PIC_SIZE);
        if (0 != album_pic)
        {
#ifdef HIDE_FILE_OPT
        vfs_file_attralter(ap_vfs_mount, ATTR_HIDDEN, album_pic, 0);
#endif
            libc_memset(temp_plist_buf, 0, SECTOR_SIZE);
    
            //�ļ�ʶ���
            libc_memcpy(temp_plist_buf, album_pic_name, 5);
            //�����ʶ��
			temp_plist_buf[16] = 'N';
			temp_plist_buf[17] = 'O';
            temp_plist_buf[510] = 0x55;
            temp_plist_buf[511] = 0xAA;
    
            vfs_file_write(ap_vfs_mount, temp_plist_buf, SECTOR_SIZE, album_pic);
            libc_memset(temp_plist_buf, 0, SECTOR_SIZE);
    
            vfs_file_close(ap_vfs_mount, album_pic);
            plt_print(album_pic_name,0,0);
        }
    }
    
    return;
}

