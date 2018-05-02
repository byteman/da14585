/**
 ****************************************************************************************
 *
 * @file wsss.h
 *
 * @brief Header file - Weight Scale Service Server.
 *
 * Copyright (C) 2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef WSSS_H_
#define WSSS_H_
/**
 ************************************************************************************
 * @addtogroup WSSS Weight Scale Service Server
 * @ingroup WSS
 * @brief Weight Scale Service Server
 * @{
 ************************************************************************************
 */

/*
 * INCLUDE FILES
 ************************************************************************************
 */
#if (BLE_WSS_SERVER)
#include "wss_common.h"
#include "wsss_task.h"
#include "atts.h"
#include "prf_types.h"
#include "prf.h"
/*
 * DEFINES
 ********************************************************************************
 */

#define WSSS_IDX_MAX        (1)

/// Maximum length of Measurement Characteristic value
#define WSS_MEAS_VAL_MAX_LEN               (15)

/*
 * MACROS
 ********************************************************************************
 */

#define WSSS_IS_ENABLED(mask) \
    (((wsss_env->ind_cfg[conidx] & mask) == mask))

/*
 * ENUMERATIONS
 ********************************************************************************
 */

///Attributes State Machine
enum
{
    WSS_IDX_SVC,
    WSS_IDX_INCL_SVC,

    WSS_IDX_FEATURE_CHAR,
    WSS_IDX_FEATURE_VAL,

    WSS_IDX_WT_MEAS_CHAR,
    WSS_IDX_WT_MEAS_VAL,
    WSS_IDX_WT_MEAS_IND_CFG,

    WSS_IDX_NB,
};

///Attribute Table Indexes
enum
{
    WSS_FEATURE_CHAR,
    WSS_MEASUREMENT_CHAR,

    WSS_CHAR_MAX,
};


/// Indication/notification configuration (put in feature flag to optimize memory usage)
enum
{
    /// Bit used to know if weight measurement indication is enabled
    WSSS_WT_MEAS_IND_CFG            = 0x02,
};

/*
 * TYPE DEFINITIONS
 ********************************************************************************
 */

///Weight Scale Service Server Environment Variable
struct wsss_env_tag
{
    /// Connection Info
    prf_env_t prf_env;

    /// Service Start HandleVAL
    uint16_t shdl;

    ///Database Configuration
    uint8_t features;

    ///Weight Scale Features
    uint32_t wss_features;

    ///Event (notification/indication) config
    uint8_t ind_cfg[BLE_CONNECTION_MAX];

    /// State of different task instances
    ke_state_t state[WSSS_IDX_MAX];

    /// Attribute Table
    uint8_t att_tbl[WSS_IDX_NB];
};

/*
 * FUNCTION DECLARATIONS
 ********************************************************************************
 */


/**
 ****************************************************************************************
 * @brief Retrieve WSS service profile interface
 *
 * @return WSS service profile interface
 ****************************************************************************************
 */
const struct prf_task_cbs* wsss_prf_itf_get(void);

/**
 ********************************************************************************
 * @brief Send a WSSS_MEAS_SEND_CFM message to the application.
 * @param status Confirmation Status
 ********************************************************************************
 */
void wsss_meas_send_cfm_send(uint8_t conidx, uint8_t status);

/**
 ********************************************************************************
 * @brief Pack Weight measurement value
 * @param p_meas_val Weight measurement value struct
 ********************************************************************************
 */
void wsss_pack_meas_value(uint8_t *packed_ws, const struct wss_wt_meas* pmeas_val);

/**
 ********************************************************************************
 * @brief Calculate Weight measurement size
 * @param p_meas_val Weight measurement value struct
 ********************************************************************************
 */
uint8_t wsss_calc_meas_value_size(uint8_t flags);

/**
 ********************************************************************************
 * @brief Disable actions grouped in getting back to IDLE and sending configuration to requester task
 ********************************************************************************
 */
void wsss_disable(uint16_t conhdl);
#endif //BLE_WSS_SERVER
/// @} WSSS
#endif // WSSS_H_
