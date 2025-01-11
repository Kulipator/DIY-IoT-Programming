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
 * @brief       Common utilities implementation
 *
 * @author      Archil Pirmisashvili <archil@rf-networks.com>
 * @}
 */

#include <ctype.h>
#include <NoRTOS.h>
#include "common_utilities.h"

static SemaphoreP_Struct rfn_usleep_semaphore_struct;
static SemaphoreP_Handle rfn_usleep_semaphore = NULL;

uint32_t get_up_time(void)
{
    uint32_t ret;
    DEFINE_CRITICAL();
    ENTER_CRITICAL();
    ret = ti_lib_aon_rtc_sec_get();
    EXIT_CRITICAL();
    return ret;
}

uint64_t get_up_time_us(void)
{
    uint64_t ret;
    uint64_t val = ti_lib_aon_rtc_current_64_bit_value_get();
    ret = (val >> 32) * 1000000ULL + (uint64_t)(((val & 0xFFFFFFFFULL) / (0x4000000FULL)) * 15625);
    return ret;
}

uint64_t get_mac_address(void)
{
    uint64_t mac;

    memcpy(&mac, (void*) MAC_ADDR, 8);

    return mac;
}

void reboot(void)
{
    ti_lib_sys_ctrl_system_reset();
}

bool rfn_posix_usleep(useconds_t usec)
{
    int res = SemaphoreP_TIMEOUT;
    static NoRTOS_Config cfg;

    if (usec > 0)
    {
        NoRTOS_getConfig(&cfg);
        rfn_usleep_semaphore = ti_lib_driver_semaphorep_constuct_binary(&rfn_usleep_semaphore_struct, 0);
        res = ti_lib_driver_semaphorep_pend(rfn_usleep_semaphore, usec / cfg.clockTickPeriod);
        ti_lib_driver_semaphorep_destruct(&rfn_usleep_semaphore_struct);
    }
    return (res == SemaphoreP_TIMEOUT);
}

bool rfn_posix_sleep(unsigned sec)
{
    return rfn_posix_usleep(sec * 1000000UL);
}

void rfn_posix_sleep_break(void)
{
    if (rfn_usleep_semaphore)
    {
        ti_lib_driver_semaphorep_post(rfn_usleep_semaphore);
    }
}

uint16_t calculate_crc16_xmodem(uint8_t *msg, size_t size)
{
    uint8_t x;
    uint16_t crc = 0x0000;

    while (size--)
    {
        crc = crc ^ (uint16_t)*msg++ << 8;
        x = 8;
        do
        {
            if (crc & 0x8000)
            {
                crc = crc << 1 ^ 0x1021;
            }
            else
            {
                crc = crc << 1;
            }
        } while(--x);
    }
    return (crc);
}
