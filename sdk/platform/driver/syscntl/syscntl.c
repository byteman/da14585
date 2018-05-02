/**
 ****************************************************************************************
 *
 * @file syscntl.c
 *
 * @brief DA1458x system controller driver.
 *
 * Copyright (C) 2012-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#include "syscntl.h"
#include "datasheet.h"


/**
 ****************************************************************************************
 * @brief Sets the Nominal VBAT3V output voltage of the boost converter.
 *
 * @param[in] level DCDC VBAT3V output voltage level (see enum SYSCNTL_DCDC_VBAT3V_LEVEL).
 *
 * @return void
 ****************************************************************************************
 */
void syscntl_set_dcdc_vbat3v_level(enum SYSCNTL_DCDC_VBAT3V_LEVEL level)
{
    SetBits16(DCDC_CTRL2_REG, DCDC_VBAT3V_LEV, level);
}


/**
 ****************************************************************************************
 * @brief Sets AMBA clocks to minimum frequency
 *
 * @return void 
 ****************************************************************************************
 */
void syscntl_use_lowest_amba_clocks(void)
{
    SetBits16(CLK_AMBA_REG, PCLK_DIV, 3);   // lowest is 250KHz (div 8, source is @2MHz)
    SetBits16(CLK_AMBA_REG, HCLK_DIV, 3);   // lowest is 2MHz (div 8, source is @16MHz)
}


/**
 ****************************************************************************************
 * @brief Sets AMBA clocks to maximum frequency
 *
 * @return void 
 ****************************************************************************************
 */
void syscntl_use_highest_amba_clocks(void)
{
    SetBits16(CLK_AMBA_REG, PCLK_DIV, 0);
    SetBits16(CLK_AMBA_REG, HCLK_DIV, 0);
}
