
#ifndef   _STUB_H
#define   _STUB_H

#include  "psp_includes.h"
#include  "case_include.h"
#include  "led_driver.h"
#include  "ud_intern.h"
#include  "dma.h"
#include  "stub_interface.h"


typedef void*(*led_op_func)(void*, void*, void*);

typedef struct
{
    led_op_func sdrv_led_display;
    led_op_func sdrv_clear_screen;
    led_op_func sdrv_flash_screen;
} led_driver_operations;

extern led_driver_operations led_driver_op;

extern int stub_read(uint16 opcode, uint8 *data_buffer, uint32 data_len) __FAR__;

extern int stub_write(uint16 opcode, uint8 *data_buffer, uint32 data_len) __FAR__;

extern int stub_ioctl(uint16 op_code, void *param1, void *param2) __FAR__;

extern int stub_init(bool need_uninstall_led) __FAR__;

extern int stub_exit(void) __FAR__;

#endif

