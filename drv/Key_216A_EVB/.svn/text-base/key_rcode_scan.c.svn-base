/*******************************************************************************
 *                              US282A
 *                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
 *                             All Rights Reserved.
 *      Description:查询按键状态和处理
 *      <author>       <time>
 *       Wekan       2015-3-26pm
 *******************************************************************************/

#include "key_inner.h"

#define KEY_MSG_DEPTH   8//gui 消息队列深度
#define DOWN_KEY_TIMER  3//按键按下消息时间为60ms
#define LONG_KEY_TIMER  5//按键长按消息时间为 60ms + (5-1)*HOLD_KEY_TIMER = 700ms
#define HOLD_KEY_TIMER  8//按键连续hold消息间隔为160ms
#define NO_KEY KEY_NULL

uint8 g_oldkey_val = NO_KEY;
uint8 g_tmpkey_val = NO_KEY;
uint8 g_tmp_count;
uint8 g_hold_count;
uint8 g_up_count;
unsigned char wait_keyup = 0;
unsigned int key_value = KEY_NULL; //当前按键的值
unsigned int key_count = 0; //当前按键消息发生的次数

//第一个byte固定为play，其他按照线控按键顺序，从小到大安排
unsigned char key_map[16];

#if (SUPPORT_ADC_KEY == 1)
#define ADC_KEY_NUM  (7)

const uint8 g_adc_data[ADC_KEY_NUM] =
{ 0x08, 0x14, 0x1f, 0x2c, 0x3a, 0x48, 0x57 };
#endif


#ifdef lock_key
unsigned char TheLastHoldKey = Msg_KeyUnHold;
unsigned char RTCHold_Counter=0;
unsigned char TheFirstHold=0;
#endif

int g_time_irq;

uint8 g_key_bat_charge_mode; //0表示内部充电，1表示外部充电
uint16 g_charge_tri_time_cnt; //涓流充电时间计数器关充电JL.Li
uint8 g_battery_check_cnt;
uint8 g_charge_bat_check;
uint8 g_need_check_battery;

unsigned char g_speakonoff;
bool g_earphone_status = FALSE;
uint8 g_earphone_mode = 0;

void PutSysMsg(unsigned int key);


#ifdef SWITCH_KEY_POWER_OFF
/******************************************************************************/
/*!
 * \par  Description:
 * \ void shut_off_switch_detect(void)
 * \param[in]    none
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 * 软关机方案检测是否有关机操作(假硬开关？)
 *******************************************************************************/

void key_shut_off_switch_detect(void)
{

    uint16 msg;
    static uint8 count3 = 0;
    //input_gui_msg_t gui_msg;
    uint32 key_data;

    //dc5v exist, we wil' not send power off
    if ((act_readl(CHG_DET) & (3<<CHG_DET_UVLO_A)) != 0)
    {
        return;
    }
    act_writel((act_readl(SHUTOFF_DETECT_IO_OUT_EN_REG) & SHUTOFF_DETECT_CLR_BIT), SHUTOFF_DETECT_IO_OUT_EN_REG);
    act_writel((act_readl(SHUTOFF_DETECT_IO_IN_EN_REG) | SHUTOFF_DETECT_SET_BIT), SHUTOFF_DETECT_IO_IN_EN_REG);
    sys_udelay(100);
    if ((act_readl(SHUTOFF_DETECT_IO_DATA_REG) & SHUTOFF_DETECT_SET_BIT)==0)
    {
        count3++;
        if(count3 >= 20)
        {
            //发送拨动按键消息
        }
    }
}
#endif


/******************************************************************************/
/*!
 * \par  Description:
 *    Hold key check
 * \param[in]    unsigned char adc
 * \param[in]    none
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 * 检查hold键状态，并有状态变化则发送相应的hold消息
 *******************************************************************************/
#ifdef lock_key
void RTC_HoldCheck(unsigned char adc)
{
    if((adc>MIN_HOLDKEY_ADC)&&(adc<MAX_HOLDKEY_ADC))
    {
        //hold on
        TheFirstHold++;
        if(TheFirstHold>3)
        {
            TheFirstHold = 0;
            if(TheLastHoldKey != Msg_KeyHold )
            {
                PutSysMsg(Msg_KeyHold); //发键值消息
                TheLastHoldKey = Msg_KeyHold;
            }
        }

    }
    else if(adc>MAX_HOLDKEY_ADC) //no key
    {
        if(TheLastHoldKey == Msg_KeyHold )
        {
            PutSysMsg(Msg_KeyUnHold); //发键值消息
            TheLastHoldKey = Msg_KeyUnHold;
            TheFirstHold = 0;
        }
    }
    else
    {

    }
}
#endif


/******************************************************************************/
/*!
 * \par  Description:
 *    earphone key check
 * \param[in]    unsigned char adc
 * \param[in]    none
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 * 检查耳机接入状态
 *******************************************************************************/
void key_inner_earphone_check(uint8 onoff, void* null1, void* null2)
{
    g_earphone_mode = onoff;
    if(onoff == 0)
    {
        act_writel(act_readl(MFP_CTL2)|(1<<31),MFP_CTL2);
    }

}

/******************************************************************************/
/*!
 * \par  Description:
 *    void post_Earphone_msg(unsigned char adc)
 * \param[in]    unsigned char adc
 * \param[in]    none
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 * 耳机插拔消息发送
 *******************************************************************************/
void post_Earphone_msg(void)
{
    bool need_send = FALSE;
    uint16 msg;
    uint32 value;
    static uint8 count1 = 0;
    static uint8 count2 = 0;

/*#ifndef ENABLE_JTAG
    act_writel(act_readl(MFP_CTL2)&(~(1<<31)),MFP_CTL2);
    //EVB 使用GPIO_B1 进行耳机检测
    act_writel((act_readl(GPIOBOUTEN) & (0xfffffffD)), GPIOBOUTEN);
    act_writel((act_readl(GPIOBINEN) | (0x00000002)), GPIOBINEN);
    //B1 pu en
    act_writel(act_readl(GPIOBPUEN)|(0x00000002),GPIOBPUEN);
    value = act_readl(GPIOBDAT) & (0x00000002);
	
    // enable MEJTAGEN function
    act_writel(act_readl(MFP_CTL2)|(1<<31),MFP_CTL2);
#else
    value = 0;
#endif
*/
    act_writel(act_readl(MFP_CTL2)&(~(1<<31)),MFP_CTL2);
    act_writel((act_readl(GPIOBOUTEN) & (0xfffffffD)), GPIOBOUTEN);
    act_writel((act_readl(GPIOBINEN) | (0x00000002)), GPIOBINEN);
    act_writel(act_readl(GPIOBPUEN)|(0x00000002),GPIOBPUEN);

    value = act_readl(GPIOBDAT) & (0x00000002);

    if (value == 0)
    {
        count1++;
        count2 = 0;
        if (count1 > 10)
        {
            count1 = 0;
            //当前检测到有耳机
            if (TRUE != g_earphone_status) //耳机插上
            {
                g_earphone_status = TRUE;
                need_send = TRUE;
                msg = MSG_EARPHONE_IN;
            }
        }

    }
    else
    {
        count2++;
        count1 = 0;

        if (count2 > 10)
        {
            count2 = 0;
            if (FALSE != g_earphone_status) //耳机拔出
            {
                g_earphone_status = FALSE;
                need_send = TRUE;
                msg = MSG_EARPHONE_OUT;
            }
        }
    }

    if (need_send == TRUE)
    {
        sys_mq_send(MQ_ID_SYS, &msg); //发送到系统消息队列
    }
}


/******************************************************************************
 * \par  Description:   检测ADC 按键是否按下
 * \param[in]     none
 * \param[out]   adc_val 按按的adc采样值
 * \return           按键结果
 * \note
 *   <author>    <time>
 *    Wekan   2015-3-26pm
 *******************************************************************************/
#if (SUPPORT_ADC_KEY == 1)
uint8 key_lradc_status_check(uint8* adc_val)
{
    //check key val
    uint8 lradc_key_val = NO_KEY;
    uint8 i;
    uint8 adc_data;

    adc_data = act_readb(LRADC1_DATA) & 0x7F;//[0~6bit]
    for (i = 0; i < ADC_KEY_NUM; i++)
    {
        if (adc_data < g_adc_data[i])
        {
            lradc_key_val =key_map[i + 1];
            break;
        }
    }

    //check on off key
    if ((act_readl(ONOFF_KEY) & (1<<ONOFF_KEY_ONOFF_PRESS_0)) != 0)
    {
        lradc_key_val = key_map[0];//play
        adc_data = 0;//skip hold state
    }

    *adc_val = adc_data;
    return lradc_key_val;
}
#endif

/******************************************************************************
 * \par  Description:   按键抬起对扫描按键使用的变量初始化
 * \param[in]     none
 * \param[out]   none
 * \return           none
 * \note
 *   <author>    <time>
 *    Wekan   2015-3-26pm
 *******************************************************************************/

void key_init_key_vars(void)
{
    //清扫描次数
    g_tmp_count = 0;
    //hold key count
    g_hold_count = 0;
    //up key count
    g_up_count = 0;
    //last key value
    g_oldkey_val = NO_KEY;

    g_tmpkey_val = NO_KEY;
}

/******************************************************************************
 * \par  Description:  按键扫描以及充电轮询，
 每隔20ms扫描一次，按键已经含有去抖处理
 * \param[in]     none
 * \param[out]   none
 * \return           none
 * \note
 *   <author>    <time>
 *    Wekan   2015-3-26pm
 *******************************************************************************/
void key_scan_int_process(void)
{
    unsigned char adcdat;
    unsigned char key_val = NO_KEY;

    key_battery_charge_deal();

#if (SUPPORT_ADC_KEY == 1)
    //check if lradc key press
    key_val = key_lradc_status_check(&adcdat);
    if (key_val == NO_KEY)
    {
        #ifdef SWITCH_KEY_POWER_OFF
        key_shut_off_switch_detect();
        #endif
    }
#endif

    if (key_val == NO_KEY)
    {
        if(g_earphone_mode == 1)
        {
            post_Earphone_msg();
        }
    }

#ifdef lock_key
    RTCHold_Counter++;
    if(RTCHold_Counter>4)
    {
        RTCHold_Counter = 0;
        RTC_HoldCheck(adcdat);
    }
#endif

    if (key_val != g_oldkey_val) //如果本次扫描值不等于上次按键值
    {
        if (key_val == g_tmpkey_val) //如果本次键值等于前次扫描值
        {
            g_tmp_count++; //累加扫描次数

            if (g_tmp_count > DOWN_KEY_TIMER)
            {
                g_tmp_count = DOWN_KEY_TIMER;
            }

            if (g_tmp_count == DOWN_KEY_TIMER) //扫描三次，是同一键值，确定有按键按下
            {
                if (key_val != NO_KEY) //如果本次扫描捕获键值不为空
                {
                    if(wait_keyup == 1)
                    {
                        PutSysMsg(Msg_KeyShortUp); //发keyup消息
                    }
                    g_oldkey_val = g_tmpkey_val;
                    wait_keyup = 1;
                    PutSysMsg(g_oldkey_val);
                }
                else //本次键值为NO_KEY
                {
                    g_hold_count = 0;
                    g_oldkey_val = NO_KEY;
                    g_tmpkey_val = NO_KEY;

                    if (wait_keyup == 1) //等待KEYUP
                    {
                        wait_keyup = 0;
                        g_tmp_count = 0; //清扫描次数
                        PutSysMsg(Msg_KeyShortUp); //发keyup消息
                    }
                }
            }
        }
        else //如果本次扫描值不等于前次扫描值，则重置键值，重新计数
        {
            g_tmpkey_val = key_val;
            if (!wait_keyup)
            {
                g_tmp_count = 1;
            }
        }
    }
    else //如果本次键值等于上次按键值,累计8次(160ms)，发一次按键消息
    {
        if (key_val != NO_KEY) //
        {
            g_hold_count++;

            if (g_hold_count == HOLD_KEY_TIMER)
            {
                g_hold_count = 0;
                PutSysMsg(g_oldkey_val); //发键值消息
            }
        }
        else
        {
            g_tmpkey_val = key_val;
            g_hold_count = 0;
            if (!wait_keyup)
            {
                g_tmp_count = 0;    
            }
        }
    }

}


/******************************************************************************/
/*!
 * \par  Description:
 *	  Post key package message to OS
 * \param[in]    *input_msg
 * \param[in]    none
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \retval           0
 * \retval           1
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 * 封装按键信息，并发送给消息队列
 *******************************************************************************/
bool post_key_msg(input_gui_msg_t *input_msg)
{
    uint32 key_data;

    //消息压缩
    key_data = (uint32) input_msg->type;//byte 0
    if (input_msg->type == INPUT_MSG_KEY)
    {
        key_data |= ((uint32) input_msg->data.kmsg.val << 8);//byte 1
        key_data |= ((uint32) input_msg->data.kmsg.type << 16);//byte 2-3
    }

    else
    {

    }

    if ((int) sys_mq_send(MQ_ID_GUI, (void *) &key_data) == 0)
    {
        //libc_print("key", key_data);
        return TRUE;
    }

    //消息队列已满，返回false
    return FALSE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  Key message deal
 * \param[in]    key
 * \param[in]    none
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 * 处理长短按按键，并发送相应按键消息
 *******************************************************************************/
void PutSysMsg(unsigned int key)
{
    input_gui_msg_t input_msg;
    input_msg.type = INPUT_MSG_KEY;
    if (key == key_value)
    {

        //g_rtc_counter = 0;
        key_count++;

        /*按住按键时，160ms发送一次相同按键消息*/
        if (key_count == LONG_KEY_TIMER)
        {
            //key = (unsigned int) (key | AP_KEY_LONG); //=0.7s,发长按键消息
            input_msg.data.kmsg.val = key_value;
            input_msg.data.kmsg.type = KEY_TYPE_LONG;
        }
        else if (key_count > LONG_KEY_TIMER)
        {
            //key = (unsigned int) (key | AP_KEY_HOLD); //>0.7s,每160ms发Key hold消息
            input_msg.data.kmsg.val = key_value;
            input_msg.data.kmsg.type = KEY_TYPE_HOLD;
        }
        else
        {
            return;
        }
        //return key;   //putmsg

        post_key_msg(&input_msg);
        return;

    }
    switch (key)
    {
        case Msg_KeyShortUp:
        //gui消息队列已都是HOLD消息，可先取走一个再发 SHORT_UP ，这样保证 SHORT_UP 消息能够发送成功
        if(key_count >= LONG_KEY_TIMER + KEY_MSG_DEPTH)
        {
            //先接收gui消息，保证gui消息队列非满
            uint32 key_data;
            sys_mq_receive(MQ_ID_GUI, (void *)&key_data);
        }
        key_count = 0;
        //g_rtc_counter = 0;
        /*key_value的置需改变*/
        input_msg.data.kmsg.val = key_value;
        input_msg.data.kmsg.type = KEY_TYPE_SHORT_UP;
        key_value = KEY_NULL;

        break;
        case Msg_KeyHold:
        key_count = 0;
        //g_rtc_counter = 0;
        /*key_value的置需改变*/
        input_msg.data.kmsg.val = KEY_LOCK;
        input_msg.data.kmsg.type = KEY_TYPE_DOWN;
        key_value = KEY_NULL;
        break;
        case Msg_KeyUnHold:
        key_count = 0;
        //g_rtc_counter = 0;
        /*key_value的置需改变*/
        input_msg.data.kmsg.val = KEY_UNLOCK;
        input_msg.data.kmsg.type = KEY_TYPE_DOWN;
        key_value = KEY_NULL;
        break;
        default: /*新按键处理*/
        //g_rtc_counter = 0;
        key_value = key;
        input_msg.data.kmsg.val = key;
        input_msg.data.kmsg.type = KEY_TYPE_DOWN;
        key_count = 1;
        break;
    }
    //puts key msg in here

    post_key_msg(&input_msg);
    return;
}




