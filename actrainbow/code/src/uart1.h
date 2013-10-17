#ifndef __UART1_H
#define __UART1_H

#include <msp430.h>
#include <stdint.h>

void uart1_config(void);
void uart1_sendbuf(uint8_t* pbuf);
void sendOutData(uint16_t data);
#endif