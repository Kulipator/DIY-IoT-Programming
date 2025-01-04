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
 * @brief       Radio driver definitions
 *
 * @author      Archil Pirmisashvili <archil@rf-networks.com>
 */

#ifndef DRIVERS_DRIVER_RADIO_H
#define DRIVERS_DRIVER_RADIO_H

#include "boards/board_common.h"
#include "smartrf_settings/smartrf_settings.h"

#ifdef RADIO_MAX_PACKET_LENGTH_COUNT
/**
 * @brief   Max. radio packet size
 */
#define RADIO_MAX_PACKET_LENGTH                 RADIO_MAX_PACKET_LENGTH_COUNT
#else
#define RADIO_MAX_PACKET_LENGTH                 (254)
#endif

#ifndef RADIO_SNIFF_RSSI
/**
 * @brief   Radio sniff RSSI in dBm
 */
#define RADIO_SNIFF_RSSI                        (-111)
#endif

#ifndef RADIO_DEFAULT_LBT_RSSI
/**
 * @brief   Radio listen before talk default RSSI in dBm
 */
#define RADIO_DEFAULT_LBT_RSSI                  (-80)
#endif

/**
 * @brief   Number of radio transfered bytes per second 
 */
#define RADIO_BYTES_PER_SECOND(r)               ((r) >> 3) /* Divide by 8 bits (1 byte) */
/**
 * @brief   Get time in microseconds that takes to transmit b bytes using r baudrate
 */
#define RADIO_BYTES_TO_US(b, r)                 ((uint32_t)ceil((b) * 1000000.0F / RADIO_BYTES_PER_SECOND(r)))
/**
 * @brief   Time in microseconds between switching operation (TX & RX)
 */
#define RADIO_OPERATIONS_SWITCH_TIME_US         (50)
/**
 * @brief   Time in microseconds between recipient starts listening and sender starts sending
 */
#define RADIO_RX_TX_DELAY_US                    (500)
/**
 * @brief   Number of additional bytes added to message while sending over radio
 */
#define RADIO_MESSAGE_ADDITIONAL_BYTES          (RF_PREAMBLE_BYTES + 6) /* Preamble bytes + Sync Word bytes (2 bytes) + CRC bytes (2 bytes) + 2 additional as walkaround */
/**
 * @brief   Number of bytes transfered over radio
 */
#define RADIO_BYTES_ON_RADIO(x)                 ((x) + RADIO_MESSAGE_ADDITIONAL_BYTES)

#ifndef RF_QUEUE_DATA_ENTRY_HEADER_SIZE
/**
 * @brief   Constant header size of a Generic Data Entry
 */
#define RF_QUEUE_DATA_ENTRY_HEADER_SIZE         (8)
#endif

/**
 * @brief   Padding offset
 */
#define RF_QUEUE_QUEUE_ALIGN_PADDING(length)    (4 - ((length + RF_QUEUE_DATA_ENTRY_HEADER_SIZE) % 4))

/**
 * @brief   Buffer size
 */
#define RF_QUEUE_DATA_ENTRY_BUFFER_SIZE(numEntries, dataSize, appendedBytes)                                                    \
(numEntries*(RF_QUEUE_DATA_ENTRY_HEADER_SIZE + dataSize + appendedBytes + RF_QUEUE_QUEUE_ALIGN_PADDING(dataSize + appendedBytes)))

/**
 * @brief   RF command termination event mask 
 */
#define RF_TERMINATION_EVENT_MASK               (RF_EventLastCmdDone | RF_EventLastFGCmdDone | RF_EventCmdAborted | RF_EventCmdStopped | RF_EventCmdCancelled)

/**
 * @brief   Packet RX Configuration
 */
#define RF_QUEUE_NUM_DATA_ENTRIES               (2)  /* NOTE: Only two data entries supported at the moment */
#define RF_QUEUE_NUM_APPENDED_BYTES             (8)  /* The Data Entries data field will contain:
                                                      * 1 Header byte (RF_cmdPropRx.rxConf.bIncludeHdr = 0x1)
                                                      * Max RADIO_MAX_PACKET_LENGTH payload bytes
                                                      * 1 RSSI byte (RF_cmdPropRx.rxConf.bAppendRssi = 0x1)
                                                      * 2 CRC bytes (RF_cmdPropRx.rxConf.bIncludeCrc = 0x1)
                                                      * 4 Timestamp bytes (RF_cmdPropRx.rxConf.bAppendTimestamp = 0x1)
                                                      * 1 status byte (RF_cmdPropRx.rxConf.bAppendStatus = 0x1) */

/**
 * @brief   Packet low power search configuration
 */
#define CORR_PERIOD_SYM_MARGIN                  (24)
#define RX_END_TIME_SYM_MARGIN                  (32)
#define CS_END_TIME_MIN_TIME_SYM                (38)
#define CS_END_TIME_MIN_TIME_STATIC_US          (150)

/*---------------------------------------------------------------------------*/
/**
 * @brief Get current radio time
 *
 * This function will return current radio timer (RAT) ticks.
 * RAT is running on 4 Mhz speed only when radio module is on.
 */
#define radio_get_time()                        ti_lib_driver_rf_get_current_time()

/* RAT timer difference in RAT timer ticks */
#define RAT_TIME_DIFF(x, y)                     ((int32_t)((x) - (y)))

/**
 * @brief Stop radio activity (TX & RX) macro
 */
#define RADIO_STOP_ACTIVITY() { \
    radio_cancel_tx_commands(); \
    radio_disable_receive(); \
}

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief   Base frequencies enumeration
  */
typedef enum base_frequency base_frequency_e;

enum base_frequency
{
    BASE_FREQUENCY_433,     /* 433.05 - 434.79 MHz */
    BASE_FREQUENCY_866,     /* 865.0 - 867.0 MHz */
    BASE_FREQUENCY_868_G,   /* 863.0 - 870.0 MHz */
    BASE_FREQUENCY_868_G1,  /* 868.0 - 868.6 MHz */
    BASE_FREQUENCY_868_G2,  /* 868.7 - 869.2 MHz */
    BASE_FREQUENCY_868_G3,  /* 869.4 - 869.65 MHz */
    BASE_FREQUENCY_868_G4,  /* 869.7 - 870.0 MHz */
    BASE_FREQUENCY_915,     /* 902.0 - 912.0 MHz */
    BASE_FREQUENCY_ERROR
};

/**
 * @brief   Radio baud-rate enumeration
 */
typedef enum radio_baudrate radio_baudrate_e;

enum radio_baudrate
{
    RADIO_BAUDRATE_4800 = 4800,
    RADIO_BAUDRATE_9600 = 9600,
    RADIO_BAUDRATE_19200 = 19200,
    RADIO_BAUDRATE_38400 = 38400,
    RADIO_BAUDRATE_57600 = 57600,
    RADIO_BAUDRATE_115200 = 115200,
    RADIO_BAUDRATE_ERROR
};

/**
 * @brief   Forward declaration for radio_packet
 */
typedef struct radio_packet radio_packet_t;

/**
 * @brief   Radio received packet
 */
struct ATTRIBUTE_PACKED_ALIGNED(1) radio_packet
{
    int8_t rssi;
    uint32_t timestamp;
    uint8_t size;
    uint8_t data[RADIO_MAX_PACKET_LENGTH + 1];
};

/**
 * @brief   Forward declaration for radio message received callback
 */
typedef void (*RF_MessageReceivedCallback)(radio_packet_t *packet);

/**
 * @brief   Forward declaration for radio command completed callback
 *
 * @param[in]   data_received   Data received flag
 */
typedef void (*RF_CommandCompletedCallback)(bool data_received);

/**
 * @brief   Forward declaration for radio message transmit callback
 *
 * @param[in]   sent   Message sent or not (in case of LBT)
 */
typedef void (*RF_MessageTransmitCallback)(bool sent);

/**
 * @brief   Forward declaration for radio RSSI measured callback
 *
 * @param[in]   rssi   RSSI in dBm
 */
typedef void (*RF_RSSIMeasured)(int8_t rssi);

/*---------------------------------------------------------------------------*/
/**
 * @brief   Forward declaration for timer timeout callback
 */
typedef void (*Timer_TimeoutCallback)(void);
/*---------------------------------------------------------------------------*/
/**
 * @brief   Forward declaration for timer timeout callback
 *
 * @param[in]   timestamp   Event timestamp in RAT ticks
 */
typedef void (*Timer_PeriodCallback)(uint32_t timestamp);

/*---------------------------------------------------------------------------*/

/**
 * @brief Initialize the Radio
 *
 * @param[in]   baudrate        RF symbol rate
 * @param[in]   freq            Frequency
 * @param[in]   enable_pa       Enable power amplifier
 * @param[in]   deviation       Deviation
 * @param[in]   rx_bandwidth    RX bandwidth
 *
 * This function will set up RF module.
 */
void radio_init(radio_baudrate_e baudrate, base_frequency_e freq, bool enable_pa, uint16_t deviation, uint8_t rx_bandwidth);

/*---------------------------------------------------------------------------*/
/**
 * @brief Validate radio baudrate
 *
 * @param[in]   baudrate   RF symbol rate
 *
 * This function will validate baudrate and return it as enum.
 * In case of invalid baudrate, will return minimal supported baudrate.
 */
radio_baudrate_e radio_validate_baudrate(uint32_t baudrate);

/*---------------------------------------------------------------------------*/
/**
 * @brief Get available channels number
 *
 * @param[in]   freq            Base frequency
 * @param[in]   baudrate        RF symbol rate
 * @param[in]   base_frequency  Base frequency reference (can be NULL)
 * @param[in]   channel_step    Channel step reference (can be NULL)
 *
 * This function will retrieves number of available channels.
 */
uint8_t radio_get_available_channels_number(base_frequency_e freq, radio_baudrate_e baudrate, uint32_t *base_frequency, uint32_t *channel_step);

/*---------------------------------------------------------------------------*/
/**
 * @brief Open the Radio
 *
 * @returns Operation success/fail
 *
 * This function will open RF module.
 */
bool radio_open(void);

/*---------------------------------------------------------------------------*/
/**
 * @brief Put radio to standby mode
 *
 * This function puts radio to standby mode.
 */
void radio_standby(void);

/*---------------------------------------------------------------------------*/
/**
 * @brief Release radio
 *
 * This function releases radio module.
 */
void radio_close(void);

/*---------------------------------------------------------------------------*/
/**
 * @brief Register radio message received callback
 *
 * @param[in]   message_received_callback   Callback function
 *
 * This function registers callback for message received event.
 * Set callback to NULL in order to disable it
 */
void radio_register_message_received_callback(RF_MessageReceivedCallback message_received_callback);

/*---------------------------------------------------------------------------*/
/**
 * @brief Register radio message send callback
 *
 * @param[in]   message_transmit_callback   Callback function
 *
 * This function registers callback for message sent event.
 * Set callback to NULL in order to disable it
 */
void radio_register_message_sent_callback(RF_MessageTransmitCallback message_transmit_callback);

/*---------------------------------------------------------------------------*/
/**
 * @brief Register radio message receive completed callback
 *
 * @param[in]   command_completed_callback   Callback function
 *
 * This function registers callback for message receive completed event.
 * Set callback to NULL in order to disable it
 */
void radio_register_command_completed_callback(RF_CommandCompletedCallback command_completed_callback);

/*---------------------------------------------------------------------------*/
/**
 * @brief Register radio RSSI measured callback
 *
 * @param[in]   rssi_measured_callback   Callback function
 *
 * This function registers callback for RSSI measured event.
 * Set callback to NULL in order to disable it
 */
void radio_register_rssi_measured_callback(RF_RSSIMeasured rssi_measured_callback);

/*---------------------------------------------------------------------------*/
/**
 * @brief Enable reception
 *
 * @param[in]   start_time          Operation start time
 * @param[in]   timeout_us          Receive timeout in us
 * @param[in]   receive_on_timeout  Continue packet reception on timeout
 * @param[in]   continue_on_receive  Continue packet reception on timeout
 *
 * @returns Operation success/fail
 *
 * This function will enable packet receive for a defined time.
 * If timeout is set to 0, receive will last forever.
 */
bool radio_enable_receive_at(uint32_t start_time, uint32_t timeout_us, bool receive_on_timeout, bool continue_on_receive);

/*---------------------------------------------------------------------------*/
/**
 * @brief Enable reception
 *
 * @param[in]   timeout_us          Receive timeout in us
 * @param[in]   receive_on_timeout  Continue packet reception on timeout
 * @param[in]   continue_on_receive  Continue packet reception on timeout
 *
 * @returns Operation success/fail
 *
 * This function will enable packet receive for a defined time.
 * If timeout is set to 0, receive will last forever.
 */
bool radio_enable_receive(uint32_t timeout_us, bool receive_on_timeout, bool continue_on_receive);

/*---------------------------------------------------------------------------*/
/**
 * @brief Disable reception
 *
 * @returns Operation success/fail
 *
 * This function will disable packet receive.
 */
bool radio_disable_receive(void);

/*---------------------------------------------------------------------------*/
/**
 * @brief   Set radio channel
 *
 * @param[in]   channel         Channel number
 * @param[in]   open_radio      Flag to open radio after taking effect
 *
 * @returns Operation success/fail
 */
bool radio_set_channel(uint8_t channel, bool open_radio);

/**
 * @brief   Set radio TX power
 *
 * @param[in]   power         Power in dBm
 *
 * @returns Operation success/fail
 *
 * This function sets radio TX power
 */
bool radio_set_tx_power(int8_t power);

/**
 * @brief   Decrease radio TX power
 *
 * @param[in]   power         Power in dBm
 *
 * @returns Power exists or not
 *
 * This function checks if TX power exists
 */
bool radio_check_tx_power(int8_t power);

/**
 * @brief   Decrease radio TX power
 *
 * @param[in]   power         Current power in dBm
 *
 * @returns Operation success/fail
 *
 * This function decreases radio TX power one step down
 */
bool radio_decrease_tx_power(int8_t power);

/**
 * @brief   Increase radio TX power
 *
 * @param[in]   power         Current power in dBm
 *
 * This function increases radio TX power one step up
 */
bool radio_increase_tx_power(int8_t power);

/**
 * @brief   Set radio sync word
 *
 * This function sets radio sync word
 */
void radio_set_sync_word(uint16_t sync_word);

/**
 * @brief   Generate modulated test signal
 *
 * @param[in]   duration        Duration in seconds
 *
 * @returns Operation success/fail
 *
 * This function generates modulated test signal for
 * defined period in seconds
 */
bool radio_test_modulated_tx(uint8_t duration);

/**
 * @brief   Generate unmodulated test signal
 *
 * @param[in]   duration        Duration in seconds
 *
 * @returns Operation success/fail
 *
 * This function generates unmodulated test signal for
 * defined period in seconds
 */
bool radio_test_unmodulated_tx(uint8_t duration);

/**
 * @brief   Send data
 *
 * @param[in]   data            Pointer to data
 * @param[in]   size            Size of data
 * @param[in]   lbt_rssi        Listen Before Talk threshold RSSI
 * @param[in]   lbt_timeout_us  Listen Before Talk timeout in microseconds (0 - to disable LBT)
 *
 * @returns Operation success/fail
 *
 * This function sends data over the radio
 */
bool radio_send(uint8_t *data, size_t size, int8_t lbt_rssi, uint32_t lbt_timeout_us);

/**
 * @brief   Send data at defined RAT time-stamp
 *
 * @param[in]   data            Pointer to data
 * @param[in]   size            Size of data
 * @param[in]   timestamp       Time-stamp in RAT ticks
 * @param[in]   lbt_rssi        Listen Before Talk threshold RSSI
 * @param[in]   lbt_timeout_us  Listen Before Talk timeout in microseconds (0 - to disable LBT)
 *
 * @returns Operation success/fail
 *
 * This function sends data over the radio at defined RAT timestamp
 */
bool radio_send_at(uint8_t *data, size_t size, uint32_t timestamp, int8_t lbt_rssi, uint32_t lbt_timeout_us);

/**
 *  @brief   Cancel radio TX
 */
void radio_cancel_tx_commands(void);

/**
 * @brief   Get radio driver handler
 *
 * @returns Handler to radio driver
 */
RF_Handle radio_get_handle(void);

/**
 * @brief   Adjust the XOSC HF cap array relative to the factory setting
 *
 * @param[in]   delta           Capacity delta
 *
 * @returns Operation success/fail
 */
bool radio_change_capacity_delta(int8_t delta);

/**
 * @brief   Sniff network for packets
 *
 * @param[in]   timeout             Timeout in microseconds
 *
 * @returns Operation success/fail
 *
 * This function sniffs radio frequency for incoming packets. Radio stays in sleep
 * mode and wakes up every defined period to listen for incoming packet. This is
 * done in order to save power
 */
bool radio_sniff_packet(uint32_t timeout);

/**
 * @brief   Sniff network for packets
 *
 * @param[in]   start_time          Sniff start time
 * @param[in]   timeout             Timeout in microseconds
 *
 * @returns Operation success/fail
 *
 * This function sniffs radio frequency for incoming packets. Radio stays in sleep
 * mode and wakes up every defined period to listen for incoming packet. This is
 * done in order to save power
 */
bool radio_sniff_packet_at(uint32_t start_time, uint32_t timeout);

/**
 * @brief   Get reception level
 *
 * @returns Operation success/fail
 *
 * This function gets RSSI of the sniffed transmission
 */
bool radio_test_RSSI(void);

/**
 * @brief   Get reception level
 *
 * @param[in]   start_time          Start time
 *
 * @returns Operation success/fail
 *
 * This function gets RSSI of the sniffed transmission
 */
bool radio_test_RSSI_at(uint32_t start_time);

/**
 * @brief   Get reception level
 *
 * @returns Current RSSI level
 *
 * This function gets current RSSI level
 */
int8_t radio_get_RSSI(void);

/*---------------------------------------------------------------------------*/
/**
 * @brief Set timeout
 *
 * @param[in]   timeout_us Timeout in microseconds
 * @param[in]   callback   Callback function
 *
 * @returns     Operation success or failure
 *
 * This function will call callback function when timeout expires.
 */
bool radio_rat_timer_set_timeout(uint32_t timeout_us, Timer_TimeoutCallback callback);

/*---------------------------------------------------------------------------*/
/**
 * @brief Start period timer
 *
 * @param[in]   period_us  Period in microseconds
 * @param[in]   callback   Callback function
 *
 * @returns     Operation success or failure
 *
 * This function will call callback function periodically.
 */
bool radio_rat_timer_start_period(uint32_t period_us, Timer_PeriodCallback callback);

/*---------------------------------------------------------------------------*/
/**
 * @brief Stop period timer
 *
 * This function will stop periodic timer.
 */
void radio_rat_timer_stop_period(void);

/*---------------------------------------------------------------------------*/
/**
 * @brief Cancel timeout
 *
 * This function will cancels timeout.
 */
void radio_rat_timer_cancel_timeout(void);

#ifdef __cplusplus
}
#endif

#endif /* DRIVERS_DRIVER_RADIO_H */
/** @} */
