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
 *
 *  @file
 *  @brief       Header file with macros which rename TI SimpleLink SDK functions.
 *
 *               Glue file which renames TI SimpleLink SDK functions.
 *               Thus, for example, PowerCtrlIOFreezeDisable()
 *               becomes power_ctrl_io_freeze_disable()
 *
 *               This is not strictly required and a call to the former will work perfectly
 *               correctly. However, in using those macros, we make the core of the RF-Software
 *               port match the naming convention.
 *
 *               Since all functions are prefixed with ti_lib, it also becomes clear to the
 *               reader that this is a call to TI SimpleLink SDK's sources and not a call to a
 *               function inside RF-Software
 *
 *  @author      Archil Pirmisashvili <archil@rf-networks.com>
 */
#ifndef TI_LIB_H
#define TI_LIB_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef GPRAM_AS_RAM
#define ATTRIBUTE_GPRAM(x)                                  __attribute__((section(".gpram:" x)))
#else
#define ATTRIBUTE_GPRAM(x)
#endif
#define OPTNONE_ATTR                                        __attribute__((optnone))
#define NORETURN_ATTR                                       __attribute__((noreturn))
/**
 * The noinit attribute identifies a global or static variable that should not be initialized at startup or reset
 */
#define NOINIT_ATTR                                         __attribute__((noinit))
#define NOINIT_LOCATION_ATTR(X)                             __attribute__((noinit,location(X)))
/**
 * The persistent attribute identifies a global or static variable that is to be initialized at load-time, but should not be re-initialized at reset
 */
#define PERSISTENT_ATTR                                     __attribute__((persistent))
#define PERSISTENT_LOCATION_ATTR(X)                         __attribute__((persistent,location(X)))
#define ATTRIBUTE_RETAIN                                    __attribute__((retain))
#define LOCATION_ATTR(X)                                    __attribute__((location(X)))
#define ATTRIBUTE_PACKED                                    __attribute__((__packed__))
#define ATTRIBUTE_PACKED_ALIGNED(X)                         __attribute__((__packed__,aligned (X)))

/*---------------------------------------------------------------------------*/
#include <ti/devices/DeviceFamily.h>

/*---------------------------------------------------------------------------*/
/* aon_rtc.h */
#include DeviceFamily_constructPath(driverlib/aon_rtc.h)

/*---------------------------------------------------------------------------*/
/* aon_batmon.h */
#include DeviceFamily_constructPath(driverlib/aon_batmon.h)

/*---------------------------------------------------------------------------*/
/* aux_adc.h */
#include DeviceFamily_constructPath(driverlib/aux_adc.h)

/*---------------------------------------------------------------------------*/
/* cpu.h */
#include DeviceFamily_constructPath(driverlib/cpu.h)

/*---------------------------------------------------------------------------*/
/* flash.h */
#include DeviceFamily_constructPath(driverlib/flash.h)

/*---------------------------------------------------------------------------*/
/* ioc.h */
#include DeviceFamily_constructPath(driverlib/ioc.h)

/*---------------------------------------------------------------------------*/
/* interrupt.h */
#include DeviceFamily_constructPath(driverlib/cpu.h)
#include DeviceFamily_constructPath(driverlib/interrupt.h)

/*---------------------------------------------------------------------------*/
/* osc.h */
#include DeviceFamily_constructPath(driverlib/osc.h)

/*---------------------------------------------------------------------------*/
/* prcm.h */
#include DeviceFamily_constructPath(driverlib/prcm.h)

/*---------------------------------------------------------------------------*/
/* pwr_ctrl.h */
#include DeviceFamily_constructPath(driverlib/pwr_ctrl.h)

/*---------------------------------------------------------------------------*/
/* rfc.h */
#include DeviceFamily_constructPath(driverlib/rfc.h)

/*---------------------------------------------------------------------------*/
/* rf_data_entry.h */
#include DeviceFamily_constructPath(driverlib/rf_data_entry.h)

/*---------------------------------------------------------------------------*/
/* rf_prop_mailbox.h */
#include DeviceFamily_constructPath(driverlib/rf_prop_mailbox.h)

/*---------------------------------------------------------------------------*/
/* sys_ctrl.h */
#include DeviceFamily_constructPath(driverlib/sys_ctrl.h)

/*---------------------------------------------------------------------------*/
/* vims.h */
#include DeviceFamily_constructPath(driverlib/vims.h)

/*---------------------------------------------------------------------------*/
/* watchdog.h */
#include DeviceFamily_constructPath(driverlib/watchdog.h)

/*---------------------------------------------------------------------------*/
/* Serial Wire Viewer */
#if defined(Board_PIN_Serial_Wire_Viewer) && (Board_PIN_Serial_Wire_Viewer != IOID_UNUSED)
#include DeviceFamily_constructPath(inc/hw_memmap.h)
#include DeviceFamily_constructPath(inc/hw_cpu_itm.h)
#include DeviceFamily_constructPath(inc/hw_cpu_scs.h)
#include DeviceFamily_constructPath(inc/hw_cpu_tpiu.h)
#endif

/*---------------------------------------------------------------------------*/
/* drivers/ADCBuf.h */
#include <ti/drivers/ADCBuf.h>
#include <ti/drivers/adcbuf/ADCBufCC26XX.h>

/*---------------------------------------------------------------------------*/
/* drivers/ADC.h */
#include <ti/drivers/ADC.h>
#include <ti/drivers/adc/ADCCC26XX.h>

/*---------------------------------------------------------------------------*/
/* drivers/crypto/CryptoCC26XX.h */
#include <ti/drivers/crypto/CryptoCC26XX.h>

/*---------------------------------------------------------------------------*/
/* drivers/dpl/HwiP.h */
#include <ti/drivers/dpl/HwiP.h>

/*---------------------------------------------------------------------------*/
/* drivers/dpl/ClockP.h */
#include <ti/drivers/dpl/ClockP.h>

/*---------------------------------------------------------------------------*/
/* drivers/dpl/ClockP.h */
#include <ti/drivers/dpl/ClockP.h>

/*---------------------------------------------------------------------------*/
/* drivers/I2C.h */
#include <ti/drivers/I2C.h>
#include <ti/drivers/i2c/I2CCC26XX.h>

/*---------------------------------------------------------------------------*/
/* drivers/PIN.h */
#include <ti/drivers/PIN.h>
#include <ti/drivers/pin/PINCC26XX.h>

/*---------------------------------------------------------------------------*/
/* drivers/Power.h */
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>

/*---------------------------------------------------------------------------*/
/* drivers/rf/RF.h */
#include <ti/drivers/rf/RF.h>

/*---------------------------------------------------------------------------*/
/* drivers/utils/RingBuf.h */
#include <ti/drivers/utils/RingBuf.h>

/*---------------------------------------------------------------------------*/
/* drivers/UART.h */
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTCC26XX.h>

/*---------------------------------------------------------------------------*/
/* drivers/Watchdog.h */
#include <ti/drivers/Watchdog.h>
#include <ti/drivers/watchdog/WatchdogCC26XX.h>

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
/* aon_rtc.h */
#define ti_lib_aon_rtc_enable(...)                              AONRTCEnable(__VA_ARGS__)
#define ti_lib_aon_rtc_disable(...)                             AONRTCDisable(__VA_ARGS__)
#define ti_lib_aon_rtc_active(...)                              AONRTCActive(__VA_ARGS__)
#define ti_lib_aon_rtc_channel_active(...)                      AONRTCChannelActive(__VA_ARGS__)
#define ti_lib_aon_rtc_reset(...)                               AONRTCReset(__VA_ARGS__)
#define ti_lib_aon_rtc_delay_config(...)                        AONRTCDelayConfig(__VA_ARGS__)
#define ti_lib_aon_rtc_combined_event_config(...)               AONRTCCombinedEventConfig(__VA_ARGS__)
#define ti_lib_aon_rtc_event_clear(...)                         AONRTCEventClear(__VA_ARGS__)
#define ti_lib_aon_rtc_event_get(...)                           AONRTCEventGet(__VA_ARGS__)
#define ti_lib_aon_rtc_sec_get(...)                             AONRTCSecGet(__VA_ARGS__)
#define ti_lib_aon_rtc_fraction_get(...)                        AONRTCFractionGet(__VA_ARGS__)
#define ti_lib_aon_rtc_sub_sec_incr_get(...)                    AONRTCSubSecIncrGet(__VA_ARGS__)
#define ti_lib_aon_rtc_mode_ch1_set(...)                        AONRTCModeCh1Set(__VA_ARGS__)
#define ti_lib_aon_rtc_mode_ch1_get(...)                        AONRTCModeCh1Get(__VA_ARGS__)
#define ti_lib_aon_rtc_mode_ch2_set(...)                        AONRTCModeCh2Set(__VA_ARGS__)
#define ti_lib_aon_rtc_mode_ch2_get(...)                        AONRTCModeCh2Get(__VA_ARGS__)
#define ti_lib_aon_rtc_channel_enable(...)                      AONRTCChannelEnable(__VA_ARGS__)
#define ti_lib_aon_rtc_channel_disable(...)                     AONRTCChannelDisable(__VA_ARGS__)
#define ti_lib_aon_rtc_compare_value_set(...)                   AONRTCCompareValueSet(__VA_ARGS__)
#define ti_lib_aon_rtc_compare_value_get(...)                   AONRTCCompareValueGet(__VA_ARGS__)
#define ti_lib_aon_rtc_current_compare_value_get(...)           AONRTCCurrentCompareValueGet(__VA_ARGS__)
#define ti_lib_aon_rtc_current_64_bit_value_get(...)            AONRTCCurrent64BitValueGet(__VA_ARGS__)
#define ti_lib_aon_rtc_inc_value_ch2_set(...)                   AONRTCIncValueCh2Set(__VA_ARGS__)
#define ti_lib_aon_rtc_inc_value_ch2_get(...)                   AONRTCIncValueCh2Get(__VA_ARGS__)
#define ti_lib_aon_rtc_capture_value_ch1_get(...)               AONRTCCaptureValueCh1Get(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
/* aon_batmon.h */
#define ti_lib_aon_batmon_enable(...)                           AONBatMonEnable(__VA_ARGS__)
#define ti_lib_aon_batmon_disable(...)                          AONBatMonDisable(__VA_ARGS__)
#define ti_lib_aon_batmon_temperature_get_deg_c(...)            AONBatMonTemperatureGetDegC(__VA_ARGS__)
#define ti_lib_aon_batmon_battery_voltage_get(...)              AONBatMonBatteryVoltageGet(__VA_ARGS__)
#define ti_lib_aon_batmon_new_battery_measure_ready(...)        AONBatMonNewBatteryMeasureReady(__VA_ARGS__)
#define ti_lib_aon_batmon_new_temp_measure_ready(...)           AONBatMonNewTempMeasureReady(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
/* flash.h */
#define ti_lib_flash_sector_erase(...)                          FlashSectorErase(__VA_ARGS__)
#define ti_lib_flash_program(...)                               FlashProgram(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
/* ioc.h */
#define ti_lib_ioc_port_configure_set(...)                      IOCPortConfigureSet(__VA_ARGS__)
#define ti_lib_ioc_port_configure_get(...)                      IOCPortConfigureGet(__VA_ARGS__)
#define ti_lib_ioc_io_shutdown_set(...)                         IOCIOShutdownSet(__VA_ARGS__)
#define ti_lib_ioc_io_mode_set(...)                             IOCIOModeSet(__VA_ARGS__)
#define ti_lib_ioc_io_port_pull_set(...)                        IOCIOPortPullSet(__VA_ARGS__)
#define ti_lib_ioc_io_hyst_set(...)                             IOCIOHystSet(__VA_ARGS__)
#define ti_lib_ioc_io_input_set(...)                            IOCIOInputSet(__VA_ARGS__)
#define ti_lib_ioc_io_slew_ctrl_set(...)                        IOCIOSlewCtrlSet(__VA_ARGS__)
#define ti_lib_ioc_io_drv_strength_set(...)                     IOCIODrvStrengthSet(__VA_ARGS__)
#define ti_lib_ioc_io_port_id_set(...)                          IOCIOPortIdSet(__VA_ARGS__)
#define ti_lib_ioc_io_int_set(...)                              IOCIOIntSet(__VA_ARGS__)
#define ti_lib_ioc_int_register(...)                            IOCIntRegister(__VA_ARGS__)
#define ti_lib_ioc_int_unregister(...)                          IOCIntUnregister(__VA_ARGS__)
#define ti_lib_ioc_int_enable(...)                              IOCIntEnable(__VA_ARGS__)
#define ti_lib_ioc_int_disable(...)                             IOCIntDisable(__VA_ARGS__)
#define ti_lib_ioc_int_clear(...)                               IOCIntClear(__VA_ARGS__)
#define ti_lib_ioc_int_status(...)                              IOCIntStatus(__VA_ARGS__)
#define ti_lib_ioc_pin_type_gpio_input(...)                     IOCPinTypeGpioInput(__VA_ARGS__)
#define ti_lib_ioc_pin_type_gpio_output(...)                    IOCPinTypeGpioOutput(__VA_ARGS__)
#define ti_lib_ioc_pin_type_uart(...)                           IOCPinTypeUart(__VA_ARGS__)
#define ti_lib_ioc_pin_type_ssi_master(...)                     IOCPinTypeSsiMaster(__VA_ARGS__)
#define ti_lib_ioc_pin_type_ssi_slave(...)                      IOCPinTypeSsiSlave(__VA_ARGS__)
#define ti_lib_ioc_pin_type_i2c(...)                            IOCPinTypeI2c(__VA_ARGS__)
#define ti_lib_ioc_pin_type_aux(...)                            IOCPinTypeAux(__VA_ARGS__)
#define ti_lib_ioc_pin_type_spis(...)                           IOCPinTypeSpis(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
/* interrupt.h */
#define ti_lib_int_master_enable(...)                           IntMasterEnable(__VA_ARGS__)
#define ti_lib_int_master_disable(...)                          IntMasterDisable(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
/* osc.h */
#define ti_lib_osc_xhf_power_mode_set(...)                      OSCXHfPowerModeSet(__VA_ARGS__)
#define ti_lib_osc_clock_loss_event_enable(...)                 OSCClockLossEventEnable(__VA_ARGS__)
#define ti_lib_osc_clock_loss_event_disable(...)                OSCClockLossEventDisable(__VA_ARGS__)
#define ti_lib_osc_clock_source_set(...)                        OSCClockSourceSet(__VA_ARGS__)
#define ti_lib_osc_clock_source_get(...)                        OSCClockSourceGet(__VA_ARGS__)
#define ti_lib_osc_hf_source_ready(...)                         OSCHfSourceReady(__VA_ARGS__)
#define ti_lib_osc_hf_source_switch(...)                        OSCHfSourceSwitch(__VA_ARGS__)
#define ti_lib_osc_hf_get_startup_time(...)                     OSCHF_GetStartupTime(__VA_ARGS__)
#define ti_lib_osc_hf_turn_on_xosc(...)                         OSCHF_TurnOnXosc(__VA_ARGS__)
#define ti_lib_osc_hf_attempt_to_switch_to_xosc(...)            OSCHF_AttemptToSwitchToXosc(__VA_ARGS__)
#define ti_lib_osc_hf_debug_get_crystal_amplitude(...)          OSCHF_DebugGetCrystalAmplitude(__VA_ARGS__)
#define ti_lib_osc_hf_debug_get_expected_average_crystal_amplitude(...) \
                                                                OSCHF_DebugGetExpectedAverageCrystalAmplitude(__VA_ARGS__)
#define ti_lib_osc_hposc_relative_frequency_offset_get(...) \
                                                                OSC_HPOSCRelativeFrequencyOffsetGet(__VA_ARGS__)
#define ti_lib_osc_hposc_relative_frequency_offset_to_rf_core_format_convert(...) \
                                                                OSC_HPOSCRelativeFrequencyOffsetToRFCoreFormatConvert(__VA_ARGS__)
#define ti_lib_osc_hf_switch_to_rc_osc_turn_off_xosc(...)       OSCHF_SwitchToRcOscTurnOffXosc(__VA_ARGS__)

#define ti_lib_osc_adjust_xosc_hf_cap_array(...)                OSC_AdjustXoscHfCapArray(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
/* prcm.h */
#define ti_lib_prcm_inf_clock_configure_set(...)                PRCMInfClockConfigureSet(__VA_ARGS__)
#define ti_lib_prcm_inf_clock_configure_get(...)                PRCMInfClockConfigureGet(__VA_ARGS__)
#define ti_lib_prcm_mcu_power_off(...)                          PRCMMcuPowerOff(__VA_ARGS__)
#define ti_lib_prcm_mcu_power_off_cancel(...)                   PRCMMcuPowerOffCancel(__VA_ARGS__)
#define ti_lib_prcm_mcu_uldo_configure(...)                     PRCMMcuUldoConfigure(__VA_ARGS__)
#define ti_lib_prcm_audio_clock_enable(...)                     PRCMAudioClockEnable(__VA_ARGS__)
#define ti_lib_prcm_audio_clock_disable(...)                    PRCMAudioClockDisable(__VA_ARGS__)
#define ti_lib_prcm_audio_clock_config_set(...)                 PRCMAudioClockConfigSet(__VA_ARGS__)
#define ti_lib_prcm_load_set(...)                               PRCMLoadSet(__VA_ARGS__)
#define ti_lib_prcm_load_get(...)                               PRCMLoadGet(__VA_ARGS__)
#define ti_lib_prcm_domain_enable(...)                          PRCMDomainEnable(__VA_ARGS__)
#define ti_lib_prcm_domain_disable(...)                         PRCMDomainDisable(__VA_ARGS__)
#define ti_lib_prcm_power_domain_on(...)                        PRCMPowerDomainOn(__VA_ARGS__)
#define ti_lib_prcm_power_domain_off(...)                       PRCMPowerDomainOff(__VA_ARGS__)
#define ti_lib_prcm_rf_power_down_when_idle(...)                PRCMRfPowerDownWhenIdle(__VA_ARGS__)
#define ti_lib_prcm_peripheral_run_enable(...)                  PRCMPeripheralRunEnable(__VA_ARGS__)
#define ti_lib_prcm_peripheral_run_disable(...)                 PRCMPeripheralRunDisable(__VA_ARGS__)
#define ti_lib_prcm_peripheral_sleep_enable(...)                PRCMPeripheralSleepEnable(__VA_ARGS__)
#define ti_lib_prcm_peripheral_sleep_disable(...)               PRCMPeripheralSleepDisable(__VA_ARGS__)
#define ti_lib_prcm_peripheral_deep_sleep_enable(...)           PRCMPeripheralDeepSleepEnable(__VA_ARGS__)
#define ti_lib_prcm_peripheral_deep_sleep_disable(...)          PRCMPeripheralDeepSleepDisable(__VA_ARGS__)
#define ti_lib_prcm_power_domain_status(...)                    PRCMPowerDomainStatus(__VA_ARGS__)
#define ti_lib_prcm_rf_ready(...)                               PRCMRfReady(__VA_ARGS__)
#define ti_lib_prcm_sleep(...)                                  PRCMSleep(__VA_ARGS__)
#define ti_lib_prcm_deep_sleep(...)                             PRCMDeepSleep(__VA_ARGS__)
#define ti_lib_prcm_cache_retention_enable(...)                 PRCMCacheRetentionEnable(__VA_ARGS__)
#define ti_lib_prcm_cache_retention_disable(...)                PRCMCacheRetentionDisable(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
/* pwr_ctrl.h */
#define ti_lib_pwr_ctrl_state_set(...)                          PowerCtrlStateSet(__VA_ARGS__)
#define ti_lib_pwr_ctrl_source_set(...)                         PowerCtrlSourceSet(__VA_ARGS__)
#define ti_lib_pwr_ctrl_source_get(...)                         PowerCtrlSourceGet(__VA_ARGS__)
#define ti_lib_pwr_ctrl_reset_source_get(...)                   PowerCtrlResetSourceGet(__VA_ARGS__)
#define ti_lib_pwr_ctrl_reset_source_clear(...)                 PowerCtrlResetSourceClear(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
/* rfc.h */
#define ti_lib_rfc_rtrim(...)                                   RFCRTrim(__VA_ARGS__)
#define ti_lib_rfc_adi3vco_ldo_voltage_mode(...)                RFCAdi3VcoLdoVoltageMode(__VA_ARGS__)
#define ti_lib_rfc_hw_int_enable(...)                           RFCHwIntEnable(__VA_ARGS__)
#define ti_lib_rfc_hw_int_disable(...)                          RFCHwIntDisable(__VA_ARGS__)
#define ti_lib_rfc_hw_int_clear(...)                            RFCHwIntClear(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
/* sys_ctrl.h */
#define ti_lib_sys_ctrl_power_everything(...)                   SysCtrlPowerEverything(__VA_ARGS__)
#define ti_lib_sys_ctrl_powerdown(...)                          SysCtrlPowerdown(__VA_ARGS__)
#define ti_lib_sys_ctrl_standby(...)                            SysCtrlStandby(__VA_ARGS__)
#define ti_lib_sys_ctrl_shutdown(...)                           SysCtrlShutdown(__VA_ARGS__)
#define ti_lib_sys_ctrl_clock_get(...)                          SysCtrlClockGet(__VA_ARGS__)
#define ti_lib_sys_ctrl_aon_sync(...)                           SysCtrlAonSync(__VA_ARGS__)
#define ti_lib_sys_ctrl_aon_update(...)                         SysCtrlAonUpdate(__VA_ARGS__)
#define ti_lib_sys_ctrl_set_recharge_before_power_down(...)     SysCtrlSetRechargeBeforePowerDown(__VA_ARGS__)
#define ti_lib_sys_ctrl_adjust_recharge_after_power_down()      SysCtrlAdjustRechargeAfterPowerDown()
#define ti_lib_sys_ctrl_dcdc_voltage_conditional_control(...)   SysCtrl_DCDC_VoltageConditionalControl(__VA_ARGS__)
#define ti_lib_sys_ctrl_reset_source_get(...)                   SysCtrlResetSourceGet(__VA_ARGS__)
#define ti_lib_sys_ctrl_system_reset(...)                       SysCtrlSystemReset(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
/* vims.h */
#define ti_lib_vims_configure(...)                              VIMSConfigure(__VA_ARGS__)
#define ti_lib_vims_mode_set(...)                               VIMSModeSet(__VA_ARGS__)
#define ti_lib_vims_mode_get(...)                               VIMSModeGet(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
/* drivers/ADCBuf.h */
#define ti_lib_driver_adc_buf_init(...)                         ADCBuf_init(__VA_ARGS__)
#define ti_lib_driver_adc_buf_params_init(...)                  ADCBuf_Params_init(__VA_ARGS__)
#define ti_lib_driver_adc_buf_open(...)                         ADCBuf_open(__VA_ARGS__)
#define ti_lib_driver_adc_buf_close(...)                        ADCBuf_close(__VA_ARGS__)
#define ti_lib_driver_adc_buf_adjust_raw_values(...)            ADCBuf_adjustRawValues(__VA_ARGS__)
#define ti_lib_driver_adc_buf_convert(...)                      ADCBuf_convert(__VA_ARGS__)
#define ti_lib_driver_adc_buf_convert_to_microvolts(...)        ADCBuf_convertAdjustedToMicroVolts(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
/* drivers/ADC.h */
#define ti_lib_driver_adc_init(...)                             ADC_init(__VA_ARGS__)
#define ti_lib_driver_adc_params_init(...)                      ADC_Params_init(__VA_ARGS__)
#define ti_lib_driver_adc_open(...)                             ADC_open(__VA_ARGS__)
#define ti_lib_driver_adc_convert(...)                          ADC_convert(__VA_ARGS__)
#define ti_lib_driver_adc_convert_to_micro_volts(...)           ADC_convertRawToMicroVolts(__VA_ARGS__)
#define ti_lib_driver_adc_close(...)                            ADC_close(__VA_ARGS__)
#define ti_lib_aux_adc_value_to_micro_volts(...)                AUXADCValueToMicrovolts(__VA_ARGS__)
#define ti_lib_aux_adc_get_adjustment_gain(...)                 AUXADCGetAdjustmentGain(__VA_ARGS__)
#define ti_lib_aux_adc_get_adjustment_offset(...)               AUXADCGetAdjustmentOffset(__VA_ARGS__)
#define ti_lib_aux_adc_adjust_value_for_gain_and_offset(...)    AUXADCAdjustValueForGainAndOffset(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
/* drivers/crypto/CryptoCC26XX.h */
#define ti_lib_driver_crypto_init(...)                          CryptoCC26XX_init(__VA_ARGS__)
#define ti_lib_driver_crypto_open(...)                          CryptoCC26XX_open(__VA_ARGS__)
#define ti_lib_driver_crypto_params_init(...)                   CryptoCC26XX_Params_init(__VA_ARGS__)
#define ti_lib_driver_crypto_close(...)                         CryptoCC26XX_close(__VA_ARGS__)
#define ti_lib_driver_crypto_allocate_key(...)                  CryptoCC26XX_allocateKey(__VA_ARGS__)
#define ti_lib_driver_crypto_release_key(...)                   CryptoCC26XX_releaseKey(__VA_ARGS__)
#define ti_lib_driver_crypto_transac_init(...)                  CryptoCC26XX_Transac_init(__VA_ARGS__)
#define ti_lib_driver_crypto_transact(...)                      CryptoCC26XX_transact(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
/* drivers/dpl/HwiP.h */
#define ti_lib_driver_hwip_disable(...)                         HwiP_disable(__VA_ARGS__)
#define ti_lib_driver_hwip_restore(...)                         HwiP_restore(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
/* drivers/dpl/ClockP.h */
#define ti_lib_driver_clockp_startup(...)                       ClockP_startup(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
/* drivers/I2C.h */
#define ti_lib_driver_i2c_init(...)                             I2C_init(__VA_ARGS__)
#define ti_lib_driver_i2c_params_init(...)                      I2C_Params_init(__VA_ARGS__)
#define ti_lib_driver_i2c_open(...)                             I2C_open(__VA_ARGS__)
#define ti_lib_driver_i2c_close(...)                            I2C_close(__VA_ARGS__)
#define ti_lib_driver_i2c_transfer(...)                         I2C_transfer(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
/* drivers/dpl/TimerP.h */
#define ti_lib_driver_timerp_get_count64(...)                   TimerP_getCount64(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
/* drivers/dpl/SemaphoreP.h */
#define ti_lib_driver_semaphorep_constuct_binary(...)           SemaphoreP_constructBinary(__VA_ARGS__)
#define ti_lib_driver_semaphorep_destruct(...)                  SemaphoreP_destruct(__VA_ARGS__)
#define ti_lib_driver_semaphorep_pend(...)                      SemaphoreP_pend(__VA_ARGS__)
#define ti_lib_driver_semaphorep_post(...)                      SemaphoreP_post(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
/* drivers/PIN.h */
#define ti_lib_driver_pin_init(...)                             PIN_init(__VA_ARGS__)
#define ti_lib_driver_pin_open(...)                             PIN_open(__VA_ARGS__)
#define ti_lib_driver_pin_close(...)                            PIN_close(__VA_ARGS__)
#define ti_lib_driver_pin_set_output_value(...)                 PIN_setOutputValue(__VA_ARGS__)
#define ti_lib_driver_pin_get_output_value(...)                 PIN_getOutputValue(__VA_ARGS__)
#define ti_lib_driver_pin_set_output_enable(...)                PIN_setOutputEnable(__VA_ARGS__)
#define ti_lib_driver_pin_get_input_value(...)                  PIN_getInputValue(__VA_ARGS__)
#define ti_lib_driver_pin_get_config(...)                       PIN_getConfig(__VA_ARGS__)
#define ti_lib_driver_pin_set_config(...)                       PIN_setConfig(__VA_ARGS__)
#define ti_lib_driver_pin_set_mux(...)                          PINCC26XX_setMux(__VA_ARGS__)
#define ti_lib_driver_pin_set_wakeup(...)                       PINCC26XX_setWakeup(__VA_ARGS__)
#define ti_lib_driver_pin_register_int_cb(...)                  PIN_registerIntCb(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
/* drivers/Power.h */

#define ti_lib_driver_power_init(...)                           Power_init(__VA_ARGS__)
#define ti_lib_driver_power_idle(...)                           Power_idleFunc(__VA_ARGS__)
#define ti_lib_driver_power_inject_calibration(...)             PowerCC26XX_injectCalibration(__VA_ARGS__)
#define ti_lib_driver_power_shutdown(...)                       Power_shutdown(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
/* drivers/rf/RF.h */
#define ti_lib_driver_rf_get_current_time(...)                  RF_getCurrentTime(__VA_ARGS__)
#define ti_lib_driver_rf_params_init(...)                       RF_Params_init(__VA_ARGS__)
#define ti_lib_driver_rf_open(...)                              RF_open(__VA_ARGS__)
#define ti_lib_driver_rf_close(...)                             RF_close(__VA_ARGS__)
#define ti_lib_driver_rf_yield(...)                             RF_yield(__VA_ARGS__)
#define ti_lib_driver_rf_flush_cmd(...)                         RF_flushCmd(__VA_ARGS__)
#define ti_lib_driver_rf_cancel_cmd(...)                        RF_cancelCmd(__VA_ARGS__)
#define ti_lib_driver_rf_run_cmd(...)                           RF_runCmd(__VA_ARGS__)
#define ti_lib_driver_rf_post_cmd(...)                          RF_postCmd(__VA_ARGS__)
#define ti_lib_driver_rf_pend_cmd(...)                          RF_pendCmd(__VA_ARGS__)
#define ti_lib_driver_rf_get_rssi(...)                          RF_getRssi(__VA_ARGS__)
#define ti_lib_driver_rf_set_tx_power(...)                      RF_setTxPower(__VA_ARGS__)
#define ti_lib_driver_rf_tx_power_table_find_value(...)         RF_TxPowerTable_findValue(__VA_ARGS__)
#define ti_lib_driver_rf_rat_config_compare_init(...)           RF_RatConfigCompare_init(__VA_ARGS__)
#define ti_lib_driver_rf_rat_compare(...)                       RF_ratCompare(__VA_ARGS__)
#define ti_lib_driver_rf_rat_disable_channel(...)               RF_ratDisableChannel(__VA_ARGS__)
#define ti_lib_driver_rf_convert_ms_to_rat_ticks(...)           RF_convertMsToRatTicks(__VA_ARGS__)
#define ti_lib_driver_rf_convert_us_to_rat_ticks(...)           RF_convertUsToRatTicks(__VA_ARGS__)
#define ti_lib_driver_rf_convert_rat_ticks_to_us(...)           RF_convertRatTicksToUs(__VA_ARGS__)
#define ti_lib_driver_rf_convert_rat_ticks_to_ms(...)           RF_convertRatTicksToMs(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
/* drivers/utils/RingBuf.h */
#define ti_lib_driver_ring_buf_construct(...)                   RingBuf_construct(__VA_ARGS__)
#define ti_lib_driver_ring_buf_put(...)                         RingBuf_put(__VA_ARGS__)
#define ti_lib_driver_ring_buf_put_arr(...)                     RingBuf_put_arr(__VA_ARGS__)
#define ti_lib_driver_ring_buf_get(...)                         RingBuf_get(__VA_ARGS__)
#define ti_lib_driver_ring_buf_get_arr(...)                     RingBuf_get_arr(__VA_ARGS__)
#define ti_lib_driver_ring_buf_get_count(...)                   RingBuf_getCount(__VA_ARGS__)
#define ti_lib_driver_ring_buf_is_full(...)                     RingBuf_isFull(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
/* drivers/UART.h */
#define ti_lib_driver_uart_init(...)                            UART_init(__VA_ARGS__)
#define ti_lib_driver_uart_params_init(...)                     UART_Params_init(__VA_ARGS__)
#define ti_lib_driver_uart_open(...)                            UART_open(__VA_ARGS__)
#define ti_lib_driver_uart_read(...)                            UART_read(__VA_ARGS__)
#define ti_lib_driver_uart_write(...)                           UART_write(__VA_ARGS__)
#define ti_lib_driver_uart_close(...)                           UART_close(__VA_ARGS__)
#define ti_lib_driver_uart_control(...)                         UART_control(__VA_ARGS__)
#define ti_lib_driver_uart_cancel(...)                          UART_readCancel(__VA_ARGS__)

/*---------------------------------------------------------------------------*/
/* drivers/Watchdog.h */

#define ti_lib_driver_watchdog_params_init(...)                 Watchdog_Params_init(__VA_ARGS__)
#define ti_lib_driver_watchdog_init(...)                        Watchdog_init(__VA_ARGS__)
#define ti_lib_driver_watchdog_open(...)                        Watchdog_open(__VA_ARGS__)
#define ti_lib_driver_watchdog_clear(...)                       Watchdog_clear(__VA_ARGS__)
#define ti_lib_driver_watchdog_close(...)                       Watchdog_close(__VA_ARGS__)
#define ti_lib_driver_watchdog_set_reload(...)                  Watchdog_setReload(__VA_ARGS__)
#define ti_lib_driver_watchdog_convert_ms_to_ticks(...)         Watchdog_convertMsToTicks(__VA_ARGS__)

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* TI_LIB_H */
