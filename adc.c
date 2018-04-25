#include "adc.h"
#include "platform.h"

static adc_config_t adc_configs[MAX_CHAN_NR];


adc_err_t adc_read(uint8_t chan, int32_t *value)
{
    *value = 1000;
    return ADC_ERR_NONE;
}


adc_err_t adc_config(uint8_t chan, adc_config_t *config)
{
   // if(config->type)
}


adc_err_t adc_ready(uint8_t chan)
{
    return ADC_ERR_NONE;
}
