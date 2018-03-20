#ifndef _KEY_INTERFACE_H            //防止重定义
#define _KEY_INTERFACE_H
#include "case_independent.h"
#include "psp_includes.h"
#include "common_bat_charge.h"

#define PER_DETECT_PERIOD           200 //外围设备检查周期
#define CARD_DETECT_TIMER           2   //卡检测周期为400ms = 200ms * 2
#define USBUHOST_DETECT_TIMER       2   //USB/UHOST检测周期为400ms（检测） + 200ms（切换后等待200ms再使能）

#define PER_DETECT_CARD             0x00000001 //要检测卡，比较耗时，所以检测周期会长一点
#define PRE_DETECT_CARD_FOR_IN      0x00000002 //置位表示当前卡不在，检测卡插入
#define PER_DETECT_CARD_TEST        0x00000004 //测试卡是否插着
#define PER_DETECT_CARD_FORCE       0x00000080 //强制进行卡检测，一般在前台应用刚切换过来后强制检测一次
#define PER_DETECT_AUX              0x00000100 //要检测AUX
#define PER_DETECT_AUX_FOR_IN       0x00000200 //置位表示当前AUX不在，检测AUX插入
#define PER_DETECT_AUX_TEST         0x00000400 //测试AUX是否插着
#define PER_DETECT_HP               0x00010000 //要检测HPEAPHONE
#define PER_DETECT_HP_FOR_IN        0x00020000 //置位表示当前HP没插着，检测HP插入
#define PER_DETECT_HP_TEST          0x00040000 //测试HP是否插着
#define PER_DETECT_USBUHOST_SWITCH  0x01000000 //要切换USB/UHOST
#define PER_DETECT_UHOST_TEST       0x02000000 //测试UHOST是否插着
#define PER_DETECT_USB_FORCE        0x40000000 //强制切换到USB
#define PER_DETECT_UHOST_FORCE      0x80000000 //强制切换到UHOST

/*1.定义提供的具体接口命令，这里是按键驱动统一提供的接口*/


typedef enum
{
    KEY_CHARGEGET = 0,
    KEY_CHARGESET , 
    KEY_KEYTABADDR,
    KEY_HOLDSTATE,
    KEY_SPEAKCHECK,
    KEY_ADJUST_FREQ_OP,    
    KEY_PERIPHERAL_DETECT,   
    KEY_EATPHONECHECK,
    KEY_SCAN,
    KEY_MAX,    
} key_cmd_e;


/*charge switch*/
typedef enum
{
    CHARGE_START = 0,
    CHARGE_STOP,
} charge_control_e;

extern void *key_op_entry(void *param1, void *param2, void *param3, key_cmd_e cmd)__FAR__;

#define key_chargeget(a)        (battery_status_e)key_op_entry((void*)(a),0,0,KEY_CHARGEGET)
#define key_chargeset(a,b,c)    key_op_entry((void*)(a),(void*)(b),(void*)(c),KEY_CHARGESET)
#define key_getkeytabaddress()  key_op_entry((void*)0,(void*)0,(void*)0,KEY_KEYTABADDR)
#define key_holdcheck()         key_op_entry((void*)0,(void*)0,(void*)0,KEY_HOLDSTATE)
#define key_speakcheck(a)       key_op_entry((void*)(a),0,0,KEY_SPEAKCHECK)

/***************************************/

typedef enum
{
    FREQ_NULL = 0,  //不耗用MIPS或DSP频率，即频率值为0
    FREQ_LEVEL1,    //如果是MIPS->1MM, 如果是DSP->35M
    FREQ_LEVEL2,    //如果是MIPS->2M,  如果是DSP->40M
    FREQ_LEVEL3,    //如果是MIPS->4M,  如果是DSP->55M
    FREQ_LEVEL4,    //如果是MIPS->6M,  如果是DSP->60M
    FREQ_LEVEL5,    //如果是MIPS->8M,  如果是DSP->70M
    FREQ_LEVEL6,    //如果是MIPS->10M, 如果是DSP->80M
    FREQ_LEVEL7,    //如果是MIPS->20M, 如果是DSP->100M
    FREQ_LEVEL8,    //如果是MIPS->30M, 如果是DSP->120M
    FREQ_LEVEL9,    //如果是MIPS->40M, 如果是DSP->140M
    FREQ_LEVEL10,   //如果是MIPS->60M, 如果是DSP->160M
    FREQ_LEVEL11,   //如果是MIPS->85M, 如果是DSP->180M
    FREQ_LEVEL_MAX
} adjust_freq_level_e;

//调频接口
typedef enum
{
    ADJUST_FQ_SET_LEVEL,
    ADJUST_FQ_ADD_VALUE,
    ADJUST_FQ_CMD_MAX,
} adjust_freq_cmd_e;


/*
 int adjust_freq_set_level(uint8 prio,uint8 level_a,uint8 level_b)
 该接口用于调整misp 和 dsp 频率档位
 prio：线程对应的优先级
 level_a：misp调频档位，adjust_freq_level_e,若为 FREQ_NULL 表示不耗频率
 level_b：dsp调频档位，adjust_freq_level_e，若为 FREQ_NULL 表示不耗频率
 返回值int：0-成功，非0失败
 !!!NOTE:bank代码，禁止在中断调用
 */
#define adjust_freq_set_level(prio,level_a,level_b)  \
(int)key_op_entry((void*)(uint32)(prio),(void*)(((uint32)(level_b) << 8) | ((uint32)(level_a))),(void*)ADJUST_FQ_SET_LEVEL, KEY_ADJUST_FREQ_OP)


/*
 int adjust_freq_add_value(int8 freq_a,int8 freq_b)
 该接口用于微调 misp 和 dsp 频率值,可调高或调低
 freq_a：叠加misp的频率，值可为负数
 freq_b：叠加dsp的频率，值可为负数
 返回值int：0-成功，非0失败
 !!!NOTE:bank代码，禁止在中断调用
 */
#define adjust_freq_add_value(freq_a,freq_b)  \
(int)key_op_entry((void*)(int)(int8)(freq_a),(void*)(int)(int8)(freq_b),(void*)ADJUST_FQ_ADD_VALUE, KEY_ADJUST_FREQ_OP)


/*
 int key_peripheral_detect_handle(uint32 detect_mode)
 该接口用于周期进行外部设备检测，检测周期为200ms，各外设自己决定检测周期为几个200ms
 detect_mode：表示要检测哪些外设，详见 PER_DETECT_XXX 宏定义
 测试模式调用时，返回1表示外设插着，返回0表示外设没插着
 */
#define key_peripheral_detect_handle(detect_mode)  \
(int)key_op_entry((void*)detect_mode,(void*)0,(void*)0, KEY_PERIPHERAL_DETECT)

#define key_earphonecheck(mode)  \
(int)key_op_entry((void*)mode,(void*)0,(void*)0, KEY_EATPHONECHECK)

#define key_scan()  \
(int)key_op_entry((void*)0,(void*)0,(void*)0, KEY_SCAN)    
#endif

