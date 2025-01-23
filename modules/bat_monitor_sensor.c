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
 * @brief       On-chip battery monitor sensor management implementation
 *
 * @author      Archil Pirmisashvili <kulipator@gmail.com>
 * @}
 */

#include "bat_monitor_sensor.h"
#include "modules/settings.h"

static bat_monitor_sensor_result_t bat_monitor_last_result;

static volatile bat_monitor_sensor_state_t bat_monitor_sensor_state;

static Sensor_MeasurementCompleted bat_monitor_sensor_measurement_completed_cb;

bool bat_monitor_sensor_init(void)
{
    bat_monitor_sensor_state = BAT_MONITOR_SENSOR_STATE_NONE;
    memset(&bat_monitor_last_result, 0, sizeof(bat_monitor_last_result));
    return true;
}

bool bat_monitor_sensor_read(bool forced)
{
    if ((bat_monitor_sensor_state != BAT_MONITOR_SENSOR_STATE_SAMPLE) && (bat_monitor_sensor_state != BAT_MONITOR_SENSOR_STATE_SAMPLE_STARTED))
    {
        bat_monitor_sensor_state = BAT_MONITOR_SENSOR_STATE_SAMPLE;
        bat_monitor_last_result.timestamp = 0;
        bat_monitor_last_result.isForced = forced;
        bat_monitor_last_result.voltage = 0;
        bat_monitor_last_result.temperature = INT32_MIN;
        return true;
    }
    return false;
}

void bat_monitor_sensor_process(void)
{
    if (bat_monitor_sensor_state == BAT_MONITOR_SENSOR_STATE_SAMPLE)
    {
        /* Start sampling */
        ti_lib_aon_batmon_enable();
        bat_monitor_sensor_state = BAT_MONITOR_SENSOR_STATE_SAMPLE_STARTED;
    }
    else if (bat_monitor_sensor_state == BAT_MONITOR_SENSOR_STATE_SAMPLE_STARTED)
    {
        if ((bat_monitor_last_result.temperature == INT32_MIN) && ti_lib_aon_batmon_new_temp_measure_ready())
        {
            /* Get temperature */
            bat_monitor_last_result.temperature = ti_lib_aon_batmon_temperature_get_deg_c();
        }
        if ((bat_monitor_last_result.voltage == 0) && ti_lib_aon_batmon_new_battery_measure_ready())
        {
            /* Get voltage */
            bat_monitor_last_result.voltage = (ti_lib_aon_batmon_battery_voltage_get() * 125) >> 5;
        }
        if (bat_monitor_last_result.voltage && bat_monitor_last_result.temperature != INT32_MIN)
        {
            /* Readout received, disable sensor for power consumption */
            ti_lib_aon_batmon_disable();
            bat_monitor_last_result.timestamp = get_up_time();

            bat_monitor_sensor_state = BAT_MONITOR_SENSOR_STATE_NONE;
            if (bat_monitor_sensor_measurement_completed_cb)
            {
                bat_monitor_sensor_measurement_completed_cb(&bat_monitor_last_result);
            }
        }
    }
    if (bat_monitor_sensor_state == BAT_MONITOR_SENSOR_STATE_NONE)
    {
        if ((settings.sensor_readout_interval_sec) && (!bat_monitor_last_result.timestamp || ((bat_monitor_last_result.timestamp + settings.sensor_readout_interval_sec) <= get_up_time())))
        {
            /* Start readout */
            bat_monitor_sensor_read(false);
        }
    }
}

bool bat_monitor_sensor_get_prevent_low_power(void)
{
    return (bat_monitor_sensor_state != BAT_MONITOR_SENSOR_STATE_NONE);
}

void bat_monitor_sensor_register_measurement_completed_callback(Sensor_MeasurementCompleted measurement_completed_callback)
{
    bat_monitor_sensor_measurement_completed_cb = measurement_completed_callback;
}
