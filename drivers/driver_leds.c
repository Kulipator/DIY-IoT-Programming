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
 * @brief       Leds driver implementations
 *
 * @author      Archil Pirmisashvili <archil@rf-networks.com>
 * @}
 */

#include <ti-lib.h>
#include "driver_leds.h"

#ifndef Board_PIN_LED0_INVERSE
#define Board_PIN_LED0_INVERSE 0
#endif

#ifndef Board_PIN_LED1_INVERSE
#define Board_PIN_LED1_INVERSE 0
#endif

#ifndef Board_PIN_LED2_INVERSE
#define Board_PIN_LED2_INVERSE 0
#endif

#ifndef Board_PIN_LED3_INVERSE
#define Board_PIN_LED3_INVERSE 0
#endif

#ifndef Board_PIN_LED4_INVERSE
#define Board_PIN_LED4_INVERSE 0
#endif

#if LEDS_COUNT > 0
static PIN_Handle driver_leds_pin_handle;
static PIN_State driver_leds_pin_state;
static leds_num_t driver_leds_leds[] = {
#if defined(Board_PIN_LED0) && Board_PIN_LED0 != IOID_UNUSED
    Board_PIN_LED0,
#endif
#if defined(Board_PIN_LED1) && Board_PIN_LED1 != IOID_UNUSED
    Board_PIN_LED1,
#endif
#if defined(Board_PIN_LED2) && Board_PIN_LED2 != IOID_UNUSED
    Board_PIN_LED2,
#endif
#if defined(Board_PIN_LED3) && Board_PIN_LED3 != IOID_UNUSED
    Board_PIN_LED3,
#endif
#if defined(Board_PIN_LED4) && Board_PIN_LED4 != IOID_UNUSED
    Board_PIN_LED4,
#endif
    0
};
static PIN_Config driver_leds_pin_table[] = {
#if defined(Board_PIN_LED0) && Board_PIN_LED0 != IOID_UNUSED
    Board_PIN_LED0 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX | Board_PIN_LED0_INVERSE,
#endif

#if defined(Board_PIN_LED1) && Board_PIN_LED1 != IOID_UNUSED
    Board_PIN_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX | Board_PIN_LED1_INVERSE,
#endif

#if defined(Board_PIN_LED2) && Board_PIN_LED2 != IOID_UNUSED
    Board_PIN_LED2 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX | Board_PIN_LED2_INVERSE,
#endif

#if defined(Board_PIN_LED3) && Board_PIN_LED3 != IOID_UNUSED
    Board_PIN_LED3 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX | Board_PIN_LED3_INVERSE,
#endif

#if defined(Board_PIN_LED4) && Board_PIN_LED4 != IOID_UNUSED
    Board_PIN_LED4 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX | Board_PIN_LED4_INVERSE,
#endif
    PIN_TERMINATE
};
#endif /* LEDS_COUNT > 0 */

void leds_init(void)
{
#if LEDS_COUNT > 0
    driver_leds_pin_handle = ti_lib_driver_pin_open(&driver_leds_pin_state, driver_leds_pin_table);
    if (driver_leds_pin_handle == NULL) {
        /* Initialization failed */
        while(1);
    }
#endif
}

void leds_single_on(leds_num_t led)
{
#if LEDS_COUNT > 0
    if(led >= LEDS_COUNT) {
        return;
    }

    ti_lib_driver_pin_set_output_value(driver_leds_pin_handle, driver_leds_leds[led], 1);
#endif
}

void leds_single_off(leds_num_t led)
{
#if LEDS_COUNT > 0
    if(led >= LEDS_COUNT) {
        return;
    }
    ti_lib_driver_pin_set_output_value(driver_leds_pin_handle, driver_leds_leds[led], 0);
#endif
}

void leds_single_toggle(leds_num_t led)
{
#if LEDS_COUNT > 0
    if(led >= LEDS_COUNT) {
        return;
    }
    ti_lib_driver_pin_set_output_value(driver_leds_pin_handle, driver_leds_leds[led], ! ti_lib_driver_pin_get_output_value(driver_leds_leds[led]));
#endif
}

void leds_single_set_mux(leds_num_t led, int32_t mux)
{
#if LEDS_COUNT > 0
    if(led >= LEDS_COUNT) {
        return;
    }
    ti_lib_driver_pin_set_mux(driver_leds_pin_handle, driver_leds_leds[led], mux);
#endif
}

void leds_blink_all(uint8_t times)
{
#if LEDS_COUNT > 0
    uint8_t i, j;
    /* Disable pin functions if applied */
    for (j = 0; j < LEDS_COUNT; j++)
    {
        leds_single_set_mux(driver_leds_leds[j], PINCC26XX_MUX_GPIO);
    }

    for (i = 0; i < times; i++)
    {
        /* Turn on */
        for (j = 0; j < LEDS_COUNT; j++)
        {
            leds_single_on(driver_leds_leds[j]);
        }
        /* Wait */
        SLEEP_MSECONDS(50);
        /* Turn off */
        for (j = 0; j < LEDS_COUNT; j++)
        {
            leds_single_off(driver_leds_leds[j]);
        }
        /* Wait */
        SLEEP_MSECONDS(450);
    }
#endif
}
