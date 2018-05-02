/**
 ****************************************************************************************
 *
 * @file app_prf_types.h
 *
 * @brief app - profiles related header file.
 *
 * Copyright (C) 2012-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef APP_PRF_TYPES_H_
#define APP_PRF_TYPES_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

 /// Format of a profile initialization function
typedef void (*prf_func_void_t)(void);
typedef void (*prf_func_uint8_t)(uint8_t);
typedef uint8_t (*prf_func_validate_t)(uint16_t, uint16_t, uint16_t, uint8_t*);

/// Structure of profile call back function table.
struct prf_func_callbacks
{
    /// Profile Task ID.
    enum KE_API_ID       task_id;
    /// Pointer to the database create function
    prf_func_void_t         db_create_func;
    /// Pointer to the profile enable function
    prf_func_uint8_t       enable_func;
};

/// Structure of custom profile call back function table.
struct cust_prf_func_callbacks
{
    /// Profile Task ID.
    enum KE_API_ID             task_id;
    /// pointer to the custom database table defined by user
    const struct attm_desc_128 *att_db;
    /// max number of attributes in custom database
    const uint8_t              max_nb_att;
    /// Pointer to the custom database create function defined by user
    prf_func_void_t            db_create_func;
    /// Pointer to the custom profile enable function defined by user
    prf_func_uint8_t           enable_func;
    /// Pointer to the custom profile initialization function
    prf_func_void_t            init_func;
    /// Pointer to the validation function defined by user
    prf_func_validate_t        value_wr_validation_func;
};

/// @} APP

#endif //APP_PRF_TYPES_H_
