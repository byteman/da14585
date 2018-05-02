/**
 ****************************************************************************************
 *
 * @file otp_hdr.h
 *
 * @brief Definition of OTP header field offsets and addresses.
 *
 * Copyright (C) 2016-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef OTP_HDR_H
#define OTP_HDR_H

#include "arch.h"


/*
 *  OTP header field offsets from OTP memory starting address
 */
#define OTP_HDR_BASE_OFFSET                 (0xFE00)

#define OTP_HDR_SWD_ENABLE_OFFSET           (0xFFF8) // SWD enable flag
#define OTP_HDR_OTP_DMA_LENGTH_OFFSET       (0xFFF0) // OTP DMA length
#define OTP_HDR_RESERVED1_OFFSET            (0xFFE8) // Reserved
#define OTP_HDR_BDADDR_OFFSET               (0xFFA8) // Bluetooth Device Address
#define OTP_HDR_SIGN_ALG_SEL_OFFSET         (0xFFA0) // Signature Algorithm Selector
#define OTP_HDR_SIGN_CUST_CODE_OFFSET       (0xFF28) // Signature of Customer Code
#define OTP_HDR_TRIM_VCO_OFFSET             (0xFF20) // Trim value for the VCO
#define OTP_HDR_TRIM_XTAL16M_OFFSET         (0xFF18) // Trim value for the XTAL16M oscillator
#define OTP_HDR_TRIM_RC16M_OFFSET           (0xFF10) // Trim value for the RC16M oscillator
#define OTP_HDR_TRIM_BANDGAP_OFFSET         (0xFF08) // Trim value for the BandGap
#define OTP_HDR_TRIM_RFIO_CAP_OFFSET        (0xFF00) // Trim value for the RFIO capacitance
#define OTP_HDR_TRIM_LNA_OFFSET             (0xFEF8) // Trim value for the LNA
#define OTP_HDR_CALIBRATION_FLAGS_OFFSET    (0xFEF0) // Calibration Flags
#define OTP_HDR_SLEEP_CLK_SOURCE_OFFSET     (0xFEE8) // Sleep Clock Source Flag
#define OTP_HDR_PACKAGE_OFFSET              (0xFEE0) // Package Flag
#define OTP_HDR_RESERVED2_OFFSET            (0xFEC8) // Reserved
#define OTP_HDR_BOOT_MAPPING_OFFSET         (0xFEC0) // Boot specific mapping
#define OTP_HDR_UART_STX_TO_OFFSET          (0xFEB8) // UART STX Timeout
#define OTP_HDR_OTP_CONTROL_OFFSET          (0xFEB0) // OTP control value
#define OTP_HDR_CUSTOMER_FIELD_OFFSET       (0xFE20) // Customer Specific Field (21 64-bit words)
#define OTP_HDR_CRC_TRIM_CAL_OFFSET         (0xFE18) // CRC for Trim and Calibration values
#define OTP_HDR_IQ_TRIM_VALUE_OFFSET        (0xFE10) // IQ Trim value
#define OTP_HDR_APP_PROG_FLAG2_OFFSET       (0xFE08) // Application Programmed Flag #2
#define OTP_HDR_APP_PROG_FLAG1_OFFSET       (0xFE00) // Application Programmed Flag #1

/*
 *  OTP header field memory addresses
 */
#define OTP_HDR_BASE_ADDR               (MEMORY_OTP_BASE + OTP_HDR_BASE_OFFSET)

#define OTP_HDR_SWD_ENABLE_ADDR         (MEMORY_OTP_BASE + OTP_HDR_SWD_ENABLE_OFFSET)
#define OTP_HDR_OTP_DMA_LENGTH_ADDR     (MEMORY_OTP_BASE + OTP_HDR_OTP_DMA_LENGTH_OFFSET)
#define OTP_HDR_RESERVED1_ADDR          (MEMORY_OTP_BASE + OTP_HDR_RESERVED1_OFFSET)
#define OTP_HDR_BDADDR_ADDR             (MEMORY_OTP_BASE + OTP_HDR_BDADDR_OFFSET)
#define OTP_HDR_SIGN_ALG_SEL_ADDR       (MEMORY_OTP_BASE + OTP_HDR_SIGN_ALG_SEL_OFFSET)
#define OTP_HDR_SIGN_CUST_CODE_ADDR     (MEMORY_OTP_BASE + OTP_HDR_SIGN_CUST_CODE_OFFSET)
#define OTP_HDR_TRIM_VCO_ADDR           (MEMORY_OTP_BASE + OTP_HDR_TRIM_VCO_OFFSET)
#define OTP_HDR_TRIM_XTAL16M_ADDR       (MEMORY_OTP_BASE + OTP_HDR_TRIM_XTAL16M_OFFSET)
#define OTP_HDR_TRIM_RC16M_ADDR         (MEMORY_OTP_BASE + OTP_HDR_TRIM_RC16M_OFFSET)
#define OTP_HDR_TRIM_BANDGAP_ADDR       (MEMORY_OTP_BASE + OTP_HDR_TRIM_BANDGAP_OFFSET)
#define OTP_HDR_TRIM_RFIO_CAP_ADDR      (MEMORY_OTP_BASE + OTP_HDR_TRIM_RFIO_CAP_OFFSET)
#define OTP_HDR_TRIM_LNA_ADDR           (MEMORY_OTP_BASE + OTP_HDR_TRIM_LNA_OFFSET)
#define OTP_HDR_CALIBRATION_FLAGS_ADDR  (MEMORY_OTP_BASE + OTP_HDR_CALIBRATION_FLAGS_OFFSET)
#define OTP_HDR_SLEEP_CLK_SOURCE_ADDR   (MEMORY_OTP_BASE + OTP_HDR_SLEEP_CLK_SOURCE_OFFSET)
#define OTP_HDR_PACKAGE_ADDR            (MEMORY_OTP_BASE + OTP_HDR_PACKAGE_OFFSET)
#define OTP_HDR_RESERVED2_ADDR          (MEMORY_OTP_BASE + OTP_HDR_RESERVED2_OFFSET)
#define OTP_HDR_BOOT_MAPPING_ADDR       (MEMORY_OTP_BASE + OTP_HDR_BOOT_MAPPING_OFFSET)
#define OTP_HDR_UART_STX_TO_ADDR        (MEMORY_OTP_BASE + OTP_HDR_UART_STX_TO_OFFSET)
#define OTP_HDR_OTP_CONTROL_ADDR        (MEMORY_OTP_BASE + OTP_HDR_OTP_CONTROL_OFFSET)
#define OTP_HDR_CUSTOMER_FIELD_ADDR     (MEMORY_OTP_BASE + OTP_HDR_CUSTOMER_FIELD_OFFSET)
#define OTP_HDR_CRC_TRIM_CAL_ADDR       (MEMORY_OTP_BASE + OTP_HDR_CRC_TRIM_CAL_OFFSET)
#define OTP_HDR_IQ_TRIM_VALUE_ADDR      (MEMORY_OTP_BASE + OTP_HDR_IQ_TRIM_VALUE_OFFSET)
#define OTP_HDR_APP_PROG_FLAG2_ADDR     (MEMORY_OTP_BASE + OTP_HDR_APP_PROG_FLAG2_OFFSET)
#define OTP_HDR_APP_PROG_FLAG1_ADDR     (MEMORY_OTP_BASE + OTP_HDR_APP_PROG_FLAG1_OFFSET)


#endif //OTP_HDR_H
