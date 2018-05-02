/**
 ****************************************************************************************
 *
 * @file app_ctsc_task.c
 *
 * @brief Current Time Service Application Task
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
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"        // SW Configuration

#if (BLE_APP_PRESENT)

#if (BLE_CTS_CLIENT)
#include <stdint.h>
#include "ctsc_task.h"          // Current Time Service Server Task API
#include "ctsc.h"               // Current Time Service Definitions
#include "app_ctsc.h"           // Current Time Service Application Definitions
#include "app_ctsc_task.h"      // Current Time Service Application Task API
#include "app_task.h"           // Application Task API
#include "app_entry_point.h"
#include "app.h"
#include "user_profiles_config.h"
#include "user_callback_config.h"

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref CTSC_ENABLE_RSP message.
 *
 * @param[in] msgid Id of the message received.
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance.
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int ctsc_enable_handler(const ke_msg_id_t msgid,
                               const struct ctsc_enable_rsp *param,
                               const ke_task_id_t dest_id,
                               const ke_task_id_t src_id)
{
    if (param->status == GAP_ERR_NO_ERROR)
    {
        CALLBACK_ARGS_2(user_app_ctsc_cb.on_connect, param->conidx, &param->cts);
    }

    return KE_MSG_CONSUMED;
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref CTSC_READ_CT_RSP message.
 *
 * @param[in] msgid Id of the message received.
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance.
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int ctsc_read_ct_handler(const ke_msg_id_t msgid,
                                const struct ctsc_read_ct_rsp *param,
                                const ke_task_id_t dest_id,
                                const ke_task_id_t src_id)
{
    if (user_app_ctsc_cb.on_ct_read != NULL)
    {
        user_app_ctsc_cb.on_ct_read(param->rsp.conidx, param->rsp.status, &param->ct_val, 0);
    }

    return KE_MSG_CONSUMED;
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref CTSC_READ_LTI_RSP message.
 *
 * @param[in] msgid Id of the message received.
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance.
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int ctsc_read_lti_handler(const ke_msg_id_t msgid,
                                 const struct ctsc_read_lti_rsp *param,
                                 const ke_task_id_t dest_id,
                                 const ke_task_id_t src_id)
{
    CALLBACK_ARGS_3(user_app_ctsc_cb.on_lti_read, param->rsp.conidx, param->rsp.status, &param->lti_val);

    return KE_MSG_CONSUMED;
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref CTSC_READ_RTI_RSP message.
 *
 * @param[in] msgid Id of the message received.
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance.
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int ctsc_read_rti_handler(const ke_msg_id_t msgid,
                                 const struct ctsc_read_rti_rsp *param,
                                 const ke_task_id_t dest_id,
                                 const ke_task_id_t src_id)
{
    CALLBACK_ARGS_3(user_app_ctsc_cb.on_rti_read, param->rsp.conidx, param->rsp.status, &param->rti_val);

    return KE_MSG_CONSUMED;
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref CTSC_READ_DESC_RSP message.
 *
 * @param[in] msgid Id of the message received.
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance.
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int ctsc_read_desc_handler(const ke_msg_id_t msgid,
                                  const struct ctsc_read_desc_rsp *param,
                                  const ke_task_id_t dest_id,
                                  const ke_task_id_t src_id)
{
    CALLBACK_ARGS_3(user_app_ctsc_cb.on_desc_read, param->rsp.conidx, param->rsp.status, param->ntf_cfg);

    return KE_MSG_CONSUMED;
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref CTSC_WRITE_CT_RSP message.
 *
 * @param[in] msgid Id of the message received.
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance.
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int ctsc_write_ct_handler(const ke_msg_id_t msgid,
                                 const struct ctsc_rsp *param,
                                 const ke_task_id_t dest_id,
                                 const ke_task_id_t src_id)
{
    CALLBACK_ARGS_2(user_app_ctsc_cb.on_ct_write, param->conidx, param->status);

    return KE_MSG_CONSUMED;
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref CTSC_WRITE_LTI_RSP message.
 *
 * @param[in] msgid Id of the message received.
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance.
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int ctsc_write_lti_handler(const ke_msg_id_t msgid,
                                  const struct ctsc_rsp *param,
                                  const ke_task_id_t dest_id,
                                  const ke_task_id_t src_id)
{
    CALLBACK_ARGS_2(user_app_ctsc_cb.on_lti_write, param->conidx, param->status);

    return KE_MSG_CONSUMED;
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref CTSC_WRITE_DESC_RSP message.
 *
 * @param[in] msgid Id of the message received.
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance.
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int ctsc_write_desc_handler(const ke_msg_id_t msgid,
                                   const struct ctsc_rsp *param,
                                   const ke_task_id_t dest_id,
                                   const ke_task_id_t src_id)
{
    CALLBACK_ARGS_2(user_app_ctsc_cb.on_desc_write, param->conidx, param->status);

    return KE_MSG_CONSUMED;
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref CTSC_CT_IND message.
 *
 * @param[in] msgid Id of the message received.
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance.
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int ctsc_ct_ind_handler(const ke_msg_id_t msgid,
                               const struct ctsc_ct_ind *param,
                               const ke_task_id_t dest_id,
                               const ke_task_id_t src_id)
{
    if (user_app_ctsc_cb.on_ct_read != NULL)
    {
        user_app_ctsc_cb.on_ct_read(param->conidx, param->status, &param->ct_val, 0);
    }

    return KE_MSG_CONSUMED;
}

/*
 * LOCAL VARIABLES DEFINITION
 ****************************************************************************************
 */

static const struct ke_msg_handler app_ctsc_process_handlers[] =
{
    {CTSC_ENABLE_RSP,      (ke_msg_func_t) ctsc_enable_handler},
    {CTSC_READ_CT_RSP,     (ke_msg_func_t) ctsc_read_ct_handler},
    {CTSC_READ_LTI_RSP,    (ke_msg_func_t) ctsc_read_lti_handler},
    {CTSC_READ_RTI_RSP,    (ke_msg_func_t) ctsc_read_rti_handler},
    {CTSC_READ_DESC_RSP,   (ke_msg_func_t) ctsc_read_desc_handler},
    {CTSC_WRITE_CT_RSP,    (ke_msg_func_t) ctsc_write_ct_handler},
    {CTSC_WRITE_LTI_RSP,   (ke_msg_func_t) ctsc_write_lti_handler},
    {CTSC_WRITE_DESC_RSP,  (ke_msg_func_t) ctsc_write_desc_handler},
    {CTSC_CT_IND,          (ke_msg_func_t) ctsc_ct_ind_handler},
};

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

enum process_event_response app_ctsc_process_handler(ke_msg_id_t const msgid,
                                                     void const *param,
                                                     ke_task_id_t const dest_id,
                                                     ke_task_id_t const src_id,
                                                     enum ke_msg_status_tag *msg_ret)
{
    return (app_std_process_event(msgid, param, src_id, dest_id, msg_ret, app_ctsc_process_handlers,
                                         sizeof(app_ctsc_process_handlers) / sizeof(struct ke_msg_handler)));
}

#endif //(BLE_CTS_CLIENT)

#endif //BLE_APP_PRESENT

/// @} APP
