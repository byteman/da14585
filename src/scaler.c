
#include "param.h"
#include "adc.h"
#include "channel.h"
#include "scaler.h"
#include "Filter_KMedian.h"
static device_param* g_param;
static scaler_info_t g_sinfo;
void scaler_init()
{
    if(PARA_ERR_NONE != param_get(&g_param))
    {
        return;
    }


}


void scaler_run()
{

    if(channel_read_all()){
        int i = 0;
        int32_t ad = 0;
        for(; i < g_param->sensor_nb; i++)
        {
            ad_channel_t *chan = channel_get(i);
            ad += g_param->corn_k[i] *chan->value ;
        }
        g_sinfo.org_ad = ad;
        //中值滤波后加上零点跟踪
        g_sinfo.filter_ad = creep_ad_deal(FKM_Filter(ad),1);

        if((g_sinfo.filter_ad > 8000000)||(g_sinfo.filter_ad < -8000000))		//超出范围
              g_sinfo.adc_soc_err = 1;
        else
              g_sinfo.adc_soc_err = 0;



    }
}
