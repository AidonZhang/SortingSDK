#include "fm_drv.h"
#include "fm_qn8035.h"

//��ȡ��ǰ����״̬
int sFM_GetAnten(void* antenna, void* null2, void* null3)
{
    uint8* p_antenna;
    uint32 value;

    p_antenna = (uint8*) antenna;
#if 0
    //Demo ��LRADC1 ���ж������
    adcdat = act_readb(LRADC1_DATA) & 0x3f;
    //����������
    if ((adcdat >= 0x2B) && (adcdat < 0x34))
    {
        *p_antenna = 1;
    }
    else
    {
        *p_antenna = 0;
    }
#endif

    // disable MEJTAGEN for GPIO_B1 function
    act_writel(act_readl(MFP_CTL2) & (~(1 << 31)), MFP_CTL2);
    //EVB ʹ��GPIO_B1 ���ж������
    act_writel((act_readl(GPIOBOUTEN) & (0xfffffffD)), GPIOBOUTEN);
    act_writel((act_readl(GPIOBINEN) | (0x00000002)), GPIOBINEN);
    //B1 pu en
    act_writel(act_readl(GPIOBPUEN) | (0x00000002), GPIOBPUEN);
    sys_mdelay(10);
    value = act_readl(GPIOBDAT) & (0x00000002);

    // enable MEJTAGEN function
    act_writel(act_readl(MFP_CTL2) | (1 << 31), MFP_CTL2);
    if (value == 0)
    {
        *p_antenna = 1;
    }
    else
    {
        *p_antenna = 0;
    }

    //��ȡ״̬�ɹ�
    return 0;
}

