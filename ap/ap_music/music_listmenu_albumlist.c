/*******************************************************************************
 *                              US212A
 *                            Module: PIC_PLAY_SUB
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-12-9 15:04:19           1.0              build this file
 *******************************************************************************/
/*!
 * \file     pic_play_sub.c
 * \brief    ͼƬ������ģ��
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               ��ģ�����ͼƬ���빦�ܣ�ʵ�ֽ����ʼ������ͼƬ�м���Ľ�����
 * \par      EXTERNALIZED FUNCTIONS:
 *               ��Ҫ����ͼƬ�м��ʵ�ֽ��빦��
 * \version 1.0
 * \date    2011-12-9
 *******************************************************************************/
#include "app_music_albumlist.h"
#include "app_music_playing.h"

static image_dsp_param_t image_dsp_param _BANK_DATA_ATTR_;
static image_fs_param_t image_fs_param _BANK_DATA_ATTR_;
static uint32 cmu_memclken_bak _BANK_DATA_ATTR_;
static uint32 cmu_memclksel_bak _BANK_DATA_ATTR_;

#ifdef USE_83NAME_CREAT
static const uint8 album_pic_name[12] = "ALBUM   PIC";
#else
static const uint8 album_pic_name[] = "ALBUM.PIC";
#endif
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
/*!
 * \par  Description: 
 *       bool store_albumart_pic(image_dsp_param_t image_param)
 * \param[in]    image_param    ר��ͼƬ�����Ĵ����ַ��ͼƬ�Ĵ�С
 * \param[in]    mmm_status     ����ɹ�Ϊ1,ʧ��Ϊ0
 * \return  bool success 1
                 fail    0
 * \note    ����Ҫ��ʾ��ר��ͼƬ����ʾ
 */
/*******************************************************************************/
bool store_albumart_pic(image_dsp_param_t image_param, uint8 mmm_status)
{
    /*uint8 ret = TRUE;
    uint8 temp_buf[30];
    handle albumart_p;
    uint32 data_p, pic_size;
   
    //ָ����Ŀ¼
    vfs_cd(g_music_mount_id, CD_ROOT, 0);
    mbyte_to_wchar(temp_buf, (uint8*) album_pic_name);
    albumart_p = vfs_file_open(g_music_mount_id, temp_buf, R_NORMAL_SEEK);
    if (albumart_p == 0)
    {
        //���ļ�ʧ��
        ret = FALSE;
        goto store_album_exit;
    }
    //����ͼƬ��С��Ϣ���ļ�ͷ
    libc_memset(temp_buf, 0x00, 6);
    vfs_file_seek(g_music_mount_id, 16, SEEK_SET, albumart_p);

    if (mmm_status == 1)
    {
        temp_buf[0] = 'O';//����ɹ�
        temp_buf[1] = 'K';//����ɹ�
    }
    else
    {
        temp_buf[0] = 'N';//����ʧ��
        temp_buf[1] = 'O';//����ʧ��
    }

    temp_buf[2] = (uint8)image_param.region2.x;
    temp_buf[3] = (uint8)image_param.region2.y;
    temp_buf[4] = (uint8)image_param.region2.w;
    temp_buf[5] = (uint8)image_param.region2.h;
    vfs_file_write(g_music_mount_id, temp_buf, 6, albumart_p);

    //����ɹ�����ͼƬ����
    if (mmm_status == 1)
    {
        //���¶�λ дͼƬ������Ϣ
        vfs_file_seek(g_music_mount_id, SECTOR_SIZE, SEEK_SET, albumart_p);
        
        data_p = (uint32)image_param.image_addr;
        pic_size = (uint32)(2 * image_param.region2.w * image_param.region2.h);

        vfs_file_write(g_music_mount_id, data_p, pic_size, albumart_p);
    } 
    //�ر�
    vfs_file_close(g_music_mount_id, albumart_p);

    store_album_exit:
    return ret;*/
    return TRUE;

}

/******************************************************************************/
/*!
 * \par  Description: 
 *       bool show_albumart_pic(void)
 * \param[in]    void
 * \param[out]   void
 * \return  bool success 1
                 fail    0
 * \note    ��ʾר��ͼƬ
 */
/*******************************************************************************/
bool show_albumart_pic(void)
{
    uint8  ret = TRUE;
    handle albumart_p;
    uint32 real_size;
    uint32 pic_size;
    wregion_t region1, region2;
    uint8 temp_buf[30];

    uint8 *album_art_buf = (uint8 *)LCD_BUF1_ADDR;

    //ָ����Ŀ¼
    vfs_cd(g_music_mount_id, CD_ROOT, 0);
    
    mbyte_to_wchar(temp_buf, (uint8*) album_pic_name);
    albumart_p = vfs_file_open(g_music_mount_id, temp_buf, R_NORMAL_SEEK);
    if(albumart_p == 0)
    {
        //���ļ�ʧ��
        ret = FALSE;
        g_status_deal_flag &= clr_AlbumART;
        goto show_album_exit;
    }
    //��ȡͼƬ��С��Ϣ
    vfs_file_read(g_music_mount_id, temp_buf, 30, albumart_p);
    if (temp_buf[16] == 'N' && temp_buf[17] == 'O')
    {
        //ͼƬ����ʧ�ܲ���ʾ
        ret = FALSE;
        g_status_deal_flag &= clr_AlbumART;
        goto show_album_exit;
    }
    
    region1.x = temp_buf[18];
    region1.y = temp_buf[19];
    region1.w = temp_buf[20];
    region1.h = temp_buf[21];
    pic_size = region1.w * region1.h * 2;
    //���ͼƬ��ϢС��ר��ͼƬ���ڴ�С������ר��ͼƬ����
    if (pic_size < ALBUMART_W*ALBUMART_H*2)
    {
        //ר��ͼƬ��������
        region2.x = ALBUMART_X;
        region2.y = ALBUMART_Y;
        region2.w = ALBUMART_W;
        region2.h = ALBUMART_H;
        ui_set_backgd_color(0x39e7);//��ɫ
        ui_clear_screen(&region2);
    }

    //��ȡͼ����Ϣ����ʾ
    vfs_file_seek(g_music_mount_id, SECTOR_SIZE, SEEK_SET, albumart_p);

    lcd_set_draw_mode(DRAW_MODE_V_DEF);
    lcd_set_window(&region1);
    
    while(pic_size > 0)
    {
        if(pic_size > region1.w * 20)
        {
            real_size = region1.w * 20;
        }
        else
        {
            real_size = pic_size;
        }

        //���ļ�
        vfs_file_read(g_music_mount_id, album_art_buf, real_size, albumart_p);
        lcd_dma_set_counter(real_size/2, (uint32)region1.w);
        lcd_dma_start_trans(LCD_RAM1_INDEX);
        
        pic_size -= real_size;
    }

    show_album_exit:    
    //close file
    vfs_file_close(g_music_mount_id, albumart_p);

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \static void bak_cmu_memclk(void)
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \ingroup      music_paint_albumart
 * \note
 */
/*******************************************************************************/
static void bak_cmu_memclk(void)
{
    cmu_memclken_bak = *((REG32) (CMU_MEMCLKEN)) ;
    cmu_memclksel_bak = *((REG32) (CMU_MEMCLKSEL));
    
    *((REG32) (CMU_MEMCLKEN)) &= (~((1 << CMU_MEMCLKEN_PCMRAM0CLKEN) | (1 << CMU_MEMCLKEN_PCMRAM1CLKEN) | (1
            << CMU_MEMCLKEN_PCMRAM2CLKEN)));

    // enable pcmram0/1/2 clock
    *((REG32) (CMU_MEMCLKEN)) |= ((1 << CMU_MEMCLKEN_PCMRAM0CLKEN) | (1 << CMU_MEMCLKEN_PCMRAM1CLKEN) | (1
            << CMU_MEMCLKEN_PCMRAM2CLKEN));

    *((REG32) (CMU_MEMCLKSEL)) &= (~(CMU_MEMCLKSEL_PCMRAM0CLKSEL_MASK | CMU_MEMCLKSEL_PCMRAM1CLKSEL_MASK
            | CMU_MEMCLKSEL_PCMRAM2CLKSEL_MASK));
    // select pcmram0/1/2 as asrc clock
    // *((REG32) (CMU_MEMCLKSEL)) |= ((1 << CMU_MEMCLKSEL_PCMRAM0CLKSEL_SHIFT) | (1
    //  << CMU_MEMCLKSEL_PCMRAM1CLKSEL_SHIFT) | (1 << CMU_MEMCLKSEL_PCMRAM2CLKSEL_SHIFT));    

}
/******************************************************************************/
/*!
 * \par  Description:
 * \static recover_cmu_memclk(void)
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \ingroup      music_paint_albumart
 * \note
 */
/*******************************************************************************/
static void recover_cmu_memclk(void)
{
    *((REG32) (CMU_MEMCLKEN)) = cmu_memclken_bak;
    *((REG32) (CMU_MEMCLKSEL)) = cmu_memclksel_bak;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void play_scene_albumart(void)
 * \����ר������album art
 * \param[in]    show_or_store     1 ���� 0 ��ʾ
 * \param[in]    file_offset   ͼƬ���ļ���ƫ�ƣ����ڷ���ǶͼƬֵΪ0
 * \param[in]    region1     ���봰����
 * \param[out]   region2     ���������
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_paint.c
 * \note
 */
/*******************************************************************************/
int32 music_paint_albumart(uint8 show_or_store, uint32 file_offset, wregion_t *region1, wregion_t *region2)
{
    int result = -1;
    style_infor_t style;
    void* mmm_id_handle = NULL;
    uint8 temp_buf[30];
    handle albumart_p;
    uint16 * data_p;

    style.type = UI_AP;
    image_fs_param.file_mount_id = (uint32) g_music_mount_id;
    image_fs_param.data_offset = file_offset;  
    image_fs_param.album_type = album_art_info.apic_type;
    image_dsp_param.region1.x = region1->x;
    image_dsp_param.region1.y = region1->y;
    image_dsp_param.region1.w = region1->w;
    image_dsp_param.region1.h = region1->h;
    image_dsp_param.rgb_mode  = MODE_RGB;
    image_dsp_param.image_addr = 0;

    //���Ƶ��
    adjust_freq_set_level(AP_FRONT_LOW_PRIO, FREQ_LEVEL11, FREQ_LEVEL11);

    //�����м������
    sys_load_mmm("mmm_id.al", FALSE);//������

    //ʹ�ÿռ串���ȱ���
    bak_cmu_memclk();
    //�򿪽����
    mmm_id_cmd(&mmm_id_handle, MMM_ID_OPEN, (uint32) NULL);

    if (mmm_id_handle != NULL)
    {
        //���ý������
        result = mmm_id_cmd(mmm_id_handle, MMM_ID_SET_ALBUM_INFO, (unsigned int) &image_fs_param);
        if (result == 0)
        {      
			
		   if (show_or_store == TRUE)
		   {
				vfs_cd(g_music_mount_id, CD_ROOT, 0);
				mbyte_to_wchar(temp_buf, (uint8*) album_pic_name);
				albumart_p = vfs_file_open(g_music_mount_id, temp_buf, R_NORMAL_SEEK);
				if (albumart_p == 0)
				{
				   result = -1;
				   goto image_decode_end;
				}
				image_dsp_param.file_handle = albumart_p;
				image_dsp_param.file_mount_id = g_music_mount_id;
				
				vfs_file_seek(g_music_mount_id, 512, SEEK_SET, albumart_p);
			 }
			 else
			 {
				 image_dsp_param.file_handle = 0;
			 }
			result = mmm_id_cmd(mmm_id_handle, MMM_ID_DECODE, (unsigned int)&image_dsp_param);
		   
			if (image_dsp_param.status == MMM_ID_ERROR)
			{
				result = -1;
				g_status_deal_flag &= clr_AlbumART;
			}

			else
			{
				data_p = (uint16 *)(image_dsp_param.image_addr);
				region2->w = image_dsp_param.region2.w;
				region2->h = image_dsp_param.region2.h;
				
				image_dsp_param.region2.x = region1->x + (region1->w - region2->w) / 2;//���д���
				image_dsp_param.region2.y = region1->y + (region1->h - region2->h) / 2;//���д���
					  
			   if (show_or_store == TRUE)
			   {
					vfs_file_seek(g_music_mount_id, 16, SEEK_SET, albumart_p);
					
					if (image_dsp_param.status == MMM_ID_SUCCESS)
					{
						temp_buf[0] = 'O';//����ɹ�
						temp_buf[1] = 'K';//����ɹ�
					}
					else
					{
						temp_buf[0] = 'N';//����ʧ��
						temp_buf[1] = 'O';//����ʧ��
					}
						
					temp_buf[2] = (uint8)image_dsp_param.region2.x;
					temp_buf[3] = (uint8)image_dsp_param.region2.y;
					temp_buf[4] = (uint8)image_dsp_param.region2.w;
					temp_buf[5] = (uint8)image_dsp_param.region2.h;
								
					vfs_file_write(g_music_mount_id, temp_buf, 6, albumart_p);
					vfs_file_close(g_music_mount_id, albumart_p);
			   }
			   else
			   {
					lcd_set_draw_mode(DRAW_MODE_V_DEF);
					lcd_set_window(&image_dsp_param.region2);
					lcd_invert_buff_data_trans((void *)data_p, region2->w * region2->h);
					lcd_buff_data_trans((void *)data_p, region2->w * region2->h);
					g_status_deal_flag &= clr_AlbumART;
			   }
			}
		}
	}
image_decode_end: 
	//�رս����
	mmm_id_cmd(mmm_id_handle, MMM_ID_CLOSE, (uint32) NULL);
	sys_free_mmm(FALSE);
	
//�ָ�����
    recover_cmu_memclk();
    adjust_freq_set_level(AP_FRONT_LOW_PRIO, FREQ_LEVEL7, FREQ_NULL);
    
    return result;
    
}


