#ifndef _AP_UPGRADE_H
#define _AP_UPGRADE_H

#include  "psp_includes.h"
#include  "case_include.h"
#include  "upgrade_res.h"
#include  "upgrade_sty.h"
#include "sys_info.h"
#include <uhost.h>


#ifdef SUPPORT_UPGRADE_DEBUG
#define upgrade_print(str, data, mode)   libc_print(str,data,mode) 
#else
#define upgrade_print(str, data, mode) 
#endif


#define  UPGRADE_POS_CONNECT_X_BASE  72 //18
#define  UPGRADE_POS_CONNECT_Y_BASE  185 //96
#define  UPGRADE_WIDTH_CONNECT       16 //12
#define  UPGRADE_HEIGHT_CONNECT      16 //12
#define  UPGRADE_POS_CONNECT_INC_X   40 //12

#define   UPGRADE_CONNECT_PIC_BUFFER  (0x36000)
#define   UPGRADE_CONNECT_PIC_LENGTH  (UPGRADE_HEIGHT_CONNECT * UPGRADE_WIDTH_CONNECT * 2)//图片尺寸16*16
#define   UPGRADE_CONNECT_PIC_FRAME   (10)

#define BUFFER_ADDR    0x28A00

#define MIN_ONCE_READ_LEN  (18 * 1024)

#define DECRYPT_USE_BUF_ADDR  0x24000 //0x20120 //0x2b000 //(DECRYPT_INIT_BUF_ADDR + DECRYPT_INIT_BUF_SIZE) //18k
#define DECRYPT_USE_BUF_SIZE 0x4800 //解密用buffer 18k
#define DECRYPT_GL_BUF_ADDR  (DECRYPT_USE_BUF_ADDR + DECRYPT_USE_BUF_SIZE) //300
#define DECRYPT_GL_BUF_SIZE  300

#define DECRYPT_INIT_BUF_ADDR 0x2D200 //(SRAM_BASE_ADDR + 0x0000) //20k
#define DECRYPT_INIT_BUF_SIZE 0x5000 //初始化解密buffer 20k

#define DECRYPT_INIT_BUF_LEN  19888
#define DECRYPT_GL_BUF_LEN  300
#define DECRYPT_USR_BUF_LEN 18432

typedef uint32 (*func_def0)(void);
typedef void (*func_def1)(uint32 a);

typedef struct
{
    uint32 file_offset;
    uint32 sec_num;
    uint32 sram_addr;
} ud_sector_param_t;

//目录项结构
typedef struct
{
    char filename[11];
    char reserve1[5];
    unsigned long offset;
    unsigned long length;
    char reserve2[4];
    unsigned long checksum;
}__attribute__ ((packed)) DIR_t; //32字节

typedef struct
{
    LFIHead_t lfi_head;
    DIR_t diritem[240];
}__attribute__ ((packed)) LFIHead_dir_t; //8k bytes

typedef struct
{
    HANDLE file_handle;
    int32 g_file_sys_id;
    uint32 brec_length;
    uint32 brec_offset;
    uint32 nor_id_length;
    uint32 nor_id_offset;
    uint32 lfi_length;
    uint32 lfi_offset;
    uint32 g_write_sector;
    int32 progress;
    uint32 lfi_start_addr;
    uint32 upgrade_type;
    uint8 udisk_setting;
    uint8 reserved[3];
    uint8 UsbSetupInfo[48];
    uint32 CapInfo[4];
}__attribute__ ((packed)) _para_t;

typedef enum
{
    ENTER_UPGREADE = 0,
    INSTALL_DRV,
    SEARCH_FMU,
    READ_BREC,
    WRITE_BREC,
    CHECK_BREC,
    READ_LFI,
    WRITE_LFI,
    CHECK_LFI,
    EARSE_VRAM,
    RENAME_FMU,
    UPGREADE_OVER
} upgrade_prg;
typedef struct
{
    unsigned char *pInOutBuffer;
    long InOutLen;
    long FileLength;
    unsigned char *pGLBuffer;
    unsigned char *initusebuffer;
    long initusebufferlen;
} PCFWDecrypt_struct;

struct fw_decrypt_operations
{
    audio_device_op_i fw_decrypt_init;
    audio_device_op_i fw_decrypt_run;
};

//2015-12-04 added
typedef enum
{
    FW_UPGRADE_NO_ERR,
    FW_UPGRADE_FILE_ERR,
    FW_UPGRADE_WRITE_ERR,
    FW_UPGRADE_CHECK_ERR
} upgrade_status_e;



#define UPGRADE_TYPE_FACTORY  0
#define UPGRADE_TYPE_ENDUSER  1

extern PCFWDecrypt_struct g_decrypt;
extern _para_t _para;
extern struct fw_decrypt_operations g_decrypt_ops;
extern uint32 g_firmware_offset;
extern uint8 g_led_state;

//百分比计数
extern uint8 g_precent_count;

extern uint8 cpu_release_count;


extern upgrade_status_e _upgrade_firmware(void) __FAR__;
extern void decrypt_init(void) __FAR__;
extern uint32 _read_file_translate(uint8 *read_buffer, uint32 read_len) __FAR__;
extern uint32 _read_file(uint32 read_len) __FAR__;
extern void PCFWDecrypt(unsigned char *pInOutBuffer, long BufferLength,unsigned char *pGLBuffer)__FAR__;
extern int PCFWDecryptInit(PCFWDecrypt_struct *PCFWDecrypt_s)__FAR__;
extern bool upgrade_fw_check(void) __FAR__;
extern bool upgrade_fw_write(void) __FAR__;
extern void _upgrade_proc_timer(void) __FAR__;
extern void _upgrade_refreash_progress(uint8 value) __FAR__;
extern app_result_e upgrade_get_app_msg(void) __FAR__;
#endif
