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
 * \file     udisk task.c
 * \brief    deal udisk message and display
 * \author   yliang
 * \version  1.0
 * \date  2011/10/07
 *******************************************************************************/
#include  "ap_udisk.h"
//const uint8 batvalue[5] =
//{ 0x58, 0x5d, 0x5f, 0x65, 0x76 };

void usb_state_check_timer(void);
/********************************************************************************
 * Description : 读battery 图片到缓存中
 *
 * Arguments  :     ramaddr
 *
 *
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
void read_battery_pic_to_ram(void)
{
    res_size_t tmp_size;
    uint8 i;
    for (i = 0; i < 5; i++)
    {
        ui_read_pic_to_buffer(P_UDISK_BATTERY_0 + i, (uint8 *) (BAT_PIC_BUFFER + i * BATTERY_PIC_LENGTH), &tmp_size);
    }
}

/********************************************************************************
 * Description : 读数据传输更新图片到缓存中
 *
 * Arguments  :     ramaddr
 *
 *
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
void read_connect_pic_to_ram(void)
{
    res_size_t tmp_size;
    uint8 i;
    for (i = 0; i < 10; i++)
    {
        ui_read_pic_to_buffer(P_UDISK_POINT_1A + i, (uint8 *) (CONNECT_PIC_BUFFER + i * CONNECT_PIC_LENGTH), &tmp_size);
    }
}

/********************************************************************************
 * Description : 根据获取到的电压值显示相应的电池图标
 *
 * Arguments  :
 *            value：当前获取到的电压值
 *
 * Returns     :
 *            无
 * Notes       :
 *
 ********************************************************************************/
void ui_show_battery(void)
{
    //   uint8 value;
    uint8 frame = 0;
    style_infor_t style_infor_battery;
    picbox_private_t picbox_param;
    /*    value = act_readb(BATADC_DATA);
     if (charge_state == BAT_NO_EXIST)
     {
     frame = 0;
     }
     else
     {*/
    //分5档显示电池图标，对应关系如下：
    /*
     电池电压范围
     1.       空格电：<=3.5V  0.05V误差

     2.       一格电：3.5-3.6V  0.05V误差

     3.       两格电：3.6-3.65V   0.05V误差

     4.       三格电：3.65-3.8V    0.05V误差

     5         满格：  3 .8V以上    0.05V 误差

     */

    /*        for (frame = 0; frame < 5; frame++)
     {
     if (value <= batvalue[frame])
     {
     break;
     }
     }
     if (frame >= 4)
     {
     frame = 4;
     }
     }*/
    if (charge_value == 0)
    {
        frame = 0;
    }
    else
    {
        frame = charge_value - 1;
    }

    if (frame >= 4)
    {
        frame = 4;
    }

    style_infor_battery.type = UI_AP;
    style_infor_battery.style_id = UDISK_BATTERY;
    picbox_param.pic_id = -1;
    picbox_param.frame_id = frame;
    //显示界面
    ui_show_picbox(&style_infor_battery, &picbox_param);
}

void timer_usb_display_battery(void)
{
    uint16 pix_cnt;
    region_t res_region;
    time_count++;
    if (time_count == 5)
    {
        time_count = 0;
    }
    /*将connect picture buffer中的图片刷到屏上*/
    res_region.x = POS_BATTERY_X_BASE;
    res_region.y = POS_BATTERY_Y_BASE;
    res_region.width = WIDTH_BATTERY;
    res_region.height = HEIGHT_BATTERY;

    lcd_set_window(&res_region); //设窗参数设置

    pix_cnt = res_region.width * res_region.height;

    libc_memcpy(LCD_BUF1_ADDR, BAT_PIC_BUFFER + time_count * BATTERY_PIC_LENGTH, pix_cnt * 2);
    //DMA到GRAM中
    lcd_dma_set_counter(pix_cnt, 0);
    //开始DMA传输
    lcd_dma_start_trans(LCD_RAM1_INDEX);


}

void timer_usb_display_connect(void)
{
    uint8 i;
    uint8 frame;
    uint16 pix_cnt;
    region_t res_region;
    time_count++;
    if (time_count == 5)
    {
        time_count = 0;
    }

    for (i = 0; i < 5; i++)
    {
        if (i == time_count)
        {
            frame = 2 * (4 - i);
        }
        else
        {
            frame = 2 * (4 - i) + 1;
        }
        /*将connect picture buffer中的图片刷到屏上*/

        res_region.x = POS_CONNECT_X_BASE + (4 - i) * POS_CONNECT_INC_X;
        res_region.y = POS_CONNECT_Y_BASE;
        res_region.width = WIDTH_CONNECT;
        res_region.height = HEIGHT_CONNECT;

        lcd_set_window(&res_region); //设窗参数设置
        pix_cnt = res_region.width * res_region.height;

        libc_memcpy(LCD_BUF1_ADDR, CONNECT_PIC_BUFFER + frame * CONNECT_PIC_LENGTH, pix_cnt * 2);
        lcd_dma_set_counter(pix_cnt, 0);
        lcd_dma_start_trans(LCD_RAM1_INDEX);

    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    message mananger
 * \param[in]
 * \param[out]   none
 * \retval       none
 * \ingroup      main_udisk.c
 * \note
 *******************************************************************************/
void display(void)
{
    uint8 i;
    picbox_private_t picbox_data;
    textbox_private_t textbox_param;
    style_infor_t style_infor;
    style_infor.type = UI_AP;
    textbox_param.lang_id = UNICODEID;
    textbox_param.str_value = NULL;
    textbox_param.str_id = -1;

    if (timer_usb_state_check_id == -1)
    {
        ud_get_status(&g_udisk_status);
        ud_set_cmd(SET_LINE_STATUS, 0);
    }

    if (line_status_bak != g_udisk_status.line_sta) //USB 状态有变化
    {
        if (g_udisk_status.line_sta == 0x00) //显示任务指示 0:无显示动作  01:上传  02:下传  03:传输结束
        {
            charge_state_bak = BAT_NORMAL;
        }
        else if (((g_udisk_status.line_sta == 0x10) || (g_udisk_status.line_sta == 0x20) || (g_udisk_status.line_sta
                == 0x60)) && (line_status_bak == 0x00))
        {
            if (timer_usb_battery_id != -1)
            {
                kill_app_timer(timer_usb_battery_id);
                timer_usb_battery_id = -1;
            }

            if (timer_usb_state_check_id == -1)
            {
                timer_usb_state_check_id = set_app_timer(APP_TIMER_ATTRB_UI, 500, usb_state_check_timer);
            }

            if (timer_usb_connect_id == -1)
            {
                timer_usb_connect_id = set_app_timer(APP_TIMER_ATTRB_UI, 1000, timer_usb_display_connect);
                time_count = 0;
            }
            style_infor.style_id = UDISK_STATE_STR;
            textbox_param.str_id = S_UDISK_LINKING;
            ui_show_textbox(&style_infor, &textbox_param, TEXTBOX_DRAW_NORMAL);
            ui_show_battery();
        }
        else
        {

        }
    }
    else
    {
    }

    if (g_udisk_status.line_sta == 0x00) //空闲状态
    {
        if (timer_usb_connect_id != -1)
        {
            kill_app_timer(timer_usb_connect_id);
            timer_usb_connect_id = -1;
        }

        if (timer_usb_state_check_id != -1)
        {
            kill_app_timer(timer_usb_state_check_id);
            timer_usb_state_check_id = -1;
        }

        if (line_status_bak != 0x00)//从非空闲状态进入到空闲状态才需要更新
        {
            for (i = 0; i < 5; i++)
            {
                style_infor.style_id = style_usb_point[i];
                picbox_data.pic_id = -1;
                picbox_data.frame_id = 0;
                //显示界面
                ui_show_picbox(&style_infor, &picbox_data);
            }
        }

        style_infor.style_id = UDISK_STATE_STR;
        charge_state = (battery_status_e) (uint32) key_chargeget(&charge_value);
        if (charge_state_bak != charge_state)
        {
            switch (charge_state)
            {
                case BAT_CHARGING:
                textbox_param.str_id = S_UDISK_CHARGING;
                if (timer_usb_battery_id == -1)
                {
                    timer_usb_battery_id = set_app_timer(APP_TIMER_ATTRB_UI, 500, timer_usb_display_battery);
                    time_count = 0;
                }
                break;
                case BAT_FULL:
                textbox_param.str_id = S_UDISK_FULL_CHARGED;
                if (timer_usb_battery_id != -1)
                {
                    kill_app_timer(timer_usb_battery_id);
                    timer_usb_battery_id = -1;
                }
                ui_show_battery();
                break;
                case BAT_NO_EXIST:
                if (timer_usb_battery_id != -1)
                {
                    kill_app_timer(timer_usb_battery_id);
                    timer_usb_battery_id = -1;
                }
                charge_value = 0;
                ui_show_battery();
                break;
                default:
                break;
            }
            ui_show_textbox(&style_infor, &textbox_param, TEXTBOX_DRAW_NORMAL);
        }
    }
    else
    {

    }
    charge_state_bak = charge_state;
    line_status_bak = g_udisk_status.line_sta;
}

void usb_state_check_timer(void)
{

    ureader_status_t udisk_status;
    ud_get_status(&udisk_status);
    if ((udisk_status.line_sta == 0x01) || (udisk_status.line_sta == 0x02))
    {
        return;
    }
    ud_get_status(&g_udisk_status);
    ud_set_cmd(SET_LINE_STATUS, 0);
}

#ifdef SUPPORT_USER_DEF_USBCMD
void set_time_date(void)
{
    uint8 i;
    time_t down_time;
    date_t down_date;
    uint8 temp_buff[7];

    libc_memset(temp_buff, 0, 7);

    while (0 != (act_readb(Out2cs_hcin2cs) & 0x02))
    {
        ;//clear_watchdog();
    }

    //改成用LB读寄存器，因为如果FIFO内的数据不是4的整数倍且收完数据，用act_readl访问Fifo2dat FIFO会出错
    for (i = 0; i < 7; i++)
    {
        asm volatile ("lb $7, %0" : : "m" (*(uint8 *)Fifo2dat) : "$7", "memory");
        asm volatile ("sb $7, %0" : "=m" (*((uint8 *) (&temp_buff) + i)) : : "$7", "memory");
    }

    down_date.year = *((uint16 *) temp_buff);
    down_date.month = *((uint8 *) temp_buff + 2);
    down_date.day = *((uint8 *) temp_buff + 3);
    down_time.hour = *((uint8 *) temp_buff + 4);
    down_time.minute = *((uint8 *) temp_buff + 5);
    down_time.second = *((uint8 *) temp_buff + 6);

    usb_print("day:", down_date.day, 2);

    sys_set_time(&down_time);
    sys_set_date(&down_date);
    act_writeb(act_readb(OSHRTPCKIR) | 0x40, OSHRTPCKIR);

    //act_writeb((act_readb(Out2cs_hcin2cs) | 0x02), Out2cs_hcin2cs);
    ud_report_status(0);
    return;
}
#endif

