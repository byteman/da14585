#include "user_app.h"
#include "app_easy_timer.h"
#include "oled.h"
#include "cs1237.h"
#include "batty.h"
#include "key.h"
#include "scaler.h"
#include "param.h"
#include "sc5040b.h"
#include "proclogic.h"

static timer_hnd user_timer;

#define USER_TIME_INTERVAL 10

static  uint8 init = 0;
void user_app_time_handle()
{
	  int i = 0;
	  int32 ad = 0;

		key_isr();
		gui_isr();
		scaler_run();
		audio_isr();
		
		user_timer =  app_easy_timer(USER_TIME_INTERVAL, user_app_time_handle);
	
}

void user_app_start()
{
		if(!init)
		{
			param_err_t err = PARA_ERR_NONE;
			sc5040b_init(); //音频播放初始化
			
			err  = param_init();
			if(err != PARA_ERR_NONE)
			{
					
					gui_show_error("load param err");
			}
			key_init();
			CS1237_Init(AD_HZ_40);
			scaler_init();
			gui_init(); //GUI显示的时候需要显示参数的值，所以要放到后面一点.
			user_timer = app_easy_timer(USER_TIME_INTERVAL, user_app_time_handle);
			init = 1;
		}
	
}	