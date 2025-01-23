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
 * @brief       Radio protocol definition
 *
 * @author      Archil Pirmisashvili <kulipator@gmail.com>
 */

#ifndef MODULES_RADIO_PROTOCOL_H
#define MODULES_RADIO_PROTOCOL_H

#include "radio_messages.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Radio state machine states enumeration
 */
typedef enum radio_state radio_state_t;

enum radio_state {
    /*
     * Initialization state, is set only once in the beginning,
     * to initialize the system
     */
    RADIO_STATE_INIT = 0x00,

    /*
     * Receive states
     */
    RADIO_STATE_RECEIVE,
    RADIO_STATE_RECEIVING,

    RADIO_STATE_SENDING_ACKNOWLEDGE,
    RADIO_STATE_PROCESS_MESSAGE,

    RADIO_STATE_SEND,
    RADIO_STATE_SENDING,

    RADIO_STATE_WAITING_ACKNOWLEDGE,

    /* Idle state, do nothing */
    RADIO_STATE_IDLE
};

/**
 * @brief   Forward declaration for data message received callback
 *
 * @param[in]   data    Data message
 */
typedef void (*RadioProtocol_DataMessageReceivedCallback)(radio_data_msg_union_p data, int8_t rssi);

/**
 * @brief   Forward declaration for command message received callback
 *
 * @param[in]   cmd     Command message
 */
typedef void (*RadioProtocol_CommandMessageReceivedCallback)(radio_cmd_msg_union_p cmd, int8_t rssi);

/**
 * @brief   Process radio state machine
 *
 * This function processes radio state machine according
 * state. This function actually does the mesh logics
 * and should be called from the main loop.
 *
 */
void radio_protocol_process(void);

/**
 * @brief   Check if radio state machine is in active state
 * 
 * @returns If radio state machine is in active state
 *
 * This function is used to check if the module can enter 
 * low power mode
 */
bool radio_protocol_is_active(void);

/**
 * @brief   Check if radio has outgoing message
 * 
 * @returns If radio has outgoing message
 *
 * This function is used to check if the module has 
 * outgoing message
 */
bool radio_protocol_has_outgoing_message(void);

/**
 * @brief   Append outgoing data message
 * 
 * @param[in]   dest_id ID of the destination unit
 * @param[in]   payload Payload
 * @param[in]   size    Payload size
 *
 * @returns Success/Fail
 *
 * This function appends data message to outgoing message queue
 */
bool radio_protocol_append_data_message(uint32_t dest_id, uint8_t *payload, size_t size);

/**
 * @brief   Append outgoing command message
 * 
 * @param[in]   dest_id ID of the destination unit
 * @param[in]   direction Command direction
 * @param[in]   command Command type
 * @param[in]   payload Payload
 * @param[in]   size    Payload size
 *
 * @returns Success/Fail
 *
 * This function appends command message to outgoing message queue
 */
bool radio_protocol_append_command_message(uint32_t dest_id, uint8_t direction, uint8_t command, uint8_t *payload, size_t size);

/**
 * @brief Register data message received callback
 *
 * @param[in]   data_message_received_callback   Callback function
 *
 * This function registers callback for data message received event.
 * Set callback to NULL in order to disable it
 */
void radio_protocol_register_data_message_received_callback(RadioProtocol_DataMessageReceivedCallback data_message_received_callback);

/**
 * @brief Register command message received callback
 *
 * @param[in]   cmd_message_received_callback   Callback function
 *
 * This function registers callback for data message received event.
 * Set callback to NULL in order to disable it
 */
void radio_protocol_register_command_message_received_callback(RadioProtocol_CommandMessageReceivedCallback command_message_received_callback);

#ifdef __cplusplus
}
#endif

#endif /* MODULES_RADIO_PROTOCOL_H */
/** @} */
