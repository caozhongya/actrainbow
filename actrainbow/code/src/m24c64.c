/* Includes ------------------------------------------------------------------*/
#include <msp430.h>

#define EEPROM_ADDRESS      0x18

typedef unsigned char u8_t;
typedef unsigned short int u16_t;
typedef short int i16_t;
typedef signed char i8_t;

u8_t M24C64_ReadReg(u8_t ReadAddr, u8_t* Data) {
  
    UCB1I2CSA = EEPROM_ADDRESS;        // LIS3DH_ADDRESS
    UCB1CTL1 |= UCTR;                 // 写模式
    UCB1CTL1 |= UCTXSTT;              // 发送启动位和写控制字节

    UCB1TXBUF = ReadAddr;                  // 发送字节地址，必须要先填充TXBUF
    // 等待UCTXIFG=1 与UCTXSTT=0 同时变化 等待一个标志位即可
    while(!(UCB1IFG & UCTXIFG))
    {
      if( UCB1IFG & UCNACKIFG )       // 若无应答 UCNACKIFG=1
      {
        return 0;
      }
    }                        

    UCB1CTL1 &= ~UCTR;                // 读模式
    UCB1CTL1 |= UCTXSTT;              // 发送启动位和读控制字节

    while(UCB1CTL1 & UCTXSTT);        // 等待UCTXSTT=0
    // 若无应答 UCNACKIFG = 1
    UCB1CTL1 |= UCTXSTP;              // 先发送停止位

    while(!(UCB1IFG & UCRXIFG));      // 读取字节内容
    *Data = UCB1RXBUF;         // 读取BUF寄存器在发送停止位之后

    while( UCB1CTL1 & UCTXSTP );

    return 1; 

}

u8_t M24C64_WriteReg(u8_t WriteAddr, u8_t Data) {
  
  UCB1I2CSA = EEPROM_ADDRESS;        // LIS3DH_ADDRESS
  while( UCB1CTL1 & UCTXSTP );
  UCB1CTL1 |= UCTR;                 // 写模式
  UCB1CTL1 |= UCTXSTT;              // 发送启动位

  UCB1TXBUF = WriteAddr;            // 发送字节地址
  // 等待UCTXIFG=1 与UCTXSTT=0 同时变化 等待一个标志位即可
  while(!(UCB1IFG & UCTXIFG))
  {
    if( UCB1IFG & UCNACKIFG )       // 若无应答 UCNACKIFG=1
    {
      return 0;
    }
  }   

  UCB1TXBUF = Data;           // 发送字节内容
  while(!(UCB1IFG & UCTXIFG));      // 等待UCTXIFG=1

  UCB1CTL1 |= UCTXSTP;
  while(UCB1CTL1 & UCTXSTP);        // 等待发送完成
  
  return 1;
}
