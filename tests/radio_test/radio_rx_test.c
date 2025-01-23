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

/*
 * @ingroup     tests_rf_driver_rx_test
 * @{
 *
 * @file
 * @brief       This test checks receive functionality of RF driver.
 *
 * @author      Archil Pirmisashvili <kulipator@gmail.com>
 * @}
 */

#ifdef TESTS_RF_RX_TEST

#include <stdio.h>
#include "boards/board_common.h"
#include "drivers/driver_leds.h"
#include "drivers/driver_radio.h"
#include "drivers/driver_console.h"

#ifndef CONSOLE_BAUDRATE_CONF
#define CONSOLE_BAUDRATE    CONSOLE_BAUDRATE_19200
#else
#define CONSOLE_BAUDRATE    CONSOLE_BAUDRATE_CONF
#endif

#ifndef RADIO_BAUDRATE_CONF
#define RADIO_BAUDRATE      RADIO_BAUDRATE_9600
#else
#define RADIO_BAUDRATE      RADIO_BAUDRATE_CONF
#endif

#ifndef RADIO_FREQUENCY_CONF
#define RADIO_FREQUENCY     BASE_FREQUENCY_433
#else
#define RADIO_FREQUENCY     RADIO_FREQUENCY_CONF
#endif

#ifndef RADIO_SYNC_WORD_CONF
#define RADIO_SYNC_WORD     0xB56B
#else
#define RADIO_SYNC_WORD     RADIO_SYNC_WORD_CONF
#endif

#ifndef RADIO_CHANNEL_CONF
#define RADIO_CHANNEL       4
#else
#define RADIO_CHANNEL       RADIO_CHANNEL_CONF
#endif

static volatile uint32_t packets_num;
static volatile bool update;
static char msg[50];

/**
 * @brief   Radio packet received callback
 */
static void radio_rx_received_cb(radio_packet_t *packet);
/**
 * @brief   Radio receive completed callback
 */
static void radio_rx_completed_cb(bool data_received);

/**
 * @brief   Test entry point
 *
 */
void *mainThread(void *arg0)
{
    /* Test initialization */
    packets_num = 0;
    update = false;

    console_initialize(CONSOLE_BAUDRATE);

    radio_init(RADIO_BAUDRATE, RADIO_FREQUENCY, false, 0xFFFF, 0xFF);
    radio_set_sync_word(RADIO_SYNC_WORD);
    radio_set_channel(RADIO_CHANNEL, true);
    radio_register_message_received_callback(&radio_rx_received_cb);
    radio_register_command_completed_callback(&radio_rx_completed_cb);

    sprintf(msg, "Radio RX test\r\n");
    console_write(msg, strlen(msg));

    if (!radio_enable_receive(0, 0, false))
    {
        while(1);
    }

    /* Main loop */
    while(1)
    {
        /* Reset watch-dog */
        WDT_RESET();

        /* Process console */
        console_process();

        if (update) 
        {
            /* Notify user */
            update = false;
            memset(msg, 0, SIZE_OF_ARRAY(msg));
            sprintf(msg, "Total packets received: %d\r\n", packets_num);
            console_write(msg, strlen(msg));
        }
    }
}

void radio_rx_received_cb(radio_packet_t *packet)
{
    if (packet != NULL && packet->size > 0)
    {
        packets_num++;
        update = true;
    }
}

void radio_rx_completed_cb(bool data_received)
{

}

#endif /* TESTS_RF_RX_TEST */
