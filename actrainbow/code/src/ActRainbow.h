#ifndef __ACT_RAINBOW__H
#define __ACT_RAINBOW__H

#include <driverlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "lis3dh_driver.h"
#include "clock.h"
#include "uart1.h"
#include "rtc.h"
#include "bsp.h"


//INT1 ---->   P1.1
#define LIS3DH_INT1_PIN    BIT1
#define RGB_OUT  P6OUT

// 断言相关操作
#define assert_param(expr) ((expr) ? \
  (void)0 : \
  assert_failed((uint8_t *)__FILE__, __LINE__));
// 软件定时相关操作
#define F_CPU ((double)8000000) 
#define delay_us(x) __delay_cycles((long)(F_CPU*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(F_CPU*(double)x/1000.0))

typedef enum {
  DAY_MODE	=	0x01,
  NIGHT_MODE	=	0x00	
} work_mode;



#endif