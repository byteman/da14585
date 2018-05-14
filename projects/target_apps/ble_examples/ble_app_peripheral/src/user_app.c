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
#include "gpio.h"
#include "user_periph_setup.h"
#include "wkupct_quadec.h"
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
static void user_app_set_button_event(uint8_t next_event);
static void user_app_button_press_cb(void)
{
    uint8_t next_btn_event = 0;

    // Read button state
    if(GPIO_GetPinStatus(KEY_ZERO_PORT, KEY_ZERO_PIN))
    {
        //app_sleep_env.custs1_btn_state = CUSTS1_BTN_STATE_RELEASED;
        next_btn_event = WKUPCT_PIN_POLARITY_LOW;
    }
    else
    {
        //app_sleep_env.custs1_btn_state = CUSTS1_BTN_STATE_PRESSED;
        next_btn_event = WKUPCT_PIN_POLARITY_HIGH;
    }

    // Configure next button event
    user_app_set_button_event(next_btn_event);
}
static void user_app_set_button_event(uint8_t next_event)
{

    wkupct_register_callback(user_app_button_press_cb);

    wkupct_enable_irq(WKUPCT_PIN_SELECT(KEY_ZERO_PORT, KEY_ZERO_PIN),
                      WKUPCT_PIN_POLARITY(KEY_ZERO_PORT, KEY_ZERO_PIN, next_event), // polarity
                      1, // 1 event
                      10); // debouncing time = 10 ms

}
void user_app_start()
{
		if(!init)
		{
			param_err_t err = PARA_ERR_NONE;
			user_app_button_press_cb();
			
			Timer_Init();
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
			//arch_ble_ext_wakeup_on();
			// Set extended sleep without OTP copy during connection
      //arch_set_extended_sleep(false);
			init = 1;
		}
	
}	