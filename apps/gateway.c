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
 * @brief       This application implements Gateway Unit.
 *
 * @author      Archil Pirmisashvili <kulipator@gmail.com>
 * @}
 */

#ifdef APPS_GATEWAY

#include "boards/board_common.h"
#include "drivers/driver_leds.h"
#include "drivers/driver_watchdog.h"
#include "modules/settings.h"
#include "modules/radio_protocol.h"
#include "modules/console_protocol.h"

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
 * @brief Console protocol command message received callback
 * 
 * @param[in] command   Command 
 * @param[in] id        Destination ID 
 * @param[in] reg       Register number
 * @param[in] val       Register value
 */
static void console_protocol_command_message_received_cb(uint8_t command, uint32_t id, uint16_t reg, uint32_t val);

/**
 * @brief   Test entry point
 *
 */
void *mainThread(void *arg0)
{
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

    /* Initialize Console protocol */
    console_protocol_initialize();
    console_protocol_register_command_message_received_callback(&console_protocol_command_message_received_cb);

    /* Main loop */
    while (1)
    {
        /* Reset watch-dog */
        WDT_RESET();

        /* Process radio protocol */
        radio_protocol_process();

        /* Process console */
        PROCESS_CONSOLE_PROTOCOL();
    }
}

void radio_protocol_data_message_received_cb(radio_data_msg_union_p data_msg, int8_t rssi)
{
    console_protocol_send_sensor_data_message(data_msg->data.header.source_id, rssi, data_msg->data.data, data_msg->data.header.size - (sizeof(radio_data_message_t) - RADIO_MAX_PAYLOAD_SIZE - 1));
}

void radio_protocol_command_message_received_cb(radio_cmd_msg_union_p cmd_msg, int8_t rssi)
{
    if (cmd_msg->cmd.direction == 1)
    {
        console_protocol_send_command_message(cmd_msg->cmd.header.source_id, rssi, cmd_msg->cmd.command, cmd_msg->cmd.params, cmd_msg->cmd.header.size - sizeof(radio_cmd_message_t) - 1);
    }
}

void console_protocol_command_message_received_cb(uint8_t command, uint32_t id, uint16_t reg, uint32_t val)
{
    static uint8_t payload[6];
    if (command == 1)
    {
        /* Get register */
        memcpy(payload, &reg, 2);
        radio_protocol_append_command_message(id, 0, command, payload, 2);
    }
    else if (command == 2)
    {
        /* Set register */
        memcpy(payload, &reg, 2);
        memcpy(payload + 2, &val, 4);
        radio_protocol_append_command_message(id, 0, command, payload, 6);
    }
}

#endif /* APPS_GATEWAY */
