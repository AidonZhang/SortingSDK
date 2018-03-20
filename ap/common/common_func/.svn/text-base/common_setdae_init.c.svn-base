/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：CASE DAE设置管理模块，包括加载默认值，初始化和设置接口；通过共享内存进行
 *       间接设置。
 * 作者：cailizhen
 ********************************************************************************/

#include "psp_includes.h"
#include "music_common.h"
#include "common_func.h"
#include "stub_command.h"

//从config.bin加载DAE默认配置项
/*void com_load_dae_config(comval_t *setting_comval)
{
    dae_config_t *p_dae_cfg;
    uint8 i;
    mdrc_config_t tmp_limiter_config;
    
    //CASE 层自定义 Graphics EQ 类型
    setting_comval->eq_enable = (uint8) com_get_config_default(DAE_GRAPHICS_EQ_ENABLE);
    setting_comval->eq_type   = (uint8) com_get_config_default(DAE_GRAPHICS_EQ_TYPE);

    setting_comval->fade_in_time = (int8) com_get_config_default(SETTING_AP_ID_FADE_IN_TIME); 
    setting_comval->fade_out_time = (int8) com_get_config_default(SETTING_AP_ID_FADE_OUT_TIME);    
    
    p_dae_cfg = &(setting_comval->dae_cfg);

    p_dae_cfg->bypass = (uint8) com_get_config_default(DAE_BYPASS_ENABLE);
    p_dae_cfg->peq_enable = (uint8) com_get_config_default(DAE_PEQ_ENABLE);
    
    for (i = 0; i < MAX_PEQ_SEG; i++)
    {
        peq_config_t tmp_peq_config;
        com_get_config_struct((uint16) (DAE_PEQ_BANK0_SETTING + i), (uint8 *) &tmp_peq_config, sizeof(peq_config_t));
        libc_memcpy(&(p_dae_cfg->peq_bands[i]), &(tmp_peq_config.eq_data), sizeof(peq_band_t));
    }

    p_dae_cfg->mdrc_enable = (uint8) com_get_config_default(DAE_MDRC_ENABLE);
    p_dae_cfg->limiter_enable = (uint8) com_get_config_default(DAE_LIMITER_ENABLE);

    com_get_config_struct(DAE_LIMITER_SETTING, (uint8 *) &tmp_limiter_config, sizeof(tmp_limiter_config));
    
    p_dae_cfg->limiter_threshold = tmp_limiter_config.mdrc_data.threshold;
    p_dae_cfg->limiter_attack_time  = tmp_limiter_config.mdrc_data.attack_time;
    p_dae_cfg->limiter_release_time = tmp_limiter_config.mdrc_data.release_time;
        
}
*/

//加载mdrc参数
void com_inner_load_mdrc_param(dae_config_t *p_dae_cfg, bool aux_flag)
{
    uint16 config_id;
    uint8 i;

    if (p_dae_cfg->mdrc_enable == 0)
    {
        return ;
    }

    if (aux_flag == TRUE)
    {
        config_id = DAE_MDRC_AUX_BANK0_SETTING;
        p_dae_cfg->limiter_threshold_diff = (int16) com_get_config_default(DAE_MDRC_AUX_LIMITER_TH_DIFF);
    }
    else
    {
        config_id = DAE_MDRC_BANK0_SETTING;
        p_dae_cfg->limiter_threshold_diff = (int16) com_get_config_default(DAE_MDRC_LIMITER_TH_DIFF);
    }

    p_dae_cfg->limiter_enable = TRUE;
    
    for (i = 0; i < MAX_MDRC_SEG; i++)
    {
        mdrc_config_t tmp_mdrc_config;

        com_get_config_struct((uint16) (config_id + i), (uint8 *) &tmp_mdrc_config, \
                sizeof(tmp_mdrc_config));

        p_dae_cfg->mdrc_bands[i].threshold = tmp_mdrc_config.mdrc_data.threshold;
        p_dae_cfg->mdrc_bands[i].attack_time = tmp_mdrc_config.mdrc_data.attack_time;
        p_dae_cfg->mdrc_bands[i].release_time = tmp_mdrc_config.mdrc_data.release_time;
        p_dae_cfg->mdrc_bands[i].reserve = 0;

        if ((p_dae_cfg->limiter_enable == 1)
                && (p_dae_cfg->mdrc_bands[i].threshold > (0 - p_dae_cfg->limiter_threshold_diff)))
        {
            p_dae_cfg->mdrc_bands[i].threshold = (0 - p_dae_cfg->limiter_threshold_diff);
        }

        p_dae_cfg->mdrc_threshold_base[i] = p_dae_cfg->mdrc_bands[i].threshold;
        if (p_dae_cfg->mdrc_threshold_base[i] > p_dae_cfg->mdrc_threshold_base_max)
        {
            p_dae_cfg->mdrc_threshold_base_max = p_dae_cfg->mdrc_threshold_base[i];
        }

        if (i < (MAX_MDRC_SEG-1))
        {
            p_dae_cfg->mdrc_crossover_freq[i] = tmp_mdrc_config.mdrc_data.crossover_freq;
        }
    }

}

//根据MDRC开关，AUX通道属性，可变信号通道属性等选择DAE配置参数
bool com_load_mdrc_config(dae_config_t *p_dae_cfg, bool load_bin)
{

    if (p_dae_cfg->enable == 0)
    {
        return TRUE;
    }

    if (p_dae_cfg->bypass == TRUE)
    {
        p_dae_cfg->equivalent_gain = 0;
        p_dae_cfg->precut_ratio = 0;
        return TRUE;
    }

    if (load_bin == TRUE)
    {
        if (p_dae_cfg->mdrc_enable == 1)
        {
            p_dae_cfg->equivalent_gain = (uint8) com_get_config_default(DAE_EQUIVALENT_GAIN);
            p_dae_cfg->precut_ratio = 0;
        }
        else
        {
            p_dae_cfg->equivalent_gain = 0;
            p_dae_cfg->precut_ratio = (uint8) com_get_config_default(DAE_PRECUT_RATIO);
        }
    }
    p_dae_cfg->makeup_gain = 0;

    if (load_bin == TRUE)
    {
        com_inner_load_mdrc_param(p_dae_cfg, FALSE);
    }
    return TRUE;
}

//每次开机都重新某些DAE配置项及参数
void com_reset_dae_config(comval_t *setting_comval , dae_config_t * p_dae_cfg)
{   
    if(setting_comval->eq_enable == 1)
    {
        p_dae_cfg->enable = TRUE;
        p_dae_cfg->bypass = FALSE;
    }
    else
    {
        p_dae_cfg->enable = FALSE;
        p_dae_cfg->bypass = TRUE;        
    }

    com_load_mdrc_config(p_dae_cfg, TRUE);

    com_init_dae_config(p_dae_cfg);
    
}

//该接口仅仅在CONFIG开机时调用一次
//注：这里初始化后DAE配置参数还是不完全的，还需要设置物理音量之后才会完整有效
void com_init_dae_config(dae_config_t * p_dae_param)
{
    dae_config_t *p_dae_cfg;

    p_dae_cfg = sys_malloc(sizeof(dae_config_t));
    if (p_dae_cfg == NULL)
    {
        PRINT_ERR("dae_cfg malloc fail!!");
        while (1)
        {
            ; //nothing for QAC
        }
    }

    libc_memcpy(p_dae_cfg, p_dae_param, sizeof(dae_config_t));

    if (sys_shm_creat(SHARE_MEM_ID_DAECFG, p_dae_cfg, sizeof(dae_config_t)) == -1)
    {
        PRINT_ERR("dae_cfg shm create fail!!");
        while (1)
        {
            ; //nothing for QAC
        }
    }

//    setting_comval->dae_inited_flag = TRUE;
}
