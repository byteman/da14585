/**
 ****************************************************************************************
 *
 * @file timer.h
 *
 * @brief timer handling functions header file.
 *
 * Copyright (C) 2012-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <stdint.h>

void start_timer(uint32_t times_x_fourms);
void stop_timer(void);

#endif
