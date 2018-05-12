#ifndef SCALER_H
#define SCALER_H

#include "common.h"
#include "state.h"
#include "channel.h"
#include "types.h"

#define MAX_CHAN 4
typedef struct _scaler_info_t{
    int32_t org_ad; //最近一次计算得到的AD码
    int32_t filter_ad; //经过滤波后的AD
    uint8_t adc_soc_err;//1 ad错误
		FP32	  wet_zero; //零点
		uint8		zeroFlag; 
		uint8		stillFlag;
		uint8		upFlow;
		uint8   downFlow;
		int32_t div_weight; //分度值后的毛重
	  int32_t org_weight; //没有经过分度值处理的重量.	
		int32_t adArrs[MAX_CHAN];
}scaler_info_t;
void    scaler_init();
void    scaler_run();
int			scaler_get_history_record(float* values, uint8 num);
void  	scaler_get_display_weight(char* buffer, uint8 size);
scaler_info_t* 		scaler_get_info(void);
#endif
