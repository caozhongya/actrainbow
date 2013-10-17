/*
 * interrupt.c
 *
 *  Created on: 2013-9-23
 *      Author: Xiezuyong
 */
#include <msp430.h>
#include <stdint.h>
#include "lis3dh_driver.h"
#include "gpio.h"


 unsigned char temp = 0; 
 unsigned char j=0;
 unsigned char buffer[25*6]={0};
//Timer0 A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
//	seg = seg_info.seg_num;
//	if(delay370us==0){
//		bitdata = *(pr_flash++);
//		LED_PORT = (bitdata<<2);
//		col++;
//		if(col==seg_info.bang_num[row]){//one row data send
//			col=0;//column reset to 0
//			if(resend==RESEND_ROW){//one row send twice?
//				row++;
//				pr_flash = (char *)(SEG_ADDR+(row*64));
//				resend=0;
//			}
//			else{//no，send one more time
//				resend++;
//			}
//		}
//		if(row==seg){//all data send once
//			row=0;
//			col=0;
//			resend=1;
//			retry--;
//		}
//		if(retry==0){//have send RETRY_ALL time
//	        //turn off LED
//			LED_PORT=0x00;
//			//reset the variable
//			retry = RETRY_ALL;
//			delay370us=3000;//1.11s
//			// CCR0 interrupt disabled
//			TA0CCTL0 &= ~CCIE;
//		    // Clear CB Interrupt flag
//			CBINT &= ~CBIFG;
//			//enable CompB Interrupt
//     		CBINT  |= CBIE;
//			_nop();// For debug
//		}
//	}
//	else{
//		delay370us--;
//	}
}

//// Comp_B ISR
#pragma vector=COMP_B_VECTOR
__interrupt void Comp_B_ISR (void)
{
//    // Clear Interrupt flag
//	CBINT &= ~CBIFG;
//	// TimerA CCR0 interrupt enabled
//    TA0CCTL0 = CCIE;
//    //disable CompB Interrupt
//    CBINT  &= ~CBIE;
//
//	_nop();// For debug
//
}

#pragma vector = PORT1_VECTOR
__interrupt void port1_ISR(void)
{

  	switch(P1IV)
    {
         case 0x04:
                   //P1.1IFG cleared
         GPIO_clearInterruptFlag(
                GPIO_PORT_P1,
                GPIO_PIN1
                );
          LIS3DH_GetFifoSourceBit(LIS3DH_FIFO_SRC_WTM,  &temp);
          if(temp == 1)
          {
//            for (j=0;j<25;j++)
//            {
//             LIS3DH_MultiReadacc(buffer+6*j ,6);
//            }
             LIS3DH_MultiReadacc(buffer,25*6);
          }
          break;
         default  :break;
	}
}
//******************************************************************************
//
//This is the USCI_A1 interrupt vector service routine.
//
//******************************************************************************
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR (void)
{
	uint16_t rxc = 0;
	switch (__even_in_range(UCA0IV,4)){
        //Vector 2 - RXIFG
        case 2:
//            //USCI_A0 TX buffer ready?
//            while (!USCI_A_UART_getInterruptStatus(USCI_A0_BASE,
//                       USCI_A_UART_TRANSMIT_INTERRUPT_FLAG)
//                   ) ;
//            //Receive echoed data
//            rxc = USCI_A_UART_receiveData(USCI_A0_BASE);
//
//            // 基于有限状态机的串口通信帧同步
//                if(0 == at_command.rxA) {
//                    if(0x41 == rxc)
//                    	at_command.rxA = 1;
//                }
//                else if(0 == at_command.rxT) {
//                    if(0x54 == rxc) {
//                    	at_command.rxT = 1;
//                    	at_command.rxLEN = 0;
//                    }
//                    else {
//                    	at_command.rxA = 0;
//                    }
//                }else if(0 == at_command.rxEqual) {
//
//                	if(0x3d == rxc) {
//                		at_command.rxEqual = 1;
//                	}else{
//                    	at_command.rxA = 0;
//                    	at_command.rxT = 0;
//                    	at_command.rxLEN = 0;
//                		at_command.rxEqual = 0;
//                	}
//                }
//                else if(0 == at_command.rxEnter1) {
//
//                	if(0x0d == rxc) {
//                		at_command.rxEnter1 = 1;
//                	}else{
//                    	at_command.rxA = 0;
//                    	at_command.rxT = 0;
//                    	at_command.rxLEN = 0;
//                		at_command.rxEnter1 = 0;
//                	}
//                }
//                else if(0 == at_command.rxEnter2){
//                  if(0x0a == rxc){
//                        at_command.rxEnter2 = 1;
//                        ATCommand_handler();	// AB_handler();
//                        at_command.rxA = 0;
//                        at_command.rxT = 0;
//                        at_command.rxLEN = 0;
//                        at_command.rxEqual =0;
//                        at_command.rxEnter1 = 0;
//                        at_command.rxEnter2 = 0;
//                  }
//                }
//                else{
//                      at_command.rxBUF[at_command.rxLEN] = rxc;
//                      at_command.rxLEN++;
//                    }
//            break;
        default: break;
    }
}
