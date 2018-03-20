
/*******************************************************************************
 *                              US282A
 *                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
 *                             All Rights Reserved.
 *      Description:接口
 *      <author>       <time>           
 *       Wekan       2015-3-26pm            
 *******************************************************************************/

#include "key_inner.h"

typedef void* (*key_op_i)(void*, void*, void*);
/*1.驱动内部实现的具体接口函数声明*/
typedef struct 
{
    key_op_i __key_chargeget;
    key_op_i __key_chargeset;
    key_op_i __key_keytabaddr;
    key_op_i __key_getholdstate;
    key_op_i __key_speakcheck;
    key_op_i __key_adjust_freq_op;
    key_op_i __key_peripheral_detect;    
    key_op_i __key_earphonecheck;
    key_op_i __key_scan;
}block_key_operations;

/*2.定义接口操作集结构体，并进行初始化*/

block_key_operations blk_op =
{   (key_op_i)key_inner_chargeget,
    (key_op_i)key_inner_chargeset,
    (key_op_i)key_inner_getkeytabaddress,
    (key_op_i)key_inner_holdcheck,
    (key_op_i)key_inner_speakcheck,
    (key_op_i)adjust_freq_op_entry,
    (key_op_i)key_inner_peripheral_detect_handle,    
    (key_op_i)key_inner_earphone_check,        
    (key_op_i)key_scan_int_process,      
};


