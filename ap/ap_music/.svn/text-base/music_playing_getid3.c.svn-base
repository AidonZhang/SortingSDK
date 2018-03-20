/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-15          1.0              create this file
 *******************************************************************************/

#include "app_music.h"

#include "app_music_playing.h"

uint8 convert_buf[Id3BufSIZE * 2] _BANK_DATA_ATTR_;

//定义const data 所有后缀数据
const uint8 support_ext[MusicExtNum][4] =
{
    "MP1", //mp3
    "MP2", //mp3
    "MP3", //mp3
    "WMA", //wma
    "APE", //ape
    "FLA", //flac
    "OGG", //ogg
    "AAC", //aac
    "AAX", //aax
    "M4A", //m4a
    "AA " //aa
};

const id3_type_e support_id3_type[] =
{
    ID3_TYPE_MP3, //mp1
    ID3_TYPE_MP3, //mp2
    ID3_TYPE_MP3, //mp3
    ID3_TYPE_WMA, //wma
    ID3_TYPE_APE, //ape
    ID3_TYPE_FLAC, //fla
    ID3_TYPE_OGG, //ogg
    ID3_TYPE_AAC, //aac
    ID3_TYPE_AAX, //aax
    ID3_TYPE_AAC, //m4a
    ID3_TYPE_AA //aa
};

/******************************************************************************/
/*!
 * \par  Description:
 * \id3_type_e play_scene_checktype(char * extname)
 * \获取后缀名
 * \param[in]    extname后缀名buffer  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_getid3.c
 * \note
 */
/*******************************************************************************/
id3_type_e play_scene_checktype(uint8 * extname)
{
    uint8 cnt;
    id3_type_e ret;
    
    //转换当前后缀名(转成大写)
    while (cnt < 3)
    {
        extname[cnt] = ((extname[cnt] >= 'a') && (extname[cnt] <= 'z')) ? (extname[cnt] - 'a' + 'A') : extname[cnt];
        cnt ++;
    }

    //比较后缀
    for (cnt = 0; cnt < MusicExtNum; cnt++)
    {
        if (libc_strncmp(extname, support_ext[cnt], sizeof(support_ext[cnt])) == 0)
        {
            ret = support_id3_type[cnt];
            break;
        }
    }
    if (cnt == MusicExtNum)
    {
        ret = ID3_TYPE_END;
    }
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void play_scene_initid3buf(void)
 * \获取后缀名
 * \param[in]    void  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_getid3.c
 * \note
 */
/*******************************************************************************/
void play_scene_initid3buf(void)
{
    //标题buffer清零
    libc_memset(g_title_buf, 0, sizeof(g_title_buf));
    //歌手buffer和长度清零
    libc_memset(g_artist_buf, 0, sizeof(g_artist_buf));
    //专辑buffer清零
    libc_memset(g_album_buf, 0, sizeof(g_album_buf));
    //文件属性buffer清零
    libc_memset(g_music_info_buf, 0, sizeof(g_music_info_buf));
    
    //标题buffer
    g_id3_info.tit2_buffer = g_title_buf;
    //歌手buffer和长度
    g_id3_info.tpe1_buffer = g_artist_buf;
    //专辑buffer
    g_id3_info.talb_buffer = g_album_buf;
    //流派buffer
    g_id3_info.genre_buffer = NULL;
    //drm的buffer
    g_id3_info.drm_buffer = NULL;
    //音轨buffer
    g_id3_info.track_buffer = NULL;

    //文件标题长度
    g_id3_info.tit2_length = Id3BufSIZE;
    //作者长度
    g_id3_info.tpe1_length = ArtBufSIZE;
    //专辑长度
    g_id3_info.talb_length = ArtBufSIZE;
    //流派长度
    g_id3_info.genre_length = 0;
    //drm长度
    g_id3_info.drm_length = 0;
    //音轨长度
    g_id3_info.track_length = 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void deal_id3_sub(uint8 *str_buf, uint8 type)
 * \处理id3的操作
 * \param[in]    str_buf id3信息的buffer  para1
 * \param[in]    type id3类型 para2
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_getid3.c
 * \note
 */
/*******************************************************************************/
void deal_id3_sub(uint8 *str_buf, uint8 type)
{
    uint16 convert_size;
    uint8 buffer_size;

    if ((str_buf[0] == 0xff) && (str_buf[1] == 0xfe))
    {
        g_scroll_content[type].language = UNICODEDATA;
    }
    else if ((str_buf[0] == 0xef) && (str_buf[1] == 0xbb) && (str_buf[2] == 0xbf))
    {
        ui_utf8_to_unicode(str_buf, convert_buf, &convert_size);
        if (type == 0)
        {
            buffer_size = Id3BufSIZE;
        }
        else
        {
            buffer_size = ArtBufSIZE;
        }
        if (convert_size > buffer_size)
        {
            convert_size = buffer_size;
            convert_buf[buffer_size - 2] = 0;
            convert_buf[buffer_size - 1] = 0;
        }
        libc_memcpy(str_buf, convert_buf, (uint32) convert_size);
        g_scroll_content[type].language = UNICODEDATA;
    }
    else
    {
        g_scroll_content[type].language = ANSIDATAAUTO;
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 * \void play_scene_dealid3(void)
 * \处理id3信息
 * \param[in]    void  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_getid3.c
 * \note
 */
/*******************************************************************************/
void play_scene_deal_id3(void)
{
    deal_id3_sub(g_title_buf, 0);
    deal_id3_sub(g_artist_buf, 1);

    deal_id3_sub(g_album_buf, 2);

    //滚屏内容填充
    g_scroll_content[0].strbuf = g_title_buf;
    g_scroll_content[1].strbuf = g_artist_buf;
    g_scroll_content[2].strbuf = g_album_buf;
    g_scroll_content[3].strbuf = g_music_info_buf;
    g_scroll_content[3].language = ANSIDATAAUTO;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \static void music_itoa(uint16 i, uint8 *buf)
 * \数字转换成字符串
 * \param[in]    i  要转换的整数
 * \param[in]    buf 保存转换后的字符串
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_getid3.c
 * \note  
 *     去除多余的空格,支持转换成5个字符
 */
/*******************************************************************************/
static void music_itoa(uint16 i, uint8 *buf)
 {
    uint8 type = 0;
    uint16 tmp = i;
    
    while(tmp)
    {
        type ++;
        tmp /= 10;
    }
    type = 5 - type;  
    switch(type)
    {
        case 0:
            buf[0] = (uint8)((i % 100000) / 10000) + '0';
        case 1:
            buf[1-type] = (uint8)((i % 10000) / 1000) + '0';
        case 2:
            buf[2-type] = (uint8)((i % 1000) / 100) + '0';
        case 3:
            buf[3-type] = (uint8)((i % 100) / 10) + '0';
        default:
            buf[4-type] = (uint8)(i % 10) + '0';
            buf[5-type] = 0;
    }

     return;
 }
/******************************************************************************/
/*!
 * \par  Description:
 * \static void get_musicfile_info(uint8* nameptr)
 * \获取歌曲属性用于滚屏显示
 * \param[in]    nameptr  音乐文件类型字符串
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_getid3.c
 * \note  
 *      g_music_info_buf[22]保存格式: XXXX[formet]+' '+12345kbps+' '+123kHZ+'0';
 */
/*******************************************************************************/
static void get_musicfile_info(uint8* nameptr)
{
    uint8  i,m,n,tmp_str[6];
    uint8  *string1 = "kHZ";
    uint8  *string2 = "kbps";
    mmm_mp_file_info_t music_fileinfo;

    if (nameptr[0]== 'A' && nameptr[1]== 'I' && nameptr[2]== 'F')//play_scene_checktype已经转换为大写
    {
        nameptr[3] = 'F';
        i = 4;
    }
    else if(nameptr[0]== 'F' && nameptr[1]== 'L' && nameptr[2]== 'A')
    {
        nameptr[3] = 'C';
        i = 4;
    }
    else if (libc_strlen(nameptr) == 3)
    {
        i = 3;
    }
    else
    {
        i = 2;
    } 
    
    music_get_fileinfo(&music_fileinfo);//获取采样率,比特率
    
    libc_memcpy(g_music_info_buf, nameptr, i);

    g_music_info_buf[i] = ' ';
    
    music_itoa(music_fileinfo.bitrate, tmp_str);
    m = libc_strlen(tmp_str);
    libc_strncat(g_music_info_buf+i+1, tmp_str, m);
    
    m = i + m + 1;
    libc_strncat(g_music_info_buf+m, string2, 4);
    
    g_music_info_buf[m + 4] = ' ';
    
    music_itoa(music_fileinfo.sample_rate, tmp_str);
    n = libc_strlen(tmp_str);
    libc_strncat(g_music_info_buf + m + 5, tmp_str, n);
    
    m = m + n + 5;
    libc_strncat(g_music_info_buf + m, string1, 3);
    
    g_music_info_buf[m + 3] = 0;//结束符
    
}
/******************************************************************************/
/*!
 * \par  Description:
 * \void void play_scene_getid3(file_path_info_t* path_info)
 * \获取id3信息
 * \param[in]    path_info文件的路径信息  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_getid3.c
 * \note
 */
/*******************************************************************************/
void play_scene_getid3(file_path_info_t* path_info)
{
    id3_type_e music_type;
    mengine_playinfo_t playinfo;
    uint8* nameptr = path_info->file_path.dirlocation.filename;
    
    //初始化变量
    play_scene_initid3buf();

    //获取当前文件类型
    music_type = play_scene_checktype(nameptr);
    //定位到文件
    if (fsel_browser_set_file(&(path_info->file_path.plist_location), path_info->file_source) == FALSE)
    {
        return;
    }
    
    get_musicfile_info(nameptr);
    music_get_playinfo(&playinfo);
    //是否取album art信息
    g_id3_info.apic_flag = 0;
    if (g_music_config.album_art_support == TRUE)
    {
        g_id3_info.apic_flag = 1;
    }
    
    g_id3_info.apic_offset = (uint32) & (album_art_info.cluster_no);

    //获取id3信息
    get_id3_info(&g_id3_info, NULL, music_type);
    
    //处理id3超长等
    play_scene_deal_id3();
    
    //初始化为没有专辑图片
    g_status_deal_flag &= clr_AlbumART;

    //前台是正在播放界面非屏保
    if (playinfo.ui_type == 0)
    {
        //check album art是否存在
        if (g_id3_info.apic_type == 1)
        {
            //专辑图片在文件中的偏移
            album_art_info.offset = g_id3_info.apic_offset;
            album_art_info.apic_type = 0;
            g_status_deal_flag |= up_AlbumART;
        }
        else if (g_id3_info.apic_type == 2)
        {
            album_art_info.apic_type = 1; 
            g_status_deal_flag |= up_AlbumART;
        }
        else
        {
            ;//for QAC
        }
    }

    /*if ((g_audible_now == TRUE) && (g_music_status.play_status == PlaySta))
    {
        //主动获取专辑图片
        music_get_audible_image(&playinfo);
    }*/

    if (g_music_config.lrc_support == TRUE)
    {
        g_lyric_flag = playinfo.cur_lyric;
        
        if (g_lyric_flag != 0)
        {
            if(lyric_get_init() == FALSE)
            {
                g_lyric_flag = 0;
            }
        }
    }
}
