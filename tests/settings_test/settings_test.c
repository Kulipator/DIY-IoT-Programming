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
 * @ingroup     tests_settings_test
 * @{
 *
 * @file
 * @brief       This test checks functionality of settings module.
 *
 * @author      Archil Pirmisashvili <archil@rf-networks.com>
 * @}
 */


#ifdef TESTS_SETTINGS_TEST
#include <stdio.h>
#include "boards/board_common.h"
#include "drivers/driver_leds.h"
#include "drivers/driver_watchdog.h"
#include "drivers/driver_console.h"
#include "modules/settings.h"

#ifndef CONSOLE_BAUDRATE_CONF
#define CONSOLE_BAUDRATE    CONSOLE_BAUDRATE_19200
#else
#define CONSOLE_BAUDRATE    CONSOLE_BAUDRATE_CONF
#endif

#define TEST_ITERATIONS     300

char msg[100];

/**
 * @brief   Test entry point
 *
 */
void *mainThread(void *arg0)
{
    bool res;
    uint16_t counter = 0;

    /* Test initialization */
    if (!settings_load()) 
    {
        /* No settings found, use default */
        settings_set_default();
        settings.sensor_readout_interval_sec = 0;
        settings_save();
    }
    console_initialize(CONSOLE_BAUDRATE);

    /* Main loop */
    while(1)
    {
        /* Reset watch-dog */
        WDT_RESET();

        /* Process console */
        console_process();

        if (!console_is_transmitting() && counter < TEST_ITERATIONS) 
        {
            /* Increment */
            res = true;
            counter++;
            settings.sensor_readout_interval_sec = counter;
            if (!settings_save() || settings_load() || (settings.sensor_readout_interval_sec != counter))
            {
                res = false;
            }
            memset(msg, 0, SIZE_OF_ARRAY(msg));
            sprintf(msg, "Iteration %d - %s\r\n", counter, (res)? "OK" : "Fail");
            console_write(msg, strlen(msg));
        }
    }
}

#endif /* TESTS_SETTINGS_TEST */
