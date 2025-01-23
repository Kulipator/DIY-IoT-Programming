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
 * @brief       Flash memory driver implementations
 *
 * @author      Archil Pirmisashvili <kulipator@gmail.com>
 * @}
 */

#include "driver_flash.h"
#include <ti-lib.h>

static uint8_t flash_ccfg_page_arr[FLASH_PAGE_SIZE];

void flash_load(uint_least32_t address, void *data, size_t length)
{
    uint8_t *pointer = (uint8_t *)address;
    memcpy(data, pointer, length);
}

bool flash_erase_page(uint8_t page_num)
{
    uint32_t ret;
    uint8_t mode;
    DEFINE_CRITICAL();

    mode = ti_lib_vims_mode_get(VIMS_BASE);

    if (mode != VIMS_MODE_DISABLED)
    {
        /* Disable flash cache */
        ti_lib_vims_mode_set(VIMS_BASE, VIMS_MODE_DISABLED);
        while (ti_lib_vims_mode_get(VIMS_BASE) != VIMS_MODE_DISABLED);
    }
    ENTER_CRITICAL();
    ret = ti_lib_flash_sector_erase(page_num * FLASH_PAGE_SIZE);
    EXIT_CRITICAL();

    if (mode != VIMS_MODE_DISABLED)
    {
        /* Re-enable flash cache */
        ti_lib_vims_mode_set(VIMS_BASE, VIMS_MODE_ENABLED);
    }
    return ret == FAPI_STATUS_SUCCESS;
}

bool flash_write(uint_least32_t addr, void *data, size_t length)
{
    uint32_t ret;
    uint8_t mode;
    DEFINE_CRITICAL();

    mode = ti_lib_vims_mode_get(VIMS_BASE);

    if (mode != VIMS_MODE_DISABLED)
    {
        /* Disable flash cache */
        ti_lib_vims_mode_set(VIMS_BASE, VIMS_MODE_DISABLED);
        while (ti_lib_vims_mode_get(VIMS_BASE) != VIMS_MODE_DISABLED);
    }

    /* Write */
    ENTER_CRITICAL();
    ret = ti_lib_flash_program((uint8_t*)data, (uint32_t)addr, (uint32_t)length);
    EXIT_CRITICAL();

    if (mode != VIMS_MODE_DISABLED)
    {
        /* Re-enable flash cache */
        ti_lib_vims_mode_set(VIMS_BASE, VIMS_MODE_ENABLED);
    }

    return (ret == FAPI_STATUS_SUCCESS);
}

void flash_load_ccfg(void *data)
{
    flash_load(CCFG_START_ADDR, data, CCFG_LENGHT);
}

bool flash_save_ccfg(void *data)
{
    /* Get current CCFG page */
    flash_load(CCFG_PAGE * FLASH_PAGE_SIZE, flash_ccfg_page_arr, FLASH_PAGE_SIZE);
    /* Apply changed */
    memcpy(flash_ccfg_page_arr + CCFG_START_OFFSET, data, CCFG_LENGHT);

    /* Write CCFG page */
    return flash_erase_page(CCFG_PAGE) && flash_write(CCFG_PAGE * FLASH_PAGE_SIZE, flash_ccfg_page_arr, FLASH_PAGE_SIZE);
}
