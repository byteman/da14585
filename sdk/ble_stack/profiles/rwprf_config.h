/**
 ****************************************************************************************
 *
 * @file rwprf_config.h
 *
 * @brief Header file - Profile Configuration
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 ****************************************************************************************
 */


#ifndef _RWPRF_CONFIG_H_
#define _RWPRF_CONFIG_H_


/**
 ****************************************************************************************
 * @addtogroup PRF_CONFIG
 * @ingroup PROFILE
 * @brief Profile configuration
 *
 * @{
 ****************************************************************************************
 */

//ATT DB,Testing and Qualification related flags
#if (BLE_CENTRAL || BLE_PERIPHERAL)
/// Proximity Profile Monitor Role
#if defined(CFG_PRF_PXPM)
#define BLE_PROX_MONITOR        1
#else
#define BLE_PROX_MONITOR        0
#endif // defined(CFG_PRF_PXPM)

/// Proximity Profile Reporter Role
#if defined(CFG_PRF_PXPR)
#define BLE_PROX_REPORTER       1
#else
#define BLE_PROX_REPORTER       0
#endif // defined(CFG_PRF_PXPR)

///Find Me Profile Locator role
#if defined(CFG_PRF_FMPL)
#define BLE_FINDME_LOCATOR      1
#else
#define BLE_FINDME_LOCATOR      0
#endif // defined(CFG_PRF_FMPL)

///Find Me Profile Target role
#if defined(CFG_PRF_FMPT)
#define BLE_FINDME_TARGET       1
#else
#define BLE_FINDME_TARGET       0
#endif // defined(CFG_PRF_FMPT)

///Health Thermometer Profile Collector Role
#if defined(CFG_PRF_HTPC)
#define BLE_HT_COLLECTOR        1
#else
#define BLE_HT_COLLECTOR        0
#endif // defined(CFG_PRF_HTPC)

///Health Thermometer Profile Thermometer Role
#if defined(CFG_PRF_HTPT)
#define BLE_HT_THERMOM          1
#else
#define BLE_HT_THERMOM          0
#endif // defined(CFG_PRF_HTPT)

///Device Information Service Client Role
#if defined(CFG_PRF_DISC)
#define BLE_DIS_CLIENT          1
#else
#define BLE_DIS_CLIENT          0
#endif // defined(CFG_PRF_DISC)

///Device Information Service Server Role
#if defined(CFG_PRF_DISS)
#define BLE_DIS_SERVER          1
#else
#define BLE_DIS_SERVER          0
#endif // defined(CFG_PRF_DISS)

///Blood Pressure Profile Collector Role
#if defined(CFG_PRF_BLPC)
#define BLE_BP_COLLECTOR        1
#else
#define BLE_BP_COLLECTOR        0
#endif // defined(CFG_PRF_BLPC)

///Blood Pressure Profile Sensor Role
#if defined(CFG_PRF_BLPS)
#define BLE_BP_SENSOR           1
#else
#define BLE_BP_SENSOR           0
#endif // defined(CFG_PRF_BLPS)

///Time Profile Client Role
#if defined(CFG_PRF_TIPC)
#define BLE_TIP_CLIENT          1
#else
#define BLE_TIP_CLIENT          0
#endif // defined(CFG_PRF_TIPC)

///Time Profile Server Role
#if defined(CFG_PRF_TIPS)
#define BLE_TIP_SERVER          1
#else
#define BLE_TIP_SERVER          0
#endif // defined(CFG_PRF_TIPS)

///Heart Rate Profile Collector Role
#if defined(CFG_PRF_HRPC)
#define BLE_HR_COLLECTOR        1
#else
#define BLE_HR_COLLECTOR        0
#endif // defined(CFG_PRF_HRPC)

///Heart Rate Profile Sensor Role
#if defined(CFG_PRF_HRPS)
#define BLE_HR_SENSOR           1
#else
#define BLE_HR_SENSOR           0
#endif // defined(CFG_PRF_HRPS)

///Scan Parameter Profile Client Role
#if defined(CFG_PRF_SCPPC)
#define BLE_SP_CLIENT           1
#else
#define BLE_SP_CLIENT           0
#endif // defined(CFG_PRF_SCPPC)

///Scan Parameter Profile Server Role
#if defined(CFG_PRF_SCPPS)
#define BLE_SP_SERVER           1
#else
#define BLE_SP_SERVER           0
#endif // defined(CFG_PRF_SCPPS)

///Battery Service Client Role
#if defined(CFG_PRF_BASC)
#define BLE_BATT_CLIENT         1
#else
#define BLE_BATT_CLIENT         0
#endif // defined(CFG_PRF_BASC)

///Battery Service Server Role
#if defined(CFG_PRF_BASS)
#define BLE_BATT_SERVER         1
#else
#define BLE_BATT_SERVER         0
#endif // defined(CFG_PRF_BASS)

///Current Time Service Client Role
#if defined(CFG_PRF_CTSC)
#define BLE_CTS_CLIENT          1
#else
#define BLE_CTS_CLIENT          0
#endif // defined(CFG_PRF_CTSC)

///Current Time Service Server Role
#if defined(CFG_PRF_CTSS)
#define BLE_CTS_SERVER          1
#else
#define BLE_CTS_SERVER          0
#endif // defined(CFG_PRF_CTSS)

///HID Device Role
#if defined(CFG_PRF_HOGPD)
#define BLE_HID_DEVICE          1
#else
#define BLE_HID_DEVICE          0
#endif // defined(CFG_PRF_HOGPD)

///HID Boot Host Role
#if defined(CFG_PRF_HOGPBH)
#define BLE_HID_BOOT_HOST       1
#else
#define BLE_HID_BOOT_HOST       0
#endif // defined(CFG_PRF_HOGPBH)

///HID Report Host Role
#if defined(CFG_PRF_HOGPRH)
#define BLE_HID_REPORT_HOST     1
#else
#define BLE_HID_REPORT_HOST     0
#endif // defined(CFG_PRF_HOGPRH)

/// Glucose Profile Collector Role
#if defined(CFG_PRF_GLPC)
#define BLE_GL_COLLECTOR        1
#else
#define BLE_GL_COLLECTOR        0
#endif // defined(CFG_PRF_GLPC)

/// Glucose Profile Sensor Role
#if defined(CFG_PRF_GLPS)
#define BLE_GL_SENSOR           1
#else
#define BLE_GL_SENSOR           0
#endif // defined(CFG_PRF_GLPS)

/// Running Speed and Cadence Profile Collector Role
#if defined(CFG_PRF_RSCPC)
#define BLE_RSC_COLLECTOR       1
#else
#define BLE_RSC_COLLECTOR       0
#endif // defined(CFG_PRF_RSCPC)

/// Running Speed and Cadence Profile Server Role
#if defined(CFG_PRF_RSCPS)
#define BLE_RSC_SENSOR          1
#else
#define BLE_RSC_SENSOR          0
#endif // defined(CFG_PRF_RSCPS)

/// Cycling Speed and Cadence Profile Collector Role
#if defined(CFG_PRF_CSCPC)
#define BLE_CSC_COLLECTOR       1
#else
#define BLE_CSC_COLLECTOR       0
#endif // defined(CFG_PRF_CSCPC)

/// Cycling Speed and Cadence Profile Server Role
#if defined(CFG_PRF_CSCPS)
#define BLE_CSC_SENSOR          1
#else
#define BLE_CSC_SENSOR          0
#endif // defined(CFG_PRF_CSCPS)

/// Cycling Power Profile Collector Role
#if defined(CFG_PRF_CPPC)
#define BLE_CP_COLLECTOR        1
#else
#define BLE_CP_COLLECTOR        0
#endif // defined (CFG_PRF_CPPC)

/// Cycling Power Profile Server Role
#if defined(CFG_PRF_CPPS)
#define BLE_CP_SENSOR            1
#else
#define BLE_CP_SENSOR            0
#endif // defined (CFG_PRF_CPPS)

/// Location and Navigation Profile Collector Role
#if defined(CFG_PRF_LANC)
#define BLE_LN_COLLECTOR        1
#else
#define BLE_LN_COLLECTOR        0
#endif // defined (CFG_PRF_LANC)

/// Location and Navigation Profile Server Role
#if defined(CFG_PRF_LANS)
#define BLE_LN_SENSOR            1
#else
#define BLE_LN_SENSOR            0
#endif // defined (CFG_PRF_LANS)

/// Alert Notification Profile Client Role
#if defined(CFG_PRF_ANPC)
#define BLE_AN_CLIENT           1
#else
#define BLE_AN_CLIENT           0
#endif // defined(CFG_PRF_ANPC)

/// Alert Notification Profile Server Role
#if defined(CFG_PRF_ANPS)
#define BLE_AN_SERVER           1
#else
#define BLE_AN_SERVER           0
#endif // defined(CFG_PRF_ANPS)

/// Phone Alert Status Profile Client Role
#if defined(CFG_PRF_PASPC)
#define BLE_PAS_CLIENT          1
#else
#define BLE_PAS_CLIENT          0
#endif // defined(CFG_PRF_PASPC)

/// Phone Alert Status Profile Server Role
#if defined(CFG_PRF_PASPS)
#define BLE_PAS_SERVER          1
#else
#define BLE_PAS_SERVER          0
#endif // defined(CFG_PRF_PASPS)

/// Bond Management Service Client Role
#if defined(CFG_PRF_BMSC)
#define BLE_BMS_CLIENT          1
#else
#define BLE_BMS_CLIENT          0
#endif // defined(CFG_PRF_BMSC)

/// Bond Management Service Server Role
#if defined(CFG_PRF_BMSS)
#define BLE_BMS_SERVER          1
#else
#define BLE_BMS_SERVER          0
#endif // defined(CFG_PRF_BMSS)

/// Apple Notification Center Service Client Role
#if defined(CFG_PRF_ANCC)
#define BLE_ANC_CLIENT      1
#else
#define BLE_ANC_CLIENT      0
#endif //defined(CFG_PRF_ANCC)

/// Body Composition Service Client Role
#if defined(CFG_PRF_BCSC)
#define BLE_BCS_CLIENT          1
#else
#define BLE_BCS_CLIENT          0
#endif // defined(CFG_PRF_BCSC)

///Weight Scale Service Collector Role
#if defined(CFG_PRF_WSSC)
#define BLE_WSS_COLLECTOR          1
#else
#define BLE_WSS_COLLECTOR          0
#endif // defined(CFG_PRF_WSSC)

///Weight Scale Service Server Role
#if defined(CFG_PRF_WSSS)
#define BLE_WSS_SERVER          1
#else
#define BLE_WSS_SERVER          0
#endif // defined(CFG_PRF_WSSS)

/// Body Composition Service Server Role
#if defined(CFG_PRF_BCSS)
#define BLE_BCS_SERVER          1
#else
#define BLE_BCS_SERVER          0
#endif // defined(CFG_PRF_BCSS)

/// User Data Service Client Role
#if defined(CFG_PRF_UDSC)
#define BLE_UDS_CLIENT          1
#else
#define BLE_UDS_CLIENT          0
#endif // defined(CFG_PRF_UDSC)

/// User Data Service Server Role
#if defined(CFG_PRF_UDSS)
#define BLE_UDS_SERVER          1
#else
#define BLE_UDS_SERVER          0
#endif // defined(CFG_PRF_UDSS)

/// Software Patching Over the Air Receiver Role
#if defined(CFG_PRF_SUOTAR)
#define BLE_SUOTA_RECEIVER      1
#else
#define BLE_SUOTA_RECEIVER      0
#endif // defined(CFG_PRF_SUOTAR)

/// Custom1 server
#if defined(CFG_PRF_CUST1)
#define BLE_CUSTOM1_SERVER      1
#else
#define BLE_CUSTOM1_SERVER      0
#endif // defined(CFG_PRF_CUST1)

/// Custom2 server
#if defined(CFG_PRF_CUST2)
#define BLE_CUSTOM2_SERVER      1
#else
#define BLE_CUSTOM2_SERVER      0
#endif // defined(CFG_PRF_CUST2)

#define BLE_CUSTOM_SERVER (BLE_CUSTOM1_SERVER || BLE_CUSTOM2_SERVER)

/// BLE_CLIENT_PRF indicates if at least one client profile is present
#if (BLE_PROX_MONITOR || BLE_FINDME_LOCATOR || BLE_HT_COLLECTOR || BLE_BP_COLLECTOR \
        || BLE_HR_COLLECTOR || BLE_DIS_CLIENT || BLE_TIP_CLIENT || BLE_SP_CLIENT \
        || BLE_BATT_CLIENT || BLE_GL_COLLECTOR || BLE_HID_BOOT_HOST || BLE_HID_REPORT_HOST \
        || BLE_RSC_COLLECTOR || BLE_CSC_COLLECTOR || BLE_CP_COLLECTOR || BLE_LN_COLLECTOR || BLE_AN_CLIENT \
        || BLE_PAS_CLIENT || BLE_BMS_CLIENT || BLE_ANC_CLIENT || BLE_BCS_CLIENT || BLE_WSS_COLLECTOR \
        || BLE_UDS_CLIENT || BLE_CTS_CLIENT)

#define BLE_CLIENT_PRF          1
#else
#define BLE_CLIENT_PRF          0
#endif //(BLE_PROX_MONITOR || BLE_FINDME_LOCATOR ...)

/// BLE_SERVER_PRF indicates if at least one server profile is present
#if (BLE_PROX_REPORTER || BLE_FINDME_TARGET || BLE_HT_THERMOM || BLE_BP_SENSOR \
        || BLE_TIP_SERVER || BLE_HR_SENSOR || BLE_DIS_SERVER || BLE_SP_SERVER \
        || BLE_BATT_SERVER || BLE_HID_DEVICE || BLE_GL_SENSOR || BLE_RSC_SENSOR  \
        || BLE_CSC_SENSOR || BLE_CP_SENSOR || BLE_LN_SENSOR || BLE_AN_SERVER \
        || BLE_PAS_SERVER || BLE_BMS_SERVER || BLE_BCS_SERVER || BLE_WSS_SERVER \
        || BLE_UDS_SERVER || BLE_CTS_SERVER || BLE_SUOTA_RECEIVER || BLE_CUSTOM_SERVER)

#define BLE_SERVER_PRF          1
#else
#define BLE_SERVER_PRF          0
#endif //(BLE_PROX_REPORTER || BLE_FINDME_TARGET ...)

//Force ATT parts depending on profile roles or compile options
/// Attribute Client
#if (BLE_CLIENT_PRF)
#define BLE_ATTC                    1
#endif //(BLE_CLIENT_PRF)

/// Attribute Server
#if (BLE_SERVER_PRF)
#define BLE_ATTS                    1
#endif //(BLE_SERVER_PRF)


#elif (BLE_OBSERVER || BLE_BROADCASTER)
/// Proximity Profile Monitor Role
#define BLE_PROX_MONITOR            0
/// Proximity Profile Reporter Role
#define BLE_PROX_REPORTER           0
///Find Me Profile Locator role
#define BLE_FINDME_LOCATOR          0
///Find Me Profile Target role
#define BLE_FINDME_TARGET           0
///Health Thermometer Profile Collector Role
#define BLE_HT_COLLECTOR            0
///Health Thermometer Profile Thermometer Role
#define BLE_HT_THERMOM              0
///Blood Pressure Profile Collector Role
#define BLE_BP_COLLECTOR            0
///Blood Pressure Profile Sensor Role
#define BLE_BP_SENSOR               0
///Heart Rate Profile Collector Role
#define BLE_HR_COLLECTOR            0
///Heart Rate Profile Sensor Role
#define BLE_HR_SENSOR               0
///Time Profile Client Role
#define BLE_TIP_CLIENT              0
///Time Profile Server Role
#define BLE_TIP_SERVER              0
/// Device Information Service Client Role
#define BLE_DIS_CLIENT              0
/// Device Information Service Server Role
#define BLE_DIS_SERVER              0
/// Scan Parameter Profile Client Role
#define BLE_SP_CLIENT               0
/// Scan Parameter Profile Server Role
#define BLE_SP_SERVER               0
/// Battery Service Client Role
#define BLE_BATT_CLIENT             0
/// Battery Service Server Role
#define BLE_BATT_SERVER             0
/// HID Device Role
#define BLE_HID_DEVICE              0
/// HID Boot Host Role
#define BLE_HID_BOOT_HOST           0
/// HID Report Host Role
#define BLE_HID_REPORT_HOST         0
/// Glucose Profile Collector Role
#define BLE_GL_COLLECTOR            0
/// Glucose Profile Sensor Role
#define BLE_GL_SENSOR               0
/// Running Speed and Cadence Collector Role
#define BLE_RSC_COLLECTOR           0
/// Running Speed and Cadence Server Role
#define BLE_RSC_SENSOR              0
/// Cycling Speed and Cadence Collector Role
#define BLE_CSC_COLLECTOR           0
/// Cycling Speed and Cadence Server Role
#define BLE_CSC_SENSOR              0
/// Cycling Power Collector Role
#define BLE_CP_COLLECTOR            0
/// Cycling Power Server Role
#define BLE_CP_SENSOR               0
/// Location and Navigation Collector Role
#define BLE_LN_COLLECTOR            0
/// Location and Navigation Server Role
#define BLE_LN_SENSOR               0
/// Alert Notification Client Role
#define BLE_AN_CLIENT               0
/// Alert Notification Server Role
#define BLE_AN_SERVER               0
/// Phone Alert Status Client Role
#define BLE_PAS_CLIENT              0
/// Phone Alert Status Server Role
#define BLE_PAS_SERVER              0
/// Bond Management Service Client Role
#define BLE_BMS_CLIENT              0
/// Bond Management Service Server Role
#define BLE_BMS_SERVER              0
/// Body Composition Service Client Role
#define BLE_BCS_CLIENT              0
/// Body Composition Service Server Role
#define BLE_BCS_SERVER              0
/// User Data Service Client Role
#define BLE_UDS_CLIENT              0
/// User Data Service Server Role
#define BLE_UDS_SERVER              0
/// SUOTA Receiver Role
#define BLE_SUOTA_RECEIVER          0
/// Custom Service Server Role
#define BLE_CUSTOM_SERVER           0

//Force ATT parts to 0
/// External database management
#define BLE_EXT_ATTS_DB             0
/// Profile Server
#define BLE_SERVER_PRF              0
/// Profile Client
#define BLE_CLIENT_PRF              0
#endif //(BLE_OBSERVER || BLE_BROADCASTER)


/// @} PRF_CONFIG

#endif /* _RWPRF_CONFIG_H_ */
