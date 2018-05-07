
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

		creep_init(g_param->mCrpValue);
		Std_ReturnType rtn = Wet_Init();

}

void scaler_run()
{

    if(!channel_read_all()){
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
				
				Wet_Process(g_sinfo.filter_ad ,20.0f);
    }
}

int			scaler_get_history_record(float* values, uint8 num)
{
	static float nv = 443;
	values[0] = nv+1;
	values[1] = nv+2;
	values[2] = nv+3;
	nv += 3;
	if(nv > 998) nv = 100;
	
	return 3;
	
}

void  scaler_get_display_weight(char* buffer, uint8 size)
{
	snprintf(buffer,size,"%0.1f",12.7);
}