/**
 ****************************************************************************************
 *
 * @file app_easy_msg_utils.h
 *
 * @brief Message related helper functions header file.
 *
 * Copyright (C) 2015-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef _APP_EASY_MSG_UTILS_H_
#define _APP_EASY_MSG_UTILS_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup
 *
 * @brief Message related helper functions header file.
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
 * @brief Clear the callback function for a given message.
 * @param[in] msg_util_id The Id of the message
 * @return Returns the message id if everything was ok
 ****************************************************************************************
 */
ke_msg_id_t app_easy_msg_free_callback(ke_msg_id_t msg_util_id);

/**
 ****************************************************************************************
 * @brief Hook a callback to a free message from a pool of messages.
 * @param[in] fn Pointer to the callback function
 * @return Returns the message id
 ****************************************************************************************
 */
ke_msg_id_t app_easy_msg_set(void (*fn)(void));

/**
 ****************************************************************************************
 * @brief Change the callback for a given message.
 * @param[in] msg_util_id The Id of the message.
 * @param[in] fn          The new pointer to the callback function.
 * @return Returns the message id, zero if no message is available.
 ****************************************************************************************
 */
ke_msg_id_t app_easy_msg_modify(ke_msg_id_t msg_util_id, void (*fn)(void));

/**
 ****************************************************************************************
 * @brief Set the callback to be invoked when the wake-up message issued from the 
 *        app_easy_wakeup function is scheduled.
 * @param[in] fn Pointer to the callback function
 * @return void
 ****************************************************************************************
 */
void app_easy_wakeup_set(void (*fn)(void));

/**
 ****************************************************************************************
 * @brief Remove the callback from the wake-up message handler.
 * @return void
 ****************************************************************************************
 */
void app_easy_wakeup_free(void);

/**
 ****************************************************************************************
 * @brief This function should be called from within the wake-up handler.
 *        It will put a message in the scheduler queue which will be called when the BLE 
 *        stack is fully awake and operationable.
 * @return void
 ****************************************************************************************
 */
void app_easy_wakeup(void);

/**
 ****************************************************************************************
 * @brief Process handler for the Message Utility messages.
 * @param[in] msgid   Id of the message received.
 * @param[in] param   Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id  ID of the sending task instance.
 * @param[in] msg_ret Result of the message handler.
 * @return Returns if the message is handled by the process handler.
 ****************************************************************************************
 */
enum process_event_response app_msg_utils_api_process_handler(ke_msg_id_t const msgid,
                                                              void const *param,
                                                              ke_task_id_t const dest_id,
                                                              ke_task_id_t const src_id, 
                                                              enum ke_msg_status_tag *msg_ret);

 /// @} APP

#endif // _APP_EASY_MSG_UTILS_H_
