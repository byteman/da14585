/**
 ****************************************************************************************
 *
 * @file syscntl.h
 *
 * @brief DA1458x system controller driver header file.
 *
 * Copyright (C) 2012-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef SYSCNTL_H_
#define SYSCNTL_H_

/*
 * ENUMERATION DEFINITIONS
 *****************************************************************************************
 */

/// Nominal VBAT3V output voltage of the boost converter
enum SYSCNTL_DCDC_VBAT3V_LEVEL
{
    SYSCNTL_DCDC_VBAT3V_LEVEL_2V4    = 4, // 2.4V
    SYSCNTL_DCDC_VBAT3V_LEVEL_2V5    = 5, // 2.5V
    SYSCNTL_DCDC_VBAT3V_LEVEL_2V62   = 6, // 2.62V
    SYSCNTL_DCDC_VBAT3V_LEVEL_2V76   = 7, // 2.76V
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

void syscntl_set_dcdc_vbat3v_level(enum SYSCNTL_DCDC_VBAT3V_LEVEL level);

void syscntl_use_lowest_amba_clocks(void);

void syscntl_use_highest_amba_clocks(void);

#endif // SYSCNTL_H_
