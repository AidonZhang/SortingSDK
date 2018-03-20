/*******************************************************************************
 *                              us212A
 *                            Module: ui驱动
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-9 16:23     1.0             build this file
 *******************************************************************************/
/*!
 * \file     lcd_driver.h
 * \brief    lcd物理驱动模块相关数据结构，宏定义，接口声明等
 * \author   liutinging
 * \version 1.0
 * \date  2011-9-9
 *******************************************************************************/

#ifndef _LCD_INNER_H
#define _LCD_INNER_H
#include "psp_includes.h"
#include "lcd_driver.h"


/*! LCD模组实际的分辨率的实际长度 */
#define LCD_WIDTH 320
/*! LCD模组分辨率的实际宽度 */
#define LCD_HEIGHT 240

/*! 132X162 的LCD居中显示时，相对于LCD模组长度的起始偏移 */
#define LCD_WIDTH_OFFSET  0
/*! 32X162 的LCD居中显示时，相对于LCD模组宽度的起始偏移 */
#define LCD_HEIGHT_OFFSET 0

#define LCD_CMD_STANDBY             0x10
#define LCD_CMD_EXIT_SLEEP         0x11
#define LCD_CMD_DISPLAY_OFF     0x28
#define LCD_CMD_DISPLAY_ON      0x29
#define LCD_CMD_SET_X           0x2a
#define LCD_CMD_SET_Y           0x2b
#define LCD_CMD_WRITE           0x2c
#define LCD_CMD_READ            0x2e
#define LCD_CMD_SETMODE         0x36
//display mode


#define LCD_PAD_LEVEL1_0    0x09249249
#define LCD_PAD_LEVEL1_1    0x0bfc9249  
#define LCD_PAD_LEVEL4_0    0x1b24b6db
#define LCD_PAD_LEVEL4_1    0x0965b6db
#define LCD_PAD_LEVEL6_0    0x2d64db6d
#define LCD_PAD_LEVEL6_1    0x0b66db6d
#define LCD_PAD_LEVEL7_0    0x3664c924
#define LCD_PAD_LEVEL7_1    0x0b676db6
#define LCD_PAD_LEVEL8_0    0x3f64c924
#define LCD_PAD_LEVEL8_1    0x0b67ffff
//内部函数
extern void store_ce(void)__FAR__;
extern void restore_ce(void)__FAR__;
extern void store_ce_gio(void)__FAR__;
extern void restore_ce_gio(void)__FAR__;
extern void write_data(uint8 *data_buf, uint16 pix_cnt)__FAR__;
extern void write_command(uint8 cmd)__FAR__;
extern void set_mfp_to_emif(void)__FAR__;
extern void set_mfp_to_lcd(void)__FAR__;
extern void lcd_hardware_init(void)__FAR__;

#ifdef WELCOM_PRINT 
extern void print(uint8 *s, uint32 Data, uint8 mode);
#endif

#ifdef __WELCOME__
void welcome_delay_us(uint8 dly_us);
void welcome_delay_ms(uint8 dly_ms);
#endif//__WELCOME__

#endif //_LCD_INNER_H
