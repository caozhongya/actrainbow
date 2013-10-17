/*********************************************************


*********************************************************/

#include "ActRainbow.h"
#include <math.h>

//存储3轴加速度值的数据结构。
AxesRaw_t AxesBuffer;
//获取到的当前的时间值。
volatile Calendar newTime;
//中断标志位。
int INTStatusFlag;
unsigned char step_time_flag =0 ;
work_mode modeFlag = DAY_MODE;//DAY_MODE;//NIGHT_MODE;
//work_mode modeFlag = NIGHT_MODE;//DAY_MODE;//NIGHT_MODE;
#define COMPARE_VALUE 2184


   /*  
  /////50HZ ?¨ê±?÷?μ?a200 bsp.C
#define SAMPLE_QUANTITY 50
#define TIME_WINDOW_MIN 10
#define TIME_WINDOW_MAX 35
#define ANGL_APP  2.0f
#define G_BUFFER_QUANTITY  5
#define ANGL_QUANTITY      7
*/
/*
  /////20HZ ?¨ê±?÷?μ?a500 bsp.C
#define SAMPLE_QUANTITY 20
#define TIME_WINDOW_MIN 3
#define TIME_WINDOW_MAX 10
#define ANGL_APP  2.0f
#define G_BUFFER_QUANTITY  5
#define ANGL_QUANTITY      7
*/
  /////15HZ  ?¨ê±?÷?μ?a666 bsp.C
#define SAMPLE_QUANTITY 15
#define TIME_WINDOW_MIN 1
#define TIME_WINDOW_MAX 12
#define ANGL_APP  1.0f
#define G_BUFFER_QUANTITY  3
#define ANGL_QUANTITY      5
//*****************************************************************************
//
short RealSample[3];
AxesRaw_t buff_raw;
short ax,ay,az;   
float sqrt_sample;
float angl_sample;
float g_x,g_y,g_z;

float g_x_old[G_BUFFER_QUANTITY],g_y_old[G_BUFFER_QUANTITY],g_z_old[G_BUFFER_QUANTITY];
float angl_sample_old[ANGL_QUANTITY];
float angl_sample_sum,angl_sample_sum1;
unsigned char sampling_counter = 0;

float		angl_sample_sum_old;
float		angl_app;
float		angl_threshold;
float		max_angl=-1;
float		min_angl =10000;
unsigned char	angl_TimerCnt=0;
unsigned char angl_TimerCnt_old[3]={0};
unsigned char	angl_fgPossitive=0;
unsigned char	angl_fgNegative=0;

float		max_g_x = -100.0f;
float		max_g_y = -100.0f;
float		max_g_z = -100.0f;
float		min_g_x = 100.0f;
float		min_g_y = 100.0f;
float		min_g_z = 100.0f;
float          g_x_app;
float          g_y_app;
float          g_z_app;

unsigned char angl_buffer[30]; 
unsigned char temp_buffer[50];  
unsigned int   STEPS=0;

unsigned char buffer1[6];
void StepCountHandler(void)
{
  
	unsigned char jtemp,i;
       
	unsigned char data[2],test;

	
//        test = readreg();
          LIS3DH_MultiReadacc(buffer1,6);
//         LIS3DH_GetAccAxesRaw(&buff_raw);
//          ax=buff_raw.AXIS_X/32767.0f*2;
//          ay=buff_raw.AXIS_Y/32767*2;
//          az=buff_raw.AXIS_Z/32767*2;
////          MEMS_Read_Data();
//          RealSample[0] = ax;
//          RealSample[1] = ay;
//          RealSample[2] = az;
//	        g_x = ax*0.001f;
//	        g_y = ay*0.001f;
//	        g_z = az*0.001f;
//          	g_x = buff_raw.AXIS_X/32767.0f*2;
//	        g_y = buff_raw.AXIS_Y/32767.0f*2;
//	        g_z = buff_raw.AXIS_Z/32767.0f*2;
//	
	        for(i=0;i<G_BUFFER_QUANTITY-1;i++)
	         { 
		    g_x_old[G_BUFFER_QUANTITY-1-i] = g_x_old[G_BUFFER_QUANTITY-i-2];
		    g_y_old[G_BUFFER_QUANTITY-1-i] = g_y_old[G_BUFFER_QUANTITY-i-2];
		    g_z_old[G_BUFFER_QUANTITY-1-i] = g_z_old[G_BUFFER_QUANTITY-i-2];
		 }
	         g_x_old[0] = buff_raw.AXIS_X/32767.0f*2;
		 g_y_old[0] = buff_raw.AXIS_Y/32767.0f*2;
		 g_z_old[0] = buff_raw.AXIS_Z/32767.0f*2;
		 g_x=0;
		 g_y=0;
		 g_z=0;
		for(i=0;i<G_BUFFER_QUANTITY;i++)
		{
		 g_x += g_x_old[i]/G_BUFFER_QUANTITY;
		 g_y += g_y_old[i]/G_BUFFER_QUANTITY;
		 g_z += g_z_old[i]/G_BUFFER_QUANTITY;
                }
// 					 g_x = (g_x_old[0]+g_x_old[1]+g_x_old[2]+g_x_old[3]+g_x_old[4])/5;
// 					 g_y = (g_y_old[0]+g_y_old[1]+g_x_old[2]+g_y_old[3]+g_y_old[4])/5;
// 					 g_z = (g_z_old[0]+g_z_old[1]+g_z_old[2]+g_z_old[3]+g_z_old[4])/5;
		sqrt_sample = sqrt(g_x*g_x+g_y*g_y+g_z*g_z);
		for(i=0;i<ANGL_QUANTITY;i++)
	         { 
		  angl_sample_old[ANGL_QUANTITY-1-i] = angl_sample_old[ANGL_QUANTITY-i-2];
		 }
                angl_sample_old[0] =(acos(g_x/sqrt_sample)*acos(g_y/sqrt_sample)+ acos(g_z/sqrt_sample)*acos(ay/sqrt_sample)+acos(ax/sqrt_sample)*acos(az/sqrt_sample));
                angl_sample_sum_old  =  angl_sample_sum;
		angl_sample_sum1 =0.0f;
		for(i=0;i<ANGL_QUANTITY;i++)
		{
		 angl_sample_sum1 += angl_sample_old[i];
                }
//		angl_sample_sum1 =angl_sample_old[0]+angl_sample_old[1]+angl_sample_old[2]+angl_sample_old[3]+angl_sample_old[4]+angl_sample_old[5]+angl_sample_old[6];
                angl_sample_sum =  angl_sample_sum1*angl_sample_sum1;
					 //	         angl_sample_sum =angl_sample_old[0]+angl_sample_old[1]+angl_sample_old[2]+angl_sample_old[3]+angl_sample_old[4];
// 					 sprintf(angl_buffer,"%5.5f,%5.5f,%5.2f,%5.2f\r\n",g_x,g_y,g_z,angl_sample_sum);
// 	         STM_EVAL_SerialSend(COM1, angl_buffer);
// 					 

					 /////////////////////////////////////////////////////////////////////////////
					 
		if(g_x > max_g_x)               {max_g_x = g_x;}
     	        if(g_x < min_g_x)               {min_g_x = g_x;}
					 
	        if(g_y > max_g_y)               {max_g_y = g_y;}
     	        if(g_y < min_g_y)               {min_g_y = g_y;}
					 
		if(g_z > max_g_z)               {max_g_z  = g_z;}
     	        if(g_z < min_g_z)               {min_g_z  = g_z;}
					 
		if(angl_sample_sum > max_angl)               {max_angl = angl_sample_sum;}
     		if(angl_sample_sum < min_angl)               {min_angl = angl_sample_sum;}
					 
		if(sampling_counter++ > SAMPLE_QUANTITY)
		{
		sampling_counter = 0;
		angl_app=max_angl-min_angl;         //·?·??μ
		g_x_app=max_g_x-min_g_x;
		g_y_app=max_g_y-min_g_y;
		g_z_app=max_g_z-min_g_z;
		angl_threshold =min_angl+0.5*angl_app;
		max_angl =  -1.0f ;
		min_angl =  10000.0f;
		max_g_x = -100.0f;
		max_g_y = -100.0f;
		max_g_z = -100.0f;
		min_g_x = 100.0f;
		min_g_y = 100.0f;
		min_g_z = 100.0f;
                }
		if (( angl_sample_sum - angl_sample_sum_old > 0.8 )&&((angl_fgPossitive == 0) && (angl_sample_sum_old < angl_threshold)))
		{     
			angl_TimerCnt = 0;
		        angl_fgPossitive = 1;
			angl_fgNegative= 0;
		}
		if (( angl_sample_sum_old - angl_sample_sum> 0.8 )&&((angl_fgNegative == 0) && (angl_sample_sum_old > angl_threshold)))
		{     
		        angl_TimerCnt =  0;
			angl_fgPossitive = 0;
			angl_fgNegative= 1;
		}	
		if (angl_TimerCnt++ >= SAMPLE_QUANTITY)
		{    
			angl_TimerCnt =  0;
			angl_fgPossitive = 0;
			angl_fgNegative= 0; 
                 }
               if (angl_app >= ANGL_APP)
		{
			if	((angl_TimerCnt>=TIME_WINDOW_MIN)&&(angl_TimerCnt<=TIME_WINDOW_MAX))
			{
                          if ((((angl_sample_sum < angl_threshold)&&(angl_fgNegative==1))	|| ((angl_sample_sum > angl_threshold)&&(angl_fgPossitive==1)))&&(g_x_app*g_x_app+g_y_app*g_y_app+g_z_app*g_z_app >=0.25))
				{
				angl_TimerCnt_old[2] = angl_TimerCnt_old[1];
				angl_TimerCnt_old[1] = angl_TimerCnt_old[0];
				angl_TimerCnt_old[0] = angl_TimerCnt;
				STEPS=STEPS+1;
//         				 Serial_SendStep();
// 								 sprintf(temp_buffer,"%4d,%4d,%4d,%5.2f,%5.2f,%5.2f,%5.2f\r\n",angl_TimerCnt_old[0],angl_TimerCnt_old[1],angl_TimerCnt_old[2],g_x_app,g_y_app,g_z_app,g_x_app*g_x_app+g_y_app*g_y_app+g_z_app*g_z_app);
//  	               STM_EVAL_SerialSend(COM1, temp_buffer);
                                sprintf(temp_buffer,"Steps is %5d\r\n",STEPS);
                                uart1_sendbuf(temp_buffer);
                                if(STEPS%2 ==0)
                                  P6OUT &= ~0x01;
                                else
                                  P6OUT |= 0x01;
				angl_TimerCnt =  0;
				angl_fgPossitive = 0;
				angl_fgNegative= 0; 
                        }
		}
	 } 
}



int main(void)
{
  int i = 0,j=0;
//  i16_t value =0;
//  i16_t valueX =0;
//  i16_t valueY =0;
//  i16_t valueZ =0;5
//  uint8_t read_byte = 0;
//  status_t response;
//  LIS3DH_Click_Response Click_Response;
  // 初始化XT1 SMCLK 8MHz
  clock_config();                           
  initRTC();
  // IO口，i2c初始化。
  board_config(); 
  // 初始化UART1。BR == 9600
  uart1_config(); 
  // 根据手册延时最少5毫秒再操作LIS3DH.	
  delay_ms(6);
  // 初始化加速度计
  //LIS3DH_Init();
  LIS3DH_FIFO_Init();			    
//uart1_sendbuf("编译日期和时间:\r\n");
//printf("编译日期和时间:");
//printf("%s %s\r\n",__DATE__,__TIME__);  
 //uart1_sendbuf("Hello sonic");  
//  for(j=0;j<10;j++){
//    P6OUT ^= BIT1;
//    delay_ms(1000);
//  }
  
  
          //Start timer in continuous mode sourced by SMCLK
        TIMER_A_configureContinuousMode( TIMER_A1_BASE,
 //                                        TIMER_A_CLOCKSOURCE_SMCLK,
                                         TIMER_A_CLOCKSOURCE_ACLK,    //modify by caozhong
                                         TIMER_A_CLOCKSOURCE_DIVIDER_1,
                                         TIMER_A_TAIE_INTERRUPT_DISABLE,
                                         TIMER_A_DO_CLEAR
                                         );

        //Initiaze compare mode
        TIMER_A_clearCaptureCompareInterruptFlag(TIMER_A1_BASE,
                                                 TIMER_A_CAPTURECOMPARE_REGISTER_0
                                                 );
        TIMER_A_initCompare(TIMER_A1_BASE,
                            TIMER_A_CAPTURECOMPARE_REGISTER_0,
                            TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE,
                            TIMER_A_OUTPUTMODE_OUTBITVALUE,
                            COMPARE_VALUE
                            );

        TIMER_A_startCounter( TIMER_A1_BASE,
                              TIMER_A_CONTINUOUS_MODE
                              );
  
  
  //P6OUT ^= BIT1;
  while (1)                                 
  {
    if(DAY_MODE==modeFlag){//计步模式
      if(step_time_flag == 1)
      {
        step_time_flag =0;
 //       StepCountHandler();
//        LIS3DH_GetAccAxesRaw(&AxesBuffer); 
      }
//      LPM3;
    }
    else{//睡眠监控模式
      //P6OUT ^= BIT1;
      //LIS3DH_GetAccAxesRaw(&AxesBuffer);
//      StepCountHandler();
      //newTime = RTC_A_getCalendarTime(RTC_A_BASE);
      //LIS3DH_GetClickResponse(&Click_Response);
      //get the click response.
      //putchar(Click_Response);
      //uart1_sendbuf("");
      
      P6OUT ^= BIT1;
      for(i=0;i<100;i++){
      
      sendOutData(AxesBuffer.AXIS_X);
      sendOutData(AxesBuffer.AXIS_Y);
      sendOutData(AxesBuffer.AXIS_Z);
      putchar(0xAA);
      putchar(0xBB);
      
      }
      
      P6OUT ^= BIT1;
//      valueX = (AxesBuffer.AXIS_X/32767)*2;
//      valueY = (AxesBuffer.AXIS_Y/32767)*2;
//      valueZ = (AxesBuffer.AXIS_Z/32767)*2;
//      value = sqrt((AxesBuffer.AXIS_X * AxesBuffer.AXIS_X) +
//                   (AxesBuffer.AXIS_Y * AxesBuffer.AXIS_Y) +
//                    (AxesBuffer.AXIS_Z * AxesBuffer.AXIS_Z));
//      
//      sendOutData(valueX);
//      sendOutData(valueY);
//      sendOutData(valueZ);
//      sendOutData(value);
        delay_ms(10000);
      //LPM1      
    }
  }
  
}

//******************************************************************************
//
//This is the TIMER1_A3 interrupt vector service routine.
//
//******************************************************************************
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
        uint16_t compVal = TIMER_A_getCaptureCompareCount(TIMER_A1_BASE,
                                                          TIMER_A_CAPTURECOMPARE_REGISTER_0)
                           + COMPARE_VALUE;
        step_time_flag = 1;
        //Add Offset to CCR0
        TIMER_A_setCompareValue(TIMER_A1_BASE,
                                TIMER_A_CAPTURECOMPARE_REGISTER_0,
                                compVal
                                );
}



