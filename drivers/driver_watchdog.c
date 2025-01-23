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
 * @brief       Watchdog driver implementations
 *
 * @author      Archil Pirmisashvili <kulipator@gmail.com>
 * @}
 */

#include <ti-lib.h>
#include "driver_watchdog.h"
#include "boards/board_common.h"

static void watchdog_cb(uintptr_t handle);

static Watchdog_Handle watchdog_handle;
static volatile uint8_t watchdog_disabled;

void watchdog_initialize(void)
{
    Watchdog_Params watchdogParams;

    /* Initialize watchdog */
    ti_lib_driver_watchdog_init();
    ti_lib_driver_watchdog_params_init(&watchdogParams);

    watchdogParams.callbackFxn = watchdog_cb;
#ifndef WATCHDOG_STALL
   watchdogParams.debugStallMode = Watchdog_DEBUG_STALL_OFF;
#endif
   watchdogParams.resetMode = Watchdog_RESET_OFF;
   watchdog_handle = ti_lib_driver_watchdog_open(WATCHDOG0, &watchdogParams);
   if (watchdog_handle == NULL)
   {
       /* Can't initialize watch-dog !!! Reboot... */
       ti_lib_sys_ctrl_system_reset();
   }

   watchdog_reset();
   watchdog_enable(true);
}

void watchdog_enable(bool enable)
{
    watchdog_disabled = !enable;
}

void watchdog_reset(void)
{
    ti_lib_driver_watchdog_clear(watchdog_handle);
}

void watchdog_close(void)
{
    ti_lib_driver_watchdog_close(watchdog_handle);
}

void watchdog_cb(uintptr_t handle)
{
    if (watchdog_disabled)
    {
        watchdog_reset();
    }
    else
    {
        /* Update time offset on reset */
        reboot();
    }
}
