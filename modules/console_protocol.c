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
 * @ingroup     modules
 * @{
 *
 * @file
 * @brief       Console protocol implementation
 *
 * @author      Archil Pirmisashvili <kulipator@gmail.com>
 */

#ifdef USE_CONSOLE

#include <stdio.h>
#include <string.h>
#include "modules/console_protocol.h"
#include "boards/common_utilities.h"

#ifndef CONSOLE_BAUDRATE_CONF
#define CONSOLE_BAUDRATE    CONSOLE_BAUDRATE_19200
#else
#define CONSOLE_BAUDRATE    CONSOLE_BAUDRATE_CONF
#endif

/**
 * @brief Outgoing message
 */
static uint8_t console_outgoing_message[CONSOLE_MAX_MESSAGE_LENGHT];
/**
 * @brief Comand parse message
 */
static char console_parse_msg[CONSOLE_MAX_MESSAGE_LENGHT + 1];
/**
 * @brief Console command message received callback function pointer
 */
static ConsoleProtocol_CommandMessageReceivedCallback console_command_message_received_cb;

/**
 * @brief   Console message received callback
 *
 * @param[in]   data       Pointer to message
 * @param[in]   size       Size of message
 */
static void console_protocol_console_message_received_cb(uint8_t *data, size_t size);

void console_protocol_initialize(void)
{
    console_initialize(CONSOLE_BAUDRATE);

    /* Attach callbacks */
    console_register_message_received_callback(&console_protocol_console_message_received_cb);
}

void console_protocol_console_message_received_cb(uint8_t *data, size_t size)
{
    char *p, *p1, *p2;
    uint16_t reg;
    uint32_t id, val;
    memcpy(console_parse_msg, data, size);
    console_parse_msg[size + 1] = 0;

    /* Parse incoming message */
    p = strstr(console_parse_msg, "GETREG:");
    if (p)
    {
        /* Get register command */
        p1 = strstr(p + 7, ",");
        if (p1)
        {
            /* All params exist, parse */
            *p1 = 0;
            id = atoi(p + 7);
            reg = atoi(p1 + 1);
            if (console_command_message_received_cb)
            {
                console_command_message_received_cb(1, id, reg, 0);
            }
        }
    }
    p = strstr(console_parse_msg, "SETREG:");
    if (p)
    {
        /* Set register command */
        p1 = strstr(p + 7, ",");
        if (p1)
        {
            p2 = strstr(p1 + 1, ",");
            if (p2)
            {
                /* All params exist, parse */
                *p1 = 0;
                *p2 = 0;
                id = atoi(p + 7);
                reg = atoi(p1 + 1);
                val = atoi(p2 + 1);
                if (console_command_message_received_cb)
                {
                    console_command_message_received_cb(2, id, reg, val);
                }
            }
        }
    }
}

void console_protocol_send_sensor_data_message(uint32_t id, int8_t rssi, uint8_t *payload, size_t size)
{
    uint32_t timestamp, voltage, temperature;
    memset(console_outgoing_message, 0, SIZE_OF_ARRAY(console_outgoing_message));
    if (size > 11)
    {
        memcpy(&timestamp, payload, 4);
        memcpy(&voltage, payload + 4, 4);
        memcpy(&temperature, payload + 8, 4);
    }
    sprintf((char*)console_outgoing_message, "[%d dBm] Sensor %u: Timestamp - %u, Voltage - %d.%03d V, Temperature - %d C\r", 
                rssi, 
                id, 
                timestamp, 
                (voltage / 1000), (voltage % 1000),
                temperature);
    console_write((const void *)console_outgoing_message, strlen((char*)console_outgoing_message));
}

void console_protocol_send_command_message(uint32_t id, int8_t rssi, uint8_t command, uint8_t *payload, size_t size)
{
    uint16_t reg;
    uint32_t value;
    if (command == 1)
    {
        memset(console_outgoing_message, 0, SIZE_OF_ARRAY(console_outgoing_message));
        memcpy(&reg, payload, 2);
        memcpy(&value, payload + 2, 4);
        sprintf((char*)console_outgoing_message, "[%d dBm] Sensor %u: Get register (%d) response - %s, Value - %u\r",
                    rssi, 
                    id, 
                    reg,
                    (payload[6])? "OK" : "Falied",
                    value);
        console_write((const void *)console_outgoing_message, strlen((char*)console_outgoing_message));
    }
    else if (command == 2)
    {
        memset(console_outgoing_message, 0, SIZE_OF_ARRAY(console_outgoing_message));
        memcpy(&reg, payload, 2);
        sprintf((char*)console_outgoing_message, "[%d dBm] Sensor %u: Set register (%d) response - %s\r",
                    rssi, 
                    id, 
                    reg,
                    (payload[2])? "OK" : "Falied");
        console_write((const void *)console_outgoing_message, strlen((char*)console_outgoing_message));
    }
}

void console_protocol_register_command_message_received_callback(ConsoleProtocol_CommandMessageReceivedCallback command_message_received_callback)
{
    console_command_message_received_cb = command_message_received_callback;
}

#endif /* USE_CONSOLE */
