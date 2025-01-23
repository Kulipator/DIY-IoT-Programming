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
 * @ingroup     tests_sensor_buffer_test
 * @{
 *
 * @file
 * @brief       This test checks functionality of sensor buffer module.
 *
 * @author      Archil Pirmisashvili <kulipator@gmail.com>
 * @}
 */

#ifdef TESTS_SENSOR_BUFFER_TEST

#include <stdio.h>
#include "boards/board_common.h"
#include "drivers/driver_leds.h"
#include "drivers/driver_watchdog.h"
#include "drivers/driver_console.h"
#include "modules/sensor_buffer.h"

#ifndef CONSOLE_BAUDRATE_CONF
#define CONSOLE_BAUDRATE    CONSOLE_BAUDRATE_19200
#else
#define CONSOLE_BAUDRATE    CONSOLE_BAUDRATE_CONF
#endif

#define TEST_ITERATIONS     300

/**
 * @brief Console output message
 */
static char msg[100];
/**
 * @brief Sensor buffer record
 */
static sensor_buffer_record_union_t record;

/**
 * @brief   Test entry point
 *
 */
void *mainThread(void *arg0)
{
    bool res;
    uint16_t tmp, counter = 0;

    /* Test initialization */
    console_initialize(CONSOLE_BAUDRATE);
    sensor_buffer_initialize();

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

            record.record.record_header.record_size = 2;
            record.record.record_header.sensor_id = 1;
            memcpy(record.record.record, &counter, 2);
            if (!sensor_buffer_push_record(record) || !sensor_buffer_pop_record(&record)) 
            {
                res = false;
            }
            else 
            {
                memcpy(&tmp, record.record.record, 2);
                if (tmp != counter)
                {
                    res = false;
                }
            }

            memset(msg, 0, SIZE_OF_ARRAY(msg));
            sprintf(msg, "Iteration %d - Result - %s\r\n", counter, (res)? "OK" : "Fail");
            console_write(msg, strlen(msg));
        }
    }
}

#endif /* TESTS_SENSOR_BUFFER_TEST */
