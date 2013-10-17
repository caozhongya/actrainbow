#include "uart1.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define BAUD_RATE   9600

#define GPIO_TXD  GPIO_PIN4
#define GPIO_RXD  GPIO_PIN5

void uart1_config(void)
{
  
  P4SEL |= (BIT4 | BIT5);                       // P5.7 UCA1@RXD P5.6 UCA1@TXD
  
  UCA1CTL1 |= UCSWRST;
  UCA1CTL1 |= UCSSEL_2;                       // 选择SMCLK时钟

  UCA1BR0 = 0x41;                               // 查表SMCLK=8MHz 波特率为115200
  UCA1BR1 = 0x03;
  UCA1MCTL |= UCBRS_3 + UCBRF_0;              // 0.404*8 = 3.232
  
  UCA1CTL1 &= ~UCSWRST;                       
  
 // UCA1IE |= UCRXIE;                           // 使能接收中断，勿忘使能全局中断
}

void uart1_sendbuf(uint8_t* pbuf)
{
  uint8_t len = strlen(pbuf);
  for( uint8_t i = 0 ; i < len ; i++ )
  {
    putchar(*pbuf++);
  }
}

int putchar(int ch)
{
	UCA1TXBUF = ch;
        //__delay_cycles(1000);
	while(!(UCA1IFG & UCTXIFG));
	return ch;
}


void sendOutData(uint16_t data){
  
//  putchar(0xAA);
//  putchar(0xBB);
//  putchar(0xCC);
//  putchar(0x22);
  putchar(data>>8);
  putchar(data);
  

}
