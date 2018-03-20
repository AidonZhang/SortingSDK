/*******************************************************************************
 *                              US212A
 *                            Module: manager
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     manager_main.c
 * \brief    manager的main函数模块
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "manager.h"
#include  <card.h>
#include  <sysdef.h>

#ifndef PC
OS_STK *ptos = (OS_STK *) AP_PROCESS_MANAGER_STK_POS;
#endif
INT8U prio = AP_PROCESS_MANAGER_PRIO + 1;


extern app_result_e manager_message_loop(void)__FAR__;
extern void print(uint8 *s, uint32 Data, uint8 mode) __FAR__;

void system_config(void);

static void install_key_drv(void)
{
    uint32 key_para;

    key_para = (uint32) com_get_config_default(SETTING_BAT_CHARGE_VOLTAGE);
    key_para |= ((uint32) com_get_config_default(SETTING_BAT_CHARGE_CURRENT) << 8);
   // key_para |= ((uint32) com_get_config_default(SETTING_INPUT_IR_CODE) << 16);
    key_para |= ((uint32) com_get_config_default(SETTING_BAT_CHARGE_MODE) << 24);
    sys_drv_install(DRV_GROUP_KEY, key_para, "key.drv");    
}

/******************************************************************************/
/*!
 * \par  Description:
 *    应用的入口函数
 * \param[in]    未使用
 * \param[out]   none
 * \return       永远不会退出
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
int main(int argc, const char *argv[])
{
    system_config();

    //安装NOR UD驱动
    sys_drv_install(DRV_GROUP_STG_BASE, 0, "nor_ud.drv");

    //初始化globe全局数据，并加载配置脚本 config.bin
    globe_data_init_applib();
     
    install_key_drv();

    adjust_freq_set_level(AP_PROCESS_MANAGER_PRIO, FREQ_LEVEL8, FREQ_NULL);//加快开机速度
       
    //初始化applib库，前台AP
    applib_init(APP_ID_MANAGER, APP_TYPE_PMNG);

    //初始化 applib 消息模块
    sys_mq_flush(MQ_ID_MNG);

    //创建config应用
    sys_exece_ap("config.ap", 0, 0);
    
    manager_message_loop();

    return 0;
}



void system_config(void)
{

#ifndef _CARD_BASE_
    g_card_para = 0x0;

    g_card_type = 0;
#endif




    DISABLE_UHOST_DETECT();
    ENABLE_USB_DETECT();



    /*系统调频时，不再调整VCC电压，系统初始化为3.0V，应用可以根据需求，
     将其调整到合适档位，比如，为了EJTAG调试，设为3.3V.*/
    act_writel((0xFFFFFF8F & act_readl(VOUT_CTL)) | 0x40, VOUT_CTL);


}
