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
 * \brief    ����DMA�������ݴ����ģ��
 * \author   liutingting
 * \version  1.0
 * \date  2011/9/20
 *******************************************************************************/
#include "lcd_inner.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    ����DMA����
 * \param[in]    dma_ram��dma��Դ��ַbuffer���ͣ�LCD_RAM_INDEX��ʾUI 1K buffer��JEPG_RAM_INDEX��ʾJPEG 8K buffer��
 *                        JPEG_CODEC_RAM_INDEX��ʾjpeg codec buffer
 * \param[out]   none
 * \return       �����Ƿ�ɹ�
 * \retval       TRUE ����ɹ�
 * \retval       FALSE ����ʧ��
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
        ; //�ȴ�DMA����
    }

    while ((reg_readl(LCD_CTL) & (0x00000001 << LCD_CTL_LCDFI)) == 0)
    {
        ; //�ȴ�LCD�������
    }

    //    sys_local_irq_restore(irq_status);

    return TRUE;
}
        /******************************************************************************/
        /*!
         * \par  Description:
         *    ����DMA����Ĳ���
         * \param[in]    pix_cnt��ҪDMA�����ͼ��ߴ�(��*��)
         * \param[in]    data_width��ҪDMA�����ͼ��Ŀ��
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
         *    ����DMA��Դ��ַ
         * \param[in]    addr0��Դ��ַ0
         * \param[in]    addr1��Դ��ַ1
         * \param[in]    addr2��Դ��ַ2
         * \param[out]   none
         * \return       none
         * \ingroup     lcd_dma_trans
         * \par         exmaple code
         * \code
         *          ����1: RGBģʽ
         *          lcd_dma_set_src_addr(addr0);
         *
         *          ����2: YUV420��YUV444ģʽ
         *          lcd_dma_set_src_addr(addr0,addr1,addr2);
         * \endcode
         * \note
         *******************************************************************************/
        void dma_set_src_addr(uint32 addr0, uint32 addr1, uint32 addr2)
        {
            reg_writel(addr0, DMA1SADDR0)

        }

