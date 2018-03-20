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

#define MAX_MUSIC_FILE_NUMS  20    //最大书签总数

#define BM_HEADER_SECTOR_NUM (1)   //索引区占用的扇区数

#define BM_DATA_SECTOR_NUM       ((MAX_MUSIC_FILE_NUMS + 1) / 2) //数据区占用的扇区数

//coeffi.bin use
#define COEFFI_OFFSET  0x1a00  //每张表都以0.5k对齐，故每张表相隔6656(0x1a00) bytes
//asrc addr
#define ASRC_BUF0_ADDR 0xbfc0e000
#define ASRC_BUF0_LEN  0xe00  //项数X 每项所占字节数:asrc buf0占896项896 * 4 = 3584
#define ASRC_BUF1_ADDR 0xbfc0f000
#define ASRC_BUF1_LEN  0xa0c  //项数X 每项所占字节数:asrc buf1 占643项643 * 4 = 2572


#ifdef SUPPORT_MUSIC_DEBUG
#define music_print(str, data, mode)   libc_print(str,data,mode) 
#else
#define music_print(str, data, mode)
#endif

/*!
 * \brief
 *  当前播放状态
 */
typedef enum
{
	/*! 停止 */   
    StopSta  = 0, 
	/*! 暂停 */       
    PauseSta, 
	/*! 播放 */     
    PlaySta, 
	/*! AB复读 */     
    PlayAB,
	/*! 快进退 */     
    PlayFast
} play_status_e;

/*!
 * \brief
 *  AB复读状态
 */
typedef enum
{
	/*! 非AB复读状态 */ 
    PlayAB_Null,
	/*! 定A点状态 */     
    PlayAB_A,
	/*! 定B点状态 */       
    PlayAB_B,
	/*! AB复读完成状态 */      
    PlayAB_Do   
} ab_status_e;

/*!
 * \brief
 *  快进退状态
 */
typedef enum
{   
	/*! 正常播放 */  
    FBPlay_Null,
	/*! 快进状态 */      
    FFPlaySta, 
	/*! 快退状态 */      
    FBPlaySta 
} fast_status_e;

/*! 引擎初始化状态机 */
typedef enum
{
    /*! 正在初始化，除了读取状态，不能对引擎进行任何操作 */
    EG_CTL_INITING,
    /*! 已经初始化完毕 */
    EG_CTL_NORMAL,
    EG_CTL_MAX
} eg_ctl_e;

/*!
 * \brief
 *  错误信息
 */
typedef enum
{
	/*! 没有错误 */ 
    EG_ERR_NONE = 0,        
    /*! 打开文件失败 */ 
    EG_ERR_OPEN_FILE,    
    /*! 文件格式不支持 */ 
    EG_ERR_NOT_SUPPORT,    
    /*! 解码出错 */ 
    EG_ERR_DECODER_ERROR,   
    /*! 无license，播放次数已满或是播放时间已经到期  */ 
    EG_ERR_NO_LICENSE,
    /*! DRM时钟错误  */     
    EG_ERR_SECURE_CLOCK,
    /*! DRM信息错误  */  
    EG_ERR_DRM_INFO    
} eg_err_e;

/*!
 * \brief
 *  引擎状态
 */
typedef struct
{
    /*! 播放状态  */ 
    play_status_e play_status;
    /*! AB复读状态  */ 
    ab_status_e ab_status;
    /*! 快进退状态  */ 
    fast_status_e fast_status;
    /*! 错误状态,获取后清除  */ 
    eg_err_e err_status;
    /*!当前引擎状态*/
    eg_ctl_e ctl_status;    
} mengine_status_t;

/*!
 * \brief
 *  EQ参数类型自定义
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
 *  当前播放文件信息
 */
typedef struct
{
    /*! 当前时间 */
    uint32 cur_time;
    /*! 比特率 */
    uint32 cur_bitrate;
    /*! audible图片位置偏移 */
    uint32 cur_imag_pos;
    /*! audible图片存在标志(非分辨图) */
    uint8 cur_imag_flag;
    /*! 歌词存在标志 */
    uint8 cur_lyric;
    /*! 文件切换标志 */
    /*! bit0: 歌曲切换标志        1(有切歌)              0(未切歌) */
    /*! bit1: audible章节切换标志 1(章节切换)            0(章节未切换) */
    uint8 cur_file_switch;
    /*! 前台AP状态 */
    /*! 1(前台非正在播放界面或屏保) 0(前台是正在播放界面非屏保) */
    uint8 ui_type;
} mengine_playinfo_t;


/*!
 * \brief
 *  引擎信息结构体变量定义
 */ 
 /*
typedef struct
{
    // *          引擎状态信息
    mengine_status_t eg_status;

   //  *          播放文件的时间和bit率信息
    mengine_playinfo_t eg_playinfo;

} mengine_info_t;
*/


/*!
 * \brief
 *  music文件总数及序号计数
 */
typedef struct
{
    /*! 当前文件序号 */
    uint16 num;
    /*! 当前文件总数 */
    uint16 total;
} musfile_cnt_t;

/*!
 * \brief
 *  开启music引擎进入模式
 */
typedef enum
{
    /*! 选择歌曲播放 */
    ENTER_START = 0x00,
    /*! 停止 */    
    ENTER_RESUME_PAUSE = 0x40, 
    /*! 断点续播 */  
    ENTER_RESUME = 0x80, 
    /*! alarm歌曲播放  */      
    ENTER_ALARM = 0xc0 
} mengine_enter_mode_e;

//循环模式
typedef enum
{
    LoopNor, //普通循环
    LoopOne, //循环放当前首
    LoopAll, //循环所有
    Intro, //浏览播放
    LoopOneDir, //循环放当前目录
    LoopDir, //顺序播放目录
    MaxRepeatMod
} repeat_mode_e;

/*!
 * \brief
 *  audible续播功能结构体定义
 */
typedef struct
{
    uint16 magic;
    //uint8 title[30];
    /*! 断点信息 */
    mmm_mp_bp_info_t bk_infor;
    /*! 路径信息 */
    file_path_info_t locat_info;
} audible_resume_info_t;


/*!
 * \brief
 *  audible切换模式
 */
typedef enum
{
    /*! 标题切换模式  */ 	
    SWITCH_TITLE = 0,
    /*! 章节切换模式  */ 	    
    SWITCH_SECTION  
} audible_switch_mode_e;

/*!
 * \brief
 *  书签头部信息
 */
typedef struct
{
    uint16 magic; //魔数
    uint16 total_index; //书签总数
    uint16 first_index;
    uint16 last_index;
    uint16 del_total; //有删除操作的书签总数
    //uint32 last_access_cluster_no; //最后一次访问书签项cluster_no
    //uint32 last_access_dir_entry; //最后一次访问书签项dir_entry
} music_bm_head;

/*!
 * \brief
 *  EQ模式
 */
typedef struct
{
    /*! eq的类型，详见mmm_mp_eqmode_t */
    uint8 eq_mode;
    /*! 具体的EQ类型如jazz,pop,normal,rock,dbb,classic,soft */
    uint8 eq_type;
    /*! 保留对齐 */
    uint8 reserved[2];
}eq_param_t;
/*!
 * \brief
 *  引擎配置变量结构体定义
 */ 
typedef struct
{
    /*! 魔数 */ 	
    uint16 magic; 
    /*! 音量值*/     
    uint8 volume; 
    /*! shuffle 开关 取高4bit,5bit*/      
    uint8 shuffle_flag; 
    /*! repeat mode*/      
    fsel_mode_e repeat_mode; 
    /*! 播放速度 */     
    uint8 play_rate; 
#if 0
    /*! 复读次数  */      
    uint8 replaytimes; 
    /*! 复读间隔  */     
    uint8 replaygap; 
#endif
    /*! cue列表播放取高6bit,7bit*/
    uint8 cue_flag;
    /*! 保留字节 */  
    uint8 reserved[1];
    /*! 播放通道模式，0：正常，1：只输出左声道，2：只输出右声道，3：左右声道互换 */     
    uint8 chanel_select_mode;
    /*! 文件选择类型 列表/收藏夹/audible/目录 */      
    fsel_type_e fsel_type;
    /*! audible章节模式 */       
    audible_switch_mode_e section_mod;
    /*! 快进步长 */     
    uint8 fff_step;
    /*! 淡出时间单位毫秒 */       
    uint32 fade_out_time;
    /*! 淡入时间单位毫秒 */      
    uint32 fade_in_time;
    /*! 文件序号及总数 */     
    musfile_cnt_t file_cnt; 
    /*! wav/mp3/wma音效参数 */     
    eq_param_t eq_info; 
    /*! 其它格式音效参数*/    
    mmm_mp_eq_info_t eq_info_other; 
    /*! 记录当前文件的路径信息/列表文件的层级关系*/      
    musfile_location_u location; 
    /*! 断点信息*/        
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
    /*! 读数据 */
    int (*read)(void *buf, int size, int count, struct storage_io_s *io);
    /*! 写数据 */
    int (*write)(void *buf, int size, int count, struct storage_io_s *io);
    /*! 定位 */
    int (*seek)(struct storage_io_s *io, int offset, int whence);
    /*! 文件位置 */
    int (*tell)(struct storage_io_s *io);
	/* 文件类型 0: VFS文件，1：SD文件 */
	int file_type;
	/* 文件句柄 */
	void *handel;
} storage_io_t;



#endif //__MUSIC_COMMON_H__
