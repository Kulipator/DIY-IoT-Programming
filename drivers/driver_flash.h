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
 * @brief       Flash memory driver definitions
 *
 * @author      Archil Pirmisashvili <archil@rf-networks.com>
 */

#ifndef DRIVERS_DRIVER_FLASH_H
#define DRIVERS_DRIVER_FLASH_H

#include "boards/common_utilities.h"

#ifndef APP_START_ADDRESS
#define APP_START_ADDRESS 0x0C000UL
#endif
#define FLASH_PAGE_SIZE     0x1000
#define CCFG_PAGE           0x1F
#define CCFG_LENGHT         0x58
#define CCFG_START_OFFSET   FLASH_PAGE_SIZE - CCFG_LENGHT
#define CCFG_START_ADDR     (CCFG_PAGE * FLASH_PAGE_SIZE) + CCFG_START_OFFSET

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Load data from flash memory
 *
 *  @param[in]   address    Address in flash memory to read from
 *  @param[in]   data       Pointer to destination
 *  @param[in]   length     Bytes to read
 *
 *  This function loads \e length bytes from the flash memory
 *  at \e address
 *
 *  @warning Do not call this function from the interrupt
 *
 */
void flash_load(uint_least32_t address, void *data, size_t length);

/**
 * @brief   Erase flash page
 *
 * @param[in]   page_num    Page to erase (number)
 *
 * This function erases flash memory page
 *
 * @returns     If operation is successful
 */
bool flash_erase_page(uint8_t page_num);

/**
 * @brief   Write data to flash memory
 *
 * @param[in]   addr       Address in flash memory to write to
 * @param[in]   data       Pointer to source
 * @param[in]   length     Bytes to write
 *
 * This function writes \e length bytes to flash memory
 * at \e address.
 *
 * @returns     If operation is successful
 */
bool flash_write(uint_least32_t addr, void *data, size_t length);

/**
 * @brief   Read CCFG from flash memory
 *
 * @param[in]   data       Pointer to CCFG structure
 *
 *  This function loads CCFG from the flash memory
 *  to \e data
 *
 *  @warning Do not call this function from the interrupt
 *
 */
void flash_load_ccfg(void *data);

/**
 * @brief   Write CCFG to flash memory
 *
 * @param[in]   data       Pointer to CCFG structure
 *
 * This function writes \e data to flash memory CCFG section.
 *
 * @returns     If operation is successful
 */
bool flash_save_ccfg(void *data);

#ifdef __cplusplus
}
#endif

#endif /* DRIVERS_DRIVER_FLASH_H */
/** @} */
