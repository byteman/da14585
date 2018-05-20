#include "batty.h"
#include "battery.h"
#include <stdlib.h>
#define ADC_CHANNEL_P01         (1)
void  battery_isr()
{

}
static uint32_t adc_get_vbat_sample2()
{
    uint32_t adc_sample, adc_sample2;

    adc_init(GP_ADC_SE, GP_ADC_SIGN, GP_ADC_ATTN3X);
    adc_usDelay(20);

   
    adc_enable_channel(ADC_CHANNEL_P01);

    adc_sample = adc_get_sample();
    adc_usDelay(1);
    adc_init(GP_ADC_SE, 0, GP_ADC_ATTN3X );

   
    adc_enable_channel(ADC_CHANNEL_P01);

    adc_sample2 = adc_get_sample();
    //We have to divide the following result by 2 if
    //the 10 bit accuracy is enough
    adc_sample = (adc_sample2 + adc_sample);
    adc_disable();

    return adc_sample;
}
uint8  battery_get()
{
	return battery_get_lvl(BATT_AAA);
}

