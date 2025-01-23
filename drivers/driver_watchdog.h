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
 * @brief       Watchdog driver definitions
 *
 * @author      Archil Pirmisashvili <kulipator@gmail.com>
 */

#ifndef DRIVERS_DRIVER_WATCHDOG_H
#define DRIVERS_DRIVER_WATCHDOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define WDT_RESET()   watchdog_reset()

/**
 * @brief       Initialize watch-dog
 *
 * This function initializes watch-dog driver
 */
void watchdog_initialize(void);

/**
 * @brief       Enable/Disable watch-dog
 *
 * @param[in]   enable  Enable/Disable watchdog
 *
 * This function enables or disabled watch-dog
 */
void watchdog_enable(bool enable);

/**
 * @brief       Reset watch-dog
 *
 * This function resets watch-dog counter
 */
void watchdog_reset(void);

/**
 * @brief       Close watch-dog
 *
 * This function closes watch-dog driver
 */
void watchdog_close(void);

#ifdef __cplusplus
}
#endif

#endif /* DRIVERS_DRIVER_WATCHDOG_H */
/** @} */
