/**
 ****************************************************************************************
 *
 * @file chacha20.h
 *
 * @brief chacha20 random generator implementation API.
 *
 * Copyright (C) 2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef _CHACHA20_H_
#define _CHACHA20_H_

#if (USE_CHACHA20_RAND)

/**
 ****************************************************************************************
 * @brief Initialize chacha20 seed
 * @param[in] key initialization value
 * @return none
 ****************************************************************************************
 */
void csprng_seed(const uint8_t key[16]);

/**
 ****************************************************************************************
 * @brief Get chacha20 random 32-bit number
 * @return Random number
 ****************************************************************************************
 */
uint32_t csprng_get_next_uint32(void);

#endif
#endif
