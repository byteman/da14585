#include "channel.h"
#include "adc.h"
#include "power.h"
#define MAX_AD_CHAN 8


static ad_channel_t g_ad_chan[MAX_AD_CHAN];
static uint8_t ad_chan_nr = 4;

int8_t channel_init(uint8_t nr)
{
    int i = 0;
    if(nr > MAX_AD_CHAN) return -1;
    ad_chan_nr = 4;
    memset(g_ad_chan,0,sizeof(g_ad_chan));
    for(i = 0; i < nr ; i++)
    {
        g_ad_chan[i].nr = i;

    }

}

uint8_t channel_read_one(uint8_t chan)
{
		
		if(adc_ready(chan)){
            //只要有一路没有准备好就
        return CHAN_ERR_BUSY;
    }
		if(CHAN_ERR_NONE!=channel_read(chan, &g_ad_chan[chan]))
    {
            return CHAN_ERR_BUSY;
    }
		
    return CHAN_ERR_NONE;
}
//如果是待机状态则只读取第一路的ad，其他路的ad任然保留最后一次读取的ad值》
uint8_t channel_read_all(void)
{
    int i = 0;
		if(power_state() == PWR_SLEEP){
				return channel_read_one(1);
		}
    for(i = 0; i < ad_chan_nr; i++)
    {
        if(adc_ready(i)){
            //只要有一路没有准备好就
            return CHAN_ERR_BUSY;
        }

    }
    for(i = 0; i < ad_chan_nr; i++)
    {
        if(CHAN_ERR_NONE!=channel_read(i, &g_ad_chan[i]))
        {
            return CHAN_ERR_BUSY;
        }
				
				
    }

    return CHAN_ERR_NONE;
}

static 	__CHANNEL_VALUE ad_value[CAP_M];  
INT32S			AD_filter[CAP_M];


static INT32S cs1237_filter_ad(int index ,INT32S  cap)
{

	int ch = index;
	int in = 0;
	
	ad_value[ch].m_index +=1;  
	if(ad_value[ch].m_index >= CAP_N ) 
		ad_value[ch].m_index = 0;	
	in = ad_value[ch].m_index;
	
	ad_value[ch].data_sum -= ad_value[ch].m_Value[in];
	ad_value[ch].m_Value[in]= cap;
	ad_value[ch].data_sum += ad_value[ch].m_Value[in] ;
	ad_value[ch].m_ad_err = 0;
	ad_value[ch].m_ad_ready = 1;
	return cap;


}

static int32_t channel_filter(uint8_t chan,int32_t ad)
{
		return cs1237_filter_ad(chan,ad);
}

int8_t channel_read(uint8_t chan, ad_channel_t *info)
{
    int32_t ad = 0;
		//static int32_t old = 0;
    adc_err_t  err = adc_read(chan,info);
    if(err == ADC_ERR_NONE)
    {
				int32_t diff = 0;
        info->nr = chan;
        info->ready = 1;
				diff = abs(info->filter - info->value);
				if(diff > 100){
					diff++;
				}
				
				info->value = channel_filter(chan,info->value);
				info->filter = info->value;
      
        return CHAN_ERR_NONE;
    }
    return CHAN_ERR_BUSY;
}


ad_channel_t *channel_get(uint8_t chan)
{
    return &g_ad_chan[chan];
}
INT32S channel_get_filter_ad(uint8_t chan)
{
		int rt_v = 0;
		rt_v = ad_value[chan].data_sum/CAP_N;
		return rt_v;
}
uint8_t channel_all_power(uint8_t en)
{
		int i = 0;
		for(; i < 4; i++)
		{
				adc_power(i,en);
		}
		return 0;
}

uint8_t channel_power(uint8_t chan, uint8_t en)
{
	return adc_power(chan,en);
}