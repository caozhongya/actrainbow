
#include <driverlib.h>
#include "rtc.h"
void initRTC(){
  
    Calendar currentTime;
//    // Select XT1
//    GPIO_setAsPeripheralModuleFunctionInputPin(
//        GPIO_PORT_P5,
//        GPIO_PIN4 + GPIO_PIN5
//        );
//    //Initialize LFXT1
//    UCS_LFXT1Start(UCS_BASE,
//               UCS_XT1_DRIVE3,
//               UCS_XCAP_3
//               );
    currentTime.Seconds    = 0x00;
    currentTime.Minutes    = 0x00;
    currentTime.Hours      = 0x12;
    currentTime.DayOfWeek  = 0x02;
    currentTime.DayOfMonth = 0x01;
    currentTime.Month      = 0x10;
    currentTime.Year       = 0x2013;
    //Initialize Calendar Mode of RTC
    /*
     * Base Address of the RTC_A_A
     * Pass in current time, intialized above
     * Use BCD as Calendar Register Format
     */
    RTC_A_calendarInit(RTC_A_BASE,
                       currentTime,
                       RTC_A_FORMAT_BCD);

    //Start RTC Clock
    RTC_A_startClock(RTC_A_BASE);


}