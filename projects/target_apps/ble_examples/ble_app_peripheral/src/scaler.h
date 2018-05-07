#ifndef SCALER_H
#define SCALER_H

#include "common.h"
#include "state.h"
#include "channel.h"
#include "types.h"

typedef struct _scaler_info_t{
    int32_t org_ad; //最近一次计算得到的AD码
    int32_t filter_ad; //经过滤波后的AD
    uint8_t adc_soc_err;//1 ad错误
		FP32	  wet_zero; //零点
}scaler_info_t;
void    scaler_init();
void    scaler_run();
int			scaler_get_history_record(float* values, uint8 num);
void  	scaler_get_display_weight(char* buffer, uint8 size);
#endif
