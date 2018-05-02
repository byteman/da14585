/**
 *****************************************************************************************
 *
 * @file app_bond_db.h
 *
 * @brief Bond database header file.
 *
 * Copyright (C) 2012-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 *****************************************************************************************
 */

#ifndef _APP_BOND_DB_H_
#define _APP_BOND_DB_H_

/**
 ****************************************************************************************
 * @addtogroup APP_BOND_DB
 *
 * @brief
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
#include "gapc_task.h"
#include "gap.h"
#include "app_utils.h"
#include "app_security.h"
#include "user_periph_setup.h"
#include "user_config.h"

/*
 * DEFINES
 ****************************************************************************************
 */

// SPI FLASH and I2C EEPROM data offset
#ifndef USER_CFG_BOND_DB_DATA_OFFSET
#if defined (USER_CFG_APP_BOND_DB_USE_SPI_FLASH)
    #define APP_BOND_DB_DATA_OFFSET     (0x1E000)
#elif defined (USER_CFG_APP_BOND_DB_USE_I2C_EEPROM)
    #define APP_BOND_DB_DATA_OFFSET     (0x8000)
#endif
#else
    #define APP_BOND_DB_DATA_OFFSET     (USER_CFG_BOND_DB_DATA_OFFSET)
#endif // USER_CFG_BOND_DB_DATA_OFFSET

// Max number of bonded peers
#ifndef USER_CFG_BOND_DB_MAX_BONDED_PEERS
#define APP_BOND_DB_MAX_BONDED_PEERS    (5)
#else
#define APP_BOND_DB_MAX_BONDED_PEERS    (USER_CFG_BOND_DB_MAX_BONDED_PEERS)
#endif // USER_CFG_BOND_DB_MAX_BONDED_PEERS

// Database version
#define BOND_DB_VERSION                 (0x0001)

// Start and end header used to mark the bond data in memory
#define BOND_DB_HEADER_START            ((0x1234) + BOND_DB_VERSION)
#define BOND_DB_HEADER_END              ((0x4321) + BOND_DB_VERSION)

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Do initial fetch of stored bond data.
 * @return void
 ****************************************************************************************
 */
void bond_db_init(void);

/**
 ****************************************************************************************
 * @brief Get the number of slots supported in bond database
 * @return number of slots present in bond database
 ****************************************************************************************
 */
uint8_t bond_db_get_size(void);

/**
 ****************************************************************************************
 * @brief Add a new bond data entry to the bond database. Searches the slot to write the
 *        new bond data by using the following criteria
 *        1) If there is a slot with the same IRK or BD address, replace that slot
 *        2) Else if there is an empty slot, store bond data to the first empty slot
 *        3) Else store bond data to the oldest written slot
 * @param[in] data    Pointer to the data to be stored.
 * @return void
 ****************************************************************************************
 */
void bond_db_add_entry(struct app_sec_bond_data_env_tag *data);

/**
 ****************************************************************************************
 * @brief Search the database to find the slot with the bond data that match.
 *        If remove_type is remove_this_entry, then remove this entry and update ext mem.
 *        If remove_type is remove_all_but_this, then remove all other entries and update ext mem.
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
void bond_db_remove_entry(enum bdb_search_by_type search_type, enum bdb_remove_type remove_type,
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
const struct app_sec_bond_data_env_tag* bond_db_search_entry(enum bdb_search_by_type search_type,
                                                             void *search_param,
                                                             uint8_t search_param_length);

/**
 ****************************************************************************************
 * @brief Search the bond database to find the entries which contain an IRK.
 *        IRKS found will be copied to an array.
 * @param[in] valid_irk_irray    Pointer to the array with the valid IRKs
 * @return Number of valid IRKs
 ****************************************************************************************
 */
uint8_t bond_db_get_stored_irks(struct gap_sec_key *valid_irk_irray);

#endif // (BLE_APP_SEC)
/// @} APP_BOND_DB

#endif // _APP_BOND_DB_H_
