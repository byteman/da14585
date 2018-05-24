//
#include "power.h"
#include "gpio.h"
#include "user_peripheral.h"
#include "user_periph_setup.h"
#include "oled.h"
#include "channel.h"

static uint8 g_pwr_state = PWR_ON;
static uint8 g_enable = 0;
static int32_t g_sleep_count = 0;
static int32_t g_old_ad = 0;
static void delay_ms(unsigned int z)
{
unsigned int x,y;
for(x=z; x>0; x--)
	for(y=1848; y>0; y--);
}

static void delay_us(int nof_us)
{
    while( nof_us-- ){
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
    }
}
void power_enable(uint8 en)
{
	g_enable = en;
}
void  power_feed(void)
{

}
//定时来做唤醒操作.
uint8  power_isr(void)
{
	if(g_pwr_state != PWR_ON){
		 return 0;
	}
	//每隔1s唤醒一次，读取ad值.
	if(0==(g_sleep_count++ % 10)){
			
	}
	return 1;
}
static void power_on(void)
{
	LCD_Init();
	channel_all_power(0);
	g_pwr_state = PWR_ON;
	
}
static void powe_sleep(void)
{
		uint8 i = 0;
		g_old_ad = channel_get_filter_ad(0);
		LCD_UnInit();
		channel_all_power(1);
		g_sleep_count = 0;
		
}
void  power_ctrl(uint8 state)
{
		if(state == PWR_SLEEP){
					powe_sleep();
		}else if(state == PWR_OFF){
		
		}else{
				
		}
		
		g_pwr_state = state;

}
uint8 power_state(void)
{
	return g_pwr_state;

}

