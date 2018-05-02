/**
 ****************************************************************************************
 *
 * @file gpio.h
 *
 * @brief Hardware GPIO abstraction layer API.
 *
 * Copyright (C) 2012-2017 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com>
 *
 ****************************************************************************************
 */

#ifndef _GPIO_H_
#define _GPIO_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdbool.h>
#include <stdint.h>
#include "arch.h"

/*
 * DEFINES
 ****************************************************************************************
 */

typedef enum {
    INPUT = 0,
    INPUT_PULLUP = 0x100,
    INPUT_PULLDOWN = 0x200,
    OUTPUT = 0x300,
} GPIO_PUPD;

typedef enum {
    GPIO_POWER_RAIL_3V = 0,
    GPIO_POWER_RAIL_1V = 1,
} GPIO_POWER_RAIL;

typedef enum {
    GPIO_PORT_0 = 0,
    GPIO_PORT_1 = 1,
    GPIO_PORT_2 = 2,
    GPIO_PORT_3 = 3,
    GPIO_PORT_3_REMAP = 4,
} GPIO_PORT;

typedef enum {
    GPIO_PIN_0 = 0,
    GPIO_PIN_1 = 1,
    GPIO_PIN_2 = 2,
    GPIO_PIN_3 = 3,
    GPIO_PIN_4 = 4,
    GPIO_PIN_5 = 5,
    GPIO_PIN_6 = 6,
    GPIO_PIN_7 = 7,
    GPIO_PIN_8 = 8,
    GPIO_PIN_9 = 9,
} GPIO_PIN;

typedef enum {
    PID_GPIO = 0,
    PID_UART1_RX,
    PID_UART1_TX,
    PID_UART2_RX,
    PID_UART2_TX,
    PID_SPI_DI,
    PID_SPI_DO,
    PID_SPI_CLK,
    PID_SPI_EN,
    PID_I2C_SCL,
    PID_I2C_SDA,
    PID_UART1_IRDA_RX,
    PID_UART1_IRDA_TX,
    PID_UART2_IRDA_RX,
    PID_UART2_IRDA_TX,
    PID_ADC,
    PID_PWM0,
    PID_PWM1,
    PID_BLE_DIAG,
    PID_UART1_CTSN,
    PID_UART1_RTSN,
    PID_UART2_CTSN,
    PID_UART2_RTSN,
    PID_PWM2,
    PID_PWM3,
    PID_PWM4,
    PID_PCM_DI,
    PID_PCM_DO,
    PID_PCM_FSC,
    PID_PCM_CLK,
    PID_PDM_DATA,
    PID_PDM_CLK,
} GPIO_FUNCTION;

typedef enum {
    GPIO_IRQ_INPUT_LEVEL_HIGH = 0,
    GPIO_IRQ_INPUT_LEVEL_LOW = 1,
} GPIO_IRQ_INPUT_LEVEL;

typedef enum {
    GPIO_POR_PIN_POLARITY_LOW = 0,
    GPIO_POR_PIN_POLARITY_HIGH = 1,
} GPIO_POR_PIN_POLARITY;

// GPIO base address
#define GPIO_BASE   P0_DATA_REG

/**
 ****************************************************************************************
 * @brief Helper macro that calculates the value to be written into POR_PIN_REG register.
 * @param[in] port          GPIO port.
 * @param[in] pin           GPIO pin.
 * @param[in] polarity      0 = Active low, 1 = Active high.
 * @return The value to be written into POR_PIN_REG register. If the port/pin/polarity
 *         values are illegal, the returned value is POR_PIN_REG_RESET (reset value).
 ****************************************************************************************
 */
#define GPIO_POR_PIN_REG(port, pin, polarity)                                                                                 \
        ( port == GPIO_PORT_0 && pin <= GPIO_PIN_7 && polarity <= GPIO_POR_PIN_POLARITY_HIGH ? (polarity << 7) | (pin + 1)  : \
          port == GPIO_PORT_1 && pin <= GPIO_PIN_5 && polarity <= GPIO_POR_PIN_POLARITY_HIGH ? (polarity << 7) | (pin + 9)  : \
          port == GPIO_PORT_2 && pin <= GPIO_PIN_9 && polarity <= GPIO_POR_PIN_POLARITY_HIGH ? (polarity << 7) | (pin + 15) : \
          port == GPIO_PORT_3 && pin <= GPIO_PIN_7 && polarity <= GPIO_POR_PIN_POLARITY_HIGH ? (polarity << 7) | (pin + 25) : \
          POR_PIN_REG_RESET                                                                                                   \
        )                                                                                                                     \

//
// Macro for pin definition structure
//      name: usage and/or module using it
//      func: GPIO, UART1_RX, UART1_TX, etc.
//
#if DEVELOPMENT_DEBUG && !defined(GPIO_DRV_PIN_ALLOC_MON_DISABLED)
#define RESERVE_GPIO( name, port, pin, func )   { GPIO[port][pin] = (GPIO[port][pin] != 0) ? (-1) : 1; GPIO_status |= ((uint64_t)GPIO[port][pin] << pin) << (port * 16);}
#else
#define RESERVE_GPIO( name, port, pin, func )   {}
#endif //DEVELOPMENT_DEBUG && !defined(GPIO_DRV_PIN_ALLOC_MON_DISABLED)

#if DEVELOPMENT_DEBUG
#ifndef GPIO_DRV_PIN_ALLOC_MON_DISABLED

#define NO_OF_PORTS 4   // cannot be bigger than 4
#define NO_OF_MAX_PINS_PER_PORT 10  // cannot be bigger than 16

extern int GPIO[NO_OF_PORTS][NO_OF_MAX_PINS_PER_PORT];

extern volatile uint64_t GPIO_status;
#endif //GPIO_DRV_PIN_ALLOC_MON_DISABLED
#endif //DEVELOPMENT_DEBUG

typedef void (*GPIO_handler_function_t)(void);

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Register Callback function for GPIO IRQ.
 * @param[in] irq       The handler of this IRQ will call the function
 * @param[in] callback  Callback function's reference.
 * @return void
 ****************************************************************************************
 */
void GPIO_RegisterCallback(IRQn_Type irq, GPIO_handler_function_t callback);

/**
 ****************************************************************************************
 * @brief Function to set the interrupt generated by the GPIO pin.
 * @param[in] port          GPIO port
 * @param[in] pin           GPIO pin
 * @param[in] irq           GPIO IRQ
 * @param[in] low_input     TRUE generates an IRQ if the input is low
 * @param[in] release_wait  TRUE waits for key release after interrupt was reset
 * @param[in] debounce_ms   duration of a debounce sequence before an IRQ is generated
 * @return void
 ****************************************************************************************
 */
void GPIO_EnableIRQ(GPIO_PORT port,
                    GPIO_PIN pin,
                    IRQn_Type irq,
                    bool low_input,
                    bool release_wait,
                    uint8_t debounce_ms);

/**
 ****************************************************************************************
 * @brief Returns the GPIO input level that triggers a GPIO IRQ.
 * @param[in] irq   GPIO IRQ number (e.g. GPIO2_IRQn)
 * @return input level that triggers the GPIO IRQ (see GPIO_IRQ_INPUT_LEVEL)
 ****************************************************************************************
 */
GPIO_IRQ_INPUT_LEVEL GPIO_GetIRQInputLevel(IRQn_Type irq);

/**
 ****************************************************************************************
 * @brief Set the GPIO input level that triggers a GPIO IRQ.
 * @param[in] irq       GPIO IRQ (e.g. GPIO2_IRQn)
 * @param[in] level     Input level that triggers the GPIO IRQ (see GPIO_IRQ_INPUT_LEVEL)
 * @return void
 ****************************************************************************************
 */
void GPIO_SetIRQInputLevel(IRQn_Type irq, GPIO_IRQ_INPUT_LEVEL level);

/**
 ****************************************************************************************
 * @brief Function to reset the interrupt generated by the GPIO pin.
 * @param[in] irq   GPIO IRQ
 * @return void
 ****************************************************************************************
 */
void GPIO_ResetIRQ(IRQn_Type irq);

/**
 ****************************************************************************************
 * @brief Initialize the GPIO assignemnt check variables.
 * @return void
 ****************************************************************************************
 */
void GPIO_init(void);

/**
 ****************************************************************************************
 * @brief Set the pin type and mode
 * @param[in] port     GPIO port
 * @param[in] pin      GPIO pin
 * @param[in] mode     GPIO pin mode. INPUT = 0, INPUT_PULLUP = 0x100, INPUT_PULLDOWN = 0x200, OUTPUT = 0x300,
 * @param[in] function GPIO pin usage. GPIO_FUNCTION enumeration.
 * @return void
 ****************************************************************************************
 */
void GPIO_SetPinFunction(GPIO_PORT port, GPIO_PIN pin, GPIO_PUPD mode,
                                GPIO_FUNCTION function);

/**
 ****************************************************************************************
 * @brief Combined function to set the state and the type and mode of the GPIO pin.
 * @param[in] port     GPIO port
 * @param[in] pin      GPIO pin
 * @param[in] mode     GPIO pin mode. INPUT = 0, INPUT_PULLUP = 0x100, INPUT_PULLDOWN = 0x200, OUTPUT = 0x300,
 * @param[in] function GPIO pin usage. GPIO_FUNCTION enumaration.
 * @param[in] high     set to TRUE to set the pin into high else low
 * @return void
 ****************************************************************************************
 */
void GPIO_ConfigurePin(GPIO_PORT port, GPIO_PIN pin, GPIO_PUPD mode,
                              GPIO_FUNCTION function, const bool high);

/**
 ****************************************************************************************
 * @brief Sets a pin high. The GPIO should have been previously configured as output!
 * @param[in] port     GPIO port
 * @param[in] pin      GPIO pin
 * @return void
 ****************************************************************************************
 */
void GPIO_SetActive(GPIO_PORT port, GPIO_PIN pin);

/**
 ****************************************************************************************
 * @brief Sets the GPIO low. The GPIO should have been previously configured as output!
 * @param[in] port     GPIO port
 * @param[in] pin      GPIO pin
 * @return void
 ****************************************************************************************
 */
void GPIO_SetInactive(GPIO_PORT port, GPIO_PIN pin);

/**
 ****************************************************************************************
 * @brief Gets the GPIO status. The GPIO should have been previously configured as input!
 * @param[in] port     GPIO port
 * @param[in] pin      GPIO pin
 * @return bool        TRUE if the pin is high, FALSE if low.
 ****************************************************************************************
*/
bool GPIO_GetPinStatus(GPIO_PORT port, GPIO_PIN pin);

/**
 ****************************************************************************************
 * @brief Set the power source of the GPIO pin.
 * @param[in] port          GPIO port
 * @param[in] pin           GPIO pin
 * @param[in] power_rail    GPIO power rail. See GPIO_POWER_RAIL enumeration.
 * @return void
 ****************************************************************************************
 */
void GPIO_ConfigurePinPower(GPIO_PORT port, GPIO_PIN pin, GPIO_POWER_RAIL power_rail);

/**
 ****************************************************************************************
 * @brief Enable the GPIO Power-On Reset (POR) source.
 * @param[in] port          GPIO port
 * @param[in] pin           GPIO pin
 * @param[in] polarity      GPIO por pin polarity. 0 = Active low, 1 = Active high.
 * @param[in] por_time      Time for the Power-On Reset to happen. The time is calculated 
 *                          based on the following equation:
 *                          Time = por_time x 4096 x RC32 clock period
 * @return void
 ****************************************************************************************
 */
void GPIO_EnablePorPin(GPIO_PORT port, GPIO_PIN pin, GPIO_POR_PIN_POLARITY polarity, uint8_t por_time);

/**
 ****************************************************************************************
 * @brief Disable the GPIO Power-On Reset (POR) source.
 * @return void
 ****************************************************************************************
 */
void GPIO_DisablePorPin(void);

/**
 ****************************************************************************************
 * @brief Set the Power-On Reset (POR) time.
 * @param[in] por_time      Time for the Power-On Reset to happen. The time is calculated 
 *                          based on the following equation:
 *                          Time = por_time x 4096 x RC32 clock period
 * @return void
 ****************************************************************************************
 */
void GPIO_SetPorTime(uint8_t por_time);

/**
 ****************************************************************************************
 * @brief Get the Power-On Reset (POR) time.
 * @return The por_time value. 
 *         The time for the POR to happen is: Time = por_time x 4096 x RC32 clock period.
 ****************************************************************************************
 */
uint8_t GPIO_GetPorTime(void);

#endif // _GPIO_H_

