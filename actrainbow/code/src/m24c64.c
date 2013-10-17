/* Includes ------------------------------------------------------------------*/
#include <msp430.h>

#define EEPROM_ADDRESS      0x18

typedef unsigned char u8_t;
typedef unsigned short int u16_t;
typedef short int i16_t;
typedef signed char i8_t;

u8_t M24C64_ReadReg(u8_t ReadAddr, u8_t* Data) {
  
    UCB1I2CSA = EEPROM_ADDRESS;        // LIS3DH_ADDRESS
    UCB1CTL1 |= UCTR;                 // дģʽ
    UCB1CTL1 |= UCTXSTT;              // ��������λ��д�����ֽ�

    UCB1TXBUF = ReadAddr;                  // �����ֽڵ�ַ������Ҫ�����TXBUF
    // �ȴ�UCTXIFG=1 ��UCTXSTT=0 ͬʱ�仯 �ȴ�һ����־λ����
    while(!(UCB1IFG & UCTXIFG))
    {
      if( UCB1IFG & UCNACKIFG )       // ����Ӧ�� UCNACKIFG=1
      {
        return 0;
      }
    }                        

    UCB1CTL1 &= ~UCTR;                // ��ģʽ
    UCB1CTL1 |= UCTXSTT;              // ��������λ�Ͷ������ֽ�

    while(UCB1CTL1 & UCTXSTT);        // �ȴ�UCTXSTT=0
    // ����Ӧ�� UCNACKIFG = 1
    UCB1CTL1 |= UCTXSTP;              // �ȷ���ֹͣλ

    while(!(UCB1IFG & UCRXIFG));      // ��ȡ�ֽ�����
    *Data = UCB1RXBUF;         // ��ȡBUF�Ĵ����ڷ���ֹͣλ֮��

    while( UCB1CTL1 & UCTXSTP );

    return 1; 

}

u8_t M24C64_WriteReg(u8_t WriteAddr, u8_t Data) {
  
  UCB1I2CSA = EEPROM_ADDRESS;        // LIS3DH_ADDRESS
  while( UCB1CTL1 & UCTXSTP );
  UCB1CTL1 |= UCTR;                 // дģʽ
  UCB1CTL1 |= UCTXSTT;              // ��������λ

  UCB1TXBUF = WriteAddr;            // �����ֽڵ�ַ
  // �ȴ�UCTXIFG=1 ��UCTXSTT=0 ͬʱ�仯 �ȴ�һ����־λ����
  while(!(UCB1IFG & UCTXIFG))
  {
    if( UCB1IFG & UCNACKIFG )       // ����Ӧ�� UCNACKIFG=1
    {
      return 0;
    }
  }   

  UCB1TXBUF = Data;           // �����ֽ�����
  while(!(UCB1IFG & UCTXIFG));      // �ȴ�UCTXIFG=1

  UCB1CTL1 |= UCTXSTP;
  while(UCB1CTL1 & UCTXSTP);        // �ȴ��������
  
  return 1;
}
