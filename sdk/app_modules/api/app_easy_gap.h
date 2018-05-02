/**
 ****************************************************************************************
 *
 * @file app_easy_gap.h
 *
 * @brief Easy GAP API.
 *
 * Copyright (C) 2015-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef _APP_EASY_GAP_H_
#define _APP_EASY_GAP_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#if (BLE_APP_PRESENT)

#include <stdint.h>          // Standard Integer Definition
#include "co_bt.h"           // Common BT Definitions
#include "arch.h"            // Platform Definitions
#include "gapc.h"

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Connection handle to connection Id transformation.
 * @param[in] conhdl    Connection handle number.
 * @return Connection Id number
 ****************************************************************************************
 */
__INLINE uint8_t conhdl_to_conidx(uint16_t conhdl)
{
    return (gapc_get_conidx(conhdl));
}

/**
 ****************************************************************************************
 * @brief Connection Id to connection handle transformation.
 * @param[in] conidx    Connection Id number.
 * @return Connection handle number
 ****************************************************************************************
 */
__INLINE uint16_t conidx_to_conhdl(uint8_t conidx)
{
    return (gapc_get_conhdl(conidx));
}

/**
 ****************************************************************************************
 * @brief Connection handle to connection Id transformation.
 * @return Connection Id number
 ****************************************************************************************
 */
int8_t active_conhdl_to_conidx(uint16_t conhdl);

/**
 ****************************************************************************************
 * @brief Connection Id to connection handle transformation
 * @return Connection handle number
 ****************************************************************************************
 */
int16_t active_conidx_to_conhdl(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Send BLE disconnect command.
 * @param[in] conidx    Connection Id
 * @return void
 ****************************************************************************************
 */
void app_easy_gap_disconnect(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Send a connection confirmation message.
 * @param[in] conidx                    Connection Id number
 * @param[in] auth                      Authentication requirements (@see gap_auth)
 * @param[in] svc_changed_ind_enable    Service changed indication enable/disable
 * @return void
 ****************************************************************************************
 */
void app_easy_gap_confirm(uint8_t conidx, enum gap_auth auth, uint8_t svc_changed_ind_enable);

/**
 ****************************************************************************************
 * @brief Start advertising for undirected connection.
 * @return void
 ****************************************************************************************
 */
void app_easy_gap_undirected_advertise_start(void);

/**
 ****************************************************************************************
 * @brief Start advertising for directed connection.
 * @param[in] ldc_enable       Enable/disable low duty cycle mode. 
 *                                 - 0 = disabled
 *                                 - 1 = enabled
 * @return void
 ****************************************************************************************
 */
void app_easy_gap_directed_advertise_start(uint8_t ldc_enable);

/**
 ****************************************************************************************
 * @brief Start advertising for non-connectable peripheral.
 * @return void
 ****************************************************************************************
 */
void app_easy_gap_non_connectable_advertise_start(void);

/**
 ****************************************************************************************
 * @brief Stop advertising.
 * @return void
 ****************************************************************************************
 */
void app_easy_gap_advertise_stop(void);

/**
 ****************************************************************************************
 * @brief Start advertising for undirected connection with timeout start.
 * @param[in] delay            Delay of the timer
 * @param[in] timeout_callback Timeout callback function
 * @return void
 ****************************************************************************************
 */
void app_easy_gap_undirected_advertise_with_timeout_start(uint32_t delay, void (*timeout_callback)(void));

/**
 ****************************************************************************************
 * @brief Stop advertising with timeout stop.
 * @return void
 ****************************************************************************************
 */
void app_easy_gap_advertise_with_timeout_stop(void);

/**
 ****************************************************************************************
 * @brief Get non connectable advertising message with filled parameters.
 * @return gapm_start_advertise_cmd Pointer to advertising message
 ****************************************************************************************
*/
struct gapm_start_advertise_cmd* app_easy_gap_non_connectable_advertise_get_active(void);

/**
 ****************************************************************************************
 * @brief Get undirected connectable advertising message with filled parameters.
 * @return gapm_start_advertise_cmd Pointer to advertising message
 ****************************************************************************************
*/
struct gapm_start_advertise_cmd* app_easy_gap_undirected_advertise_get_active(void);

/**
 ****************************************************************************************
 * @brief Get directed connectable advertising message with filled parameters. It
 *        supports the low duty cycle directed advertising mode.
 * @param[in] ldc_enable       Enable/disable low duty cycle mode.
 *                                 - 0 = disabled
 *                                 - 1 = enabled
 * @return gapm_start_advertise_cmd Pointer to advertising message
 ****************************************************************************************
*/
struct gapm_start_advertise_cmd* app_easy_gap_directed_advertise_get_active(uint8_t ldc_enable);

/**
 ****************************************************************************************
 * @brief Send the command which updates the advertising or scan response data.
 * @param[in] update_adv_data               Buffer that holds the new advertising data.
 * @param[in] update_adv_data_len           Length of the advertising data buffer.
 * @param[in] update_scan_rsp_data          Buffer that holds the new scan response data.
 * @param[in] update_scan_rsp_data_len      Length of the scan response data buffer.
 * @return void
 ****************************************************************************************
 */
void app_easy_gap_update_adv_data(const uint8_t *update_adv_data,
                                  uint8_t update_adv_data_len,
                                  const uint8_t *update_scan_rsp_data,
                                  uint8_t update_scan_rsp_data_len);

/**
 ****************************************************************************************
 * @brief Param update start request.
 * @param[in] conidx    Connection Id number
 * @return void
 ****************************************************************************************
*/
void app_easy_gap_param_update_start(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Get param update request message with filled parameters.
 * @param[in] conidx    Connection Id number
 * @return gapc_param_update_cmd Pointer to param update request message
 ****************************************************************************************
*/
struct gapc_param_update_cmd* app_easy_gap_param_update_get_active(uint8_t conidx);

/**
 ****************************************************************************************
 * @brief Start connection.
 * @return void
 ****************************************************************************************
*/
void app_easy_gap_start_connection_to(void);

/**
 ****************************************************************************************
 * @brief Start direct connection to known peer.
 * @param[in] peer_addr_type    Peer to connect address type
 * @param[in] peer_addr         Peer to connect address
 * @param[in] intv              Connection interval
 * @return void
 ****************************************************************************************
*/
void app_easy_gap_start_connection_to_set(uint8_t peer_addr_type,
                                          const uint8_t *peer_addr, uint16_t intv);

/**
 ****************************************************************************************
 * @brief Get connection message with filled parameters
 * @return gapm_start_connection_cmd Pointer to connection message
 ****************************************************************************************
*/
struct gapm_start_connection_cmd* app_easy_gap_start_connection_to_get_active(void);

/**
 ****************************************************************************************
 * @brief Get device configuration message with filled parameters
 * @return gapm_set_dev_config_cmd Pointer to device configuration message
 ****************************************************************************************
*/
struct gapm_set_dev_config_cmd* app_easy_gap_dev_config_get_active(void);

/**
 ****************************************************************************************
 * @brief Configure device
 * @return void
 ****************************************************************************************
*/
void app_easy_gap_dev_configure(void);

/**
 ****************************************************************************************
 * @brief Set maximum transmission packet size and maximum packet transmission time to be 
 *        used for the connection. This command may be sent either from peripheral or 
 *        central.
 * @param[in] conidx            Connection Id number.
 * @param[in] tx_octets         Maximum transmission packet size.
 * @param[in] tx_time           Maximum packet transmission time.
 * @return void
 ****************************************************************************************
*/
void app_easy_gap_set_data_packet_length(uint8_t conidx, uint16_t tx_octets, uint16_t tx_time);

/// @} APP

#endif //(BLE_APP_PRESENT)

#endif // _APP_EASY_GAP_H_
