
/*******************************************************************************
 *                              US282A
 *                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
 *                             All Rights Reserved.
 *      Description:按键初始化
 *      <author>       <time>           
 *       Wekan       2015-3-26pm            
 *******************************************************************************/


#include "key_inner.h"


/******************************************************************************
* \par  Description:   adc按键初始化
* \param[in]     none
* \param[out]   none
* \return           none
* \note     
*   <author>    <time>           <version >             <desc>
*    Wekan   2015-3-23pm         1.0             review this file
*******************************************************************************/
#if (SUPPORT_ADC_KEY == 1)
void key_adc_init(void)
{
    //初始化线控按键 LRADC1234 设置后需要等待10ms才能读出正确的数据，
    //否则可能全0,建议放到welcome进行初始化，则不需要等待10ms
    
    //select PMUADC_CTL_LRADC_FS 250khz
    act_writel((act_readl(PMUADC_CTL) | (1 << PMUADC_CTL_BATADC_FS)), PMUADC_CTL);
    
    //enable LRADC1
    act_writel((act_readl(PMUADC_CTL) | (1 << PMUADC_CTL_LRADC1_EN)), PMUADC_CTL);
    //enable LRADC234
    //act_writel((act_readl(PMUADC_CTL) | (1 << PMUADC_CTL_LRADC234_EN)), PMUADC_CTL);
    
    //mfp switch
    act_writel((act_readl(GPIOB0_CTL) & (~GPIOB0_CTL_GPIOB0_MFP_MASK)), GPIOB0_CTL);    
    //b0 as lradc1
    act_writel((act_readl(GPIOB0_CTL) | (1<<GPIOB0_CTL_GPIOB0_MFP_SHIFT)), GPIOB0_CTL);
       
    //rtc register
    act_writel(0xA596, RTC_REGUPDATA);
    while (act_readl(RTC_REGUPDATA) != 0x5A69)
    {
        ;//wait for register update
    }  
    
    sys_mdelay(20);
}
#endif


/******************************************************************************
 * \par  Description:   按键初始化入口
 * \param[in]     null0,null1保留参数，
 * \ param[in]    key_para 是在config.txt中配置按键信息
 * \param[out]   none
 * \return           返回0
 * \note     
 *   <author>    <time>           <version >             <desc>
 *    Wekan   2015-3-23pm         1.0             review this file
 *******************************************************************************/

int key_init(void* null0, void *null1, uint32 key_para)
{
    //lradc key init
#if (SUPPORT_ADC_KEY == 1)
    key_adc_init();
#endif

    //电池充电方式
    g_key_bat_charge_mode = (uint8) (key_para>>24);

    key_bat_charge_init((uint8)(key_para>>8), (uint8)key_para);
    key_sampling_battery_voltage_init();
    
    //for scan irq 20ms
    g_time_irq = sys_set_irq_timer1(key_scan_int_process, 20);
    
    //使能AUX GPIO IN
#if (SUPPORT_AUX_DETECT == DETECT_BY_GPIO)
    act_writel((act_readl(AUX_GPIO_IN) | AUX_GPIO_PIN), AUX_GPIO_IN);
#endif

#if (SUPPORT_AUX_DETECT == DETECT_BY_SIO)
    act_writel((act_readl(MFP_CTL3) & (~MFP_CTL3_VRO_MASK) | (2 << MFP_CTL3_VRO_SHIFT)), MFP_CTL3); //MFP选择SIO7
    act_writel((act_readl(DAC_ANACTL) & (~(1 << DAC_ANACTL_OPVROEN))), DAC_ANACTL); //VRO与SIO7复用，需关闭VRO
    act_writel((act_readl(SIO_INEN) | AUX_SIO_PIN), SIO_INEN);
#endif

    //使能HP GPIO IN
#if (SUPPORT_HP_DETECT == DETECT_BY_GPIO)
    act_writel((act_readl(HP_GPIO_IN) | HP_GPIO_PIN), HP_GPIO_IN);
#endif

    //使能SD卡GPIO IN
#if (SUPPORT_CARD_DETECT == DETECT_BY_GPIO)
    act_writel((act_readl(CARD_GPIO_IN) | CARD_GPIO_PIN), CARD_GPIO_IN);
#endif

    adjust_freq_do_init();

    return 0;
}

module_init(key_init)

