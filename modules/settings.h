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
 * @brief       Application Settings definition
 *
 * @author      Archil Pirmisashvili <kulipator@gmail.com>
 */

#ifndef MODULES_SETTINGS_H
#define MODULES_SETTINGS_H

#include "boards/board_common.h"
#include "drivers/driver_flash.h"

/**
 * @brief   Location of the settings structure
 *
 * This constant defines flash page number, where settings
 * structure is located
 */
#ifndef SETTINGS_PAGE
#define SETTINGS_PAGE                           (30)
#endif

/**
 * @brief   Default sensor readout interval in seconds
 */
#ifndef DEFAULT_SENSOR_READOUT_INTERVAL_SEC
#define DEFAULT_SENSOR_READOUT_INTERVAL_SEC     (60)
#endif

/**
 * @brief   Anchor bytes of settings structure
 */
#define SETTINGS_ANCHOR 0xAABBCCDDULL

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Unit configuration settings
 *
 * This structure includes unit configuration settings.
 */
typedef struct unit_settings_s unit_settings_t, *unit_settings_p;

struct ATTRIBUTE_PACKED_ALIGNED(1) unit_settings_s
{
    /* Anchor */
    uint64_t anchor;
    /* Sensor readout interval */
    uint32_t sensor_readout_interval_sec;
    /* Settings checksum needed for settings validation */
    uint16_t checksum;
};

/**
 * @brief   Unit configuration settings object
 */
extern unit_settings_t settings;

/**
 * @brief   Load settings
 *
 * This function loads settings from flash and validates checksum
 *
 * @returns     Checksum is correct or not
 */
bool settings_load(void);

/**
 * @brief   Save settings
 *
 * This function stores settings structure to flash
 *
 * @returns     If save was successful
 */
bool settings_save(void);

/**
 * @brief   Set default settings
 *
 * This function assigns default settings to the settings object
 *
 * @warning The function does not stores settings to flash
 */
void settings_set_default(void);

#ifdef __cplusplus
}
#endif

#endif /* MODULES_SETTINGS_H */
/** @} */
