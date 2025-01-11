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
 * @brief       Sensor readouts buffer implementation
 *
 * @author      Archil Pirmisashvili <archil@rf-networks.com>
 * @}
 */

#include "modules/sensor_buffer.h"
#include "boards/common_utilities.h"
#include "drivers/driver_flash.h"
#include "drivers/driver_watchdog.h"

static sens_buffer_t sensor_buffer;
static uint8_t sensor_buffer_array[SENSOR_BUFFER_RAM_SIZE];

/**
 * @brief   Get byte from sensor buffer's ring buffer
 *
 * @param[in]   byte   Pointer to retrieved byte
 *
 * @returns Seccess/fail
 *
 * This function gets byte from the sensor buffer's ring buffer
 */
static bool sensor_buffer_get_byte(uint8_t *byte);

/**
 * @brief   Put byte to sensor buffer's ring buffer
 *
 * @param[in]   byte   Byte to put
 *
 * @returns Seccess/fail
 *
 * This function puts byte to the sensor buffer's ring buffer
 */
static bool sensor_buffer_put_byte(uint8_t byte);

void sensor_buffer_initialize(void)
{
    /* Initialize RAM ring buffer */
    RingBuf_construct(&sensor_buffer.buf_obj, sensor_buffer_array, SENSOR_BUFFER_RAM_SIZE);
    sensor_buffer.buf_count = 0;
}

uint16_t sensor_buffer_get_records_count(void)
{
    return sensor_buffer.buf_count;
}

bool sensor_buffer_pop_record(sensor_buffer_record_union_p record)
{
    size_t i;
    if (record == NULL || sensor_buffer.buf_count == 0)
    {
        return false;
    }
    memset(record->array, 0, SIZE_OF_ARRAY(record->array));
    for (i = 0; i < SENSOR_RECORD_HEADER_SIZE; i++)
    {
        sensor_buffer_get_byte(record->array + i);
    }
    for (i = 0; i < MIN(SENSOR_BUFFER_RECORD_MAX_SIZE, record->record.record_header.record_size); i++)
    {
        sensor_buffer_get_byte(record->array + SENSOR_RECORD_HEADER_SIZE + i);
    }
    sensor_buffer.buf_count--;
    return true;
}

bool sensor_buffer_push_record(sensor_buffer_record_union_t record)
{
    size_t i;
    static sensor_buffer_record_union_t tmp;

    /* Free buffer if needed */
    WDT_RESET();
    while (sensor_buffer.buf_obj.count && 
        ((sensor_buffer.buf_obj.length - sensor_buffer.buf_obj.count) < MIN(sizeof(record.array), SENSOR_RECORD_HEADER_SIZE + record.record.record_header.record_size)))
    {
        /* Need to remove */
        sensor_buffer_pop_record(&tmp);
    }

    /* Put to buffer */
    for (i = 0; i < MIN(sizeof(record.array), record.record.record_header.record_size + SENSOR_RECORD_HEADER_SIZE); i++)
    {
        if (!sensor_buffer_put_byte(record.array[i]))
        {
            return false;
        }
    }
    sensor_buffer.buf_count++;
    return true;
}

bool sensor_buffer_get_byte(uint8_t *byte)
{
    if (sensor_buffer.buf_obj.count > 0)
    {
        RingBuf_get(&sensor_buffer.buf_obj, byte);
        return true;
    }
    return false;
}

bool sensor_buffer_put_byte(uint8_t byte)
{
    if (sensor_buffer.buf_obj.count < sensor_buffer.buf_obj.length)
    {
        return RingBuf_put(&sensor_buffer.buf_obj, byte);
    }
    else
    {
        return false;
    }
}
