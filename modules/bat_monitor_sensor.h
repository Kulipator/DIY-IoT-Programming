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
 * @brief       On-chip battery monitor sensor management definition
 *
 * @author      Archil Pirmisashvili <kulipator@gmail.com>
 */

#ifndef SENSORS_ONCHIP_BAT_MONITOR_H
#define SENSORS_ONCHIP_BAT_MONITOR_H

#include "boards/board_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Battery monitor sensor state enumeration
 */
typedef enum bat_monitor_sensor_state bat_monitor_sensor_state_t;

enum bat_monitor_sensor_state {
    BAT_MONITOR_SENSOR_STATE_SAMPLE,
    BAT_MONITOR_SENSOR_STATE_SAMPLE_STARTED,
    BAT_MONITOR_SENSOR_STATE_NONE
};

/**
 * @brief   Battery monitor sensor result structure
 */
typedef struct bat_monitor_sensor_result_s bat_monitor_sensor_result_t, *bat_monitor_sensor_result_p;

struct ATTRIBUTE_PACKED bat_monitor_sensor_result_s
{
    /* Measurement time */
    uint32_t timestamp;
    /* Voltage value (in milli-volts) */
    uint32_t voltage;
    /* Temperature */
    int32_t temperature;
    /* Forced readout */
    bool isForced;
};

/**
 * @brief   Forward declaration for sensor measurement completed callback
 *
 * @param[in]   result   Pointer to measurement result object
 */
typedef void (*Sensor_MeasurementCompleted)(void *result);

/**
 * @brief Initialize sensor
 * 
 * @returns Initialization result
 */
bool bat_monitor_sensor_init(void);

/**
 * @brief   Start sensor readout
 *
 * @param[in]   forced   Forced readout flag
 *
 * @returns  Start success/failed
 *
 * This function starts sensor readout
 */
bool bat_monitor_sensor_read(bool forced);

/**
 * @brief   Processes sensor
 *
 * This function processes sensor
 */
void bat_monitor_sensor_process(void);

/**
 * @brief   Get prevent entering low power flag
 *
 * @returns  Prevent low power
 *
 * This function returns if the sensor does not prevent
 * entering low power mode
 */
bool bat_monitor_sensor_get_prevent_low_power(void);

/**
 * @brief Register measurement completed callback
 *
 * @param[in]   measurement_completed_callback   Callback function
 *
 * This function registers callback for sensor measurement completed event.
 * Set callback to NULL in order to disable it
 */
void bat_monitor_sensor_register_measurement_completed_callback(Sensor_MeasurementCompleted measurement_completed_callback);

#ifdef __cplusplus
}
#endif

#endif /* SENSORS_ONCHIP_BAT_MONITOR_H */
/** @} */
