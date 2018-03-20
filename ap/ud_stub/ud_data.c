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

//device_requests_t setup_data;
//定义cbw结构体变量
bulkcsw CSW_Struct =
{
    ._dCSWSignature = (uint32)0x53425355,
    ._dCSWTag = (uint32)0,
    ._dCSWDataResidue = (uint32)0,
    ._bCSWStatus = (uint8)0,
}; //定义csw结构体变量

uint32 bulk_dma_no = 0;
uint16 ep1_in_status = 0;
uint16 ep1_out_status = 0;
uint8 config_value = 0;
uint16 device_status = 0x0100;
uint16 interface_status = 0;
uint8 alter_setting = 0;
uint16 Max_Pkt_Size = 0;
uint32 prev_freq = 0;
uint8 usb_state = 0;
int timeout = 300000;
//uint8 Removable = 0;
uint8 g_deal_setup = 0;

uint8 g_mem_id = STUB_USE_URAM1;

uint8 g_read_mode;

