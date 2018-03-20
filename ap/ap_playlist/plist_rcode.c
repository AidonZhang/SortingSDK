/*******************************************************************************
 *                              US212A
 *                            Module: PLAYLSIT
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     *.c
 * \brief    ������д�ļ��ĸ���
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               ������ļ���������
 * \par      EXTERNALIZED FUNCTIONS:
 *               �����������õ������ģ��
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/
#include "plist.h"
#include "rdata_ex.h"

const uint8 m3u_ext_buf[2][4] = {"M3U",""};

/******************************************************************************/
/*
 * \par  Description: д���ļ���������֤��������
 *
 * \param[in]     data_buf--д�����ݵĵ�ַ
 len--���ݳ���(�ֽ�)

 * \param[out]

 * \return        TRUE OR FALSE

 * \note   ��¼��ǰ�ļ�λ��
 *******************************************************************************/
//д���ļ���������֤�������룬��¼�ļ�λ��
bool save_to_file(uint8 *data_buf, uint32 len)
{
    uint16 sectors;
    uint16 i;

    sectors = (uint16) ((len - 1) / SECTOR_SIZE) + 1;

#if 0
    //������д��
    if(0==vfs_file_write(ap_vfs_mount,data_buf,SECTOR_SIZE*sectors,ap_obj_handle))
    {
        return FALSE;
    }
#else
    // ������д��
    for (i = 0; i < sectors; i++)
    {
        if (0 == vfs_file_write(ap_vfs_mount, data_buf, SECTOR_SIZE, ap_obj_handle))
        {
            return FALSE;
        }
        data_buf += SECTOR_SIZE;
    }

#endif
    ap_pos_file_offset += sectors;
    return TRUE;
}

/******************************************************************************/
/* !
 * \par  Description: ������д�ļ�
 *  bool write_to_file(uint8* dest,uint8* source, uint32 len)
 * \param[in]
 * \param[out]
 * \return  TRUE    �ɹ�
 FALSE   ʧ��
 * \note
 */
/*******************************************************************************/
bool write_to_file(uint8* dest, uint8* source, uint32 len)
{
    uint16 sectors;
    uint32 file_offs;
    uint16 i;

    file_offs = (uint32) dest;
    sectors = (uint16)((len - 1) / SECTOR_SIZE + 1);

    vfs_file_seek(ap_vfs_mount, file_offs, SEEK_SET, ap_obj_handle);

    //������д��
    for (i = 0; i < sectors; i++)
    {
        if (0 == vfs_file_write(ap_vfs_mount, source, SECTOR_SIZE, ap_obj_handle))
        {
            return FALSE;
        }
        source += SECTOR_SIZE;
    }

    //���¶�λ��֮ǰ���ļ�ƫ�Ƶ�ַ
    file_offs = ap_pos_file_offset * SECTOR_SIZE;
    vfs_file_seek(ap_vfs_mount, file_offs, SEEK_SET, ap_obj_handle);
    return TRUE;
}
/******************************************************************************/
/* !
 * \par  Description: ���������ļ�
 *  bool write_to_file(uint8* dest,uint8* source, uint32 len)
 * \param[in]
 * \param[out]
 * \return  TRUE    �ɹ�
 FALSE   ʧ��
 * \note
 */
/*******************************************************************************/
bool read_from_file(uint8* source, uint8* dest, uint32 len)
{
    uint16 sectors;
    uint32 file_offs;
    uint16 i;

    file_offs = (uint32) dest;
    sectors = (uint16)((len - 1) / SECTOR_SIZE + 1);

    vfs_file_seek(ap_vfs_mount, file_offs, SEEK_SET, ap_obj_handle);

    //��������
    for (i = 0; i < sectors; i++)
    {
        if (0 == vfs_file_read(ap_vfs_mount, source, SECTOR_SIZE, ap_obj_handle))
        {
            return FALSE;
        }
        source += SECTOR_SIZE;
    }

    //���¶�λ��֮ǰ���ļ�ƫ�Ƶ�ַ
    file_offs = ap_pos_file_offset * SECTOR_SIZE;
    vfs_file_seek(ap_vfs_mount, file_offs, SEEK_SET, ap_obj_handle);
    return TRUE;
}

/*
 void handle_timers()
 {

 }
 */

//���ˣ�gui��Ϣ�����Ѿ��������
//��ȡap˽����Ϣ��ϵͳ��Ϣ������ת��Ϊ˽����Ϣ�ٷ��أ���������

app_result_e plist_get_app_msg(void)
{
    private_msg_t private_msg;
    input_gui_msg_t gui_msg;
    app_result_e result = RESULT_NULL;

    if (get_gui_msg(&gui_msg) == TRUE)
    {
        if (INPUT_MSG_KEY == gui_msg.type)
        {
            if ((gui_msg.data.kmsg.val == KEY_LOCK) || (gui_msg.data.kmsg.val == KEY_UNLOCK))
            {
                gui_keylock(TRUE);
            }
        }
    }
    g_sys_counter.battery_counter = 100;//ÿ�ν��붼����͵�
    if (get_app_msg(&private_msg) == TRUE)
    {
        if (private_msg.msg.type == MSG_EARPHONE_IN)
        {
            /*���������ߣ�����*/
            change_antenna_state(TRUE);
        }
        else if (private_msg.msg.type == MSG_EARPHONE_OUT)
        {
            /*���������ߣ��γ�*/
            change_antenna_state(FALSE);
        }
        else
        {
        }
    }

    //����10ms�����������
    cpu_release_count++;
    if ((cpu_release_count % 5) == 00)
    {
        //��ʱ�ͷ�cpu
        sys_os_time_dly(1);
    }

    return result;
}
