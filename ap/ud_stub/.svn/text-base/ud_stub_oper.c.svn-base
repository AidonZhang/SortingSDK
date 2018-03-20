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
#include "stub_interface.h"
#include "ud_intern.h"
#include "libc_interface.h"

int stub_read_data_sub(uint8 *data_buffer, uint32 data_len)
{
    int32 ret_val;
    uint32 *pdata = (uint32 *)data_buffer;
    
retry:     
    pdata[0] = 0xffffffff; 
           
    //读取数据
    ret_val = cpu_data_transfer(EP2OUT, (uint32) data_buffer, data_len, (uint32) timeout, Max_Pkt_Size);

    if (g_read_mode == 0)
    {
        if (ret_val != 0)
        {
            reset_out_fifo(g_mem_id);
        }
    }
    else
    {
        if((act_readb(DPDMCTRL) & 0x40) == 0)
        {
            ret_val = -1;
        } 
                
        if (ret_val != 0)
        {
            if(pdata[0] != 0xffffffff) 
            {
                ret_val = 0;   
            } 
            else
            {            
                goto retry;    
            }
        }
    }
    
    return ret_val;                   
}

int stub_read(uint16 opcode, uint8 *data_buffer, uint32 data_len)
{
    stub_trans_cmd_t cmd;
    int ret_val = 0;
    
    cmd.type = 0xae;
    cmd.opcode = (uint8)(opcode >> 8);
    cmd.sub_opcode = (uint8) opcode;
    //cmd.reserved1 = 0;
    cmd.reserv = 0;
    cmd.data_len = data_len;
    CLEAR_WATCH_DOG;
    
    //发送设置命令
    ret_val = cpu_data_transfer(EP1IN, (uint32) & cmd, (uint32) sizeof(cmd), (uint32) timeout, Max_Pkt_Size);

    if (g_read_mode == 0)
    {  
        if (ret_val != 0)
        {
            reset_in_fifo(g_mem_id);    
            return ret_val;    
        }          
    }
    
    ret_val = stub_read_data_sub(data_buffer, data_len);
    
    return ret_val;
}

int32 stub_ext_rw(stub_ext_param_t *ext_param, uint32 rw_mode)
{
    int i, cmd_len;
    uint16 *pdata;
    stub_ext_cmd_t *stub_ext_cmd;
    uint16 check_sum;
    int ret_val;

    stub_ext_cmd = (stub_ext_cmd_t *) ext_param->rw_buffer;

    pdata = (uint16 *) stub_ext_cmd;

    cmd_len = sizeof(stub_ext_cmd_t) + ext_param->payload_len;

    if (rw_mode == 1)
    {
        stub_ext_cmd->type = 0xAE;
        stub_ext_cmd->reserved = 0;
        stub_ext_cmd->opcode = (uint8)(ext_param->opcode >> 8);
        stub_ext_cmd->sub_opcode = (uint8)(ext_param->opcode);
        stub_ext_cmd->payload_len = ext_param->payload_len;

        check_sum = 0;
        for (i = 0; i < (cmd_len / 2); i++)
        {
            check_sum += pdata[i];
        }

        pdata[cmd_len / 2] = check_sum;

        //发送命令
        ret_val = cpu_data_transfer(EP1IN, (uint32) stub_ext_cmd, cmd_len + 2, timeout, Max_Pkt_Size);

        if (ret_val != 0)
        {
            if((act_readb(DPDMCTRL) & 0x40) == 0)
            {
                return -1;
            }
        }
    }
    else
    {
        ret_val = stub_read_data_sub(stub_ext_cmd, cmd_len + 2);
        
        if(ret_val == 0)
        {
            check_sum = 0;
            for (i = 0; i < (cmd_len / 2); i++)
            {
                check_sum += pdata[i];
            }

            if (pdata[cmd_len / 2] != check_sum)
            {
                return -2;
            }                
        }
    }

    return ret_val;
}

int stub_ext_raw_rw(void *ext_cmd, uint32 cmd_len, uint8 rw_mode)
{
    int ret_val;

    if (rw_mode == 1)
    {
        //发送命令
        ret_val = cpu_data_transfer(EP1IN, (uint32) ext_cmd, cmd_len, timeout, Max_Pkt_Size);

        if (ret_val != 0)
        {
            reset_in_fifo(g_mem_id);

            return ret_val;
        }
    }
    else
    {
        ret_val = stub_read_data_sub(ext_cmd, cmd_len);
                        
        return ret_val;
    }
}

int stub_write(uint16 opcode, uint8 *data_buffer, uint32 data_len)
{
    stub_trans_cmd_t *cmd;
    stub_trans_cmd_t zero_cmd;
    int ret_val = 0;

    if (data_buffer != NULL)
    {
        cmd = (stub_trans_cmd_t *) data_buffer;
    }
    else
    {
        cmd = &zero_cmd;
    }

    cmd->type = 0xae;
    cmd->opcode = (uint8)(opcode >> 8);
    cmd->sub_opcode = (uint8) opcode;
    //cmd->reserved1 = 0;
    cmd->reserv = 0;
    cmd->data_len = data_len;

    //发送数据
    ret_val = cpu_data_transfer(EP1IN, (uint32) cmd, (data_len + STUB_COMMAND_HEAD_SIZE), (uint32) timeout,
            Max_Pkt_Size);

    if (ret_val != 0)
    {
        reset_in_fifo(g_mem_id);
    }
    return ret_val;
}

int stub_ioctl(uint16 op_code, void *param1, void *param2)
{
    switch (op_code)
    {
        case SET_TIMEOUT:
        timeout = (int) sys_get_delay_val(param1, 40);
        break;

        case RESET_FIFO:
        if (param1 == 0)
        {
            reset_in_fifo(g_mem_id);
        }
        else
        {
            reset_out_fifo(g_mem_id);
        }
        break;

        case SWITCH_URAM:
        switch_fifo_mem(param1);
        break;
        
        case SET_READ_MODE:
        g_read_mode = (uint8)param1;
        break;        

        default:
        break;
    }

    return 0;
}
