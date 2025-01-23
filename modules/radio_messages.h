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
 * @brief       Radio messages definition
 *
 * @author      Archil Pirmisashvili <kulipator@gmail.com>
 */

#ifndef MODULES_RADIO_MESSAGES_H
#define MODULES_RADIO_MESSAGES_H

#include <ti-lib.h>

/**
 * @brief   Radio message max. payload size
 */
#define RADIO_MAX_PAYLOAD_SIZE              (50)

/**
 * @brief   Radio message types definitions
 */
#define RADIO_MSG_TYPE_ACKNOWLEDGE          (1)
#define RADIO_MSG_TYPE_DATA                 (2)
#define RADIO_MSG_TYPE_CMD                  (3)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Radio message structure
 */
typedef struct radio_message_header_s radio_msg_header_t, *radio_msg_header_p;

struct ATTRIBUTE_PACKED_ALIGNED(1) radio_message_header_s
{
    /* Message size from next byte to the end */
    uint8_t size;
    /* Message type */
    uint8_t msg_type;
    /* Source ID */
    uint32_t source_id;
    /* Destination ID */
    uint32_t dest_id;
    /* Message number counter */
    uint8_t msg_num;
};

/**
 * @brief   Radio message header union
 */
typedef union radio_msg_header_union_u radio_msg_header_union_t, *radio_msg_header_union_p;

union radio_msg_header_union_u
{
    radio_msg_header_t header;
    uint8_t array[11];
};

/**
 * @brief   Radio acknowledge message structure
 */
typedef struct radio_acknowledge_message_s radio_acknowledge_message_t, *radio_acknowledge_message_p;

struct ATTRIBUTE_PACKED_ALIGNED(1) radio_acknowledge_message_s
{
    /* Header */
    radio_msg_header_t header;
    /* Acknowledge message number */
    uint8_t ack_msg_num;
};

/**
 * @brief   Radio acknowledge message header union
 */
typedef union radio_acknowledge_msg_union_u radio_acknowledge_msg_union_t, *radio_acknowledge_msg_union_p;

union radio_acknowledge_msg_union_u
{
    radio_acknowledge_message_t acknowledge;
    uint8_t array[12];
};

/**
 * @brief   Radio data message structure
 */
typedef struct radio_data_message_s radio_data_message_t, *radio_data_message_p;

struct ATTRIBUTE_PACKED_ALIGNED(1) radio_data_message_s
{
    /* Header */
    radio_msg_header_t header;
    /* Data */
    uint8_t data[RADIO_MAX_PAYLOAD_SIZE];
};

/**
 * @brief   Radio data message header union
 */
typedef union radio_data_msg_union_u radio_data_msg_union_t, *radio_data_msg_union_p;

union radio_data_msg_union_u
{
    radio_data_message_t data;
    uint8_t array[11 + RADIO_MAX_PAYLOAD_SIZE];
};

/**
 * @brief   Radio command message structure
 */
typedef struct radio_cmd_message_s radio_cmd_message_t, *radio_cmd_message_p;

struct ATTRIBUTE_PACKED_ALIGNED(1) radio_cmd_message_s
{
    /* Header */
    radio_msg_header_t header;
    /* Direction (Request - 0, Response - 1) */
    uint8_t direction;
    /* Command (Get - 1, Set - 2) */
    uint8_t command;
    /* Params */
    uint16_t params[7];
};

/**
 * @brief   Radio command message header union
 */
typedef union radio_cmd_msg_union_u radio_cmd_msg_union_t, *radio_cmd_msg_union_p;

union radio_cmd_msg_union_u
{
    radio_cmd_message_t cmd;
    uint8_t array[20];
};

#ifdef __cplusplus
}
#endif

#endif /* MODULES_RADIO_MESSAGES_H */
/** @} */
