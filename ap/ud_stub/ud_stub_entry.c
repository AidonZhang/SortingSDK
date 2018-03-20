/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
//#include "stub_interface.h"
#include "ud_stub.h"
//#include "ud_intern.h"


//int usb_op(int cmd, uint8 *buff, int data_len)
int stub_op_entry(void *param0, void *param1, void *param2, stub_cmd_t cmd)
{
    int32 ret = 0;

    switch (cmd)
    {
        case STUB_OP_OPEN:
        ret = stub_init(param0);
        break;

        case STUB_OP_READ:
        ret = stub_read(param0, param1, (uint32) param2);
        break;

        case STUB_OP_WRITE:
        ret = stub_write(param0, param1, (uint32) param2);
        break;

        case STUB_OP_CLOSE:
        ret = stub_exit();
        break;

        case STUB_OP_IOCTL:
        ret = stub_ioctl(param0, param1, param2);
        //timeout = (int) param0;
        break;

        case STUB_OP_EXT_RW:
        ret = stub_ext_rw(param0, param1, param2);
        break;

        case STUB_OP_RAW_RW:
        ret = stub_ext_raw_rw(param0, param1, param2);
        break;

        default:
        ret = -1;
        break;
    }

    return ret;

}

void led_null_func(void)
{
    return;
}

led_driver_operations led_driver_op =
{ (led_op_func) led_null_func, (led_op_func) led_null_func, (led_op_func) led_null_func };
