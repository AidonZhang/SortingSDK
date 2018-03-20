#include "app_music.h"
#include "app_music_bookmark.h"

//.rodata
static const uint8 BM_FILENAME_SHORT[12] = "MUSICBMKBMK";
static const uint8 BM_FILENAME[] =
{
    0xff, 0xfe, //unicode flag
    0x4D, 0x00, //M
    0x55, 0x00, //U
    0x53, 0x00, //S
    0x49, 0x00, //I
    0x43, 0x00, //C
    0x42, 0x00, //B
    0x4D, 0x00, //M
    0x4B, 0x00, //K
    0x2E, 0x00, //.
    0x42, 0x00, //B
    0x4D, 0x00, //M
    0x4B, 0x00, //K
    0x00, 0x00 //end flag
};

/******************************************************************************/
/*!
 * \par  Description:
 * \void write_sector(uint32 sector_num, handle fp)
 * \дһ�����������ݵ��ļ�
 * \param[in]   sector_num ������� param1
 * \param[in]   fp �ļ���� param2
 * \param[out]
 * \return     void
 * \retval
 * \ingroup     music_bookmark
 * \note
 *******************************************************************************/
void write_sector(uint32 sector_num, uint32 fp)
{
    sector_num <<= 9;
    vfs_file_seek(g_music_mount_id, sector_num, SEEK_SET, fp);
    vfs_file_write(g_music_mount_id, bm_buf, BM_BUFF_SIZE, fp);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void read_sector(uint32 sector_num, handle fp)
 * \��һ������������
 * \param[in]   sector_num ������� param1
 * \param[in]   fp �ļ���� param2
 * \param[out]
 * \return     void
 * \retval
 * \ingroup    music_bookmark
 * \note
 *******************************************************************************/
void read_sector(uint32 sector_num, uint32 fp)
{
    if (sector_num != bmk_cache_sector)
    {
        bmk_cache_sector = (uint16)sector_num;
        sector_num <<= 9;
        vfs_file_seek(g_music_mount_id, sector_num, SEEK_SET, fp);
        vfs_file_read(g_music_mount_id, bm_buf, BM_BUFF_SIZE, fp);
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 * \���������Ŷ�ȡ����������
 * \param[in]   data_index ���
 * \param[in]   bm_fp ��ǩ���
 * \param[out]  none
 * \return      mbmk_index_t * ����������ָ��
 * \retval
 * \ingroup    music_bookmark
 * \note
 *******************************************************************************/

mbmk_index_t *read_bmk_index(uint16 data_index, uint32 bm_fp)
{
    uint8 sector_num;
    uint8 item_offset;
    //�������ݽṹ
    mbmk_index_t* bmk_index_item;

    uint16 index = data_index + 2;

    sector_num = (uint8)(index / BM_SECTOR_PER_SORT_INDEX);
    item_offset = (uint8)(index % BM_SECTOR_PER_SORT_INDEX);

    read_sector(sector_num, bm_fp);

    bmk_index_item = (mbmk_index_t*) (bm_buf);

    bmk_index_item += item_offset;

    return bmk_index_item;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \����������д������������
 * \param[in]   data_index ���
 * \param[in]   bm_fp ��ǩ���
 * \param[out]  none
 * \return      mbmk_index_t * ����������ָ��
 * \retval
 * \ingroup    music_bookmark
 * \note
 *******************************************************************************/

void write_bmk_index(uint16 data_index, uint32 bm_fp)
{
    uint8 sector_num;
    uint16 index = data_index + 2;

    sector_num = (uint8)(index / BM_SECTOR_PER_SORT_INDEX);
    write_sector(sector_num, bm_fp);
}
/******************************************************************************/
/*!
 * \par  Description:
 * \���������Ŷ�ȡ����������
 * \param[in]   data_index ���
 * \param[in]   bm_fp ��ǩ���
 * \param[out]  none
 * \return      mfile_bm_items_t * ����������ָ��
 * \retval
 * \ingroup    music_bookmark
 * \note
 *******************************************************************************/

mfile_bm_items_t *read_bmk_item(uint16 index, uint32 bm_fp)
{
    uint16 sector_no;
    uint8 sector_offset;
    //�������ṹ��
    mfile_bm_items_t* bm_data_ptr = (mfile_bm_items_t*) (bm_buf);

    sector_no = BM_HEADER_SECTOR_NUM;
    sector_no += index / 2;

    sector_offset = (uint8)(index % 2);
    //��ȡ��������
    //bmk_read_sector(sector_no, bm_fp);
    read_sector(sector_no, bm_fp);

    bm_data_ptr += sector_offset;

    return bm_data_ptr;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint32 open_bookmark(void)
 * \����ǩ�ļ�
 * \param[in]   none
 * \param[out]  none
 * \return      uint32 ��ǩ���
 * \retval      �򿪳ɹ��򴴽��ɹ�,���ش򿪵ľ����
 * \ingroup     music_bookmark
 * \note
 *******************************************************************************/
uint32 open_bookmark(void)
{
    uint32 bm_fp;

    bmk_cache_sector = 0xffff;
    bm_fp = vfs_file_open(g_music_mount_id, BM_FILENAME, OPEN_MODIFY);

    return bm_fp;
}


