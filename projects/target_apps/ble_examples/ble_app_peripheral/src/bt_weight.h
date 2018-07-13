#ifndef __BT_WEIGHT_H
#define __BT_WEIGHT_H

/**
 ****************************************************************************************
 * @brief call to start advertising.
 * @return void
 ****************************************************************************************
*/
void app_start_advertising(void);

/**
 ****************************************************************************************
 * @brief call to update advertising data.
 * @return void
 ****************************************************************************************
*/
void app_update_adv_data(void);


/**
 ****************************************************************************************
 * @brief call to set weight data
 * @para  state         £º0x00=unstable  0x01=stable
          current_weight: current weight data
					total_weight  £ºtotal weight data
 * @return void
 ****************************************************************************************
*/
void app_set_data(uint8_t state,float current_weight,float total_weight);


void app_catch_rest_hndl(ke_msg_id_t const msgid,
                          void const *param,
                          ke_task_id_t const dest_id,
                          ke_task_id_t const src_id);

bool factory_test_check(uint8_t ver1, uint8_t ver2);
#endif