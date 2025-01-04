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
 * @brief       Radio driver implementations
 *
 * @author      Archil Pirmisashvili <archil@rf-networks.com>
 * @}
 */

#include <ti-lib.h>
#ifdef RADIO_ACTIVITY_LED
#include "driver_leds.h"
#endif
#include "driver_radio.h"
#include "driver_watchdog.h"

/**
 * @brief Default TX power settings for the 431-527MHz band
 */
ATTRIBUTE_GPRAM("radio_driver_tx_power_table") static RF_TxPowerTable_Entry radio_driver_tx_power_table[] = {
    { .power = -10,  .value = { .rawValue = 0x04c0, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 0,  .value = { .rawValue = 0x0ec1, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 2,  .value = { .rawValue = 0x12c1, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 3,  .value = { .rawValue = 0x16c2, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 4,  .value = { .rawValue = 0x18c2, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 5,  .value = { .rawValue = 0x20c3, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 6,  .value = { .rawValue = 0x24c4, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 7,  .value = { .rawValue = 0x2ac5, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 8,  .value = { .rawValue = 0x2ec6, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 9,  .value = { .rawValue = 0x38c8, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 10,  .value = { .rawValue = 0x46cb, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 11,  .value = { .rawValue = 0x4e48, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 12,  .value = { .rawValue = 0x784e, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 13,  .value = { .rawValue = 0xd80f, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 14,  .value = { .rawValue = 0xb83f, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 15,  .value = { .rawValue = 0x913f, .paType = RF_TxPowerTable_HighPA }},
    RF_TxPowerTable_TERMINATION_ENTRY
};

/**
 * @brief Default TX power settings for the 779-930MHz band
 */
ATTRIBUTE_GPRAM("radio_driver_tx_power_table_779_930") static RF_TxPowerTable_Entry radio_driver_tx_power_table_779_930[] = {
    { .power = -10,  .value = { .rawValue = 0x08c0, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 0,  .value = { .rawValue = 0x0041, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 1,  .value = { .rawValue = 0x10c3, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 2,  .value = { .rawValue = 0x1042, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 3,  .value = { .rawValue = 0x14c4, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 4,  .value = { .rawValue = 0x18c5, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 5,  .value = { .rawValue = 0x18c6, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 6,  .value = { .rawValue = 0x1cc7, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 7,  .value = { .rawValue = 0x20c9, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 8,  .value = { .rawValue = 0x24cb, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 9,  .value = { .rawValue = 0x2ccd, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 10,  .value = { .rawValue = 0x38d3, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 11,  .value = { .rawValue = 0x50da, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 12,  .value = { .rawValue = 0xb818, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 13,  .value = { .rawValue = 0xa63f, .paType = RF_TxPowerTable_DefaultPA }},
    { .power = 14,  .value = { .rawValue = 0xa73f, .paType = RF_TxPowerTable_DefaultPA }},
    RF_TxPowerTable_TERMINATION_ENTRY
};

/**
 * @brief Default TX power settings for the 779-930MHz band with power amplifier enabled
 */
ATTRIBUTE_GPRAM("radio_driver_tx_power_table_779_930_pa") static RF_TxPowerTable_Entry radio_driver_tx_power_table_779_930_pa[] = {
     { 12, RF_TxPowerTable_DEFAULT_PA_ENTRY(0, 3, 0, 0) },
     { 18, RF_TxPowerTable_DEFAULT_PA_ENTRY(1, 3, 0, 0) },
     { 21, RF_TxPowerTable_DEFAULT_PA_ENTRY(2, 3, 0, 0) },
     { 23, RF_TxPowerTable_DEFAULT_PA_ENTRY(3, 3, 0, 0) },
     { 24, RF_TxPowerTable_DEFAULT_PA_ENTRY(4, 3, 0, 0) },
     { 25, RF_TxPowerTable_DEFAULT_PA_ENTRY(6, 3, 0, 0) },
     { 26, RF_TxPowerTable_DEFAULT_PA_ENTRY(11, 3, 0, 0) },
     RF_TxPowerTable_TERMINATION_ENTRY
};

/*
 *  =============================== RF Driver ===============================
 */
const RFCC26XX_HWAttrsV2 RFCC26XX_hwAttrs =
{
    .hwiPriority = ~0, /* Lowest HWI priority */
    .swiPriority = 0, /* Lowest SWI priority */
    .xoscHfAlwaysNeeded = true, /* Keep XOSC dependency while in standby */

    /* Register the board specific callback */
    /* Subscribe the callback to both events */
    .globalEventMask = RF_GlobalEventRadioSetup | RF_GlobalEventRadioPowerDown
};


/**
 * @brief   Buffer which contains all Data Entries for receiving data.
 *          Pragmas are needed to make sure this buffer is 4 byte aligned (requirement from the RF Core) 
 */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_ALIGN (radio_driver_rx_data_entry_buffer, 4)
static uint8_t
radio_driver_rx_data_entry_buffer[RF_QUEUE_DATA_ENTRY_BUFFER_SIZE(RF_QUEUE_NUM_DATA_ENTRIES,
                                                  (RADIO_MAX_PACKET_LENGTH + 1),
                                                  RF_QUEUE_NUM_APPENDED_BYTES)];
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment = 4
static uint8_t
radio_driver_rx_data_entry_buffer[RF_QUEUE_DATA_ENTRY_BUFFER_SIZE(RF_QUEUE_NUM_DATA_ENTRIES,
                                                  (RADIO_MAX_PACKET_LENGTH + 1),
                                                  RF_QUEUE_NUM_APPENDED_BYTES)];
#elif defined(__GNUC__)
static uint8_t
radio_driver_rx_data_entry_buffer[RF_QUEUE_DATA_ENTRY_BUFFER_SIZE(RF_QUEUE_NUM_DATA_ENTRIES,
                                                  (RADIO_MAX_PACKET_LENGTH + 1),
                                                  RF_QUEUE_NUM_APPENDED_BYTES)]
                                                  __attribute__((aligned(4)));
#else
#error This compiler is not supported.
#endif

/**
 * @brief   Radio driver handle
 */
static RF_Handle radio_driver_rf_handle = NULL;
/**
 * @brief   Radio configuration parameters
 */
static RF_Params radio_driver_rf_params;
/**
 * @brief   Radio driver object
 */
static RF_Object radio_driver_rf_object;
/**
 * @brief   Radio RX command handler
 */
static RF_CmdHandle radio_driver_rx_cmd_handle;
/**
 * @brief   Radio TX command handler
 */
static RF_CmdHandle radio_driver_tx_cmd_handle;
/**
 * @brief   Radio TX test command handler
 */
static RF_CmdHandle radio_driver_tx_test_cmd_handle;
/**
 * @brief   Radio RX packet received flag
 */
static volatile bool radio_driver_data_received_flag;
/**
 * @brief   Radio test RSSI value
 */
static volatile int8_t radio_driver_test_rssi;
/**
 * @brief   Radio data queue
 */
static dataQueue_t radio_driver_rf_data_queue;

/**
 * @brief   Receive entry pointer to keep track of read items
 */
rfc_dataEntryGeneral_t* radio_driver_read_entry;

/**
 * @brief   Radio message received callback handler
 */
static RF_MessageReceivedCallback radio_driver_rf_message_received_cb;
/**
 * @brief   Radio message sent callback handler
 */
static RF_MessageTransmitCallback radio_driver_rf_message_sent_cb;
/**
 * @brief   Radio command completed callback handler
 */
static RF_CommandCompletedCallback radio_driver_rf_command_completed_cb;
/**
 * @brief   Radio RSSI measured callback handler
 */
static RF_RSSIMeasured radio_driver_rf_rssi_measured_cb;

/**
 * @brief   Radio baudrate
 */
static uint32_t radio_driver_rf_baudrate;

/**
 * @brief   Radio band
 */
static base_frequency_e radio_driver_rf_band;

/**
 * @brief   Radio base frequency
 */
static uint32_t radio_driver_base_frequency;

/**
 * @brief   Radio base frequency channel step
 */
static uint32_t radio_driver_frequency_channel_step;

/**
 * @brief   Radio current TX power table
 */
static RF_TxPowerTable_Entry* radio_driver_tx_powers;

/**
 * @brief   Sniff command for doing combined Carrier Sense and RX
 */
static rfc_propRxOutput_t radio_driver_rx_statistics;

/**
 * @brief   Radio enable power amplifier
 */
static bool radio_enable_pa;

/**
 * @brief   Radio power amplifier pins handle
 */
static PIN_Handle ampPinHandle;

/**
 * @brief   Radio power amplifier pins state
 */
static PIN_State ampPinState;

/**
 * @brief   RAT timer variables
 */
static RF_RatConfigCompare radio_driver_rat_timer_periodic_config;
static RF_RatHandle radio_driver_rat_timer_periodic_handle;
static RF_RatConfigCompare radio_driver_rat_timer_timeout_config;
static RF_RatHandle radio_driver_rat_timer_timeout_handle;
static uint32_t radio_driver_rat_timer_period_us;
static Timer_PeriodCallback radio_driver_rat_timer_period_callback;
static Timer_TimeoutCallback radio_driver_rat_timer_timeout_callback;

/**
 * @brief Enable reception
 *
 * @param[in]   start_time          Operation start time
 * @param[in]   timeout_us          Receive timeout in us
 * @param[in]   receive_on_timeout  Continue packet reception on timeout
 * @param[in]   use_start_time      Use receive start time or start operation now
 * @param[in]   continue_on_receive Continue reception if packet received when timeout is enabled
 *
 * @returns Operation success/fail
 *
 * This function will enable packet receive for a defined time.
 * If timeout is set to 0, receive will last forever.
 */
bool radio_enable_receive_internal(uint32_t start_time, uint32_t timeout_us, bool receive_on_timeout, bool use_start_time, bool continue_on_receive);

/**
 * @brief   Send data
 *
 * @param[in]   data            Pointer to data
 * @param[in]   size            Size of data
 * @param[in]   timestamp       TX start time-stamp in RAT ticks
 * @param[in]   lbt_rssi        Listen Before Talk threshold RSSI
 * @param[in]   lbt_timeout_us  Listen Before Talk timeout in microseconds (0 - to disable LBT)
 * @param[in]   use_start_time  Use receive start time or start operation now
 *
 * @returns Operation success/fail
 *
 * This function sends data over the radio at defined RAT timestamp
 */
bool radio_send_internal(uint8_t *data, size_t size, uint32_t timestamp, int8_t lbt_rssi, uint32_t lbt_timeout_us, bool use_start_time);

/**
 * @brief   Sniff network for packets
 *
 * @param[in]   start_time          Sniff start time
 * @param[in]   timeout             Timeout in microseconds
 * @param[in]   use_start_time      Use receive start time or start operation now
 *
 * @returns Operation success/fail
 *
 * This function sniffs radio frequency for incoming packets. Radio stays in sleep
 * mode and wakes up every defined period to listen for incoming packet. This is
 * done in order to save power
 */
bool radio_sniff_packet_internal(uint32_t start_time, uint32_t timeout, bool use_start_time);

/**
 * @brief   Get reception level
 *
 * @param[in]   start_time          Start time
 * @param[in]   use_start_time      Use receive start time or start operation now
 *
 * @returns Operation success/fail
 *
 * This function gets RSSI of the sniffed transmission
 */
bool radio_test_RSSI_internal(uint32_t start_time, bool use_start_time);

/**
 * @brief   Radio receive operation callback
 */
static void radio_receive_callback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e);
/**
 * @brief   Radio transmit operation callback
 */
static void radio_transmit_callback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e);
/**
 * @brief   Radio test RSSI callback
 */
static void radio_test_RSSI_callback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e);

/**
 * @brief Move to next dataEntry
 */
static uint8_t radio_rf_queue_nextEntry(void);

/**
 * @brief Get the current dataEntry
 *
 * @returns rfc_dataEntry*
 */
static rfc_dataEntryGeneral_t* radio_rf_queue_get_data_entry(void);

/**
 * @brief Define a queue
 *
 * @param[in]   dataQueue   Pointer to the queue to use
 * @param[in]   buf         Prealocated byte buffer to use
 * @param[in]   buf_len     The number of preallocated bytes
 * @param[in]   numEntries  The number of dataEntries to split the buffer into
 * @param[in]   length      The length of data in every dataEntry
 *
 * @returns uint8_t
 */
static uint8_t radio_rf_queue_define_queue(dataQueue_t *queue, uint8_t *buf, uint16_t buf_len, uint8_t numEntries, uint16_t length);

/**
 * @brief   RAT timer period callback
 */
static void rf_rat_timer_periodic_cb(RF_Handle h, RF_RatHandle rh, RF_EventMask e, uint32_t compareCaptureTime);
/**
 * @brief   RAT timer timeout callback
 */
static void rf_rat_timer_timeout_cb(RF_Handle h, RF_RatHandle rh, RF_EventMask e, uint32_t compareCaptureTime);

void radio_init(radio_baudrate_e baudrate, base_frequency_e freq, bool enable_pa, uint16_t deviation, uint8_t rx_bandwidth)
{
    /* Power amplifier is controlled by 3 pins, so we need to initialize them in case we are using power amplifier */
    static PIN_Config ampPinTable[] = {
#if defined(Board_PIN_HGM) && Board_PIN_HGM != IOID_UNUSED
         Board_PIN_HGM | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
#endif
#if defined(Board_PIN_LNA_EN) && Board_PIN_LNA_EN != IOID_UNUSED
         Board_PIN_LNA_EN | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
#endif
#if defined(Board_PIN_PA_EN) && Board_PIN_PA_EN != IOID_UNUSED
         Board_PIN_PA_EN | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
#endif
         PIN_TERMINATE
    };
    /* Validate baudrate */
    if (baudrate >= RADIO_BAUDRATE_ERROR)
    {
        while(1);
    }
    if (radio_driver_rf_handle)
    {
        /* Close if is already opened */
        WDT_RESET();
        radio_close();
    }
#ifdef RADIO_ACTIVITY_LED
    /* If radio led is defined, bind it to the radio module. Actually we are routing IO to the RF module output pin */
    leds_single_set_mux(RADIO_ACTIVITY_LED, PINCC26XX_MUX_RFC_GPO2);
#endif

    if (ampPinTable[0] != PIN_TERMINATE)
    {
        /* In case we are using power amplifier (PA), try to initialize PA control pins */
        ampPinHandle = ti_lib_driver_pin_open(&ampPinState, ampPinTable);

        if (ampPinHandle == NULL) {
            /* Initialization failed */
            while(1);
        }
    }

    /* In case of using PA, set control pins initial state and route LNA, PA control pins to the RF module */
#if defined(Board_PIN_HGM) && Board_PIN_HGM != IOID_UNUSED
    ti_lib_driver_pin_set_output_value(ampPinHandle, Board_PIN_HGM, (enable_pa)? 1 : 0);
#endif

#if defined(Board_PIN_LNA_EN) && Board_PIN_LNA_EN != IOID_UNUSED
    ti_lib_driver_pin_set_mux(ampPinHandle, Board_PIN_LNA_EN, PINCC26XX_MUX_RFC_GPO0);
#endif
#if defined(Board_PIN_PA_EN) && Board_PIN_PA_EN != IOID_UNUSED
    ti_lib_driver_pin_set_mux(ampPinHandle, Board_PIN_PA_EN, PINCC26XX_MUX_RFC_GPO1);
#endif

    /* Initialize module variables */
    radio_driver_rf_handle = NULL;
    radio_driver_rx_cmd_handle = 0;
    radio_driver_tx_cmd_handle = 0;
    radio_driver_tx_test_cmd_handle = 0;
    radio_driver_rf_message_received_cb = NULL;
    radio_driver_rf_message_sent_cb = NULL;
    radio_driver_rf_command_completed_cb = NULL;
    radio_driver_rat_timer_period_callback = NULL;
    radio_driver_rat_timer_timeout_callback = NULL;
    radio_driver_rat_timer_periodic_handle = RF_ALLOC_ERROR;
    radio_driver_rat_timer_timeout_handle = RF_ALLOC_ERROR;
    radio_enable_pa = enable_pa;

    /* Initialize radio data message RX queue. This is actually radio module RX buffer */
    if (radio_rf_queue_define_queue(&radio_driver_rf_data_queue,
                                    radio_driver_rx_data_entry_buffer,
                                    sizeof(radio_driver_rx_data_entry_buffer),
                                    RF_QUEUE_NUM_DATA_ENTRIES,
                                    (RADIO_MAX_PACKET_LENGTH + RF_QUEUE_NUM_APPENDED_BYTES + 1)))
    {
        /* Store error code and wait for watchdog reset */
        while(1);
    }

    /* Initialize radio parameters */
    ti_lib_driver_rf_params_init(&radio_driver_rf_params);
    cmdRx.pQueue = &radio_driver_rf_data_queue;
    cmdPropCs.pNextOp = (rfc_radioOp_t*) &cmdTx;

    radio_driver_rf_baudrate = baudrate;
    radio_driver_rf_band = freq;
    /* Set speed */
    RF_cmdPropRadioDivSetup.symbolRate.rateWord = ((uint64_t)radio_driver_rf_baudrate * 0xF00000ULL / 0x16E3600ULL) & 0x1FFFFF;

    /* Set frequency */
    if (radio_get_available_channels_number(freq, baudrate, &radio_driver_base_frequency, &radio_driver_frequency_channel_step) == 0)
    {
        /* No available channels */
        while(1);
    }
    /* According to required frequency, we need to use one of two available frequency ranges (Low or High) */
    RF_cmdPropRadioDivSetup.loDivider = (freq == BASE_FREQUENCY_433)? 0x0A : 0x05; 
    RF_cmdPropRadioDivSetup.pRegOverride = (freq == BASE_FREQUENCY_433)? pOverrides : pOverrides868;
    radio_driver_tx_powers = (freq == BASE_FREQUENCY_433)? radio_driver_tx_power_table : ((radio_enable_pa)? radio_driver_tx_power_table_779_930_pa : radio_driver_tx_power_table_779_930);

    RF_cmdPropRadioDivSetup.centerFreq = (uint16_t)(radio_driver_base_frequency / 100);

    /*
     * Optimal Deviation & BW filter
     *
     * Carson's bandwidth rule
     * DataRate = 2 x Deviation
     * OBW ~ DataRate + 2 x Deviation
     */
    switch (baudrate)
    {
    case RADIO_BAUDRATE_4800:
    case RADIO_BAUDRATE_9600:
        /*
         * Deviation 5kHz, OBW 39kHz
         */
        RF_cmdPropRadioDivSetup.modulation.deviation = 0x14; /* 5 << 2 */
        RF_cmdPropRadioDivSetup.rxBw = 0x20;
        break;
    case RADIO_BAUDRATE_19200:
        /*
         * Deviation 10kHz, OBW 49kHz
         */
        RF_cmdPropRadioDivSetup.modulation.deviation = 0x28; /* 10 << 2 */
        RF_cmdPropRadioDivSetup.rxBw = 0x21;
        break;
    case RADIO_BAUDRATE_38400:
        /*
         * Deviation 20kHz, OBW 78kHz
         */
        RF_cmdPropRadioDivSetup.modulation.deviation = 0x50; /* 20 << 2 */
        RF_cmdPropRadioDivSetup.rxBw = 0x23;
        break;
    case RADIO_BAUDRATE_57600:
        /*
         * Deviation 30kHz, OBW 98kHz
         */
        RF_cmdPropRadioDivSetup.modulation.deviation = 0x50; /* 30 << 2 */
        RF_cmdPropRadioDivSetup.rxBw = 0x24;
        break;
    case RADIO_BAUDRATE_115200:
        /*
         * Deviation 60kHz, OBW 236kHz
         */
        RF_cmdPropRadioDivSetup.modulation.deviation = 0x50; /* 60 << 2 */
        RF_cmdPropRadioDivSetup.rxBw = 0x28;
        break;
    default:
        break;
    }

    if (deviation != 0xFFFF)
    {
        /* Apply custom deviation if provided */
        RF_cmdPropRadioDivSetup.modulation.deviation = deviation;
    }
    if (rx_bandwidth != 0xFF)
    {
        /* Apply custom RX bandwidth if provided */
        RF_cmdPropRadioDivSetup.rxBw = rx_bandwidth;
    }
}

radio_baudrate_e radio_validate_baudrate(uint32_t baudrate)
{
    switch (baudrate)
    {
    case 4800:
        return RADIO_BAUDRATE_4800;
    case 9600:
        return RADIO_BAUDRATE_9600;
    case 19200:
        return RADIO_BAUDRATE_19200;
    case 38400:
        return RADIO_BAUDRATE_38400;
    case 57600:
        return RADIO_BAUDRATE_57600;
    case 115200:
        return RADIO_BAUDRATE_115200;
    default:
        return RADIO_BAUDRATE_ERROR;
    }
}

uint8_t radio_get_available_channels_number(base_frequency_e freq, radio_baudrate_e baudrate, uint32_t *base_frequency, uint32_t *channel_step)
{
    /* Each frequency spectrum has range, which can be splitted to channels according channel frequency window width */
    uint8_t ret = 0;
    switch (freq)
    {
    case BASE_FREQUENCY_433:
        ret = (baudrate < RADIO_BAUDRATE_115200)? 8 : 4;
        if (base_frequency)
        {
            *base_frequency = 43302;
        }
        if (channel_step)
        {
            *channel_step = 20; /* In 10KHz */
        }
        break;
    case BASE_FREQUENCY_866:
        ret = (baudrate < RADIO_BAUDRATE_115200)? 10 : 0;
        if (base_frequency)
        {
            *base_frequency = 86500;
        }
        if (channel_step)
        {
            *channel_step = 20; /* In 10KHz */
        }
        break;
    case BASE_FREQUENCY_868_G:
        if (baudrate < RADIO_BAUDRATE_19200)
        {
            ret = 60;
        }
        else if (baudrate == RADIO_BAUDRATE_19200)
        {
            ret = 20;
        }
        else if (baudrate == RADIO_BAUDRATE_38400)
        {
            ret = 10;
        }
        break;
    case BASE_FREQUENCY_868_G1:
        if (baudrate < RADIO_BAUDRATE_19200)
        {
            ret = 12;
        }
        else if (baudrate == RADIO_BAUDRATE_19200)
        {
            ret = 6;
        }
        else if (baudrate == RADIO_BAUDRATE_38400)
        {
            ret = 3;
        }
        else
        {
            ret = 1;
        }
        break;
    case BASE_FREQUENCY_868_G2:
        if (baudrate < RADIO_BAUDRATE_19200)
        {
            ret = 10;
        }
        else if (baudrate == RADIO_BAUDRATE_19200)
        {
            ret = 5;
        }
        else if (baudrate == RADIO_BAUDRATE_38400)
        {
            ret = 2;
        }
        else
        {
            ret = 1;
        }
        break;
    case BASE_FREQUENCY_868_G3:
        ret = (baudrate < RADIO_BAUDRATE_115200)? 1 : 0;
        break;
    case BASE_FREQUENCY_868_G4:
        if (baudrate < RADIO_BAUDRATE_19200)
        {
            ret = 6;
        }
        else if (baudrate == RADIO_BAUDRATE_19200)
        {
            ret = 3;
        }
        else if (baudrate == RADIO_BAUDRATE_38400)
        {
            ret = 2;
        }
        break;
    case BASE_FREQUENCY_915:
        ret = (baudrate < RADIO_BAUDRATE_38400)? 20 : 0;
        if (base_frequency)
        {
            *base_frequency = 90200;
        }
        if (channel_step)
        {
            *channel_step = 50; /* In 10KHz */
        }
        break;
    default:
        break;
    }
    return ret;
}

bool radio_open(void)
{
    /* Open radio driver */
    radio_driver_rf_handle = ti_lib_driver_rf_open((RF_Object *)&radio_driver_rf_object, (RF_Mode *)&RF_prop, (RF_RadioSetup*)&RF_cmdPropRadioDivSetup, (RF_Params *)&radio_driver_rf_params);
    if (!radio_driver_rf_handle)
    {
        /* Failed opening driver */
        radio_driver_rf_handle = NULL;
        while(1);
    }
    WDT_RESET();
    /* Configure frequency synthesizer */
    if (ti_lib_driver_rf_run_cmd(radio_driver_rf_handle, (RF_Op*) &RF_cmdFs, RF_PriorityNormal, (RF_Callback)NULL, 0) == (RF_EventMask)RF_ALLOC_ERROR)
    {
        return false;
    }
    /* Initialize radnom number generator */
    srand(ti_lib_driver_rf_get_current_time());
    return true;
}

void radio_standby(void)
{
    if (radio_driver_rf_handle)
    {
        /* Abort all pending commands */
        RADIO_STOP_ACTIVITY();
        ti_lib_driver_rf_flush_cmd(radio_driver_rf_handle, RF_CMDHANDLE_FLUSH_ALL, RF_ABORT_PREEMPTION);
        /* Go to sleep */
        ti_lib_driver_rf_yield(radio_driver_rf_handle);
    }
}

void radio_close(void)
{
    if (radio_driver_rf_handle)
    {
        /* Abort all pending commands */
        RADIO_STOP_ACTIVITY();
        ti_lib_driver_rf_flush_cmd(radio_driver_rf_handle, RF_CMDHANDLE_FLUSH_ALL, RF_ABORT_PREEMPTION);
        ti_lib_driver_rf_close(radio_driver_rf_handle);
        radio_driver_rf_handle = NULL;
        radio_driver_rx_cmd_handle = 0;
        radio_driver_tx_cmd_handle = 0;
        radio_driver_tx_test_cmd_handle = 0;
    }
}

void radio_register_message_received_callback(RF_MessageReceivedCallback message_received_callback)
{
    radio_driver_rf_message_received_cb = message_received_callback;
}

void radio_register_message_sent_callback(RF_MessageTransmitCallback message_transmit_callback)
{
    radio_driver_rf_message_sent_cb = message_transmit_callback;
}

void radio_register_command_completed_callback(RF_CommandCompletedCallback command_completed_callback)
{
    radio_driver_rf_command_completed_cb = command_completed_callback;
}

void radio_register_rssi_measured_callback(RF_RSSIMeasured rssi_measured_callback)
{
    radio_driver_rf_rssi_measured_cb = rssi_measured_callback;
}

bool radio_enable_receive_at(uint32_t start_time, uint32_t timeout_us, bool receive_on_timeout, bool continue_on_receive)
{
    return radio_enable_receive_internal(start_time, timeout_us, receive_on_timeout, true, continue_on_receive);
}

bool radio_enable_receive(uint32_t timeout_us, bool receive_on_timeout, bool continue_on_receive)
{
    return radio_enable_receive_internal(0, timeout_us, receive_on_timeout, false, continue_on_receive);
}

bool radio_disable_receive(void)
{
    if (radio_driver_rf_handle)
    {
        /* In case of continous RX, we need to prevent entering endless loop on RX stop, by disabling NextOp command */
        cmdRx.pNextOp = NULL;
        if (radio_driver_rx_cmd_handle)
        {
            /* Disable currently active RX command */
            ti_lib_driver_rf_flush_cmd(radio_driver_rf_handle, radio_driver_rx_cmd_handle, RF_ABORT_GRACEFULLY);
            ti_lib_driver_rf_flush_cmd(radio_driver_rf_handle, RF_CMDHANDLE_FLUSH_ALL, RF_ABORT_GRACEFULLY);
            WDT_RESET();
            radio_driver_rx_cmd_handle = 0;
            return true;
        }
    }
    return false;
}

bool radio_set_channel(uint8_t channel, bool open_radio)
{
    uint32_t frequency = radio_driver_base_frequency + (radio_driver_frequency_channel_step >> 1) + (channel * radio_driver_frequency_channel_step);
    if (channel > radio_get_available_channels_number(radio_driver_rf_band, (radio_baudrate_e)radio_driver_rf_baudrate, NULL, NULL))
    {
        /* Not available */
        return false;
    }
    RF_cmdFs.frequency = (uint16_t)(frequency / 100);
    RF_cmdFs.fractFreq = (uint16_t)((frequency % 100) * 0xFFFF / 100);
    RF_cmdFs.status = IDLE;

    /* Need to restart radio in order to take effect */
    if (open_radio)
    {
        if (radio_driver_rf_handle)
        {
            radio_close();
        }
        //ti_lib_osc_adjust_xosc_hf_cap_array(get_capacity_delta());
        radio_open();
    }
    else if (radio_driver_rf_handle)
    {
        ti_lib_driver_rf_post_cmd(radio_driver_rf_handle, (RF_Op*)&RF_cmdFs, RF_PriorityNormal, NULL, 0);
    }
    return true;
}

bool radio_set_tx_power(int8_t power)
{
    RF_TxPowerTable_Value power_val = RF_TxPowerTable_findValue(radio_driver_tx_powers, power);
    if (power_val.rawValue == RF_TxPowerTable_INVALID_VALUE)
    {
        return false;
    }
    RF_cmdPropRadioDivSetup.txPower = power_val.rawValue;
    return (ti_lib_driver_rf_set_tx_power(radio_driver_rf_handle, power_val) == RF_StatSuccess);
}

bool radio_check_tx_power(int8_t power)
{
    RF_TxPowerTable_Value power_val = RF_TxPowerTable_findValue(radio_driver_tx_powers, power);
    return (power_val.rawValue != RF_TxPowerTable_INVALID_VALUE);
}

bool radio_decrease_tx_power(int8_t power)
{
    uint8_t i = 0;
    int8_t found_power = RF_TxPowerTable_INVALID_DBM;
    /* Search in available TX power list */
    while ((radio_driver_tx_powers[i].power != RF_TxPowerTable_INVALID_DBM) && (radio_driver_tx_powers[i + 1].power != RF_TxPowerTable_INVALID_DBM))
    {
        if (radio_driver_tx_powers[i].power == power)
        {
            found_power = radio_driver_tx_powers[i + 1].power;
            break;
        }
        i++;
    }
    if (found_power != RF_TxPowerTable_INVALID_DBM)
    {
        /* Can decrease */
        return radio_set_tx_power(found_power);
    }
    return false;
}

bool radio_increase_tx_power(int8_t power)
{
    uint8_t i = 0;
    int8_t found_power = RF_TxPowerTable_INVALID_DBM;
    /* Search in available TX power list */
    while (radio_driver_tx_powers[i].power != RF_TxPowerTable_INVALID_DBM)
    {
        if ((i > 0) && (radio_driver_tx_powers[i].power == power))
        {
            found_power = radio_driver_tx_powers[i - 1].power;
            break;
        }
        i++;
    }
    if (found_power != RF_TxPowerTable_INVALID_DBM)
    {
        /* Can increase */
        return radio_set_tx_power(found_power);
    }
    return false;
}

void radio_set_sync_word(uint16_t sync_word)
{
    cmdRx.syncWord0 = sync_word;
    cmdTx.syncWord = sync_word;
    RF_cmdTxTest.syncWord = sync_word;
    RF_cmdPropRxAdvSniff.syncWord0 = sync_word;
}

bool radio_test_modulated_tx(uint8_t duration)
{
    /* 
     * Modulated TX is sequence of ones and zeros bits that is transmitted over the air, which means that the
     * generated frequency varies from lower to higher and back, according defined frequency deviation.
     */
    if (!radio_driver_rf_handle)
    {
        /* Driver is not initialized */
        return false;
    }

    /* Disable all active RX or TX commands */
    RADIO_STOP_ACTIVITY();

    RF_cmdTxTest.startTime = 0x00000000;
    RF_cmdTxTest.startTrigger.triggerType = TRIG_NOW;
    RF_cmdTxTest.startTrigger.pastTrig = 1;
    RF_cmdTxTest.endTime = (duration << 2) * 1000000UL;
    RF_cmdTxTest.config.bUseCw = 0;
    RF_cmdTxTest.config.bFsOff = 1;
    RF_cmdTxTest.config.whitenMode = 2;
    RF_cmdTxTest.txWord = 0xAAAA;
    RF_cmdTxTest.endTrigger.triggerType = (duration)? TRIG_REL_START : TRIG_NEVER;
    RF_cmdTxTest.status = IDLE;

#ifdef RADIO_ACTIVITY_LED
    /* If radio led is defined, bind it to the radio module */
    leds_single_set_mux(RADIO_ACTIVITY_LED, PINCC26XX_MUX_RFC_GPO3);
#endif

    /* Start TX */
    radio_driver_tx_test_cmd_handle = ti_lib_driver_rf_post_cmd(radio_driver_rf_handle, (RF_Op*) &RF_cmdTxTest, RF_PriorityNormal, &radio_transmit_callback, 0);

    if (radio_driver_tx_test_cmd_handle == RF_ALLOC_ERROR)
    {
        /* TX failed */
        radio_driver_tx_test_cmd_handle = 0;
        return false;
    }
    return true;
}

bool radio_test_unmodulated_tx(uint8_t duration)
{
    /* 
     * Unmodulated TX is a tone (stable center frequency) that is transmitted over the air.
     * This function is basically used to validate module calibration, which means that the generated frequency is just as expected.
     */
    if (!radio_driver_rf_handle)
    {
        /* Driver is not initialized */
        return false;
    }

    /* Disable all active RX or TX commands */
    RADIO_STOP_ACTIVITY();

    RF_cmdTxTest.startTime = 0x00000000;
    RF_cmdTxTest.startTrigger.triggerType = TRIG_NOW;
    RF_cmdTxTest.startTrigger.pastTrig = 1;
    RF_cmdTxTest.endTime = (duration << 2) * 1000000UL;
    RF_cmdTxTest.config.bUseCw = 1;
    RF_cmdTxTest.config.bFsOff = 1;
    RF_cmdTxTest.config.whitenMode = 2;
    RF_cmdTxTest.txWord = 0xFFFF;
    RF_cmdTxTest.endTrigger.triggerType = (duration)? TRIG_REL_START : TRIG_NEVER;
    RF_cmdTxTest.status = IDLE;

#ifdef RADIO_ACTIVITY_LED
    /* If radio led is defined, bind it to the radio module */
    leds_single_set_mux(RADIO_ACTIVITY_LED, PINCC26XX_MUX_RFC_GPO3);
#endif

    /* Start TX */
    radio_driver_tx_test_cmd_handle = ti_lib_driver_rf_post_cmd(radio_driver_rf_handle, (RF_Op*) &RF_cmdTxTest, RF_PriorityNormal, &radio_transmit_callback, 0);

    if (radio_driver_tx_test_cmd_handle == RF_ALLOC_ERROR)
    {
        /* TX failed */
        radio_driver_tx_test_cmd_handle = 0;
        return false;
    }
    return true;
}

bool radio_send(uint8_t *data, size_t size, int8_t lbt_rssi, uint32_t lbt_timeout_us)
{
    return radio_send_internal(data, size, 0, lbt_rssi, lbt_timeout_us, false);
}

bool radio_send_at(uint8_t *data, size_t size, uint32_t timestamp, int8_t lbt_rssi, uint32_t lbt_timeout_us)
{
    return radio_send_internal(data, size, timestamp, lbt_rssi, lbt_timeout_us, true);
}

void radio_cancel_tx_commands(void)
{
    if (radio_driver_rf_handle)
    {
        /* Cancel previous command if not canceled */
        if (radio_driver_tx_test_cmd_handle)
        {
            ti_lib_driver_rf_flush_cmd(radio_driver_rf_handle, radio_driver_tx_test_cmd_handle, 0);
            radio_driver_tx_test_cmd_handle = 0;
        }
        if (radio_driver_tx_cmd_handle)
        {
            ti_lib_driver_rf_flush_cmd(radio_driver_rf_handle, radio_driver_tx_cmd_handle, RF_ABORT_GRACEFULLY);
            radio_driver_tx_cmd_handle = 0;
        }
    }
}

RF_Handle radio_get_handle(void)
{
    return radio_driver_rf_handle;
}

bool radio_change_capacity_delta(int8_t delta)
{
    /* When changing calibration value (capacity delta) of the RF module, the module should be reinitialized. */
    bool reenable_receive = ((radio_driver_rx_cmd_handle) && (!cmdRx.endTime));
    radio_close();
    ti_lib_osc_adjust_xosc_hf_cap_array(delta);
    radio_open();
    if (reenable_receive)
    {
        /* Start RX if was active while applying the change */
        return radio_enable_receive(0, false, true);
    }
    return true;
}

bool radio_sniff_packet(uint32_t timeout)
{
    return radio_sniff_packet_internal(0, timeout, false);
}

bool radio_sniff_packet_at(uint32_t start_time, uint32_t timeout)
{
    return radio_sniff_packet_internal(start_time, timeout, true);
}

bool radio_test_RSSI(void)
{
    return radio_test_RSSI_internal(0, false);
}

bool radio_test_RSSI_at(uint32_t start_time)
{
    return radio_test_RSSI_internal(start_time, true);
}

int8_t radio_get_RSSI(void)
{
    if (!radio_driver_rf_handle)
    {
        /* Driver is not initialized */
        return (int8_t)RF_GET_RSSI_ERROR_VAL;
    }

    return ti_lib_driver_rf_get_rssi(radio_driver_rf_handle);
}

bool radio_rat_timer_set_timeout(uint32_t timeout_us, Timer_TimeoutCallback callback)
{
    if (!timeout_us || !callback)
    {
        return false;
    }

    radio_driver_rat_timer_timeout_callback = NULL;
    radio_rat_timer_cancel_timeout();
    radio_driver_rat_timer_timeout_callback = callback;

    ti_lib_driver_rf_rat_config_compare_init(&radio_driver_rat_timer_timeout_config);
    radio_driver_rat_timer_timeout_config.callback = &rf_rat_timer_timeout_cb;
    radio_driver_rat_timer_timeout_config.channel = RF_RatChannelAny;
    radio_driver_rat_timer_timeout_config.timeout = radio_get_time() + ti_lib_driver_rf_convert_us_to_rat_ticks(timeout_us);
    radio_driver_rat_timer_timeout_handle = ti_lib_driver_rf_rat_compare(radio_get_handle(), &radio_driver_rat_timer_timeout_config, NULL);
    return true;
}

bool radio_rat_timer_start_period(uint32_t period_us, Timer_PeriodCallback callback)
{
    if (!period_us || !callback || radio_driver_rat_timer_periodic_handle != RF_ALLOC_ERROR)
    {
        return false;
    }

    radio_driver_rat_timer_period_us = period_us;
    radio_driver_rat_timer_period_callback = callback;

    ti_lib_driver_rf_rat_config_compare_init(&radio_driver_rat_timer_periodic_config);
    radio_driver_rat_timer_periodic_config.callback = &rf_rat_timer_periodic_cb;
    radio_driver_rat_timer_periodic_config.channel = RF_RatChannelAny;
    radio_driver_rat_timer_periodic_config.timeout = radio_get_time() + ti_lib_driver_rf_convert_us_to_rat_ticks(period_us);
    radio_driver_rat_timer_periodic_handle = ti_lib_driver_rf_rat_compare(radio_get_handle(), &radio_driver_rat_timer_periodic_config, NULL);
    return true;
}

void radio_rat_timer_stop_period(void)
{
    if (radio_driver_rat_timer_periodic_handle != RF_ALLOC_ERROR)
    {
        ti_lib_driver_rf_rat_disable_channel(radio_get_handle(), radio_driver_rat_timer_periodic_handle);
        radio_driver_rat_timer_periodic_handle = RF_ALLOC_ERROR;
    }
}

void radio_rat_timer_cancel_timeout(void)
{
    if (radio_driver_rat_timer_timeout_handle != RF_ALLOC_ERROR)
    {
        ti_lib_driver_rf_rat_disable_channel(radio_get_handle(), radio_driver_rat_timer_timeout_handle);
        radio_driver_rat_timer_timeout_handle = RF_ALLOC_ERROR;
    }
}

void radio_receive_callback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e)
{
    radio_packet_t packet;
    uint8_t payload_length, *data_p;
    rfc_dataEntryGeneral_t* currentDataEntry;
    uint8_t rssi_offset = 1;
    RF_EventMask mask = RF_EventRxOk | RF_EventRxEntryDone;

    if (!cmdRx.rxConf.bAutoFlushCrcErr)
    {
        mask |= RF_EventRxNOk;
    }
    if (e & mask)
    {
        currentDataEntry = radio_rf_queue_get_data_entry();
        while ((currentDataEntry != NULL) && (currentDataEntry->status == DATA_ENTRY_FINISHED) && cmdRx.rxConf.bAppendRssi && cmdRx.rxConf.bAppendTimestamp)
        {
            WDT_RESET();
            /* Can access entry */
            if (cmdRx.rxConf.bIncludeCrc)
            {
                rssi_offset += 2;
            }
            if (currentDataEntry->length > 7)
            {
                data_p = (uint8_t *) (&(currentDataEntry->data));
                payload_length = data_p[0];
                if (payload_length > 0) {
                    memset(&packet, 0, sizeof(radio_packet_t));
                    /* Copy data to packet structure */
                    packet.size = ((payload_length < RADIO_MAX_PACKET_LENGTH)? (payload_length) : RADIO_MAX_PACKET_LENGTH) + 1;
                    packet.rssi = data_p[payload_length + rssi_offset];
                    memcpy(&packet.timestamp, data_p + payload_length + rssi_offset + 1, 4);
                    memcpy(&packet.data, data_p, packet.size + 1);
                    radio_driver_data_received_flag = true;
                    if (radio_driver_rf_message_received_cb)
                    {
                        radio_driver_rf_message_received_cb(&packet);
                    }
                }
            }
            radio_rf_queue_nextEntry();
            currentDataEntry = radio_rf_queue_get_data_entry();
        }
    }
    if (e & RF_TERMINATION_EVENT_MASK)
    {
        /* RX command completed */
        if (radio_driver_rf_command_completed_cb)
        {
            radio_driver_rf_command_completed_cb(radio_driver_data_received_flag);
        }
        radio_driver_data_received_flag = false;
    }
}

void radio_transmit_callback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e)
{
    bool sent = false;
    if (ch == radio_driver_tx_cmd_handle)
    {
        sent = (cmdTx.status == PROP_DONE_OK);
        radio_driver_tx_cmd_handle = 0;
    }
    else if (ch == radio_driver_tx_test_cmd_handle)
    {
        radio_driver_tx_test_cmd_handle = 0;
        sent = true;
        /* After unmodulated TX frequency is changed (may be bug), so need to reset frequency */
        ti_lib_driver_rf_post_cmd(radio_driver_rf_handle, (RF_Op*)&RF_cmdFs, RF_PriorityNormal, NULL, 0);
    }
    if ((radio_driver_rf_message_sent_cb) && (e & RF_TERMINATION_EVENT_MASK))
    {
        /* Call registered callback function */
        radio_driver_rf_message_sent_cb(sent);
    }
}

void radio_test_RSSI_callback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e)
{
    if ((radio_driver_rf_rssi_measured_cb) && (e & RF_TERMINATION_EVENT_MASK))
    {
        radio_driver_rf_rssi_measured_cb(radio_driver_test_rssi);
    }
    radio_driver_test_rssi = 0;
}

uint8_t radio_rf_queue_nextEntry(void)
{
    /* Set status to pending */
    radio_driver_read_entry->status = DATA_ENTRY_PENDING;

    /* Move read entry pointer to next entry */
    radio_driver_read_entry = (rfc_dataEntryGeneral_t*)radio_driver_read_entry->pNextEntry;

    return (radio_driver_read_entry->status);
}

rfc_dataEntryGeneral_t* radio_rf_queue_get_data_entry(void)
{
    return (radio_driver_read_entry);
}

uint8_t radio_rf_queue_define_queue(dataQueue_t *queue, uint8_t *buf, uint16_t buf_len, uint8_t numEntries, uint16_t length)
{
    uint8_t pad;
    uint8_t *first_entry;
    int i;

    if (buf_len < (numEntries * (length + RF_QUEUE_DATA_ENTRY_HEADER_SIZE + RF_QUEUE_QUEUE_ALIGN_PADDING(length))))
    {
        /* queue does not fit into buffer */
        return (1);
    }

    /* Padding needed for 4-byte alignment? */
    pad = RF_QUEUE_QUEUE_ALIGN_PADDING(length);

    /* Set the Data Entries common configuration */
    first_entry = buf;
    for (i = 0; i < numEntries; i++)
    {
        buf = first_entry + i * (RF_QUEUE_DATA_ENTRY_HEADER_SIZE + length + pad);
        ((rfc_dataEntry_t*)buf)->status        = DATA_ENTRY_PENDING;        // Pending - starting state
        ((rfc_dataEntry_t*)buf)->config.type   = DATA_ENTRY_TYPE_GEN;       // General Data Entry
        ((rfc_dataEntry_t*)buf)->config.lenSz  = 0;                         // No length indicator byte in data
        ((rfc_dataEntry_t*)buf)->length        = length;                    // Total length of data field

        ((rfc_dataEntryGeneral_t*)buf)->pNextEntry = &(((rfc_dataEntryGeneral_t*)buf)->data)+length+pad;
    }
    /* Make circular Last.Next -> First */
    ((rfc_dataEntry_t*)buf)->pNextEntry = first_entry;

    /* Create Data Entry Queue and configure for circular buffer Data Entries */
    queue->pCurrEntry = first_entry;
    queue->pLastEntry = NULL;

    /* Set read pointer to first entry */
    radio_driver_read_entry = (rfc_dataEntryGeneral_t*)first_entry;

    return (0);
}

void rf_rat_timer_periodic_cb(RF_Handle h, RF_RatHandle rh, RF_EventMask e, uint32_t compareCaptureTime)
{
    if (radio_driver_rat_timer_period_callback)
    {
        radio_driver_rat_timer_period_callback(radio_driver_rat_timer_periodic_config.timeout);
    }

    if (radio_driver_rat_timer_periodic_handle != RF_ALLOC_ERROR)
    {
        /* Setup next event */
        radio_driver_rat_timer_periodic_config.timeout += ti_lib_driver_rf_convert_us_to_rat_ticks(radio_driver_rat_timer_period_us);
        radio_driver_rat_timer_periodic_handle = ti_lib_driver_rf_rat_compare(radio_get_handle(), &radio_driver_rat_timer_periodic_config, NULL);
    }
}

void rf_rat_timer_timeout_cb(RF_Handle h, RF_RatHandle rh, RF_EventMask e, uint32_t compareCaptureTime)
{
    radio_rat_timer_cancel_timeout();
    if (radio_driver_rat_timer_timeout_callback)
    {
        radio_driver_rat_timer_timeout_callback();
    }
}

bool radio_enable_receive_internal(uint32_t start_time, uint32_t timeout_us, bool receive_on_timeout, bool use_start_time, bool continue_on_receive)
{
    int32_t diff;

    if (!radio_driver_rf_handle)
    {
        /* Driver is not initialized */
        return false;
    }

    /* Disable all active RX or TX commands */
    RADIO_STOP_ACTIVITY();

    if (use_start_time)
    {
        diff = RAT_TIME_DIFF(start_time, radio_get_time());
        if (diff < 1)
        {
            /* Start time is missed */
            use_start_time = false;
            if ((timeout_us) && (ti_lib_driver_rf_convert_rat_ticks_to_us(-diff) < timeout_us))
            {
                timeout_us -= ti_lib_driver_rf_convert_rat_ticks_to_us(-diff);
            }
        }
    }
    /* Subscribe reader */
    cmdRx.pNextOp = (RF_Op*) &cmdRx;
    cmdRx.condition.rule = (timeout_us)? ((!continue_on_receive)? COND_NEVER: COND_STOP_ON_FALSE) : COND_ALWAYS;
    cmdRx.startTrigger.triggerType = (use_start_time)? TRIG_ABSTIME : TRIG_NOW;
    cmdRx.startTime = (use_start_time)? start_time : 0;
    cmdRx.startTrigger.pastTrig = 1;
    cmdRx.maxPktLen = RADIO_MAX_PACKET_LENGTH + 1;
    cmdRx.pktConf.bUseCrc = 1;
    cmdRx.endTime = ti_lib_driver_rf_convert_us_to_rat_ticks(timeout_us);
    /* Receive till the end if has timeout */
    cmdRx.pktConf.endType = (!timeout_us && !receive_on_timeout)? 1 : 0;
    cmdRx.endTrigger.triggerType = (timeout_us)? TRIG_REL_START : TRIG_NEVER;
    cmdRx.status = IDLE;
    radio_driver_data_received_flag = false;
#ifdef RADIO_ACTIVITY_LED
    /* If radio led is defined, bind it to the radio module */
    leds_single_set_mux(RADIO_ACTIVITY_LED, PINCC26XX_MUX_RFC_GPO2);
#endif
    /* Start RX */
    radio_driver_rx_cmd_handle = ti_lib_driver_rf_post_cmd(radio_driver_rf_handle, (RF_Op*) &cmdRx, RF_PriorityNormal, &radio_receive_callback,
                                         RF_EventCmdDone | RF_EventRxOk | RF_EventRxEntryDone);
    if (radio_driver_rx_cmd_handle == RF_ALLOC_ERROR)
    {
        /* Receive failed */
        radio_driver_rx_cmd_handle = 0;
        return false;
    }
    return true;
}

bool radio_send_internal(uint8_t *data, size_t size, uint32_t timestamp, int8_t lbt_rssi, uint32_t lbt_timeout_us, bool use_start_time)
{
    if ((!radio_driver_rf_handle) || (size > (RADIO_MAX_PACKET_LENGTH + 1)) || (size < 1) || (data == NULL))
    {
        /* Driver is not initialized */
        return false;
    }

    /* Disable all active RX or TX commands */
    RADIO_STOP_ACTIVITY();

    if (timestamp && (RAT_TIME_DIFF(timestamp, radio_get_time()) < 40))
    {
        /* Start time is missed */
        timestamp = 0;
        use_start_time = false;
    }

    cmdTx.pktLen = (uint16_t)size;
    cmdTx.pPkt = data;
#ifdef RADIO_ACTIVITY_LED
    /* If radio led is defined, bind it to the radio module */
    leds_single_set_mux(RADIO_ACTIVITY_LED, PINCC26XX_MUX_RFC_GPO3);
#endif
    cmdTx.status = IDLE;
    if (lbt_timeout_us)
    {
        /* Setup listen before talk (LBT) */
        cmdTx.startTime = 0;
        cmdTx.startTrigger.triggerType = TRIG_NOW;
        cmdTx.startTrigger.pastTrig = 0;

        cmdNop.startTime = timestamp;
        cmdNop.startTrigger.triggerType = (use_start_time)? TRIG_ABSTIME : TRIG_NOW;
        cmdNop.startTrigger.pastTrig = 1;

        /* Set up the next pointers for the command chain */
        cmdNop.pNextOp = (rfc_radioOp_t*)&cmdPropCs;
        cmdPropCs.pNextOp = (rfc_radioOp_t*)&cmdCountBranch;
        cmdCountBranch.pNextOp = (rfc_radioOp_t*)&cmdTx;
        cmdCountBranch.pNextOpIfOk = (rfc_radioOp_t*)&cmdPropCs;

        /* Customize the API commands with application specific defines */
        cmdPropCs.rssiThr = (lbt_rssi && lbt_rssi > -120)? lbt_rssi : RADIO_DEFAULT_LBT_RSSI;
        cmdPropCs.csEndTime = ti_lib_driver_rf_convert_us_to_rat_ticks(lbt_timeout_us);
        cmdCountBranch.counter = (lbt_timeout_us <= 400)? 1 : ((lbt_timeout_us - 150) / 250);

        cmdNop.status = IDLE;
        cmdPropCs.status = IDLE;
        cmdCountBranch.status = IDLE;

        radio_driver_tx_cmd_handle = ti_lib_driver_rf_post_cmd(radio_driver_rf_handle, (RF_Op*) &cmdNop, RF_PriorityNormal, &radio_transmit_callback, 0);
    }
    else
    {
        /* Do not use listen before talk (LBT) */
        cmdTx.startTime = timestamp;
        cmdTx.startTrigger.triggerType = (use_start_time)? TRIG_ABSTIME : TRIG_NOW;
        cmdTx.startTrigger.pastTrig = 1;
        radio_driver_tx_cmd_handle = ti_lib_driver_rf_post_cmd(radio_driver_rf_handle, (RF_Op*) &cmdTx, RF_PriorityNormal, &radio_transmit_callback, 0);
    }
    if (radio_driver_tx_cmd_handle == RF_ALLOC_ERROR)
    {
        /* TX failed */
        radio_driver_tx_cmd_handle = 0;
        return false;
    }
    return true;
}

bool radio_sniff_packet_internal(uint32_t start_time, uint32_t timeout, bool use_start_time)
{
    if (!radio_driver_rf_handle)
    {
        /* Driver is not initialized */
        return false;
    }
    /* Disable all active RX or TX commands */
    RADIO_STOP_ACTIVITY();

    /* Configure RX part of RX_SNIFF command */
    RF_cmdPropRxAdvSniff.status = 0;
    RF_cmdPropRxAdvSniff.pQueue    = &radio_driver_rf_data_queue;
    RF_cmdPropRxAdvSniff.pOutput   = (uint8_t*)&radio_driver_rx_statistics;
    RF_cmdPropRxAdvSniff.maxPktLen = RADIO_MAX_PACKET_LENGTH + 1;
    RF_cmdPropRxAdvSniff.rssiThr = (int8_t)RADIO_SNIFF_RSSI;
    if (use_start_time)
    {
        RF_cmdPropRxAdvSniff.startTrigger.triggerType = TRIG_ABSTIME;
        RF_cmdPropRxAdvSniff.startTime = start_time;
        RF_cmdPropRxAdvSniff.startTrigger.pastTrig = 1;
    }
    else
    {
        RF_cmdPropRxAdvSniff.startTrigger.triggerType = TRIG_NOW;
        RF_cmdPropRxAdvSniff.startTime = 0;
        RF_cmdPropRxAdvSniff.startTrigger.pastTrig = 0;
    }

    uint32_t symbolLengthUs  = 1000000UL / radio_driver_rf_baudrate;

    /* Represents the time in which we need to receive corrConfig.numCorr* correlation peaks to detect preamble.
     * When continously checking the preamble quality, this period has to be wide enough to also contain the sync
     * word, with a margin. If it is not, then there is a chance the SNIFF command will abort while receiving the
     * sync word, as it no longer detects a preamble. */
    uint32_t correlationPeriodUs = (RF_cmdPropRadioDivSetup.formatConf.nSwBits + CORR_PERIOD_SYM_MARGIN)*symbolLengthUs;
    /* Represents the time where we will force a check if preamble is present (only done once).
     * The main idea is that his should be shorter than "correlationPeriodUs" so that if we get RSSI valid, but
     * there is not a valid preamble on the air, we will leave RX as quickly as possible. */
    uint32_t csEndTimeUs = (CS_END_TIME_MIN_TIME_SYM*symbolLengthUs + CS_END_TIME_MIN_TIME_STATIC_US);

    /* Represents the maximum time from the startTrigger to when we expect a sync word to be received. */
    uint32_t rxEndTimeUs = ((RF_cmdPropRadioDivSetup.preamConf.nPreamBytes << 3) + RF_cmdPropRadioDivSetup.formatConf.nSwBits + RX_END_TIME_SYM_MARGIN)*symbolLengthUs;

    /* Set sniff mode timing configuration in sniff command in RAT ticks */
    RF_cmdPropRxAdvSniff.corrPeriod = (uint16_t)ti_lib_driver_rf_convert_us_to_rat_ticks(correlationPeriodUs);
    RF_cmdPropRxAdvSniff.csEndTime  = (uint32_t)ti_lib_driver_rf_convert_us_to_rat_ticks(csEndTimeUs);
    RF_cmdPropRxAdvSniff.endTime    = (uint32_t)ti_lib_driver_rf_convert_us_to_rat_ticks(rxEndTimeUs);

    radio_driver_data_received_flag = false;
#ifdef RADIO_ACTIVITY_LED
    /* If radio led is defined, bind it to the radio module */
    leds_single_set_mux(RADIO_ACTIVITY_LED, PINCC26XX_MUX_RFC_GPO2);
#endif
    /* Start sniffing */
    radio_driver_rx_cmd_handle = ti_lib_driver_rf_post_cmd(radio_driver_rf_handle, (RF_Op*) &RF_cmdPropRxAdvSniff, RF_PriorityNormal,
                                         &radio_receive_callback, RF_EventCmdDone | RF_EventRxOk | RF_EventRxEntryDone);

    if (radio_driver_rx_cmd_handle == RF_ALLOC_ERROR)
    {
        /* Sniff failed */
        radio_driver_rx_cmd_handle = 0;
        return false;
    }
    return true;
}

bool radio_test_RSSI_internal(uint32_t start_time, bool use_start_time)
{
    int8_t rssi;
    uint32_t end_time = 2000;

    if (!radio_driver_rf_handle)
    {
        /* Driver is not initialized */
        return false;
    }
    /* Disable all active RX or TX commands */
    RADIO_STOP_ACTIVITY();

    radio_driver_test_rssi = (int8_t)RF_GET_RSSI_ERROR_VAL;
    RF_cmdRxTest.status = IDLE;
    RF_cmdRxTest.endTrigger.triggerType = TRIG_REL_START;
    RF_cmdRxTest.endTime = ti_lib_driver_rf_convert_us_to_rat_ticks(end_time);

    RF_cmdRxTest.startTrigger.triggerType = (use_start_time)? TRIG_ABSTIME : TRIG_NOW;
    RF_cmdRxTest.startTime = (use_start_time)? start_time : 0;
    RF_cmdRxTest.startTrigger.pastTrig = (use_start_time)? 1 : 0;

    radio_driver_rx_cmd_handle = ti_lib_driver_rf_post_cmd(radio_driver_rf_handle, (RF_Op*) &RF_cmdRxTest, RF_PriorityNormal, &radio_test_RSSI_callback, RF_EventCmdDone);

    if (radio_driver_rx_cmd_handle == RF_ALLOC_ERROR)
    {
        /* Sniff failed */
        radio_driver_rx_cmd_handle = 0;
        return false;
    }
    if (use_start_time)
    {
        end_time = ti_lib_driver_rf_convert_us_to_rat_ticks(end_time) + start_time;
    }
    else
    {
        end_time = ti_lib_driver_rf_convert_us_to_rat_ticks(end_time) + radio_get_time();
    }

    while ((radio_driver_test_rssi == RF_GET_RSSI_ERROR_VAL) && (RF_cmdRxTest.status < SKIPPED))
    {
        WDT_RESET();
        rssi = ti_lib_driver_rf_get_rssi(radio_driver_rf_handle);
        if ((rssi > radio_driver_test_rssi) && (rssi < 0))
        {
            radio_driver_test_rssi = rssi;
        }
    }
    return (RF_cmdRxTest.status == ACTIVE)? true : false;
}
