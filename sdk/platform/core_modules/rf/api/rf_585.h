/**
 ****************************************************************************************
 *
 * @file rf_585.h
 *
 * @brief DA14585 RF control Header File.
 *
 * Copyright (C) 2012-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef _RF_585_H_
#define _RF_585_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip.h"               // for RF API structure definition

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 *****************************************************************************************
 * @brief Initialize Radio module.
 * @param[in] api  Stucture of API functions of the RF driver that are used by the BLE
 *                 software.
 * @return void
 *****************************************************************************************
 */
void rf_init_func(struct rwip_rf_api *api);

/**
 *****************************************************************************************
 * @brief Re-initialize Radio module. Called when system exits sleep mode.
 * @return void
 *****************************************************************************************
 */
void rf_reinit_func(void);

/**
 *****************************************************************************************
 * @brief Initialize RF module registers.
 * @return void
 *****************************************************************************************
 */
void rf_regs(void);

/*
 * NEAR FIELD MODE API
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Checks if Near Field mode is enabled.
 * @return true if Near Field mode is enabled, otherwise false.
 ****************************************************************************************
 */
bool rf_nfm_is_enabled(void);

/**
 ****************************************************************************************
 * @brief Enables Near Field mode for all connections.
 * @return void
 ****************************************************************************************
 */
void rf_nfm_enable(void);

/**
 ****************************************************************************************
 * @brief Disables Near Field mode for all connections.
 * @return void
 ****************************************************************************************
 */
void rf_nfm_disable(void);

#endif // _RF_585_H_
