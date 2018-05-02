/**
 ****************************************************************************************
 *
 * @file app_proxr.h
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

#ifndef _APP_PROXR_H_
#define _APP_PROXR_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief Proximity Reporter Application entry point.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#if (BLE_PROX_REPORTER)
#include "gpio.h"
#include "proxr.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct
{
    uint32_t     blink_timeout;
    uint8_t      blink_toggle;
    uint8_t      lvl;
    uint8_t      ll_alert_lvl;
    int8_t       txp_lvl;
    GPIO_PORT    port;
    GPIO_PIN     pin;
} app_alert_state;

/*
 * VARIABLES DECLARATION
 ****************************************************************************************
 */

struct app_proxr_cb
{
    void (*on_proxr_alert_ind)(uint8_t conidx, uint8_t alert_lvl, uint8_t char_code);
};

/*
 * EXTERNAL VARIABLES DECLARATION
 ****************************************************************************************
 */

extern app_alert_state alert_state;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize Proximity Reporter application.
 * @return void
 ****************************************************************************************
 */
void app_proxr_init(void);

/**
 ****************************************************************************************
 * @brief Create proximity reporter Database.
 * @return void
 ****************************************************************************************
 */
void app_proxr_create_db(void);

/**
 ****************************************************************************************
 * @brief Start proximity Alert.
 * @param[in] lvl               Alert level. Mild or High
 * @return void
 ****************************************************************************************
 */
void app_proxr_alert_start(uint8_t lvl);

/**
 ****************************************************************************************
 * @brief Stop proximity Alert.
 * @return void
 ****************************************************************************************
 */
void app_proxr_alert_stop(void);

/**
 ****************************************************************************************
 * @brief Reinit of proximity reporter LED pins and push button. Called by periph_init().
 * param[in] port               GPIO port
 * param[in] pin                GPIO pin
 * @return void
 ****************************************************************************************
 */
void app_proxr_port_reinit(GPIO_PORT port, GPIO_PIN pin);

/**
 ****************************************************************************************
 * @brief Default application handler for handling a request to start an alert on the
 *        device.
 * param[in] conidx             Connection index
 * param[in] alert_lvl          Alert level
 * param[in] char_code          Char Code - Indicate if LLS or IAS
 * @return void
 ****************************************************************************************
 */
void default_proxr_alert_ind_handler(uint8_t conidx, uint8_t alert_lvl, uint8_t char_code);

#endif // (BLE_PROX_REPORTER)

/// @} APP

#endif // _APP_PROXR_H_
