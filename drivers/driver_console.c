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
 * @brief       Console driver implementations
 *
 * @author      Archil Pirmisashvili <archil@rf-networks.com>
 * @}
 */

#include "boards/common_utilities.h"
#ifdef USE_CONSOLE
#include <math.h>
#include "boards/board_common.h"
#include "driver_console.h"
#include "driver_leds.h"

static UART_Params console_params;
static UART_Handle console_uart;
static uint8_t console_rx_buf[CONSOLE_RX_BUFFER_SIZE];
static uint8_t console_tx_buf[CONSOLE_TX_BUFFER_SIZE];
static RingBuf_Object console_rx_buf_handle;
static RingBuf_Object console_tx_buf_handle;
static uint8_t console_rx_buf_tmp[CONSOLE_TMP_BUFFER_SIZE];
static uint8_t console_tx_buf_tmp[CONSOLE_TMP_BUFFER_SIZE];
static uint8_t console_tmp_rx_msg[CONSOLE_MAX_MSG_SIZE + 1];
static size_t pointer;
static uint32_t console_byte_transmit_rat_time;

static Console_MessageReceivedCallback console_message_received_cb;
static volatile bool console_is_sending;
static volatile bool console_is_reading;
static volatile uint32_t last_rx_ts;

#if ((defined(UART_CTS) && (UART_CTS != PIN_UNASSIGNED)) || \
     (defined(UART_RTS) && (UART_RTS != PIN_UNASSIGNED)))
static PIN_Handle console_flow_ctrl_pin_handle;
static PIN_State console_flow_ctrl_pin_state;
#endif

static void uart_rx_callback(UART_Handle handle, void *rxBuf, size_t count);
static void uart_tx_callback(UART_Handle handle, void *txBuf, size_t count);
static uint32_t console_baudrate_enum_to_baudrate(console_baudrate_e baudrate);

void console_initialize(console_baudrate_e baudrate)
{
#if ((defined(UART_CTS) && (UART_CTS != PIN_UNASSIGNED)) || \
     (defined(UART_RTS) && (UART_RTS != PIN_UNASSIGNED)))
    PIN_Config pinTable[] =
    {
#if defined(UART_CTS) && (UART_CTS != PIN_UNASSIGNED)
        UART_CTS | PIN_INPUT_EN | PIN_NOPULL,
#endif
#if defined(UART_RTS) && (UART_RTS != PIN_UNASSIGNED)
        UART_RTS | PIN_INPUT_DIS | PIN_PUSHPULL | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH,
#endif
        PIN_TERMINATE
    };
#endif

    console_is_sending = false;
    console_is_reading = false;
    pointer = 0;

    last_rx_ts = ti_lib_driver_rf_get_current_time();

    /* Initialize buffers */
    ti_lib_driver_ring_buf_construct(&console_rx_buf_handle, console_rx_buf, CONSOLE_RX_BUFFER_SIZE);
    ti_lib_driver_ring_buf_construct(&console_tx_buf_handle, console_tx_buf, CONSOLE_TX_BUFFER_SIZE);

    memset(console_rx_buf_tmp, 0, sizeof(console_rx_buf_tmp));
    memset(console_tx_buf_tmp, 0, sizeof(console_tx_buf_tmp));

#ifdef CONSOLE_LED
    /* Turn off led if assigned */
    leds_single_off(CONSOLE_LED);
#endif

#if ((defined(UART_CTS) && (UART_CTS != PIN_UNASSIGNED)) || \
     (defined(UART_RTS) && (UART_RTS != PIN_UNASSIGNED)))
    console_flow_ctrl_pin_handle = ti_lib_driver_pin_open(&console_flow_ctrl_pin_state, pinTable);
    if (console_flow_ctrl_pin_handle == NULL) 
    {
        /* Initialization failed */
        while(1);
    }
#endif

    /* Initialize UART */
    ti_lib_driver_uart_init();
    ti_lib_driver_uart_params_init(&console_params);
    console_params.baudRate = console_baudrate_enum_to_baudrate(baudrate);
    console_params.readMode = UART_MODE_CALLBACK;
    console_params.writeMode = UART_MODE_CALLBACK;
    console_params.readDataMode = UART_DATA_BINARY;
    console_params.readCallback = uart_rx_callback;
    console_params.writeDataMode = UART_DATA_BINARY;
    console_params.writeCallback = uart_tx_callback;
    console_params.readEcho = UART_ECHO_OFF;

    console_byte_transmit_rat_time = ti_lib_driver_rf_convert_us_to_rat_ticks((uint32_t)ceil(8000000.0F / console_params.baudRate));

    /* Open UART and start receive */
    console_uart = ti_lib_driver_uart_open(UART0, &console_params);
    if (console_uart == NULL)
    {
        /* Can't open UART */
        while(1);
    }
    /* Enable RX internal pull-up */
    ti_lib_ioc_io_port_pull_set(UART_RX, IOC_IOPULL_UP);
    ti_lib_driver_uart_control(console_uart, UARTCC26XX_CMD_RETURN_PARTIAL_ENABLE, NULL);
    /* Start new read */
    console_is_reading = true;
    ti_lib_driver_uart_read(console_uart, console_rx_buf_tmp, sizeof(console_rx_buf_tmp));
}

void console_process(void)
{
    int i, available;
    size_t taken = 0;
#ifdef CONSOLE_END_CHAR 
    static int end_pos = -1;
    uint8_t ch;
#endif

    /* Get message from the UART */
    available = ti_lib_driver_ring_buf_get_count(&console_rx_buf_handle);
    if (available > 0)
    {
#ifdef CONSOLE_END_CHAR 
        /* End character */
        for (i = 0; (i < available) && (end_pos == -1); i++)
        {
            if (ti_lib_driver_ring_buf_get(&console_rx_buf_handle, &ch) < 0)
            {
                /* Can't retrieve character */
                available = 0;
                break;
            }
            if (ch == CONSOLE_END_CHAR)
            {
                end_pos = pointer;
            }
            console_tmp_rx_msg[pointer++] = ch;
            if (pointer >= SIZE_OF_ARRAY(console_tmp_rx_msg))
            {
                /* Buffer overflow, clear */
                pointer = 0;
                end_pos = -1;
                break;
            }
        }

        if (end_pos > -1 && pointer > 0)
        {
            /* Complete bit-stuffing message received */
            if (console_message_received_cb)
            {
                console_message_received_cb(console_tmp_rx_msg, pointer);
            }
            /* Clear message */
            memset(console_tmp_rx_msg, 0, sizeof(console_tmp_rx_msg));
            pointer = 0;
            end_pos = -1;
        }
#else
        /* Using timeout */
        if (((int32_t)(ti_lib_driver_rf_get_current_time() - last_rx_ts) > ti_lib_driver_rf_convert_ms_to_rat_ticks(CONSOLE_RX_TIMEOUT_MS)) ||
            (available >= SIZE_OF_ARRAY(console_tmp_rx_msg)))
        {

            if (available >= SIZE_OF_ARRAY(console_tmp_rx_msg))
            {
                available = SIZE_OF_ARRAY(console_tmp_rx_msg);
            }

            pointer = 0;
            memset(console_tmp_rx_msg, 0, SIZE_OF_ARRAY(console_tmp_rx_msg));
            for (i = 0; i < available; i++)
            {
                ti_lib_driver_ring_buf_get(&console_rx_buf_handle, console_tmp_rx_msg + pointer);
                pointer++;
            }
            if (pointer)
            {
                /* Complete bit-stuffing message received */
                if (console_message_received_cb)
                {
                    console_message_received_cb(console_tmp_rx_msg, pointer);
                }
            }
            /* Clear message */
            pointer = 0;
        }
#endif
    }

    /* Send if available */
    if (!console_is_sending)
    {
        /* Start transfer */
        for (taken = 0; taken < SIZE_OF_ARRAY(console_tx_buf_tmp); taken++)
        {
            if (ti_lib_driver_ring_buf_get(&console_tx_buf_handle, console_tx_buf_tmp + taken) < 0)
            {
                /* Empty */
                break;
            }
        }
        if (taken)
        {
            console_is_sending = true;
            ti_lib_driver_uart_write(console_uart, console_tx_buf_tmp, taken);
#ifdef CONSOLE_LED
            leds_single_on(CONSOLE_LED);
#endif
        }
    }

    /* Restart read if needed */
    if (!console_is_reading)
    {
        /* Start new read */
        console_is_reading = true;
        ti_lib_driver_uart_read(console_uart, console_rx_buf_tmp, sizeof(console_rx_buf_tmp));
    }
}

bool console_write(const void *buffer, size_t size)
{
    uint8_t *t_p = (uint8_t *)buffer;
    int count;
    size_t taken;

    if ((t_p == NULL) || (size < 1) || (size > CONSOLE_TX_BUFFER_SIZE))
    {
        return false;
    }

    count = ti_lib_driver_ring_buf_get_count(&console_tx_buf_handle);
    if ((count < 0) || (count >= (CONSOLE_TX_BUFFER_SIZE - size)))
    {
        /* Not enough space */
        return false;
    }

    /* Append to TX buffer */
    for (count = 0; count < size; count++)
    {
        ti_lib_driver_ring_buf_put(&console_tx_buf_handle, *(t_p + count));
    }

    if (!console_is_sending)
    {
        /* Start transfer */
        for (taken = 0; taken < SIZE_OF_ARRAY(console_tx_buf_tmp); taken++)
        {
            if (ti_lib_driver_ring_buf_get(&console_tx_buf_handle, console_tx_buf_tmp + taken) < 0)
            {
                /* No characters left in buffer */
                break;
            }
        }
        if (taken)
        {
            console_is_sending = true;
            ti_lib_driver_uart_write(console_uart, console_tx_buf_tmp, taken);
#ifdef CONSOLE_LED
            leds_single_on(CONSOLE_LED);
#endif
        }
    }
    return true;
}

void console_register_message_received_callback(Console_MessageReceivedCallback message_received_callback)
{
    console_message_received_cb = message_received_callback;
}

bool console_is_transmitting(void)
{
    return console_is_sending;
}

bool console_can_send(void)
{
    uint32_t value = false;
#if ((UART_CTS != PIN_UNASSIGNED) && (UART_RTS != PIN_UNASSIGNED))
    value = ti_lib_driver_pin_get_input_value(UART_CTS);
#endif
    return !value;
}

void console_set_is_busy(bool is_busy)
{
#if ((UART_CTS != PIN_UNASSIGNED) && (UART_RTS != PIN_UNASSIGNED))
    ti_lib_driver_pin_set_output_value(console_flow_ctrl_pin_handle, UART_RTS, is_busy);
#endif
}

void uart_rx_callback(UART_Handle handle, void *rxBuf, size_t count)
{
    size_t i;
    uint8_t *p = (uint8_t *)rxBuf;
    if (count > 0)
    {
        /* Append to buffer */
        last_rx_ts = ti_lib_driver_rf_get_current_time() + (count * console_byte_transmit_rat_time);
        for (i = 0; i < count; i++)
        {
            ti_lib_driver_ring_buf_put(&console_rx_buf_handle, *(p + i));
        }
    }
    console_is_reading = false;
}

void uart_tx_callback(UART_Handle handle, void *txBuf, size_t count)
{
    /* If TX buffer is not empty send bytes */
    if (! ti_lib_driver_ring_buf_get_count(&console_tx_buf_handle))
    {
#ifdef CONSOLE_LED
        leds_single_off(CONSOLE_LED);
#endif
    }
    console_is_sending = false;
}

uint32_t console_baudrate_enum_to_baudrate(console_baudrate_e baudrate)
{
    switch (baudrate)
    {
    case CONSOLE_BAUDRATE_1200:
        return 1200;
    case CONSOLE_BAUDRATE_2400:
        return 2400;
    case CONSOLE_BAUDRATE_4800:
        return 4800;
    case CONSOLE_BAUDRATE_9600:
        return 9600;
    case CONSOLE_BAUDRATE_14400:
        return 14400;
    case CONSOLE_BAUDRATE_19200:
        return 19200;
    case CONSOLE_BAUDRATE_28800:
        return 28800;
    case CONSOLE_BAUDRATE_38400:
        return 38400;
    case CONSOLE_BAUDRATE_56000:
        return 56000;
    case CONSOLE_BAUDRATE_57600:
        return 57600;
    case CONSOLE_BAUDRATE_115200:
        return 115200;
    default:
        return 19200;
    }
}

#endif /* USE_CONSOLE */
