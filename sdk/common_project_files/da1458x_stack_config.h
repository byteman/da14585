/**
 ****************************************************************************************
 *
 * @file da1458x_stack_config.h
 *
 * @brief RW stack configuration file.
 *
 * Copyright (C) 2014-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef DA1458X_STACK_CONFIG_H_
#define DA1458X_STACK_CONFIG_H_

/////////////////////////////////////////////////////////////
/* Do not alter */

#define CFG_EMB
#define CFG_HOST
#define CFG_ALLROLES        1
#define CFG_GTL
#define CFG_BLE
#define CFG_EXT_DB
#define CFG_CON             8
#define CFG_SECURITY_ON     1
#define CFG_ATTC
#define CFG_ATTS
#define CFG_PRF
#define CFG_NB_PRF          32
#define CFG_H4TL

#define CFG_CHNL_ASSESS

/* Enable power optimizations */
#define CFG_POWER_OPTIMIZATIONS

/*Misc*/
#define __NO_EMBEDDED_ASM

/*
 * Enable RF DIAG interrupt
 * NOTE: to be enabled for PLL LUT and production test
 */
#define CFG_RF_DIAG_INT                  (1)

/****************************************************************************************************************/
/* Use the 580 PLL LUT RF calibration or the 585 VCO coarse RF calibration.                                     */
/* - DEFINE   : 580 PLL LUT RF calibration.                                                                     */
/* - UNDEFINE : 585 VCO coarse RF calibration.                                                                  */
/****************************************************************************************************************/
#undef CFG_LUT_PATCH

/*FPGA*/
#define FPGA    (0)

#if (FPGA)
    #define XTAL_32000HZ
#endif

/*Radio interface*/
#if (FPGA)
    #define RADIO_RIPPLE        1
    #define RIPPLE_ID           66
    #define RADIO_585           0
#else
    #define RADIO_RIPPLE        0
    #define RADIO_585           1
#endif

#endif // DA1458X_STACK_CONFIG_H_
