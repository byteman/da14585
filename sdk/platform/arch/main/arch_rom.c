/**
 ****************************************************************************************
 *
 * @file arch_rom.c
 *
 * @brief ROM initialization functions.
 *
 * Copyright (C) 2016-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#include <string.h>
#include "llm.h"
#include "gap_cfg.h"
#include "da1458x_scatter_config.h"
#include "_reg_ble_em_rx_buffer.h"
#include "_reg_ble_em_tx_buffer.h"
#include "em_map_ble.h"
#include "em_map.h"
#include "arch_rom.h"
#include "system_library.h"

#ifdef CFG_DBG_TASK
#include "hci_vs.h"
#endif 

/**
 * @addtogroup DRIVERS
 * @{
 */

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

uint16_t heap_log __attribute__((section("retention_mem_area0"), zero_init));

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void arch_rom_init(void)
{
    // Initialize variables used by ROM patches
    patch_global_vars_init();
    
    // Enable heap log
    heap_log = LOG_HEAP_USAGE;

    // Mandatory HCI root table setup
    memcpy(hci_cmd_desc_root_tab, rom_hci_cmd_desc_root_tab, 48);

#ifdef CFG_DBG_TASK
    // Vendor specific HCI commands initialization
    hci_vs_initialize_commands_tab();
#endif

#ifdef AUTO_DATA_LENGTH_NEGOTIATION_UPON_NEW_CONNECTION
    length_exchange_needed = true;
#endif

    use_h4tl = CFG_USE_H4TL;

    gap_cfg_user = (struct gap_cfg_user_struct *)&gap_cfg_user_var_struct;

    rom_func_addr_table = &rom_func_addr_table_var[0];
    rom_cfg_table = &rom_cfg_table_var[0];

    ble_duplicate_filter_max = CFG_BLE_DUPLICATE_FILTER_MAX;
#ifdef CFG_BLE_DUPLICATE_FILTER_FOUND
    ble_duplicate_filter_found = true;
#else
    ble_duplicate_filter_found = false;
#endif
    llm_resolving_list_max = CFG_LLM_RESOLVING_LIST_MAX;

    BLE_TX_DESC_DATA_USER           = __SCT_BLE_TX_DESC_DATA;
    BLE_TX_DESC_CNTL_USER           = __SCT_BLE_TX_DESC_CNTL;

    LLM_LE_ADV_DUMMY_IDX            = __SCT_BLE_TX_DESC_DATA          + __SCT_BLE_TX_DESC_CNTL - 1;
    LLM_LE_SCAN_CON_REQ_ADV_DIR_IDX = LLM_LE_ADV_DUMMY_IDX            + 1;
    LLM_LE_SCAN_RSP_IDX             = LLM_LE_SCAN_CON_REQ_ADV_DIR_IDX + 1;
    LLM_LE_ADV_IDX                  = LLM_LE_SCAN_RSP_IDX             + 1;

    REG_BLE_EM_RX_BUFFER_SIZE       = __SCT_REG_BLE_EM_RX_BUFFER_SIZE;
    REG_BLE_EM_TX_BUFFER_SIZE       = __SCT_REG_BLE_EM_TX_BUFFER_SIZE;

    ASSERT_ERROR(
        (BLE_TX_DESC_DATA == __SCT_BLE_TX_DESC_DATA) &&
        (BLE_TX_DESC_CNTL == __SCT_BLE_TX_DESC_CNTL) &&
        // offsets
        (_EM_BLE_RX_BUFFER_OFFSET == __SCT__EM_BLE_RX_BUFFER_OFFSET) &&
        (EM_BLE_RX_BUFFER_OFFSET  == __SCT_EM_BLE_RX_BUFFER_OFFSET ) &&
        (_EM_BLE_CNXADD_OFFSET    == __SCT__EM_BLE_CNXADD_OFFSET   ) &&
        (EM_BLE_CNXADD_OFFSET     == __SCT_EM_BLE_CNXADD_OFFSET    ) &&
        (_EM_BLE_END              == __SCT__EM_BLE_END             ) &&
        (EM_BLE_END               == __SCT_EM_BLE_END              )
    );

    _ble_base = __SCT_BLE_BASE; // Initialize BLE EM base
}

/// @} DRIVERS
