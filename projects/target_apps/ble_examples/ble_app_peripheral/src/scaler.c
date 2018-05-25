
#include "param.h"
#include "adc.h"
#include "channel.h"
#include "scaler.h"
#include "Filter_KMedian.h"
#include "WetApp.h"
static device_param* g_param;
static scaler_info_t g_sinfo;
static uint8_t g_addr[32] = {0,};
static uint8_t default_addr[6] = CFG_NVDS_TAG_BD_ADDRESS;
void		scaler_set_ble_addr(char* addr)
{
	memset(g_addr,0,32);
	strcpy(g_addr,addr);
}
const char* scaler_get_ble_addr(void)
{
	if(g_addr[0] == 0){
		
		snprintf(g_addr,32,
							"%02X:%02X:%02X:%02X:%02X:%02X",
							default_addr[5],
							default_addr[4],
							default_addr[3],
							default_addr[2],
							default_addr[1],
							default_addr[0]
		);
		
	}
	return g_addr;
}
void scaler_init()
{
    if(PARA_ERR_NONE != param_get(&g_param))
    {
        return;
    }
		WetApp_Init();
		creep_init(g_param->mCrpValue);
		Std_ReturnType rtn = Wet_Init();

}

void scaler_run()
{

    if(!channel_read_all()){
        int i = 0;
        int32_t ad = 0;
        for(; i < MAX_CHAN; i++)
        {
            ad_channel_t *chan = channel_get(i);
						g_sinfo.adArrs[i]  = chan->value;
            ad += g_param->corn_k[i] * chan->value ;
        }
        g_sinfo.org_ad = ad;
        //中值滤波后加上零点跟踪
        g_sinfo.filter_ad = creep_ad_deal(FKM_Filter(ad),1);

        if((g_sinfo.filter_ad > 8000000)||(g_sinfo.filter_ad < -8000000))		//超出范围
              g_sinfo.adc_soc_err = 1;
        else
              g_sinfo.adc_soc_err = 0;
				Wet_Process(g_sinfo.filter_ad ,20.0f);
				Wet_Service();		
				
				
		//
    }
}

scaler_info_t* 		scaler_get_info(void)
{
		g_sinfo.stillFlag = gScaleAppData.mScaleStatue.stStable;
		g_sinfo.zeroFlag = gScaleAppData.mScaleStatue.stZero;
		g_sinfo.downFlow = gScaleAppData.mScaleStatue.stDownOverLoad;
		g_sinfo.upFlow = gScaleAppData.mScaleStatue.stUpOverLoad;
		g_sinfo.adc_soc_err = 0;
		g_sinfo.div_weight = gScaleAppData.mScaleWet;
		g_sinfo.org_weight = gScaleAppData.mScaleWetInter;
		g_sinfo.ready = gScaleAppData.mScaleStatue.stReady;
		return &g_sinfo;
	
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
	snprintf(buffer,size,"%0.1f",gScaleAppData.mScaleWet);
}

uint8		scaler_set_zero()
{
		WetApp_SetZero();
		return 0;
}
void		scaler_reset_history(void)
{
		
}