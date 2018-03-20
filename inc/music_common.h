/********************************************************************************
 *                            Module: music_engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-08 15:00     1.0             build this file
********************************************************************************/
#ifndef __MUSIC_COMMON_H__
#define __MUSIC_COMMON_H__

#include "psp_includes.h"
#include "enhanced.h"
#include "mmm_mp.h"
#include "mmm_id.h"

#define MAX_MUSIC_FILE_NUMS  20    //�����ǩ����

#define BM_HEADER_SECTOR_NUM (1)   //������ռ�õ�������

#define BM_DATA_SECTOR_NUM       ((MAX_MUSIC_FILE_NUMS + 1) / 2) //������ռ�õ�������

//coeffi.bin use
#define COEFFI_OFFSET  0x1a00  //ÿ�ű���0.5k���룬��ÿ�ű����6656(0x1a00) bytes
//asrc addr
#define ASRC_BUF0_ADDR 0xbfc0e000
#define ASRC_BUF0_LEN  0xe00  //����X ÿ����ռ�ֽ���:asrc buf0ռ896��896 * 4 = 3584
#define ASRC_BUF1_ADDR 0xbfc0f000
#define ASRC_BUF1_LEN  0xa0c  //����X ÿ����ռ�ֽ���:asrc buf1 ռ643��643 * 4 = 2572


#ifdef SUPPORT_MUSIC_DEBUG
#define music_print(str, data, mode)   libc_print(str,data,mode) 
#else
#define music_print(str, data, mode)
#endif

/*!
 * \brief
 *  ��ǰ����״̬
 */
typedef enum
{
	/*! ֹͣ */   
    StopSta  = 0, 
	/*! ��ͣ */       
    PauseSta, 
	/*! ���� */     
    PlaySta, 
	/*! AB���� */     
    PlayAB,
	/*! ����� */     
    PlayFast
} play_status_e;

/*!
 * \brief
 *  AB����״̬
 */
typedef enum
{
	/*! ��AB����״̬ */ 
    PlayAB_Null,
	/*! ��A��״̬ */     
    PlayAB_A,
	/*! ��B��״̬ */       
    PlayAB_B,
	/*! AB�������״̬ */      
    PlayAB_Do   
} ab_status_e;

/*!
 * \brief
 *  �����״̬
 */
typedef enum
{   
	/*! �������� */  
    FBPlay_Null,
	/*! ���״̬ */      
    FFPlaySta, 
	/*! ����״̬ */      
    FBPlaySta 
} fast_status_e;

/*! �����ʼ��״̬�� */
typedef enum
{
    /*! ���ڳ�ʼ�������˶�ȡ״̬�����ܶ���������κβ��� */
    EG_CTL_INITING,
    /*! �Ѿ���ʼ����� */
    EG_CTL_NORMAL,
    EG_CTL_MAX
} eg_ctl_e;

/*!
 * \brief
 *  ������Ϣ
 */
typedef enum
{
	/*! û�д��� */ 
    EG_ERR_NONE = 0,        
    /*! ���ļ�ʧ�� */ 
    EG_ERR_OPEN_FILE,    
    /*! �ļ���ʽ��֧�� */ 
    EG_ERR_NOT_SUPPORT,    
    /*! ������� */ 
    EG_ERR_DECODER_ERROR,   
    /*! ��license�����Ŵ����������ǲ���ʱ���Ѿ�����  */ 
    EG_ERR_NO_LICENSE,
    /*! DRMʱ�Ӵ���  */     
    EG_ERR_SECURE_CLOCK,
    /*! DRM��Ϣ����  */  
    EG_ERR_DRM_INFO    
} eg_err_e;

/*!
 * \brief
 *  ����״̬
 */
typedef struct
{
    /*! ����״̬  */ 
    play_status_e play_status;
    /*! AB����״̬  */ 
    ab_status_e ab_status;
    /*! �����״̬  */ 
    fast_status_e fast_status;
    /*! ����״̬,��ȡ�����  */ 
    eg_err_e err_status;
    /*!��ǰ����״̬*/
    eg_ctl_e ctl_status;    
} mengine_status_t;

/*!
 * \brief
 *  EQ���������Զ���
 */
typedef enum
{
    EQ_PARAM_NONE = 0x0,
    EQ_PARAM_ROCK,
    EQ_PARAM_FUNK,
    EQ_PARAM_HIPHOP,
    EQ_PARAM_JAZZ,
    EQ_PARAM_CALSSIC,
    EQ_PARAM_TECHNO,
    EQ_PARAM_MAX
}eq_param_e;


/*!
 * \brief
 *  ��ǰ�����ļ���Ϣ
 */
typedef struct
{
    /*! ��ǰʱ�� */
    uint32 cur_time;
    /*! ������ */
    uint32 cur_bitrate;
    /*! audibleͼƬλ��ƫ�� */
    uint32 cur_imag_pos;
    /*! audibleͼƬ���ڱ�־(�Ƿֱ�ͼ) */
    uint8 cur_imag_flag;
    /*! ��ʴ��ڱ�־ */
    uint8 cur_lyric;
    /*! �ļ��л���־ */
    /*! bit0: �����л���־        1(���и�)              0(δ�и�) */
    /*! bit1: audible�½��л���־ 1(�½��л�)            0(�½�δ�л�) */
    uint8 cur_file_switch;
    /*! ǰ̨AP״̬ */
    /*! 1(ǰ̨�����ڲ��Ž��������) 0(ǰ̨�����ڲ��Ž��������) */
    uint8 ui_type;
} mengine_playinfo_t;


/*!
 * \brief
 *  ������Ϣ�ṹ���������
 */ 
 /*
typedef struct
{
    // *          ����״̬��Ϣ
    mengine_status_t eg_status;

   //  *          �����ļ���ʱ���bit����Ϣ
    mengine_playinfo_t eg_playinfo;

} mengine_info_t;
*/


/*!
 * \brief
 *  music�ļ���������ż���
 */
typedef struct
{
    /*! ��ǰ�ļ���� */
    uint16 num;
    /*! ��ǰ�ļ����� */
    uint16 total;
} musfile_cnt_t;

/*!
 * \brief
 *  ����music�������ģʽ
 */
typedef enum
{
    /*! ѡ��������� */
    ENTER_START = 0x00,
    /*! ֹͣ */    
    ENTER_RESUME_PAUSE = 0x40, 
    /*! �ϵ����� */  
    ENTER_RESUME = 0x80, 
    /*! alarm��������  */      
    ENTER_ALARM = 0xc0 
} mengine_enter_mode_e;

//ѭ��ģʽ
typedef enum
{
    LoopNor, //��ͨѭ��
    LoopOne, //ѭ���ŵ�ǰ��
    LoopAll, //ѭ������
    Intro, //�������
    LoopOneDir, //ѭ���ŵ�ǰĿ¼
    LoopDir, //˳�򲥷�Ŀ¼
    MaxRepeatMod
} repeat_mode_e;

/*!
 * \brief
 *  audible�������ܽṹ�嶨��
 */
typedef struct
{
    uint16 magic;
    //uint8 title[30];
    /*! �ϵ���Ϣ */
    mmm_mp_bp_info_t bk_infor;
    /*! ·����Ϣ */
    file_path_info_t locat_info;
} audible_resume_info_t;


/*!
 * \brief
 *  audible�л�ģʽ
 */
typedef enum
{
    /*! �����л�ģʽ  */ 	
    SWITCH_TITLE = 0,
    /*! �½��л�ģʽ  */ 	    
    SWITCH_SECTION  
} audible_switch_mode_e;

/*!
 * \brief
 *  ��ǩͷ����Ϣ
 */
typedef struct
{
    uint16 magic; //ħ��
    uint16 total_index; //��ǩ����
    uint16 first_index;
    uint16 last_index;
    uint16 del_total; //��ɾ����������ǩ����
    //uint32 last_access_cluster_no; //���һ�η�����ǩ��cluster_no
    //uint32 last_access_dir_entry; //���һ�η�����ǩ��dir_entry
} music_bm_head;

/*!
 * \brief
 *  EQģʽ
 */
typedef struct
{
    /*! eq�����ͣ����mmm_mp_eqmode_t */
    uint8 eq_mode;
    /*! �����EQ������jazz,pop,normal,rock,dbb,classic,soft */
    uint8 eq_type;
    /*! �������� */
    uint8 reserved[2];
}eq_param_t;
/*!
 * \brief
 *  �������ñ����ṹ�嶨��
 */ 
typedef struct
{
    /*! ħ�� */ 	
    uint16 magic; 
    /*! ����ֵ*/     
    uint8 volume; 
    /*! shuffle ���� ȡ��4bit,5bit*/      
    uint8 shuffle_flag; 
    /*! repeat mode*/      
    fsel_mode_e repeat_mode; 
    /*! �����ٶ� */     
    uint8 play_rate; 
#if 0
    /*! ��������  */      
    uint8 replaytimes; 
    /*! �������  */     
    uint8 replaygap; 
#endif
    /*! cue�б���ȡ��6bit,7bit*/
    uint8 cue_flag;
    /*! �����ֽ� */  
    uint8 reserved[1];
    /*! ����ͨ��ģʽ��0��������1��ֻ�����������2��ֻ�����������3�������������� */     
    uint8 chanel_select_mode;
    /*! �ļ�ѡ������ �б�/�ղؼ�/audible/Ŀ¼ */      
    fsel_type_e fsel_type;
    /*! audible�½�ģʽ */       
    audible_switch_mode_e section_mod;
    /*! ������� */     
    uint8 fff_step;
    /*! ����ʱ�䵥λ���� */       
    uint32 fade_out_time;
    /*! ����ʱ�䵥λ���� */      
    uint32 fade_in_time;
    /*! �ļ���ż����� */     
    musfile_cnt_t file_cnt; 
    /*! wav/mp3/wma��Ч���� */     
    eq_param_t eq_info; 
    /*! ������ʽ��Ч����*/    
    mmm_mp_eq_info_t eq_info_other; 
    /*! ��¼��ǰ�ļ���·����Ϣ/�б��ļ��Ĳ㼶��ϵ*/      
    musfile_location_u location; 
    /*! �ϵ���Ϣ*/        
    mmm_mp_bp_info_t bk_infor; 
} mengine_config_t;

typedef enum
{
   NEED_INIT_PARA = 0x00000001,
   NEED_FILE_DATA = 0x00000008,
   NEED_FILE_SEEK = 0x00000009,   
   TIME_SEEK_END = 0x0000000C,
   FRAME_DECODE_END = 0X00000010,
   POST_PROCESS_END = 0x00000011,
   //for ext share memory use
   NEED_SHARE_INFO = 0x0000001E,
   NEED_TO_STOP = 0x0000001F,
   RESERVE
}dsp_cmd_type_t;

typedef struct
{
    bool prefetch_flag;

    uint8 *prefetch_buf_ptr;

    int prefetch_buf_size;

    int prefetch_buf_remain;

    dsp_cmd_type_t cmd_type;

} mmm_prefetch_data_t;

typedef struct storage_io_s {
    /*! ������ */
    int (*read)(void *buf, int size, int count, struct storage_io_s *io);
    /*! д���� */
    int (*write)(void *buf, int size, int count, struct storage_io_s *io);
    /*! ��λ */
    int (*seek)(struct storage_io_s *io, int offset, int whence);
    /*! �ļ�λ�� */
    int (*tell)(struct storage_io_s *io);
	/* �ļ����� 0: VFS�ļ���1��SD�ļ� */
	int file_type;
	/* �ļ���� */
	void *handel;
} storage_io_t;



#endif //__MUSIC_COMMON_H__
