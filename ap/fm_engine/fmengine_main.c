/*******************************************************************************
 *                              US212A
 *                            Module: radio engine
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      mikeyang  2011-09-05        1.0              create this file
 *******************************************************************************/

#include "App_fmengine.h"

//globle variable
//FM 引擎状态信息
Engine_Status_t g_fmengine_status;

//引擎配置信息
FMEngine_cfg_t g_fmengine_cfg;

//FM 模组所处状态
Module_Status_e g_module_status;

mmm_pp_ainout_setting_t g_ainout_param;
//globle variable
//保存中间件句柄
void *g_mmm_pp_handle = NULL;


//mmm_pp_ainout_setting_t g_ainout_param;

//fm通道选择
uint8 g_fm_chanel;


//for dae adjust freq
dae_config_t *g_dae_cfg_shm;
    
//FM 引擎任务栈
#ifndef PC
OS_STK *ptos = (OS_STK *) AP_FMENGINE_STK_POS;
#endif

//FM 引擎主线程优先级
INT8U prio = AP_FMENGINE_PRIO;


static void _set_ainout_param(void)
{
    g_ainout_param.input_type = MMM_PP_FM_IN;
    g_ainout_param.asrc_index = K_OUT0_P01_IN_P2;
    g_ainout_param.dac_sample = 48;
    g_ainout_param.dac_chanel = 0;
    g_ainout_param.asrc_mode_sel = 3;
    g_ainout_param.adc_sample = 48;
    g_ainout_param.adc_insel = AI_SOURCE_FM;
    g_ainout_param.ain_gain = (uint8) com_get_config_default(FMENGINE_GAIN_INPUT);
    g_ainout_param.adc_gain = (uint8) com_get_config_default(FMENGINE_GAIN_ADC);
}


/******************************************************************************/
/*!
 * \par  Description:
 * \void fmengine_load_cfg(void)
 * \读取配置信息
 * \param[in]    void
 * \param[out]   none
 * \return       void
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void fmengine_load_cfg(void)
{
    //获取FM 引擎配置信息
    sys_vm_read(&g_fmengine_cfg, VM_AP_FMENGINE, sizeof(FMEngine_cfg_t));
    /*if (g_fmengine_cfg.magic != MAGIC_USER1) //0x55AA
     {
     g_fmengine_cfg.magic = MAGIC_USER1;*/
    if (g_fmengine_cfg.magic != VRAM_MAGIC(VM_AP_FMENGINE))
    {
        g_fmengine_cfg.magic = VRAM_MAGIC(VM_AP_FMENGINE);
        g_fmengine_cfg.fm_threshold = (uint8) com_get_config_default(FMENGINE_AP_ID_SEEKTH); //THRESHOLD_DEFAULT;
        g_fmengine_cfg.paddv_mode = (PA_DDV_Mode_e) com_get_config_default(FMENGINE_AP_ID_PAMODE); //PA_MODE;
        /*保存VM变量*/
        sys_vm_write(&g_fmengine_cfg, VM_AP_FMENGINE, sizeof(FMEngine_cfg_t));
    }
    g_fm_chanel = (uint8) com_get_config_default(FMENGINE_CHANNEL_SEL); //fm通道选择;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void fmengine_save_cfg(void)
 * \保存配置信息
 * \param[in]    void
 * \param[out]   none
 * \return       void
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void fmengine_save_cfg(void)
{
    /*保存VM变量*/
    sys_vm_write(&g_fmengine_cfg, VM_AP_FMENGINE, sizeof(FMEngine_cfg_t));
}
void get_fm_init_param(FM_init_parm_t *p_fm)
{
    p_fm->freq_mode = (uint8) com_get_config_default(FMENGINE_FREQ_32K_13M);
    p_fm->gpio_level = (uint8) com_get_config_default(FMENGINE_CLK_PAD_LEVEL);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int _app_init(void)
 * \读取配置信息，安装FM  驱动
 * \param[in]    void
 * \param[out]   none
 * \return       int the result
 * \retval           0 sucess
 * \retval          -1 failed
 * \note
 */
/*******************************************************************************/
int fmengine_init(void)
{
    int app_init_ret = 0;
    FM_init_parm_t fm_arg;
    char *mmm_name =
    { "mmm_pp.al" };

    //初始化applib库，后台AP
    applib_init(APP_ID_FMENGINE, APP_TYPE_CONSOLE);
    //升频
    adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL8, FREQ_NULL);

    //读取配置信息 (包括vm信息)
    fmengine_load_cfg();
    get_fm_init_param(&fm_arg);

    //初始化 applib 消息模块
    applib_message_init(NULL);

    key_earphonecheck(1);
    
    //安装fm  驱动
    app_init_ret = sys_drv_install(DRV_GROUP_FM, &fm_arg, "drv_fm.drv");

    //驱动安装成功
    if (app_init_ret == 0)
    {
        //模组尚未初始化，需处理MSG_FMENGINE_INIT  消息后进行初始化
        g_module_status = MODULE_NO_INIT;
    }

    _set_ainout_param();
        
    if (g_fm_chanel == 1)
    {
        //初始化中间件库
        //加载中间件驱动
        sys_load_mmm(mmm_name, TRUE);
    }

    //降频
    adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL2, FREQ_NULL);
    g_dae_cfg_shm = (dae_config_t *) sys_shm_mount(SHARE_MEM_ID_DAECFG);
    if (g_dae_cfg_shm == NULL)
    {
        PRINT_ERR("dae_cfg shm not exist!!");
     	 while (1)
         {
             ; //nothing for QAC
         }
    }

    return app_init_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool fmengine_deinit(void)
 * \退出app的功能函数,保存配置信息
 * \param[in]    void
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool fmengine_deinit(void)
{
    //升频
    adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL8, FREQ_NULL);

     key_earphonecheck(0);
     
    if (g_fm_chanel == 1)
    {
        adjust_freq_set_level(AP_BACK_HIGH_PRIO, FREQ_NULL, FREQ_NULL);
        sys_free_mmm(TRUE);
    }

    //save config 写vram
    fmengine_save_cfg();

    //卸载fm 驱动
    sys_drv_uninstall(DRV_GROUP_FM);


    //降频
    adjust_freq_set_level(AP_BACK_LOW_PRIO, FREQ_LEVEL2, FREQ_NULL);
    //执行applib库的注销操作
    applib_quit();

    return TRUE;
}

/******************************************************************************/
/*!
 * \Description: FM Engine ap entry function
 * \int main(int argc, const char *argv[])
 * \app入口函数
 * \param[in]    argc  para1
 * \param[in]    argc  para2
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
int main(int argc, const char *argv[])
{

    app_result_e result = RESULT_NULL;

    //初始化
    act_writel(act_readl(MFP_CTL3) & (uint32)(~(1 << 6)), MFP_CTL3); 
    act_writel(act_readl(MFP_CTL3) & (uint32)(~(1 << 7)), MFP_CTL3);
    
    if (fmengine_init() == 0)
    {
        //引擎刚安装时，默认为播放
        change_engine_state(ENGINE_STATE_PLAYING);
        result = fmengine_control_block();
    }
    fmengine_deinit();//退出
  
    return result;
}

