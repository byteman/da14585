#include "batty.h"
//#include "battery.h"
#include <stdlib.h>
#define ADC_CHANNEL_P01         (1)

static inline uint16_t battery_filter_value (uint16_t new_adc_value)
{

     return(new_adc_value);
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
#define BATTERY_MEASUREMENT_BUCK_AT_1V8     (0x370)
#define BATTERY_MEASUREMENT_BUCK_AT_2V0     (0x3F2)
#define BATTERY_MEASUREMENT_BUCK_AT_2V4     (0x4F6)
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
*/
uint8  battery_get()
{
	uint8 level = 0;
	uint32 value = battery_get_lvl2(0);
	
	if(value < BATTERY_MEASUREMENT_BUCK_AT_1V8) return 0;
	else if(value < BATTERY_MEASUREMENT_BUCK_AT_2V0) return 1;
	else if(value < BATTERY_MEASUREMENT_BUCK_AT_2V4) return 2;
	else if(value < BATTERY_MEASUREMENT_BUCK_AT_2V8) return 3;
	else {
			return 4;
	}
	
}

