/*******************************************************************************
 *                              US212A
 *                            Module: lcd driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       liutingting     2011-09-20 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     lcd_functions_3.c
 * \brief    lcd物理控制器的操作控制模块
 * \author   liutingting
 * \version  1.0
 * \date  2011/9/20
 *******************************************************************************/
#include "./../lcd_inner.h"
/*! \cond LCD_DRIVER_API */

void store_ce(void)
{

    if (!reent_flag)
    {
        #ifndef __WELCOME__
        irq_status = sys_local_irq_save(); //关中断
        #endif
        //片选CE
        set_mfp_to_emif(); //set MFP to External Memory I/F
        store_ce_gio();
    }
    reent_flag++;

}

void restore_ce(void)
{

    reent_flag--;
    if (!reent_flag)
    {
        //释放CE
        restore_ce_gio();
        #ifndef __WELCOME__
        sys_local_irq_restore(irq_status); //开中断
        #endif
    }
    else if (reent_flag < 0)
    {
        reent_flag = 0;
    }
    else
    {
        //do nothing for QAC
    }

}

/******************************************************************************/
/*!
 * \par  Description:
 *       设置LCD的显示窗口
 * \param[in]   win_region：要设置的窗口区域
 * \param[out]  none
 * \return      none
 * \ingroup     lcd_settings
 * \par         exmaple code
 * \code
 *          region_t kregion;
 *          kregion.x = 10;
 *          kregion.y = 20;
 *          kregion.width = 50;
 *          kregion.height = 80;
 *          lcd_set_window(&kregion);
 * \endcode
 * \note
 *******************************************************************************/
#ifndef PC
void set_window(region_t *win_region, void *null2, void *null3)
{
    //设窗
    uint16 x_start, x_end;
    uint16 y_start, y_end;

    uint8 word_data[4] = {0, 0, 0, 0};
    bool w_h_change_flag = FALSE;


    store_ce();
    //Lcd_heightoffset
    x_start =  win_region->x ;
    x_end = win_region->x + win_region->width - 1;

    y_start = win_region->y;
    y_end = win_region->y + win_region->height - 1;

    if (pic_mode == MVENDFLAG) //xy互换
    {
        w_h_change_flag = TRUE;
    }
    else if (pic_mode == SCROLLFLAG)
    {
        x_start =win_region->y;
        x_end =win_region->y + win_region->height - 1;

        y_start = win_region->x;
        y_end = win_region->x + win_region->width - 1;

        w_h_change_flag = TRUE;
    }
    else if (pic_mode == MYENDFLAG)
    {
        y_start = DISPLAY_HEIGHT - (win_region->y + win_region->height);
        y_end = DISPLAY_HEIGHT - 1 - win_region->y;
    }
    else //if (pic_mode == MNONEFLAG)
    {
    }
    //else//MXENDFLAG 暂时不用
    //{
    //    x_start = (uint8) (DISPLAY_LENGTH - (win_region->x + win_region->width - 1));
    //    x_end = (uint8) (DISPLAY_LENGTH - win_region->x);
    //}

/*    if (w_h_change_flag == TRUE)
    {
        x_start += LCD_HEIGHT_OFFSET;
        x_end += LCD_HEIGHT_OFFSET;
        y_start += LCD_WIDTH_OFFSET;
        y_end += LCD_WIDTH_OFFSET;
    }
    else
    {
        x_start += LCD_WIDTH_OFFSET;
        x_end += LCD_WIDTH_OFFSET;
        y_start += LCD_HEIGHT_OFFSET;
        y_end += LCD_HEIGHT_OFFSET;
    }*/

    //set x start and end
    write_command(LCD_CMD_SET_X);
    word_data[0] = (uint8)((x_start & 0xff00)>>8);
    word_data[1] = (uint8)(x_start & 0x00ff) ;
    word_data[2] = (uint8)((x_end & 0xff00)>>8 );
    word_data[3] = (uint8)(x_end & 0x00ff) ;
    write_data(word_data, 4);

    //set y start and end
    write_command(LCD_CMD_SET_Y);
    word_data[0] = (uint8)((y_start & 0xff00)>>8) ;
    word_data[1] = (uint8)(y_start & 0x00ff) ;
    word_data[2] = (uint8)((y_end & 0xff00)>>8 );
    word_data[3] = (uint8)(y_end & 0x00ff) ;
    write_data(word_data, 4);

    write_command(LCD_CMD_WRITE); //设完窗后应开始写屏
    restore_ce();


}
#endif
/******************************************************************************/
/*!
 * \par  Description:
 *       设置LCD的显示模式
 * \param[in]   mode：显示模式
 * \param[out]  none
 * \return      none
 * \ingroup     lcd_settings
 * \par         exmaple code
 * \code
 *          lcd_set_draw_mode(DRAW_MODE_H_DEF);
 * \endcode
 * \note
 *******************************************************************************/
#ifndef PC
void set_draw_mode(uint8 mode, void *null2, void *null3)
{
    //设置刷屏模式
    uint8 data_mode;
    store_ce();


    write_command(LCD_CMD_SETMODE);
    lcd_draw_mode = mode;

    switch (lcd_draw_mode)
    {
        case DRAW_MODE_H_DEF:
        data_mode = 0xc0 | RGBFLAG;
        pic_mode = MNONEFLAG;
        break;

        case DRAW_MODE_H_PIC_DEF:
        data_mode = 0x40 | RGBFLAG;
        pic_mode = MYENDFLAG;
        break;

        case DRAW_MODE_V_PIC_DEF:
        data_mode = 0xe0 | RGBFLAG;
        pic_mode = MVENDFLAG;
        break;

        case DRAW_MODE_V_DEF:
        data_mode = 0xa0 | RGBFLAG;
        pic_mode = MVENDFLAG;
        break;

        case DRAW_MODE_H_SCROLL_DEF:
        data_mode = 0xe0 | RGBFLAG;
        pic_mode = SCROLLFLAG;
        break;

        case DRAW_MODE_V_SCROLL_DEF:
        data_mode = 0x80 | RGBFLAG;
        pic_mode = SCROLLFLAG;
        break;

        default:
        break;
    }
    write_data(&data_mode, 1);
    restore_ce();
}
#endif
/*! \endcond */
