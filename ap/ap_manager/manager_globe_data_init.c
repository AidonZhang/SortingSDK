/*******************************************************************************
 *                              us212A
 *                            Module: manager
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-10-8 11:21     1.0             build this file
 *******************************************************************************/
/*!
 * \file     manager_config.c
 * \brief    应用配置脚本操作接口实现
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了一套简单的应用配置接口，包括读取配置项等。
 * \par      EXTERNALIZED FUNCTIONS:
 *               直接调用sd 文件系统接口进行定位和读取。
 * \version 1.0
 * \date  2011-10-8
 *******************************************************************************/

#include  "manager.h"

// config 脚本文件名字
static const char cfg_file[] = "config.bin";

/* 串口 stub 程序库需要在启动时加载并常驻 */
//static const char stub_name[] = { "stub.al" };

bool globe_data_init_applib(void)
{
    uint8 config_file[12];
    uint8 i, ret;
    uint8 temp = 0;

    for (i = 0; i < MAX_APP_COUNT; i++)
    {
        libc_memset(&(g_app_info_vector[i]), 0x00, sizeof(app_info_t));
        g_app_info_vector[i].app_id = APP_ID_MAX;
    }

    libc_memcpy(config_file, cfg_file, sizeof(cfg_file) + 1);
    ret = (uint8) com_open_config_file(config_file);

//    libc_memset(&g_app_info_state_all, 0x00, sizeof(g_app_info_state_all));

    libc_memset(&g_app_info_state, 0x00, sizeof(g_app_info_state));

    g_app_info_state.type = ENGINE_NULL;
    g_app_info_state.state = ENGINE_STATE_NULL;
    g_app_info_state.app_state = APP_STATE_NO_PLAY;
    g_app_info_state.filter_key = KEY_NULL;
    g_app_info_state.keylock_state = FALSE;
    g_app_info_state.sound_state = SOUND_OUT_STOP;
    g_app_info_state.volume = 0;
    g_app_info_state.backlight_state = BACKLIGHT_STATE_NORMAL;
    g_app_info_state.screensave_state = FALSE;
    g_app_info_state.card_state = CARD_STATE_CARD_NULL;
    g_app_info_state.cable_state = CABLE_STATE_CABLE_NULL;
    g_app_info_state.antenna_state = FALSE;
    g_app_info_state.charge_state = BAT_NORMAL;
    g_app_info_state.bat_value = 5;
    g_app_info_state.card_plist = FALSE;
    //g_app_info_state.video_state = 0;
    g_app_info_state.card_update = FALSE;
    //获取DAC 和ADC BIAS的电流值配置
    temp = (uint8) com_get_config_default(SETTING_AUDIO_DAC_BIAS_AND_ANACTL);
    g_dac_adc_bias_type = temp << 4;
    temp = (uint8) com_get_config_default(SETTING_AUDIO_ADC_BIAS_TYPE);
    g_dac_adc_bias_type |= temp;
    
    g_app_last_state.last_app = APP_ID_MAX;
    g_app_last_state.last_type = ENGINE_NULL;
    g_app_last_state.last_state = ENGINE_STATE_NULL;
    g_app_last_state.last_app_state = APP_STATE_NO_PLAY;
    g_app_last_state.last_card_state = CARD_STATE_CARD_NULL;
    g_app_last_state.last_cable_state = CABLE_STATE_CABLE_NULL;
    g_app_last_state.last_light_timer = 0;
    g_app_last_state.last_screen_save_mode = 0;
    g_app_last_state.last_sleep_timer = 0;

    libc_memset(&g_sys_counter, 0x00, sizeof(sys_counter_t));
    

    libc_memset(&g_keytone_infor, 0x00, sizeof(keytone_infor_t));
    libc_memset(g_keytone_infor.kt_fifo, 0xff, KEYTONE_FIFO_DEPTH);
    g_keytone_infor.dac_chan = -1;//按键音无效
    
    //设置UART PRINT开关
    if (ret == TRUE)
    {
        uint8 uart_enable;

        uart_enable = (uint8) com_get_config_default(SETTING_UART_PRINT_ENABLE);
        sys_set_sys_info(uart_enable, SYS_PRINT_ONOFF);
#if 0		
        if (uart_enable == TRUE)
        { //使能UART TX

#if (CASE_BOARD_TYPE == CASE_BOARD_EVB)
            reg_writel((act_readl(GPIOAOUTEN) & (~(1 << 23))), GPIOAOUTEN);
            reg_writel((act_readl(GPIOAINEN) & (~(1 << 23))), GPIOAINEN);
            reg_writel((act_readl(MFP_CTL0) & (~MFP_CTL0_GPIOA23_MASK)) | (6 << MFP_CTL0_GPIOA23_SHIFT), MFP_CTL0);
#else
            reg_writel((act_readl(GPIOAOUTEN) & (~(1 << 3))), GPIOAOUTEN);
            reg_writel((act_readl(GPIOAINEN) & (~(1 << 3))), GPIOAINEN);
            reg_writel((act_readl(MFP_CTL1) & (~MFP_CTL1_GPIOA3_MASK)) | (5 << MFP_CTL1_GPIOA3_SHIFT), MFP_CTL1);          
#endif
        }
#endif		
    }


    //使用内部电容，需要配置
    if (com_get_config_default(SETTING_HARDWARE_26MHz_CYRSTAL_CAPACITOR) == 0)
    {
        manager_config_hosc_freq();
    }

    return ret;
}
