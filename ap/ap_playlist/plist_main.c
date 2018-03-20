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
 * \brief    ������д�ļ��ĸ���
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "plist.h"
#include "plist_8k.h"
#include "plist_16k.h"

#include "rdata_ex.h"

#define MENU_REC_MAX  1

extern uint8 m3u_ext_buf[4];

static fsel_param_t fsel_param _BANK_DATA_ATTR_; //bank data

//static void init_sys_data(uint8 type);
static void init_plist_data(void);
static bool music_build_plist(void);
static bool audible_build_plist(void);
static bool others_build_plist(void);



typedef bool (*object_build_f)(void);

static const object_build_f object_build_plist[PL_LIB_MAX] =
{ music_build_plist, audible_build_plist, others_build_plist };

static const uint8 stg_drv_type[] =
{ DISK_C, DISK_H, DISK_U };

static const char plist_ui_sty[] = "playlist.sty";

#ifdef PC

//simulator ���������M3U��׺�ĳ�LRC������M3U�б���������
static const uint32 file_type_bitmap[PL_LIB_MAX] =
{ MUSIC_BITMAP, AUDIBLE_BITMAP, EXT_NAME_LRC};//

#else

static const uint32 file_type_bitmap[PL_LIB_MAX] =
{ MUSIC_BITMAP, AUDIBLE_BITMAP, (uint32) m3u_ext_buf };

#endif

static const uint8 stg_creat_fail[] =
{ S_PLIST_C_CREAT_FAIL, S_PLIST_H_CREAT_FAIL, S_PLIST_U_CREAT_FAIL };

extern bool creat_plist_lib(uint8 type) __FAR__;
extern bool close_plist_lib(uint8 type) __FAR__;

extern uint16 scan_music_file(void) __FAR__;
extern uint16 scan_audible_file(void) __FAR__;
extern uint16 scan_others_info(void) __FAR__;

extern void sort_music_list(void) __FAR__;
extern void sort_others_list(void) __FAR__;
extern void sort_audible_list(void) __FAR__;

extern bool plist_fsel_set_typebit(uint32 file_type_bit, bool reset_dir_flag) __FAR__;

extern void plist_timer_proc(void) __FAR__;
extern void _app_display_init(void) __FAR__;

extern void creat_album_pic(void) __FAR__;
extern bool check_bookmark_exist(void) __FAR__;
extern bool creat_userpl_table(void) __FAR__;

extern int8 delete_lib_file(uint8 lib_bitmap,uint8 tmp_drv_sn) __FAR__;

extern void plist_sys_init(void) __FAR__;
extern void plist_sys_exit(app_param_e enter_mode) __FAR__;

extern bool check_m3u_exist(void) __FAR__;
extern void plist_reflash_string(uint8 str_id) __FAR__;

/***************************/
void _read_var(void)
{
    setting_comval_t *g_setting_comval_p;
    /*��ȡcommon VM����������Ϣ*/
    g_setting_comval_p = (setting_comval_t*) temp_plist_buf;
    sys_vm_read(g_setting_comval_p, VM_AP_SETTING, sizeof(setting_comval_t));
    libc_memcpy(&g_comval, &g_setting_comval_p->g_comval, sizeof(comval_t));
    com_set_sys_comval(&g_comval);
    language_id = g_setting_comval_p->g_comval.language_id;

}

/******************************************************************************/
/*
 * \par  Description: AP���
 *
 * \param[in]    argc--�����������
 argv--��������ָ��
 * \param[out]

 * \return       >0 �ɹ���<=0 ʧ��

 * \note
 *******************************************************************************/

int main(int argc, const char *argv[])
{
    uint8 disk_param;
    uint8 build_param;
    int8 app_timer_id; //����ʱ��id
    uint16 plist_param;
    uint16 lib_file_total[MENU_REC_MAX];

    bool creat_file_flag;

    app_param_e enter_mode = (app_param_e) (argc & 0x1f);

    //��Ƶ
    adjust_freq_set_level(AP_FRONT_LOW_PRIO, FREQ_LEVEL11, FREQ_NULL);

    sys_mdelay(1);

    plist_sys_init();

    _read_var();

    //����MAIN_MENU��VM��
    //sys_vm_read(lib_file_total, VM_FILE_NUM, sizeof(lib_file_total));
    libc_memset(lib_file_total, 0x0000, sizeof(lib_file_total));

    com_clear_all_history(PATH_HISTORY_DIR);

    //argc ��bit7~bit5Ϊ���������б��Ľ���
#ifdef _CARD_BASE_
    plist_param = (uint16) ((uint8) argc & PLIST_DISK_C) << 8;
#else
    plist_param = (uint16) ((uint8) argc & PLIST_DISK_H) << 8;
#endif

    //Ĭ�ϴ������õı���
    plist_param = plist_param + (PL_BUILD_ALL & 0x03);// don't creat m3u
    disk_param  = (uint8) ((plist_param >> 8) & PLIST_DISK_ALL);
    build_param = (uint8) (plist_param & PL_BUILD_ALL);

#ifndef _CARD_BASE_
    if ((disk_param & PLIST_DISK_H) != 0)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            disk_param = disk_param & (~PLIST_DISK_H);
        }
    }
#endif

    if ((disk_param == 0) || (build_param == 0))
    {
        //������Ч
        goto ap_exit;
    }

    if ((enter_mode == PARAM_FROM_CONFIG) || (enter_mode == PARAM_FROM_UDISK))
    {
        creat_file_flag = TRUE;
    }
#ifdef _CARD_BASE_
    else if (0 != (disk_param & (PLIST_DISK_C | PLIST_DISK_U)))
#else
    else if (0 != (disk_param & (PLIST_DISK_H | PLIST_DISK_U)))
#endif
    {
        creat_file_flag = TRUE;
    }
    else
    {
        creat_file_flag = FALSE;
    }

    if (creat_file_flag == TRUE)
    {
        plist_param = plist_param | PL_BUILD_M3U;//creat m3u
        build_param = build_param | PL_BUILD_M3U;//creat m3u
    }

    ui_set_display_buf(LCD_RAM0_INDEX);

    //��ʼ��applib�⣬ǰ̨AP
    applib_init(APP_ID_PLAYLIST, APP_TYPE_GUI);
    //��ʼ�� applib ��Ϣģ��
    applib_message_init(NULL);
    //��ʼ������ʱ��
    init_app_timers(playlist_app_timer_vector, APP_TIMER_TOTAL);

    ui_res_open(plist_ui_sty, UI_AP);
    //��ʱ��350ms

    app_timer_id = set_app_timer(APP_TIMER_ATTRB_UI, 350, plist_timer_proc);

    _app_display_init();

    cpu_release_count = 0;
    drv_sn = 0;

    while (disk_param != 0)
    {
        if ((disk_param & 0x80) == 0)
        {
            goto next_disk;
        }

        //BANK DATA,����ÿ�ζ�Ҫ��ʼ��
        fsel_param.brow_type = 0;
        fsel_param.fsel_mode = FSEL_MODE_NORMAL;
        fsel_param.fsel_type = FSEL_TYPE_COMMONDIR;
        fsel_param.file_type_bit = file_type_bitmap[drv_sn];
        fsel_param.disk = stg_drv_type[drv_sn];

        ap_vfs_mount = fsel_init(&fsel_param, MODE_NORMAL);
        lib_num = 0;
        if (ap_vfs_mount == -1)
        {
            plist_reflash_string(stg_creat_fail[drv_sn & 0x03]);
            sys_os_time_dly(35);//��ʾ����ʹ��
            goto stg_exit;
        }

        plt_print("<plist start> ", sys_get_ab_timer(), 2);
        build_param = (uint8) (plist_param & PL_BUILD_ALL);
        while (build_param != 0)
        {
            if ((build_param & 0x01) != 0)
            {
                plist_get_app_msg();

#ifdef ENHANCED_FIND_MODE
                fsel_set_typebit(file_type_bitmap[lib_num],TRUE);//Ĭ�Ͻ���root
#else
                plist_fsel_set_typebit(file_type_bitmap[lib_num], TRUE);
#endif
                //fsel_param.file_type_bit=file_type_bitmap[lib_num];
                if(lib_num!=0)
                {
                    //musicҪ�����⴦�������ݸ���������������Ӧ����
                    libc_print("lib_no.",lib_num,2);
                    init_sys_data(lib_num);
                }
                plist_reflash_progress(-1);

                if (FALSE != creat_plist_lib(lib_num))
                {
                    init_plist_data();
                    plist_get_app_msg();
                    object_build_plist[lib_num]();
                    
                    if (FALSE == close_plist_lib(lib_num))
                    {
                        goto build_err;
                    }

                    //card������¼�ܸ���
                    if (lib_num < MENU_REC_MAX)
                    {
                        lib_file_total[lib_num] = ap_plist_file_total;
                    }

                    plist_reflash_progress(-1);

                }
                else
                {
                    build_err: plist_reflash_string(stg_creat_fail[drv_sn & 0x03]);
                    sys_os_time_dly(35);//��ʾ����ʹ��
                    goto stg_exit;
                }

            }
            lib_num++;
            build_param = build_param >> 1;
        }

        //�����ղؼ��ļ�
        if (creat_file_flag == TRUE)
        {
            //����ר��ͼƬ�ļ�
            creat_album_pic();
            //�����ղؼ��ļ�
            creat_userpl_table();
            plist_get_app_msg();
            //������ǩ�ļ�
            check_bookmark_exist();
            //����3uu.lib������dat;
            check_m3u_exist();
        }

        change_card_plist(TRUE);

        stg_exit:

        fsel_exit();

        next_disk: drv_sn++;
        disk_param = disk_param << 1;
    }

    plt_print("<plist end> ", sys_get_ab_timer(), 2);
    plist_reflash_progress(10);

    kill_app_timer(app_timer_id);
    //�ر�ui��Դ�ļ�
    ui_res_close(UI_AP);
    ui_set_display_buf(LCD_RAM1_INDEX);
    
    //ִ��applib���ע������
    applib_quit();

    ap_exit:

    adjust_freq_set_level(AP_FRONT_LOW_PRIO, FREQ_NULL, FREQ_NULL);

    //plt_print("<playlist>", 0, 0);
	
    //д�������ļ�����
	  sys_vm_read(temp_plist_buf, VM_FILE_NUM, SECTOR_SIZE);
    libc_memcpy(temp_plist_buf, lib_file_total, sizeof(lib_file_total));
    sys_vm_write(temp_plist_buf, VM_FILE_NUM, SECTOR_SIZE);
    plist_sys_exit(enter_mode);

#ifndef PC
    //   act_writel(prev_nandpllctl, NANDCLKCTL);
    //   sys_mdelay(1);
    //   sys_adjust_clk(save_freq_level, 0);
#endif

    return 1;
}

/******************************************************************************/
/*
 * \par  Description: ��ʼ��ϵͳ�ڲ�����
 *
 * \param[in]    NULL

 * \param[out]   NULL

 * \return       NULL

 * \note
 *******************************************************************************/
 void init_sys_data(uint8 type)
{
    fileinfo_data_pt = (uint8*)  FILE_INFO_ADDR;
    title_data_pt    = (uint8*)  SORT_DATA_ADDR; //ram�б���title����,���64k�ռ�
    track_data_pt    = (uint8*)  TRACK_DATA_ADDR;//����4k����,1byte*4000
    file_index_buf   = (uint16*) FILE_INDEX_BUF_ADDR;
    sort_index_buf   = (uint16*) SORT_INDEX_BUF_ADDR;
    tidy_buf_a       = (uint16*) TIDY_BUF_A_ADDR;
    tidy_buf_b       = (uint16*) TIDY_BUF_B_ADDR;   
    count_buf_a      = (uint16*) COUNT_BUF_A_ADDR;
    count_buf_b      = (uint16*) COUNT_BUF_B_ADDR;
    if (ap_plist_sorting_level == 4000)
    {
	    count_buf_c      = (uint16*) COUNT_BUF_C_ADDR;
	    resort_data      = (uint16*) COUNT_BUF_C_ADDR;//����}
    }else {
    	resort_data      = (uint16*) TIDY_BUF_B_ADDR;//����}
 	}
    return;

}

//8k������ʱ������һ��궨����г�ʼ��
 void init_mem_by_songs_num(void)
{
     fileinfo_data_pt = (uint8*)  FILE_INFO_ADDR;
	   title_data_pt	= (uint8*)	SORT_DATA_ADDR_8k; //ram�б���title����,���64k�ռ�
	   track_data_pt	= (uint8*)	TRACK_DATA_ADDR_8k;//����4k����,1byte*4000
	   file_index_buf	= (uint16*) FILE_INDEX_BUF_ADDR_8k;
	   sort_index_buf	= (uint16*) SORT_INDEX_BUF_ADDR_8k;
	   tidy_buf_a		= (uint16*) TIDY_BUF_A_ADDR_8k;
	   tidy_buf_b		= (uint16*) TIDY_BUF_B_ADDR_8k;   
	   count_buf_a		= (uint16*) COUNT_BUF_A_ADDR_8k;
	   count_buf_b		= (uint16*) COUNT_BUF_B_ADDR_8k;
	   if (ap_plist_sorting_level == 4000)
	   {
		   count_buf_c		= (uint16*) COUNT_BUF_C_ADDR;
		   resort_data		= (uint16*) COUNT_BUF_C_ADDR;//����}
	   }else {
		   resort_data		= (uint16*) TIDY_BUF_B_ADDR_8k;//����}
	   }
	   return;

}

void init_mem_by_songs_16k(void)
{
    
     fileinfo_data_pt =   (uint8*)   FILE_INFO_ADDR_16k;
	   title_data_pt    	= (uint8*)	 SORT_DATA_ADDR_16k; //ram�б���title����,���64k�ռ�
	   track_data_pt	    = (uint8*)	 TRACK_DATA_ADDR_16k;//����4k����,1byte*4000
	   file_index_buf	    = (uint16*) FILE_INDEX_BUF_ADDR_16k;
	   sort_index_buf	    = (uint16*) COUNT_BUF_C_ADDR_16k; 
	   count_buf_a		    = (uint16*) COUNT_BUF_A_ADDR_16k;
	   count_buf_b		    = (uint16*) COUNT_BUF_B_ADDR_16k;
		 resort_data		    = (uint16*) COUNT_BUF_C_ADDR_16k ;//����}

	   return;

}
/******************************************************************************/
/*
 * \par  Description: ��ʼ������ÿ���б�ʹ�õ�������
 *
 * \param[in]    NULL

 * \param[out]   NULL

 * \return       NULL

 * \note
 *******************************************************************************/
static void init_plist_data(void)
{
    libc_memset(ap_file_index_offset, 0x00, sizeof(ap_file_index_offset));
    libc_memset(ap_sort_index_offset, 0x00, sizeof(ap_sort_index_offset));
    libc_memset(ap_index_parent_offset, 0x00, sizeof(ap_index_parent_offset));
    libc_memset(ap_son_tree_num, 0x00, sizeof(ap_son_tree_num));
    libc_memset(ap_son_tree_offset, 0x00, sizeof(ap_son_tree_offset));

    fsel_scan_flag = 0;
    ap_plist_file_total = 0;
    ap_plist_file_offset = ap_pos_file_offset;

    return;
}
/******************************************************************************/
/*
 * \par  Description: ����MUSIC�����б�
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool music_build_plist(void)
{

    if (scan_music_file() > 0)
    {
        //libc_print("sm*",sys_get_ab_timer(),2);
        plist_get_app_msg();
        plist_reflash_progress(-1);
        sort_music_list();
        plist_get_app_msg();
        plist_reflash_progress(-1);
		//libc_print("smend",sys_get_ab_timer(),2);
    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: ����AUDIBLE�����б�
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
static bool audible_build_plist(void)
{
    if (scan_audible_file() > 0)
    {
        sort_audible_list();
    }

    return TRUE;
}

/******************************************************************************/
/*
 * \par  Description: ���� ��Ƶ or ͼƬ or ���ӵĲ����б�
 *
 * \param[in]

 * \param[out]

 * \return        TRUE OR FALSE

 * \note
 *******************************************************************************/
//ebook
static bool others_build_plist(void)
{
    if (scan_others_info() > 0)
    {
        plist_get_app_msg();
        plist_reflash_progress(-1);
        sort_others_list();
    }

    return TRUE;
}
