
#include "bsp.h"

void i2c_config()
{
  P4SEL &= ~BIT2;                         // P3.2@UCB1SCL
  P4DIR |= BIT2;
  P4OUT |= BIT2;
  // ���9��ʱ�� �Իָ�I2C����״̬
  for( uint8_t i = 0 ; i < 9 ; i++ )
  {
    P4OUT |= BIT2;
    __delay_cycles(8000);
    P4OUT &= ~BIT2;
    __delay_cycles(8000);
  }
  
  P4SEL |= (BIT1 + BIT2);                  // P4.1-->SDA ��P4.2 -->SCL
  
  UCB1CTL1 |= UCSWRST;
  UCB1CTL0 = UCMST + UCMODE_3 + UCSYNC ;  // I2C����ģʽ
  UCB1CTL1 |= UCSSEL_2;                   // ѡ��SMCLK
  UCB1BR0 = 40;
  UCB1BR1 = 0;
  UCB1CTL0 &= ~UCSLA10;                   // 7λ��ַģʽ
//  UCB1I2CSA = EEPROM_ADDRESS;            // �����ַ���ڶ�д����ȥ���壬�Ա�ѡ��ͬ��I2C����
  UCB1CTL1 &= ~UCSWRST;
}

void board_config()
{
  port_Init();
  i2c_config();
  
  _EINT();                                    // ��ȫ���ж�
}



void assert_failed(uint8_t* file, uint16_t line)
{ 
  // ���������Ϣ
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
    P1REN |= LIS3DH_INT1_PIN;           //������������ʹ��
    P1OUT |= LIS3DH_INT1_PIN;           //����
    P1IES &= ~LIS3DH_INT1_PIN;          //�½�����
    P1IE  |=  LIS3DH_INT1_PIN;          //�ж�ʹ��
    P1IFG  = 0;                         //�жϱ�־�������ֹ���ù����г����жϱ�־Ϊ1
  //  /**********PORT2.0 V_EEPROM *************************/
    P2SEL = 0x00;                     //����IO��Ϊ��ͨI/Oģʽ
    P2DIR = 0xff;                     //����IO�ڷ���Ϊ���
    P2OUT = 0x00;                     //��ʼ����Ϊ00,�ر�EEPROM
}