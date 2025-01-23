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
 * @ingroup     boards_common
 * @{
 *
 * @file
 * @brief       Common board definition
 *
 * @author      Archil Pirmisashvili <kulipator@gmail.com>
 */

#ifndef BOARDS_BOARD_COMMON_H
#define BOARDS_BOARD_COMMON_H

#include <unistd.h>
#include <string.h>

#include <ti-lib.h>
#include "common_utilities.h"
#include "drivers/driver_watchdog.h"

#define BOARD_CC1310_LAUNCHPAD  1
#define BOARD_CC1350_LAUNCHPAD  2

#if BOARD == BOARD_CC1310_LAUNCHPAD
#include "cc1310_launchpad/board.h"
#elif BOARD == BOARD_CC1350_LAUNCHPAD
#include "cc1350_launchpad/board.h"
#else
#error No Board defined
#endif

#ifdef CONFIG_ANALOG_A_PIN
#if CONFIG_ANALOG_A_PIN == IOID_7
#define CONFIG_DIO7_ANALOG_PIN
#define CONFIG_ANALOG_A_ADC 0
#elif CONFIG_ANALOG_A_PIN == IOID_8
#define CONFIG_DIO8_ANALOG_PIN
#define CONFIG_ANALOG_A_ADC 1
#elif CONFIG_ANALOG_A_PIN == IOID_9
#define CONFIG_DIO9_ANALOG_PIN
#define CONFIG_ANALOG_A_ADC 2
#elif CONFIG_ANALOG_A_PIN == IOID_10
#define CONFIG_DIO10_ANALOG_PIN
#define CONFIG_ANALOG_A_ADC 3
#elif CONFIG_ANALOG_A_PIN == IOID_11
#define CONFIG_DIO11_ANALOG_PIN
#define CONFIG_ANALOG_A_ADC 4
#elif CONFIG_ANALOG_A_PIN == IOID_12
#define CONFIG_DIO12_ANALOG_PIN
#define CONFIG_ANALOG_A_ADC 5
#elif CONFIG_ANALOG_A_PIN == IOID_13
#define CONFIG_DIO13_ANALOG_PIN
#define CONFIG_ANALOG_A_ADC 6
#elif CONFIG_ANALOG_A_PIN == IOID_14
#define CONFIG_DIO14_ANALOG_PIN
#define CONFIG_ANALOG_A_ADC 7
#endif
#endif

#ifdef CONFIG_ANALOG_B_PIN
#if CONFIG_ANALOG_B_PIN == IOID_7
#define CONFIG_DIO7_ANALOG_PIN
#define CONFIG_ANALOG_B_ADC 0
#elif CONFIG_ANALOG_B_PIN == IOID_8
#define CONFIG_DIO8_ANALOG_PIN
#define CONFIG_ANALOG_B_ADC 1
#elif CONFIG_ANALOG_B_PIN == IOID_9
#define CONFIG_DIO9_ANALOG_PIN
#define CONFIG_ANALOG_B_ADC 2
#elif CONFIG_ANALOG_B_PIN == IOID_10
#define CONFIG_DIO10_ANALOG_PIN
#define CONFIG_ANALOG_B_ADC 3
#elif CONFIG_ANALOG_B_PIN == IOID_11
#define CONFIG_DIO11_ANALOG_PIN
#define CONFIG_ANALOG_B_ADC 4
#elif CONFIG_ANALOG_B_PIN == IOID_12
#define CONFIG_DIO12_ANALOG_PIN
#define CONFIG_ANALOG_B_ADC 5
#elif CONFIG_ANALOG_B_PIN == IOID_13
#define CONFIG_DIO13_ANALOG_PIN
#define CONFIG_ANALOG_B_ADC 6
#elif CONFIG_ANALOG_B_PIN == IOID_14
#define CONFIG_DIO14_ANALOG_PIN
#define CONFIG_ANALOG_B_ADC 7
#endif
#endif

#ifdef CONFIG_DIO7_ANALOG_PIN
#define BOARD_DIO7_ANALOG IOID_7
#else
#define BOARD_DIO7_ANALOG PIN_UNASSIGNED
#endif

#ifdef CONFIG_DIO8_ANALOG_PIN
#define BOARD_DIO8_ANALOG IOID_8
#else
#define BOARD_DIO8_ANALOG PIN_UNASSIGNED
#endif

#ifdef CONFIG_DIO9_ANALOG_PIN
#define BOARD_DIO9_ANALOG IOID_9
#else
#define BOARD_DIO9_ANALOG PIN_UNASSIGNED
#endif

#ifdef CONFIG_DIO10_ANALOG_PIN
#define BOARD_DIO10_ANALOG IOID_10
#else
#define BOARD_DIO10_ANALOG PIN_UNASSIGNED
#endif

#ifdef CONFIG_DIO11_ANALOG_PIN
#define BOARD_DIO11_ANALOG IOID_11
#else
#define BOARD_DIO11_ANALOG PIN_UNASSIGNED
#endif

#ifdef CONFIG_DIO12_ANALOG_PIN
#define BOARD_DIO12_ANALOG IOID_12
#else
#define BOARD_DIO12_ANALOG PIN_UNASSIGNED
#endif

#ifdef CONFIG_DIO13_ANALOG_PIN
#define BOARD_DIO13_ANALOG IOID_13
#else
#define BOARD_DIO13_ANALOG PIN_UNASSIGNED
#endif

#ifdef CONFIG_DIO14_ANALOG_PIN
#define BOARD_DIO14_ANALOG IOID_14
#else
#define BOARD_DIO14_ANALOG PIN_UNASSIGNED
#endif

#define SLEEP_SECONDS_WITH_WDT_ON(x) { rfn_posix_sleep(x); }
#define SLEEP_USECONDS_WITH_WDT_ON(x) { rfn_posix_usleep(x); }
#define SLEEP_MSECONDS_WITH_WDT_ON(x) SLEEP_USECONDS(x * 1000)
#define SLEEP_SECONDS(x) { watchdog_enable(false); rfn_posix_sleep(x); WDT_RESET(); watchdog_enable(true); }
#define SLEEP_USECONDS(x) { watchdog_enable(false); rfn_posix_usleep(x); WDT_RESET(); watchdog_enable(true); }
#define SLEEP_MSECONDS(x) SLEEP_USECONDS(x * 1000)
#define SLEEP_BREAK() { rfn_posix_sleep_break(); }

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @def     UARTName
 *  @brief   Enum of UARTs
 */
typedef enum  UARTName {
     UART0 = 0,
     UARTCOUNT
}  UARTName;

/**
 *  @def    ADCBufName
 *  @brief  Enum of ADCBufs
 */
typedef enum ADCBufName {
    ADCBUF0 = 0,

    ADCBUFCOUNT
} ADCBufName;

/**
 *  @def    ADCBuf0ChannelName
 *  @brief  Enum of ADCBuf channels
 */
typedef enum ADCBuf0ChannelName {
    ADCBUF0CHANNEL0 = 0,
    ADCBUF0CHANNEL1,
    ADCBUF0CHANNEL2,
    ADCBUF0CHANNEL3,
    ADCBUF0CHANNEL4,
    ADCBUF0CHANNEL5,
    ADCBUF0CHANNEL6,
    ADCBUF0CHANNEL7,
    ADCBUF0CHANNELVDDS,
    ADCBUF0CHANNELDCOUPL,
    ADCBUF0CHANNELVSS,

    ADCBUF0CHANNELCOUNT
} ADCBuf0ChannelName;

/**
 *  @def    ADCName
 *  @brief  Enum of ADCs
 */
typedef enum ADCName {
    ADC0 = 0,
    ADC1,
    ADC2,
    ADC3,
    ADC4,
    ADC5,
    ADC6,
    ADC7,
    ADCDCOUPL,
    ADCVSS,
    ADCVDDS,

    ADCCOUNT
} ADCName;

extern ADCCC26XX_Object adcCC26xxObjects[ADCCOUNT];
extern const ADCCC26XX_HWAttrs adcCC26xxHWAttrs[ADCCOUNT];

/**
 *  @def    GPTimerName
 *  @brief  Enum of GPTimer parts
 */
typedef enum GPTimerName {
    GPTIMER0A = 0,
    GPTIMER0B,
    GPTIMER1A,
    GPTIMER1B,
    GPTIMER2A,
    GPTIMER2B,
    GPTIMER3A,
    GPTIMER3B,

    GPTIMERPARTSCOUNT
} GPTimerName;

/**
 *  @def    GPTimers
 *  @brief  Enum of GPTimers
 */
typedef enum GPTimers {
    GPTIMER0 = 0,
    GPTIMER1,
    GPTIMER2,
    GPTIMER3,

    GPTIMERCOUNT
} GPTimers;

/**
 *  @def    I2CName
 *  @brief  Enum of I2C names
 */
typedef enum  I2CName {
     I2C0 = 0,
     I2CCOUNT
}  I2CName;

/**
 * @def     UDMAName
 * @brief   Enum of DMA buffers
 */
typedef enum UDMAName {
     UDMA0 = 0,

     UDMACOUNT
} UDMAName;

/*!
 *  @def    WatchdogName
 *  @brief  Enum of Watchdogs
 */
typedef enum WatchdogName {
    WATCHDOG0 = 0,

    WATCHDOGCOUNT
} WatchdogName;

/**
 * @brief       Application entry point
 *
 * This function is the application entry point
 */
extern NORETURN_ATTR void *mainThread(void *arg0);

/**
 * @brief       Initialize board
 *
 * This function initializes board and common modules
 */
extern void board_initialize(void);

#ifdef __cplusplus
}
#endif

#endif /* BOARDS_BOARD_COMMON_H */
/** @} */
