//
#include "SC5040B.h"
#include "gpio.h"
#include "user_peripheral.h"
#include "user_periph_setup.h"
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
void SC50X0B_SDA(unsigned char data)
{
	unsigned char i;
	GPIO_SetInactive(AUDIO_SDA_PORT,AUDIO_SDA_PIN);
	delay_ms(5);
	for(i=0;i < 8;i++)
	{
	//SDA = 1;
		GPIO_SetActive(AUDIO_SDA_PORT,AUDIO_SDA_PIN);
	if(data & 0x01)
	{
	delay_us(1500);
	//SDA = 0;
		GPIO_SetInactive(AUDIO_SDA_PORT,AUDIO_SDA_PIN);
	delay_us(500);
	}else
	{
	delay_us(500);
	//SDA = 0;
	GPIO_SetInactive(AUDIO_SDA_PORT,AUDIO_SDA_PIN);
	delay_us(1500);
	}
	data >>= 1;
	}
	//SDA = 1;
	GPIO_SetActive(AUDIO_SDA_PORT,AUDIO_SDA_PIN);
	delay_ms(50);
}

int sc5040b_play(unsigned char index)
{
	SC50X0B_SDA(index);
	
	return 0;
}