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
 * \brief    manager��main����ģ��
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
 *    Ӧ�õ���ں���
 * \param[in]    δʹ��
 * \param[out]   none
 * \return       ��Զ�����˳�
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

    //��װNOR UD����
    sys_drv_install(DRV_GROUP_STG_BASE, 0, "nor_ud.drv");

    //��ʼ��globeȫ�����ݣ����������ýű� config.bin
    globe_data_init_applib();
     
    install_key_drv();

    adjust_freq_set_level(AP_PROCESS_MANAGER_PRIO, FREQ_LEVEL8, FREQ_NULL);//�ӿ쿪���ٶ�
       
    //��ʼ��applib�⣬ǰ̨AP
    applib_init(APP_ID_MANAGER, APP_TYPE_PMNG);

    //��ʼ�� applib ��Ϣģ��
    sys_mq_flush(MQ_ID_MNG);

    //����configӦ��
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



    /*ϵͳ��Ƶʱ�����ٵ���VCC��ѹ��ϵͳ��ʼ��Ϊ3.0V��Ӧ�ÿ��Ը�������
     ������������ʵ�λ�����磬Ϊ��EJTAG���ԣ���Ϊ3.3V.*/
    act_writel((0xFFFFFF8F & act_readl(VOUT_CTL)) | 0x40, VOUT_CTL);


}
