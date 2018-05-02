#ifndef ADC_H
#define ADC_H

#include "common.h"

typedef enum {
    ADC_CHIP_CS1237=0,
}adc_chip_type_t;
typedef enum {
    ADC_ERR_NONE=0,
    ADC_ERR_SOC, //芯片错误.
    ADC_ERR_BRIGE, //桥路断开.
    ADC_ERR_BUSY, //还未转换成功.
}adc_err_t;
typedef struct _adc_config{
    uint8_t hz; //转换速率
    uint8_t type; //adc类型 0 cs1237

}adc_config_t;
//读取某一路通道的AD值
adc_err_t adc_read(uint8_t chan,int32_t *value);
adc_err_t adc_config(uint8_t chan,adc_config_t* config);

/**
 * @brief 某路ad转换是否完成.
 * @return
 */
adc_err_t adc_ready(uint8_t chan);
#endif
