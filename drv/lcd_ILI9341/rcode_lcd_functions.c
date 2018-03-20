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
 * \file     lcd_functions.c
 * \brief    lcd物理控制器的操作控制模块
 * \author   liutingting
 * \version  1.0
 * \date  2011/9/20
 *******************************************************************************/
#include "lcd_inner.h"

void write_command(uint8 cmd)
{
    //RS输出低电平
    reg_writel(reg_readl(EXTMEM_CTL) & (~(0x00000001 << EXTMEM_CTL_RS)), EXTMEM_CTL)
    reg_writel(cmd, EXTMEM_DATA)
//    #ifdef __WELCOME__
//    welcome_delay_us(1);
//    #else
//   sys_udelay(1);
//   #endif
}

void write_data(uint8 *data_buf, uint16 pix_cnt)
{
    //写data
    uint16 i;
    reg_writel(reg_readl(EXTMEM_CTL) | (0x00000001 << EXTMEM_CTL_RS), EXTMEM_CTL)
    for (i = 0; i < pix_cnt; i++)
    {
        reg_writel(data_buf[i], EXTMEM_DATA)
//    #ifdef __WELCOME__
//    welcome_delay_us(1);
 //   #else
 //   sys_udelay(1);
  //  #endif            
    }
}

#ifdef __WELCOME__
void welcome_delay_ms(uint8 dly_ms)
{
    uint8 i, j;
    while (dly_ms != 0)
    {
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 255; j++)
            {
                ; //do nothing
            }
        }
        dly_ms--;
    }
}

void welcome_delay_us(uint8 dly_us)
{
    uint8 i;
    while (dly_us != 0)
    {
        for (i = 0; i < 2; i++)
        {
            ;//空语句，查看lst确定循环体执行周期数
        }
        dly_us--;
    }
}
#endif//__WELCOME__


/*! \cond LCD_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *       将buffer中的数据送屏
 * \param[in]   pix_cnt: 要写的像素点个数
 * \param[in]   buff: 写数据的buffer
 * \return      none
 * \ingroup     lcd_write_read
 * \par         exmaple code
 * \code
 *          uint8 *display_buffer = (uint8 *) display_buf_addr;
 *          uint16 pix_cnt = 1024;
 *          lcd_buff_data_trans(display_buffer,pix_cnt);
 * \endcode
 * \note
 *******************************************************************************/
#ifndef PC
void buff_data_trans(uint8 *buff, uint16 pix_cnt, void *null3)
{
    //写屏，传输一个lcd buffer
//    uint32 cpuclk_ctl, cpuclk_ctl_data; //保存memory 时钟状态
    store_ce();
    //CPU CLK 24M
/*    cpuclk_ctl = cpuclk_ctl_data = reg_readl(CMU_SYSCLK);    
    cpuclk_ctl_data &= ((~CMU_SYSCLK_CORE_CLKSEL_MASK) & (~(0x00000001 << CMU_SYSCLK_AHBCLKDIV)));
    cpuclk_ctl_data |= ((0x03 << CMU_SYSCLK_CORE_CLKSEL_SHIFT));
    reg_writel(cpuclk_ctl_data, CMU_SYSCLK)*/
        
    write_data(buff, pix_cnt * 2);
//    reg_writel(cpuclk_ctl, CMU_SYSCLK)

    restore_ce();
}
#endif

/******************************************************************************/
/*!
 * \par  Description:
 *       读屏
 * \param[in]   pix_cnt: 要读取的像素点个数
 * \param[out]  buff: 保存读取的屏幕数据的buffer
 * \return      none
 * \ingroup     lcd_write_read
 * \par         exmaple code
 * \code
 *          uint8 *display_buffer = (uint8 *) display_buf_addr;
 *          uint16 pix_cnt = 1024;
 *          lcd_get_buff_data(display_buffer,pix_cnt);
 * \endcode
 * \note
 *******************************************************************************/
#ifndef PC
//该接口只能读入通过DMA传输像素点数据
void get_buff_data(uint8 *buff, uint16 pix_cnt, void *null3)
{
//    uint32 cpuclk_ctl, cpuclk_ctl_data; //保存memory 时钟状态
    uint16 i;      

    store_ce();
    sys_udelay(1);
/*    cpuclk_ctl = cpuclk_ctl_data = reg_readl(CMU_SYSCLK);
    cpuclk_ctl_data &= ((~CMU_SYSCLK_CORE_CLKSEL_MASK) & (~(0x00000001 << CMU_SYSCLK_AHBCLKDIV)));
    cpuclk_ctl_data |= (0x03 << CMU_SYSCLK_CORE_CLKSEL_SHIFT);
    cpuclk_ctl_data |= (0x01 << CMU_SYSCLK_AHBCLKDIV);    
    reg_writel(cpuclk_ctl_data, CMU_SYSCLK)*/


    write_command(LCD_CMD_READ);
    reg_writel(reg_readl(EXTMEM_CTL) | (0x00000001 << EXTMEM_CTL_RS), EXTMEM_CTL)
    //先空读一次
    reg_readb(EXTMEM_DATA);
    for (i = 0; i < pix_cnt; i++)
    {
        //EXTMEM_DATA 依次是 red green blue data
        *(uint16 *)buff = ( ((uint16) (reg_readb(EXTMEM_DATA) & 0xf8) << 8) 
                          | ((uint16) (reg_readb(EXTMEM_DATA) & 0xfc) << 3) 
                          | ((uint16) (reg_readb(EXTMEM_DATA) & 0xf8) >> 3) );
                          
        buff += 2;
    }
//    reg_writel(cpuclk_ctl, CMU_SYSCLK)
    restore_ce();

}
#endif

/*! \endcond */
