/**
 ****************************************************************************************
 *
 * @file app_findme.h
 *
 * @brief Findme Target/Locator application.
 *
 * Copyright (C) 2012-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef _APP_FINDME_H_
#define _APP_FINDME_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief Findme Target/Locator Application entry point.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdbool.h>
#include "user_profiles_config.h"

#if (BLE_FINDME_TARGET)
#include "gpio.h"
#include "findt_task.h"
#endif

#if (BLE_FINDME_LOCATOR)
#include "findl_task.h"
#endif

/*
 * VARIABLES DECLARATION
 ****************************************************************************************
 */

#if (BLE_FINDME_TARGET) || (BLE_FINDME_LOCATOR)
struct app_findt_cb
{
#if (BLE_FINDME_TARGET)
    void (*on_findt_alert_ind)(uint8_t conidx, uint8_t alert_lvl);
#endif

#if (BLE_FINDME_LOCATOR)
    void (*on_findl_enable_rsp)(struct findl_enable_rsp const *param);
    void (*on_findl_set_alert_rsp)(uint8_t status);
#endif
};
#endif //(BLE_FINDME_TARGET) || (BLE_FINDME_LOCATOR)

#if (BLE_FINDME_TARGET)

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct
{
    uint32_t blink_timeout;
    uint8_t blink_toggle;
    uint8_t lvl;
    GPIO_PORT port;
    GPIO_PIN pin;
} app_alert_state;

extern app_alert_state alert_state;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize Findme Target profile.
 * @return void
 ****************************************************************************************
 */
void app_findt_init(void);

/**
 ****************************************************************************************
 * @brief Create the Findme Target task.
 * @return void
 ****************************************************************************************
 */
void app_findt_create_db(void);

/**
 ****************************************************************************************
 * @brief Start the FindMe Target alert logic.
 * @param[in] lvl       The alert level. The valid values are:
 *                          FINDL_ALERT_NONE
 *                          FINDL_ALERT_MILD
 *                          FINDL_ALERT_HIGH
 * @return void
 ****************************************************************************************
 */
void app_findt_alert_start(uint8_t lvl);

/**
 ****************************************************************************************
 * @brief Stop the FindMe Target alert logic.
 * @return void
 ****************************************************************************************
 */
void app_findt_alert_stop(void);

/**
 ****************************************************************************************
 * @brief Default Findme Target alert indication handler.
 * @param[in] conidx            The connection id number
 * @param[in] lvl               The alert level
 * @return void
 ****************************************************************************************
 */
void default_findt_alert_ind_handler(uint8_t conidx, uint8_t lvl);
#endif // BLE_FINDME_TARGET

#if (BLE_FINDME_LOCATOR)
/**
 ****************************************************************************************
 * @brief Initialize Findme Locator profile.
 * @return void
 ****************************************************************************************
 */
void app_findl_init(void);

/**
 ****************************************************************************************
 * @brief Create the Findme Locator task.
 * @return void
 ****************************************************************************************
 */
void app_findl_create_task(void);

/**
 ****************************************************************************************
 * @brief Enable Findme Locator profile.
 * @param[in] conidx            The connection id number
 * @return void
 ****************************************************************************************
 */
void app_findl_enable(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Set alert level.
 * @param[in] lvl       The alert level. The valid values are:
 *                          FINDL_ALERT_NONE
 *                          FINDL_ALERT_MILD
 *                          FINDL_ALERT_HIGH
 * @return void
 ****************************************************************************************
 */
void app_findl_set_alert(uint8_t lvl);
#endif // BLE_FINDME_LOCATOR

/// @} APP

#endif // _APP_FINDME_H_
