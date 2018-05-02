/**
 ****************************************************************************************
 *
 * @file bmss.h
 *
 * @brief Header file - Bond Management Service Client Declaration.
 *
 * Copyright (C) 2015-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef BMSC_H_
#define BMSC_H_

/**
 ****************************************************************************************
 * @addtogroup BMSC Bond Management 'Profile' Client
 * @ingroup BMSC
 * @brief Bond Management 'Profile' Client
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"

#if (BLE_BMS_CLIENT)

#include "ke_task.h"
#include "prf_types.h"
#include "prf_utils.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/// Maximum number of Proximity Monitor task instances
#define BMSC_IDX_MAX       (BLE_CONNECTION_MAX)

/// Pointer to the connection clean-up function
#define BMSC_CLEANUP_FNCT   (NULL)

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/// Characteristics
enum
{
    /// Bond Management Control Point
    BMSC_CHAR_CP,
    /// Bond Management Feature
    BMSC_CHAR_FEATURE,
    BMSC_CHAR_MAX,
};

enum
{
    /// Bond Management Control Point
    BMSC_DESC_RELIABLE_WRITE,
    BMSC_DESC_MAX,
};

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

///Structure containing the characteristics handles, value handles and descriptors
struct bms_content
{
    /// service info
    struct prf_svc svc;
    /// characteristic info:
    /// - Bond Management Control Point
    /// - Bond Management Feature
    struct prf_char_inf chars[BMSC_CHAR_MAX];
    /// descriptors:
    /// - Extended Properties - Reliable Write
    struct prf_char_desc_inf descs[BMSC_DESC_MAX];
};


/// Environment variable for each Connections
struct bmsc_cnx_env
{
    /// Last requested UUID(to keep track of the two services and char)
    uint16_t last_uuid_req;
    /// Last Char. for which a read request has been sent
    uint8_t last_char_code;
    /// counter used to check service uniqueness
    uint8_t nb_svc;
    /// BMS characteristics
    struct bms_content bms;
};

/// Battery 'Profile' Client environment variable
struct bmsc_env_tag
{
    /// profile environment
    prf_env_t prf_env;
    /// Environment variable pointer for each connections
    struct bmsc_cnx_env* env[BMSC_IDX_MAX];
    /// State of different task instances
    ke_state_t state[BMSC_IDX_MAX];
};

/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @brief Retrieve BMS client profile interface
 *
 * @return BMS client profile interface
 ****************************************************************************************
 */
const struct prf_task_cbs* bmsc_prf_itf_get(void);

/**
 ****************************************************************************************
 * @brief Send enable confirmation to application.
 * @param status Status of the enable: either OK or error encountered while discovery.
 ****************************************************************************************
 */
void bmsc_enable_rsp_send(struct bmsc_env_tag *bmsc_env, uint8_t conidx, uint8_t status);

/**
 ****************************************************************************************
 * @brief Check if client request is possible or not
 * @param[in] bmsc_env Pointer to BMSC environment variables structure
 * @param[in] conidx Connection index.
 * @param[in] char_code Characteristic code.
 * @return PRF_ERR_OK if request can be performed, error code else.
 ****************************************************************************************
 */
uint8_t bmsc_validate_request(struct bmsc_env_tag *bmsc_env, uint8_t conidx, uint8_t char_code);

/**
 ****************************************************************************************
 * @brief Send error indication from profile to Host, with proprietary status codes.
 * @param[in] bmsc_env Pointer to BMSC environment variables structure
 * @param[in] conidx Connection index.
 * @param[in] status Status code of error
 * @return void
 ****************************************************************************************
 */
void bmsc_error_ind_send(struct bmsc_env_tag *bmsc_env, uint8_t conidx, uint8_t status);

#endif // (BLE_BMS_CLIENT)

/// @} BMSC

#endif // BMSC_H_
