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
 * @brief       Console protocol definition
 *
 * @author      Archil Pirmisashvili <kulipator@gmail.com>
 */

#ifdef USE_CONSOLE
#ifndef MODULES_CONSOLE_PROTOCOL_H
#define MODULES_CONSOLE_PROTOCOL_H

#include "boards/common_utilities.h"
#include "drivers/driver_console.h"

#define CONSOLE_MAX_MESSAGE_LENGHT (255)
#define PROCESS_CONSOLE_PROTOCOL() console_process()

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Forward declaration for command message received callback
 *
 * @param[in]   command Command code
 * @param[in]   id      Destination ID
 * @param[in]   reg     Register number
 * @param[in]   val     Register value
 */
typedef void (*ConsoleProtocol_CommandMessageReceivedCallback)(uint8_t command, uint32_t id, uint16_t reg, uint32_t val);

/**
 * @brief   Initialize console protocol
 *
 */
void console_protocol_initialize(void);

/**
 * @brief   Send sensor data message
 * 
 * @param id        Sensor ID
 * @param rssi      RSSI
 * @param payload   Payload
 * @param size      Payload size
 *
 * This function sends sensor data message in readable format
 */
void console_protocol_send_sensor_data_message(uint32_t id, int8_t rssi, uint8_t *payload, size_t size);

/**
 * @brief   Send command message
 * 
 * @param id        Sensor ID
 * @param rssi      RSSI
 * @param command   Command
 * @param payload   Payload
 * @param size      Payload size
 *
 * This function sends command message in readable format
 */
void console_protocol_send_command_message(uint32_t id, int8_t rssi, uint8_t command, uint8_t *payload, size_t size);

/**
 * @brief Register command message received callback
 *
 * @param[in]   cmd_message_received_callback   Callback function
 *
 * This function registers callback for data message received event.
 * Set callback to NULL in order to disable it
 */
void console_protocol_register_command_message_received_callback(ConsoleProtocol_CommandMessageReceivedCallback command_message_received_callback);

#ifdef __cplusplus
}
#endif

#endif /* MODULES_CONSOLE_PROTOCOL_H */
#endif /* USE_CONSOLE */
/** @} */
