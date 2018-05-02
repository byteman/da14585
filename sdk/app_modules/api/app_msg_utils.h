/**
 ****************************************************************************************
 *
 * @file app_msg_utils.h
 *
 * @brief Complementary API to handle user defined messages.
 *
 * Copyright (C) 2015-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef _APP_MSG_UTILS_H_
#define _APP_MSG_UTILS_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup 
 *
 * @brief
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "ke_msg.h"

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Check if the BLE is active or not.
 * @return Returns true if BLE is active.
 ****************************************************************************************
 */
bool app_check_BLE_active(void);

/**
 ****************************************************************************************
 * @brief Sends a message and wakes BLE up if not active.
 * @param[in] cmd Pointer to the message.
 * @return void
 ****************************************************************************************
 */
void app_msg_send_wakeup_ble(void *cmd);

/**
 ****************************************************************************************
 * @brief Wrapper to the kernel send message function.
 * @param[in] cmd Pointer to the message.
 * @return void
 ****************************************************************************************
 */
__INLINE void app_msg_send(void *cmd)
{
    ke_msg_send(cmd);
}

/// @} APP

#endif // _APP_MSG_UTILS_H_
