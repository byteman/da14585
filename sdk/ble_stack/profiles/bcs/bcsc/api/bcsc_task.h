/**
 ****************************************************************************************
 *
 * @file bcsc_task.c
 *
 * @brief Header file - Body Composition Service Client Task Declaration.
 *
 * Copyright (C) 2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef BCSC_TASK_H_
#define BCSC_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup BCSCTASK Center Client Task
 * @ingroup BCSC
 * @brief Body Composition Service Client Task
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"
#if (BLE_BCS_CLIENT)

#include "ke_task.h"
#include "gattc_task.h"
#include "bcsc.h"
#include "bcs_common.h"
#include "prf_types.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/// Possible states of the BCSC task
enum bcsc_state
{
    /// Free state
    BCSC_FREE,
    /// IDLE state
    BCSC_IDLE,
    /// Discovering Body Composition Service
    BCSC_DISCOVERING,
    /// Busy state
    BCSC_BUSY,
    /// Number of defined states
    BCSC_STATE_MAX
};

enum
{
    /// Start the Body Composition Service - at connection
    BCSC_ENABLE_REQ = KE_FIRST_MSG(TASK_ID_BCSC),
    /// Confirm that cfg connection has finished with discovery results, or that normal cnx started
    BCSC_ENABLE_RSP,
    /// Generic message to read BCS characteristics
    BCSC_RD_CHAR_REQ,
    /// Generic message for read response
    BCSC_RD_CHAR_RSP,
    /// Message for configuration
    BCSC_BC_MEAS_CFG_IND_REQ,
    /// Configuration response
    BCSC_BC_MEAS_CFG_IND_RSP,
    /// Body Composition value send to APP
    BCSC_BC_MEAS_IND,
};

/// Codes for reading BCS characteristics
enum
{
    // Read Body Compostion Feature
    BCSC_RD_BC_FEAT,
    // Read Body Composition Measurment Client Cfg Descriptor
    BCSC_RD_BC_MEAS_CLI_CFG
};

/// Parameters of the @ref BCSC_ENABLE_REQ message
struct bcsc_enable_req
{
    /// Connection type
    uint8_t con_type;
    /// BCS handle values and characteristic properties
    struct bcs_content bcs;
};

/// Parameters of the @ref BCSC_ENABLE_CFM message
struct bcsc_enable_rsp
{
    /// Connection type
    uint8_t status;
    /// BCS handle values and characteristic properties
    struct bcs_content bcs;
};

/// Parameters of the @ref BCSC_RD_CHAR_REQ message
struct bcsc_rd_char_req
{
    /// Characteristic value code
    uint8_t char_code;
};

/// Parameters of the @ref BCSC_RD_CHAR_RSP message
struct bcsc_rd_char_rsp
{
    /// Requested Characteristic value code
    uint8_t char_code;
    /// Operation status
    uint8_t status;
    /// Value lenght
    uint8_t lenght;
    /// Read value
    uint8_t value[__ARRAY_EMPTY];
};

/// Parameters of the @ref BCSC_BC_MEAS_CFG_IND_REQ message
struct bcsc_bc_meas_cfg_ind_req
{
    /// Stop/notify/indicate value to configure into the peer characteristic
    uint16_t cfg_val;
};

/// Parameters of the @ref BCSC_BC_MEAS_CFG_IND_RSP message
struct bcsc_bc_meas_cfg_ind_rsp
{
    /// Status
    uint8_t status;
};

/// Parameters of the @ref BCSC_BC_MEAS_IND message
struct bcsc_bc_meas_ind
{
    uint16_t flags;
    /// Body Composition measurment value
    bcs_meas_t meas_val;
};

/*
 * TASK DESCRIPTOR DECLARATIONS
 ****************************************************************************************
 */

extern const struct ke_state_handler bcsc_default_handler;
extern ke_state_t bcsc_state[BCSC_IDX_MAX];

#endif // (BLE_BCS_CLIENT)

/// @} BCSCTASK

#endif // BCSC_TASK_H_
