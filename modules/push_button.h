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
 * @ingroup     sensors
 * @{
 *
 * @file
 * @brief       Push button management definition
 *
 * @author      Archil Pirmisashvili <kulipator@gmail.com>
 */

#ifndef SENSORS_PUSH_BUTTON_PUSH_BUTTON_H
#define SENSORS_PUSH_BUTTON_PUSH_BUTTON_H

#include "boards/board_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(Board_PIN_REED_SW) && (Board_PIN_REED_SW != PIN_UNASSIGNED)

/**
 * @brief   Forward declaration for reed switch change callback
 */
typedef void (*PushButton_ReedSwitchChangeCallback)(bool is_pressed);

/**
 * @brief   Forward declaration for reed switch released callback
 */
typedef void (*PushButton_ReedSwitchReleasedCallback)(uint32_t pressed_time);

/**
 * @brief Register reed switch change callback
 *
 * @param[in]   callback   Callback function
 *
 * This function registers callback for reed switch change event.
 * Set callback to NULL in order to disable it
 */
void push_button_register_reed_switch_change_callback(PushButton_ReedSwitchChangeCallback callback);

/**
 * @brief Register reed switch released callback
 *
 * @param[in]   callback   Callback function
 *
 * This function registers callback for reed switch released event.
 * Set callback to NULL in order to disable it
 */
void push_button_register_reed_switch_released_callback(PushButton_ReedSwitchReleasedCallback callback);

#endif

/**
 * @brief   Initialize push button
 *
 * @returns Initialization result
 *
 * This function initializes push button
 */
bool push_button_init(void);

#ifdef __cplusplus
}
#endif

#endif /* SENSORS_PUSH_BUTTON_PUSH_BUTTON_H */
/** @} */
