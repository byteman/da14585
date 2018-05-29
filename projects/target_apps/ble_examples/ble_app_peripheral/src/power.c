//
#include "power.h"
#include "gpio.h"
#include "user_peripheral.h"
#include "user_periph_setup.h"


static uint8 g_pwr_state = PWR_ON;
static void power_on(void);
static void powe_sleep(void);
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

//电源启用
static void power_on(void)
{
	LCD_Init();
	channel_all_power(1);
	g_pwr_state = PWR_ON;
	
}
//睡眠状态就关闭lcd和adc通道.
static void powe_sleep(void)
{
		LCD_UnInit();
		channel_all_power(0);
		
}
void  power_ctrl(uint8 state)
{
		if(state == PWR_SLEEP){
					powe_sleep();
		}else if(state == PWR_OFF){
					power_off();
		}else{
					power_on();
		}
		
		g_pwr_state = state;

}
uint8 power_state(void)
{
	return g_pwr_state;

}
static void power_off(void)
{
		
		GPIO_SetInactive(PWR_OFF_PORT,PWR_OFF_PIN);
		//GPIO_SetInactive(PWR_OFF_PORT,PWR_OFF_PIN);
		while(1);
}
