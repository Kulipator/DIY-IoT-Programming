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
 * @ingroup     tests_console_test
 * @{
 *
 * @file
 * @brief       This test puts emulates console echo.
 *
 * @author      Archil Pirmisashvili <kulipator@gmail.com>
 * @}
 */

#ifdef TESTS_CONSOLE_TEST

#include "boards/board_common.h"
#include "drivers/driver_leds.h"
#include "drivers/driver_watchdog.h"
#include "drivers/driver_console.h"

#ifndef CONSOLE_BAUDRATE_CONF
#define CONSOLE_BAUDRATE    CONSOLE_BAUDRATE_19200
#else
#define CONSOLE_BAUDRATE    CONSOLE_BAUDRATE_CONF
#endif

/**
 * @brief   Console message received callback
 *
 * @param[in]   data    Pointer to received data array
 * @param[in]   size    Size of received data
 */
static void console_message_received_cb(uint8_t *data, size_t size);

/**
 * @brief   Test entry point
 *
 */
void *mainThread(void *arg0)
{
    /* Test initialization */
    console_initialize(CONSOLE_BAUDRATE);
    console_register_message_received_callback(&console_message_received_cb);

    /* Main loop */
    while(1)
    {
        /* Reset watch-dog */
        WDT_RESET();

        /* Process console */
        console_process();
    }
}

void console_message_received_cb(uint8_t *data, size_t size)
{
    console_write(data, size);
}

#endif /* TESTS_CONSOLE_TEST */
