/**
 ****************************************************************************************
 *
 * @file app_bmss.h
 *
 * @brief Bond Management Service Server Application header file
 *
 * Copyright (C) 2015-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef APP_BMSS_H_
#define APP_BMSS_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "user_profiles_config.h"

#if BLE_BMS_SERVER

#include "bms_common.h"

/*
 * VARIABLES DECLARATION
 ****************************************************************************************
 */

struct app_bmss_cb
{
    void (*on_bmss_del_bond_req_ind)(uint8_t con_idx,
                                        const struct bms_cntl_point_op *operation);
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initializes BMSS Apllication.
 *
 * @return      void
 ****************************************************************************************
 */
void app_bmss_init(void);

/**
 ****************************************************************************************
 * @brief Creates BMSS service database.
 *
 * @param[in]   void
 *
 * @return      void
 ****************************************************************************************
 */
void app_bmss_create_db(void);

/**
 ****************************************************************************************
 * @brief Sends bond operation status to Bond Management Service.
 * @param[in] status          Operation status
 * @return void
 ****************************************************************************************
 */
void app_bmss_del_bond_cfm(uint8_t conidx, uint8_t status);

#endif //BLE_BMS_SERVER

/// @} APP

#endif // APP_BMSS_H_
