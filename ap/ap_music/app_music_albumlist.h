/******************************************************************************
 *                               US212A
 *                            Module: PICTURE_PREVIEW
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan      2011-12-9 13:57:39          1.0              build this file
 ******************************************************************************/
/*!
 * \file     picture_preview.h
 * \brief    ͼƬ����ͼģ��ͷ�ļ�
 * \author   wuyufan
 * \version  1.0
 * \date     2011-12-9
 *******************************************************************************/
#ifndef _PICTURE_PREVIEW_H
#define _PICTURE_PREVIEW_H

#include "app_music.h"
#include  "mmm_id.h"

#define PIC_PER_ROW     3
#define PIC_PER_COLUME  3
#define PIC_ONE_SCREEN (PIC_PER_ROW * PIC_PER_COLUME)
#define PIC_PREV_VIEW_WIDTH  40
#define PIC_PREV_VIEW_HEIGHT 40
#define PIC_FRAME_WIDTH  44
#define PIC_FRAME_HEIGHT 2

#define MOVE_UP   0
#define MOVE_DOWN 1
#define MOVE_PREV 2
#define MOVE_NEXT 3

//ר��ͼƬ��С��Ϣ
#define ALBUMART_X      0
#define ALBUMART_Y      28
#define ALBUMART_W      150
#define ALBUMART_H      150
#define ALBUM_PIC_SIZE  250*512//��һ��������Ϊ�ļ�ͷ��־

extern void scroll_play_flag(void);

#endif

/*! \endcond */


