/**
 ****************************************************************************************
 *
 * @file app_bass_task.h
 *
 * @brief Battery server application task header file.
 *
 * Copyright (C) 2012-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef APP_BASS_TASK_H_
#define APP_BASS_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief Battery Service Application.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwble_config.h"

#if (BLE_BATT_SERVER)

#include "bass_task.h"
#include "ke_msg.h"
#include "app_bass.h"

/*
 * EXTERNAL VARIABLES DECLARATION
 ****************************************************************************************
 */

extern uint16_t bat_poll_timeout;
extern uint8_t bat_lvl_alert_used;
extern GPIO_PORT bat_led_port;
extern GPIO_PIN bat_led_pin;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Process handler for the Application BASS messages.
 * @param[in] msgid   Id of the message received
 * @param[in] param   Pointer to the parameters of the message
 * @param[in] dest_id ID of the receiving task instance (probably unused)
 * @param[in] src_id  ID of the sending task instance
 * @param[in] msg_ret Result of the message handler
 * @return Returns if the message is handled by the process handler
 ****************************************************************************************
 */
enum process_event_response app_bass_process_handler(ke_msg_id_t const msgid,
                                                     void const *param,
                                                     ke_task_id_t const dest_id,
                                                     ke_task_id_t const src_id,
                                                     enum ke_msg_status_tag *msg_ret);

#endif // BLE_BATT_SERVER

/// @} APP

#endif // APP_BASS_TASK_H_
