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
 * @ingroup     sensors
 * @{
 *
 * @file
 * @brief       Push button management implementation
 *
 * @author      Archil Pirmisashvili <kulipator@gmail.com>
 * @}
 */

 #include "push_button.h"

/**
 * @brief   Pin driver state
 */
static PIN_State pushButtonPinState;
/**
 * @brief   Pin driver handle
 */
static PIN_Handle pushButtonPinHandle;

#if defined(Board_PIN_REED_SW) && (Board_PIN_REED_SW != PIN_UNASSIGNED)
/**
 * @brief   Reed switch press timestamp
 */
static uint32_t reed_start_ts;
/**
 * @brief   Reed switch change callback function
 */
static PushButton_ReedSwitchChangeCallback change_callback;
/**
 * @brief   Reed switch release callback function
 */
static PushButton_ReedSwitchReleasedCallback released_callback;
#endif

/**
 * @brief   GPIO change callback function
 *
 * @param[in]   handle  GPIO driver handle
 * @param[in]   pinId   GPIO pin ID
 *
 * This function is an GPIO interrupt callback
 */
static void push_button_gpio_changed_cb(PIN_Handle handle, PIN_Id pinId);

bool push_button_init(void)
{
    static PIN_Config pushButtonPinTable[] = {
#if defined(Board_PIN_REED_SW) && (Board_PIN_REED_SW != PIN_UNASSIGNED)
        Board_PIN_REED_SW | PIN_INPUT_EN | Board_PIN_REED_SW_PULL | PIN_IRQ_BOTHEDGES
                      | PIN_HYSTERESIS | PIN_INV_INOUT,
#endif
        PIN_TERMINATE
    };

    pushButtonPinHandle = ti_lib_driver_pin_open(&pushButtonPinState, pushButtonPinTable);

    if (!pushButtonPinHandle)
    {
        return false;
    }

    /* Setup callback for button pins */
    if (ti_lib_driver_pin_register_int_cb(pushButtonPinHandle, &push_button_gpio_changed_cb) != PIN_SUCCESS)
    {
        return false;
    }

    return true;
}

void push_button_gpio_changed_cb(PIN_Handle handle, PIN_Id pinId)
{
#if defined(Board_PIN_REED_SW) && (Board_PIN_REED_SW != PIN_UNASSIGNED)
    uint32_t reed_state, ts_change;

    if (pinId == Board_PIN_REED_SW)
    {
        reed_state = ti_lib_driver_pin_get_input_value(Board_PIN_REED_SW);
        /* Save activation time */
        if (reed_state)
        {
            /* Pressed */
            reed_start_ts = get_up_time();
        }
        else
        {
            /* Released */
            ts_change = get_up_time() - reed_start_ts;
        }

        /* Fire callbacks if needed */
        if (change_callback)
        {
            change_callback(reed_state);
        }
        if (!reed_state)
        {
            if (released_callback && reed_start_ts)
            {
                released_callback(ts_change);
            }
            reed_start_ts = 0;
        }
    }
#endif
}

#if defined(Board_PIN_REED_SW) && (Board_PIN_REED_SW != PIN_UNASSIGNED)

void push_button_register_reed_switch_change_callback(PushButton_ReedSwitchChangeCallback callback)
{
    change_callback = callback;
}

void push_button_register_reed_switch_released_callback(PushButton_ReedSwitchReleasedCallback callback)
{
    released_callback = callback;
}

#endif
