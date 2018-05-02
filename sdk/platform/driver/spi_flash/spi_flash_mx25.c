/**
 ****************************************************************************************
 *
 * @file spi_flash.c
 *
 * @brief flash memory driver extension for Macronix devices file.
 *
 * Copyright (C) 2016-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#include "spi_flash.h"
#include "spi_flash_mx25.h"

/**
 ****************************************************************************************
 * @brief MX25R Read Configuration Register
 * @return content of the configuration register
 ****************************************************************************************
 */
uint32_t spi_flash_MX25R_read_config_reg(void)
{
    uint32_t tmpData;

    spi_set_bitmode(SPI_MODE_32BIT);                          // set SPI bitmode to 32-bit
    tmpData =  spi_transaction((uint32_t)(MX25R_READ_CONFIG_REG<<24));
    //recieved data contains Config reg1 + Config reg2 + Config reg 1 due to 32bit spi transaction
    return (tmpData >> 8) & 0x0000FFFF ;
}

/**
 ****************************************************************************************
 * @brief MX25R Write Status & Config register
 * @param[in] dataToWrite:   Value to be written to Status and Configuration Register
 * @return error code or success (ERR_OK)
 ****************************************************************************************
 */
int32_t spi_flash_MX25R_write_status_config_reg(uint32_t dataToWrite)
{
    int8_t spi_flash_status;

    spi_flash_status = spi_flash_wait_till_ready();
    if (spi_flash_status != ERR_OK)
        return spi_flash_status; // an error has occured

    dataToWrite &= 0x00ffffff; // MSB must be 0

    spi_flash_set_write_enable();

    spi_set_bitmode(SPI_MODE_32BIT);
    spi_transaction((WRITE_STATUS_REG << 24) | dataToWrite);     // send  Write Status Register-1 instruction
    return spi_flash_wait_till_ready();
}

/**
 ****************************************************************************************
 * @brief MX25R Get Power Mode
 * @return current power mode
 ****************************************************************************************
 */
int8_t spi_flash_MX25R_get_power_mode(void)
{
    return spi_flash_MX25R_read_config_reg();
}

/**
 ****************************************************************************************
 * @brief MX25R Set Power Mode
 * @param[in] powerMode: Low Power Mode (MX25R_LPM) or High Performance Mode (MX25R_HPM)
 * @return error code or success (ERR_OK)
 ****************************************************************************************
 */
int32_t spi_flash_MX25R_set_power_mode(int8_t powerMode)
{
    int8_t spi_flash_status;
    int32_t tmp_data;

    if (spi_flash_MX25R_get_power_mode() == powerMode)
        return ERR_OK;

    //read out status and config reg to avoid changing any settings
    spi_flash_status = spi_flash_read_status_reg();
    tmp_data = spi_flash_MX25R_read_config_reg();
    tmp_data |= (uint32_t) (spi_flash_status << 16); //add the status reg data

    tmp_data &= ~0xff;
    tmp_data |= powerMode;

    return spi_flash_MX25R_write_status_config_reg(tmp_data);
}

/**
 ****************************************************************************************
 * @brief MX25R Read Security Register
 * @return content of the Security Register
 ****************************************************************************************
 */
int8_t spi_flash_MX25R_read_security_reg(void)
{
    spi_set_bitmode(SPI_MODE_16BIT);   // set SPI bitmode to 16-bit
    return spi_transaction((uint32_t)(MX25R_READ_SECURITY_REG << 8));
}

/**
 ****************************************************************************************
 * @brief MX25R Check P_fail bit
 * @return 1 if last page program failed, 0 otherwise
 ****************************************************************************************
 */
int8_t spi_flash_MX25R_program_fail(void)
{
    return ((spi_flash_MX25R_read_security_reg() >> MX25R_SECURITY_REG_P_FAIL) & 0x01);
}

/**
 ****************************************************************************************
 * @brief MX25R Check E_fail bit
 * @return 1 if last erase failed, 0 otherwise
 ****************************************************************************************
 */
int8_t spi_flash_MX25R_erase_fail(void)
{
    return ((spi_flash_MX25R_read_security_reg() >> MX25R_SECURITY_REG_E_FAIL) & 0x01);
}
