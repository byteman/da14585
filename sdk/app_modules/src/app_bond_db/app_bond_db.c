/**
 *****************************************************************************************
 *
 * @file app_bond_db.c
 *
 * @brief Bond database code file.
 *
 * Copyright (C) 2012-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 *****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP_BOND_DB
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"

#if (BLE_APP_SEC)

#include "rwip.h"
#include "app_bond_db.h"

#if defined (USER_CFG_APP_BOND_DB_USE_SPI_FLASH)
#include "spi_flash.h"
#elif defined (USER_CFG_APP_BOND_DB_USE_I2C_EEPROM)
#include "i2c_eeprom.h"
#endif

/*
 * DEFINES
 ****************************************************************************************
 */
#define BOND_DB_VALID_ENTRY             (0xAA)
#define BOND_DB_EMPTY_SLOT              (0)
#define BOND_DB_SLOT_NOT_FOUND          (0xFF)

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

struct bond_db
{
    uint16_t start_hdr;
    uint8_t valid_slot[APP_BOND_DB_MAX_BONDED_PEERS];
    uint32_t timestamp_counter;
    uint32_t timestamp[APP_BOND_DB_MAX_BONDED_PEERS];
    struct app_sec_bond_data_env_tag data[APP_BOND_DB_MAX_BONDED_PEERS];
    uint16_t end_hdr;
};

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

static struct bond_db bdb __attribute__((section("retention_mem_area0"), zero_init)); //@RETENTION MEMORY

/*
 * STATIC FUNCTION DEFINITIONS
 ****************************************************************************************
 */

#if defined (USER_CFG_APP_BOND_DB_USE_SPI_FLASH)

static void bond_db_spi_flash_init(void)
{
    static int8_t dev_id;

    dev_id = spi_flash_enable(SPI_EN_GPIO_PORT, SPI_EN_GPIO_PIN);
    if (dev_id == SPI_FLASH_AUTO_DETECT_NOT_DETECTED)
    {
        // The device was not identified. The default parameters are used.
        // Alternatively, an error can be asserted here.
        spi_flash_init(SPI_FLASH_DEFAULT_SIZE, SPI_FLASH_DEFAULT_PAGE);
    }
}

static void bond_db_load_flash(void)
{
    bond_db_spi_flash_init();

    spi_flash_read_data((uint8_t *)&bdb, APP_BOND_DB_DATA_OFFSET, sizeof(struct bond_db));

    // Power down flash
    spi_flash_power_down();

    spi_release();
}

/**
 ****************************************************************************************
 * @brief Erase Flash sectors where bond database is stored
 * @param[in] scheduler_en  True: Enable rwip_scheduler while Flash is being erased
 *                          False: Do not enable rwip_scheduler. Blocking mode
 * @return ret              Error code or success (ERR_OK)
 ****************************************************************************************
 */
static int8_t bond_db_erase_flash_sectors(bool scheduler_en)
{
    uint32_t sector_nb;
    uint32_t offset;
    int8_t ret;
    int i;
    uint32_t timeout_cnt;

    // Calculate the starting sector offset
    offset = (APP_BOND_DB_DATA_OFFSET / SPI_SECTOR_SIZE) * SPI_SECTOR_SIZE;

    // Calculate the numbers of sectors to erase
    sector_nb = (sizeof(bdb) / SPI_SECTOR_SIZE);
    if (sizeof(bdb) % SPI_SECTOR_SIZE)
        sector_nb++;

    for (i = 0; i < sector_nb; i++)
    {
        if (scheduler_en)
        {
            // Non-Blocking Erase of a Flash sector
            ret = spi_flash_block_erase_no_wait(offset, SECTOR_ERASE);
            if (ret != ERR_OK)
                break;

            timeout_cnt = 0;

            while ((spi_flash_read_status_reg() & STATUS_BUSY) != 0)
            {
                // Check if BLE is on and not in deep sleep and call rwip_schedule()
                if ((GetBits16(CLK_RADIO_REG, BLE_ENABLE) == 1) &&
                   (GetBits32(BLE_DEEPSLCNTL_REG, DEEP_SLEEP_STAT) == 0))
                {
                    // Assuming that the WDG is not active, timeout will be reached in case of a Flash erase error.
                    // NOTE: In case the WDG is active, the WDG timer will expire (much) earlier than the timeout
                    // is reached and therefore an NMI will be triggered.
                    if (++timeout_cnt > MAX_READY_WAIT_COUNT)
                    {
                        return ERR_TIMEOUT;
                    }
                    rwip_schedule();
                }
            }
        }
        else
        {
            // Blocking Erase of a Flash sector
            ret = spi_flash_block_erase(offset, SECTOR_ERASE);
            if (ret != ERR_OK)
                break;
        }
        offset += SPI_SECTOR_SIZE;
    }

    return ret;
}

/**
 ****************************************************************************************
 * @brief Store Bond Database to Flash memory
 * @param[in] scheduler_en  True: Enable rwip_scheduler while Flash is being erased
 *                          False: Do not enable rwip_scheduler. Blocking mode
 * @return none
 ****************************************************************************************
 */
static void bond_db_store_flash(bool scheduler_en)
{
    int8_t ret;

    bond_db_spi_flash_init();

    ret = bond_db_erase_flash_sectors(scheduler_en);
    if (ret == ERR_OK)
    {
        spi_flash_write_data((uint8_t *)&bdb, APP_BOND_DB_DATA_OFFSET, sizeof(struct bond_db));
    }

    // Power down flash
    spi_flash_power_down();

    spi_release();
}

#elif defined (USER_CFG_APP_BOND_DB_USE_I2C_EEPROM)

static void bond_db_load_eeprom(void)
{
    uint32_t bytes_read;

    i2c_eeprom_init(I2C_SLAVE_ADDRESS, I2C_SPEED_MODE, I2C_ADDRESS_MODE, I2C_ADDRESS_SIZE);

    i2c_eeprom_read_data((uint8_t *)&bdb, APP_BOND_DB_DATA_OFFSET, sizeof(struct bond_db), &bytes_read);
    ASSERT_ERROR(bytes_read == sizeof(struct bond_db));

    i2c_eeprom_release();
}

static void bond_db_store_eeprom(void)
{
    uint32_t bytes_written;

    i2c_eeprom_init(I2C_SLAVE_ADDRESS, I2C_SPEED_MODE, I2C_ADDRESS_MODE, I2C_ADDRESS_SIZE);

    i2c_eeprom_write_data((uint8_t *)&bdb, APP_BOND_DB_DATA_OFFSET, sizeof(struct bond_db), &bytes_written);
    ASSERT_ERROR(bytes_written == sizeof(struct bond_db));

    i2c_eeprom_release();
}
#endif

/**
 ****************************************************************************************
 * @brief Load Bond Database from external memory
 * @return none
 ****************************************************************************************
 */
static inline void bond_db_load_ext(void)
{
    #if defined (USER_CFG_APP_BOND_DB_USE_SPI_FLASH)
    bond_db_load_flash();
    #elif defined (USER_CFG_APP_BOND_DB_USE_I2C_EEPROM)
    bond_db_load_eeprom();
    #endif
}

/**
 ****************************************************************************************
 * @brief Store Bond Database to external memory
 * @param[in] scheduler_en  Only used if external memory is Flash
                            True: Enable rwip_scheduler while Flash is being erased
 *                          False: Do not enable rwip_scheduler. Blocking mode
 * @return none
 ****************************************************************************************
 */
static inline void bond_db_store_ext(bool scheduler_en)
{
    #if defined (USER_CFG_APP_BOND_DB_USE_SPI_FLASH)
    bond_db_store_flash(scheduler_en);
    #elif defined (USER_CFG_APP_BOND_DB_USE_I2C_EEPROM)
    bond_db_store_eeprom();
    #endif
}

/**
 ****************************************************************************************
 * @brief Store Bond data entry to external memory
 * @param[in] *data  Data to be stored
 * @param[in] *idx   Entry in the database
 * @return none
 ****************************************************************************************
 */
static void bond_db_store_at_idx(struct app_sec_bond_data_env_tag *data, int idx)
{
    bdb.valid_slot[idx] = BOND_DB_VALID_ENTRY;
    // Update the cache
    memcpy(&bdb.data[idx], data, sizeof(struct app_sec_bond_data_env_tag));
    // Store new bond data to external memory
    // In case of Flash (erase then write) enable the scheduler
    bond_db_store_ext(true);
}

/**
 ****************************************************************************************
 * @brief Clear bond data.
 * @return void
 ****************************************************************************************
 */
static void bond_db_clear(bool scheduler_en)
{
    memset((void *)&bdb, 0, sizeof(struct bond_db) ); // zero bond data
    bdb.start_hdr = BOND_DB_HEADER_START;
    bdb.end_hdr = BOND_DB_HEADER_END;
    // Store zero bond data to external memory
    // In case of Flash (erase then write) do not enable the scheduler
    bond_db_store_ext(scheduler_en);
}

/*
 * EXPOSED FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void bond_db_init(void)
{
    // Load bond data from the external memory resource
    bond_db_load_ext();

    // Simple check for garbage in memory (this also catches the 0xFF of cleared memory)
    if ((bdb.start_hdr != BOND_DB_HEADER_START) || (bdb.end_hdr != BOND_DB_HEADER_END))
    {
        bond_db_clear(false);
    }
}

uint8_t bond_db_get_size(void)
{
    return APP_BOND_DB_MAX_BONDED_PEERS;
}

void bond_db_add_entry(struct app_sec_bond_data_env_tag *data)
{
    bool first_empty_slot_found = false;
    uint8_t idx = 0;
    uint32_t min_timestamp;
    uint8_t slot_to_write = BOND_DB_SLOT_NOT_FOUND;

    for(idx = 0; idx < APP_BOND_DB_MAX_BONDED_PEERS; idx++)
    {
        // If current slot is not valid (is empty)
        if (bdb.valid_slot[idx] != BOND_DB_VALID_ENTRY)
        {
            // Check if an empty slot has already been found
            if (first_empty_slot_found == false)
            {
                // First empty slot has been found
                first_empty_slot_found = true;
                // Store empty slot to write
                slot_to_write = idx;
            }
        }
        // If current slot is not empty
        else
        {
            // Check if IRK is present in current slot and in new pairing data
            if ((bdb.data[idx].valid_keys & *(&data->valid_keys)) & RIRK_PRESENT)
            {
                // Check if stored IRK matches with new IRK
                if (memcmp(&data->rirk.irk, &bdb.data[idx].rirk.irk, sizeof(struct gap_sec_key)) == 0)
                {
                    // IRK matches, store this slot to be replaced and exit
                    slot_to_write = idx;
                    break;
                }
            }
            // If IRK is not present
            else
            {
                // Check if stored BD address matches with new BD address
                if (memcmp(&data->peer_bdaddr, &bdb.data[idx].peer_bdaddr, sizeof(struct gap_bdaddr)) == 0)
                {
                    // BD address matches, store this slot to be replaced and exit
                    slot_to_write = idx;
                    break;
                }
            }
        }
    }

    // If there is no available slot, find the least recently written slot to replace
    if (slot_to_write == BOND_DB_SLOT_NOT_FOUND)
    {
        min_timestamp = bdb.timestamp[0];
        slot_to_write = 0;
        for(idx = 1; idx < APP_BOND_DB_MAX_BONDED_PEERS; idx++)
        {
            // Find the slot with the oldest (minimum) timestamp
            if (bdb.timestamp[idx] < min_timestamp)
            {
                min_timestamp = bdb.timestamp[idx];
                slot_to_write = idx;
            }
        }
    }
    bdb.timestamp[slot_to_write] = bdb.timestamp_counter++;
    // Store bond db entry
    data->bdb_slot = slot_to_write;
    bond_db_store_at_idx(data, slot_to_write);
}

void bond_db_remove_entry(enum bdb_search_by_type search_type, enum bdb_remove_type remove_type,
                          void *search_param, uint8_t search_param_length)
{
    uint8_t idx = 0;
    uint8_t slot_found = BOND_DB_SLOT_NOT_FOUND;

    if (remove_type == REMOVE_ALL)
    {
        bond_db_clear(true);
        return;
    }

    if (search_type == SEARCH_BY_SLOT_TYPE)
    {
        slot_found = *((uint8_t *)search_param);
    }
    else
    {
        for(idx = 0; idx < APP_BOND_DB_MAX_BONDED_PEERS; idx++)
        {
            if (search_type == SEARCH_BY_EDIV_TYPE)
            {
                // Check if EDIV stored in the current slot and given EDIV match
                if ((memcmp(&(bdb.data[idx].ltk.ediv), search_param, search_param_length) == 0))
                {
                    slot_found = idx;
                    break;
                }
            }
            else if (search_type == SEARCH_BY_BDA_TYPE)
            {
                // Check if BD address stored in the current slot and given BD address match
                if ((memcmp(&(bdb.data[idx].peer_bdaddr.addr), search_param, search_param_length) == 0))
                {
                    slot_found = idx;
                    break;
                }
            }
            else if (search_type == SEARCH_BY_IRK_TYPE)
            {
                // Check if IRK stored in the current slot and given IRK match
                if (memcmp(&bdb.data[idx].rirk, search_param, search_param_length) == 0)
                {
                    slot_found = idx;
                    break;
                }
            }
        }
    }

    // Check if a valid slot has been found
    if (slot_found != BOND_DB_SLOT_NOT_FOUND)
    {
        if (remove_type == REMOVE_THIS_ENTRY)
        {
            // Remove entry from cache
            memset((void *)&bdb.data[slot_found], 0, sizeof(struct app_sec_bond_data_env_tag));
            bdb.timestamp[slot_found] = 0;
            bdb.valid_slot[slot_found] = BOND_DB_EMPTY_SLOT;
        }
        else
        {
            // If remove_all_but_this is true, remove all other entries from cache
            for(idx = 0; idx < APP_BOND_DB_MAX_BONDED_PEERS; idx++)
            {
                if (idx != slot_found)
                {
                    memset((void *)&bdb.data[idx], 0, sizeof(struct app_sec_bond_data_env_tag));
                    bdb.timestamp[idx] = 0;
                    bdb.valid_slot[idx] = BOND_DB_EMPTY_SLOT;
                }
            }
        }
        // Store the updated cache to the external non volatile memory
        bond_db_store_ext(true);
    }
}

const struct app_sec_bond_data_env_tag* bond_db_search_entry(enum bdb_search_by_type search_type,
                                                             void *search_param,
                                                             uint8_t search_param_length)
{
    uint8_t idx = 0;
    struct app_sec_bond_data_env_tag *found_data = NULL;

    for(idx = 0; idx < APP_BOND_DB_MAX_BONDED_PEERS; idx++)
    {
        // Check if EDIV stored in the current slot and given EDIV match
        if (search_type == SEARCH_BY_EDIV_TYPE)
        {
            if ((memcmp(&(bdb.data[idx].ltk.ediv), search_param, search_param_length) == 0))
            {
                found_data = &bdb.data[idx];
                break;
            }
        }
        else
        {
            // Calculate and store the BD address type of the current slot
            uint8_t bdaddr_type;
            bdaddr_type = app_get_address_type(bdb.data[idx].peer_bdaddr.addr_type, bdb.data[idx].peer_bdaddr.addr);

            if (search_type == SEARCH_BY_BDA_TYPE)
            {
                // Compare only if BD address type is Public or Random Static, else skip
                if ((bdaddr_type == APP_PUBLIC_ADDR_TYPE) || (bdaddr_type == APP_RANDOM_STATIC_ADDR_TYPE))
                {
                    // Check if bond_db BD address and given BD address match
                    if ((memcmp(&(bdb.data[idx].peer_bdaddr.addr), search_param, search_param_length) == 0))
                    {
                        found_data = &bdb.data[idx];
                        break;
                    }
                }
            }
            else if (search_type == SEARCH_BY_IRK_TYPE)
            {
                // Compare only if BD address type is Random Private Resolvable, else skip
                if (bdaddr_type == APP_RANDOM_PRIVATE_RESOLV_ADDR_TYPE)
                {
                    // Check if bond_db IRK and given IRK match
                    if (memcmp(&bdb.data[idx].rirk, search_param, search_param_length) == 0)
                    {
                        found_data = &bdb.data[idx];
                        break;
                    }
                }
            }
        }
    }

    return found_data;
}

uint8_t bond_db_get_stored_irks(struct gap_sec_key *valid_irk_irray)
{
    uint8_t idx = 0;
    uint8_t nb_key = 0;
    uint8_t bdaddr_type;

    // Search DB to find the entries which contain an IRK
    for(idx = 0; idx < APP_BOND_DB_MAX_BONDED_PEERS; idx++)
    {
        // Check if BD address type is Random Private Resolvable, else skip
        bdaddr_type = app_get_address_type(bdb.data[idx].peer_bdaddr.addr_type, bdb.data[idx].peer_bdaddr.addr);
        if (bdaddr_type == APP_RANDOM_PRIVATE_RESOLV_ADDR_TYPE)
        {
            // Copy IRK to the IRK array
            memcpy(&valid_irk_irray[nb_key], &bdb.data[idx].rirk.irk, sizeof(struct gap_sec_key));
            nb_key++;
        }
    }

    return nb_key;
}

#endif // (BLE_APP_SEC)

/// @} APP_BOND_DB
