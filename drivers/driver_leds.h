/**
 * MIT License
 * Copyright (c) 2024 Archil Pirmisashvili
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @ingroup     drivers
 * @{
 *
 * @file
 * @brief       Leds driver definitions
 *
 * @author      Archil Pirmisashvili <archil@rf-networks.com>
 */

#ifndef DRIVERS_DRIVER_LEDS_H
#define DRIVERS_DRIVER_LEDS_H

#include "boards/board_common.h"

#ifdef LEDS_CONF_COUNT
#define LEDS_COUNT LEDS_CONF_COUNT
#else
#define LEDS_COUNT 0
#endif

/*---------------------------------------------------------------------------*/
/**
 * @brief The OR mask representation of all device LEDs
 */
#define LEDS_ALL ((1 << LEDS_COUNT) - 1)
/*---------------------------------------------------------------------------*/
#define LEDS_LED1     0x00 /**< Convenience macro to refer to the 1st LED (LED 1) */
#define LEDS_LED2     0x01 /**< Convenience macro to refer to the 2nd LED (LED 2) */
#define LEDS_LED3     0x02 /**< Convenience macro to refer to the 3rd LED (LED 3) */
#define LEDS_LED4     0x03 /**< Convenience macro to refer to the 4th LED (LED 4) */
#define LEDS_LED5     0x04 /**< Convenience macro to refer to the 5th LED (LED 5) */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/**
 * @brief A default LED color for non-existing LEDs
 */
#define LEDS_COLOR_NONE 0xFF
/*---------------------------------------------------------------------------*/
/* LED color to number mappings. Applicable to both APIs */
#ifdef LEDS_CONF_RED
#define LEDS_RED LEDS_CONF_RED
#else
#define LEDS_RED LEDS_COLOR_NONE
#endif

#ifdef LEDS_CONF_GREEN
#define LEDS_GREEN LEDS_CONF_GREEN
#else
#define LEDS_GREEN LEDS_COLOR_NONE
#endif

#ifdef LEDS_CONF_BLUE
#define LEDS_BLUE LEDS_CONF_BLUE
#else
#define LEDS_BLUE LEDS_COLOR_NONE
#endif

#ifdef LEDS_CONF_YELLOW
#define LEDS_YELLOW LEDS_CONF_YELLOW
#else
#define LEDS_YELLOW LEDS_COLOR_NONE
#endif

#ifdef LEDS_CONF_ORANGE
#define LEDS_ORANGE LEDS_CONF_ORANGE
#else
#define LEDS_ORANGE LEDS_COLOR_NONE
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
/**
 * @brief The LED number
 */
typedef uint8_t leds_num_t;

/*---------------------------------------------------------------------------*/
/**
 * @brief Initialize the LED HAL
 *
 * This function will set corresponding LED GPIO pins to output and will also
 * set the initial state of all LEDs to off.
 */
void leds_init(void);

/**
 * @brief Turn a single LED on
 *
 * @param[in]   led     The led
 *
 * The \e led argument should be the LED's number, in other words one of the
 * LED_Ln macros.
 *
 * This function will not change the state of other LEDs.
 */
void leds_single_on(leds_num_t led);

/**
 * @brief Turn a single LED off
 *
 * @param[in]   led     The led
 *
 * The \e led argument should be the LED's number, in other words one of the
 * LED_Ln macros.
 *
 * This function will not change the state of other LEDs.
 */
void leds_single_off(leds_num_t led);

/**
 * @brief Toggle a single LED
 *
 * @param[in]   led     The led
 *
 * The \e led argument should be the LED's number, in other words one of the
 * LED_Ln macros.
 *
 * This function will not change the state of other LEDs.
 */
void leds_single_toggle(leds_num_t led);

/**
 * @brief Connect single LED pin to HW peripheral, signal or to GPIO
 *
 * @param[in]   led     The led
 *
 * @param[in]   mux     Device-specific index of peripheral port or hardware signal.
 *
 * The \e led argument should be the LED's number, in other words one of the
 * LED_Ln macros.
 *
 * This function will not change the state of other LEDs.
 */
void leds_single_set_mux(leds_num_t led, int32_t mux);

/**
 * @brief Blink all LEDs N times
 *
 * @param[in]   times   Times to blink
 *
 * This function blinks all LED's N times
 */
void leds_blink_all(uint8_t times);

#ifdef __cplusplus
}
#endif

#endif /* DRIVERS_DRIVER_LEDS_H */
/** @} */
