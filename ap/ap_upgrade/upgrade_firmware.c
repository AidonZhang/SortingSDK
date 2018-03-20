/*******************************************************************************
 *                              US212A
 *                            Module: UPGRADE_FIRMWARE
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2012-10-19 14:00:41           1.0              build this file
 *******************************************************************************/
/*!
 * \file     upgrade_firmware.c
 * \brief    固件升级模块
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date    2012-10-19
 *******************************************************************************/
#include "ap_upgrade.h"

static const uint8 lfi_name[] =
{ "FWIMAGE FW " };
static const uint8 nor_id_name[] =
{ "NOR_ID  BIN" };
static const uint8 brec_name[] =
{ "CARDBRECBIN" };

/******************************************************************************/
/*!
 * \par  Description:
 *      获取LFI数据在文件中偏移
 * \param[in]    buf   data buffer address
 * \param[in]    buffer_len   buffer length
 * \param[in]    key_word 关键字字符串
 * \param[in]    key_len  关键字字符串长度
 * \param[in]    offset  偏移
 * \return       AFI_DIR_t* 目录项指针
 * \retval
 * \note
 *******************************************************************************/
static AFI_DIR_t *check_key_offset_exist(char *buffer, uint32 buffer_len, const uint8 *key_word, uint8 key_len,
        uint8 offset)
{
    uint32 str_len = 0;

    while (str_len < buffer_len)
    {
        if (libc_memcmp(&buffer[str_len + offset], key_word, (uint32) key_len) == 0)
        {
            return (AFI_DIR_t *) (&buffer[str_len]);
        }
        else
        {
            str_len += 32;
        }
    }

    return NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 *  搜索目录项
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      search_content
 * \note
 *******************************************************************************/

int search_content(void)
{
    AFI_DIR_t *dir_p = NULL;

    PRINT_INFO("start decrypt...");

    decrypt_init();

    PRINT_INFO("decrypt init over");

    _read_file(MIN_ONCE_READ_LEN);

    if (base_upg_init(BUFFER_ADDR) != 0)
    {
        PRINT_INFO("decrypt file err");

        return -1;
    }

    g_firmware_offset = (uint32) (DECRYPT_USR_BUF_LEN - g_decrypt.InOutLen);
    //libc_print("off9",g_firmware_offset,2);
    dir_p = check_key_offset_exist((char *) BUFFER_ADDR, (uint32) g_decrypt.InOutLen, lfi_name, 11, 0);
    if (dir_p != NULL)
    {
        _para.lfi_length = dir_p->length;
        _para.lfi_offset = dir_p->offset + g_firmware_offset;
        upgrade_print("lfilen",_para.lfi_length,2);
        upgrade_print("lfioff",_para.lfi_offset,2);
    }
    else
    {
        return -1;
    }

    dir_p = check_key_offset_exist((char *) BUFFER_ADDR, (uint32) g_decrypt.InOutLen, nor_id_name, 11, 0);
    if (dir_p != NULL)
    {
        _para.nor_id_length = dir_p->length;
        _para.nor_id_offset = dir_p->offset + g_firmware_offset;
    }
    else
    {
        return -1;
    }

    dir_p = check_key_offset_exist((char *) BUFFER_ADDR, (uint32) g_decrypt.InOutLen, brec_name, 11, 0);
    if (dir_p != NULL)
    {
        _para.brec_length = dir_p->length;
        _para.brec_offset = dir_p->offset + g_firmware_offset;
        upgrade_print("blen",_para.brec_length,2);
        upgrade_print("boff",_para.brec_offset,2);
    }
    else
    {
        return -1;
    }

    //get USB SN type
    sys_get_fw_info(&(_para.udisk_setting), FW_INFO_USB_SN_TYPE_ADDR, FW_INFO_USB_SN_TYPE_LEN);

    //获取USB序列号
    sys_get_fw_info(&(_para.UsbSetupInfo), FW_INFO_USB_ID_ADDR, FW_INFO_USB_ID_LEN);

    //获取相关盘符信息
    sys_get_fw_info(&(_para.CapInfo), FW_INFO_CAPINFO_ADDR, FW_INFO_CAPINFO_LEN);
    _para.progress = SEARCH_FMU;

    PRINT_INFO("search file over");

    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  固件升级程序
 * \param[in]    none
 * \return       upgrade_status_e
 * \retval       结果返回值
 * \note
 *******************************************************************************/
upgrade_status_e _upgrade_firmware(void)
{

    if (upgrade_fw_check() == FALSE)
    {
        upgrade_print("fw_file_err",0,0); //2015-12-03
        return FW_UPGRADE_CHECK_ERR;
    }

    if (search_content() != 0)
    {
        upgrade_print("fw_file_err1",0,0); //2015-12-03
        return FW_UPGRADE_FILE_ERR;
    }

    if (upgrade_fw_write() == FALSE)
    {
        return FW_UPGRADE_WRITE_ERR;
    }
    return FW_UPGRADE_NO_ERR;
}

