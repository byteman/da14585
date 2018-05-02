/**
 ****************************************************************************************
 *
 * @file jump_table.c
 *
 * @brief Jump table that holds function pointers and variables used in ROM code.
 *
 * Copyright (C) 2016-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */
#include <string.h>          // for mem* functions
#include <stdint.h>
#include "rwip_config.h"     // RW SW configuration
#include "co_version.h"
#include "rwip.h"            // RW definitions
#include "arch.h"            // Platform architecture definition
#include "em_map_ble.h"
#include "em_map.h"
#include "da1458x_scatter_config.h" // for BLE_CONNECTION_MAX_USER

#if (BLE_EMB_PRESENT)
#include "rwble.h"           // rwble definitions
#endif //BLE_EMB_PRESENT

#if (BLE_HOST_PRESENT)
#include "rwble_hl.h"        // BLE HL definitions
#include "gapc.h"
#include "smpc.h"
#include "gattc.h"
#include "attc.h"
#include "atts.h"
#include "l2cc.h"
#endif //BLE_HOST_PRESENT

#include "lld_sleep.h"       // definitions for sleep mode
#include "llc.h"
#include "plf.h"             // platform definition
#include "rf.h"              // RF definitions

#if (GTL_ITF)
#include "gtl.h"
#endif //GTL_ITF

#include "ke.h"              // kernel definition
#include "ke_event.h"        // kernel event
#include "ke_timer.h"        // definitions for timer
#include "ke_mem.h"          // kernel memory manager
#include "dbg.h"             // debug definition


#if ((BLE_APP_PRESENT) || ((BLE_HOST_PRESENT && (!GTL_ITF))))
#include "app.h"
#endif //BLE_APP_PRESENT

#include "nvds.h"

#if (USE_CHACHA20_RAND)
#include "chacha20.h"
#endif

/*
 * forward declarations
 ******************************/

extern void rf_init_func(struct rwip_rf_api *api);
extern void rf_reinit_func(void);
extern void uart_init_func(uint16_t baudr,uint8_t dlf_value, uint8_t mode );
extern bool uart_flow_off_func(void);
extern void uart_flow_on_func(void);
extern void uart_finish_transfers_func(void);
extern void uart_read_func(uint8_t *bufptr, uint32_t size, void (*callback) (uint8_t));
extern void uart_write_func(uint8_t *bufptr, uint32_t size, void (*callback) (uint8_t));
extern void UART_Handler_func(void);
extern void gtl_init_func(const struct rwip_eif_api* eif);
extern void gtl_eif_init_func(void);
extern void gtl_eif_read_start_func(void);
extern void gtl_eif_read_hdr_func(uint8_t len);
extern void gtl_eif_read_payl_func(uint16_t len, uint8_t* p_buf);
extern void gtl_eif_tx_done_func(uint8_t status);
extern void gtl_eif_rx_done_func(uint8_t status);
extern void h4tl_init_func(const struct rwip_eif_api* eif);
extern void h4tl_read_start_func(void);
extern void h4tl_read_hdr_func(uint8_t len);
extern void h4tl_read_payl_func(uint16_t len);
extern void h4tl_read_next_out_of_sync_func(void);
extern void h4tl_out_of_sync_func(void);
extern void h4tl_tx_done_func(uint8_t status);
extern void h4tl_rx_done_func(uint8_t status);
extern void ke_task_init_func(void);
extern void ke_timer_init_func(void);
extern void llm_encryption_done_func(void);

extern uint8_t nvds_get_func(uint8_t tag, nvds_tag_len_t * lengthPtr, uint8_t *buf);
extern uint8_t nvds_put_func(uint8_t tag, nvds_tag_len_t length, uint8_t *buf);
extern uint8_t nvds_del_func(uint8_t tag);
extern uint8_t nvds_init_func(uint8_t *base, uint32_t len);
extern const struct rwip_eif_api* rwip_eif_get_func(uint8_t type);

extern void platform_reset_func(uint32_t error);
extern void lld_sleep_compensate_func(void);
extern void lld_sleep_init_func(void);
extern uint32_t lld_sleep_us_2_lpcycles_func(uint32_t us);
extern uint32_t lld_sleep_lpcycles_2_us_func(uint32_t lpcycles);

extern void lld_test_stop_func(struct ea_elt_tag *elt);
extern struct ea_elt_tag *lld_test_mode_tx_func(struct co_buf_tx_node *txdesc, uint8_t tx_freq);
extern struct ea_elt_tag *lld_test_mode_rx_func(uint8_t rx_freq);
extern uint8_t smpc_check_param_func(struct l2cc_pdu *pdu);
extern void smpc_pdu_recv_func(uint8_t conidx, struct l2cc_pdu *pdu);

extern void prf_init_func(bool reset);
struct gapm_profile_task_add_cmd;
extern uint8_t prf_add_profile_func(struct gapm_profile_task_add_cmd * params, ke_task_id_t* prf_task);
extern void prf_create_func(uint8_t conidx);
extern void prf_cleanup_func(uint8_t conidx, uint8_t reason);
extern ke_task_id_t prf_get_id_from_task_func(ke_msg_id_t task);
extern ke_task_id_t prf_get_task_from_id_func(ke_msg_id_t id);

extern void SetSystemVars_func(void);

extern void dbg_init_func(void);
extern void dbg_platform_reset_complete_func(uint32_t error);
extern int hci_rd_local_supp_feats_cmd_handler_func(ke_msg_id_t const msgid, void const *param,
                                                    ke_task_id_t const dest_id, ke_task_id_t const src_id);

extern uint8_t l2cc_pdu_pack_func(struct l2cc_pdu *p_pdu, uint16_t *p_offset, uint16_t *p_length, uint8_t *p_buffer,
                                  uint8_t conidx, uint8_t *llid);
extern uint8_t l2cc_pdu_unpack_func(struct l2cc_pdu *p_pdu, uint16_t *p_offset, uint16_t *p_rem_len,
                                    const uint8_t *p_buffer, uint16_t pkt_length, uint8_t conidx, uint8_t llid);
extern void l2c_send_lecb_message_func(uint8_t conidx, uint16_t cid, uint16_t size);
extern uint16_t l2c_process_sdu_func(uint8_t conidx, struct l2cc_pdu *pdu);

extern int l2cc_pdu_recv_ind_handler_func(ke_msg_id_t const msgid, struct l2cc_pdu_recv_ind *ind,
                                          ke_task_id_t const dest_id, ke_task_id_t const src_id);
extern int gapc_lecb_connect_cfm_handler_func(ke_msg_id_t const msgid,
                                              struct gapc_lecb_connect_cfm *param,
                                              ke_task_id_t const dest_id, ke_task_id_t const src_id);
extern int atts_l2cc_pdu_recv_handler_func(uint8_t conidx, struct l2cc_pdu_recv_ind *param);
extern int attc_l2cc_pdu_recv_handler_func(uint8_t conidx, struct l2cc_pdu_recv_ind *param);

extern void crypto_init_func(void);
extern void llm_le_adv_report_ind_func(struct co_buf_rx_desc *rxdesc);
extern int PK_PointMult_func(const ec_point *A, const uint8_t k[ECDH_KEY_LEN], ec_point *dst);
extern void llm_p256_start_func(struct ke_msg * msg);
extern uint8_t llm_create_p256_key_func(uint8_t state, uint8_t *A, uint8_t *priv);
extern int llm_p256_req_handler_func(ke_msg_id_t const msgid,
                                     struct llm_p256_req const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id);
extern bool llc_le_length_effective_func(uint16_t conhdl);
extern void llc_le_length_conn_init_func(uint16_t conhdl);
extern void lld_data_tx_prog_func(struct lld_evt_tag *evt);
struct lld_evt_tag;
struct lld_data_ind;
extern void lld_data_tx_check_func(struct lld_evt_tag *evt, struct lld_data_ind *msg);
extern void llc_pdu_send_func(uint16_t conhdl, uint8_t length);
extern void llc_data_notif_func(uint16_t conhdl, bool bad, uint16_t *status, uint8_t chnl);

extern int dia_rand_func(void);
extern void dia_srand_func (unsigned int seed);
extern uint8_t ba431_get_rand_func(uint32_t id, uint8_t * dest, uint32_t byte_size);

extern bool modulationGainCalibration_func(int16_t average, int16_t length, int16_t modindex);
extern void DCoffsetCalibration_func(void);
extern void PllLF_IFF_MIX_CapCalibration_func(void);

extern void smpc_public_key_exchange_start_func(uint8_t idx);
extern void smpc_dhkey_calc_ind_func(uint8_t conidx);
extern void smpm_ecdh_key_create_func(struct ecdh_key_pair *keys);

/*
 * Heaps
 ****************************************************************************************
 */

#if (BLE_CONNECTION_MAX_USER == 1)
#define RWIP_HEAP_NON_RET_SIZE_USER     1024
#else
#define RWIP_HEAP_NON_RET_SIZE_USER     2048 //(1024 * BLE_CONNECTION_MAX_USER)
#endif

#define RWIP_HEAP_ENV_SIZE_USER         ((BLE_HEAP_ENV_SIZE+  BLEHL_HEAP_ENV_SIZE)  * BLE_CONNECTION_MAX_USER)

#define BLE_HEAP_MSG_SIZE_USER          (256 * (BLE_CONNECTION_MAX_USER+1) + 80 * (BLE_CONNECTION_MAX_USER) + 96 * (2*BLE_CONNECTION_MAX_USER+1))
#define BLEHL_HEAP_MSG_SIZE_USER        (256 + 256 * BLE_CONNECTION_MAX_USER)
#define RWIP_HEAP_MSG_SIZE_USER         ( BLE_HEAP_MSG_SIZE_USER  +  BLEHL_HEAP_MSG_SIZE_USER )


// Heap header size is 12 bytes
#define RWIP_HEAP_HEADER                (12 / sizeof(uint32_t)) //header size in uint32_t
// ceil(len/sizeof(uint32_t)) + RWIP_HEAP_HEADER
#define RWIP_CALC_HEAP_LEN(len)         ((((len) + (sizeof(uint32_t) - 1)) / sizeof(uint32_t)) + RWIP_HEAP_HEADER)

#define HEAP_HDR_LEN  (12) // in bytes

/// Not retained heap
#ifndef NON_RET_HEAP_SZ
    #define RWIP_HEAP_NON_RET_SIZE_JT       RWIP_HEAP_NON_RET_SIZE_USER // in bytes
#else
    #define RWIP_HEAP_NON_RET_SIZE_JT       NON_RET_HEAP_SZ             // in bytes
#endif

/// Env Heap
#ifndef ENV_HEAP_SZ
    #define RWIP_HEAP_ENV_SIZE_JT           RWIP_HEAP_ENV_SIZE_USER     // in bytes
#else
    #define RWIP_HEAP_ENV_SIZE_JT           ENV_HEAP_SZ                 // in bytes
#endif

/// DB Heap
#ifndef DB_HEAP_SZ
    #define RWIP_HEAP_DB_SIZE_JT            1024                        // in bytes
#else
    #define RWIP_HEAP_DB_SIZE_JT            DB_HEAP_SZ                  // in bytes
#endif

/// Msg Heap
#ifndef MSG_HEAP_SZ
    #define RWIP_HEAP_MSG_SIZE_JT           RWIP_HEAP_MSG_SIZE_USER     // in bytes
#else
    #define RWIP_HEAP_MSG_SIZE_JT           MSG_HEAP_SZ                 // in bytes
#endif

uint32_t rwip_heap_non_ret[RWIP_CALC_HEAP_LEN(RWIP_HEAP_NON_RET_SIZE_JT)]   __attribute__((section("heap_mem_area_not_ret"), zero_init));
uint32_t rwip_heap_env_ret[RWIP_CALC_HEAP_LEN(RWIP_HEAP_ENV_SIZE_JT)]       __attribute__((section("heap_env_area"), zero_init));
uint32_t rwip_heap_msg_ret[RWIP_CALC_HEAP_LEN(RWIP_HEAP_MSG_SIZE_JT)]       __attribute__((section("heap_msg_area"), zero_init));
uint32_t rwip_heap_db_ret[RWIP_CALC_HEAP_LEN(RWIP_HEAP_DB_SIZE_JT)]         __attribute__((section("heap_db_area"), zero_init));


/*
 * ROM function table function definitions
 ****************************************************************************************
 */

void dummyf()
{
}

void crypto_init_func(void)
{
}

uint8_t ba431_get_rand_func(uint32_t id, uint8_t * dest, uint32_t byte_size)
{
    uint8_t i;
    for (i = 0; i < 32; i++)
        dest[i] = co_rand_byte();

   return 0;
}

int dia_rand_func(void)
{
#if (USE_CHACHA20_RAND)
    return csprng_get_next_uint32();
#else
    return rand();
#endif
}

void dia_srand_func(unsigned int seed)
{
#if (USE_CHACHA20_RAND)
    uint8_t temp[16];
    temp[0] = (seed >> 24) & 0xFF;
    temp[1] = (seed >> 16) & 0xFF;
    temp[2] = (seed >> 8) & 0xFF;
    temp[3] = seed & 0xFF;
    csprng_seed(temp);
#else
    srand(seed);
#endif
}

void dbg_init_func_empty(void)
{
}

void dbg_platform_reset_complete_func_empty(uint32_t error)
{
}

void platform_reset_func(uint32_t error)
{
    uint16_t tmp;

#if (!PRODUCTION_TEST)
    // Trigger assert if the reset reason is other than RESET_AFTER_SUOTA_UPDATE
    ASSERT_WARNING(error == RESET_AFTER_SUOTA_UPDATE);
#endif
    // Trigger SW reset
    tmp = GetWord16(SYS_CTRL_REG);
    tmp = (tmp & ~REMAP_ADR0) | 0; // Map ROM at address 0
    tmp |= SW_RESET;
    SetWord16(SYS_CTRL_REG, tmp);
}

/*
 * ROM function table
 ****************************************************************************************
 */
const void * const rom_func_addr_table_var[] =
{
    (void *) rf_init_func,
    (void *) rf_reinit_func,
    (void *) uart_init_func,
    (void *) uart_flow_on_func,
    (void *) uart_flow_off_func,
    (void *) uart_finish_transfers_func,
    (void *) uart_read_func,
    (void *) uart_write_func,
    (void *) UART_Handler_func,
    (void *) gtl_init_func,
    (void *) gtl_eif_init_func,
    (void *) gtl_eif_read_start_func,
    (void *) gtl_eif_read_hdr_func,
    (void *) gtl_eif_read_payl_func,
    (void *) gtl_eif_tx_done_func,
    (void *) gtl_eif_rx_done_func,
    (void *) h4tl_init_func,
    (void *) h4tl_read_start_func,
    (void *) h4tl_read_hdr_func,
    (void *) h4tl_read_payl_func,
    (void *) h4tl_read_next_out_of_sync_func,
    (void *) h4tl_out_of_sync_func,
    (void *) h4tl_tx_done_func,
    (void *) h4tl_rx_done_func,
    (void *) ke_task_init_func,
    (void *) ke_timer_init_func,
    (void *) llm_encryption_done_func,
    
    (void *) nvds_get_func,
    (void *) nvds_put_func,
    (void *) nvds_del_func,
    (void *) nvds_init_func,
    (void *) rwip_eif_get_func,

    (void *) platform_reset_func,
    (void *)  lld_sleep_compensate_func,
    (void *)  lld_sleep_init_func,
    (void *)  lld_sleep_us_2_lpcycles_sel_func,
    (void *)  lld_sleep_lpcycles_2_us_sel_func,

    (void *) lld_test_stop_func,
    (void *) lld_test_mode_tx_func,
    (void *) lld_test_mode_rx_func,
    (void *) smpc_check_param_func,
    (void *) smpc_pdu_recv_func,

    (void *) prf_init_func,
    (void *) prf_add_profile_func,
    (void *) prf_create_func,
    (void *) prf_cleanup_func,

    (void *) prf_get_id_from_task_func,
    (void *) prf_get_task_from_id_func,


    (void *) NULL, //SetSystemVars_func,
#ifdef CFG_DBG_TASK
    (void *) dbg_init_func,
    (void *) dbg_platform_reset_complete_func,
#else
    (void *) dbg_init_func_empty,
    (void *) dbg_platform_reset_complete_func_empty,
#endif
    (void *) hci_rd_local_supp_feats_cmd_handler_func,
    
    (void *) l2cc_pdu_pack_func,
    (void *) l2cc_pdu_unpack_func,
    (void *) l2c_send_lecb_message_func,
    (void *) l2c_process_sdu_func,
    
    (void *) l2cc_pdu_recv_ind_handler_func,
    (void *) gapc_lecb_connect_cfm_handler_func,
    (void *) atts_l2cc_pdu_recv_handler_func,
    (void *) attc_l2cc_pdu_recv_handler_func,

    (void *) crypto_init_func,
    (void *) llm_le_adv_report_ind_func,
    (void *) PK_PointMult_func,
    (void *) llm_p256_start_func,
    (void *) llm_create_p256_key_func,
    (void *) llm_p256_req_handler_func,
    (void *) llc_le_length_effective_func,
    (void *) llc_le_length_conn_init_func,
    (void *) lld_data_tx_prog_func,
    (void *) lld_data_tx_check_func,
    (void *) llc_pdu_send_func,
    (void *) NULL, //llc_data_notif_func,
    (void *) dia_rand_func,
    (void *) dia_srand_func,
    (void *) ba431_get_rand_func,

    (void *) modulationGainCalibration_func,
    (void *) DCoffsetCalibration_func,
    (void *) PllLF_IFF_MIX_CapCalibration_func,
    (void *) smpc_public_key_exchange_start_func,
    (void *) smpc_dhkey_calc_ind_func,
    (void *) smpm_ecdh_key_create_func,

    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};


/*
 * ROM configuration table
 ****************************************************************************************
 */
const uint32_t rom_cfg_table_var[] =
{

    (uint32_t)TASK_GTL, //TASK_APP_SEC        // 0. app task          this entry is for the security app 
#pragma diag_suppress 1296
    (uint32_t)&rwip_heap_non_ret[0],          // 1. rwip_heap_non_ret_pos
    (uint32_t)RWIP_HEAP_NON_RET_SIZE_JT,      // 2. rwip_heap_non_ret_size
    (uint32_t)&rwip_heap_env_ret[0],          // 3. rwip_heap_env_pos
    (uint32_t)RWIP_HEAP_ENV_SIZE_JT,          // 4. rwip_heap_env_size
    (uint32_t)&rwip_heap_db_ret[0],           // 5. rwip_heap_db_pos
    (uint32_t)RWIP_HEAP_DB_SIZE_JT,           // 6. rwip_heap_db_size
    (uint32_t)&rwip_heap_msg_ret[0],          // 7. rwip_heap_msg_pos
    (uint32_t)RWIP_HEAP_MSG_SIZE_JT,          // 8. rwip_heap_msg_size
#pragma diag_warning 1296
    MAX_SLEEP_DURATION_PERIODIC_WAKEUP,     // 9. max_sleep_duration_periodic_wakeup_pos  0x320
    MAX_SLEEP_DURATION_EXTERNAL_WAKEUP,     // 10.max_sleep_duration_external_wakeup_pos  0x3e80
    #if BLE_APP_PRESENT
    TASK_APP,                               // 11. app_main_task_pos
    #else // BLE_HOST_PRESENT
    TASK_GTL,                               // 11.
    #endif
    0x00D2,                                 // 12. man_id_pos Dialog Semi ManId

    50,     //13. max_tl_pending_packets_adv_pos   
    60,     //14.  max_tl_pending_packets_pos        
    1,      //15. ea_timer_prog_delay_pos             
    1,      //16. ea_clock_corr_lat_pos
    2,      //17. ea_be_used_dft_pos                    
    2,      //18. start_margin_pos                     
    4,      //19. test_mode_margin_pos                    
    3,      //20.  bw_used_slave_dft_pos                 
    6,      //21.  bw_used_adv_dft_pos                    
#ifdef XTAL_32000HZ
    1,      //22. hz32000_pos
#else
    0,      //22. hz32000_pos                             
#endif
    2,      //23. rwble_prog_latency_dft_pos            
    2,      //24. rwble_asap_latency_pos                  24
    0,      // rwble_priority_adv_ldc_pos              25
    0,      // rwble_priority_scan_pos                 26
    3,      // rwble_priority_mconnect_pos             27
    3,      // rwble_priority_sconnect_pos             28
    5,      // rwble_priority_adv_hdc_pos              29
    5,      // rwble_priority_init_pos                 30
    6,      // rwble_priority_max_pos                  31
    10,     //gap_lecb_cnx_max
    10,     //gapm_scan_filter_size
    200,    //smpc_rep_attempts_timer_def_val
    3000,   //smpc_rep_attempts_timer_max_val
    2,      //smpc_rep_attempts_timer_mult
    3000,   //smpc_timeout_timer_duration
    0x0BB8, //att_trans_rtx
    0x10,   //att_sec_enc_key_size
    468,    //wakeup_boundary_var
    485,    //lld_evt_abort_cnt_duration
    624,    //ea_check_halfslot_boundary
    106,    //ea_check_slot_boundary
    (BLE_RX_BUFFER_CNT/2),      //RX interrupt threshold
    BLE_CONNECTION_MAX_USER,    //nb_links_user
    LLM_ADV_INTERVAL_MIN,       //llm_adv_interval_min_noncon_disc_pos
};

