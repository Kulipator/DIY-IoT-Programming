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
 * @ingroup     app_settings
 * @{
 *
 * @file
 * @brief       Application Settings implementation
 *
 * @author      Archil Pirmisashvili <archil@rf-networks.com>
 * @}
 */

#include "modules/settings.h"
#include "boards/common_utilities.h"

NOINIT_ATTR unit_settings_t settings;

static uint_least32_t settings_find_pointer(void);

bool settings_load(void)
{
    uint_least32_t address;
    bool res = false;

    /* Get load address */
    address = settings_find_pointer();

    if (address < (FLASH_PAGE_SIZE - sizeof(unit_settings_t))) 
    {
        /* Load settings structure from flash */
        flash_load(address, &settings, sizeof(unit_settings_t));
    }
    else 
    {
        /* Returns false */
        return res;
    }

    /* Validate and return */
    res = (calculate_crc16_xmodem((uint8_t*)&settings, sizeof(unit_settings_t) - 2) == settings.checksum);
    return res;
}

bool settings_save(void)
{
    uint_least32_t address;
    unit_settings_t t_erase;

    memset(&t_erase, 0, sizeof(t_erase));

    /* Calculate store address */
    address = settings_find_pointer();

    WDT_RESET();

    if ((address + (sizeof(unit_settings_t) * 2)) >= ((SETTINGS_PAGE + 1) * FLASH_PAGE_SIZE))
    {
        /* Need to erase page */
        if (!flash_erase_page(SETTINGS_PAGE))
            return false;
        WDT_RESET();
        address = SETTINGS_PAGE * FLASH_PAGE_SIZE;
    }
    else
    {
        /* There is more space on page, remove old settings */
        flash_write(address, &t_erase, sizeof(t_erase));
        address += sizeof(t_erase);
    }

    /* Update checksum before saving */
    settings.checksum = calculate_crc16_xmodem((uint8_t*)&settings, sizeof(unit_settings_t) - 2);

    WDT_RESET();
    /* Save */
    return (flash_write(address, &settings, sizeof(settings)));
}

void settings_set_default(void)
{
    /* Set defaults */
    settings.anchor = 0xA5A5A5A5UL;
    settings.sensor_readout_interval_sec = DEFAULT_SENSOR_READOUT_INTERVAL_SEC;
    /* Update checksum */
    settings.checksum = calculate_crc16_xmodem((uint8_t*)&settings, sizeof(unit_settings_t) - 2);
}

uint_least32_t settings_find_pointer(void)
{
    size_t i;
    uint64_t t_anchor;
    uint_least32_t start_address = SETTINGS_PAGE * FLASH_PAGE_SIZE;
    for (i = 0; i < (FLASH_PAGE_SIZE - sizeof(unit_settings_t)); i++)
    {
        WDT_RESET();
        flash_load(start_address, &t_anchor, sizeof(t_anchor));
        if (t_anchor == SETTINGS_ANCHOR)
        {
            break;
        }
        start_address++;
    }
    return start_address;
}
