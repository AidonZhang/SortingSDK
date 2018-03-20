/*!
 * \file  new_font.h
 * \brief 新版字库, 配合 USE_MULTI_FONTLIB 使用.
 */

#ifndef __included_new_font_h
#define __included_new_font_h

#include "display.h"


#define USE_NEW_FONTLIB  1   // 是否使用新版字库

#define FONT_DEBUG       0   // 字库调试
#define FONT_ANTIALIAS   0   // 抗锯齿, 平滑字体
#define FONT_MAX_SIZE    20  // 最大字体大小
#define FONT_MAX_BPP     1   // 最大像素位数


#if (USE_MULTI_FONTLIB && USE_NEW_FONTLIB)

#if (FONT_DEBUG)

#include <stdio.h>

#define Font_Printf  printf
#else
#define Font_Printf(...)  do {} while(0)
#endif


#undef  MAX_CHARPOINTE_SIZE
#define MAX_CHARPOINTE_SIZE  (((FONT_MAX_SIZE * FONT_MAX_BPP + 7) / 8) * FONT_MAX_SIZE + 1)


#define Font_NumPages  1024
#define Font_PageSize  (0x10000 / Font_NumPages)


typedef struct
{
    uint16  charIndex;
    uint8   codesMask[Font_PageSize / 8];
    
} __attribute__((packed)) Font_PageEntry_t;


typedef struct
{
    char    fontFormat[3];    // 字库格式
    uint8   swVersion;        // 软件版本
    
    uint8   fontSize;         // 字体大小
    uint8   charWidth;        // 字符宽度
    uint8   charHeight;       // 字符高度
    int8    offsetX;          // X 偏移
    int8    offsetY;          // Y 偏移
    uint8   bitsPerPixel;     // 每像素位数
    
    uint16  bitsSizePerChar;  // 每字符位图数据大小
    uint16  bitsOffs;         // 字符位图数据偏移
    uint16  numPages;         // 编码页数
    
} Font_FileInfo_t;  // 字库文件信息


extern Font_FileInfo_t  Font_FileInfo;  // 字库文件信息
 

#if (FONT_ANTIALIAS)

#define Font_GetBits(_bitsData, _bitPos, _bitsLen, _bitsVal)        \
do                                                                  \
{                                                                   \
    int  _i, _j;                                                    \
                                                                    \
    _bitsVal = 0;                                                   \
                                                                    \
    for (_i = (_bitsLen) - 1, _j = (_bitPos); _i >= 0; _i--, _j++)  \
    {                                                               \
        if ((_bitsData)[_j/8] & (1 << (7 - (_j%8))))                \
        {                                                           \
            _bitsVal |= (1 << _i);                                  \
        }                                                           \
    }                                                               \
}                                                                   \
while (0)


#define Font_AlphaBlend(_p1, _p2, _alpha)                                         \
do                                                                                \
{                                                                                 \
    if ((_alpha) >= 0xf8)                                                         \
    {                                                                             \
        *_p1 = (_p2);                                                             \
    }                                                                             \
    else if ((_alpha) >= 0x08)                                                    \
    {                                                                             \
        uint32  _v1 = ((*_p1) | ((*_p1) << 16)) & 0x07e0f81f;                     \
        uint32  _v2 = ((_p2) | ((_p2) << 16)) & 0x07e0f81f;                       \
                                                                                  \
        uint32  _v3 = (((_v2 - _v1) * ((_alpha) >> 3) >> 5) + _v1) & 0x07e0f81f;  \
                                                                                  \
        *_p1 = (uint16)((_v3 & 0xffff) | (_v3 >> 16));                            \
    }                                                                             \
}                                                                                 \
while (0)


/* 绘制字符位图 */

static inline void Font_DrawChar(uint16* dispBuf, region_t* dispRect, uint8* charBits, 

    region_t* charRect, char_disp_e alignMode, uint8 drawMode)
{
    Font_FileInfo_t*  fileInfo = &Font_FileInfo;

    uint8   char_w = charBits[fileInfo->bitsSizePerChar - 1];
    
    uint8   bpp   = fileInfo->bitsPerPixel;
    uint8   pitch = (char_w * bpp + 7) / 8;
    
    uint16  color = dc.pen_color;
    
    uint8   i, j;
    uint8   m, n;

    if (alignMode == CHAR_DISP_LEFT2RIGHT)
    {
        charRect->x = charRect->x;
        charRect->width = region.width;
    }
    else if (alignMode == CHAR_DISP_RIGHT2LEFT)
    {
        charRect->x = charRect->x - (region.width - 1);
        charRect->width = region.width;
    }
    else
    {
        charRect->x = 0;
        charRect->width = char_w;
    }

    m = (drawMode == 0) ? charRect->height : charRect->width;
    n = (drawMode == 0) ? charRect->width  : charRect->height;

    for (i = 0; i < m; i++)
    {
        uint16*  p1;
        uint8*   p2, bitPos;

        if (drawMode == 0)
        {
            p1 = dispBuf  + (dispRect->y + i) * dispRect->width + dispRect->x;
            p2 = charBits + (charRect->y + i) * pitch;

            bitPos = charRect->x * bpp;
        }
        else
        {
            p1 = dispBuf  + (dispRect->x + i) * dispRect->height + dispRect->y;
            p2 = charBits + charRect->y * pitch;

            bitPos = (charRect->x + i) * bpp;
        }
        
        for (j = 0; j < n; j++, p1++)
        {
            uint8  bitsVal;
            uint8  alpha;
            
            Font_GetBits(p2, bitPos, bpp, bitsVal);

            alpha = 0xff * bitsVal / ((1 << bpp) - 1);

            Font_AlphaBlend(p1, color, alpha);

            if (drawMode == 0)
                bitPos += bpp;
            else
                p2 += pitch;
        }
    }
}

#endif  // FONT_ANTIALIAS

#endif  // USE_NEW_FONTLIB

#endif  // __included_new_font_h

/* end of new_font.h */


