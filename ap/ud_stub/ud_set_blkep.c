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
#include "ud_setup.h"
#include "ud_intern.h"
#include "stub_interface.h"

/******************************************************************************/
/*!
 * \par  Description:
 *   set bulk in endpoint.
 * \param[in]   none
 * \return  none
 * \retval  none
 * \ingroup ud_setup.c
 * \par
 * \note
 *******************************************************************************/
void set_bulk_in_ep(uint32 mem_sel)
{
    if (((act_readb(UsbCTRL_STUS)) & 0x02) == 0x00)
    {
        act_writeb(0x40, Hcout1maxpckl);
        act_writeb(0x00, Hcout1maxpckh);
        Max_Pkt_Size = 0x40;
    }
    else
    {
        act_writeb(0x00, Hcout1maxpckl);
        act_writeb(0x02, Hcout1maxpckh);
        Max_Pkt_Size = 0x200;
    }

    act_writeb(0x88, In1ctrl_hcout1ctrl); //single buff
    //act_writel(0x00000800, IN1STADDR);
    if (mem_sel == STUB_USE_URAM0)
    {
        act_writel(0x00000000, IN1STADDR);
    }
    else
    {
        act_writel(0x00000800, IN1STADDR);
    }

    act_writeb(0x01, EPRST);
    act_writeb(0x71, EPRST);
    act_writeb(0x61, EPRST);
    act_writeb(0x21, Fifoctrl); //set inep1 autorun
    act_writeb(0x31, Fifoctrl); //set inep1 autorun
}

/******************************************************************************/
/*!
 * \par  Description:
 *   set bulk out endpoint.
 * \param[in]   none
 * \return  none
 * \retval  none
 * \ingroup ud_setup.c
 * \par
 * \note
 *******************************************************************************/
void set_bulk_out_ep(uint32 mem_sel)
{
    if (((act_readb(UsbCTRL_STUS)) & 0x02) == 0x00)
    {
        act_writeb(0x40, Hcin2maxpckl);
        act_writeb(0x00, Hcin2maxpckh);
    }
    else
    {
        act_writeb(0x00, Hcin2maxpckl);
        act_writeb(0x02, Hcin2maxpckh);
    }

    act_writeb(0x88, Out2ctrl_hcin2ctrl); //single buff

    if (mem_sel == STUB_USE_URAM0)
    {
        act_writel(0x00000400, OUT2STADDR);
    }
    else
    {
        act_writel(0x00000c00, OUT2STADDR);
    }

    act_writeb(0x02, EPRST);
    act_writeb(0x72, EPRST);
    act_writeb(0x62, EPRST);
    act_writeb(0x22, Fifoctrl);
    act_writeb(0x32, Fifoctrl);
}

/******************************************************************************/
/*!
 * \par  Description:
 *   deal usb reset.
 * \param[in]   none
 * \return  none
 * \retval  none
 * \ingroup ud_msc.c
 * \par
 * \note
 *******************************************************************************/
void reset_in_fifo(uint32 mem_sel)
{
    act_writeb(0x88, In1ctrl_hcout1ctrl); //single buff
    //act_writel(0x00000800, IN1STADDR);
    if (mem_sel == STUB_USE_URAM0)
    {
        act_writel(0x00000000, IN1STADDR);
    }
    else
    {
        act_writel(0x00000800, IN1STADDR);
    }

    act_writeb(0x11, EPRST);
    act_writeb(0x31, EPRST);
    act_writeb(0x51, EPRST);
    act_writeb(0x31, Fifoctrl); //set inep1 autorun

}

/******************************************************************************/
/*!
 * \par  Description:
 *   deal USB disconnect.
 * \param[in]   none
 * \return  none
 * \retval  none
 * \ingroup ud_msc.c
 * \par
 * \note
 *******************************************************************************/
void reset_out_fifo(uint32 mem_sel)
{
    act_writeb(0x88, Out2ctrl_hcin2ctrl); //single buff
    //act_writel(0x00000c00, OUT2STADDR);
    if (mem_sel == STUB_USE_URAM0)
    {
        act_writel(0x00000400, OUT2STADDR);
    }
    else
    {
        act_writel(0x00000c00, OUT2STADDR);
    }

    act_writeb(0x02, EPRST);
    act_writeb(0x62, EPRST);
    act_writeb(0x72, EPRST);

    //set auto bit
    act_writeb(0x32, Fifoctrl);
    act_writeb(0x22, Fifoctrl);
}

/******************************************************************************/
/*!
 * \par  Description:
 *   deal USB disconnect.
 * \param[in]   none
 * \return  none
 * \retval  none
 * \ingroup ud_msc.c
 * \par
 * \note
 *******************************************************************************/
void switch_fifo_mem(uint32 mem_id)
{

    g_mem_id = mem_id;

    if (mem_id == STUB_USE_URAM0)
    {
        //uram0
        act_writel((act_readl(CMU_MEMCLKSEL) & (~CMU_MEMCLKSEL_URAM0CLKSEL_MASK)) | (1
                << CMU_MEMCLKSEL_URAM0CLKSEL_SHIFT), CMU_MEMCLKSEL);

    }
    else
    {
        //uram1
        act_writel((act_readl(CMU_MEMCLKSEL) & (~CMU_MEMCLKSEL_URAM1CLKSEL_MASK)) | (1
                << CMU_MEMCLKSEL_URAM1CLKSEL_SHIFT), CMU_MEMCLKSEL);
    }

    set_bulk_in_ep(mem_id);

    set_bulk_out_ep(mem_id);

}

