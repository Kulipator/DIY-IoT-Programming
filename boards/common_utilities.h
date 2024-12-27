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
 * @ingroup     common_utilities
 * @{
 *
 * @file
 * @brief       Common utilities definition
 *
 * @author      Archil Pirmisashvili <archil@rf-networks.com>
 */

#ifndef BOARDS_COMMON_UTILITIES_H
#define BOARDS_COMMON_UTILITIES_H

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "ti-lib.h"

#define MAC_ADDR                                            (0x500012F0)

#define SIZE_OF_ARRAY(x)                                    (sizeof(x)/sizeof(x[0]))

#define DEFINE_CRITICAL()                                   uintptr_t critical_key
#define ENTER_CRITICAL()                                    critical_key = ti_lib_driver_hwip_disable()
#define EXIT_CRITICAL()                                     ti_lib_driver_hwip_restore(critical_key)

#ifndef MIN
#define MIN(n, m)    (((n) > (m)) ? (m) : (n))
#endif

#ifndef MAX
#define MAX(n, m)    (((n) < (m)) ? (m) : (n))
#endif

#define RTC_TIME_ELAPSED(x)                                 ((int32_t)(x - get_up_time()) <= 0)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Get system up time
 *
 * This function gets RTC time
 *
 * @returns     Up time in seconds
 */
uint32_t get_up_time(void);

/**
 * @brief   Get system up time
 *
 * This function gets RTC time
 *
 * @returns     Up time in microseconds
 */
uint64_t get_up_time_us(void);

/**
 * @brief   Get unit MAC address
 *
 * This function retrieves unit MAC address
 *
 * @returns     MAC address
 */
uint64_t get_mac_address(void);

/**
 * @brief   Reboot unit
 *
 * This function reboots unit
 */
void reboot(void);

/**
 * @brief   Posix usleep implementation
 *
 * @param[in] usec    Microseconds to sleep
 *
 * This function enters standby mode for defined micro-seconds.
 * Sleep can be interrupted
 */
bool rfn_posix_usleep(useconds_t usec);

/**
 * @brief   Posix sleep implementation
 *
 * @param[in] sec    Seconds to sleep
 *
 * This function enters standby mode for defined seconds.
 * Sleep can be interrupted
 */
bool rfn_posix_sleep(unsigned sec);

/**
 * @brief   Posix sleep interruption
 *
 * This function interrupts sleep.
 */
void rfn_posix_sleep_break(void);

#ifdef __cplusplus
}
#endif

#endif /* BOARDS_COMMON_UTILITIES_H */
/** @} */
