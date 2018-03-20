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

_para_t _para =
{ 0 };

typedef struct
{
    uint32 cache_addr;
    uint32 cache_len;
} decrypt_cache_t;

decrypt_cache_t g_decrypt_cache;

struct fw_decrypt_operations g_decrypt_ops;

uint32 g_firmware_offset;

/******************************************************************************/
/*!
 * \par  Description:
 *      计算两字节校验和
 * \param[in]    buf   data buffer address
 * \param[in]    len   data length, 2byte uint
 * \param[out]   none
 * \return       the checksum
 * \retval
 * \note
 *******************************************************************************/
uint16 check_sum16(uint16 *buf, UINT32 len)
{
    uint32 index;
    uint16 sum = 0;

    for (index = 0; index < len; index++)
    {
        sum += buf[index];
    }

    return sum;
}

/******************************************************************************/
/*!
 * \par  Description:
 *      封装底层写逻辑扇区接口
 * \param[in]    pbuf           写入数据buffer
 * \param[in]    addr           写入逻辑区地址
 * \param[in]    len            写入长度
 * \return       the result
 * \retval       TRUE succeed  FALSE fail
 * \note
 *******************************************************************************/
uint32 ud_sector_write(void *pbuf, uint32 addr, uint32 len)
{
    uint32 ret_val;
    ud_sector_param_t ud_sec_param;

    if ((addr & 0x1ff) != 0)
    {
        //非512字节对齐
        return 0;
    }

    ud_sec_param.file_offset = (addr >> 9);
    ud_sec_param.sec_num = (len + 0x1ff) >> 9;
    ud_sec_param.sram_addr = (uint32) pbuf;

    ret_val = (uint32)base_fw_write(ud_sec_param.file_offset, ud_sec_param.sram_addr, ud_sec_param.sec_num);
    if (ret_val != 0)
    {
        return 0;
    }
    else
    {
        _para.g_write_sector += ud_sec_param.sec_num;
        return 1;
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 *      封装底层读逻辑扇区接口
 * \param[in]    pbuf           读入数据buffer
 * \param[in]    addr           读入逻辑区地址
 * \param[in]    len            读入长度
 * \return       the result
 * \retval       0 succeed  -1 fail
 * \note
 *******************************************************************************/
uint32 ud_sector_read(void *pbuf, uint32 addr, uint32 len)
{
    uint32 ret_val;
    ud_sector_param_t ud_sec_param;

    if ((addr & 0x1ff) != 0)
    {
        //非512字节对齐
        return 0;
    }

    ud_sec_param.file_offset = addr >> 9;
    ud_sec_param.sec_num = (len + 0x1ff) >> 9;
    ud_sec_param.sram_addr = (uint32) pbuf;

    ret_val = (uint32) base_fw_read(ud_sec_param.file_offset, ud_sec_param.sram_addr, ud_sec_param.sec_num);
    if (ret_val != 0)
    {
        return 0;
    }
    return 1;
}

/******************************************************************************/
/*!
 * \par  Description:
 *      封装底层brec逻辑扇区接口
 * \param[in]    pbuf           读入数据buffer
 * \param[in]    addr           读入逻辑区地址
 * \param[in]    len            读入长度
 * \return       the result
 * \retval       0 succeed  -1 fail
 * \note
 *******************************************************************************/
bool brec_sector_write(void *pbuf, uint32 addr, uint32 len)
{
    uint32 ret_val;
    ud_sector_param_t ud_sec_param;
    uint16 *p_lfi_length;
    uint32 lfi_sector_total;
    if ((addr & 0x1ff) != 0)
    {
        //非512字节对齐
        return FALSE;
    }

    if (addr == 0)
    {
        lfi_sector_total = (_para.lfi_length + 511) / 512;
        p_lfi_length = (uint16 *) (pbuf);
        p_lfi_length += 4;
        *p_lfi_length = (uint16) (lfi_sector_total);
    }

    ud_sec_param.file_offset = addr >> 0x9;
    ud_sec_param.sec_num = (len + 0x1ff) >> 9;
    ud_sec_param.sram_addr = (uint32) pbuf;

    ret_val = (uint32) base_brec_write(ud_sec_param.file_offset, ud_sec_param.sram_addr, ud_sec_param.sec_num);
    if (ret_val != 0)
    {
        return FALSE;
    }
    else
    {
        _para.g_write_sector += ud_sec_param.sec_num;
        return TRUE;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *      计算四字节校验和
 * \param[in]    buf   data buffer address
 * \param[in]    len   data length, 4byte uint
 * \param[out]   none
 * \return       the checksum
 * \retval
 * \note
 *******************************************************************************/
uint32 check_sum32(uint32 *buf, uint32 len)
{
    uint32 index;
    uint32 sum = 0;

    for (index = 0; index < len; index++)
    {
        sum += buf[index];
    }

    return sum;
}

/******************************************************************************/
/*!
 * \par  Description:
 *      读出brec修改nor id后写入nor
 * \param[in]    None           文件句柄
 * \return       bool
 * \retval       写入操作时返回值
 * \note
 *******************************************************************************/
bool brec_write(void)
{
    uint32 ret_val;
    uint16 sector_num;
    uint32 write_addr = 0;

    uint16 sector_total = (uint16) ((_para.brec_length + 511) / 512);
    //Mbrec_write();
    vfs_file_seek(_para.g_file_sys_id, _para.brec_offset, SEEK_SET, _para.file_handle);

    _para.progress = WRITE_BREC;

    while (sector_total > 0)
    {
        if (sector_total > 32)
        {
            sector_num = 32;
        }
        else
        {
            sector_num = sector_total;
        }

        ret_val = _read_file(16 * 1024);
        if (ret_val == 0)
        {
            //解密失败
            return FALSE;
        }
        //libc_print("snum",sector_num,2);
        ret_val = brec_sector_write((uint32 *) BUFFER_ADDR, write_addr, sector_num << 9);
        if (ret_val == 0)
        {
            return FALSE;
        }
        write_addr += sector_num << 9;
        sector_total -= sector_num;
        //libc_print("stotal",sector_total,2);
    }

    PRINT_INFO("brec write over");

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *      校验写入固件区的数据是否正确
 * \param[in]    none
 * \return       upgrade_status_e
 * \retval       校验结果返回值
 * \note
 *******************************************************************************/
bool lfi_check(void)
{
    LFIHead_t *lfi_head;
    LFIHead_dir_t *lfi_dir;
    uint32 check_sum;
    uint32 index;
    uint8 *buf;
    uint32 sector_num;
    uint32 sector_total;
    uint32 addr;

    lfi_head = (LFIHead_t *) BUFFER_ADDR;
    lfi_dir = (LFIHead_dir_t *) BUFFER_ADDR;

    //读入前8K
    ud_sector_read((uint32 *) BUFFER_ADDR, _para.lfi_start_addr * 512, 0x2000);

    //计算头部校验和
    check_sum = check_sum16((uint16*) BUFFER_ADDR, (sizeof(LFIHead_t) - 2) / 2);
    if (check_sum != lfi_head->Headchecksum)
    {
        return FALSE;
    }

    //SD区文件校验用另外8K空间
    buf = (uint8 *) (BUFFER_ADDR + 0x2000);

    for (index = 0; index < 240; index++)
    {
        if (0 == lfi_dir->diritem[index].filename[0])
        {
            break;
        }

        sector_total = (uint32) (lfi_dir->diritem[index].length / 0x200);
        addr = (lfi_dir->diritem[index].offset << 9);
        addr += _para.lfi_start_addr * 512;
        check_sum = 0;

        while (sector_total != 0)
        {
            if (sector_total > 16)
            {
                sector_num = 16;
            }
            else
            {
                sector_num = sector_total;
            }

            if (TRUE != ud_sector_read(buf, addr, sector_num << 9))
            {
                return FALSE;
            }

            check_sum = check_sum + check_sum32((uint32*) buf, sector_num * 512 / 4);

            addr += (sector_num << 9);
            sector_total -= sector_num;
        }

        if (lfi_dir->diritem[index].checksum != check_sum)
        {
            return FALSE;
        }

    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *      读取固件升级文件并写入固件区
 * \param[in]    None           文件句柄
 * \return       bool
 * \retval       写入操作时返回值
 * \note
 *******************************************************************************/
bool lfi_write(void)
{
    uint32 ret_val;
    uint32 sector_num;
    LFIHead_t *lfi_head;
    uint16 check_sum;
    uint32 write_addr;

    uint32 sector_total = (uint32) ((_para.lfi_length + 511) / 512);

    lfi_head = (LFIHead_t *) BUFFER_ADDR;

    //起始地址加参数区，以及引导扇区
#ifdef _CARD_BASE_
    _para.lfi_start_addr = 0;
#else
    _para.lfi_start_addr = ((_para.brec_length + 511) / 512) + 1 + 2;
#endif

    //准备解析FW
    vfs_file_seek(_para.g_file_sys_id, _para.lfi_offset, SEEK_SET, _para.file_handle);

    //读取并解析FW前16K数据
    ret_val = _read_file(16 * 1024);
    if (ret_val == 0)
    {
        //解密失败
        return FALSE;
    }

    _para.progress = READ_LFI;

    //get USB SN type
    sys_get_fw_info((uint8 *) &lfi_head->udisk_setting, FW_INFO_USB_SN_TYPE_ADDR, FW_INFO_USB_SN_TYPE_LEN);
    //获取USB序列号
    sys_get_fw_info(lfi_head->UsbSetupInfo, FW_INFO_USB_ID_ADDR, FW_INFO_USB_ID_LEN);

    //获取相关盘符信息
    sys_get_fw_info((uint32) & (lfi_head->CapInfo.vm_disk_cap), FW_INFO_CAPINFO_ADDR, FW_INFO_CAPINFO_LEN);
    libc_memcpy(&lfi_head->udisk_setting, &_para.udisk_setting, sizeof(_para.udisk_setting));

    libc_memcpy(&lfi_head->UsbSetupInfo, _para.UsbSetupInfo, sizeof(_para.UsbSetupInfo));

    libc_memcpy(&(lfi_head->CapInfo.vm_disk_cap), _para.CapInfo, sizeof(_para.CapInfo));

    //计算校验和
    check_sum = check_sum16((uint16*) BUFFER_ADDR, (sizeof(LFIHead_t) - 2) / 2);

    //写入头部校验和
    lfi_head->Headchecksum = check_sum;

    write_addr = _para.lfi_start_addr * 512;

    //写入16K数据
    ret_val = ud_sector_write((uint32 *) BUFFER_ADDR, write_addr, 16 * 1024);

    if (ret_val == 0)
    {
        return FALSE;
    }
    else
    {
        write_addr += 16 * 1024;
    }

    sector_total -= 32;

    while (sector_total > 0)
    {
        if (sector_total > 32)
        {
            sector_num = 32;
        }
        else
        {
            sector_num = sector_total;
        }

        ret_val = _read_file(16 * 1024);
        if (ret_val == 0)
        {
            //解密失败
            return FALSE;
        }

        ret_val = ud_sector_write((uint32 *) BUFFER_ADDR, write_addr, sector_num << 9);
        if (ret_val == 0)
        {
            return FALSE;
        }

        write_addr += sector_num << 9;
        sector_total -= sector_num;
    }

    _para.progress = WRITE_LFI;

    //PRINT_INFO("lfi write over");

    if (!lfi_check())
    {
        PRINT_INFO("lfi check failed");
        return FALSE;
    }

    //PRINT_INFO("upgrade over");

    _para.progress = UPGREADE_OVER;
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *  装载物理介质和文件系统，并检测是否存在升级文件
 * \param[in]    disk_type 磁盘类型
 * \param[out]   none
 * \return       none
 * \ingroup      upgrade
 * \note
 *******************************************************************************/
bool upgrade_fw_write(void)
{
    if ((brec_write()) && (lfi_write()))
    {
        base_clear_vram(); //擦除VRAM

        //量产成功
        //sys_vm_write(p_temp_buffer, VM_BTSTACK, 512); //回写蓝牙地址

        //立即退出，避免执行bank函数
        upgrade_print("now_will_boot_up",0,0); //2015-12-03
        //量产成功或进行到写nor阶段后失败，都重启
        CLEAR_WATCH_DOG;
        ENABLE_WATCH_DOG(3);
        while (1)
        {
            ;//nothing
        }
        //return TRUE;
    }

    return FALSE;
}

