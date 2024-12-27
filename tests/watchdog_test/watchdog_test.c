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

/*
 * @ingroup     tests_watchdog_test
 * @{
 *
 * @file
 * @brief       This test changes state of all board LEDs with incrementing period.
 *              Each cycle period is incremented by SLEEP_INCREMENT until SLEEP_MAX_VALUE
 *              is reached. When SLEEP_MAX_VALUE is reached period is set to SLEEP_START_VALUE
 *              and application stops resetting the watchdog, which will lead to watchdog reset.
 *
 * @author      Archil Pirmisashvili <archil@rf-networks.com>
 * @}
 */

#ifdef TESTS_WATCHDOG_TEST

#include "boards/board_common.h"
#include "drivers/driver_leds.h"

#define SLEEP_INCREMENT     100000UL  /* 100 ms */
#define SLEEP_START_VALUE   100000UL  /* 100 ms */
#define SLEEP_MAX_VALUE     5000000UL /* 5 sec  */

/**
 * @brief   Test entry point
 *
 */
void *mainThread(void *arg0)
{
    bool stop_watchdog;
    unsigned int sleep_time;
    /* Test initialization */
    stop_watchdog = false;
    sleep_time = SLEEP_START_VALUE;

    /* Main loop */
    while(1)
    {
        if (stop_watchdog)
        {
            WDT_RESET();
        }
/* Toggle led */
#if defined(LEDS_RED) && (LEDS_RED != LEDS_COLOR_NONE)
        leds_single_toggle(LEDS_RED);
#endif
#if defined(LEDS_GREEN) && (LEDS_GREEN != LEDS_COLOR_NONE)
        leds_single_toggle(LEDS_GREEN);
#endif
#if defined(LEDS_BLUE) && (LEDS_BLUE != LEDS_COLOR_NONE)
        leds_single_toggle(LEDS_BLUE);
#endif
#if defined(LEDS_YELLOW) && (LEDS_YELLOW != LEDS_COLOR_NONE)
        leds_single_toggle(LEDS_YELLOW);
#endif
#if defined(LEDS_ORANGE) && (LEDS_ORANGE != LEDS_COLOR_NONE)
        leds_single_toggle(LEDS_ORANGE);
#endif
        /* Sleep (go to low power) */
        if (stop_watchdog)
        {
            SLEEP_USECONDS_WITH_WDT_ON(sleep_time);
        }
        else 
        {
            SLEEP_USECONDS(sleep_time);
        }

        /* Update delay time */
        sleep_time += SLEEP_INCREMENT;
        if (sleep_time > SLEEP_MAX_VALUE)
        {
            sleep_time = SLEEP_START_VALUE;
            stop_watchdog = true;
        }
    }
}

#endif /* TESTS_WATCHDOG_TEST */
