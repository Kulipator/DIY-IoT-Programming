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
 * @file
 * @brief       This application implements Tag Unit.
 *
 * @author      Archil Pirmisashvili <kulipator@gmail.com>
 * @}
 */

#include "boards/common_utilities.h"
#ifdef APPS_TAG

#include "boards/board_common.h"
#include "drivers/driver_leds.h"
#include "drivers/driver_watchdog.h"
#include "modules/settings.h"
#include "modules/radio_protocol.h"
#include "modules/bat_monitor_sensor.h"
#include "modules/push_button.h"
#include "modules/sensor_buffer.h"

/**
 * @brief Sensor readout was forced flag
 */
static volatile bool force_readout;

/**
 * @brief Process sensor buffer messages
 */
static void process_sensor_buffer(void);

/**
 * @brief Radio protocol data message received callback
 * 
 * @param[in] data_msg  Data message
 */
static void radio_protocol_data_message_received_cb(radio_data_msg_union_p data_msg, int8_t rssi);

/**
 * @brief Radio protocol command message received callback
 * 
 * @param[in] cmd_msg   Command message
 */
static void radio_protocol_command_message_received_cb(radio_cmd_msg_union_p cmd_msg, int8_t rssi);

/**
 * @brief Battery monitor sensor completed callback
 * 
 * @param[in] result Battery monitor sensor result
 */
static void bat_monitor_sensor_measurement_completed_cb(void *result);

/**
 * @brief Push button released callback
 * 
 * @param[in] result Push button pressed time
 */
static void push_button_released_cb(uint32_t pressed_time);

/**
 * @brief   Test entry point
 *
 */
void *mainThread(void *arg0)
{
    force_readout = false;

    /* Load settings */
    if (!settings_load())
    {
        settings_set_default();
        settings_save();
    }

#if defined(RADIO_ACTIVITY_LED) || defined(CONSOLE_LED)
    /* Blink leds to indicate program start */
    leds_blink_all(2);
#endif

    /* Initialize Radio protocol */
    radio_protocol_process();
    radio_protocol_register_data_message_received_callback(&radio_protocol_data_message_received_cb);
    radio_protocol_register_command_message_received_callback(&radio_protocol_command_message_received_cb);

    /* Initialize sensor */
    sensor_buffer_initialize();
    bat_monitor_sensor_init();
    bat_monitor_sensor_register_measurement_completed_callback(&bat_monitor_sensor_measurement_completed_cb);
    bat_monitor_sensor_process();
    push_button_init();
    push_button_register_reed_switch_released_callback(&push_button_released_cb);

    /* Main loop */
    while (1)
    {
        /* Reset watch-dog */
        WDT_RESET();

        /* Process radio protocol */
        radio_protocol_process();

        /* Process sensor */
        bat_monitor_sensor_process();

        /* Process messages */
        process_sensor_buffer();

        /* Sleep if needed */
        if (!radio_protocol_is_active() && !bat_monitor_sensor_get_prevent_low_power())
        {
            /* Can enter low power */
            SLEEP_MSECONDS(((radio_protocol_has_outgoing_message())? 1 : settings.sensor_readout_interval_sec) * 1000);
        }
    }
}

void process_sensor_buffer(void)
{
    static sensor_buffer_record_union_t record;

    if (sensor_buffer_get_records_count())
    {
        /* Message exists, forward it to radio module */
        memset(&record, 0, sizeof(record));
        if (sensor_buffer_pop_record(&record))
        {
            /* Forward as data message */
            radio_protocol_append_data_message(0xFFFFFFFFU, record.record.record, record.record.record_header.record_size);
        }
    }
}

void radio_protocol_data_message_received_cb(radio_data_msg_union_p data_msg, int8_t rssi)
{
    /* We do not expect receive data message, so we do nothing */
}

void radio_protocol_command_message_received_cb(radio_cmd_msg_union_p cmd_msg, int8_t rssi)
{
    uint16_t reg;
    uint32_t val;
    static uint8_t params[7];

    /* Parse command and respond only to requests */
    if (cmd_msg->cmd.direction == 0)
    {
        if (cmd_msg->cmd.command == 1)
        {
            /* Get register */
            memcpy(&reg, cmd_msg->cmd.params, 2);
            memcpy(params, cmd_msg->cmd.params, 2);
            memset(params + 2, 0, 4);
            params[6] = 0;
            if (reg == 1)
            {
                memcpy(params + 2, &settings.sensor_readout_interval_sec, 4);
                params[6] = 1;
            }
            radio_protocol_append_command_message(0xFFFFFFFFU, 1, 1, params, 7);
        }
        else if (cmd_msg->cmd.command == 2)
        {
            /* Set register */
            memcpy(&reg, cmd_msg->cmd.params, 2);
            memcpy(&val, cmd_msg->cmd.params + 2, 2);
            memcpy(params, cmd_msg->cmd.params, 2);
            memset(params + 2, 0, 5);

            if (reg == 0 && val == 1)
            {
                /* Reboot */
                reboot();
                return;
            }
            else if (reg == 1)
            {
                settings.sensor_readout_interval_sec = val;
                params[2] = settings_save()? 1 : 0;
            }
            radio_protocol_append_command_message(0xFFFFFFFFU, 1, 2, params, 3);
        }
    }
}

void bat_monitor_sensor_measurement_completed_cb(void *result)
{
    bat_monitor_sensor_result_p res = (bat_monitor_sensor_result_p)result;
    static sensor_buffer_record_union_t record;

    /* Put result to sensor buffer */
    memset(&record, 0, sizeof(record));
    record.record.record_header.sensor_id = 1; /* Battery monitor */
    record.record.record_header.record_size = 12;
    memcpy(record.record.record, &res->timestamp, 4);
    memcpy(record.record.record + 4, &res->voltage, 4);
    memcpy(record.record.record + 8, &res->temperature, 4);

    sensor_buffer_push_record(record);

    if (force_readout)
    {
        force_readout = false;
        SLEEP_BREAK();
    }
}

void push_button_released_cb(uint32_t pressed_time)
{
    /* Force measure voltage */
    bat_monitor_sensor_read(true);
    force_readout = true;
    SLEEP_BREAK();
}

#endif /* APPS_TAG */
