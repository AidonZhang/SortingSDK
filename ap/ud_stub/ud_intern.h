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
#ifndef __USOUND_INTERN_H__
#define __USOUND_INTERN_H__

#include "ud_setup.h"
#include "irq.h"
#include "typeext.h"
#include "kernel_interface.h"
#include "storage_device.h"
#include "driver_manager.h"
#include "libc_interface.h"
#include "pm.h"
#include "vm_fwsp_def.h"
#include <addr_map.h>
#include "sysdef.h"

#ifndef REG32
#define REG32 volatile unsigned int*
#endif

//define ep num
#define     EP0                 0
#define     EP1IN               1
#define     EP1OUT              2
#define     EP2IN               3
#define     EP2OUT              4
#define     EP3IN               5
#define     EP3OUT              6

//define err type
#define    NOERR                 0
#define    CMDRESET              1
#define    MEDIAPROTECT          2
#define    RESETUDISK            3
#define    MEDIANOTPRESENT       4
#define    INVALIDCMD            5
#define    INVALIDFIELD          6

#define  USB_DMA_NO      0x00

#define  BUFF_ADDRESS       0x33000
#define  BUFF1_ADDRESS       0x33000
#define  BUFF2_ADDRESS       0x34000
//#define  BUFF1_ADDRESS      0x33000
//#define  BUFF2_ADDRESS      0x34800
#define  BUFF_SECTOR_NUM      0x8

#define  PLAY_BUFF_START    0x32f00
//#define  RECORD_BUFF_START  0x33000
#define  PLAY_BUFF_SIZE     0x100
//#define  RECORD_BUFF_SIZE   0x1000

#define THRESHOLD_BUFFER_LOW           0x200
#define THRESHOLD_BUFFER_HIGH          0xC00
#define THRESHOLD_BUFFER_OVERFLOW      0xF40
#define THRESHOLD_LOST_SAMPLE          0xE00

#define  USB_IS_CONNECT    0x00
#define  USB_CONNECT_RESET 0x01
#define  USB_DISCONNECT    0x02

//set disconnect
#define     SET_DISCONNECT()  act_writeb((1<<6)|act_readb(UsbCTRL_STUS),UsbCTRL_STUS)
//set connect
#define     SET_CONNECT()     act_writeb(act_readb(UsbCTRL_STUS)|~(1<<6),UsbCTRL_STUS)
//#define     clear_watchdog()    act_writel((0x00000001|act_readl(WD_CTL)),WD_CTL)
//check connect
#define     HSDISABLE   (1<<7)
#define     HSENABLE    (1<<7)
#define     USBSPEED    (1<<1)          //UsbCTRL_STUS bit1
#define     HSMODE_DISABLE()    \
                act_writeb(act_readb(BKDOOR)|HSDISABLE,BKDOOR)
#define     HSMODE_ENABLE()    \
                act_writeb(act_readb(BKDOOR)&(~HSENABLE),BKDOOR)
#define     GET_HS_ENDI()   \
                (act_readb(UsbCTRL_STUS)&USBSPEED)
#ifndef _FPGA_VERIFY_
#define     USB_CONNECT_STATUS (act_readb(DPDMCTRL)&(1<<6))
#else
#define     USB_CONNECT_STATUS ((uint8)((act_readb(LINESTATUS)&0x18) != 0x18))
#endif
#define     WRITE_DATA_IN_FIFO(x) \
                act_writeb((x),Fifo1dat)
//set busy status ep
#define     SET_EP_BUSY()   \
                act_writeb(act_readb(In1cs_hcout1cs)|0x02,In1cs_hcout1cs)

#pragma pack(1)
typedef struct
{
    uint32 _dCBWSignature;
    uint32 _dCBWTag;
    uint32 _dCBWDataTransferLength;
    uint8 _bmCBWFlags;
    uint8 _bCBWLUN;
    uint8 _bCBWCBLength;
    uint8 _CBWCB[9];
} bulkcbw;

typedef struct
{
    uint32 _dCSWSignature;
    uint32 _dCSWTag;
    uint32 _dCSWDataResidue;
    uint8 _bCSWStatus;
} bulkcsw;

typedef struct
{
    uint32 LastLogBlockAddr;
    uint32 BlockLengthInBytes;
} cap_info;

typedef struct
{
    //Capacity List Header
    uint8 reserved[3];
    uint8 CapListLength;

    //Current/Maximum Capacity Descriptor
    uint32 CurrentCap;
    uint8 Descriptor_Type;
    uint8 Reserved1;
    uint16 Current_BlkLength;

    //Formattable Capacity Descriptor
    uint32 FormatCap_BlkNum;
    uint32 FormatCap_BlkLength;
} format_cap_info;

#pragma pack() //±£´æ¶ÔÆë×´Ì¬
extern uint8 usb_state;
extern bulkcbw CBW_Struct;
extern bulkcsw CSW_Struct;
extern uint32 bulk_dma_no;
extern uint16 ep1_in_status;
extern uint16 Max_Pkt_Size;
extern uint16 ep1_out_status;
extern uint8 config_value;
extern uint16 device_status;
extern uint16 interface_status;
extern uint8 alter_setting;
extern device_requests_t setup_data;
extern device_msc_configuration_t high_speed_configuration;
extern device_msc_configuration_t other_speed_configuration;
//extern usb_qualifier_descriptor device_qualifier_descriptor;
//extern device_descriptor_t Device_Descriptor;
//extern string_language_id_t language_string;
extern int timeout;
extern uint8 g_deal_setup;

extern uint8 g_mem_id;
extern uint8 g_read_mode;

extern void usb_isr(void);
extern void usb_disconnect_isr(void);
extern void usb_reset_isr(void);
extern void usb_device_init(void);
extern void usb_device_exit(void);
extern void usb_device_start(void);
extern int cpu_data_transfer(uint8 epselect, uint32 address, uint32 datalength, uint32 time_out_max, uint16 max_size);

#endif

