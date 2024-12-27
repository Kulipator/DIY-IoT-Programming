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
 * @brief       Common board implementation
 *
 * @author      Archil Pirmisashvili <archil@rf-networks.com>
 * @}
 */

 #include "board_common.h"

/*
 *  =============================== ADCBuf ===============================
 */
ADCBufCC26XX_Object adcBufCC26xxObjects[ADCBUFCOUNT];

/*
 *  This table converts a virtual adc channel into a dio and internal analogue
 *  input signal. This table is necessary for the functioning of the adcBuf
 *  driver. Comment out unused entries to save flash. Dio and internal signal
 *  pairs are hardwired. Do not remap them in the table. You may reorder entire
 *  entries. The mapping of dio and internal signals is package dependent.
 */
const ADCBufCC26XX_AdcChannelLutEntry ADCBufCC26XX_adcChannelLut[ADCBUF0CHANNELCOUNT] = {
    {BOARD_DIO7_ANALOG, ADC_COMPB_IN_AUXIO7},
    {BOARD_DIO8_ANALOG, ADC_COMPB_IN_AUXIO6},
    {BOARD_DIO9_ANALOG, ADC_COMPB_IN_AUXIO5},
    {BOARD_DIO10_ANALOG, ADC_COMPB_IN_AUXIO4},
    {BOARD_DIO11_ANALOG, ADC_COMPB_IN_AUXIO3},
    {BOARD_DIO12_ANALOG, ADC_COMPB_IN_AUXIO2},
    {BOARD_DIO13_ANALOG, ADC_COMPB_IN_AUXIO1},
    {BOARD_DIO14_ANALOG, ADC_COMPB_IN_AUXIO0},
    {PIN_UNASSIGNED, ADC_COMPB_IN_VDDS},
    {PIN_UNASSIGNED, ADC_COMPB_IN_DCOUPL},
    {PIN_UNASSIGNED, ADC_COMPB_IN_VSS},
};

const ADCBufCC26XX_HWAttrs adcBufCC26xxHWAttrs[ADCBUFCOUNT] = {
    {
        .intPriority       = ~0,
        .swiPriority       = 0,
        .adcChannelLut     = ADCBufCC26XX_adcChannelLut,
    }
};

const ADCBuf_Config ADCBuf_config[ADCBUFCOUNT] = {
    {
        &ADCBufCC26XX_fxnTable,
        &adcBufCC26xxObjects[ADCBUF0],
        &adcBufCC26xxHWAttrs[ADCBUF0]
    },
};

const uint_least8_t ADCBuf_count = ADCBUFCOUNT;

/*
 *  ================================== ADC ==================================
 */
ADCCC26XX_Object adcCC26xxObjects[ADCCOUNT];

const ADCCC26XX_HWAttrs adcCC26xxHWAttrs[ADCCOUNT] = {
   {
       .adcDIO              = BOARD_DIO7_ANALOG,
       .adcCompBInput       = ADC_COMPB_IN_AUXIO7,
       .refSource           = ADCCC26XX_FIXED_REFERENCE,
       .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
       .inputScalingEnabled = true,
       .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
       .returnAdjustedVal   = false
   },
   {
       .adcDIO              = BOARD_DIO8_ANALOG,
       .adcCompBInput       = ADC_COMPB_IN_AUXIO6,
       .refSource           = ADCCC26XX_FIXED_REFERENCE,
       .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
       .inputScalingEnabled = true,
       .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
       .returnAdjustedVal   = false
   },
   {
       .adcDIO              = BOARD_DIO9_ANALOG,
       .adcCompBInput       = ADC_COMPB_IN_AUXIO5,
       .refSource           = ADCCC26XX_FIXED_REFERENCE,
       .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
       .inputScalingEnabled = true,
       .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
       .returnAdjustedVal   = false
   },
   {
       .adcDIO              = BOARD_DIO10_ANALOG,
       .adcCompBInput       = ADC_COMPB_IN_AUXIO4,
       .refSource           = ADCCC26XX_FIXED_REFERENCE,
       .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
       .inputScalingEnabled = true,
       .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
       .returnAdjustedVal   = false
   },
   {
       .adcDIO              = BOARD_DIO11_ANALOG,
       .adcCompBInput       = ADC_COMPB_IN_AUXIO3,
       .refSource           = ADCCC26XX_FIXED_REFERENCE,
       .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
       .inputScalingEnabled = true,
       .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
       .returnAdjustedVal   = false
   },
   {
       .adcDIO              = BOARD_DIO12_ANALOG,
       .adcCompBInput       = ADC_COMPB_IN_AUXIO2,
       .refSource           = ADCCC26XX_FIXED_REFERENCE,
       .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
       .inputScalingEnabled = true,
       .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
       .returnAdjustedVal   = false
   },
   {
       .adcDIO              = BOARD_DIO13_ANALOG,
       .adcCompBInput       = ADC_COMPB_IN_AUXIO1,
       .refSource           = ADCCC26XX_FIXED_REFERENCE,
       .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
       .inputScalingEnabled = true,
       .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
       .returnAdjustedVal   = false
   },
   {
       .adcDIO              = BOARD_DIO14_ANALOG,
       .adcCompBInput       = ADC_COMPB_IN_AUXIO0,
       .refSource           = ADCCC26XX_FIXED_REFERENCE,
       .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_10P9_MS,
       .inputScalingEnabled = true,
       .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
       .returnAdjustedVal   = false
   },
   {
       .adcDIO              = PIN_UNASSIGNED,
       .adcCompBInput       = ADC_COMPB_IN_DCOUPL,
       .refSource           = ADCCC26XX_FIXED_REFERENCE,
       .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
       .inputScalingEnabled = true,
       .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
       .returnAdjustedVal   = false
   },
   {
       .adcDIO              = PIN_UNASSIGNED,
       .adcCompBInput       = ADC_COMPB_IN_VSS,
       .refSource           = ADCCC26XX_FIXED_REFERENCE,
       .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
       .inputScalingEnabled = true,
       .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
       .returnAdjustedVal   = false
   },
   {
       .adcDIO              = PIN_UNASSIGNED,
       .adcCompBInput       = ADC_COMPB_IN_VDDS,
       .refSource           = ADCCC26XX_FIXED_REFERENCE,
       .samplingDuration    = ADCCC26XX_SAMPLING_DURATION_2P7_US,
       .inputScalingEnabled = true,
       .triggerSource       = ADCCC26XX_TRIGGER_MANUAL,
       .returnAdjustedVal   = false
   }
};

const ADC_Config ADC_config[ADCCOUNT] = {
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[ADC0], &adcCC26xxHWAttrs[ADC0]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[ADC1], &adcCC26xxHWAttrs[ADC1]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[ADC2], &adcCC26xxHWAttrs[ADC2]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[ADC3], &adcCC26xxHWAttrs[ADC3]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[ADC4], &adcCC26xxHWAttrs[ADC4]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[ADC5], &adcCC26xxHWAttrs[ADC5]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[ADC6], &adcCC26xxHWAttrs[ADC6]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[ADC7], &adcCC26xxHWAttrs[ADC7]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[ADCDCOUPL], &adcCC26xxHWAttrs[ADCDCOUPL]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[ADCVSS], &adcCC26xxHWAttrs[ADCVSS]},
    {&ADCCC26XX_fxnTable, &adcCC26xxObjects[ADCVDDS], &adcCC26xxHWAttrs[ADCVDDS]},
};

const uint_least8_t ADC_count = ADCCOUNT;

/*
 *  =============================== GPTimer ===============================
 */
GPTimerCC26XX_Object gptimerCC26XXObjects[GPTIMERCOUNT];

const GPTimerCC26XX_HWAttrs gptimerCC26xxHWAttrs[GPTIMERPARTSCOUNT] = {
    { .baseAddr = GPT0_BASE, .intNum = INT_GPT0A, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT0, .pinMux = GPT_PIN_0A, },
    { .baseAddr = GPT0_BASE, .intNum = INT_GPT0B, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT0, .pinMux = GPT_PIN_0B, },
    { .baseAddr = GPT1_BASE, .intNum = INT_GPT1A, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT1, .pinMux = GPT_PIN_1A, },
    { .baseAddr = GPT1_BASE, .intNum = INT_GPT1B, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT1, .pinMux = GPT_PIN_1B, },
    { .baseAddr = GPT2_BASE, .intNum = INT_GPT2A, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT2, .pinMux = GPT_PIN_2A, },
    { .baseAddr = GPT2_BASE, .intNum = INT_GPT2B, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT2, .pinMux = GPT_PIN_2B, },
    { .baseAddr = GPT3_BASE, .intNum = INT_GPT3A, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT3, .pinMux = GPT_PIN_3A, },
    { .baseAddr = GPT3_BASE, .intNum = INT_GPT3B, .intPriority = (~0), .powerMngrId = PowerCC26XX_PERIPH_GPT3, .pinMux = GPT_PIN_3B, },
};

const GPTimerCC26XX_Config GPTimerCC26XX_config[GPTIMERPARTSCOUNT] = {
    { &gptimerCC26XXObjects[GPTIMER0], &gptimerCC26xxHWAttrs[GPTIMER0A], GPT_A },
    { &gptimerCC26XXObjects[GPTIMER0], &gptimerCC26xxHWAttrs[GPTIMER0B], GPT_B },
    { &gptimerCC26XXObjects[GPTIMER1], &gptimerCC26xxHWAttrs[GPTIMER1A], GPT_A },
    { &gptimerCC26XXObjects[GPTIMER1], &gptimerCC26xxHWAttrs[GPTIMER1B], GPT_B },
    { &gptimerCC26XXObjects[GPTIMER2], &gptimerCC26xxHWAttrs[GPTIMER2A], GPT_A },
    { &gptimerCC26XXObjects[GPTIMER2], &gptimerCC26xxHWAttrs[GPTIMER2B], GPT_B },
    { &gptimerCC26XXObjects[GPTIMER3], &gptimerCC26xxHWAttrs[GPTIMER3A], GPT_A },
    { &gptimerCC26XXObjects[GPTIMER3], &gptimerCC26xxHWAttrs[GPTIMER3B], GPT_B },
};

/*
 *  ================================== I2C ==================================
 */
I2CCC26XX_Object i2cCC26xxObjects[I2CCOUNT];
const I2CCC26XX_HWAttrsV1 i2cCC26xxHWAttrs[I2CCOUNT] = { { .baseAddr = I2C0_BASE, .powerMngrId =
PowerCC26XX_PERIPH_I2C0,
                                                           .intNum = INT_I2C_IRQ, .intPriority = ~0,
                                                           .swiPriority = 0, .sdaPin = I2C0_SDA0,
                                                           .sclPin =
                                                           I2C0_SCL0, } };

const I2C_Config I2C_config[I2CCOUNT] = { { .fxnTablePtr = &I2CCC26XX_fxnTable, .object =
                                                    &i2cCC26xxObjects[I2C0],
                                            .hwAttrs = &i2cCC26xxHWAttrs[I2C0] } };

const uint_least8_t I2C_count = I2CCOUNT;

/*
 *  ================================== PIN ==================================
 */
extern const PIN_Config BoardGpioInitTable[];
const PINCC26XX_HWAttrs PINCC26XX_hwAttrs = { .intPriority = ~0, .swiPriority = 0 };

/*
 *  ================================= Power =================================
 */
const PowerCC26XX_Config PowerCC26XX_config = {
    .policyInitFxn = NULL,
    .policyFxn = &PowerCC26XX_standbyPolicy,
    .calibrateFxn = &PowerCC26XX_calibrate,
    .enablePolicy = true,
    .calibrateRCOSC_LF = true,
    .calibrateRCOSC_HF = true
};

/*
 *  ================================= UART ==================================
 */

static UARTCC26XX_Object uartCC26XXObjects[UARTCOUNT];

static uint8_t uartCC26XXRingBuffer[UARTCOUNT][64];

const UARTCC26XX_HWAttrsV2 uartCC26XXHWAttrs[UARTCOUNT] = { {
        .baseAddr = UART0_BASE, .powerMngrId = PowerCC26XX_PERIPH_UART0, .intNum = INT_UART0_COMB,
        .intPriority = ~0, .swiPriority = 0, .txPin = UART_TX, .rxPin =
        UART_RX,
        .ctsPin = PIN_UNASSIGNED, .rtsPin = PIN_UNASSIGNED, .ringBufPtr =
                uartCC26XXRingBuffer[UART0],
        .ringBufSize = sizeof(uartCC26XXRingBuffer[UART0]), .txIntFifoThr =
                UARTCC26XX_FIFO_THRESHOLD_1_8,
        .rxIntFifoThr = UARTCC26XX_FIFO_THRESHOLD_4_8 } };

const UART_Config UART_config[UARTCOUNT] = { { .fxnTablePtr = &UARTCC26XX_fxnTable, .object =
                                                       &uartCC26XXObjects[UART0],
                                               .hwAttrs = &uartCC26XXHWAttrs[UART0] }, };

const uint_least8_t UART_count = UARTCOUNT;

/*
 *  =============================== UDMA ===============================
 */

UDMACC26XX_Object udmaObjects[UDMACOUNT];

const UDMACC26XX_HWAttrs udmaHWAttrs[UDMACOUNT] = {
    {
        .baseAddr    = UDMA0_BASE,
        .powerMngrId = PowerCC26XX_PERIPH_UDMA,
        .intNum      = INT_DMA_ERR,
        .intPriority = ~0
    }
};

const UDMACC26XX_Config UDMACC26XX_config[UDMACOUNT] = {
    {
         .object  = &udmaObjects[UDMA0],
         .hwAttrs = &udmaHWAttrs[UDMA0]
    },
};

/*
 *  =============================== Watchdog ===============================
 */
const WatchdogCC26XX_HWAttrs watchdogHWAttrs[] = {
    {
         .baseAddr = WDT_BASE, // baseAddress
         .reloadValue = 1000 // reloadTime = 1sec
    },
};

WatchdogCC26XX_Object watchdogCC26XXObjects[WATCHDOGCOUNT];

const Watchdog_Config Watchdog_config[WATCHDOGCOUNT] = {
    {
        .fxnTablePtr = &WatchdogCC26XX_fxnTable,
        .object      = &watchdogCC26XXObjects[WATCHDOG0],
        .hwAttrs     = &watchdogHWAttrs[WATCHDOG0]
    },
};

const uint_least8_t Watchdog_count = WATCHDOGCOUNT;

void board_initialize(void)
{
    uint32_t reset_source;

    /* Initialize power */
    ti_lib_driver_power_init();

    /* Get reset reason */
    reset_source = ti_lib_sys_ctrl_reset_source_get();

    /* Initialize pins */
    if (ti_lib_driver_pin_init(BoardGpioInitTable) != PIN_SUCCESS) {
        /* Error with PIN_init */
        while (1);
    }
}
