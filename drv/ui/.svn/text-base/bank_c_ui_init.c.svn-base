/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       liutingting     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     ui_init.c
 * \brief    UI?????
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               ??UI?????????
 * \par      EXTERNALIZED FUNCTIONS:
 *              ?????????buffer???????????buffer???,??????
 * \version 1.0
 * \date  2011-9-05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
#include "vm_def.h"

#if (USE_MULTI_FONTLIB == 0)
const uint8 FontLibName[] = "UNICODE.FON";

#elif (USE_NEW_FONTLIB)
const uint8 FontLib_S_Name[] = "NEW_S.FNT";
#ifndef  _CARD_BASE_
const uint8 FontLib_Name[]   = "NEW_M.FNT";
#else
const uint8 FontLib_Name[]   = "NEW_MC.FNT";
#endif
const uint8 FontLib_L_Name[] = "NEW_L.FNT";
#else
const uint8 FontLib_S_Name[] = "UNICODES.FON";
const uint8 FontLib_Name[] = "UNICODE.FON";
const uint8 FontLib_L_Name[] = "UNICODEL.FON";
#endif


#if (USE_NEW_FONTLIB)
/* ???????? */
Font_FileInfo_t* Font_GetFileInfo(sd_file_t* fontFile)
{
    Font_FileInfo_t*  fileInfo = &Font_FileInfo;
    
    sys_sd_fseek(fontFile, SEEK_SET, 0);
    sys_sd_fread(fontFile, fileInfo, sizeof(Font_FileInfo_t));

    Font_Printf(__LSTR("  %s: %dx%d.\n"), __FUNCTION__, fileInfo->charWidth, fileInfo->charHeight);

    return fileInfo;
}
#endif

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    UI??????
 * \param[in]   none
 * \param[out]  none
 * \return      bool?????,????
 * \ingroup     install
 * \note
 *******************************************************************************/
int ui_init(void *null0, void*null1, void*null2)
{
#if (USE_MULTI_FONTLIB == 0)
    FontLib_fp = sys_sd_fopen(FontLibName);
    fix_font_height = 16;
    fix_font_height_actual = 16;
    fix_font_height_up_margin = 0;
    fix_font_height_line = 20;
    fix_font_width = 16;
    fix_font_size = 33;
    fix_p_char_point = char_point_buffer;
#else

#if ((USE_MULTI_FONTLIB & 0x01) != 0)
    FontLib_fp = sys_sd_fopen(FontLib_S_Name);
    libc_memcpy(&fix_font_files[0], FontLib_fp, sizeof(sd_file_t));
    sys_sd_fclose(FontLib_fp);
#endif
    
#if ((USE_MULTI_FONTLIB & 0x04) != 0)
    FontLib_fp = sys_sd_fopen(FontLib_L_Name);
    libc_memcpy(&fix_font_files[2], FontLib_fp, sizeof(sd_file_t));
    sys_sd_fclose(FontLib_fp);
#endif
    
    FontLib_fp = sys_sd_fopen(FontLib_Name);
    libc_memcpy(&fix_font_files[1], FontLib_fp, sizeof(sd_file_t));

#if (USE_NEW_FONTLIB)
    {
        Font_FileInfo_t*  fileInfo = Font_GetFileInfo(FontLib_fp);
        
        fix_current_font          = 1;
        fix_font_height           = fileInfo->charHeight;
        fix_font_height_actual    = fileInfo->charHeight;
        fix_font_height_up_margin = 0;
        fix_font_height_line      = fix_font_height_actual + (fix_font_height_actual / 4);
        fix_font_width            = fileInfo->charWidth;
        fix_font_size             = fileInfo->bitsSizePerChar;
        fix_p_char_point          = fix_char_point_buffer;
    }
#else
    fix_current_font = 1;
    fix_font_height = FONTLIB_2_HEIGHT;
    fix_font_height_actual = FONTLIB_2_HEIGHT_ACTUAL;
    fix_font_height_up_margin = (fix_font_height - fix_font_height_actual)/2;
    fix_font_height_line = fix_font_height_actual + (fix_font_height_actual/4);
    fix_font_width = FONTLIB_2_WIDTH;
    fix_font_size = FONTLIB_2_SIZE;
    fix_p_char_point = fix_char_point_buffer;
#endif
#endif

    set_display_buf(LCD_RAM1_INDEX, 0, 0);//??? JPEG_RAM ??

    g_ap_res.res_fp = NULL;
    g_com_res.res_fp = NULL;
    rotate_flag = FALSE; 
    lang_option = 0;
    region_win.x = 0;
    region_win.y = 0;
    region_win.width = DISPLAY_LENGTH;
    region_win.height = DISPLAY_HEIGHT;

    char_fill_hook = ramf_char_fill_hook;
    dma_buffer_data_hook = ramf_dma_buffer_data_hook;
    read_str_bgpic_hook = ramf_read_str_bgpic_hook;
    get_next_char_point_hook = ramf_get_next_char_point_hook;
    next_char_ansi_hook = ramf_next_char_ansi_hook;
    next_char_unicode_hook = ramf_next_char_unicode_hook;
    set_lcd_param_hook = ramf_set_lcd_param_hook;

    put_sd_image = romf_put_sd_image;
    black_char_to_color = romf_black_char_to_color;
    char_fill = romf_char_fill;
    get_next_char_type_unicode = romf_get_next_char_type_unicode;
    get_next_char_type_ansi = romf_get_next_char_type_ansi;
    search_cache = romf_search_cache;
    put_char = romf_put_char;
    read_str_bgpic = romf_read_str_bgpic;

#ifndef PC
    arabic_string_buf = 0;
    //if(sys_malloc(&arabic_string_buf, STRING_BUFFER_LEN) == -1)//??????
    arabic_string_buf = sys_malloc(STRING_BUFFER_LEN);//??????
    if(arabic_string_buf == NULL)
    {
#ifdef UI_DEBUG
        bool loop = TRUE;
        while (1)//?????
        {
            loop = TRUE;
        }
#endif
    }
#endif

    return 0;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    UI?????
 * \param[in]   none
 * \param[out]  none
 * \return      none
 * \ingroup     install
 * \note
 *******************************************************************************/
int ui_exit(void *null0, void *null1, void *null2)
{
    sys_sd_fclose(FontLib_fp); //??????
    sys_sd_fclose(MBtoUniTab_fp); //?????????

#ifndef PC
    //sys_free(&arabic_string_buf);
    sys_free(arabic_string_buf);
#endif

    return 0;
}
/*! \endcond */

#ifndef PC
module_init(ui_init)
module_exit(ui_exit)
#endif
