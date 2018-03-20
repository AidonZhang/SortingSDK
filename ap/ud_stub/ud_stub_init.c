/*******************************************************************************
 *                              US212A
 *                            Module: user1
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-19 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     stub_main.c
 * \brief    stub��ģ�飬������̳�ʼ�����˳�������������
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "ud_stub.h"

int stub_init(bool need_uninstall_led)
{
    //uint8 wait_setup = 0;

    sys_os_sched_lock();

    g_stub_setup = 0;

    if (need_uninstall_led == TRUE)
    {
        sys_drv_uninstall(DRV_GROUP_LCD);

        //�滻led����������
        sys_set_drv_ops(DRV_GROUP_LCD, &led_driver_op);
    }

    /*USB��ʼ��*/
    usb_device_init();

    /*����STUB��ʼ����ϣ�����ʹ���жϲ���ʼwork*/
    //usb_device_start();//�ýӿڴ�usb_device_init�������Ϊ�˱�����USB�ж�����bank


    /*һ����5���������ö��*/
    while (1)
    {

        //sys_os_time_dly(50);
        if (Max_Pkt_Size != 0)
        {
            //ö�ٳɹ�
            break;
        }
        //wait_setup++;
        if (g_stub_setup == 1)
        {
            //set up deal
            g_stub_setup = 0;

            deal_setup();
        }
        CLEAR_WATCH_DOG;
    }

    //timeout = OT_500MS_72MHZ;
    timeout = sys_get_delay_val(STUB_DELAY_500MS, 40);

    sys_os_sched_unlock();

    //led_display(NUMBER1, NUM_T, 1);
    //led_display(NUMBER2, NUM_E, 1);
    //led_display(NUMBER3, NUM_S, 1);
    //led_display(NUMBER4, NUM_T, 1);

    return 0;
}

