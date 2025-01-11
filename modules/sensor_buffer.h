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
 * @brief       Sensor readouts buffer definition
 *
 * @author      Archil Pirmisashvili <archil@rf-networks.com>
 */

#ifndef MODULES_SENSOR_BUFFER_H
#define MODULES_SENSOR_BUFFER_H

#include "boards/board_common.h"

#ifndef SENSOR_BUFFER_RAM_SIZE
#define SENSOR_BUFFER_RAM_SIZE          (300)
#endif

#define SENSOR_BUFFER_RECORD_MAX_SIZE   (12)
#define SENSOR_RECORD_HEADER_SIZE       (2)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Structure containing sensor buffer record
 */
typedef struct  sensor_buffer_record_s sensor_buffer_record_t, *sensor_buffer_record_p;

struct ATTRIBUTE_PACKED_ALIGNED(1) sensor_buffer_record_s
{
    /* Header */
    struct header
    {
        /* Sensor ID */
        uint8_t sensor_id;
        /* Record size */
        uint8_t record_size;
    } record_header;
    /* Records array */
    uint8_t record[SENSOR_BUFFER_RECORD_MAX_SIZE];
};

/**
 * @brief   Sensor buffer record union
 */
typedef union sensor_buffer_record_union_u sensor_buffer_record_union_t, *sensor_buffer_record_union_p;

union sensor_buffer_record_union_u
{
    sensor_buffer_record_t record;
    uint8_t array[SENSOR_BUFFER_RECORD_MAX_SIZE + SENSOR_RECORD_HEADER_SIZE];
};


/**
 * @brief   Structure containing sensor readout buffer
 */
typedef struct  sens_buffer_s sens_buffer_t;

struct ATTRIBUTE_PACKED_ALIGNED(1) sens_buffer_s
{
    /* Ring buffer object */
    RingBuf_Object  buf_obj;
    /* Ring buffer records count */
    uint16_t buf_count;
};

/**
 * @brief   Initialize sensor buffer
 *
 * This function initializes sensor buffer
 */
void sensor_buffer_initialize(void);

/**
 * @brief   Get available records count in the sensor buffer
 *
 * @returns Records count
 *
 * This function checks number of available records count in the
 * sensor buffer
 */
uint16_t sensor_buffer_get_records_count(void);

/**
 * @brief   Pop record from sensor buffer
 *
 * @param[in]   record   Pointer to sensor buffer record
 *
 * @returns Pop success/failed
 *
 * This function pops record from the sensor buffer
 */
bool sensor_buffer_pop_record(sensor_buffer_record_union_p record);

/**
 * @brief   Push record to sensor buffer
 *
 * @param[in]   record   Sensor buffer record
 *
 * @returns Push success/failed
 *
 * This function pushes record to the sensor buffer
 */
bool sensor_buffer_push_record(sensor_buffer_record_union_t record);

#ifdef __cplusplus
}
#endif

#endif /* MODULES_SENSOR_BUFFER_H */
/** @} */
