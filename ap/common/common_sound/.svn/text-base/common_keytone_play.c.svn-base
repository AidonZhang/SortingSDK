/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：按键音管理模块，支持FIFO方式延迟播放，通过创建一个独立的按键音播放子线程
 *       进行播放；该模块还包括设置按键音ASRC通道，设置DAC采样率，以及按键音开关。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_keytone.h"

typedef struct
{
    sd_handle kt_handle;
    uint16 kt_offset;
    uint16 kt_length;
    uint16 kt_frame_size;
    uint8  kt_fill_flag;
    uint32 kt_compact;
} kt_play_info_t;

#define KEYTONE_FRAME_SIZE  512

uint8 g_bitstream_buffer[KEYTONE_FRAME_SIZE] _BANK_DATA_ATTR_;

const uint8 g_keytone_file[KEYTONE_FILENAME_LEN] =
{
    "keytone.pcm",
};



uint8 keytone_fifo_out(void);
bool keytone_fifo_in(uint8 kt_id);

/******************************************************************************/
/*!
 * \par  Description:
 * \void keytone_init_set(void)
 * \按键音初始化配置
 * \param[in]    none
 * \param[out]   none
 * \return       int the result
 * \retval           0 sucess
 * \retval           -1 failed
 * \ingroup
 * \note
 */
/*******************************************************************************/
void keytone_init_set(void)
{
    //CHAN 0 -- URAM0 = 2KB
    //CHAN 1 -- PCMRAM2 = 4KB
    enable_dac(DAFIS_ASRC, g_keytone_infor.dac_chan);
//    config_asrc((g_keytone_infor.dac_chan == DAF0_EN)? K_OUT0_U0 : K_OUT1_P2, g_keytone_infor.dac_chan);
    config_asrc(K_OUT1_P2, 2);
    set_asrc_rate(g_keytone_infor.dac_sample, g_keytone_infor.dac_chan, 5000);
}

void keytone_send_to_asrc(kt_play_info_t *p_kt_play_info)
{
    uint32 data_count = p_kt_play_info->kt_frame_size;
    //temp use
    long long shift_use;
    //position to buffer
    uint32 buf_ptr = 0;
    int16 chanel_data;
    uint32 check_ret;

    while (data_count > 0)
    {
        if (g_keytone_infor.dac_chan == DAF0_EN)
        {
            check_ret = act_readl(ASRC_OUT0_NUM);
        }
        else
        {
            check_ret = act_readl(ASRC_OUT1_NUM);
        }

        if (check_ret != 0)
        {
            chanel_data = *(int16 *) &g_bitstream_buffer[buf_ptr];
            shift_use = (long long) chanel_data * p_kt_play_info->kt_compact;
#pragma __PRQA_IGNORE_START__
            shift_use >>= 10;
#pragma __PRQA_IGNORE_END__
            shift_use <<= 16;
            if (shift_use > 0x7fffffff)
            {
                shift_use = 0x7fffffff;
            }
            if (shift_use < -0x7fffffff)
            {
                shift_use = -0x7fffffff;
            }

            if (g_keytone_infor.dac_chan == DAF0_EN)
            {
                (*(volatile int32 *) ASRC_OUT0_WFIFO) = shift_use;
                (*(volatile int32 *) ASRC_OUT0_WFIFO) = shift_use;
            }
            else
            {
                (*(volatile int32 *) ASRC_OUT1_WFIFO) = shift_use;
                (*(volatile int32 *) ASRC_OUT1_WFIFO) = shift_use;
            }
              
            //position move
            buf_ptr += 2;
            //data count sub
            data_count -= 2;
        }
        else
        {
            p_kt_play_info->kt_fill_flag = TRUE;
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int32 keytone_play_frame(void* null0, void* null1, void* null2)
 * \按键音播放
 * \param[in]    none
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup
 * \note
 */
/*******************************************************************************/
int32 keytone_play_frame(kt_play_info_t *p_kt_play_info)
{
    //播放音频帧
    int32 play_ret;

    if (p_kt_play_info->kt_offset >= p_kt_play_info->kt_length)
    {
        return -1;
    }

    sys_os_sched_lock();

    //播放音频帧
    if (sys_sd_fseek(p_kt_play_info->kt_handle, 0, p_kt_play_info->kt_offset) == -1)
    {
        play_ret = -1;
        goto file_seek_fail;
    }
    //read frame data
    play_ret = sys_sd_fread(p_kt_play_info->kt_handle, g_bitstream_buffer, p_kt_play_info->kt_frame_size);

    if (play_ret != -1)
    {
        if (p_kt_play_info->kt_offset == 0)
        {//前4字节为文件长度，清为0
            g_bitstream_buffer[0] = 0;
            g_bitstream_buffer[1] = 0;
            g_bitstream_buffer[2] = 0;
            g_bitstream_buffer[3] = 0;
        }

        //change data offset
        p_kt_play_info->kt_offset += (uint16) play_ret;

        //file size
        if ((uint16) play_ret < p_kt_play_info->kt_frame_size)
        {
            p_kt_play_info->kt_frame_size = (uint16) play_ret;
        }

        //send data to asrc
        keytone_send_to_asrc(p_kt_play_info);
    }

    file_seek_fail:
    sys_os_sched_unlock();

    return play_ret;
}

void keytone_play_loop(void *param)
{
    kt_play_info_t kt_play_info;
    int32 play_ret;

    g_keytone_infor.playing = TRUE;

    kt_play_info.kt_compact = 0x0fec; //固定系数
    kt_play_info.kt_fill_flag = FALSE;
    sys_os_sched_lock();
    kt_play_info.kt_handle = sys_sd_fopen(g_keytone_file);
    sys_os_sched_unlock();
    if (kt_play_info.kt_handle == NULL)
    {
        //open fail
        goto fail_open;
    }

    //set hardware
    keytone_init_set();

    kt_play_info.kt_frame_size = KEYTONE_FRAME_SIZE;
    sys_sd_fseek(kt_play_info.kt_handle, 0, 0);
    sys_sd_fread(kt_play_info.kt_handle, &(kt_play_info.kt_length), 2);
    kt_play_info.kt_offset = 0;

    while (1)
    {
        play_ret = keytone_play_frame(&kt_play_info);
        if (play_ret == -1)
        {
            break;
        }

        //ASRC OUT启动后才需要延时
        if (kt_play_info.kt_fill_flag == TRUE)
        {
            kt_play_info.kt_fill_flag = FALSE;
            sys_os_time_dly(2);
        }
    }
    //等待按键音播放结束
    while (1)
    {
        if (g_keytone_infor.dac_chan == DAF0_EN)
        {
            if ((act_readl(ASRC_OUT0_IP) & (1 << ASRC_OUT0_IP_PCMFIFOEmpty)) != 0)
            {
                break;
            }
        }
        else
        {
            if ((act_readl(ASRC_OUT1_IP) & (1 << ASRC_OUT1_IP_PCMFIFOEmpty)) != 0)
            {
                break;
            }
        }

    //    sys_os_time_dly(1);
    }
    
    close_asrc(K_OUT1_P2);

    //disable dac channel zero
    disable_dac(g_keytone_infor.dac_chan);
    //close source file
    sys_sd_fclose(kt_play_info.kt_handle);

    fail_open:

    g_keytone_infor.playing = FALSE;

    libc_pthread_exit();
}

void keytone_play_thread(uint8 kt_id)
{
    //打开按键音线程
    pthread_ext_param_t param;

    //初始化创建线程的参数
    param.pthread_param.start_rtn = (void *) keytone_play_loop;
    param.pthread_param.arg = (void *) (uint32) kt_id;
    param.pthread_param.ptos = (void *) KEYTONE_PLAY_STK_POS;/*实际使用0x190*/
    param.stk_size = KEYTONE_PLAY_STK_SIZE;

    libc_pthread_create(&param, KEYTONE_PLAY_PRIO, CREATE_NOT_MAIN_THREAD);
}

void keytone_set_dac_chan(dac_chenel_e dac_chan)
{
    if (dac_chan == g_keytone_infor.dac_chan)
    {
        return;
    }

    //等待按键音结束后才能切换dac通道
    while (g_keytone_infor.playing == TRUE)
    {
        sys_os_time_dly(1);
    }

    g_keytone_infor.dac_chan = dac_chan;
}


void keytone_set_dac_sample(uint8 dac_sample)
{
    g_keytone_infor.dac_sample = dac_sample;
}


void keytone_play_deal(void)
{
    uint8 kt_id;

    if (g_keytone_infor.playing == TRUE)
    {
        return;
    }

    kt_id = keytone_fifo_out();
    if (kt_id == 0xff)
    {
//        PRINT_WARNING("kt fifo out err!!");
        return;
    }

    keytone_play_thread(kt_id);

}


bool keytone_play(uint8 kt_id, uint8 mode)
{
    keytone_fifo_in(kt_id);
    keytone_play_deal();
    return TRUE;
}

void com_start_key_tone(uint8 mode)
{
    //按键音使能，才发出按键音
    if (sys_comval->key_tone == TRUE)
    {
        keytone_play(KEYTONE_KEYTONE, mode);
    }
}

uint8 keytone_fifo_out(void)
{
    uint8 kt_id, i;

    if (g_keytone_infor.count == 0)
    {
        return -1;
    }

    kt_id = g_keytone_infor.kt_fifo[0];
    g_keytone_infor.count--;

    for (i = 0; i < g_keytone_infor.count; i++)
    {
        g_keytone_infor.kt_fifo[i] = g_keytone_infor.kt_fifo[i + 1];
    }

    return kt_id;
}

bool keytone_fifo_in(uint8 kt_id)
{
    if (g_keytone_infor.count >= KEYTONE_FIFO_DEPTH)
    {
//        PRINT_WARNING("kt fifo in full!");
        return FALSE;
    }

    g_keytone_infor.kt_fifo[g_keytone_infor.count] = kt_id;
    g_keytone_infor.count++;

    return TRUE;
}

/*! \endcond */
