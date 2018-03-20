/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：物理音量管理接口，包括设置音量和获取音量；支持I2S。
 * 作者：cailizhen
 ********************************************************************************/

#include "psp_includes.h"
#include "music_common.h"
#include "common_func.h"
#include "ccd_interface.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \设置硬件音量调节
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      common_func
 * \note
 * \li
 *******************************************************************************/
void com_set_phy_volume(uint8 volume_val)
{
    
/*    if (sys_comval->volume_gain >= VOLUME_TABLE_MAX)
    {
 //       PRINT_WARNING("phy Invalid volume gain!!");
        sys_comval->volume_gain = VOLUME_TABLE_MAX-1;
    }
*/
            
    switch (g_app_info_state.aout_mode)
    {
        case AO_SOURCE_I2S: //i2s
        //case AO_SOURCE_SPDIF: //spdif
 //       ccd_i2s_pa_set_volume(sys_comval->volume_gain, volume_val);
        break;

        //internel dac & i2s or spdif and external dac both chanel
        case AO_SOURCE_DACI2S: //i2s + internel dac
        case AO_SOURCE_DACSPDIF: //spdif +internel dac
        default:
        {
            uint32 vol_hard_pa, vol_hard_da;
            int tab_index;

            if (volume_val > VOLUME_VALUE_MAX)
            {
                volume_val = VOLUME_VALUE_MAX;
            }
            tab_index = VOLUME_VALUE_MAX - volume_val;

//            vol_hard_pa = g_hard_volume_table[sys_comval->volume_gain][tab_index].vol_pa;
//            vol_hard_da = g_hard_volume_table[sys_comval->volume_gain][tab_index].vol_da;
            vol_hard_pa = g_hard_volume_table[0][tab_index].vol_pa;
            vol_hard_da = g_hard_volume_table[0][tab_index].vol_da;

            set_pa_volume(vol_hard_pa, vol_hard_da);
        }
        break;
    }
}

/*! \endcond */

