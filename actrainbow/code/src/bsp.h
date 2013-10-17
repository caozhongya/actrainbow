#ifndef __BSP__H
#define __BSP__H

#include "ActRainbow.h"

 void assert_failed(uint8_t* file, uint16_t line);
 void port_Init();
 void board_config();
 void i2c_config();

#endif