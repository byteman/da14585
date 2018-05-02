#ifndef STATE_H
#define STATE_H

#include "common.h"

typedef struct _adc_err_state_t{
    int32_t	NoExcBit:1;			//没有激励电压
    int32_t	OpenBridgeBit:1;	//桥路开路
    int32_t	NoAdcBit:1;			// 1 秒钟没检查到转换完成
}adc_err_state_t;
typedef struct _state{
    uint8_t err;
}device_state;
#endif
