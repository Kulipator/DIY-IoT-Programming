/*
 * @ingroup     tests_flash_test
 * @{
 *
 * @file
 * @brief       This test checks flash driver functionality.
 *
 * @author      Archil Pirmisashvili <kulipator@gmail.com>
 * @}
 */

#ifdef TESTS_FLASH_TEST

#include <stdio.h>
#include "boards/board_common.h"
#include "drivers/driver_leds.h"
#include "drivers/driver_watchdog.h"
#include "drivers/driver_console.h"
#include "drivers/driver_flash.h"

#ifndef CONSOLE_BAUDRATE_CONF
#define CONSOLE_BAUDRATE    CONSOLE_BAUDRATE_19200
#else
#define CONSOLE_BAUDRATE    CONSOLE_BAUDRATE_CONF
#endif

#define TEST_FLASH_PAGE     (30)

/**
 * @brief   Output message array
 */
static char msg[255];
/**
 * @brief   Test data arrays
 */
static uint8_t test_data[10], test_data_orig[10];

/**
 * @brief   Test entry point
 *
 */
void *mainThread(void *arg0)
{
    uint8_t i;
    bool res = true;
    uint8_t step = 0;

    /* Test initialization */
    memset(msg, 0, sizeof(msg));
    for (i = 0; i < sizeof(test_data); i++)
    {
        /* Fill both arrays, test_data_orig array we will use to compare */
        test_data[i] = test_data_orig[i] = i + 1;
    }
    console_initialize(CONSOLE_BAUDRATE);

    /* Blink all leds to indicate application is alive */
    leds_blink_all(4);

    /* Main loop */
    while(1)
    {
        /* Reset watch-dog */
        WDT_RESET();

        /* Process console */
        console_process();

        switch (step)
        {
        case 0:
            /* Erase page */
            sprintf(msg, "Erase result: %s\r\n", flash_erase_page(TEST_FLASH_PAGE)? "OK" : "Failed");
            console_write((uint8_t *)msg, strlen(msg));
            step++;
            break;
        case 1:
            /* Write data */
            sprintf(msg, "Write result: %s\r\n", flash_write(TEST_FLASH_PAGE * FLASH_PAGE_SIZE, test_data_orig, sizeof(test_data_orig))? "OK" : "Failed");
            console_write((uint8_t *)msg, strlen(msg));
            step++;
            break;
        case 2:
            /* Read data & compare */
            flash_load(TEST_FLASH_PAGE * FLASH_PAGE_SIZE, test_data, sizeof(test_data));
            for (i = 0; i < sizeof(test_data); i++)
            {
                if (test_data[i] != test_data_orig[i])
                {
                    res = false;
                    break;
                }
            }
            sprintf(msg, "Read & compare result: %s\r\n", res? "OK" : "Failed");
            console_write((uint8_t *)msg, strlen(msg));
            step++;
            break;
        default:
            break;
        }
    }
}

#endif /* TESTS_FLASH_TEST */
