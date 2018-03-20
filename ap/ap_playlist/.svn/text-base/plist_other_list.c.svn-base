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
#include "rdata_ex.h"

static const uint16 others_file_max[PL_LIB_MAX] =
{ 0, 0, M3U_LIST_MAX };

static void sort_all_file(char *str_buf);
static void get_others_info(uint32 ext_name);
static void store_others_info(uint16 file_num);
extern void init_file_index(void);
extern void build_ex_index(uint16 *ex_buf);
extern bool list_resort(uint16* index_buf, uint16 index_num, char* str_buf, plist_type_e type, uint8 resort_len, uint8 len);

//过滤空格,转换为 xx.yyy格式
//返回字符长度，不包含结束符
static void deal_if_83name(uint8 *nbuf)
{
    uint8 i, j;
    uint8 *name_buf = nbuf;

    if ((name_buf[0] == 0xff) && (name_buf[1] == 0xfe))
    {
        return;
    }

    //扩展名往后移1个字节
    for (i = 11; i > 8; i--)
    {
        name_buf[i] = name_buf[i - 1];
    }

    //过滤文件名空格
    for (i = 7; i > 0; i--)
    {
        if (name_buf[i] != 0x20)
        {
            break;
        }
    }

    //加点
    i++;
    name_buf[i] = 0x2e; //加"."

    //过滤扩展名空格
    for (j = 9; j < 12; j++)
    {
        if (name_buf[j] != 0x20)
        {
            i++;
            name_buf[i] = name_buf[j];
        }
    }

    if (name_buf[i] != 0x2e)
    {
        i++;
    }

    name_buf[i] = 0x00;

    return;
}

/******************************************************************************/
/*
 * \par  Description: 获取找到的文件信息(路径，扩展名等)
 *
 * \param[in]    ext_name--扩展名

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
static void get_others_info(uint32 ext_name)
{
    uint8 i, len;
    len = (uint8) vfs_get_name(ap_vfs_mount, other_f_p->title_string, (ID3_TITLE_SIZE) / 2);

    if (lib_num == 2)
    {
        //m3u.lib
        //m3u列表文件不显示后缀名
        if ((uint8) other_f_p->title_string[0] != 0xff)
        {
            //短名
            other_f_p->title_string[8] = 0x20;
            other_f_p->title_string[9] = 0x20;
            other_f_p->title_string[10] = 0x20;
        }
        else
        {
            //长名
            len = (len - 1) << 1;
            for (i = len - 8; i < len; i = i + 2)
            {
                if ((uint8) other_f_p->title_string[i] == 0x2e)
                {
                    other_f_p->title_string[i] = 0x00;
                    break;
                }
            }
        }

    }

    deal_if_83name(other_f_p->title_string);

    *(uint32*) (other_f_p->filename) = ext_name;
    vfs_file_dir_offset(ap_vfs_mount, &other_f_p->dir_layer_info, &other_f_p->cluster_no, 0);
    other_f_p->flag = 1;
    return;
}
/******************************************************************************/
/*
 * \par  Description: 缓存整理好的文件信息，排序信息
 *
 * \param[in]    file_num--当前已存放的文件个数

 * \param[out]

 * \return

 * \note        先判断buffer是否满了，再写入
 *******************************************************************************/
static void store_others_info(uint16 file_num)
{
    if (file_num != 0)
    {
        //64K空间排4000*16字节足够,不用写文件
        title_data_pt += sort_cmp_len;
  
        if ((file_num % (FILE_INFO_BUF_SIZE / PLIST_FILE_SIZE)) == 0)
        {
            save_to_file((uint8*) FILE_INFO_ADDR, FILE_INFO_BUF_SIZE);
            fileinfo_data_pt = (uint8*) FILE_INFO_ADDR;
        }
        else
        {

            fileinfo_data_pt += PLIST_FILE_SIZE;

        }
    }

    other_f_p->prev_offset = file_num - 1;
    other_f_p->next_offset = file_num + 1;

    copy_deal_string(title_data_pt, other_f_p->title_string, language_id, sort_cmp_len);
    libc_memcpy(fileinfo_data_pt, other_f_p, sizeof(plist_f_info_t));

    return;
}
/******************************************************************************/
/*
 * \par  Description: 初始化
 *  static void init_other_size_info(void)
 * \param[in]
 * \param[out]
 * \return        void
 * \note
 *******************************************************************************/
static void init_other_size_info(void)
{
    uint16 tmp;
    other_f_p = (plist_f_info_t *) file_buffer;

    if (ap_plist_file_total != 0)
    {
        tmp = SORT_BUFFER_SIZE / ap_plist_file_total;
        if(tmp > ID3_TITLE_SIZE)
        {
            tmp = ID3_TITLE_SIZE;
        }
        ap_plist_file_total = 0;
    }
    else
    {
        tmp = 0;
    }
    
    sort_cmp_len = (uint8)tmp;
    plt_print("<len>: ", sort_cmp_len, 2);
}

/******************************************************************************/
/*
 * \par  Description:扫描 磁盘上的文件
 *
 * \param[in]

 * \param[out]

 * \return        文件的总数

 * \note
 *******************************************************************************/
uint16 scan_others_info(void)
{
    uint32 temp;
    uint32 ext_name;
    //    uint16 music_num;

#ifdef ENHANCED_FIND_MODE
    while(FALSE!=fsel_get_nextfile((char*)&ext_name))
#else
    while (FALSE != plist_fsel_get_nextfile((char*) &ext_name))
#endif
    {
        
        ap_plist_file_total++;//获取文件数
        if (ap_plist_file_total >= others_file_max[lib_num])
        {
            break;
        }
    }

    plist_get_app_msg();
    init_other_size_info();//初始化
    vfs_cd(ap_vfs_mount, CD_ROOT, 0);//回到根目录

#ifdef ENHANCED_FIND_MODE
    while(FALSE!=fsel_get_nextfile((char*)&ext_name))
#else
    while (FALSE != plist_fsel_get_nextfile((char*) &ext_name))
#endif
    {
        if (0 == (ap_plist_file_total & 0x07))
        {
            plist_get_app_msg();
        }
        
        libc_memset(other_f_p, 0x00, sizeof(plist_f_info_t));
        
        get_others_info(ext_name);
        
        store_others_info(ap_plist_file_total);

        ap_plist_file_total++;
        if (ap_plist_file_total >= others_file_max[lib_num])
        {
            break;
        }
    }

    if (ap_plist_file_total > 0)
    {
        temp = ap_plist_file_total % (FILE_INFO_BUF_SIZE / PLIST_FILE_SIZE);
        if (temp == 0)
        {
            temp = FILE_INFO_BUF_SIZE / PLIST_FILE_SIZE;
        }
        save_to_file((uint8 *) FILE_INFO_ADDR, temp * PLIST_FILE_SIZE);
    }
    
    plist_get_app_msg();
    return ap_plist_file_total;
}

/******************************************************************************/
/*
 * \par  Description: 排序列表
 *
 * \param[in]

 * \param[out]

 * \return        NULL

 * \note
 *******************************************************************************/
void sort_others_list(void)
{
    uint8 *str_buf;

    //clear buffer
    libc_memset(file_index_buf, 0, FILE_INFO_BUF_SIZE);

    sort_file_total = ap_plist_file_total;

    plist_get_app_msg();

    str_buf = (uint8 *) SORT_DATA_ADDR;//64K

    sort_all_file(str_buf);
    plist_get_app_msg();

}

/******************************************************************************/
/*
 * \par  Description: 排序文件名
 *
 * \param[in]

 * \param[out]

 * \return

 * \note
 *******************************************************************************/
static void sort_all_file(char *str_buf)
{
    uint8 str_len;
    
    init_file_index();
    plist_get_app_msg();
    str_len = sort_cmp_len;
    if (str_len >ID3_TITLE_SIZE)
    {
        str_len = ID3_TITLE_SIZE;
    }
    list_sort((void*) file_index_buf, sort_file_total, 2, str_buf, SORT_CMP_STRING, str_len); //64+8k
    plist_get_app_msg();
    list_resort(file_index_buf, sort_file_total, str_buf, PLIST_TITLE, ID3_TITLE_SIZE, str_len);//resort
    //必须在title排序后才能请buffer，因为和title的数据区复用
    libc_memset(sort_index_buf, 0, INDEX_BUFFER_SIZE);

    plist_get_app_msg();
    plist_reflash_progress(-1);
    
    build_ex_index(sort_index_buf); //8+8k

    ap_file_index_offset[0] = ap_pos_file_offset;
    save_to_file((void*) file_index_buf, sort_file_total * 2);

    ap_sort_index_offset[0] = ap_pos_file_offset;
    save_to_file((void*) sort_index_buf, sort_file_total * 2);

    return;
}

