/**
 ****************************************************************************************
 *
 * @file app_easy_security.h
 *
 * @brief Application security helper functions header file.
 *
 * Copyright (C) 2015-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef _APP_EASY_SECURITY_H_
#define _APP_EASY_SECURITY_H_

/**
 ****************************************************************************************
 * @addtogroup APP_SECURITY
 * @ingroup
 *
 * @brief Application security helper functions header file.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"

#if (BLE_APP_SEC)

#include "co_bt.h"
#include "gap.h"                // GAP API Definition
#include "gapc_task.h"          // GAP Controller Task API Definition
#include "gapm_task.h"          // GAP Manager Task API Definition
#include "app_utils.h"
#include "app_security.h"

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Get confirm requested bond information message in pairing procedure.
 * @param[in] conidx         Connection Id index
 * @return gapc_bond_cfm     Pointer to GAPC_BOND_CFM message
 ****************************************************************************************
 */
struct gapc_bond_cfm* app_easy_security_pairing_rsp_get_active(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Get confirm requested bond information message using term key exchange.
 * @param[in] conidx         Connection Id index
 * @return gapc_bond_cfm     Pointer to GAPC_BOND_CFM message
 ****************************************************************************************
 */
struct gapc_bond_cfm* app_easy_security_tk_get_active(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Get confirm requested bond information message using connection signature
 *        resolving key exchange.
 * @param[in] conidx         Connection Id index
 * @return gapc_bond_cfm     Pointer to GAPC_BOND_CFM message
 ****************************************************************************************
 */
struct gapc_bond_cfm* app_easy_security_csrk_get_active(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Get confirm requested bond information message using long term key exchange.
 * @param[in] conidx         Connection Id index
 * @return gapc_bond_cfm     Pointer to GAPC_BOND_CFM message
 ****************************************************************************************
 */
struct gapc_bond_cfm* app_easy_security_ltk_exch_get_active(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Get confirm requested encryption information message.
 * @param[in] conidx         Connection Id index
 * @return gapc_encrypt_cfm  Pointer to GAPC_ENCRYPT_CFM message
 ****************************************************************************************
 */
struct gapc_encrypt_cfm* app_easy_security_encrypt_cfm_get_active(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Set long term key loaded from security environment.
 * @param[in] conidx    Connection Id index
 * @return void
 ****************************************************************************************
 */
void app_easy_security_set_ltk_exch_from_sec_env(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Set long term key.
 * @param[in] conidx                 Connection Id index
 * @param[in] long_term_key          Pointer to long term key
 * @param[in] encryption_key_size    Encryption key size
 * @param[in] keylen                 Length of the key
 * @param[in] random_number          Pointer to random number
 * @param[in] encryption_diversifier Encryption diversifier
 * @return void
 ****************************************************************************************
 */
void app_easy_security_set_ltk_exch(uint8_t conidx, uint8_t* long_term_key,
                                    uint8_t encryption_key_size, uint8_t* random_number,
                                    uint16_t encryption_diversifier);

/**
 ****************************************************************************************
 * @brief Indicate that a long term key has been found for the peer device
 * @param[in] conidx    Connection Id index
 * @return void
 ****************************************************************************************
 */
void app_easy_security_set_encrypt_req_valid(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Indicate that a long term key has not been found for the peer device
 * @param[in] conidx    Connection Id index
 * @return void
 ****************************************************************************************
 */
void app_easy_security_set_encrypt_req_invalid(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Send confirm requested bond information in pairing procedure.
 *          Reject if minimum security requirement is GAP_SEC1_SEC_PAIR_ENC and it is not
 *          reached.
 * @param[in] conidx    Connection Id index
 * @param[in] gapc_bond_req_ind Pointer to GAPC_BOND_REQ_IND message
 * @return void
 ****************************************************************************************
 */
void app_easy_security_send_pairing_rsp(uint8_t conidx,
                                        struct gapc_bond_req_ind const *param);

/**
 ****************************************************************************************
 * @brief Send bond confirmation message using TK exchange.
 * @param[in] conidx         Connection Id index
 * @param[in] key            Pointer to the key that will be sent over TK exchange message.
 * @param[in] length         Length of the pass key in octets
 * @param[in] accept         True: Accept, False: Reject
 * @return void
 * @note                     The #key can be either a 6-digit (4 octets) pass key or an
 *                           OOB provided key. The max size of the OOB key is 128-bit
 *                           (16 octets).
 ****************************************************************************************
 */
void app_easy_security_tk_exch(uint8_t conidx, uint8_t *key, uint8_t length, bool accept);

/**
 ****************************************************************************************
 * @brief Send confirm requested bond information using connection signature resolving key
 *        exchange.
 * @param[in] conidx    Connection Id index
 * @return void
 ****************************************************************************************
 */
void app_easy_security_csrk_exch(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Send confirm requested bond information using long term key exchange.
 * @param[in] conidx     Connection Id index
 * @return void
 ****************************************************************************************
 */
void app_easy_security_ltk_exch(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Send encryption confirmation message.
 * @param[in] conidx    Connection Id index
 * @return void
 ****************************************************************************************
 */
void app_easy_security_encrypt_cfm(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Get the active security request message.
 * @param[in] conidx          Connection Id index
 * @return gapc_security_cmd  Pointer to the active security request message
 ****************************************************************************************
 */
struct gapc_security_cmd* app_easy_security_request_get_active(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Send encryption request message.
 * @param[in] conidx    Connection Id index
 * @return void
 ****************************************************************************************
 */
void app_easy_security_request(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Accept encryption request
 * @param[in] conidx    Connection Id index
 * @return void
 ****************************************************************************************
 */
void app_easy_security_accept_encryption(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Reject encryption request
 * @param[in] conidx    Connection Id index
 * @return void
 ****************************************************************************************
 */
void app_easy_security_reject_encryption(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Request tp resolve private resolvable address
 * @param[in] conidx    Connection Id index
 * @return Number of valid IRKs stored in bond database
 ****************************************************************************************
 */
uint8_t app_easy_security_resolve_bdaddr(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Initialize bond database environment and fetch bond data if a non-volatile
 *        memory is used.
 * @return void
 ****************************************************************************************
 */
void app_easy_security_bdb_init(void);

/**
 ****************************************************************************************
 * @brief Get the number of slots supported in bond database
 * @return number of slots present in bond database
 ****************************************************************************************
 */
uint8_t app_easy_security_bdb_get_size(void);

/**
 ****************************************************************************************
 * @brief Add a new bond data entry to the bond database.
 * @param[in] data    Pointer to the data to be stored.
 * @return void
 ****************************************************************************************
 */
void app_easy_security_bdb_add_entry(struct app_sec_bond_data_env_tag *data);

/**
 ****************************************************************************************
 * @brief Search the database to find the slot with the bond data that match.
 *        If remove_type is remove_this_entry, then remove this entry and update ext mem.
 *        If remove_type is remove_all_but_this, then remove all other entries and update
 *        ext mem.
 *        If remove_type is remove_all, then remove all entries and update ext mem.
 *
 * @param[in] search_type   Indicates the type with which bond database will be searched.
 *                          An entry can be found either by searching by EDIV, BDA, IRK,
 *                          custom type or if its slot is already known.
 * @param[in] remove_type   Indicates the operation that will be performed
 * @param[in] search_param  Pointer to the value that will be matched
 * @param[in] search_param_length  Size of the value that will be matched
 * @return None
 ****************************************************************************************
 */
void app_easy_security_bdb_remove_entry(enum bdb_search_by_type search_type,
                                        enum bdb_remove_type remove_type,
                                        void *search_param, uint8_t search_param_length);

/**
 ****************************************************************************************
 * @brief Search the database to find the slot with the bond data that match.
 * @param[in] search_type   Indicates the type with which bond database will be searched.
 *                          A slot can be matched either by EDIV, BDA, IRK, or custom type.
 * @param[in] search_param  Pointer to the value that will be matched
 * @param[in] search_param_length  Size of the value that will be matched
 * @return Pointer to the bond data if they were found. Otherwise null.
 ****************************************************************************************
 */
const struct app_sec_bond_data_env_tag* app_easy_security_bdb_search_entry(
                                                        enum bdb_search_by_type search_type,
                                                        void *search_param,
                                                        uint8_t search_param_length);

/**
 ****************************************************************************************
 * @brief Search the bond database to find the entries which contain an IRK.
 * @param[out] valid_irk_irray    Pointer to the array with the valid IRKs
 * @return Number of valid IRKs
 ****************************************************************************************
 */
uint8_t app_easy_security_bdb_get_stored_irks(struct gap_sec_key *valid_irk_irray);

#endif // (BLE_APP_SEC)

/// @} APP_SECURITY

#endif // _APP_EASY_SECURITY_H_
