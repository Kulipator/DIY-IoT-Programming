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
 * @ingroup     drivers
 * @{
 *
 * @file
 * @brief       Console driver definitions
 *
 * @author      Archil Pirmisashvili <kulipator@gmail.com>
 */

#ifdef USE_CONSOLE

#ifndef DRIVERS_DRIVER_CONSOLE_H
#define DRIVERS_DRIVER_CONSOLE_H

#include "boards/board_common.h"

#ifdef CONSOLE_BAUD_RATE_CONF
#define CONSOLE_BAUD_RATE           CONSOLE_BAUD_RATE_CONF
#else
#define CONSOLE_BAUD_RATE           (19200)
#endif

#ifdef CONSOLE_RX_BUFFER_SIZE_CONF
#define CONSOLE_RX_BUFFER_SIZE      CONSOLE_RX_BUFFER_SIZE_CONF
#else
#define CONSOLE_RX_BUFFER_SIZE      (1024)
#endif

#ifdef CONSOLE_TX_BUFFER_SIZE_CONF
#define CONSOLE_TX_BUFFER_SIZE      CONSOLE_TX_BUFFER_SIZE_CONF
#else
#define CONSOLE_TX_BUFFER_SIZE      (512)
#endif

#ifdef CONSOLE_MAX_MSG_SIZE_CONF
#define CONSOLE_MAX_MSG_SIZE        CONSOLE_MAX_MSG_SIZE_CONF
#else
#define CONSOLE_MAX_MSG_SIZE        (256)
#endif

#ifndef CONSOLE_END_CHAR 
#if defined(CONSOLE_CONF_RX_TIMEOUT_MS)
#define CONSOLE_RX_TIMEOUT_MS       CONSOLE_CONF_RX_TIMEOUT_MS
#else
#define CONSOLE_RX_TIMEOUT_MS       (100)
#endif
#endif

#ifdef CONSOLE_TMP_BUFFER_SIZE_CONF
#define CONSOLE_TMP_BUFFER_SIZE      CONSOLE_TMP_BUFFER_SIZE_CONF
#else
#define CONSOLE_TMP_BUFFER_SIZE  (64)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Baudrate enum
 */
typedef enum console_baudrate console_baudrate_e;

enum console_baudrate {
    CONSOLE_BAUDRATE_1200 = 0,
    CONSOLE_BAUDRATE_2400,
    CONSOLE_BAUDRATE_4800,
    CONSOLE_BAUDRATE_9600,
    CONSOLE_BAUDRATE_14400,
    CONSOLE_BAUDRATE_19200,
    CONSOLE_BAUDRATE_28800,
    CONSOLE_BAUDRATE_38400,
    CONSOLE_BAUDRATE_56000,
    CONSOLE_BAUDRATE_57600,
    CONSOLE_BAUDRATE_115200,
    CONSOLE_BAUDRATE_ERROR
};

/**
 * @brief   Forward declaration for radio message received callback
 */
typedef void (*Console_MessageReceivedCallback)(uint8_t *data, size_t size);

/**
 * @brief   Initialize console
 *
 */
void console_initialize(console_baudrate_e baudrate);

/**
 * @brief   Process console
 *
 * Because there is no thread support the function handles console
 * input stream for incoming messages. The function should be called
 * from the main loop.
 *
 */
void console_process(void);

/**
 * @brief   Write array to console
 *
 *  @param[in]   buffer     Pointer to source array
 *  @param[in]   size       Size of source array
 *
 *  @return true if was successful
 *
 *  This function starts transmitting data to console UART.
 *  If CONSOLE_LED is defined, it will turn ON on transmit start,
 *  and OFF on transmit completion.
 *
 *  @warning Do not call this function from the interrupt
 *
 */
bool console_write(const void *buffer, size_t size);

/**
 * @brief   Register console message received callback
 *
 * @param[in]   message_received_callback   Callback function
 *
 * This function registers callback for message received event.
 * Set callback to NULL in order to disable it
 */
void console_register_message_received_callback(Console_MessageReceivedCallback message_received_callback);

/**
 * @brief   Check if console is transmitting
 *
 * @returns Transmitting or not
 *
 * This function returns if the console is in transmit state
 */
bool console_is_transmitting(void);

/**
 * @brief   Check if console can send data
 *
 * @returns If console is allowed to send
 *
 * This function checks RTS pin to define if console
 * can send data
 */
bool console_can_send(void);

/**
 * @brief   Set console flow control busy pin
 *
 * @param[in]  is_busy     Is busy or not
 *
 * This function sets flow control CTS pin
 */
void console_set_is_busy(bool is_busy);

#ifdef __cplusplus
}
#endif

#endif /* DRIVERS_DRIVER_CONSOLE_H */

#endif /* USE_CONSOLE */
/** @} */
