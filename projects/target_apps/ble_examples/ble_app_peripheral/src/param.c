#include "param.h"
#include "i2c_eeprom.h"
static device_param g_param;
static uint8_t g_init =0;
param_err_t param_init(void)
{
    int i = 0;
	
		i2c_eeprom_init(I2C_SLAVE_ADDRESS, I2C_SPEED_MODE, I2C_ADDRESS_MODE, I2C_ADDRESS_SIZE);

    g_param.sensor_nb = 3;
    for(i = 0; i < g_param.sensor_nb; i++)
    {
        g_param.corn_k[i] = 1.0;
    }
		g_init = 1;
		return PARA_ERR_NONE;
}


param_err_t param_get(device_param **para)
{
    if(!g_init) return PARA_ERR_UNINIT;
    if(para == 0) return PARA_ERR_INVALID_PARA;
    *para = &g_param;
    return PARA_ERR_NONE;
}
