#include "user_app.h"
#include "app_easy_timer.h"
#include "oled.h"
#include "cs1237.h"

static timer_hnd user_timer;

#define USER_TIME_INTERVAL 100 
char buffer[16] = {0,};
void user_app_time_handle()
{
	  int i = 0;
	  int32 ad = 0;
		user_timer =  app_easy_timer(USER_TIME_INTERVAL, user_app_time_handle);
		for(i=0; i<8; i++)
		{
		 LCD_P16x16Ch(i*16,0,i);  //µãÕóÏÔÊ¾
		} 
		for(i=0;i < 1;i++)
		{
			
			memset(buffer,16,0);
			CS1237_ReadAD(i,&ad);
			sprintf(buffer,"ad=%d",ad);
			LCD_P6x8Str(0,4+i,buffer);
		}
		//sc5040b_play(1);
}

void user_app_start()
{
		LCD_Init();
		CS1237_Init(AD_HZ_40);
		user_timer = app_easy_timer(USER_TIME_INTERVAL, user_app_time_handle);
}	