/*******************************************************************************
 *                              US212A
 *                            Module: lcd driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       liutingting     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     lcd_hardware_init.c
 * \brief    lcd 硬件初始化模块
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               实现lcd 硬件初始化
 * \par      EXTERNALIZED FUNCTIONS:
 *               none
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "lcd_inner.h"


//8BITS LCD
const uint8 lcd_init_buf[] =
{
    0xfd,0xcf,0x00,0xc3,0x30,
    0xfd,0xed,0x64,0x03,0x12,0x81,
    0xfd,0xe8,0x85,0x00,0x78,
    0xfd,0xcb,0x39,0x2c,0x00,0x34,0x02,
    0xfd,0xf7,0x20,
    0xfd,0xea,0x00,0x00,
    0xfd,0xc0,0x19,
    0xfd,0xC1,0x11,
    0xfd,0xc5,0x3c,0x3f,
    0xfd,0xC7,0x96,
    0xfd,0x36,0x08,
    0xfd,0x3a,0x55,
    0xfd,0xb1,0x00,0x17,
    0xfd,0xb6,0x0a,0xa2,
    0xfd,0xf6,0x01,0x30,
    0xfd,0xf2,0x00,
    0xfd,0x26,0x01,
//------------------------------------ Gamma Sequence-----------------------------------------//
    0xfd,0xe0,0x0f,0x26,0x22,0x0a,0x10,0x0a,0x4c,0xca,0x36,0x00,0x15,0x00,0x10,0x10,0x00,//
    0xfd,0xe1,0x00,0x19,0x1b,0x05,0x0f,0x05,0x33,0x35,0x49,0x0f,0x1f,0x0f,0x3f,0x3f,0x0f,//
//------------------------------------End 

    0x00fd,0x0011,
    0x00fe,0x78,    
    0xfd,0x29,
    0xff  
};


/******************************************************************************/
/*!
 * \par  Description:
 *    LCD reset
 * \param[in]   none
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void lcd_reset(void)
{
    //enable RST
    reg_writel(act_readl(LCMRST_GIO_EN_REG) | LCMRST_GIO_EN_BIT, LCMRST_GIO_EN_REG)
    //RST=1
    reg_writel(act_readl(LCMRST_GIO_DATA_REG) | LCMRST_SET_BIT, LCMRST_GIO_DATA_REG)
    #ifdef __WELCOME__
    welcome_delay_ms(10);
    #else
    sys_mdelay(10);
    #endif
    //RST=0
    reg_writel(act_readl(LCMRST_GIO_DATA_REG) & LCMRST_CLR_BIT, LCMRST_GIO_DATA_REG)
    #ifdef __WELCOME__
    welcome_delay_ms(15);
    #else
    sys_mdelay(15);
    #endif
    //RST=1
    reg_writel(act_readl(LCMRST_GIO_DATA_REG) | LCMRST_SET_BIT, LCMRST_GIO_DATA_REG)
    #ifdef __WELCOME__
    welcome_delay_ms(120);
    #else
    sys_mdelay(120);
    #endif
}

/******************************************************************************/
/*!
 * \par  Description:
 *    LCD init buf
 * \param[in]   none
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void lcd_buf_init(void)
{
    uint8 i = 0;
    store_ce();
    while ((i < sizeof(lcd_init_buf)) && (lcd_init_buf[i] != 0xff))
    {
        if (lcd_init_buf[i] == 0xfd)
        {
            i++;
            if (i < sizeof(lcd_init_buf))
            {
                write_command(lcd_init_buf[i]); //写CMD
            }
        }
        else if (lcd_init_buf[i] == 0xfe)
        {
            i++;
            if (i < sizeof(lcd_init_buf))
            {
                #ifdef __WELCOME__
                welcome_delay_ms(lcd_init_buf[i]);
                #else
                sys_mdelay(lcd_init_buf[i]);
                #endif
            }
        }
        else
        {
            write_data((uint8 *) (&lcd_init_buf[i]), 1);
        }
        i++;
    }
    restore_ce();
}


/******************************************************************************/
/*!
 * \par  Description:
 *    LCD物理控制器初始化
 * \param[in]   lcd_mode：数据传输模式，RGB565  或 YUV420
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void lcd_clock_init(void)
{

    //enable LCD clock
    reg_writel(reg_readl(CMU_DEVCLKEN) | (0x00000001 << CMU_DEVCLKEN_LCDCLKEN), CMU_DEVCLKEN)
    //lcd reset
    reg_writel(reg_readl(MRCR) | (0x00000001 << MRCR_LCDReset), MRCR)
    //LCD select HOSC
//    reg_writel(reg_readl(CMU_LCDCLK) & (~(0x00000001 << CMU_LCDCLK_LCDCLKSEL)), CMU_LCDCLK)    
    
    //LCD select CORE_PLL     
    reg_writel((reg_readl(CORE_PLL_CTL) &(~CORE_PLL_CTL_SCORE_MASK) )| 0x8c, CORE_PLL_CTL)    
#ifdef __WELCOME__     
    welcome_delay_us(100);
#endif
    reg_writel(reg_readl(CMU_LCDCLK) | (0x00000001 << CMU_LCDCLK_LCDCLKSEL), CMU_LCDCLK)    

    //LCD clock div 24M
    reg_writel(reg_readl(CMU_LCDCLK) & (~CMU_LCDCLK_LCDCLKDIV_MASK) , CMU_LCDCLK)

    //CLKHDU:0;CLKLDU:0; LCD CLK=24M/2=12M
    reg_writel(0x00000000, LCD_CLKCTL)
    //EXCLKH:0;EXCLKL:0; EXTMEM_CLK=AHB_clk/2=6M
    reg_writel(0x00000000, EXTMEM_CLKCTL)
    //enable DMA clock
    reg_writel(reg_readl(CMU_DEVCLKEN) | (0x00000001 << CMU_DEVCLKEN_DMACLKEN), CMU_DEVCLKEN)
    //DMA reset;
    reg_writel(reg_readl(MRCR) | (0x00000001 << MRCR_DMA012345Reset), MRCR)    

}

/******************************************************************************/
/*!
 * \par  Description:
 *    LCD 硬件初始化
 * \param[in]   none
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
/******************************************************************************/
/*!
 * \par  Description:
 *       LCD 硬件初始化
 * \param[in]   none
 * \return      none
 * \ingroup     lcd_control
 * \par         exmaple code
 * \code
 *          //进行 lcd 硬件初始化
 *          lcd_hardware_init();
 * \endcode
 * \note
 * \li  该接口一般在 welcome 中调用，完成 lcd 硬件初始化，然后显示欢迎界面。
 * \li  如果系统没有 welcome ，那么需要在 lcd 驱动初始化时调用。
 * \li  如果系统在某些特定条件下会破坏 lcd 硬件状态，比如意外发送了 Reset 信号，那么
 *      需要重新调用该接口进行硬件初始化。
 *******************************************************************************/
void lcd_hardware_init(void)
{
    lcd_clock_init();
    lcd_reset(); 
    lcd_buf_init(); 
}

