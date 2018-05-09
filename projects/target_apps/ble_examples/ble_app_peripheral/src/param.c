#include "param.h"
#include "i2c_eeprom.h"
#include "user_peripheral.h"
#include "user_periph_setup.h"
static device_param g_param;
static uint8_t g_init =0;
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
static void test_write_data(uint8_t *data, uint32_t address, uint32_t size, uint32_t *bytes_written)
{
    //printf_string("\n\rWriting page to EEPROM (values 0x00-FF)...\n\r");
    i2c_error_code code = i2c_eeprom_write_data(data, address, size, bytes_written);
    if (code == I2C_NO_ERROR)
    {
        //printf_string("\n\rPage written.\n\r");
        //printf_string("\n\rBytes written: 0x");
        //printf_byte(((*bytes_written) >> 16) & 0xFF);
        //printf_byte(((*bytes_written) >> 8) & 0xFF);
        //printf_byte((*bytes_written) & 0xFF);
        //printf_string("\n\r");
    }
    else
    {
        //printf_string(" - Test failed! - I2C Error code: 0x");
        //printf_byte(code & 0xFF);
        //printf_string("\n\r");
    }
}

/**
 ****************************************************************************************
 * @brief Test write byte.
 * @param[in] address Memory position to write the byte to
 * @param[in] byte    Byte to be written
 * @return void
 ****************************************************************************************
 */
static void test_write_byte(uint32_t address, uint8_t byte)
{
    //printf_string("\n\rWrite byte (0x");
    //printf_byte(byte & 0xFF);
    //printf_string(") @ address ");
    //printf_byte_dec(address & 0xFF);
    //printf_string(" (zero based)...\n\r");
    i2c_error_code code = i2c_eeprom_write_byte(address, byte);
    if (code == I2C_NO_ERROR)
    {
        //printf_string("\n\rWrite done.\n\r");
    }
    else
    {
        //printf_string(" - Test failed! - I2C Error code: 0x");
        //printf_byte(code & 0xFF);
        //printf_string("\n\r");
    }
}

/**
 ****************************************************************************************
 * @brief Test read byte.
 * @param[in] address  Memory address to read the byte from
 * @param[in|out] byte Pointer to the read byte
 * @return void
 ****************************************************************************************
 */
static void test_read_byte(uint32_t address, uint8_t *byte)
{
    i2c_error_code code = i2c_eeprom_read_byte(address, byte);
    //printf_string("\n\rRead byte @ address ");
    //printf_byte_dec(address & 0xFF);
    //printf_string(": 0x");
    //printf_byte((*byte) & 0xFF);
    //printf_string("\n\r");
    if (code != I2C_NO_ERROR)
    {
        //printf_string(" - Test failed! - I2C Error code: 0x");
        //printf_byte(code & 0xFF);
        //printf_string("\n\r");
    }
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
static void test_read_data(uint8_t *data, uint32_t address, uint32_t size, uint32_t *bytes_read)
{
    i2c_error_code code = i2c_eeprom_read_data(data, address, size, bytes_read);
    if (code == I2C_NO_ERROR)
    {
        //printf_string("\n\r");
        // Display Results
        //for (uint32_t i = 0 ; i < size ; i++)
        //{
         //   printf_byte(data[i]);
        //    printf_string(" ");
        //}
        //printf_string("\n\r\n\rBytes read: 0x");
        //printf_byte(((*bytes_read) >> 16) & 0xFF);
        //printf_byte(((*bytes_read) >> 8) & 0xFF);
        //printf_byte((*bytes_read) & 0xFF);
        //printf_string("\n\r");
    }
    else
    {
        //printf_string(" - Test failed! - I2C Error code: 0x");
        //printf_byte(code & 0xFF);
        //printf_string("\n\r");
    }
}
uint8_t byte;
param_err_t param_init(void)
{
    int i = 0;
	
		
    // Initialize I2C interface for the I2C EEPROM
    i2c_eeprom_init(I2C_SLAVE_ADDRESS, I2C_SPEED_MODE, I2C_ADDRESS_MODE, I2C_ADDRESS_SIZE);
		
		test_write_byte(22, 0x5A);

    // Random Read Byte
    test_read_byte(22, &byte);
	
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
