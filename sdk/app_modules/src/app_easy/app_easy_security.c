/**
 ****************************************************************************************
 *
 * @file app_easy_security.c
 *
 * @brief Application Security Entry Point
 *
 * Copyright (C) 2015-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP_SECURITY
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"

#if (BLE_APP_SEC)

#include <stdlib.h>
#include "co_bt.h"
#include "app_mid.h"
#include "app_easy_security.h"
#include "app_security.h"
#include "user_callback_config.h"

/*
 * DEFINES
 ****************************************************************************************
 */

#define APP_EASY_SECURITY_MAX_CONNECTION APP_EASY_MAX_ACTIVE_CONNECTION

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

static struct gapc_bond_cfm *gapc_bond_cfm_pairing_rsp[APP_EASY_SECURITY_MAX_CONNECTION] __attribute__((section("retention_mem_area0"),zero_init));
static struct gapc_bond_cfm *gapc_bond_cfm_tk_exch[APP_EASY_SECURITY_MAX_CONNECTION]     __attribute__((section("retention_mem_area0"),zero_init));
static struct gapc_bond_cfm *gapc_bond_cfm_csrk_exch[APP_EASY_SECURITY_MAX_CONNECTION]   __attribute__((section("retention_mem_area0"),zero_init));
static struct gapc_bond_cfm *gapc_bond_cfm_ltk_exch[APP_EASY_SECURITY_MAX_CONNECTION]    __attribute__((section("retention_mem_area0"),zero_init));
static struct gapc_encrypt_cfm *gapc_encrypt_cfm[APP_EASY_SECURITY_MAX_CONNECTION]       __attribute__((section("retention_mem_area0"),zero_init));
static struct gapc_security_cmd *gapc_security_req[APP_EASY_SECURITY_MAX_CONNECTION]     __attribute__((section("retention_mem_area0"),zero_init));

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Creates a GAPC_BOND_CFM pairing feature response message and stores it into
 *        retention memory, if the message does not already exist.
 * @param[in] conidx        Connection index.
 * @return The pointer to the message.
 ****************************************************************************************
 */
static inline struct gapc_bond_cfm* pairing_rsp_create_msg(uint8_t conidx)
{
    // Allocate a message for GAP
    if (gapc_bond_cfm_pairing_rsp[conidx] == NULL)
    {
        struct gapc_bond_cfm* cfm = app_gapc_bond_cfm_pairing_rsp_msg_create(conidx);
        gapc_bond_cfm_pairing_rsp[conidx] = cfm;
        cfm->accept = true;
        memcpy(&cfm->data.pairing_feat, &user_security_conf, sizeof(struct security_configuration));
    }
    return gapc_bond_cfm_pairing_rsp[conidx];
}

struct gapc_bond_cfm* app_easy_security_pairing_rsp_get_active(uint8_t conidx)
{
    ASSERT_WARNING(conidx < APP_EASY_SECURITY_MAX_CONNECTION);
    return pairing_rsp_create_msg(conidx);
}

void app_easy_security_send_pairing_rsp(uint8_t conidx, struct gapc_bond_req_ind const *param)
{
    struct gapc_bond_cfm* cmd = app_gapc_bond_cfm_pairing_rsp_msg_create(conidx);
    uint8_t auth;   // Temporary viariable to store actual authentication level

    ASSERT_WARNING(conidx < APP_EASY_SECURITY_MAX_CONNECTION);

    if (gapc_bond_cfm_pairing_rsp[conidx] == NULL)
    {
        gapc_bond_cfm_pairing_rsp[conidx] = cmd;

        // Store pairing features to pairing response
        memcpy(&cmd->data.pairing_feat, &user_security_conf, sizeof(struct security_configuration));

        // Store authentication level to temporary variable
        auth = user_security_conf.auth;

        // Ensure that if local device has OOB support, then Legacy Pairing will be used
        if (user_security_conf.oob == GAP_OOB_AUTH_DATA_PRESENT)
        {
            // Change the actual authentication level to Legacy Pairing
            auth = user_security_conf.auth & (~GAP_AUTH_SEC);
            cmd->data.pairing_feat.auth = auth;
        }
        cmd->accept = true;
    }
    ke_msg_send(cmd);
    gapc_bond_cfm_pairing_rsp[conidx] = NULL;
}

/**
 ****************************************************************************************
 * @brief Creates a GAPC_BOND_CFM temporary key (TK) exchange message and stores it into
 *        retention memory, if hte message does not already exist.
 * @param[in] conidx        Connection index.
 * @return The pointer to the message.
 ****************************************************************************************
 */
static inline struct gapc_bond_cfm* tk_exch_create_msg(uint8_t conidx)
{
    // Allocate a message for GAP
    if (gapc_bond_cfm_tk_exch[conidx] == NULL)
    {
        gapc_bond_cfm_tk_exch[conidx] = app_gapc_bond_cfm_tk_exch_msg_create(conidx);
    }
     return gapc_bond_cfm_tk_exch[conidx];
}

struct gapc_bond_cfm* app_easy_security_tk_get_active(uint8_t conidx)
{
    ASSERT_WARNING(conidx < APP_EASY_SECURITY_MAX_CONNECTION);
    return tk_exch_create_msg(conidx);
}

void app_easy_security_tk_exch(uint8_t conidx, uint8_t *key, uint8_t length, bool accept)
{
    struct gapc_bond_cfm* cmd;
    ASSERT_WARNING(conidx < APP_EASY_SECURITY_MAX_CONNECTION);
    cmd = tk_exch_create_msg(conidx);

    // Load the pass key or the OOB provided key to the TK member of the created GAPC_BOND_CFM message
    memset((void*)gapc_bond_cfm_tk_exch[conidx]->data.tk.key, 0, KEY_LEN);
    memcpy(gapc_bond_cfm_tk_exch[conidx]->data.tk.key, key, length * sizeof(uint8_t));

    cmd->accept = accept;
    ke_msg_send(cmd);
    gapc_bond_cfm_tk_exch[conidx] = NULL;
}

/**
 ****************************************************************************************
 * @brief Creates a GAPC_BOND_CFM connection signature resolving key (CSRK) exchange
 *        message and stores it into retention memory, if the message does not already exist.
 * @param[in] conidx        Connection index.
 * @return The pointer to the message.
 ****************************************************************************************
 */
static inline struct gapc_bond_cfm* csrk_exch_create_msg(uint8_t conidx)
{
    // Allocate a message for GAP
    if (gapc_bond_cfm_csrk_exch[conidx] == NULL)
    {
        gapc_bond_cfm_csrk_exch[conidx] = app_gapc_bond_cfm_csrk_exch_msg_create(conidx);
        app_sec_gen_csrk(conidx);
        memcpy((void*)gapc_bond_cfm_csrk_exch[conidx]->data.csrk.key, app_sec_env[conidx].lcsrk.key, KEY_LEN);
    }
     return gapc_bond_cfm_csrk_exch[conidx];
}

struct gapc_bond_cfm* app_easy_security_csrk_get_active(uint8_t conidx)
{
    ASSERT_WARNING(conidx < APP_EASY_SECURITY_MAX_CONNECTION);
    return csrk_exch_create_msg(conidx);
}

void app_easy_security_csrk_exch(uint8_t conidx)
{
    struct gapc_bond_cfm* cfm;
    ASSERT_WARNING(conidx < APP_EASY_SECURITY_MAX_CONNECTION);
    cfm = csrk_exch_create_msg(conidx);
    ke_msg_send(cfm);
    gapc_bond_cfm_csrk_exch[conidx] = NULL;
}

/**
 ****************************************************************************************
 * @brief Creates a GAPC_BOND_CFM long term key (LTK) exchange message and stores it into
 *        retention memory, if the message does not already exist.
 * @param[in] conidx        Connection index.
 * @return The pointer to the message.
 ****************************************************************************************
 */
static inline struct gapc_bond_cfm* ltk_exch_create_msg(uint8_t conidx)
{
    // Allocate a message for GAP
    if (gapc_bond_cfm_ltk_exch[conidx] == NULL)
    {
        gapc_bond_cfm_ltk_exch[conidx] = app_gapc_bond_cfm_ltk_exch_msg_create(conidx);
    }
    return gapc_bond_cfm_ltk_exch[conidx];
}

struct gapc_bond_cfm* app_easy_security_ltk_exch_get_active(uint8_t conidx)
{
    ASSERT_WARNING(conidx < APP_EASY_SECURITY_MAX_CONNECTION);
    return ltk_exch_create_msg(conidx);
}

void app_easy_security_ltk_exch(uint8_t conidx)
{
    struct gapc_bond_cfm* cfm;
    ASSERT_WARNING(conidx < APP_EASY_SECURITY_MAX_CONNECTION);
    cfm = ltk_exch_create_msg(conidx);
    ke_msg_send(cfm);
    gapc_bond_cfm_ltk_exch[conidx] = NULL;
}

void app_easy_security_set_ltk_exch_from_sec_env(uint8_t conidx)
{
    struct gapc_bond_cfm* cfm;
    ASSERT_WARNING(conidx < APP_EASY_SECURITY_MAX_CONNECTION);
    cfm = ltk_exch_create_msg(conidx);
    cfm->data.ltk.key_size = app_sec_env[conidx].ltk.key_size;
    cfm->data.ltk.ediv = app_sec_env[conidx].ltk.ediv;

    memcpy(&cfm->data.ltk.randnb, &app_sec_env[conidx].ltk.randnb , RAND_NB_LEN);
    memcpy(&cfm->data.ltk.ltk, &app_sec_env[conidx].ltk , KEY_LEN);
}

void app_easy_security_set_ltk_exch(uint8_t conidx, uint8_t* long_term_key, uint8_t encryption_key_size,
                                            uint8_t* random_number, uint16_t encryption_diversifier)
{
    struct gapc_bond_cfm* cfm;
    ASSERT_WARNING(conidx < APP_EASY_SECURITY_MAX_CONNECTION);
    cfm = ltk_exch_create_msg(conidx);
    cfm->data.ltk.key_size = encryption_key_size;
    cfm->data.ltk.ediv = encryption_diversifier;

    memcpy(&(cfm->data.ltk.randnb), random_number , RAND_NB_LEN);
    memcpy(&(cfm->data.ltk.ltk), long_term_key , KEY_LEN);
}

/**
 ****************************************************************************************
 * @brief Creates a GAPC_ENCRYPT_CFM encryption information message and stores it into
          retention memory, if the message does not already exist.
 * @param[in] conidx Connection index.
 * @return The pointer to the message.
 ****************************************************************************************
 */
static inline struct gapc_encrypt_cfm* encrypt_cfm_create_msg(uint8_t conidx)
{
    // Allocate a message for GAP
    if (gapc_encrypt_cfm[conidx] == NULL)
    {
        gapc_encrypt_cfm[conidx] = app_gapc_encrypt_cfm_msg_create(conidx);
    }
    return gapc_encrypt_cfm[conidx];
}

void app_easy_security_encrypt_cfm(uint8_t conidx)
{
    struct gapc_encrypt_cfm* cfm;
    ASSERT_WARNING(conidx < APP_EASY_SECURITY_MAX_CONNECTION);
    cfm = encrypt_cfm_create_msg(conidx);
    ke_msg_send(cfm);
    gapc_encrypt_cfm[conidx] = NULL;
}

struct gapc_encrypt_cfm* app_easy_security_encrypt_cfm_get_active( uint8_t conidx )
{
    ASSERT_WARNING(conidx < APP_EASY_SECURITY_MAX_CONNECTION);
    return encrypt_cfm_create_msg(conidx);
}

void app_easy_security_set_encrypt_req_valid(uint8_t conidx)
{
    ASSERT_WARNING(conidx < APP_EASY_SECURITY_MAX_CONNECTION);
    struct gapc_encrypt_cfm* cfm = encrypt_cfm_create_msg(conidx);
    cfm->found = true;
    cfm->key_size = app_sec_env[conidx].ltk.key_size;
    memcpy(&cfm->ltk, &app_sec_env[conidx].ltk.ltk, KEY_LEN);
}

void app_easy_security_set_encrypt_req_invalid(uint8_t conidx)
{
    ASSERT_WARNING(conidx < APP_EASY_SECURITY_MAX_CONNECTION);
    struct gapc_encrypt_cfm* cfm = encrypt_cfm_create_msg(conidx);
    cfm->found = false;
}

/**
 ****************************************************************************************
 * @brief Creates a GAPC_SECURITY_CMD security request command message and stores it into
          retention memory, if the message does not already exist.
 * @param[in] conidx        Connection index.
 * @return The pointer to the message.
 ****************************************************************************************
 */
static inline struct gapc_security_cmd* security_request_create_msg(uint8_t conidx)
{
    // Allocate a message for GAP
    if (gapc_security_req[conidx] == NULL)
    {
        gapc_security_req[conidx] = app_gapc_security_request_msg_create(conidx, (enum gap_auth) user_security_conf.auth);
    }
    return gapc_security_req[conidx];
}

struct gapc_security_cmd* app_easy_security_request_get_active(uint8_t conidx)
{
     ASSERT_WARNING(conidx < APP_EASY_SECURITY_MAX_CONNECTION);
     return security_request_create_msg(conidx);
}

void app_easy_security_request(uint8_t conidx)
{
    struct gapc_security_cmd* req;
    ASSERT_WARNING(conidx < APP_EASY_SECURITY_MAX_CONNECTION);
    req = app_easy_security_request_get_active(conidx);
    ke_msg_send(req);
    gapc_security_req[conidx] = NULL;
}

void app_easy_security_accept_encryption(uint8_t conidx)
{
    // Update connection auth
    app_easy_gap_confirm(conidx, (enum gap_auth) app_sec_env[conidx].auth, 1);
    app_easy_security_set_encrypt_req_valid(conidx);
    app_easy_security_encrypt_cfm(conidx);
}

void app_easy_security_reject_encryption(uint8_t conidx)
{
    app_easy_security_set_encrypt_req_invalid(conidx);
    app_easy_security_encrypt_cfm(conidx);
    app_easy_gap_disconnect(conidx);
}

uint8_t app_easy_security_resolve_bdaddr(uint8_t conidx)
{
    uint8_t nb_key = 0;
    uint8_t bond_db_slots = app_easy_security_bdb_get_size();
    struct gap_sec_key valid_irk_irray[bond_db_slots];

    nb_key = app_easy_security_bdb_get_stored_irks(valid_irk_irray);

    if(nb_key)
    {
        struct gapm_resolv_addr_cmd *cmd = KE_MSG_ALLOC_DYN(GAPM_RESOLV_ADDR_CMD,
                                                        TASK_GAPM, TASK_APP, gapm_resolv_addr_cmd,
                                                        nb_key * sizeof(struct gap_sec_key));

        cmd->operation = GAPM_RESOLV_ADDR;                                      // GAPM requested operation
        cmd->nb_key = nb_key;                                                   // Number of provided IRKs
        memcpy( &cmd->addr, app_env[conidx].peer_addr.addr, BD_ADDR_LEN);       // Resolvable random address to resolve
        memcpy( cmd->irk, (uint8_t *)valid_irk_irray, nb_key * sizeof(struct gap_sec_key));
        // Send the message
        ke_msg_send(cmd);
    }
    return nb_key;
}

void app_easy_security_bdb_init(void)
{
    CALLBACK_ARGS_0(user_app_bond_db_callbacks.app_bdb_init)
}

uint8_t app_easy_security_bdb_get_size(void)
{
    if (user_app_bond_db_callbacks.app_bdb_get_size != NULL)
    {
        return user_app_bond_db_callbacks.app_bdb_get_size();
    }
    return 0;
}

void app_easy_security_bdb_add_entry(struct app_sec_bond_data_env_tag *data)
{
    CALLBACK_ARGS_1(user_app_bond_db_callbacks.app_bdb_add_entry, data)
}

void app_easy_security_bdb_remove_entry(enum bdb_search_by_type search_type, enum bdb_remove_type remove_type,
                                        void *search_param, uint8_t search_param_length)
{
    CALLBACK_ARGS_4(user_app_bond_db_callbacks.app_bdb_remove_entry, search_type, remove_type, search_param, search_param_length)
}

const struct app_sec_bond_data_env_tag* app_easy_security_bdb_search_entry(enum bdb_search_by_type search_type, void *search_param,
                                        uint8_t search_param_length)
{
    if (user_app_bond_db_callbacks.app_bdb_search_entry != NULL)
    {
        return user_app_bond_db_callbacks.app_bdb_search_entry(search_type, search_param, search_param_length);
    }
    return NULL;
}

uint8_t app_easy_security_bdb_get_stored_irks(struct gap_sec_key *valid_irk_irray)
{
    if (user_app_bond_db_callbacks.app_bdb_get_stored_irks != NULL)
    {
        return user_app_bond_db_callbacks.app_bdb_get_stored_irks(valid_irk_irray);
    }
    return 0;
}

#endif // (BLE_APP_SEC)

/// @} APP_SECURITY
