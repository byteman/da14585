/**
 ****************************************************************************************
 *
 * @file app_wsss_task.c
 *
 * @brief Weight Scale Profile Server Application task implementation.
 *
 * Copyright (C) 2017 Dialog Semiconductor.
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

#include "rwip_config.h"               // SW configuration

#if (BLE_APP_PRESENT)

#if (BLE_WSS_SERVER)
#include "wsss_task.h"
#include "app_wsss.h"
#include "app_wsss_task.h"
#include "app_task.h"                  // Application Task API
#include "app_entry_point.h"
#include "app.h"
#include "user_callback_config.h"


/**
 ****************************************************************************************
 * @brief Message handler for the WSS Server Application message (service enable).
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance.
 * @param[in] src_id    ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int wsss_enable_cfm_handler(ke_msg_id_t const msgid,
                                       struct wsss_enable_cfm const *param,
                                       ke_task_id_t const dest_id,
                                       ke_task_id_t const src_id)
{
    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Message handler for the WSS Application measurement sent confirmation.
 * @param[in] msgid   Id of the message received
 * @param[in] param   Pointer to the parameters of the message
 * @param[in] dest_id ID of the receiving task instance
 * @param[in] src_id  ID of the sending task instance
 * @return Returns if the message is handled by the process handler
 ****************************************************************************************
 */
static int wsss_meas_send_cfm_handler(ke_msg_id_t const msgid,
                                      struct wsss_meas_send_cfm const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{

    CALLBACK_ARGS_2(user_app_wsss_cb.on_wsss_send_measurement_cfm,
                    KE_IDX_GET(src_id), param->status);

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Message handler for the WSS Application CCC descriptor reconfiguration.
 * @param[in] msgid   Id of the message received
 * @param[in] param   Pointer to the parameters of the message
 * @param[in] dest_id ID of the receiving task instance
 * @param[in] src_id  ID of the sending task instance
 * @return Returns if the message is handled by the process handler
 ****************************************************************************************
 */
static int wsss_cfg_indntf_ind_handler(ke_msg_id_t const msgid,
                                       struct wsss_cfg_indntf_ind const *param,
                                       ke_task_id_t const dest_id,
                                       ke_task_id_t const src_id)
{
    bool enabled = param->cfg_val ? true : false;

    CALLBACK_ARGS_2(user_app_wsss_cb.on_wsss_ind_cfg_ind,
                    KE_IDX_GET(src_id), enabled);
    return (KE_MSG_CONSUMED);
}

/*
 * GLOBAL VARIABLES DEFINITION
 ****************************************************************************************
 */

const struct ke_msg_handler app_wsss_process_handlers[]=
{
    {WSSS_ENABLE_CFM,                  (ke_msg_func_t)wsss_enable_cfm_handler},
    {WSSS_MEAS_SEND_CFM,               (ke_msg_func_t)wsss_meas_send_cfm_handler},
    {WSSS_CFG_INDNTF_IND,              (ke_msg_func_t)wsss_cfg_indntf_ind_handler},
};

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

enum process_event_response app_wsss_process_handler(ke_msg_id_t const msgid,
                                                       void const *param,
                                                       ke_task_id_t const dest_id,
                                                       ke_task_id_t const src_id,
                                                       enum ke_msg_status_tag *msg_ret)
{
    return app_std_process_event(msgid, param, src_id, dest_id, msg_ret, app_wsss_process_handlers,
                                         sizeof(app_wsss_process_handlers) / sizeof(struct ke_msg_handler));
}

#endif //BLE_WSS_SERVER

#endif //(BLE_APP_PRESENT)

/// @} APPTASK
