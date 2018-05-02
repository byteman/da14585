#ifndef SCALER_H
#define SCALER_H

#include "common.h"
#include "state.h"
#include "channel.h"

typedef struct _scaler_info_t{
    int32_t org_ad; //���һ�μ���õ���AD��
    int32_t filter_ad; //�����˲����AD
    uint8_t adc_soc_err;//1 ad����
}scaler_info_t;
void    scaler_init();
void    scaler_run();
#endif
