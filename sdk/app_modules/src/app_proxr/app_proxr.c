/**
 ****************************************************************************************
 *
 * @file app_proxr.c
 *
 * @brief Proximity Reporter application.
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
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwble_config.h"

#if (BLE_APP_PRESENT)

#if (BLE_PROX_REPORTER)
#include "app_api.h"                // application task definitions
#include "proxr_task.h"              // proximity functions
#include "gpio.h"
#include "wkupct_quadec.h"
#include "app_proxr.h"
#include "app_prf_perm_types.h"
#include "user_periph_setup.h"

//application alert state structure
app_alert_state alert_state __attribute__((section("retention_mem_area0"), zero_init)); //@RETENTION MEMORY

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void app_proxr_init()
{
    alert_state.port = GPIO_ALERT_LED_PORT;
    alert_state.pin = GPIO_ALERT_LED_PIN;
    alert_state.ll_alert_lvl = PROXR_ALERT_HIGH; // Link Loss default Alert Level
    alert_state.txp_lvl = 0x00;                  // TX power level indicator
}

void app_proxr_create_db(void)
{
    struct proxr_db_cfg* db_cfg;

    struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,
                                                             TASK_GAPM,
                                                             TASK_APP,
                                                             gapm_profile_task_add_cmd,
                                                             sizeof(struct proxr_db_cfg));

    // Fill message
    req->operation = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl = get_user_prf_srv_perm(TASK_ID_PROXR);
    req->prf_task_id = TASK_ID_PROXR;
    req->app_task = TASK_APP;
    req->start_hdl = 0;

    // Set parameters
    db_cfg = (struct proxr_db_cfg* ) req->param;
    db_cfg->features = PROXR_IAS_TXPS_SUP;

    // Send the message
    ke_msg_send(req);
}

void app_proxr_alert_start(uint8_t lvl)
{
    alert_state.lvl = lvl;

    if (alert_state.lvl == PROXR_ALERT_MILD)
    {
        alert_state.blink_timeout = 150;
    }
    else
    {
        alert_state.blink_timeout = 50;
    }

    alert_state.blink_toggle = 1;

    GPIO_SetActive( alert_state.port, alert_state.pin);

    ke_timer_set(APP_PROXR_TIMER, TASK_APP, alert_state.blink_timeout);
}

void app_proxr_alert_stop(void)
{
    alert_state.lvl = PROXR_ALERT_NONE;

    alert_state.blink_timeout = 0;
    alert_state.blink_toggle = 0;

    GPIO_SetInactive(alert_state.port, alert_state.pin);

    ke_timer_clear(APP_PROXR_TIMER, TASK_APP);
}

void app_proxr_port_reinit(GPIO_PORT port, GPIO_PIN pin)
{
    alert_state.port = port;
    alert_state.pin = pin;

    if (alert_state.blink_toggle == 1)
    {
        GPIO_SetActive(alert_state.port, alert_state.pin);
    }
    else
    {
        GPIO_SetInactive(alert_state.port, alert_state.pin);
    }
}

/**
 ****************************************************************************************
 * @brief Update proximity application alert.
 * @param[in] lvl     Alert level: PROXR_ALERT_NONE, PROXR_ALERT_MILD, PROXR_ALERT_HIGH
 * @return void
 ****************************************************************************************
 */
static void alert_update(uint8_t alert_lvl)
{
    if (alert_lvl)
    {
        app_proxr_alert_start(alert_lvl);
    }
    else
    {
        app_proxr_alert_stop();
    }
}

void default_proxr_alert_ind_handler(uint8_t conidx, uint8_t alert_lvl, uint8_t char_code)
{
    alert_update(alert_lvl);
}

#endif // BLE_PROX_REPORTER

#endif // BLE_APP_PRESENT

/// @} APP
