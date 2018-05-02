/**
 ****************************************************************************************
 *
 * @file app_proxr_task.c
 *
 * @brief Proximity Reporter application Task Implementation.
 *
 * Copyright (C) 2012-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APPTASK
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwble_config.h"              // SW configuration

#if (BLE_APP_PRESENT)

#if (BLE_PROX_REPORTER)

#include "app_proxr.h"
#include "app_proxr_task.h"
#include "arch_api.h"
#include "app_entry_point.h"
#include "app.h"
#include "user_callback_config.h"

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Handles proximity alert timer.
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int app_proxr_timer_handler(ke_msg_id_t const msgid,
                                   void const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
    if (alert_state.blink_toggle)
    {
        GPIO_SetInactive(alert_state.port, alert_state.pin);
        alert_state.blink_toggle = 0;
    }
    else
    {
        GPIO_SetActive(alert_state.port, alert_state.pin);
        alert_state.blink_toggle = 1;
    }

    ke_timer_set(APP_PROXR_TIMER, dest_id, alert_state.blink_timeout);

    return (KE_MSG_CONSUMED);
}

static int proxr_alert_ind_handler(ke_msg_id_t const msgid,
                                   struct proxr_alert_ind const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
    CALLBACK_ARGS_3(user_app_proxr_cb.on_proxr_alert_ind, param->conidx, param->alert_lvl, param->char_code)

    return (KE_MSG_CONSUMED);
}

/*
 * LOCAL VARIABLES DEFINITION
 ****************************************************************************************
 */

static const struct ke_msg_handler app_proxr_process_handlers[] =
{
    {APP_PROXR_TIMER,                       (ke_msg_func_t)app_proxr_timer_handler},
    {PROXR_ALERT_IND,                       (ke_msg_func_t)proxr_alert_ind_handler},
};

/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

enum process_event_response app_proxr_process_handler(ke_msg_id_t const msgid,
                                                      void const *param,
                                                      ke_task_id_t const dest_id,
                                                      ke_task_id_t const src_id,
                                                      enum ke_msg_status_tag *msg_ret)
{
    return app_std_process_event(msgid, param, src_id, dest_id, msg_ret,
                                 app_proxr_process_handlers,
                                 sizeof(app_proxr_process_handlers) / sizeof(struct ke_msg_handler));
}

#endif //BLE_PROX_REPORTER

#endif //(BLE_APP_PRESENT)

/// @} APPTASK
