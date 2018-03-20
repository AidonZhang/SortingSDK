/********************************************************************************
 *                              US280A
 *                            Module: USB device
 *                 Copyright(c) 2001-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>            <version >             <desc>
 *      dengtaiping 2013-01-18 16:40     1.0                build this file
 ********************************************************************************/
#include "ud_intern.h"
#include "dma.h"

/******************************************************************************/
/*!
 * \par  Description:
 *   设置枚举描述符.
 * \param[in]   none
 * \return  nome
 * \retval  nome
 * \retval  nome
 * \ingroup enumerate.c
 * \par
 * \note
 *******************************************************************************/
void set_usb_descriptor(device_msc_configuration_t* hs_cfg_p, device_msc_configuration_t* others_cfg_p)
{
    unsigned char bkdoorvalue;
    //BKDOOR 宏定义GET_HS_ENDI
    //bkdoorvalue = GET_HS_ENDI();
    bkdoorvalue = (act_readb(UsbCTRL_STUS) & (1 << 1));
    if (bkdoorvalue == 0x00) //非0,表示hs被禁止
    {
        hs_cfg_p->Endpiont_Descriptor_Setting_bk_in.wMaxPacketSize = 0x0040;
        hs_cfg_p->Endpiont_Descriptor_Setting_bk_out.wMaxPacketSize = 0x0040;
        others_cfg_p->Endpiont_Descriptor_Setting_bk_in.wMaxPacketSize = 0x0200;
        others_cfg_p->Endpiont_Descriptor_Setting_bk_out.wMaxPacketSize = 0x0200;
    }
    else
    {
        hs_cfg_p->Endpiont_Descriptor_Setting_bk_in.wMaxPacketSize = 0x0200;
        hs_cfg_p->Endpiont_Descriptor_Setting_bk_out.wMaxPacketSize = 0x0200;
        others_cfg_p->Endpiont_Descriptor_Setting_bk_in.wMaxPacketSize = 0x0040;
        others_cfg_p->Endpiont_Descriptor_Setting_bk_out.wMaxPacketSize = 0x0040;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *  set endpoint0 no NAK.
 * \param[in]   none
 * \return  none
 * \retval  none
 * \ingroup ud_setup.c
 * \par
 * \note
 *******************************************************************************/
void no_data_control(void)
{
    act_writeb(0x02 | act_readb(Ep0cs_hcep0cs), Ep0cs_hcep0cs);
}

/******************************************************************************/
/*!
 * \par  Description:
 *   updata by endpoint0.
 * \param[in]   none
 * \return  none
 * \retval  none
 * \ingroup ud_setup.c
 * \par
 * \note
 *******************************************************************************/
void control_write(uint8* data_address, uint8 data_length, uint8 request_length)
{
    uint8 transfer_length, temp_count;
    uint8 *p_temp;

    /* if length of data to be received is more than PC sent, only send data length that pc sent.  */
    if (data_length <= request_length)
    {
        transfer_length = data_length;
    }
    else
    {
        transfer_length = request_length;
    }
    p_temp = data_address;

    /* clear OUT0BC to receive data from PC.  */
    act_writeb(0x00, Out0bc_hcin0bc);

    /* wait transfer complete. */
    while (1)
    {
        //act_writel(act_readl(WD_CTL) | 0x01, WD_CTL);
        /* check if busy.   */
        if ((act_readb(Ep0cs_hcep0cs) & 0x08) == 0x00)
        {
            break;
        }
    }/* while(1) */

    for (temp_count = 0; temp_count < transfer_length; temp_count++)
    {
        *(p_temp + temp_count) = act_readb(Ep0outdata + temp_count);
    }
    act_writeb(0x01, Out0bc_hcin0bc);
    act_writeb(act_readb(Ep0cs_hcep0cs) | 0x02, Ep0cs_hcep0cs);
}

/******************************************************************************/
/*!
 * \par  Description:
 *   download data by endpoint0.
 * \param[in]   none
 * \return  none
 * \retval  none
 * \ingroup ud_setup.c
 * \par
 * \note
 *******************************************************************************/
void control_read(uint8 *d_addr, uint8 actul_dlen, uint8 rqst_dlen)
{
    uint8 translen_sum;
    uint8 translen_once;
    uint8 i;
    //calucate data length of this control read
    if (rqst_dlen == 0x00)
    {
        translen_sum = actul_dlen;
    }
    else if (rqst_dlen > actul_dlen)
    {
        translen_sum = actul_dlen;
    }
    else
    {
        translen_sum = rqst_dlen;
    }

    //transferring
    while (translen_sum != 0)
    {
        if (translen_sum > 0x40)
        {
            translen_once = 0x40;
            translen_sum = translen_sum - 0x40;
        }
        else
        {
            translen_once = translen_sum;
            translen_sum = 0x00;
        }
        for (i = 0; i < translen_once; i++)
        {
            //write data to ep0infofo
            act_writeb(*d_addr, (Ep0indata + i));
            d_addr++;
        }
        //wtite in0 conter,it will start packet transfer and set in0busy
        act_writeb((uint8) translen_once, In0bc_hcout0bc);
        //wait transfer end,Inbsy_hcoutbsy = b'0'
        while ((act_readb(Ep0cs_hcep0cs) & 0x04) != 0x00)
        {
            ;//do nothing //act_writel(act_readl(WD_CTL) | 0x01, WD_CTL);
        }
    }
    act_writeb(0x02 | act_readb(Ep0cs_hcep0cs), Ep0cs_hcep0cs);
}

/******************************************************************************/
/*!
 * \par  Description:
 *   stall endpoint.
 * \param[in]   none
 * \return  none
 * \retval  none
 * \ingroup ud_setup.c
 * \par
 * \note
 *******************************************************************************/
void stall_endpoint(uint8 ep_num)
{
    switch (ep_num)
    {
        case EP0:
        //stall ep0
        act_writeb(0x01 | act_readb(Ep0cs_hcep0cs), Ep0cs_hcep0cs);
        break;
        case EP1OUT:
        //stall ep1out
        act_writeb(0x40 | act_readb(Out1ctrl_hcin1ctrl), Out1ctrl_hcin1ctrl);
        break;
        case EP2OUT:
        //stall ep2out
        act_writeb(0x40 | act_readb(Out2ctrl_hcin2ctrl), Out2ctrl_hcin2ctrl);
        break;
        case EP3OUT:
        //stall ep3out
        act_writeb(0x40 | act_readb(Out3ctrl_hcin3ctrl), Out3ctrl_hcin3ctrl);
        break;
        case EP1IN:
        //stall ep1in
        act_writeb(0x40 | act_readb(In1ctrl_hcout1ctrl), In1ctrl_hcout1ctrl);
        break;
        case EP2IN:
        //stall ep2in
        act_writeb(0x40 | act_readb(In2ctrl_hcout2ctrl), In2ctrl_hcout2ctrl);
        case EP3IN:
        //stall ep3in
        act_writeb(0x40 | act_readb(In3ctrl_hcout3ctrl), In3ctrl_hcout3ctrl);
        break;
        default:
        break;
    }
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 *   recover enpoint.
 * \param[in]   none
 * \return  none
 * \retval  none
 * \ingroup ud_setup.c
 * \par
 * \note
 *******************************************************************************/
void recover_stall_endpoint(uint8 ep_num)
{
    switch (ep_num)
    {
        case EP0:
        //claer stall bit for ep0
        act_writeb(0xfe & act_readb(Ep0cs_hcep0cs), Ep0cs_hcep0cs);
        break;
        case EP1OUT:
        //claer stall bit for ep1out
        act_writeb(0xbf & act_readb(Out1ctrl_hcin1ctrl), Out1ctrl_hcin1ctrl);
        break;
        case EP2OUT:
        //claer stall bit for ep2out
        act_writeb(0xbf & act_readb(Out2ctrl_hcin2ctrl), Out2ctrl_hcin2ctrl);
        break;
        case EP3OUT:
        //claer stall bit for ep3out
        act_writeb(0xbf & act_readb(Out3ctrl_hcin3ctrl), Out3ctrl_hcin3ctrl);
        break;
        case EP1IN:
        //claer stall bit for ep1in
        act_writeb(0xbf & act_readb(In1ctrl_hcout1ctrl), In1ctrl_hcout1ctrl);
        break;
        case EP2IN:
        //claer stall bit for ep2in
        act_writeb(0xbf & act_readb(In2ctrl_hcout2ctrl), In2ctrl_hcout2ctrl);
        break;
        case EP3IN:
        //claer stall bit for ep3in
        act_writeb(0xbf & act_readb(In3ctrl_hcout3ctrl), In3ctrl_hcout3ctrl);
        break;
        default:
        break;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *   reset endpoint toggle.
 * \param[in]   none
 * \return  none
 * \retval  none
 * \ingroup ud_setup.c
 * \par
 * \note
 *******************************************************************************/
void rest_endp_toggle(void)
{
    //reset ep1 toggle
    act_writeb(0x01, EPRST);
    act_writeb(0x71, EPRST);
    act_writeb(0x61, EPRST);

    //reset ep2 toggle
    act_writeb(0x02, EPRST);
    act_writeb(0x72, EPRST);
    act_writeb(0x62, EPRST);
}

/*
 *********************************************************************************************************
 * \                                          access_usbfifo_by_cpu
 * \
 * \Description: 使用cpu在USB FIFO与RAM之间传输数据, 只传送整包.
 * \
 * \Arguments  : de: Data Transfer Length, byte unit;
 * \           : c: enpoint and B1/B2 select;
 * \
 * \Returns    : None.
 * \
 * \Note(s)    :
 **********************************************************************************************************/
void access_usbfifo_by_cpu(uint8 epsel, uint32 *address, uint32 data_lenth)
{
    uint32 i;

    data_lenth >>= 2; //Fifodat是32位

    for (i = 0; i < data_lenth; i++)
    {
        if (epsel == EP1IN)
        {
            act_writel(*address, Fifo1dat);
        }
        else /* if (epsel == EP2OUT)*/
        {
            *address = act_readl(Fifo2dat);
        }
        address++;
    }
}

/*
 *********************************************************************************************************
 * \                                          cpu_data_transfer
 *
 * \Description: 使用cpu在USB FIFO与RAM之间传输数据, 只传送整包.
 *
 * \Arguments  : de: Data Transfer Length, byte unit;
 * \           : c: enpoint and B1/B2 select;
 *
 * \Returns    : None.
 *
 * \Note(s)    :
 **********************************************************************************************************/
int cpu_data_transfer(uint8 epselect, uint32 address, uint32 datalength, uint32 time_out_max, uint16 max_size)
{
    uint32 cpu_trans_once;
    int timeout_count;
    int ret_v = 0;

    while (datalength != 0)
    {
        timeout_count = time_out_max;
        if (datalength > max_size)
        {
            cpu_trans_once = max_size;
            datalength = datalength - max_size;
        }
        else
        {
            cpu_trans_once = datalength;
            datalength = 0x00;
        }

        if (epselect == EP2OUT)
        {
            while (0 != (act_readb(Out2cs_hcin2cs) & 0x02))
            {
                timeout_count--;
                if (((act_readb(DPDMCTRL) & 0x40) == 0) || (timeout_count < 0))
                {
                    ret_v = -1;
                    break;
                }
            }
            access_usbfifo_by_cpu(epselect, (uint32 *) address, cpu_trans_once);
            if (cpu_trans_once < max_size)
            {
                act_writeb(act_readb(Out2cs_hcin2cs) | 0x02, Out2cs_hcin2cs);
            }
        }
        else /* if (epselect == EP1IN)*/
        {
            access_usbfifo_by_cpu(epselect, (uint32 *) address, cpu_trans_once);
            if (cpu_trans_once < max_size)
            {
                act_writeb(act_readb(In1cs_hcout1cs) | 0x02, In1cs_hcout1cs);
            }
            while ((act_readb(In1cs_hcout1cs) & 0x02) != 0x00)
            {
                timeout_count--;
                if (((act_readb(DPDMCTRL) & 0x40) == 0) || (timeout_count < 0))
                {
                    ret_v = -1;
                    break;
                }
            }
        }
        address += cpu_trans_once;
    }

    return ret_v;
}

/******************************************************************************/
/*!
 * \par  Description:
 *   USB intrrupt entry.
 * \param[in]   none
 * \return  none
 * \retval  none
 * \ingroup ud_irq.c
 * \par
 * \note
 *******************************************************************************/
//extern void deal_setup(void);
void usb_isr(void)
{
    act_writeb((0x80 | act_readb(USBEIRQ)) & 0x8f, USBEIRQ);

    if (/*(irq_type == IRQ_SUDAV) && */(0 != ((act_readb(Usbirq_hcusbirq)) & 0x01))) //setup
    {
        act_writeb(0x01 | act_readb(Usbirq_hcusbirq), Usbirq_hcusbirq);
        //deal_setup();
        g_stub_setup = 1;
    }
    else if (0 != ((act_readb(USBEIRQ)) & 0x10)) //disconnect
    {
        act_writeb((0x10 | act_readb(USBEIRQ)) & 0x10, USBEIRQ);
        //usb_disconnect_isr();
    }
    else if (0 != ((act_readb(OSHRTPCKIR)) & 0x40)) //short package
    {
        act_writeb(0x40 | act_readb(OSHRTPCKIR), OSHRTPCKIR);
        //short_pk_isr();
    }

    else if (/*(irq_type == IRQ_SUSPEND) && */(0 != ((act_readb(Usbirq_hcusbirq)) & 0x08))) //usb suspend
    {
        act_writeb(0x08 | act_readb(Usbirq_hcusbirq), Usbirq_hcusbirq);
        //usb_suspend_isr();
    }
    else if (/*(irq_type == IRQ_USBRESET) && */(0 != ((act_readb(Usbirq_hcusbirq)) & 0x10))) //usb bus reset
    {
        act_writeb(0x10 | act_readb(Usbirq_hcusbirq), Usbirq_hcusbirq);
        //usb_reset_isr();
    }
    else if (/*(irq_type == IRQ_HSPEED) && */(0 != ((act_readb(Usbirq_hcusbirq)) & 0x20))) //change speed
    {
        act_writeb(0x20 | act_readb(Usbirq_hcusbirq), Usbirq_hcusbirq);
        //speed_change_isr();
    }
    else
    {
    }

    //return;
}

