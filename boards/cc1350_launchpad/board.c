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
 * @ingroup     boards_cc1350_launchpad
 * @{
 * @file
 * @brief       Board specific implementations for TI CC1350 Launchpad
 *
 * @author      Archil Pirmisashvili <kulipator@gmail.com>
 * @}
 */

#include "boards/board_common.h"

#if BOARD == BOARD_CC1350_LAUNCHPAD

#include "board.h"

/*
 *  =============================== PIN ===============================
 */
const PIN_Config BoardGpioInitTable[] = {
     PIN_TERMINATE
};

#endif /* BOARD == BOARD_CC1350_LAUNCHPAD */

