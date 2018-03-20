/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：物理音量管理接口，包括设置音量和获取音量；支持I2S。
 * 作者：cailizhen
 ********************************************************************************/

#include "psp_includes.h"
#include "music_common.h"
#include "common_func.h"
#include "ccd_interface.h"


//音量值 -> pa vol + da vol, step by 0.375db * N, or 1db, 1.6db, 2db, 3db
const vol_hard_t g_hard_volume_table[VOLUME_TABLE_MAX+1][VOLUME_VALUE_MAX+1] =
{
    //0db
    {
        /* 0db,     -0.375db,   -0.75db,    -1.375db,   -2db        -2.75db,    -3.375db,   -4db */
        {40, 0xbf}, {40, 0xbe}, {40, 0xbd}, {39, 0xbe}, {38, 0xbf}, {38, 0xbd}, {37, 0xbe}, {36, 0xbf},
        /* -4.75db, -5.375db,   -6db,       -6.75db,    -7.375db    -8db,       -8.75db,    -9.375db */
        {36, 0xbd}, {35, 0xbe}, {34, 0xbf}, {34, 0xbd}, {33, 0xbe}, {32, 0xbf}, {32, 0xbd}, {31, 0xbe},
        /* -10db,   -11db,      -12db,      -13db,      -14.3db     -15.8db,    -17.4db,    -19db */
        {30, 0xbf}, {29, 0xbf}, {28, 0xbf}, {27, 0xbf}, {26, 0xbf}, {25, 0xbf}, {24, 0xbf}, {23, 0xbf},
        /* -21.25db, -23.5db,   -25.75db,   -28.1db,    -31.2db     -35.3db,    -41.6db,    -60db */
        {22, 0xbd}, {20, 0xbf}, {19, 0xbd}, {17, 0xbf}, {15, 0xbf}, {13, 0xbf}, {10, 0xbf}, {0, 0xbf},
    },
    //-1db
    {
        /* -1db,    -1.375db,   -1.75db,    -2.375db,   -3db        -3.75db,    -4.375db,   -5db */
        {39, 0xbf}, {39, 0xbe}, {39, 0xbd}, {38, 0xbe}, {37, 0xbf}, {37, 0xbd}, {36, 0xbe}, {35, 0xbf},
        /* -5.75db, -6.375db,   -7db,       -7.75db,    -8.375db    -9db,       -9.75db,    -10.375db */
        {35, 0xbd}, {34, 0xbe}, {33, 0xbf}, {33, 0xbd}, {32, 0xbe}, {31, 0xbf}, {31, 0xbd}, {30, 0xbe},
        /* -11db,   -12db,      -13db,      -14.3db     -15.8db,    -17.4db,    -19db,      -20.5db */
        {29, 0xbf}, {28, 0xbf}, {27, 0xbf}, {26, 0xbf}, {25, 0xbf}, {24, 0xbf}, {23, 0xbf}, {22, 0xbf},
        /* -22.85db,   -25db,   -27.25db,   -29.7db     -32.7db,    -37.3db,    -43.7db,    -60db */
        {21, 0xbd}, {19, 0xbf}, {18, 0xbd}, {16, 0xbf}, {14, 0xbf}, {12, 0xbf}, {9, 0xbf},  {0, 0xbf},
    },
    //-2db
    {
        /* -2db,    -2.375db,   -2.75db,    -3.375db,   -4db        -4.75db,    -5.375db,   -6db */
        {38, 0xbf}, {38, 0xbe}, {38, 0xbd}, {37, 0xbe}, {36, 0xbf}, {36, 0xbd}, {35, 0xbe}, {34, 0xbf},
        /* -6.75db, -7.375db,   -8db,       -8.75db,    -9.375db    -10db,      -10.75db,   -11.375db */
        {34, 0xbd}, {33, 0xbe}, {32, 0xbf}, {32, 0xbd}, {31, 0xbe}, {30, 0xbf}, {30, 0xbd}, {29, 0xbe},
        /* -12db,   -13db,      -14.3db     -15.8db,    -17.4db,    -19db,      -20.5db,    -22.1db */
        {28, 0xbf}, {27, 0xbf}, {26, 0xbf}, {25, 0xbf}, {24, 0xbf}, {23, 0xbf}, {22, 0xbf}, {21, 0xbf},
        /* -24.25db,  -26.5db,  -28.85db,   -31.2db     -35.3db,    -39.4db,    -45.7db,    -60db */
        {20, 0xbd}, {18, 0xbf}, {17, 0xbd}, {15, 0xbf}, {13, 0xbf}, {11, 0xbf}, {8, 0xbf},  {0, 0xbf},
    },
    //-3db
    {
        /* -3db,    -3.375db,   -3.75db,    -4.375db,   -5db        -5.75db,    -6.375db,   -7db */
        {37, 0xbf}, {37, 0xbe}, {37, 0xbd}, {36, 0xbe}, {35, 0xbf}, {35, 0xbd}, {34, 0xbe}, {33, 0xbf},
        /* -7.75db, -8.375db,   -9db,       -9.75db,    -10.375db   -11db,      -11.75db,   -12.375db */
        {33, 0xbd}, {32, 0xbe}, {31, 0xbf}, {31, 0xbd}, {30, 0xbe}, {29, 0xbf}, {29, 0xbd}, {28, 0xbe},
        /* -13db,   -14.3db     -15.8db,    -17.4db,    -19db,      -20.5db,    -22.1db,    -23.5db */
        {27, 0xbf}, {26, 0xbf}, {25, 0xbf}, {24, 0xbf}, {23, 0xbf}, {22, 0xbf}, {21, 0xbf}, {20, 0xbf},
        /* -25db,   -26.5db,    -28.85db,   -31.2db     -35.3db,    -39.4db,    -45.7db,    -60db */
        {19, 0xbf}, {18, 0xbf}, {17, 0xbd}, {15, 0xbf}, {13, 0xbf}, {11, 0xbf}, {8, 0xbf},  {0, 0xbf},
    },
   
};

/*! \endcond */

