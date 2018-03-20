/*******************************************************************************
 *                              5116
 *                            Module: musicenc
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       shenlei      2015-12-15    1.0             build this file
 *******************************************************************************/

#include "ap_record.h"
/******************************************************************************
 *                    
 * par  Description:
 *    录音文件写接口。
 * param[in]    buf:	录音数据
 * param[in]    size:	sector number default:1
 * param[in]    count:	bytes per sector default:512
 * param[in]    storage_io_t:	提供和标准文件系统操作接口一致的存储设备输入输出接口
 * return       写入的实际字节数
 * note: 加密算法可以在这里编写
*******************************************************************************/


int32 mmm_mr_write(void *buf, int32 size, int32 count, storage_io_t *io)
{
    int32 ret = 0;
    //libc_print("REC WR!",0,0);
    //  if( size != 1 || count != 512 || buf == NULL){
    //    return -1;
    //  }
    //libc_print("mr write",0,0);
#ifdef WIN32
    ret = fwrite(buf, 1, count, io->handel);
#else
    ret = (int32) vfs_file_write(g_fs_param.vfs_handle, buf, count, io->handel);
#endif

    return ret;
}