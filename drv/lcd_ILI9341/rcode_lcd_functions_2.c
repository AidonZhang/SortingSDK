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
 * \file     lcd_functions_2.c
 * \brief    控制DMA进行数据传输的模块
 * \author   liutingting
 * \version  1.0
 * \date  2011/9/20
 *******************************************************************************/
#include "lcd_inner.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    启动DMA传输
 * \param[in]    dma_ram：dma的源地址buffer类型，LCD_RAM_INDEX表示UI 1K buffer，JEPG_RAM_INDEX表示JPEG 8K buffer，
 *                        JPEG_CODEC_RAM_INDEX表示jpeg codec buffer
 * \param[out]   none
 * \return       传输是否成功
 * \retval       TRUE 传输成功
 * \retval       FALSE 传输失败
 * \ingroup     lcd_dma_trans
 * \par         exmaple code
 * \code
 *          uint8 lcd_buffer_type = LCD_RAM_INDEX;
 *          lcd_dma_start_trans(lcd_buffer_type);
 * \endcode
 * \note
 *******************************************************************************/

bool dma_start_trans(uint8 dma_ram, void *null2, void *null3)
{

    //   irq_status = sys_local_irq_save();

    set_mfp_to_lcd();

    reg_writel(reg_readl(LCD_CTL) | ((0x00000001 << LCD_CTL_LCDFI) | (0x00000001 << LCD_CTL_FOVF)), LCD_CTL)

    if (dma_ram == LCD_RAM0_INDEX)
    {
        dma_set_src_addr(LCD_BUF0_ADDR, 0, 0);
    }
    else
    {
        dma_set_src_addr(LCD_BUF1_ADDR, 0, 0);
    }

    reg_writel(reg_readl(DMA1CTL) | 0x00000001, DMA1CTL)
    reg_writel(reg_readl(LCD_CTL) | (0x00000001 << LCD_CTL_EN), LCD_CTL)

    while ((reg_readl(DMA1CTL) & 0x00000001) != 0)
    {
        ; //等待DMA结束
    }

    while ((reg_readl(LCD_CTL) & (0x00000001 << LCD_CTL_LCDFI)) == 0)
    {
        ; //等待LCD传输完成
    }

    //    sys_local_irq_restore(irq_status);

    return TRUE;
}
        /******************************************************************************/
        /*!
         * \par  Description:
         *    设置DMA传输的参数
         * \param[in]    pix_cnt：要DMA传输的图像尺寸(宽*高)
         * \param[in]    data_width：要DMA传输的图像的宽度
         * \param[out]   none
         * \return       none
         * \ingroup     lcd_dma_trans
         * \par         exmaple code
         * \code
         *          lcd_dma_set_counter(scroll->width * scroll->height, scroll->width);
         * \endcode
         * \note
         *******************************************************************************/
        void dma_set_counter(uint32 pix_cnt, uint16 data_width, void *null3)
        {
            reg_writel(pix_cnt, DMA1FrameLen)
        }

        /******************************************************************************/
        /*!
         * \par  Description:
         *    设置DMA的源地址
         * \param[in]    addr0：源地址0
         * \param[in]    addr1：源地址1
         * \param[in]    addr2：源地址2
         * \param[out]   none
         * \return       none
         * \ingroup     lcd_dma_trans
         * \par         exmaple code
         * \code
         *          例子1: RGB模式
         *          lcd_dma_set_src_addr(addr0);
         *
         *          例子2: YUV420或YUV444模式
         *          lcd_dma_set_src_addr(addr0,addr1,addr2);
         * \endcode
         * \note
         *******************************************************************************/
        void dma_set_src_addr(uint32 addr0, uint32 addr1, uint32 addr2)
        {
            reg_writel(addr0, DMA1SADDR0)

        }

