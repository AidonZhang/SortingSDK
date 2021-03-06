/*******************************************************************************
 *                              US212A
 *                            Module: PLAYLSIT
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       younghonor  2015-9-16 9:15    1.0             build this file
 *******************************************************************************/
/*!
 * \file     plist_sort_music_ex.c
 * \brief    这里填写文件的概述
 * \author   younghonor
 * \par      GENERAL DESCRIPTION:
 *               对歌曲TITLE进行256字节全排序
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date  2015/9/16
 *******************************************************************************/

#include "plist.h"
#include "plist_8k.h"
#include "plist_16k.h"

#include "rdata_ex.h"

#ifdef DEBUG_PRINT
void debug_printf(plist_type_e type)
{
     char* file_info = temp_plist_buf + PLIST_FILE_SIZE;//copy_deal_string中使用temp_plist_buf前256
     char* data = file_buffer;
     uint32 file_offs, tag_addr, tag_size;
     uint16 i;
 
     switch (type)
     {
         case PLIST_TITLE:
         tag_addr = ID3_TITLE_OFFSET;
         tag_size = ID3_TITLE_SIZE;
         break;
         case PLIST_ARTIST:
         tag_addr = ID3_ARTIST_OFFSET;
         tag_size = ID3_ARTIST_SIZE;
         break;
         case PLIST_ALBUM:
         tag_addr = ID3_ALBUM_OFFSET;
         tag_size = ID3_ALBUM_SIZE;
         break;
    #if (ID3_GENRE_SUPPORT == 1)
         case PLIST_GENRE:
         tag_addr = ID3_GENRE_OFFSET;
         tag_size = ID3_GENRE_SIZE;
         break;
    #endif
         default:
         break;
     }
     //print the ID3 type
     libc_print("<ID3 TYPE> ", type, 2);
     for (i = 0; i< sort_file_total; i++)
     {
         file_offs = SECTOR_SIZE + file_index_buf[i]*PLIST_FILE_SIZE;
         vfs_file_seek(ap_vfs_mount, file_offs, SEEK_SET, ap_obj_handle);
         vfs_file_read(ap_vfs_mount, file_info, PLIST_FILE_SIZE, ap_obj_handle);
         copy_deal_string(data, file_info+tag_addr, language_id, tag_size);//按照语言拷贝
         libc_print(data, 0, 0);
     }

     //重新seek到之前的文件偏移地址
     file_offs = ap_pos_file_offset * SECTOR_SIZE;
     vfs_file_seek(ap_vfs_mount, file_offs, SEEK_SET, ap_obj_handle);
}
#endif
	void _init_sorting_map_level_8k(void)
 {
			 sorting_map_level_8k.sort_buf_size 				= (uint32)SORT_BUFFER_SIZE_8k ;
			 sorting_map_level_8k.index_buf_size				= (uint16)INDEX_BUFFER_SIZE_8k;
			 sorting_map_level_8k.file_info_buf_size		= (uint16)FILE_INFO_BUF_SIZE_8k;
			 sorting_map_level_8k.track_buf_size				= (uint16)TRACK_BUFFER_SIZE_8k ;
		 
			 sorting_map_level_8k.title_cmp_lenth 			= (uint8)TITLE_CMP_LENGTH;
			 sorting_map_level_8k.album_cmp_lenth 			= (uint8)ALBUM_CMP_LENGTH;
			 sorting_map_level_8k.artist_cmp_lenth			= (uint8) ARTIST_CMP_LENGTH;
			 sorting_map_level_8k.genre_cmp_lenth 			= (uint8)GENRE_CMP_LENGTH;
			 sorting_map_level_8k.track_cmp_lenth 			= (uint8)TRACK_CMP_LENGTH;
		 
			 sorting_map_level_8k.music_lib_size				= (uint32)MUSIC_LIB_SIZE_8k 	;
			 sorting_map_level_8k.audible_lib_size			= (uint32)AUDIBLE_LIB_SIZE;
			 sorting_map_level_8k.m3u_lib_size					= (uint32)M3U_LIB_SIZE_8k;
			 sorting_map_level_8k.userpl_pl_size				= (uint32)USERPL_PL_SIZE;
			 sorting_map_level_8k.album_pic_size				= (uint32)ALBUM_PIC_SIZE;
		 
			 sorting_map_level_8k.store_music_addr						 = (uint32)STORE_MUSIC_ADDR_8k;
			 sorting_map_level_8k.title_sort_index_vm_addr		 =(uint32)TITLE_SORT_INDEX_VM_ADDR_8k ;
			 sorting_map_level_8k.album_sort_index_vm_addr		 = (uint32)ALBUM_SORT_INDEX_VM_ADDR_8k;
			 sorting_map_level_8k.artist_sort_index_vm_addr 	 = (uint32)ARTIST_SORT_INDEX_VM_ADDR_8k;
			 sorting_map_level_8k.tidy_buf_a_vm_addr					 = (uint32)TIDY_BUF_A_VM_ADDR_8k;
			 sorting_map_level_8k.tidy_buf_b_vm_addr					 = (uint32)TIDY_BUF_B_VM_ADDR_8k;
		 
			 sorting_map_level_8k.store_others_addr 					 = (uint32)STORE_OTHERS_ADDR;
			 sorting_map_level_8k.others_title_addr 					 = (uint32)OTHERS_TITLE_ADDR;
		 
			 sorting_map_level_8k.plist_data_base_addr				 = (uint32)PLIST_DATA_BASE_ADDR;
			 sorting_map_level_8k.plist_data_top_addr 				 = (uint32)PLIST_DATA_TOP_ADDR ;
		 
		 
			 sorting_map_level_8k.file_info_addr							 = (uint32)FILE_INFO_ADDR_8k;
			 sorting_map_level_8k.sort_data_addr							 = (uint32)SORT_DATA_ADDR_8k;
		 
			 
			 sorting_map_level_8k.file_index_buf_addr 				 = (uint32)FILE_INDEX_BUF_ADDR_8k;
			 sorting_map_level_8k.sort_index_buf_addr 				 = (uint32)SORT_INDEX_BUF_ADDR_8k;
			 sorting_map_level_8k.tidy_buf_a_addr 						 = (uint32)TIDY_BUF_A_ADDR_8k;
			 sorting_map_level_8k.count_buf_a_addr						 = (uint32)COUNT_BUF_A_ADDR_8k;
			 sorting_map_level_8k.tidy_buf_b_addr 						 = (uint32)TIDY_BUF_B_ADDR_8k;
			 sorting_map_level_8k.count_buf_b_addr						 = (uint32)COUNT_BUF_B_ADDR_8k;
			 sorting_map_level_8k.track_data_addr 						 = (uint32) TRACK_DATA_ADDR_8k;
			 //sorting_map_level_8k.count_buf_c_addr						 = (uint32) COUNT_BUF_C_ADDR_// ;
 
 }

 void _init_sorting_map_level_16k(void)
{
	   sorting_map_level_16k.sort_buf_size				 = (uint32)SORT_BUFFER_SIZE_16k ;
			sorting_map_level_16k.file_info_buf_size 	 = (uint16)FILE_INFO_BUF_SIZE_16k;
			
			sorting_map_level_16k.title_cmp_lenth			 = (uint8)TITLE_CMP_LENGTH;
			sorting_map_level_16k.album_cmp_lenth			 = (uint8)ALBUM_CMP_LENGTH;
			
			sorting_map_level_16k.file_info_addr               = (uint32)FILE_INFO_ADDR_16k;
			sorting_map_level_16k.sort_data_addr               = (uint32)SORT_DATA_ADDR_16k;
				
			sorting_map_level_16k.file_index_buf_addr          = (uint32)FILE_INDEX_BUF_ADDR_16k;
			sorting_map_level_16k.sort_index_buf_addr          = (uint32)COUNT_BUF_C_ADDR_16k;

			sorting_map_level_16k.count_buf_a_addr             = (uint32)COUNT_BUF_A_ADDR_16k;
			sorting_map_level_16k.count_buf_b_addr             = (uint32)COUNT_BUF_B_ADDR_16k;
			sorting_map_level_16k.count_buf_c_addr             = (uint32) COUNT_BUF_C_ADDR_16k ;
			sorting_map_level_16k.track_data_addr              = (uint32) TRACK_DATA_ADDR_16k;
 return ;
}

/******************************************************************************/
/*
 * \par  Description: 读取title的某一段长度
 *
 * \param[in]   index_buf      --数据索引
                index_num      --数据段数
                str_buf        --保存数据的起始地址
                str_offset_len --偏移的长度
                str_len        --读取的长度
 * \param[out]

 * \return      TRUE

 * \note
 *******************************************************************************/
bool read_string(uint16* index_buf, uint16 index_num, char* buf, uint8 offset, uint8 len, plist_type_e type)
{
    char* file_info = temp_plist_buf + PLIST_FILE_SIZE;//copy_deal_string中使用temp_plist_buf前256bytes
    char* data = temp_plist_buf;
    uint32 file_offs, tag_addr, tag_size;
    uint32 num;
    uint16 i;

    switch (type)
    {
        case PLIST_TITLE:
        tag_addr = ID3_TITLE_OFFSET;
        tag_size = ID3_TITLE_SIZE;
        break;
        case PLIST_ARTIST:
        tag_addr = ID3_ARTIST_OFFSET;
        tag_size = ID3_ARTIST_SIZE;
        break;
        case PLIST_ALBUM:
        tag_addr = ID3_ALBUM_OFFSET;
        tag_size = ID3_ALBUM_SIZE;
        break;
    #if (ID3_GENRE_SUPPORT == 1)
        case PLIST_GENRE:
        tag_addr = ID3_GENRE_OFFSET;
        tag_size = ID3_GENRE_SIZE;
        break;
    #endif
        default:
        break;
    }
    
    for (i = 0, num = 0; i < index_num; i++)
    {
        file_offs = SECTOR_SIZE + index_buf[i] * PLIST_FILE_SIZE + tag_addr;
                
        vfs_file_seek(ap_vfs_mount, file_offs, SEEK_SET, ap_obj_handle);
        vfs_file_read(ap_vfs_mount, file_info, tag_size, ap_obj_handle);

        /*
         *因为data使用的空间与copy_deal_string使用的是相同的temp_plist_buf
         *故不需要再copy一次,dest参数传NULL即可
         */
        copy_deal_string(NULL, file_info, language_id, tag_size);//按照语言拷贝
        libc_memcpy(&buf[num], data + offset, (uint32)len);
        
        num += len;
    }

    //重新seek到之前的文件偏移地址
    file_offs = ap_pos_file_offset * SECTOR_SIZE;
    vfs_file_seek(ap_vfs_mount, file_offs, SEEK_SET, ap_obj_handle);

    return TRUE;
}
/******************************************************************************/
/*
 * \par  Description: 256字节全排序歌曲title
 *
 * \param[in]   index_buf --序列的起始地址
 index_num --序列的数目
 str_buf   --读取的title前16字节保存地址
 str_len   --title段的长度,不能大于CMPSTR_MAX_BUF
 * \param[out]
 * \return true
 * \note
 *******************************************************************************/
bool list_resort(uint16* index_buf, uint16 index_num, char* str_buf, plist_type_e type, uint8 resort_len, uint8 len)
{
    uint16 *sub_index_buf = (uint16*)(sorting_map_level_cur->count_buf_a_addr);//复用
    uint8  *offsets_len = (uint8*)(sorting_map_level_cur->count_buf_b_addr);//复用1/2 COUNT_BUF_B_ADDR
    
    uint16 i, j, k, n;
    uint16 sub_str_len = 0;
    bool resort_flag = FALSE;
    
    if (len < resort_len || (type != PLIST_TITLE))
    {
        //查找str_len长度中不相同的段
        sort_depend_buf = str_buf;
        resort_data[0] = 0;
        offsets_len[index_num-1] = len;//不需要排序
        
        for (i = 1, n = 0; i < index_num; i++)
        {
            if (0 != compare_string(&index_buf[i], &index_buf[i - 1], len))
            {
                //不相等     
                 n = 0;
            }
            else
            {
                n++;
            }

            resort_data[i] = n;
            offsets_len[i-1] = len;//不需要排序
        } 
    }
    
    plist_get_app_msg();
    
    if (len >= resort_len)
    {
        return TRUE;
    }
   
    resort:
    //读title排序相同的title
    for (i = 0; i < index_num; i++)
    {
        if (resort_data[i] != 0)
        {
            continue;
        }

        n = 1;

        while (((i + n) < index_num) && (resort_data[i + n] != 0))
        {
            n++;
        }

        if (n < 2)
        {
            continue;
        }

        //求取排序长度
        sub_str_len = (sorting_map_level_cur->sort_buf_size) / n;

        if (sub_str_len > (resort_len - offsets_len[i]))
        {
            sub_str_len = resort_len - offsets_len[i];
        }

        //不能大于CMPSTR_MAX_BUF
        if (sub_str_len > CMPSTR_MAX_BUF)
        {
            sub_str_len = CMPSTR_MAX_BUF;
        }

        if ((offsets_len[i] >= resort_len) || (sub_str_len == 0))
        {
            i += n - 1;
            continue;
        }
        //排序长度合理，偏移未越界，继续
        plt_print("len:", sub_str_len, 2);
        plt_print("off:", offsets_len[i], 2);
        plt_print("i:", i, 2);
        plt_print("n:", n, 2);

        //获取n段数据，偏移offsets_len[i]，长度sub_str_len
        read_string(&index_buf[i], n, str_buf, offsets_len[i], (uint8)sub_str_len, type);
        
        for (j = 0; j < n; j++)
        {
            sub_index_buf[j] = j;//初始化
        }
        
        //重排序,索引信息保存在sub_index_buf
        list_sort((char*)sub_index_buf, n, 2, str_buf, SORT_CMP_STRING, (uint8)sub_str_len);

        //偏移量增加
        for (j = i; j < (i + n); j++)
        {
            offsets_len[j] += sub_str_len;
        }

        //查找重排后的title段是否还有相同的title
        resort_data[i] = 0;
        for (j = 1, k = 0; j < n; j++)
        {
            if (0 != compare_string(&sub_index_buf[j], &sub_index_buf[j - 1], (uint8)sub_str_len))
            {
                //不相等
                k = 0;
            }
            else
            {
                k += 1;
                if ((resort_flag == FALSE) && (k >= 1) && (offsets_len[i] < resort_len))
                {
                    //有相同的字段,且没有到尾部
                    resort_flag = TRUE;
                }
            }
            resort_data[i+j] = k;
        }
        
        //更新索引信息到index_buf
        for (j = 0, k = 0; j < n; j++)
        {
            k = sub_index_buf[j];
            sub_index_buf[j] = index_buf[i + k];
        }

        for (j = 0; j < n; j++)
        {
            index_buf[i + j] = sub_index_buf[j];
        }

        i += n - 1;
    }

    //判断是否还有相同的title
    if (resort_flag == TRUE)
    {
        plist_get_app_msg();
        resort_flag = FALSE;
        //需要深一层的重排
        goto resort;
    }
    
    return TRUE;
}

