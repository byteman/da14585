#include "batty.h"
//#include "battery.h"
#include <stdlib.h>
#define ADC_CHANNEL_P01         (1)
static inline uint16_t battery_moving_average_filter(uint16_t new_adc_value)
{
    static uint32_t old_value __attribute__((section("retention_mem_area0"),zero_init));
    const int filter_coefficient = 4;

    //this part of the filter is used to remove the delay on start up
    if ( old_value == 0)
        old_value = new_adc_value << filter_coefficient;
    else
    //this part filters the values
        old_value = old_value - (old_value >> filter_coefficient) + new_adc_value;
    return ( old_value >> filter_coefficient);
}
static inline uint16_t battery_filter_value (uint16_t new_adc_value)
{
	return(battery_moving_average_filter(new_adc_value));
    
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
#define BATTERY_MEASUREMENT_BUCK_AT_1V8     (0x3A0)
#define BATTERY_MEASUREMENT_BUCK_AT_2V0     (0x430)
#define BATTERY_MEASUREMENT_BUCK_AT_2V4     (0x510)
#define BATTERY_MEASUREMENT_BUCK_AT_2V8     (0x600)



static uint8_t batt_cal_aaa2(uint16_t adc_sample)
{
    uint8_t batt_lvl;

  
		//Buck mode
		if(adc_sample >= BATTERY_MEASUREMENT_BUCK_AT_1V8)
				batt_lvl = (adc_sample - BATTERY_MEASUREMENT_BUCK_AT_1V8)*100/
										(BATTERY_MEASUREMENT_BUCK_AT_2V8-BATTERY_MEASUREMENT_BUCK_AT_1V8);
		else
				batt_lvl = 0;
    
    return batt_lvl;
}

static uint32_t battery_get_lvl2(uint8_t batt_type)
{
    uint8_t batt_lvl;
    uint16_t adc_sample;

    adc_calibrate();

 
            // BUCK mode (2 x AAA batteries in series)
    adc_sample = adc_get_vbat_sample(false);
  
    adc_sample = battery_filter_value(adc_sample);


		return adc_sample;
    //batt_lvl = batt_cal_aaa2(adc_sample);


    //return batt_lvl;
}
/*
//获取电池电量级别
0-> 电量极低 闪烁图标  < 1.8V
1-> 电量为空 < 2V
2-> 1格电量  2V < x < 2.4V
3-> 2格电量	 2.4V < x < 2.8V
4-> 3格电量  >= 2.8V

0-> 电量极低 闪烁图标  < 3.45V
1-> 电量为空  3.45 < x < 3.7V
2-> 1格电量  3.7V < x < 3.8V
3-> 2格电量	 3.8V < x < 3.9V
4-> 3格电量  >= 3.9V
 
*/
#define BATTERY_MEASUREMENT_BUCK_AT_3V9     (0x5A5)
#define BATTERY_MEASUREMENT_BUCK_AT_3V8     (0x580)
#define BATTERY_MEASUREMENT_BUCK_AT_3V7     (0x55D)
#define BATTERY_MEASUREMENT_BUCK_AT_3V45     (0x500)

uint8 filter_batty(uint8 level)
{
	 
}
uint8  battery_get()
{
	uint8 level = 0;
	
	//level = battery_get_lvl(1);
	//uint32 old_value = 0;
	uint32 value = battery_get_lvl2(0);
	
	if(value < BATTERY_MEASUREMENT_BUCK_AT_3V45) return 0;
	else if(value < BATTERY_MEASUREMENT_BUCK_AT_3V7) return 1;
	else if(value < BATTERY_MEASUREMENT_BUCK_AT_3V8) return 2;
	else if(value < BATTERY_MEASUREMENT_BUCK_AT_3V9) return 3;
	else {
			return 4;
	}
	
}

