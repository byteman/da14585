#ifndef CHANNEL_H
#define CHANNEL_H

#include "common.h"
#include "state.h"

typedef struct {
    uint8_t nr; //通道号
    uint8_t ready; //是否转换完成
    int32_t value; //值.
    adc_err_state_t state; //当前的芯片状态.
}ad_channel_t;


typedef enum {
    CHAN_ERR_NONE=0, //成功
    CHAN_ERR_BUSY,//转换未成功
}ad_channel_err_t;
/**
 * @brief 初始化通道
 * @param 通道数目
 * @return
 */
int8_t  channel_init(uint8_t nr);

/**
 * @brief 读取出某个通道的信息
 * @param 指定的通道
 * @param 通道信息
 * @return 0 成功 其他失败
 */
int8_t  channel_read(uint8_t chan,ad_channel_t* info);

/**
 * @brief 获取某路通道最后一次的数据.
 * @param chan 通道编号
 * @return 通道信息.
 */
ad_channel_t* channel_get(uint8_t chan);

/**
 * @brief 读取全部通道的AD
 * @return 0 完成 1 还未转换完成.
 */

uint8_t channel_read_all();



#endif
