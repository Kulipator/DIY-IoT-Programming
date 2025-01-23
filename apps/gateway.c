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

    /* Main loop */
    while (1)
    {
        /* Reset watch-dog */
        WDT_RESET();

        /* Process radio protocol */
        radio_protocol_process();
    }
}

void radio_protocol_data_message_received_cb(radio_data_msg_union_p data_msg, int8_t rssi)
{

}

void radio_protocol_command_message_received_cb(radio_cmd_msg_union_p cmd_msg, int8_t rssi)
{

}

#endif /* APPS_GATEWAY */
