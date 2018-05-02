/****************************************************************************************
 *
 * @file aes_api.h
 *
 * @brief AES header file.
 *
 * Copyright (C) 2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef _AES_API_H_
#define _AES_API_H_

#include <stdint.h>
#include "sw_aes.h"

#define GETU32(pt)     (((uint32_t)(pt)[0] << 24) ^ ((uint32_t)(pt)[1] << 16) ^ ((uint32_t)(pt)[2] <<  8) ^ ((uint32_t)(pt)[3]))
#define PUTU32(ct, st) { (ct)[0] = (uint8_t)((st) >> 24); (ct)[1] = (uint8_t)((st) >> 16); (ct)[2] = (uint8_t)((st) >>  8); (ct)[3] = (uint8_t)(st); }

typedef AES_CTX AES_KEY;

#define BLE_SAFE_MASK   0x0F
#define BLE_SYNC_MASK   0xF0

/**
 ****************************************************************************************
 * @brief AES set encrypt/decrypt key.
 * @param[in] userKey   The key data.
 * @param[in] bits      Key number of bits. Should be 128
 * @param[in] key       AES_KEY structure pointer.
 * @param[in] enc_dec   0 set decrypt key, 1 set encrypt key.
 * @return               0 if successfull,
 *                      -1 if userKey or key are NULL,
 *                      -2 if bits is not 128.
 ****************************************************************************************
 */
int aes_set_key(const uint8_t *userKey, const uint32_t bits, AES_KEY *key, uint8_t enc_dec);

/**
 ****************************************************************************************
 * @brief AES encrypt/decrypt block.
 * @param[in] in            The input data block (16bytes).
 * @param[in] out           The output data block (16bytes).
 * @param[in] key           The key data.
 * @param[in] enc_dec       AES_ENCRYPT for encryption, AES_DECRYPT for decryption.
 * @param[in] ble_flags     Used to specify whether the encryption/decryption
 *                          will be performed synchronously or asynchronously (message based)
 *                          also if ble_safe is specified in ble_flags rwip_schedule() will be called
 *                          to avoid missing any ble events.
 * @return                   0 if successfull,
 *                          -1 if the AES HW block is in use.
 ****************************************************************************************
 */
int aes_enc_dec(uint8_t *in, uint8_t *out, AES_KEY *key, uint8_t enc_dec, uint8_t ble_flags);

#endif  //_AES_API_H_
