#include "user_app.h"
#include "app_easy_timer.h"
#include "oled.h"
static timer_hnd user_timer;

#define USER_TIME_INTERVAL 100 
void user_app_time_handle()
{
	  int i = 0;
		user_timer =  app_easy_timer(USER_TIME_INTERVAL, user_app_time_handle);
		for(i=0; i<8; i++)
		{
		 LCD_P16x16Ch(i*16,0,i);  //µãÕóÏÔÊ¾
		} 
		LCD_P6x8Str(0,4,"12345");
}

void user_app_start()
{
		LCD_Init();
		user_timer = app_easy_timer(USER_TIME_INTERVAL, user_app_time_handle);
}	