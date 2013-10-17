
#include "bsp.h"

void i2c_config()
{
  P4SEL &= ~BIT2;                         // P3.2@UCB1SCL
  P4DIR |= BIT2;
  P4OUT |= BIT2;
  // 输出9个时钟 以恢复I2C总线状态
  for( uint8_t i = 0 ; i < 9 ; i++ )
  {
    P4OUT |= BIT2;
    __delay_cycles(8000);
    P4OUT &= ~BIT2;
    __delay_cycles(8000);
  }
  
  P4SEL |= (BIT1 + BIT2);                  // P4.1-->SDA ，P4.2 -->SCL
  
  UCB1CTL1 |= UCSWRST;
  UCB1CTL0 = UCMST + UCMODE_3 + UCSYNC ;  // I2C主机模式
  UCB1CTL1 |= UCSSEL_2;                   // 选择SMCLK
  UCB1BR0 = 40;
  UCB1BR1 = 0;
  UCB1CTL0 &= ~UCSLA10;                   // 7位地址模式
//  UCB1I2CSA = EEPROM_ADDRESS;            // 这个地址放在读写里面去定义，以便选择不同的I2C器件
  UCB1CTL1 &= ~UCSWRST;
}

void board_config()
{
  port_Init();
  i2c_config();
  
  _EINT();                                    // 打开全局中断
}



void assert_failed(uint8_t* file, uint16_t line)
{ 
  // 输出错误信息
  printf(" file %s on line %d\r\n", file , line);
  while (1)
  {
  }
}

void port_Init(){

  /**********PORT1.1  RGB PIN  *************************/
    //set P3 output
    P6DIR |= 0x07;
    //output off
    P6OUT &= ~0x07;
  /**********PORT1.1  LIS3DH INT1 PIN *************************/
    P1DIR &= ~LIS3DH_INT1_PIN ;         //P1.1
    P1REN |= LIS3DH_INT1_PIN;           //上拉下拉电阻使能
    P1OUT |= LIS3DH_INT1_PIN;           //上拉
    P1IES &= ~LIS3DH_INT1_PIN;          //下降触发
    P1IE  |=  LIS3DH_INT1_PIN;          //中断使能
    P1IFG  = 0;                         //中断标志清除，防止配置过程中出现中断标志为1
  //  /**********PORT2.0 V_EEPROM *************************/
    P2SEL = 0x00;                     //设置IO口为普通I/O模式
    P2DIR = 0xff;                     //设置IO口方向为输出
    P2OUT = 0x00;                     //初始设置为00,关闭EEPROM
}