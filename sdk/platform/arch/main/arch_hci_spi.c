/**
 ****************************************************************************************
 *
 * @file arch_hci_spi.c
 *
 * @brief Contains spi api definition and function rwip_get_func_spi.
 *
 * Copyright (C) 2012-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */
 
 #include <stdint.h>
 #include "rwip.h"       // BLE initialization
 #include "spi_hci.h"
 
// Creation of SPI external interface api
const struct rwip_eif_api spi_api =
{
    spi_hci_read_func,
    spi_hci_write_func,
    spi_hci_flow_on_func,
    spi_hci_flow_off_func,
};

 
extern const struct rwip_eif_api* rwip_eif_get_func_spi(uint8_t type)
{
    const struct rwip_eif_api* ret = NULL;
    switch(type)
    {
        case RWIP_EIF_AHI:
        case RWIP_EIF_HCIC:
        {
            ret = &spi_api;
        }
        break;
        default:
        {
            ASSERT_ERR(0);
        }
        break;
    }
    return ret;
}
