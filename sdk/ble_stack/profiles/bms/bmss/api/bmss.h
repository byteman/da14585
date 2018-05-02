/**
 ****************************************************************************************
 *
 * @file bmss.h
 *
 * @brief Header file - Bond Management Service Server Declaration.
 *
 * Copyright (C) 2015-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef BMSS_H_
#define BMSS_H_

/**
 ****************************************************************************************
 * @addtogroup BMSS Bond Management 'Profile' Server
 * @ingroup BMSS
 * @brief Bond Management 'Profile' Server
 * @{
 ****************************************************************************************
 */

#if (BLE_BMS_SERVER)

#include "prf_types.h"
#include "prf_utils.h"

/*
 * DEFINES
 ****************************************************************************************
 */

#define BMS_CNTL_PT_MAX_LEN (512)

/// Maximum number of Bond Management Server task instances
#define BMSS_IDX_MAX        (1)

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/// Bond Management Service Attribute Table Indices
enum
{
 /// Bond Management Service
 BMS_IDX_SVC,
 /// Bond Management Control Point
 BMS_IDX_BM_CNTL_PT_CHAR,
 BMS_IDX_BM_CNTL_PT_VAL,
 BMS_IDX_BM_CNTL_PT_EXT_PROP,
 /// Bond Management Feature
 BMS_IDX_BM_FEAT_CHAR,
 BMS_IDX_BM_FEAT_VAL,
 /// Number of attributes
 BMS_IDX_NB,
};

/*
 * STRUCTURES
 ****************************************************************************************
 */

/// Bond Management Service environment variable
struct bmss_env_tag
{
    /// Profile environment
    prf_env_t prf_env;
    /// BMS Start Handles
    uint16_t shdl;
    /// Database configuration
    uint32_t features;
    uint8_t reliable_writes;
    /// State of different task instances
    ke_state_t state[BMSS_IDX_MAX];
};

/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/// Full BMS Database Description
extern const struct attm_desc bms_att_db[BMS_IDX_NB];

/// Bond Management Service
extern const att_svc_desc_t bms_svc;

/// Bond Management Service Control Point Characteristic
extern const struct att_char_desc bms_ctrl_pt_char;

/// Bond Management Service Feature Characteristic
extern const struct att_char_desc bms_feat_char;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Retrieve Bond Management service profile interface
 *
 * @return Bond Management service profile interface
 ****************************************************************************************
 */
const struct prf_task_cbs* bmss_prf_itf_get(void);

#endif // (BLE_BMS_SERVER)

/// @} BMSS

#endif // BMSS_H_
