/*
 ********************************************************************************
 *                       ACTOS
 *                 show logo when boot
 *
 *                (c) Copyright 2003-2009, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File   : welcome.c
 * By     : johnsen
 ********************************************************************************
 */

#include "uhost.h"
#include "case_independent.h"
#include "lcd_inner.h"

#ifdef WELCOM_PRINT
void print(uint8 *s, uint32 Data, uint8 mode)
{
    uint8 dat, t1, t2;
    uint8 *pDat;
    uint32 temp, i, buf_cnt;
    uint32 MRCR_S, CLKENCTL_S, MFP_CTL_S;
    uint32 BAUDRATE_S, UART_CTL_S;

    uint32 uart_tx_mfp_ctl_reg;
    uint32 uart_tx_gpio_cfg_mask;
    uint32 uart_tx_gpio_cfg_value;
    uint32 uart_baudrate_div;

    uart_tx_gpio_cfg_mask = 0xc7ffffff; //GPIO A23
    uart_tx_gpio_cfg_value = 0x30000000; //GPIO A23
    uart_tx_mfp_ctl_reg = MFP_CTL0;
    uart_baudrate_div = 225|((uint32)225<<16);//26M;

    //normal
    MRCR_S = act_readl(MRCR);
    temp = MRCR_S | (1 << MRCR_UART1Reset);
    act_writel(temp, MRCR);

    //UART clock enable
    CLKENCTL_S = act_readl(CMU_DEVCLKEN);
    temp = CLKENCTL_S | (1 << CMU_DEVCLKEN_UART1CLKEN);

    act_writel(temp, CMU_DEVCLKEN);
    MFP_CTL_S = act_readl(uart_tx_mfp_ctl_reg);

    temp = (MFP_CTL_S & uart_tx_gpio_cfg_mask) | uart_tx_gpio_cfg_value;
    act_writel(temp, uart_tx_mfp_ctl_reg);

    BAUDRATE_S = act_readl(UART1_BR);

    act_writel(uart_baudrate_div, UART1_BR);

    UART_CTL_S = act_readl(UART1_CTL);
    act_writel(0x00028203, UART1_CTL);

    buf_cnt = 0;

    if (mode == 3)
    {
        pDat = s;
        //print buf's data
        for (i = 0; i < Data; i++)
        {
            dat = *pDat;
            pDat++;
            t1 = dat & 0x0f;
            t2 = (dat & 0xf0) >> 4;
            if (t2 < 10)
            {
                t2 += '0';
            }
            else
            {
                t2 += ('a' - 10);
            }
            if (t1 < 10)
            {
                t1 += '0';
            }
            else
            {
                t1 += ('a' - 10);
            }

            if ((i % 16) == 0)//每行16个

            {
                act_writel('\n', UART1_TXDAT);
            }
            act_writel(t2, UART1_TXDAT);
            act_writel(t1, UART1_TXDAT);
            act_writel(' ', UART1_TXDAT);
            buf_cnt += 3;

            if (buf_cnt> 12)
            {
                buf_cnt = 0;
                while ((act_readl(UART1_STA) & (1 << UART1_STA_UTBB1)) != 0)
                {
                    ;//qac
                }
            }
        }

        goto finish_print;
    }

    if (s != NULL)
    {
        pDat = s;
        while (*pDat != 0)
        {
            act_writel(*pDat, UART1_TXDAT);
            pDat++;
            buf_cnt++;
            if (16 == buf_cnt)//max is 16

            {
                buf_cnt = 0;
                while ((act_readl(UART1_STA) & (1 << UART1_STA_UTBB1)) != 0)
                {
                    ;//qac
                }
            }
        }
    }

    if (mode == 0)
    {
        goto finish_print;
    }

    while ((act_readl(UART1_STA) & (1 << UART1_STA_UTBB1)) != 0)
    {
        ;//qac
    }

    pDat = (uint8*) (&Data);
    pDat += 3;

    act_writel('0', UART1_TXDAT);
    act_writel('x', UART1_TXDAT);

    for (i = 0; i < 4; i++)
    {
        dat = *pDat--;

        t1 = dat & 0x0f;
        t2 = (dat & 0xf0) >> 4;
        if (t2 < 10)
        {
            act_writel(t2 + '0', UART1_TXDAT);
        }
        else
        {
            act_writel(t2 - 10 + 'a', UART1_TXDAT);
        }
        if (t1 < 10)
        {
            act_writel(t1 + '0', UART1_TXDAT);
        }
        else
        {
            act_writel(t1 - 10 + 'a', UART1_TXDAT);
        }

    }

    finish_print:

    act_writel('\n', UART1_TXDAT);
    while ((act_readl(UART1_STA) & (1 << UART1_STA_UTBB1)) != 0)
    {
        ;//qac
    }

    act_writel(MRCR_S, MRCR);
    act_writel(CLKENCTL_S, CMU_DEVCLKEN);
    act_writel(MFP_CTL_S, uart_tx_mfp_ctl_reg);
    act_writel(BAUDRATE_S, UART1_BR);
    act_writel(UART_CTL_S, UART1_CTL);

}

#endif

void flush_rtc(void)
{
    act_writel(0xA596, RTC_REGUPDATA);
    while (0x5A69 != act_readl(RTC_REGUPDATA))
    {
        ;//delay for RTC update complete
    }
}

void mcu_enter_sleep_S4(void)
{

    act_writel((act_readl(VD15_DCDC_CTL) & (~VD15_DCDC_CTL_VD15_MODE_S1_MASK)), VD15_DCDC_CTL);
    act_writel((act_readl(VD15_DCDC_CTL) | (0x02 << VD15_DCDC_CTL_VD15_MODE_S1_SHIFT)), VD15_DCDC_CTL); //DC-LDO AUTO

    act_writel(0x00, POWER_CTL); //休眠

    //act_writel(0xA596, RTC_REGUPDATA);
    //while (act_readl(RTC_REGUPDATA) != 0x5A69)
    //{
    //    ; //nothing
    //}

    while (1)
    {
        ; //nothing
    }

}

bool is_usb_cable_in(void)
{
    bool usb_in = FALSE;

    usb_in = (bool) (((act_readb(DPDMCTRL) & (1 << DPDMCTRL_Plugin)) != 0) || ((act_readl(CHG_DET)
            & (3 << CHG_DET_UVLO)) != 0));

    return usb_in;
}

void disp_starting_board_config(uint8 charge_mode)
{

    //select PMUADC_CTL_LRADC_FS 250khz
    act_writel((act_readl(PMUADC_CTL) | (1 << PMUADC_CTL_BATADC_FS)), PMUADC_CTL);
    //enable LRADC1
    act_writel((act_readl(PMUADC_CTL) | (1 << PMUADC_CTL_LRADC1_EN)), PMUADC_CTL);
    //act_writel((act_readl(PMUADC_CTL) | (1 << PMUADC_CTL_LRADC234_EN)), PMUADC_CTL);
    //mfp switch
    act_writel((act_readl(GPIOB0_CTL) & (~GPIOB0_CTL_GPIOB0_MFP_MASK)), GPIOB0_CTL);
    //b0 as lradc1
    act_writel((act_readl(GPIOB0_CTL) | (1 << GPIOB0_CTL_GPIOB0_MFP_SHIFT)), GPIOB0_CTL);

    //rtc register
    act_writel(0xA596, RTC_REGUPDATA);
    while (act_readl(RTC_REGUPDATA) != 0x5A69)
    {
        ; //wait for register update
    }

    act_writel(act_readl(HOSC_CTL) | 0x001f0000, HOSC_CTL); //HGMC 0x1f
    welcome_delay_ms(5);

    act_writel((act_readl(LDO_CTL) & 0xffe7ffff) | 0x00100000, LDO_CTL); //OSCVDD 1.3v
    welcome_delay_ms(5);

}

void disp_starting_adfu_check(void(*adfu_launch)(void))
{
    bool adfu_flag = FALSE;

    if (is_usb_cable_in() == TRUE)
    {
        welcome_delay_ms(20);

        if ((act_readb(LRADC1_DATA) & 0x7F) < 0x62)//ADC按键
        {
            adfu_flag = TRUE;
        }
    }

    if (adfu_flag == TRUE)
    {
        adfu_launch();
    }
}

void disp_starting_low_power(uint8 charge_mode)
{
    bool enter_s4 = FALSE;
    bool usb_cable_adapter = is_usb_cable_in();

    if (usb_cable_adapter == FALSE)
    {

        if ((act_readb(BATADC_DATA) & 0x7f) < BAT_ADC_3_30_V)
        {
            enter_s4 = TRUE;
        }

    }

    if (enter_s4 == TRUE)
    {
        mcu_enter_sleep_S4();
    }
}

void disp_starting_show_welcome(void(*rd_res)(uint8*, uint16, uint16))
{
    region_t picreg;
    uint8 *buffer_addr = (uint8*) 0xbfc28000;
    uint32 pixcnt;
    uint16 sec_cnt = 0;

    lcd_hardware_init();

    set_draw_mode(DRAW_MODE_V_DEF, 0, 0);

    picreg.x = 0;
    picreg.y = 0;
    picreg.width = DISPLAY_LENGTH;
    picreg.height = DISPLAY_HEIGHT;

    set_window(&picreg, 0, 0);

    pixcnt = (uint32)(DISPLAY_LENGTH * DISPLAY_HEIGHT);
    sec_cnt = 0;

    //分多次读取显示，每次读取并显示显示暂存区的大小的图片数据
    while (pixcnt > 0)
    {
        //读取数据
        if (pixcnt >= 0x2800)
        {
            rd_res(buffer_addr, sec_cnt, 40);
            buff_data_trans(buffer_addr, 0x2800, 0);
            sec_cnt += 40;
            pixcnt -= 0x2800;
        }
        else
        {
            rd_res(buffer_addr, sec_cnt, (uint16)((pixcnt * 2) / 512));
            buff_data_trans(buffer_addr, (uint16)pixcnt, 0);
            break;
        }
    }

    backlight_on_off(TRUE, 0, 0); //enable backlight
    set_contrast(8, 0, 0); //set contrast duty 8/16

    //    act_writel(act_readl(GPIO_AOUTEN) & 0xfffffffd, GPIO_AOUTEN);//disable speak control??
    //    act_writel(act_readl(GPIO_ADAT) & 0xfffffffd, GPIO_ADAT);//enable speak control mute

}

__section__(".FIRST") void disp_starting(void (*adfu_launch)(void), void (*read_res)(uint8*, uint16 , uint16),
        welcome_argument_t *welcome_arg)
{
    uint16 *cfg_value = (uint16 *) welcome_arg->cfg_buffer;
    uint8 charge_mode;
    uint8 debug_mode;

    //disable DSP EJTAG
    act_writel(act_readl(MFP_CTL2) & (~(1<<30)), MFP_CTL2);

    //获取充电模式配置项默认值

    charge_mode = (uint8) cfg_value[0]; //index 0 -> 充电模式

    debug_mode = (uint8)cfg_value[1];

    if(debug_mode == 0)
    {
        ENABLE_WATCH_DOG(6);
    }

    //step 1: 板子初始化
    disp_starting_board_config(charge_mode);

    //step 2: 是否进入ADFU升级，如果进入ADFU升级，函数调用就不会返回
    disp_starting_adfu_check(adfu_launch);

    //step 3: 低电判断，如果低电就关机，函数调用就不会返回
    disp_starting_low_power(charge_mode);

    //step 4: 显示 welcome UI
    disp_starting_show_welcome(read_res);

}

