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
 * @ingroup     boards_cc1310_launchpad
 * @{
 *
 * @file
 * @brief       Board specific definitions for TI CC1310 Launchpad
 *
 * @author      Archil Pirmisashvili <kulipator@gmail.com>
 */

#ifndef BOARDS_CC1310_LAUNCHPAD_BOARD_H
#define BOARDS_CC1310_LAUNCHPAD_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   LED HAL configuration
 *
 * Those values are not meant to be modified by the user
 * @{
 */
#define LEDS_CONF_COUNT                 2
#define LEDS_CONF_RED                   0
#define LEDS_CONF_GREEN                 1
/** @} */

/*---------------------------------------------------------------------------*/
/**
 * @brief   LED IOID mappings
 *
 * Those values are not meant to be modified by the user
 * @{
 */
#define Board_PIN_LED0                  IOID_6
#define Board_PIN_LED1                  IOID_7
/** @} */

/*---------------------------------------------------------------------------*/
/**
 * @brief   Button IOID mappings
 *
 * Those values are not meant to be modified by the user
 * @{
 */
#define Board_PIN_REED_SW               IOID_14
#define Board_PIN_REED_SW_PULL          PIN_PULLUP
/** @} */

/*---------------------------------------------------------------------------*/
/**
 * @brief   I2C IOID mappings
 *
 * Those values are not meant to be modified by the user
 * @{
 */
#define  I2C0_SCL0                      IOID_13
#define  I2C0_SDA0                      IOID_12
/** @} */

/*---------------------------------------------------------------------------*/
/**
 * @brief   UART IOID mappings
 *
 * Those values are not meant to be modified by the user
 * @{
 */
#ifdef UART_TX_CONF
#define  UART_TX                        UART_TX_CONF             /* TXD */
#else
#define UART_TX                         IOID_3                   /* TXD */
#endif
#ifdef UART_RX_CONF
#define  UART_RX                        UART_RX_CONF             /* RXD */
#else
#define UART_RX                         IOID_2                   /* RXD */
#endif
#ifdef UART_CTS_CONF
#define UART_CTS                        UART_CTS_CONF            /* CTS */
#else
#define UART_CTS                        PIN_UNASSIGNED           /* CTS */
#endif
#ifdef UART_RTS_CONF
#define UART_RTS                        UART_RTS_CONF            /* RTS */
#else
#define UART_RTS                        PIN_UNASSIGNED           /* RTS */
#endif
/** @} */

/*---------------------------------------------------------------------------*/
/**
 * @brief   Device string used on startup
 *
 * @{
 */
#define BOARD_STRING                    "CC1310 Launchpad"
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* BOARDS_CC1310_LAUNCHPAD_BOARD_H */
/** @} */
