/**
 ****************************************************************************************
 *
 * @file system_settings.h
 *
 * @brief DA1458x RF preferred settings.
 *
 * Copyright (C) 2012-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef _SYSTEM_SETTINGS_H_
#define _SYSTEM_SETTINGS_H_

/*
 * Radio preferred settings
 ****************************************************************************************
 */

#define PREF_BLE_RADIOPWRUPDN_REG           0x754054C
#define PREF_RF_ENABLE_CONFIG13_REG         0xD030
#define PREF_RF_DC_OFFSET_CTRL3_REG         0xE6EB
#define PREF_RF_VCOCAL_CTRL_REG             0x63

#endif // _SYSTEM_SETTINGS_H_
