/*******************************************************************************
 *                              US212A
 *                            Module: udiskap
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       yliang     2011-11-07      1.0             build this file
 *******************************************************************************/
/*!
 * \file     ap_udisk.h
 * \brief
 * \author   yliang
 * \version  1.0
 * \date     2011/10/07
 *******************************************************************************/
#ifndef   _UDISKAP_H_
#define   _UDISKAP_H_

#include  "psp_includes.h"
#include  "case_include.h"
#include  "udisk_sty.h"
#include  "udisk_res.h"
#include "ud_interface.h"

typedef void (*p_func_entry)(void);

#define  POS_CONNECT_X_BASE  115 //18
#define  POS_CONNECT_Y_BASE  177 //96
#define  WIDTH_CONNECT       16 //12
#define  HEIGHT_CONNECT      16 //12
#define  POS_CONNECT_INC_X   30 //12

#define   CONNECT_PIC_BUFFER  (0x38000)                 //(0x23000)              //(0x38000)
#define   CONNECT_PIC_LENGTH  (HEIGHT_CONNECT * WIDTH_CONNECT * 2)//图片尺寸12*12
#define   CONNECT_PIC_FRAME   (10)

#define  POS_BATTERY_X_BASE  270 //104
#define  POS_BATTERY_Y_BASE  4 ////1
#define  WIDTH_BATTERY       41 //20
#define  HEIGHT_BATTERY      20 //13

#define   BAT_PIC_BUFFER  (CONNECT_PIC_BUFFER + CONNECT_PIC_LENGTH * CONNECT_PIC_FRAME)
#define   BATTERY_PIC_LENGTH  (HEIGHT_BATTERY * WIDTH_BATTERY * 2)
#define   BATTERY_PIC_FRAME   (5)
typedef void (*PTRFUNC)(void);



/*!定义udisk应用的任务栈和主线程优先级*/
#define    AP_UDISK_STK_POS  AP_FRONT_LOW_STK_POS
#define    AP_UDISK_PRIO     AP_FRONT_LOW_PRIO


#ifdef SUPPORT_USER_DEF_USBCMD
//定义用户自定义的usb命令
#define    ACT_SET_TIME     0xb0               //通过工具设置日期时间

#endif

extern battery_status_e charge_state;
extern battery_status_e charge_state_bak;
extern uint8 time_count;
extern int8 timer_usb_connect_id;
extern int8 timer_usb_battery_id;
extern int8 timer_usb_state_check_id;
extern uint8 returnkey_press;
extern ureader_status_t g_udisk_status;  
extern uint32 line_status_bak;
extern uint32 update_flag;
extern const uint16 style_usb_point[5];
extern uint8 g_usb_state;
extern uint8 charge_value;


extern void udiskpullcardout(void)__FAR__;
extern void udiskhandle(void)__FAR__;
extern uint8 udiskgetstatus(void)__FAR__;
extern void UDiskSetDiskNum(uint8 bset)__FAR__;
extern void AlwaysShowCardDisk(unsigned char bset)__FAR__;
extern void usbinit (PTRFUNC ptrfunc,unsigned char usbset)__FAR__;
extern void usb_setup (void)__FAR__;
extern void udisk_exit(void)__FAR__;
extern void display(void)__FAR__;
extern uint8 *get_usb_data_pointer(uint8 index)__FAR__;


void timer_usb_display_battery(void)__FAR__;
void timer_usb_display_connect(void)__FAR__;
void ui_show_battery(void)__FAR__;
void read_connect_pic_to_ram(void)__FAR__;
void read_battery_pic_to_ram(void)__FAR__;
void udisk_init_all(void)__FAR__;
app_result_e udisk_exit_all(int argc)__FAR__;

extern void set_usb_info(void) __FAR__;
extern bool config_disk_label(void) __FAR__;


#ifdef SUPPORT_USER_DEF_USBCMD
extern void set_time_date(void);
#endif

#endif  //_UDISKAP_H_
