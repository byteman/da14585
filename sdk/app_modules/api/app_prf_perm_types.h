/**
 ****************************************************************************************
 *
 * @file app_prf_perm_types.h
 *
 * @brief app - Service access permission rights api.
 *
 * Copyright (C) 2012-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef _APP_PRF_PERM_TYPES_H_
#define _APP_PRF_PERM_TYPES_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "attm.h"                       // Service access permissions 

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/// Service access rights
typedef enum app_prf_srv_perm
{
    /// Disable access
    SRV_PERM_DISABLE  = PERM(SVC_AUTH, DISABLE),
    /// Enable access
    SRV_PERM_ENABLE   = PERM(SVC_AUTH, ENABLE),
    /// Access Requires Unauthenticated link
    SRV_PERM_UNAUTH   = PERM(SVC_AUTH, UNAUTH),
    /// Access Requires Authenticated link
    SRV_PERM_AUTH     = PERM(SVC_AUTH, AUTH),
    /// Access Requires Secure Connection Pairing
    SRV_PERM_SECURE   = PERM(SVC_AUTH, SECURE),
} app_prf_srv_perm_t;

typedef struct app_prf_srv_sec
{
    enum KE_API_ID        task_id;
    app_prf_srv_perm_t    perm;
    
} app_prf_srv_sec_t;

extern app_prf_srv_perm_t* app_srv_perm;

/*
 * DEFINES
 ****************************************************************************************
 */

// Maximum number for profiles (TASK_IDs) that can be included in a user application
#define PRFS_TASK_ID_MAX    (10)

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/*
 ****************************************************************************************
 * @brief Returns the Service permission set by user. If user has not set any service
 * permission, the default "ENABLE" is used.
 * @param[in] task_id           Task type definition of the profile.
 * @return app_prf_srv_perm_t   Service access permission rights.
 ****************************************************************************************
*/
app_prf_srv_perm_t get_user_prf_srv_perm(enum KE_API_ID task_id);

/*
 ****************************************************************************************
 * @brief Sets the service permission access rights for a profile.
 * @param[in] task_id           Task type definition of the profile.
 * @param[in] srv_perm          Service permission access rights of the profile.
 * @return void
 ****************************************************************************************
 */
void app_set_prf_srv_perm(enum KE_API_ID task_id, app_prf_srv_perm_t srv_perm);

/*
 ****************************************************************************************
 * @brief Initialises service permissions to "ENABLE". Should be called once upon app 
 * initialization.
 * @return void
 ****************************************************************************************
 */
void prf_init_srv_perm(void);

/// @} APP

#endif // _APP_PRF_TYPES_H_
