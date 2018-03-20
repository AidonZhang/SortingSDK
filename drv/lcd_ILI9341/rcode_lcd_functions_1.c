/*******************************************************************************
 *                              US212A
 *                            Module: lcd  driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       liutingting     2011-09-20 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     lcd_functions_1.c
 * \brief    lcd物理控制器的操作控制模块
 * \author   liutingting
 * \version  1.0
 * \date  2011/9/20
 *******************************************************************************/
#include "lcd_inner.h"

/*! \cond */
//MFP切到EXT bus
void set_mfp_to_emif(void)
{
    unsigned int mft_ctl0_data, mft_ctl1_data;

    //set EM_RDB to GPIO_A2; set EM_WRB to GPIO_A0; set EM_RS to GPIO_A1
    mft_ctl1_data = reg_readl(MFP_CTL1);

#if (CASE_BOARD_TYPE == CASE_BOARD_EVB)
    mft_ctl1_data &= ((~MFP_CTL1_GPIOA2_MASK) & (~MFP_CTL1_GPIOA1_MASK) & (~MFP_CTL1_GPIOA0_MASK)
            & (~MFP_CTL1_GPIOA3_MASK));
    mft_ctl1_data |= ((0x00000001 << MFP_CTL1_GPIOA2_SHIFT) | (0x00000001 << MFP_CTL1_GPIOA1_SHIFT) | (0x00000001
            << MFP_CTL1_GPIOA0_SHIFT) | (0x00000001 << MFP_CTL1_GPIOA3_SHIFT));

#else
    mft_ctl1_data &= ((~MFP_CTL1_GPIOA2_MASK) & (~MFP_CTL1_GPIOA1_MASK) & (~MFP_CTL1_GPIOA0_MASK)
            & (~MFP_CTL1_GPIOA4_MASK));
    mft_ctl1_data |= ((0x00000001<<MFP_CTL1_GPIOA2_SHIFT) | (0x00000001 << MFP_CTL1_GPIOA1_SHIFT)
            |(0x00000001<<MFP_CTL1_GPIOA0_SHIFT ) |(0x00000001<<MFP_CTL1_GPIOA4_SHIFT ) );
#endif

    reg_writel(mft_ctl1_data, MFP_CTL1)

    //set LCD_D0-D2 to GPIO_A8-10
    mft_ctl1_data &= ((~MFP_CTL1_GPIOA8_MASK) & (~MFP_CTL1_GPIOA9_MASK) & (~MFP_CTL1_GPIOA10_MASK));
    mft_ctl1_data |= ((0x00000001 << MFP_CTL1_GPIOA8_SHIFT) | (0x00000001 << MFP_CTL1_GPIOA9_SHIFT) | (0x00000001
            << MFP_CTL1_GPIOA10_SHIFT));

    reg_writel(mft_ctl1_data, MFP_CTL1)

    //set LCD_D7-D3 to GPIO_A15-11
    mft_ctl0_data = reg_readl(MFP_CTL0);
    mft_ctl0_data &= ((~MFP_CTL0_GPIOA11_MASK) & (~MFP_CTL0_GPIOA12_MASK) & (~MFP_CTL0_GPIOA13_MASK)
            & (~MFP_CTL0_GPIOA14_MASK) & (~MFP_CTL0_GPIOA15_MASK));
    mft_ctl0_data |= ((0x00000001 << MFP_CTL0_GPIOA11_SHIFT) | (0x00000001 << MFP_CTL0_GPIOA12_SHIFT) | (0x00000001
            << MFP_CTL0_GPIOA13_SHIFT) | (0x00000001 << MFP_CTL0_GPIOA14_SHIFT)
            | (0x00000001 << MFP_CTL0_GPIOA15_SHIFT));

    reg_writel(mft_ctl0_data, MFP_CTL0)

}
//MFP切到lcd

void set_mfp_to_lcd(void)
{
    unsigned int mft_ctl0_data, mft_ctl1_data;

    mft_ctl1_data = reg_readl(MFP_CTL1);
#if (CASE_BOARD_TYPE == CASE_BOARD_EVB)
    mft_ctl1_data &= ((~MFP_CTL1_GPIOA0_MASK) & (~MFP_CTL1_GPIOA1_MASK) & (~MFP_CTL1_GPIOA3_MASK));
    mft_ctl1_data |= ((0x00000002 << MFP_CTL1_GPIOA0_SHIFT) | (0x00000002 << MFP_CTL1_GPIOA1_SHIFT) | (0x00000002
            << MFP_CTL1_GPIOA3_SHIFT));
#else
    mft_ctl1_data &= ((~MFP_CTL1_GPIOA0_MASK) & (~MFP_CTL1_GPIOA1_MASK) & (~MFP_CTL1_GPIOA4_MASK) );
    mft_ctl1_data |= ((0x00000002<<MFP_CTL1_GPIOA0_SHIFT ) | (0x00000002 << MFP_CTL1_GPIOA1_SHIFT)
            | (0x00000002 << MFP_CTL1_GPIOA4_SHIFT));
#endif

    reg_writel(mft_ctl1_data, MFP_CTL1)

    //set LCD_D0-D2 to GPIO_A8-A10
    mft_ctl1_data &= ((~MFP_CTL1_GPIOA8_MASK) & (~MFP_CTL1_GPIOA9_MASK) & (~MFP_CTL1_GPIOA10_MASK));
    mft_ctl1_data |= ((0x00000002 << MFP_CTL1_GPIOA8_SHIFT) | (0x00000002 << MFP_CTL1_GPIOA9_SHIFT) | (0x00000002
            << MFP_CTL1_GPIOA10_SHIFT));

    reg_writel(mft_ctl1_data, MFP_CTL1)

    //set LCD_D7-D3 to GPIO_A15-A11

    mft_ctl0_data = reg_readl(MFP_CTL0);
    mft_ctl0_data &= ((~MFP_CTL0_GPIOA11_MASK) & (~MFP_CTL0_GPIOA12_MASK) & (~MFP_CTL0_GPIOA13_MASK)
            & (~MFP_CTL0_GPIOA14_MASK) & (~MFP_CTL0_GPIOA15_MASK));
    mft_ctl0_data |= ((0x00000002 << MFP_CTL0_GPIOA11_SHIFT) | (0x00000002 << MFP_CTL0_GPIOA12_SHIFT) | (0x00000002
            << MFP_CTL0_GPIOA13_SHIFT) | (0x00000002 << MFP_CTL0_GPIOA14_SHIFT)
            | (0x00000002 << MFP_CTL0_GPIOA15_SHIFT));

    reg_writel(mft_ctl0_data, MFP_CTL0)

}

void store_ce_gio(void)
{
    //将外扩总线的CE选为CE0
#if (CASE_BOARD_TYPE == CASE_BOARD_EVB)
    reg_writel((reg_readl(EXTMEM_CTL)&(~EXTMEM_CTL_CESEL_MASK))|(EXTMEM_CE0_SEL << EXTMEM_CTL_CESEL_SHIFT), EXTMEM_CTL)
#else
    reg_writel((reg_readl(EXTMEM_CTL)&(~EXTMEM_CTL_CESEL_MASK))|(EXTMEM_CE1_SEL << EXTMEM_CTL_CESEL_SHIFT), EXTMEM_CTL)
#endif
}

void restore_ce_gio(void)
{
    //将外扩总线的CE选为CE1
#if (CASE_BOARD_TYPE == CASE_BOARD_EVB)
    reg_writel((reg_readl(EXTMEM_CTL)&(~EXTMEM_CTL_CESEL_MASK))|(EXTMEM_CE1_SEL << EXTMEM_CTL_CESEL_SHIFT), EXTMEM_CTL)
#else
    reg_writel((reg_readl(EXTMEM_CTL)&(~EXTMEM_CTL_CESEL_MASK))|(EXTMEM_CE0_SEL << EXTMEM_CTL_CESEL_SHIFT), EXTMEM_CTL)
#endif
}

/******************************************************************************/
/*!
 * \par  Description:
 *       LCD物理控制器初始化
 * \param[in]   lcd_mode：数据传输模式，参见lcd_mode_e定义
 * \param[out]  none
 * \return      none
 * \ingroup     lcd_dma_trans
 * \par         exmaple code
 * \code
 *      例子1: 设置成RGB模式
 *      lcd_controller_init(RGB_MODE);
 *
 *      例子2: 设置成YUV420模式
 *      lcd_controller_init(YUV420_MODE);
 * \endcode
 * \note
 *******************************************************************************/
void controller_init(lcd_mode_e lcd_mode, void *null2, void *null3)
{
    unsigned int lcd_ctl_data;

    lcd_ctl_data = reg_readl(LCD_CTL);

    //RGB sequence; 8bit; enable DRQ
    lcd_ctl_data &= ((~(0x00000001 << LCD_CTL_SEQ)) & (~(0x00000001 << LCD_CTL_FORMATS)));
    lcd_ctl_data |= ((0x00000001 << LCD_CTL_EMDE));
    reg_writel(lcd_ctl_data, LCD_CTL)

    reg_writel((reg_readl(DMA1CTL) & (~0x00000ff0)) | LCD_DMA1CTL_SET , DMA1CTL)
    //clear DMA irq pending bit
    reg_writel(( 1 << DMAIP_DMA1TCIP) | (1 << DMAIP_DMA1HFIP), DMAIP)

}
/*! \endcond */

