//
#include "power.h"
#include "gpio.h"
#include "user_peripheral.h"
#include "user_periph_setup.h"
#include "oled.h"

static uint8 g_pwr_state = PWR_ON;
static uint8 g_enable = 0;
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
uint8  power_isr(void)
{
	if(0 == g_enable){
		 return 0;
	}
	return 1;
}
static void powe_sleep(void)
{
		LCD_UnInit();
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


}

