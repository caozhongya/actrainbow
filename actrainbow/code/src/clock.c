#include "clock.h"
#include <driverlib.h>
static void select_xt1(void);
static void dco_config(void);

void clock_config(void)
{
    WDTCTL = WDTPW + WDTHOLD;                   // 停止看门狗
    __delay_cycles(250000);
    //UCS_LFXT1Start(,UCS_XT1_DRIVE3,UCS_XCAP_3);
    select_xt1();                               // 选择XT1
    dco_config();                               // ACLK = XT1 = 32.768K MCLK = SMCLK = 8000K
}

static void select_xt1(void)
{
    // Set up XT1
    P5SEL |= BIT4+BIT5;                       // Select XT1
    UCSCTL6 &= ~(XT1OFF);                     // XT1 On
    UCSCTL6 |= XCAP_3;                        // Internal load cap 
    UCSCTL6 |= XT1DRIVE_3;
    //UCSCTL6 &= ~(0x0020u);                    //LOW fRE MODE
    do
    {
        UCSCTL7 &= ~XT1LFOFFG;                  // 清楚XT1错误标记
    }while (UCSCTL7&XT1LFOFFG);                 // 检测XT1错误标记

}

static void dco_config(void)
{
    __bis_SR_register(SCG0);                    // Disable the FLL control loop		禁止FLL功能
    UCSCTL0 = 0x0000;                           // Set lowest possible DCOx, MODx
    UCSCTL1 = DCORSEL_5;                        // Select DCO range 16MHz operation
    UCSCTL2 = FLLD_1 + 243;                     // Set DCO Multiplier for 8MHz
                                                // (N + 1) * FLLRef = Fdco
                                                // (243 + 1) * 32768 = 8MHz         计算公式
                                                // Set FLL Div = fDCOCLK/2
    __bic_SR_register(SCG0);                    // Enable the FLL control loop		使能FLL功能

    // Worst-case settling time for the DCO when the DCO range bits have been
    // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
    // UG for optimization.
    // 32 x 32 x 8 MHz / 32,768 Hz = 250000 = MCLK cycles for DCO to settle
    __delay_cycles(250000);

    // Loop until XT1,XT2 & DCO fault flag is cleared
    do
    {
       // UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
                                                // Clear XT2,XT1,DCO fault flags	清除所有错误标志位
        SFRIFG1 &= ~OFIFG;                      // Clear fault flags				清除振荡器错误
    }while (SFRIFG1&OFIFG);                     // Test oscillator fault flag		等待
}
