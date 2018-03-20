#ifndef __ATT_INTERFACE_H
#define __ATT_INTERFACE_H

typedef struct
{
    uint8 filename[11];
    uint8 reserved1[5];
    uint32 offset;
    uint32 length;
    uint8 reserved2[8];
} atf_dir_t;

typedef struct
{
    uint8 magic[8];
    uint8 sdk_version[4];
    uint8 reserved[20];
    atf_dir_t atf_dir[15];
} atf_head_t;

typedef struct
{
    uint8 base_type;
    uint8 file_sys_id;
    uint16 ap_file_offset;
    uint16 cfg_file_offset;
    uint16 cfg_file_len;
}test_ap_info_t;

#endif
