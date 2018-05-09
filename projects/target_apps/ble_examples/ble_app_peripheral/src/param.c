#include "param.h"
#include "i2c_eeprom.h"
#include "user_peripheral.h"
#include "user_periph_setup.h"
static device_param g_param;
static uint8_t g_init =0;


static void _enter(void)
{
		init_eeprom_pin();
    i2c_eeprom_init(I2C_SLAVE_ADDRESS, I2C_SPEED_MODE, I2C_ADDRESS_MODE, I2C_ADDRESS_SIZE);
	
}
static void _leave(void)
{
		i2c_eeprom_release();
		init_oled_pin();
}

/**
 ****************************************************************************************
 * @brief Test write byte.
 * @param[in] address Memory position to write the byte to
 * @param[in] byte    Byte to be written
 * @return void
 ****************************************************************************************
 */
static i2c_error_code _write_byte(uint32_t address, uint8_t byte)
{
    _enter();
    i2c_error_code code = i2c_eeprom_write_byte(address, byte);  
		_leave();
		return code;
}

/**
 ****************************************************************************************
 * @brief Test read byte.
 * @param[in] address  Memory address to read the byte from
 * @param[in|out] byte Pointer to the read byte
 * @return void
 ****************************************************************************************
 */
static i2c_error_code _read_byte(uint32_t address, uint8_t *byte)
{
	
	   _enter();
    i2c_error_code code = i2c_eeprom_read_byte(address, byte);  
		_leave();
		return code;

}
/**
 ****************************************************************************************
 * @brief Test write data.
 * @param[in] data              Pointer to the array of bytes to be written
 * @param[in] address           Starting address of the write process
 * @param[in] size              Size of the data to be written
 * @param[in|out] bytes_written Bytes that were actually written (due to memory size limitation)
 * @return void
 ****************************************************************************************
 */
static i2c_error_code _write_data(uint8_t *data, uint32_t address, uint32_t size, uint32_t *bytes_written)
{
		 _enter();
    i2c_error_code code = i2c_eeprom_write_data(data, address, size, bytes_written);
		_leave();
		return code;
}
/**
 ****************************************************************************************
 * @brief Test read data.
 * @param[in] data           Pointer to the array of bytes to be read
 * @param[in] address        Starting memory address
 * @param[in] size           Size of the data to be read
 * @param[in|out] bytes_read Bytes that were actually read (due to memory size limitation)
 * @return void
 ****************************************************************************************
 */
static i2c_error_code _read_data(uint8_t *data, uint32_t address, uint32_t size, uint32_t *bytes_read)
{
	
	 _enter();
    i2c_error_code code = i2c_eeprom_read_data(data, address, size, bytes_read);
   _leave();
		return code;
}

uint8_t byte;
INT8U checksum(INT8U *str,INT32U len)
{
	INT32U i;
	INT8U chksum = 0xAA;

	for(i=0; i<len; i++)
	{	
		chksum += *str;
		str++;
	}
	return chksum;	
}
param_err_t param_init(void)
{
    int i = 0;
		uint32_t bytes_read = 0;
		i2c_error_code code = _read_data((uint8_t*)&g_param, 0, sizeof(g_param), &bytes_read);
		if(code != I2C_NO_ERROR)
		{
				return PARA_ERR_READ_TIMEOUT;
		}
		
		if(*(g_param.checksum) != checksum((uint8_t*)&g_param,sizeof(g_param)-1))
		return TRUE;
	else
		return FALSE;
	
    // Initialize I2C interface for the I2C EEPROM
		//_read_data
		_write_byte(24, 0x5B);

    // Random Read Byte
    _read_byte(24, &byte);
	
    g_param.sensor_nb = 3;
    for(i = 0; i < g_param.sensor_nb; i++)
    {
        g_param.corn_k[i] = 1.0;
    }
		g_init = 1;
		
		return PARA_ERR_NONE;
}
param_err_t param_save(void)
{
		byte = 0;
		_write_byte(24, 0x5A);

    // Random Read Byte
    _read_byte(24, &byte);
}

param_err_t param_get(device_param **para)
{
    if(!g_init) return PARA_ERR_UNINIT;
    if(para == 0) return PARA_ERR_INVALID_PARA;
    *para = &g_param;
    return PARA_ERR_NONE;
}
