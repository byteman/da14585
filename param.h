#ifndef PARAM_H
#define PARAM_H

#include "common.h"
#define MAX_CORN_NUM 5

typedef enum {
  PARA_ERR_NONE=0,
  PARA_ERR_UNINIT, //未初始化
  PARA_ERR_INVALID_PARA, //传入参数无效
  PARA_ERR_READ_TIMEOUT, //读取超时
  PARA_ERR_WRITE_TIMEOUT, //写入超时
  PARA_ERR_READ_CRC, //校验错误
}param_err_t;
//需要持久化保存的设备参数.
typedef struct _device_param{
    uint8_t sensor_nb; //传感器个数
    float32	corn_k[MAX_CORN_NUM]; //角差系数

}device_param;
//从芯片中读取参数
param_err_t param_load();
//保存参数到芯片中
param_err_t param_save();


/**
 * @brief 获取内存中全局的参数
 * @param 获取到的内存参数
 * @return 成功与否
 */

param_err_t* param_get(device_param** para);

/**
 * @brief 写入参数到内存中.
 * @param 修改指定的参数.
 * @return
 */
param_err_t param_set(device_param* para);


#endif
