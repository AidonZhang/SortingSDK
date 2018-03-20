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
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "plist.h"
#include "plist_8k.h"
#include "plist_16k.h"

#include "rdata_ex.h"

//music playlist
static const char no_artist[] = "No Artist";
static const char no_album[]  = "No Album";
static const char no_genre[]  = "No Genre";

extern id3_type_e check_id3_type(uint32 ext_name);
extern bool unicode_to_char(char *str,uint16 len,uint8 encode);

/******************************************************************************/
/*
 * \par  Description: 转换字符为大写
 *  static void switch_a_to_A(uint8* ext_name)

 * \param[out]
 * \return        void
 * \note
 *******************************************************************************/
static void switch_a_to_A(uint8* ext_name)
{
    ext_name[0] = ((ext_name[0] >= 'a') && (ext_name[0] <= 'z')) ? (ext_name[0] - 'a' + 'A') : ext_name[0];
    ext_name[1] = ((ext_name[1] >= 'a') && (ext_name[1] <= 'z')) ? (ext_name[1] - 'a' + 'A') : ext_name[1];
    ext_name[2] = ((ext_name[2] >= 'a') && (ext_name[2] <= 'z')) ? (ext_name[2] - 'a' + 'A') : ext_name[2];

}

static void deal_if_83name(uint8 *name_buf)
{

#if 1
    uint8 i,j;

    if((name_buf[0]==0xff) && (name_buf[1]==0xfe))
    {
        return;
    }

    //扩展名往后移1个字节
    for(i=11;i>8;i--)
    {
        name_buf[i]=name_buf[i-1];
    }

    //过滤文件名空格
    for(i=7;i>0;i--)
    {
        if(name_buf[i]!=0x20)
        {
            break;
        }
    }

    //加点
    i++;
    name_buf[i]=0x2e; //加"."

    //过滤扩展名空格
    for(j=9;j<12;j++)
    {
        if(name_buf[j]!=0x20)
        {
            i++;
            name_buf[i]= name_buf[j];
        }
    }

    if(name_buf[i]!=0x2e)
    {
        i++;
    }

    name_buf[i]=0x00;

#endif

    return;
}

/******************************************************************************/
/*
 * \par  Description: 对获取到的ID3信息进行处理
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
static void deal_music_id3info(void)
{
    //处理其他的music格式(大于ID3_TYPE_END)，直接获取其文件名信息
    if ((music_f_p->title_string[0] == 0) || (music_f_p->id3_tag_flag == 0))
    {
        //无ID3信息直接读取文件名
        vfs_get_name(ap_vfs_mount, music_f_p->title_string, ID3_TITLE_SIZE / 2);
        deal_if_83name((uint8*) &music_f_p->title_string);
    }

    if ((music_f_p->artist_string[0] == 0) || (music_f_p->id3_tag_flag == 0))
    {
        libc_memcpy(music_f_p->artist_string, no_artist, sizeof(no_artist));
    }

    if ((music_f_p->album_string[0] == 0) || (music_f_p->id3_tag_flag == 0))
    {
        libc_memcpy(music_f_p->album_string, no_album, sizeof(no_album));
    }
    
#if (ID3_GENRE_SUPPORT == 1)
    if ((music_f_p->gener_string[0] == 0) || (music_f_p->id3_tag_flag == 0))
    {
        libc_memcpy(music_f_p->gener_string, no_genre, sizeof(no_genre));
    }
#endif
#if TRACK_SUPPORT

    if (music_f_p->id3_tag_flag == 0)
    {
        music_f_p->track_num = 0;
    }
    else
    {

        if (music_f_p->track_num == UNICODE_FLAG)
        {
            unicode_to_char(track_string_buf, ID3_TRACK_SIZE, language_id);
            music_f_p->track_num = 0;
        }

        if ((music_f_p->track_num == 0) && (track_string_buf[0] != 0))
        {
            uint8 i;
            uint8 j = 1;
            for (i = 0; i < 4; i++)
            {
                if ((track_string_buf[i] == 0) || (track_string_buf[i] == '/'))
                {
                    break;
                }
                if ((track_string_buf[i] < 0x30) || (track_string_buf[i] > 0x39))
                {
                    break;
                }
                else
                {
                    track_string_buf[i] -= 0x30;
                }
            }
            while (i > 0)
            {
                i--;
                music_f_p->track_num += track_string_buf[i] * j;
                j *= 10;
            }

        }

        if (music_f_p->track_num == 0)
        {
            music_f_p->track_num = TRACK_NULL;
        }
    }
#endif

}
/******************************************************************************/
/*
 * \par  Description: 获取文件的信息(ID3,磁盘信息)
 *
 * \param[in]    ext_name--扩展名

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
void get_music_info(uint32 ext_name)
{
    id3_type_e id3_type;

    id3_type = check_id3_type(ext_name);

    if (id3_type < ID3_TYPE_END)
    {
        if (FALSE != get_id3_info(&id3_file_info, NULL, id3_type))
        {
            music_f_p->id3_tag_flag = 1;
            if ((id3_file_info.track_num != 0) && (id3_file_info.track_num != UNICODE_FLAG))
            {
                music_f_p->track_num = id3_file_info.track_num;
            }
        }
        else
        {
            music_f_p->id3_tag_flag = 0;
        }

    }
    else
    {
        music_f_p->id3_tag_flag = 0;
        deal_music_id3info();
    }

    *(uint32*) (music_f_p->filename) = ext_name;
    vfs_file_dir_offset(ap_vfs_mount, &music_f_p->dir_layer_info, &music_f_p->cluster_no, 0);
    music_f_p->flag = 1;

}

/******************************************************************************/
/*
 * \par  Description: 存储信息到RAM或VM区
 *
 * \param[in]    file_num--当前已存放的文件信息总数

 * \param[out]

 * \return

 * \note    先判断buffer是否满了，再写入
 *******************************************************************************/
void store_music_info(uint16 file_num)
{
//libc_print("1",0,0);
    if (file_num != 0)
    {
        title_data_pt  += sort_cmp_len;
        track_data_pt  += TRACK_CMP_LENGTH;//只保留1bytes
        //libc_print("4",0,0);

        //libc_print("info_size",(sorting_map_level_cur->file_info_buf_size),2);
        if ((file_num % ((sorting_map_level_cur->file_info_buf_size) / PLIST_FILE_SIZE)) == 0)
        { //libc_print("1",0,0);
            save_to_file((uint8*) FILE_INFO_ADDR, (sorting_map_level_cur->file_info_buf_size));
            fileinfo_data_pt = (uint8*) FILE_INFO_ADDR;
        }
        else
        {
            fileinfo_data_pt += PLIST_FILE_SIZE;
        }
    }
    
    music_f_p->prev_offset = file_num - 1;
    music_f_p->next_offset = file_num + 1;
    
    //ram中只保存title数据,最大64k空间
    copy_deal_string(title_data_pt, music_f_p->title_string, language_id, sort_cmp_len);
    //libc_print("2",0,0);
 #if TRACK_SUPPORT
 copy_deal_string(music_f_p->track_string, track_string_buf, language_id, 3);
 #endif
  //  libc_print("3",0,0);
    libc_memcpy(fileinfo_data_pt, music_f_p, sizeof(plist_f_info_t));
		
#if TRACK_SUPPORT
   // libc_print("track_allowed",0,0);
    if ((music_f_p->track_num == 0xFFFF) && (music_f_p->track_string[0] != 0x00))
    {
        if ((music_f_p->track_string[0] >= '0') && (music_f_p->track_string[0] <= '9'))
        {
            music_f_p->track_num = music_f_p->track_string[0] - '0';
        }
        if ((music_f_p->track_string[1] >= '0') && (music_f_p->track_string[1] <= '9'))
        {
            music_f_p->track_num = music_f_p->track_num + (music_f_p->track_string[1] - '0') * 10;
        }
    }
    *track_data_pt = (uint8) music_f_p->track_num;//只排序低8位->1byte
#endif
}

/******************************************************************************/
/*
 * \par  Description: 初始化
 *  static void init_id3_size_info(void)
 * \param[in]
 * \param[out]
 * \return        void
 * \note
 *******************************************************************************/
static void init_id3_size_info(void)
{
    uint16 tmp;
    music_f_p = (void*) file_buffer;
    
    id3_file_info.tit2_buffer  = music_f_p->title_string;
    id3_file_info.tpe1_buffer  = music_f_p->artist_string;
    id3_file_info.talb_buffer  = music_f_p->album_string;
    id3_file_info.track_buffer = track_string_buf;//music_f_p->track_string;
#if (ID3_GENRE_SUPPORT == 1)
    id3_file_info.genre_buffer = music_f_p->gener_string;
#else
    id3_file_info.genre_buffer = NULL;
#endif

    id3_file_info.tit2_length  = ID3_TITLE_SIZE;
    id3_file_info.tpe1_length  = ID3_ARTIST_SIZE;
    id3_file_info.talb_length  = ID3_ALBUM_SIZE;
    id3_file_info.genre_length = ID3_GENRE_SIZE;
    id3_file_info.track_length = ID3_TRACK_SIZE;// 3

    id3_file_info.drm_length   = 0;
    id3_file_info.drm_buffer   = 0;
    
    if (ap_plist_file_total != 0)
    {
   // libc_print("file_total",ap_plist_file_total,2);
        tmp = (uint16)((sorting_map_level_cur->sort_buf_size) / ((uint32)ap_plist_file_total));//SORT_BUFFER_SIZE / ap_plist_file_total;
        if(tmp > (uint16)(sorting_map_level_cur->title_cmp_lenth))// TITLE_CMP_LENGTH)
        {
            tmp =(uint16) (sorting_map_level_cur->title_cmp_lenth);//TITLE_CMP_LENGTH;
        }
        ap_plist_file_total = 0;
    }
    else
    {
        tmp = 0;
    }
    
    fsel_scan_flag = 0;//扫描文件标志清零
    sort_cmp_len = (uint8)tmp;
    vfs_cd(ap_vfs_mount, CD_ROOT, 0);//回到根目录
    plt_print("<len>: ", sort_cmp_len, 2);
}

static sorting_map_init(void)
{
	_init_sorting_map_level_4k();
	_init_sorting_map_level_8k();
	_init_sorting_map_level_16k();

}
/******************************************************************************/
/* !
 * \par  Description: 扫描文件
 *  uint16 scan_music_file(void)
 * \param[in]
 * \param[out]
 * \return        文件总数
 * \note
 */
/*******************************************************************************/
uint16 scan_music_file(void)
{
    uint32 temp;
    uint32 ext_name;
    //libc_print("scan0ny04",sys_get_ab_timer(),2);
    sorting_map_init();
#ifdef ENHANCED_FIND_MODE
    while(FALSE!=fsel_get_nextfile((char*)&ext_name))
#else
    while (FALSE != plist_fsel_get_nextfile((char*) &ext_name))
#endif
    {
        ap_plist_file_total++;//获取文件数
        if (ap_plist_file_total >= PLIST_MUSIC_MAX)
        {
            break;
        }
    }
	set_sorting_level();

    
    init_id3_size_info();//初始化
    
#ifdef ENHANCED_FIND_MODE
    while(FALSE!=fsel_get_nextfile((char*)&ext_name))
#else
    while (FALSE != plist_fsel_get_nextfile((char*) &ext_name))
#endif
    {
        //针对exFat文件系统类型 转换当前后缀名(转成大写)
        switch_a_to_A((char*) &ext_name);

        if (0 == (ap_plist_file_total & 0xff))
        {
            plist_get_app_msg();
        }
        
        libc_memset(music_f_p, 0x00, sizeof(plist_f_info_t));
        #if TRACK_SUPPORT
        libc_memset(track_string_buf, 0x00, ID3_TRACK_SIZE);
        #endif
        get_music_info(ext_name);
        //libc_print("$",0,0);
        store_music_info(ap_plist_file_total);
            
        ap_plist_file_total++;
        if (ap_plist_file_total >= PLIST_MUSIC_MAX)
        {
            break;
        }
    }

    if (ap_plist_file_total > 0) {
       // libc_print("#",0,0);      
        temp = (uint32)(ap_plist_file_total % ((sorting_map_level_cur->file_info_buf_size)/PLIST_FILE_SIZE));//(FILE_INFO_BUF_SIZE / PLIST_FILE_SIZE);
        if (temp == 0)
        {
            //刚好写满
            temp = (uint32)((sorting_map_level_cur->file_info_buf_size)/PLIST_FILE_SIZE);//FILE_INFO_BUF_SIZE / PLIST_FILE_SIZE;
        }
        save_to_file((uint8*) (sorting_map_level_cur->file_info_addr), temp * PLIST_FILE_SIZE);
    }

    //plist_get_app_msg();
    
    return ap_plist_file_total;

}

