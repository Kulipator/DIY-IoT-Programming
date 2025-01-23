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
 * @brief       Radio protocol implementation
 *
 * @author      Archil Pirmisashvili <kulipator@gmail.coms>
 * @}
 */

#include "modules/radio_protocol.h"
#include "boards/board_common.h"
#include "boards/common_utilities.h"
#include "drivers/driver_radio.h"
#include "modules/radio_messages.h"
#include "modules/settings.h"

#ifndef RADIO_BAUDRATE
#define RADIO_BAUDRATE      (RADIO_BAUDRATE_4800)
#endif
#ifndef RADIO_FREQUENCY
#define RADIO_FREQUENCY     (BASE_FREQUENCY_433)
#endif
#ifndef RADIO_ENABLE_PA
#define RADIO_ENABLE_PA     (false)
#endif
#ifndef RADIO_SYNC_WORD
#define RADIO_SYNC_WORD     (0xb56b)
#endif
#ifndef RADIO_CHANNEL
#define RADIO_CHANNEL       (4)
#endif
#ifndef RADIO_TX_POWER
#define RADIO_TX_POWER      (10)
#endif

/**
 * @brief Radio state machine state
 */
static volatile radio_state_t radio_state;

/**
 * @brief Radio incoming message bytes
 */
static uint8_t incoming_message[sizeof(radio_msg_header_t) + RADIO_MAX_PAYLOAD_SIZE + 1];

/**
 * @brief Radio outgoing message bytes
 */
static uint8_t outgoing_message[sizeof(radio_msg_header_t) + RADIO_MAX_PAYLOAD_SIZE];

/**
 * @brief Radio outgoing message number
 */
static uint8_t outgoing_message_number;

/**
 * @brief Current unit ID
 */
static uint32_t my_id;

/**
 * @brief Radio acknowledge message for fast response
 */
static radio_acknowledge_msg_union_t acknowledge_message;

/**
 * @brief Radio data message received callback function pointer
 */
static RadioProtocol_DataMessageReceivedCallback data_message_received_cb;

/**
 * @brief Radio command message received callback function pointer
 */
static RadioProtocol_CommandMessageReceivedCallback command_message_received_cb;

/**
 * @brief Initialization state of state maching
 */
static void radio_protocol_initialize_state_machine(void);

/**
 * @brief Receive state of state maching
 */
static void radio_protocol_receive_state_machine(void);

/**
 * @brief Send state of state maching
 */
static void radio_protocol_send_state_machine(void);

/**
 * @brief Process message state of state maching
 */
static void radio_protocol_process_message_state_machine(void);

/**
 * @brief Idle state of state maching
 */
static void radio_protocol_idle_state_machine(void);

/**
 * @brief   Radio message received event callback
 *
 * @param[in]   data       Pointer to received message
 * @param[in]   length     Length of received message
 *
 * This is a interrupt callback function, which is called every time the
 * packet is received from the RF module.
 */
static void radio_protocol_radio_message_received_cb(radio_packet_t *packet);

/**
 * @brief   Radio command completed event callback
 *
 * This is a interrupt callback function, which is called every time the
 * receive id completed.
 */
static void radio_protocol_radio_command_completed_cb(bool data_received);

/**
 * @brief   Radio message sent event callback
 *
 * @param[in]   sent       Message sent flag
 *
 * This is a interrupt callback function, which is called every time the
 * packet is sent to the RF module.
 */
static void radio_protocol_radio_message_sent_cb(bool sent);

void radio_protocol_process(void)
{
    static bool is_initialized = false;

    if (!is_initialized)
    {
        /* Enter only once */
        is_initialized = true;
        outgoing_message_number = 0;
#ifdef ROOT_NODE
        my_id = 0xFFFFFFFFU;
#else
        my_id = get_mac_address() & 0xFFFFFFFFU;
#endif
        radio_state = RADIO_STATE_INIT;
    }

    /* Act according to the state of the state machine */
    switch (radio_state)
    {
    case RADIO_STATE_INIT:
        /* Initialize radio state machine */
        {
            radio_protocol_initialize_state_machine();
        }
        break;
    case RADIO_STATE_RECEIVE:
        /* Start receiving incoming messages */
        {
            radio_protocol_receive_state_machine();
        }
        break;
    case RADIO_STATE_RECEIVING:
        /* Waiting for incoming messages */
        break;
    case RADIO_STATE_SENDING_ACKNOWLEDGE:
        /* Sending acknowledge message */
        break;
    case RADIO_STATE_PROCESS_MESSAGE:
        /* Process incoming message */
        {
            radio_protocol_process_message_state_machine();
        }
        break;
    case RADIO_STATE_SEND:
        /* Send outgoing message */
        {
            radio_protocol_send_state_machine();
        }
        break;
    case RADIO_STATE_SENDING:
        /* Sending outgoing message */
        break;
    case RADIO_STATE_WAITING_ACKNOWLEDGE:
        /* Waiting for acknowledge for sent message */
        break;
    case RADIO_STATE_IDLE:
        /* Idle state, nothing to do */
        {
            radio_protocol_idle_state_machine();
        }
        break;
    default:
        /* Not handled state, do nothing */
        break;
    }
}

bool radio_protocol_is_active(void)
{
    return (radio_state != RADIO_STATE_IDLE);
}

bool radio_protocol_has_outgoing_message(void)
{
    bool res;
    DEFINE_CRITICAL();

    ENTER_CRITICAL();
    res = (outgoing_message[0] != 0);
    EXIT_CRITICAL();
    return res;
}

bool radio_protocol_append_data_message(uint32_t dest_id, uint8_t *payload, size_t size)
{
    DEFINE_CRITICAL();
    radio_data_msg_union_p data_msg = (radio_data_msg_union_p)outgoing_message;
    if (outgoing_message[0] || size < 1 || payload == NULL)
    {
        return false;
    }

    /* Build message, it has to be unary operation, so wrap it in critical section */
    ENTER_CRITICAL();
    data_msg->data.header.size = sizeof(radio_data_message_t) - 1 - RADIO_MAX_PAYLOAD_SIZE + size;
    data_msg->data.header.msg_type = RADIO_MSG_TYPE_DATA;
    data_msg->data.header.source_id = my_id;
    data_msg->data.header.dest_id = dest_id;
    data_msg->data.header.msg_num = outgoing_message_number++;
    memcpy(data_msg->data.data, payload, size);
    EXIT_CRITICAL();
    return true;
}

bool radio_protocol_append_command_message(uint32_t dest_id, uint8_t direction, uint8_t command, uint8_t *payload, size_t size)
{
    DEFINE_CRITICAL();
    radio_cmd_msg_union_p cmd_msg = (radio_cmd_msg_union_p)outgoing_message;
    if (outgoing_message[0] || size < 1 || size > 7 || payload == NULL)
    {
        return false;
    }

    /* Build message, it has to be unary operation, so wrap it in critical section */
    ENTER_CRITICAL();
    cmd_msg->cmd.header.size = sizeof(radio_cmd_message_t) - 1 + size;
    cmd_msg->cmd.header.msg_type = RADIO_MSG_TYPE_CMD;
    cmd_msg->cmd.header.source_id = my_id;
    cmd_msg->cmd.header.dest_id = dest_id;
    cmd_msg->cmd.header.msg_num = outgoing_message_number++;
    cmd_msg->cmd.command = command;
    cmd_msg->cmd.direction = direction;
    memcpy(cmd_msg->cmd.params, payload, size);
    EXIT_CRITICAL();
    return true;
}

void radio_protocol_register_data_message_received_callback(RadioProtocol_DataMessageReceivedCallback data_message_received_callback)
{
    data_message_received_cb = data_message_received_callback;
}

void radio_protocol_register_command_message_received_callback(RadioProtocol_CommandMessageReceivedCallback command_message_received_callback)
{
    command_message_received_cb = command_message_received_callback;
}

void radio_protocol_initialize_state_machine(void)
{
    /* Initialize radio driver */
    radio_init(RADIO_BAUDRATE, RADIO_FREQUENCY, RADIO_ENABLE_PA, 0xFFFF, 0xFF);
    radio_set_sync_word(RADIO_SYNC_WORD);
    radio_set_channel(RADIO_CHANNEL, true);
    radio_set_tx_power(RADIO_TX_POWER);

    /* Attach radio driver callbacks */
    radio_register_message_received_callback(&radio_protocol_radio_message_received_cb);
    radio_register_command_completed_callback(&radio_protocol_radio_command_completed_cb);
    radio_register_message_sent_callback(&radio_protocol_radio_message_sent_cb);

    radio_state = RADIO_STATE_IDLE;
}

void radio_protocol_receive_state_machine(void)
{
    radio_state = RADIO_STATE_RECEIVING;
    acknowledge_message.acknowledge.header.size = 0;
#ifdef ROOT_NODE
    if (!radio_enable_receive(0, true, true))
#else
    if (!radio_enable_receive(RADIO_BYTES_TO_US(RADIO_BYTES_ON_RADIO(sizeof(radio_data_message_t)), RADIO_BAUDRATE), true, false))
#endif
    {
        radio_state = RADIO_STATE_IDLE;
    }
}

void radio_protocol_send_state_machine(void)
{
    if (!outgoing_message[0])
    {
        /* Outgoing message is empty */
#ifdef ROOT_NODE
        /* Root node goes to receive state instead of idle */
        radio_state = RADIO_STATE_RECEIVE;
#else
        radio_state = RADIO_STATE_IDLE;
#endif
    }
    else 
    {
        /* Send message */
        radio_state = RADIO_STATE_SENDING;
        if (!radio_send(outgoing_message, outgoing_message[0] + 1, 0, RADIO_BYTES_TO_US(RF_PREAMBLE_BYTES, RADIO_BAUDRATE)))
        {
            /* Failed to send */
            radio_state = RADIO_STATE_IDLE;
        }
    }
}

void radio_protocol_process_message_state_machine(void)
{
    radio_data_msg_union_p data;
    radio_cmd_msg_union_p cmd;
    int8_t rssi = incoming_message[0];
    radio_msg_header_p header = (radio_msg_header_p)(incoming_message + 1);
#ifdef ROOT_NODE
    radio_msg_header_p out_header = (radio_msg_header_p)(outgoing_message);
#endif    
    radio_state = RADIO_STATE_IDLE;
    /* Proccess message */
    if (header->size > 0)
    {
#ifdef ROOT_NODE
        if (out_header->size && out_header->dest_id == header->source_id)
        {
            radio_state = RADIO_STATE_SEND;
        }
#endif
        if (data_message_received_cb && header->msg_type == RADIO_MSG_TYPE_DATA)
        {
            /* Data message, notify observer */
            data = (radio_data_msg_union_p)(incoming_message + 1);
            data_message_received_cb(data, rssi);
        }
        else if (command_message_received_cb && header->msg_type == RADIO_MSG_TYPE_CMD)
        {
            /* Command message */
            cmd = (radio_cmd_msg_union_p)(incoming_message + 1);
            command_message_received_cb(cmd, rssi);
        }
        /* Clear message */
        memset(incoming_message, 0, SIZE_OF_ARRAY(incoming_message));
    }
}

void radio_protocol_idle_state_machine(void)
{
    if (outgoing_message[0])
    {
        /* There is outgoing message, that needs to be sent */
        radio_state = RADIO_STATE_SEND;
    }
    else
    {
#ifdef ROOT_NODE
        radio_state = RADIO_STATE_RECEIVE;
#else
        /* Put radio module to power saving mode */
        RADIO_STOP_ACTIVITY();
        radio_standby();
#endif
    }
}


void radio_protocol_radio_message_received_cb(radio_packet_t *packet)
{
    /*
     * +---------------------------------------------------------+
     * |                                                         |
     * |    !!!!!   I   M   P   O   R   T   A   N   T   !!!!!    |
     * |                                                         |
     * |    This is interrupt callback, so all operations        |
     * |    should be fast.                                      |
     * |    Also do not change mesh STATE here. Change it        |
     * |    in complete callback.                                |
     * |                                                         |
     * +---------------------------------------------------------+
     */
    radio_msg_header_p header;
    radio_acknowledge_msg_union_p ack;

    if (packet->size >= sizeof(radio_msg_header_t))
    {
        /* Recceived message has enough bytes to parse header, parse it */
        header = (radio_msg_header_p)(packet->data);

        if (header->dest_id == my_id)
        {
            /* My message */
            if (header->msg_type == RADIO_MSG_TYPE_ACKNOWLEDGE &&
                radio_state == RADIO_STATE_WAITING_ACKNOWLEDGE)
            {
                /* Acknowledge received */
                ack = (radio_acknowledge_msg_union_p)(packet->data);
                header = (radio_msg_header_p)(outgoing_message);
                if (ack->acknowledge.ack_msg_num == header->msg_num)
                {
                    /* Clear outgoing message */
                    header->size = 0;
                }
                radio_disable_receive();
            }
            else if (((header->msg_type == RADIO_MSG_TYPE_DATA) || (header->msg_type == RADIO_MSG_TYPE_CMD)) &&
                     (radio_state == RADIO_STATE_RECEIVING))
            {
                /* Message received, copy and generate acknowledge */
                incoming_message[0] = packet->rssi;
                memcpy(incoming_message + 1, packet->data, packet->size);
                acknowledge_message.acknowledge.header.dest_id = header->source_id;
                acknowledge_message.acknowledge.ack_msg_num = header->msg_num;
                acknowledge_message.acknowledge.header.size = sizeof(acknowledge_message) - 1;
                radio_disable_receive();
            }
        }
    }
}

void radio_protocol_radio_command_completed_cb(bool data_received)
{
    if (radio_state == RADIO_STATE_WAITING_ACKNOWLEDGE)
    {
#ifdef ROOT_NODE  
        radio_state = RADIO_STATE_IDLE;
#else
        radio_state = RADIO_STATE_RECEIVE;
#endif
    }
    else if (radio_state == RADIO_STATE_RECEIVING)
    {
        if (acknowledge_message.acknowledge.header.size)
        {
            /* Message received, need to send acknowledge */
            radio_state = RADIO_STATE_SENDING_ACKNOWLEDGE;
            acknowledge_message.acknowledge.header.msg_type = RADIO_MSG_TYPE_ACKNOWLEDGE;
            acknowledge_message.acknowledge.header.source_id = my_id;
            if (!radio_send(acknowledge_message.array, acknowledge_message.array[0] + 1, 0, 0))
            {
                radio_state = RADIO_STATE_PROCESS_MESSAGE;
            }
        }
        else 
        {
            radio_state = RADIO_STATE_IDLE;
        }
    }
}

void radio_protocol_radio_message_sent_cb(bool sent)
{
    if (radio_state == RADIO_STATE_SENDING)
    {
        radio_state = RADIO_STATE_WAITING_ACKNOWLEDGE;
        if (sent && radio_enable_receive(RADIO_BYTES_TO_US(RADIO_BYTES_ON_RADIO(sizeof(radio_acknowledge_message_t)), RADIO_BAUDRATE), true, false))
        {
            /* Wait for acknowledge */
            return;
        }
        radio_state = RADIO_STATE_IDLE;
    }
    else if (radio_state == RADIO_STATE_SENDING_ACKNOWLEDGE)
    {
        radio_state = RADIO_STATE_PROCESS_MESSAGE;
    }
}
