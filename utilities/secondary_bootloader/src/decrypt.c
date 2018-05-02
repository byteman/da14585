/**
 ****************************************************************************************
 *
 * @file decrypt.c
 *
 * @brief  Decrypt API for secondary booloader
 *
 * Copyright (C) 2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#include "sw_aes.h"
#include "uart_booter.h"

AES_CTX ctx;
const uint8_t Key[16]= {0x06,0xa9,0x21,0x40,0x36,0xb8,0xa1,0x5b,0x51,0x2e,0x03,0xd5,0x34,0x12,0x00,0x06};
const uint8_t IV[16] = {0x3d,0xaf,0xba,0x42,0x9d,0x9e,0xb4,0x30,0xb4,0x22,0xda,0x80,0x2c,0x9f,0xac,0x41};

/**
****************************************************************************************
* @brief Decrypts the encryped image in place.
* @param[in] nsize: the size of the encrypted image. It is expected to be a multiple of AES_BLOCKSIZE.
****************************************************************************************
*/
void Decrypt_Image(int nsize)
{
    AES_set_key(&ctx,Key,IV,AES_MODE_128);
    AES_convert_key(&ctx);
    AES_cbc_decrypt(&ctx, SYSRAM_BASE_ADDRESS,SYSRAM_BASE_ADDRESS,nsize);
}
