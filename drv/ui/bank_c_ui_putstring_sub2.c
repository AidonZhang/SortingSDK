
/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       cailizhen     2012-10-16 9:27     1.0             build this file
 *******************************************************************************/
/*!
 * \file     ui_putstring_sub.c
 * \brief    字符串处理阶段 bank c 接口
 * \author   cailizhen
 * \par      GENERAL DESCRIPTION:
 *           
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2012-10-16
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

#if (USE_NEW_FONTLIB)
/* 获取字符索引 */

int Font_GetCharIndex(sd_file_t* fontFile, uint16 unicode)
{
    Font_PageEntry_t  pageEntry;

    uint16  m = unicode / Font_PageSize;
    uint16  n = unicode % Font_PageSize;

    uint16  i, j;
    
    sys_sd_fseek(fontFile, SEEK_SET, sizeof(Font_FileInfo_t) + m * sizeof(Font_PageEntry_t));
    sys_sd_fread(fontFile, &pageEntry, sizeof(Font_PageEntry_t));

    if (!(pageEntry.codesMask[n/8] & (1 << (n%8))))
    {
        return -1;
    }    
    for (i = j = 0; i < n; i++)
    {
        if (pageEntry.codesMask[i/8] & (1 << (i%8)))
        {
            j++;
        }
    }

    i = pageEntry.charIndex + j;

    return i;
}


/* 获取字符位图数据 */

int Font_GetCharBits(sd_file_t* fontFile, uint16 unicode, uint8* bitsBuf)
{
    Font_FileInfo_t*  fileInfo = &Font_FileInfo;

    int  bpc = fileInfo->bitsSizePerChar;
    int  charIndex;

    if (isspace(unicode))
    {
        unicode = ' ';
    }
    
    if ((charIndex = Font_GetCharIndex(fontFile, unicode)) < 0)
    {

        if ((charIndex = Font_GetCharIndex(fontFile, '?')) < 0)
        {
            return -1;
        }    
    }

    sys_sd_fseek(fontFile, SEEK_SET, fileInfo->bitsOffs + charIndex * bpc);
    sys_sd_fread(fontFile, bitsBuf, bpc);
    
    return bpc;
}
#endif