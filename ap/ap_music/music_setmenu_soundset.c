/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-21          1.0              create this file
 *******************************************************************************/

#include "app_music.h"

/******************************************************************************/
/*!
 * \par  Description:
 ��Ҷ�Ӳ˵�����ʱ���ݵ�ǰ���õ�EQ��������EQ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool
 * \ingroup      music_setmenu_soundset.c
 * \note
 * \li  ����Ҷ�Ӳ˵�֧��ʵʱ�����ص�����˿��ܸı�ʵ�ʵ�EQ�����ظ��˵�ʱ���øú���
 �ָ������赱ǰEQ
 */
/*******************************************************************************/
static void restore_music_eq(void)
{
    _scene_play_seteq(g_setting_comval.g_comval.eq_type);
}


/******************************************************************************/
/*!
 * \par  Description:
 EQ���ø��˵�ȷ��ִ�к��������ݵ�ǰ��EQֵ���м�����ƥ��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eq_sure(void* param)
{
    //ָ�򷵻ز�����ָ��
    menu_title_action_t* retptr = (menu_title_action_t*) param;

    //ȷ���ַ���id
    switch (g_setting_comval.g_comval.eq_type)
    {
        //eq off
        case VALUE_EQ_OFF:
        retptr->str_id = S_OFF;
        break;
        //eq rock
        case VALUE_EQ_ROCK:
        retptr->str_id = S_ROCK;
        break;
        //ea funk
        case VALUE_EQ_FUNK:
        retptr->str_id = S_FUNK;
        break;
        //eq hip hop
        case VALUE_EQ_HIP_HOP:
        retptr->str_id = S_HIPHOP;
        break;
        //eq jazz
        case VALUE_EQ_JAZZ:
        retptr->str_id = S_JAZZ;
        break;
        //eq classical
        case VALUE_EQ_CLASSICAL:
        retptr->str_id = S_CLASSIC;
        break;
        //eq teckno
        case VALUE_EQ_TECHNO:
        retptr->str_id = S_TECHNO;
        break;

        default:
        retptr->str_id = -1;
        break;
    }

    //���÷��ص�ǰ���˵��Ļص�����
    g_eq_restore = 1;
    gui_menulist_set_back2parent(restore_music_eq, &g_eq_restore);
    //������һ���˵�
    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq����ΪnormalģʽҶ�Ӳ˵�ִ�к���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT ������һ���˵�
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqnormal_sure(void *param)
{
    //����eqģʽ
    eq_param_t eq_info;

    eq_info.eq_mode = EQ_NORMAL;
    eq_info.eq_type = VALUE_EQ_OFF;
    
    music_set_eq(&eq_info);
    //eq�ر�
    g_setting_comval.g_comval.eq_type = VALUE_EQ_OFF;
    //���ص���һ���˵�
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq����ΪrockģʽҶ�Ӳ˵�ִ�к���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT ������һ���˵�
 * \retval       �ȼ�����ֵ
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqrock_sure(void *param)
{
    //����eqģʽ
    eq_param_t eq_info;
    
    eq_info.eq_mode = EQ_EQ_USR_MODE;
    eq_info.eq_type = VALUE_EQ_ROCK;
    
    music_set_eq(&eq_info);
    //eq rock
    g_setting_comval.g_comval.eq_type = VALUE_EQ_ROCK;


    //���ص���һ���˵�
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq����ΪfunkģʽҶ�Ӳ˵�ִ�к���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT ������һ���˵�
 * \retval       �ȼ�����ֵ
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqfunk_sure(void *param)
{
    //����eqģʽ
    eq_param_t eq_info;
    
    eq_info.eq_mode = EQ_EQ_USR_MODE;
    eq_info.eq_type = VALUE_EQ_FUNK;

    music_set_eq(&eq_info);
    //eq funk
    g_setting_comval.g_comval.eq_type = VALUE_EQ_FUNK;


    //���ص���һ���˵�
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq����ΪhiphopģʽҶ�Ӳ˵�ִ�к���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT ������һ���˵�
 * \retval       �ȼ�����ֵ
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqhiphop_sure(void *param)
{
    //����eqģʽ
    eq_param_t eq_info;
    
    eq_info.eq_mode = EQ_EQ_USR_MODE;
    eq_info.eq_type = VALUE_EQ_HIP_HOP;

    music_set_eq(&eq_info);
    //eq soft
    g_setting_comval.g_comval.eq_type = VALUE_EQ_HIP_HOP;

    //���ص���һ���˵�
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq����ΪjazzģʽҶ�Ӳ˵�ִ�к���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT ������һ���˵�
 * \retval       �ȼ�����ֵ
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqjazz_sure(void *param)
{
    //����eqģʽ
    eq_param_t eq_info;
    
    eq_info.eq_mode = EQ_EQ_USR_MODE;
    eq_info.eq_type = VALUE_EQ_JAZZ;
    
    music_set_eq(&eq_info);
    //eq jazz
    g_setting_comval.g_comval.eq_type = VALUE_EQ_JAZZ;

    //���ص���һ���˵�
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq����ΪclassicalģʽҶ�Ӳ˵�ִ�к���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT ������һ���˵�
 * \retval       �ȼ�����ֵ
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqclassical_sure(void *param)
{
    //����eqģʽ
    eq_param_t eq_info;
    
    eq_info.eq_mode = EQ_EQ_USR_MODE;
    eq_info.eq_type = VALUE_EQ_CLASSICAL;
    
    music_set_eq(&eq_info);
    //eq classical
    g_setting_comval.g_comval.eq_type = VALUE_EQ_CLASSICAL;

    //���ص���һ���˵�
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq����ΪtechnoģʽҶ�Ӳ˵�ִ�к���
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT ������һ���˵�
 * \retval       �ȼ�����ֵ
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqtechno_sure(void *param)
{
    //����eqģʽ
    eq_param_t eq_info;

    eq_info.eq_mode = EQ_EQ_USR_MODE;
    eq_info.eq_type = VALUE_EQ_TECHNO;
    
    music_set_eq(&eq_info);
    //eq dbb
    g_setting_comval.g_comval.eq_type = VALUE_EQ_TECHNO;

    //���ص���һ���˵�
    return RESULT_MENU_PARENT;
}



/******************************************************************************/
/*!
 * \par  Description:
 ����ʵ�ʵ�EQ�������ص�����
 * \param[in]    eq_mode  EQ���ͣ��μ�eq_param_t
 * \param[in]    eq_type  EQ�������μ�eq_param_e
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
static app_result_e set_scene_set_real_eq(uint8 eq_mode, uint8 eq_type)
{
    //����eqģʽ
    eq_param_t eq_info;

    eq_info.eq_mode = eq_mode;
    eq_info.eq_type = eq_type;
    
    music_set_eq(&eq_info);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 ����EQΪNORMAL��ʱ�ص�����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_eqnormal_callback(void)
{
    set_scene_set_real_eq(EQ_NORMAL, EQ_PARAM_NONE);
}

/******************************************************************************/
/*!
 * \par  Description:
 ����EQΪrock��ʱ�ص�����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_eqrock_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_ROCK);
}

/******************************************************************************/
/*!
 * \par  Description:
 ����EQΪfunk��ʱ�ص�����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_eqfunk_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_FUNK);
}

/******************************************************************************/
/*!
 * \par  Description:
 ����EQΪhiphop��ʱ�ص�����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_eqhiphop_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_HIPHOP);
}

/******************************************************************************/
/*!
 * \par  Description:
 ����EQΪjazz��ʱ�ص�����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_eqjazz_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_JAZZ);
}

/******************************************************************************/
/*!
 * \par  Description:
 ����EQΪclassical��ʱ�ص�����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_eqclassic_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_CALSSIC);
}

/******************************************************************************/
/*!
 * \par  Description:
 ����EQΪtechno��ʱ�ص�����
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_scene_eqtechno_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_TECHNO);
}

